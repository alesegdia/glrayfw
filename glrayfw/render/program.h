
/* El diseño bueno seria hacerlo todo en el context, es decir,
 * llevar la cuenta de los shaders cargados y si nos piden uno
 * mirar en la cache, así tb podemos evitar el gl aqui
 */
#pragma once
#include "context.h"
#include "shader.h"
class Program
{

private:

	GLuint handle;
	Shader frag;
	Shader vert;

	DynamicArray<GLuint> locations;

public:

	void Prepare( Render::Context* gl, const char* vs_path, const char* fs_path )
	{
		vert.LoadFromFile( gl, vs_path, Shader::VERTEX );
		frag.LoadFromFile( gl, fs_path, Shader::FRAGMENT );
		//Compile( gl );
		handle = gl->CreateProgram();

	}

	void BindAttribLocation( Render::Context* gl, GLuint loc, const char* shadervar )
	{
		gl->BindAttribLocation( handle, loc, shadervar );
		locations.Add( loc );
	}

	void Dispose( Render::Context* gl )
	{
		gl->UseProgram(0);
		for( int i = 0; i < locations.Size(); i++ ) gl->DisableVertexAttribArray( locations[i] );
		gl->DetachShader( handle, frag.Object() );
		gl->DetachShader( handle, vert.Object() );
		gl->DeleteShader( frag.Object() );
		gl->DeleteShader( vert.Object() );
		gl->DeleteProgram( handle );
		// gl->DeleteShader( geom.Object() );
	}

	void SetShaders( Shader vert, Shader frag )
	{
		this->frag = frag;
		this->vert = vert;
	}

	void Compile( Render::Context* gl )
	{
		gl->AttachShader( handle, vert.Object() );
		gl->AttachShader( handle, frag.Object() );
		gl->LinkProgram( handle );
	}

	GLuint Object()
	{
		return handle;
	}


};

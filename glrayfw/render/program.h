
/* El diseño bueno seria hacerlo todo en el context, es decir,
 * llevar la cuenta de los shaders cargados y si nos piden uno
 * mirar en la cache, así tb podemos evitar el gl aqui
 */
#pragma once
#include "context.h"
#include "shader.h"
#include "../core/dynamicarray.h"
#include <unordered_map>
#include <string>

class Program
{

private:

	GLuint handle;
	Shader frag;
	Shader vert;
	bool m_compiled = false;

	DynamicArray<GLuint> locations;
	std::unordered_map<std::string, GLint> attribs;
	std::unordered_map<std::string, GLint> uniforms;

	Render::Context* m_gl = nullptr;

public:

	void Prepare( Render::Context* gl, const char* vs_path, const char* fs_path )
	{
		m_gl = gl;
		vert.LoadFromFile( gl, vs_path, Shader::VERTEX );
		frag.LoadFromFile( gl, fs_path, Shader::FRAGMENT );
		//Compile( gl );
		handle = gl->CreateProgram();

	}

	void AddUniform(const std::string& uniformName)
	{
		assert(!m_compiled);
		uniforms[uniformName] = -1;
	}

	void AddAttrib(const std::string& attribName)
	{
		assert(!m_compiled);
		attribs[attribName] = -1;
	}

	void SetUniformMatrix4fv(const std::string& uniformName, const GLfloat* data)
	{
		assert(m_gl != nullptr);
		assert(uniforms.count(uniformName) == 1);
		m_gl->UniformMatrix4fv(uniforms[uniformName], 1, false, data);
	}

	void BindAttribLocation( Render::Context* gl, GLuint loc, const char* shadervar )
	{
		gl->BindAttribLocation( handle, loc, shadervar );
		locations.Add( loc );
	}

	void Dispose( Render::Context* gl )
	{
		gl->UseProgram(0);
		for( size_t i = 0; i < locations.Size(); i++ ) gl->DisableVertexAttribArray( locations[i] );
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
		m_compiled = true;
		gl->AttachShader( handle, vert.Object() );
		gl->AttachShader( handle, frag.Object() );
		gl->LinkProgram( handle );

		for (auto& entry : uniforms)
		{
			entry.second = m_gl->GetUniformLocation(Object(), entry.first.c_str());
		}
	}

	GLuint Object()
	{
		return handle;
	}


};

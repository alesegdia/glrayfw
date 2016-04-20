
#pragma once

#include "glcorearb.h"
#include "context.h"
#include <cml/cml.h>

class Mesh
{

public:

	enum class AttribConfig
	{
		XYZ_UV_RST,
		XYZUV_INST,
		XY_UV
	};


protected:

	GLfloat* vertices;
	GLfloat* normals;
	GLfloat* uvs;
	GLuint vao;
	GLsizei numvbo;
	GLuint* vbo;
	size_t numElements;

	AttribConfig attrconf;


public:

	Mesh( Mesh::AttribConfig conf )
	{
		attrconf = conf;
		vertices = normals = uvs = NULL;
		vbo = NULL;
	}

	virtual ~Mesh()
	{
		if( vertices ) delete [] vertices;
		if( normals ) delete [] normals;
		if( uvs ) delete [] uvs;
		if( vbo ) delete [] vbo;
	}

	GLuint GetVAO()
	{
		return vao;
	}

	GLuint GetVBO( int index )
	{
		return vbo[index];
	}

	size_t NumElements()
	{
		return numElements;
	}

	void Prepare( Render::Context* gl )
	{
		switch( attrconf )
		{
		case AttribConfig::XYZUV_INST: ConfigXYZUV_INST( gl ); break;
		case AttribConfig::XY_UV: ConfigXY_UV( gl ); break;
		default: break;
		}
	}

	void ConfigXY_UV( Render::Context* gl )
	{
		GLuint pos_loc = 0;
		GLuint tex_loc = 1;

		gl->GenVertexArrays( 1, &vao );
		gl->BindVertexArray( vao );

		gl->GenBuffers( numvbo, vbo );

		gl->BindBuffer( GL_ARRAY_BUFFER, vbo[0] );
		UploadBufferData( gl, vbo[0], 2, &vertices[0] );
		ConfigAttrib( gl, pos_loc, 2 );

	 	gl->BindBuffer( GL_ARRAY_BUFFER, 0 );

		gl->BindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		UploadBufferData( gl, vbo[1], 2, &uvs[0] );
	 	ConfigAttrib( gl, tex_loc, 2 );

	 	gl->BindBuffer( GL_ARRAY_BUFFER, 0 );

		//ConfigAttrib4fv( gl, model_loc, 1 );

		gl->BindVertexArray(0);
	}

	void ConfigXYZUV_INST( Render::Context* gl )
	{
		GLuint pos_loc = 0;
		GLuint tex_loc = 1;
		GLuint model_loc = 2;

		gl->GenVertexArrays( 1, &vao );
		gl->BindVertexArray( vao );

		gl->GenBuffers( numvbo, vbo );

		gl->BindBuffer( GL_ARRAY_BUFFER, vbo[0] );
		UploadBufferData( gl, vbo[0], 5, &vertices[0] );

		ConfigAttrib( gl, pos_loc, 3, 5 );
		ConfigAttrib( gl, tex_loc, 2, 5, 3 );

	 	gl->BindBuffer( GL_ARRAY_BUFFER, 0 );

		gl->BindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		ConfigAttrib4fv( gl, model_loc, 1 );

	 	gl->BindBuffer( GL_ARRAY_BUFFER, 0 );

		gl->BindVertexArray(0);
	}

	void Dispose( Render::Context* gl )
	{
		gl->DeleteBuffers( numvbo, vbo );
		gl->DeleteVertexArrays( 1, &vao );
	}

	void ConfigAttrib( Render::Context* gl, GLuint location, size_t num_floats, size_t stride = 0, size_t offset = 0, int attrdivisor = 0 /* 0 => NO INSTANCING */ )
	{
		gl->EnableVertexAttribArray( location );
		gl->VertexAttribPointer( location, num_floats, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (const GLvoid*)(offset * sizeof(GLfloat)) );
		if( attrdivisor ) gl->VertexAttribDivisor( location, attrdivisor );
	}

	void ConfigAttrib4fv( Render::Context* gl, GLuint location, int attrdivisor = 0 )
	{
		for( int i = 0; i < 4; i++ )
		{
			gl->EnableVertexAttribArray( location + i );
			gl->VertexAttribPointer( location + i,
									4, GL_FLOAT, GL_FALSE,
									sizeof(cml::matrix44f_c),
									(void*) (sizeof(cml::vector4f) * i) );
			if( attrdivisor ) gl->VertexAttribDivisor( location + i, 1 );
		}
	}

	void UploadBufferData( Render::Context* gl, GLuint vbo, int items_per_attr, GLfloat* buffer )
	{
		gl->BufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * items_per_attr * numElements, buffer, GL_STATIC_DRAW );
	}


protected:

	template <typename T>
	void LoadBuffer( T* target, const T* source, int items_per_element )
	{
		int N = numElements * items_per_element;

		for( int i = 0; i < N; i++ )
		{
			target[i] = source[i];
		}
	}

	void Alloc( int num )
	{
		numElements = num;
		switch( attrconf )
		{
		case AttribConfig::XYZUV_INST:
			vertices = new GLfloat[num * 5];
			numvbo = 2;
			vbo = new GLuint[numvbo];
			break;
		case AttribConfig::XY_UV:
			vertices = new GLfloat[num * 2];
			numvbo = 2;
			uvs = new GLfloat[num * 2];
			vbo = new GLuint[numvbo];
			break;
		default:
			break;
		}
	}

};

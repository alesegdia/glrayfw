
#pragma once

#include "../render/sdlcontext.h"

#include <cml/cml.h>
#include "context.h"
#include "Texture.h"
#include "quad.h"
#include "block.h"
#include "sprite3d.h"
#include "program.h"
#include "../entity/entity.h"
#include "plane.h"
#include "../core/random.h"
#include "../entity/pawn.h"
#include "../map/map.h"
#include "camera.h"
#include "font.h"


class Renderer
{
private:

	const float PLAYER_VISION_RANGE = 17.f;

	float sprite3d_fog_range;
	float cube_fog_range;

	Render::Context* gl;
	Program quadprog, blockprog, planeprog, postprog, fontprog;

	// usado para posteffects
	GLuint frameBuffer, texColorBuffer, rboDepthStencil, postvao, postvbo;
	GLfloat array[256];
	RNG rng;

	Block block;

	GLuint attribute_coord, uniform_tex, uniform_color;
	GLuint fontvao, fontvbo;
	int ww, wh;
	Camera* cam;
	Plane plane;
	Font default_font;

public:

	void Prepare( Render::Context* gl, Camera* cam, int winWidth, int winHeight )
	{
		this->gl = gl;
		this->ww = winWidth;
		this->wh = winHeight;
		this->cam = cam;

		fontprog.Prepare( gl, "assets/vs_font.vert", "assets/fs_font.frag" );
		fontprog.Compile(gl);
		attribute_coord = gl->GetAttribLocation( fontprog.Object(), "coord" );
		uniform_tex = gl->GetUniformLocation( fontprog.Object(), "tex" );
		uniform_color = gl->GetUniformLocation( fontprog.Object(), "color" );
		Font::Initialize();
		default_font.Prepare( gl, "assets/mine.ttf", 96 );

		gl->GenVertexArrays(1, &fontvao);
		gl->GenBuffers(1, &fontvbo);

		blockprog.Prepare( gl, "assets/vs_mvptex_inst.vert", "assets/fs_mvptex_inst.frag" );
		quadprog.Prepare( gl, "assets/vs_quadanim.vert", "assets/fs_quadanim.frag" );
		planeprog.Prepare( gl, "assets/vs_plane.vert", "assets/fs_plane.frag" );
		postprog.Prepare( gl, "assets/vs_post.vert", "assets/fs_post.frag" );

		GLuint pos_loc = 0;
		GLuint tex_loc = 1;
		GLuint model_loc = 2;

		blockprog.BindAttribLocation( gl, tex_loc, "in_TexCoord" );
		blockprog.BindAttribLocation( gl, pos_loc, "in_Position" );
		blockprog.BindAttribLocation( gl, model_loc, "in_ModelMatrix" );

		quadprog.BindAttribLocation( gl, tex_loc, "in_TexCoord" );
		quadprog.BindAttribLocation( gl, pos_loc, "in_Position" );

		blockprog.Compile(gl);
		quadprog.Compile(gl);
		planeprog.Compile(gl);
		postprog.Compile(gl);
		block.Prepare( gl );

		gl->Enable(GL_CULL_FACE);
		gl->Enable(GL_DEPTH_TEST);
		gl->DepthFunc(GL_LEQUAL);
		gl->CullFace( GL_FRONT );
		gl->FrontFace( GL_CW );
		gl->Enable (GL_BLEND);
		gl->BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//gl->BlendFunc (GL_ONE, GL_ONE);
		//

		gl->GenFramebuffers(1, &frameBuffer);
		gl->BindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		gl->GenTextures(1, &texColorBuffer);
		gl->BindTexture(GL_TEXTURE_2D, texColorBuffer);

		gl->TexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
		);

		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		gl->FramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
		);

		gl->GenRenderbuffers(1, &rboDepthStencil);
		gl->BindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
		gl->RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);


		gl->FramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
		);

		gl->BindFramebuffer( GL_FRAMEBUFFER, 0);

		AllocPostQuad();

		plane.Prepare(gl,300,300,4,4);

	}

	void RenderText( const char* text, float x, float y, cml::vector4f color = cml::vectorf(1.f,0.f,0.f,1.f), float sx_ = 1.f, float sy_ = 1.f )
	{
		float sx = sx_ * (2.0 / ww);
		float sy = sy_ * (2.0 / wh);
		gl->UseProgram( fontprog.Object() );
		gl->BindTexture( GL_TEXTURE_2D, default_font.Texture() );
		gl->Uniform1i( uniform_tex, 0 );
		gl->Uniform4f( uniform_color, color[0], color[1], color[2], color[3] );

		gl->BindVertexArray( fontvao );
		gl->BindBuffer( GL_ARRAY_BUFFER, fontvbo );
		gl->EnableVertexAttribArray( attribute_coord );
		gl->VertexAttribPointer( attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0 );


		int nvert = default_font.UpdateGPUQuads( gl, text, x, y, sx, sy );
		gl->DrawArrays(GL_TRIANGLES, 0, nvert);
		gl->DisableVertexAttribArray(attribute_coord);
	}

	void BindPostFBO()
	{
		gl->BindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
	}
	void AllocPostQuad()
	{
		/*
		GLfloat quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};
		*/

		GLfloat postdata[] = {
			-1.f, -1.f, 	0.f, 0.f,
			1.f, -1.f, 		1.f, 0.f,
			1.f, 1.f, 		1.f, 1.f,
			-1.f, 1.f, 		0.f, 1.f
		};
		gl->GenVertexArrays(1,&postvao);
		gl->BindVertexArray(postvao);
		gl->GenBuffers(1,&postvbo);
		gl->BindBuffer(GL_ARRAY_BUFFER,postvbo);
		gl->BufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, postdata, GL_STATIC_DRAW );
		GLuint xy_loc = gl->GetAttribLocation(postprog.Object(),"position");
		gl->EnableVertexAttribArray( xy_loc );
		gl->VertexAttribPointer( xy_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0 );
		GLuint uv_loc = gl->GetAttribLocation(postprog.Object(),"texcoord");
		gl->EnableVertexAttribArray( uv_loc );
		gl->VertexAttribPointer( uv_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)) );
		gl->BindBuffer( GL_ARRAY_BUFFER, 0 );
		gl->BindVertexArray(0);
	}

	void RenderPostQuad()
	{
		gl->BindVertexArray(0);
	}

	void UseDefaultFBO() { gl->BindFramebuffer( GL_FRAMEBUFFER, 0 ); }
	void UseCreatedFBO() { gl->BindFramebuffer( GL_FRAMEBUFFER, frameBuffer ); }
	void RenderClear()
	{
		gl->ClearColor(0.0, 0.0, 0.0, 1.0);
		gl->Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	inline void BindVP( GLuint shaderprogram )
	{
		gl->UniformMatrix4fv( gl->GetUniformLocation( shaderprogram, "view" ), 1, false, this->cam->view().data() );
		gl->UniformMatrix4fv( gl->GetUniformLocation( shaderprogram, "proj" ), 1, false, this->cam->projection().data() );
	}

	inline void BindVisionRange( GLuint shaderprogram )
	{
		gl->Uniform1f( gl->GetUniformLocation( shaderprogram, "range" ), PLAYER_VISION_RANGE );
	}

	void RenderMap( Map& map, tdogl::Texture* tex1, tdogl::Texture* tex2, tdogl::Texture* tex3 )
	{
		GLuint shaderprogram = blockprog.Object();
		gl->UseProgram( shaderprogram );
		BindVP( shaderprogram );
		BindVisionRange( shaderprogram );

		GLuint VAO = block.GetVAO();
		gl->BindVertexArray(VAO);
			RenderBlocks( map, tex1, 1 );
			RenderBlocks( map, tex2, 2 );
			RenderBlocks( map, tex3, 3 );
		gl->BindVertexArray(0);
	}

	void BatchSprite3D()
	{
		GLuint shaderprogram = quadprog.Object();
		gl->UseProgram( shaderprogram );
		BindVP( shaderprogram );
		BindVisionRange( shaderprogram );
	}


	void RenderPlane( Plane* p, const cml::matrix44f_c& model, tdogl::Texture* tex )
	{
		GLuint shaderprogram = planeprog.Object();
		gl->UseProgram( shaderprogram );
		BindVP( shaderprogram );
		BindVisionRange( shaderprogram );

		gl->BindVertexArray( p->GetVAO() );
		    gl->UniformMatrix4fv( gl->GetUniformLocation( planeprog.Object(), "view" ), 1, false, cam->view().data() );
			gl->UniformMatrix4fv( gl->GetUniformLocation( planeprog.Object(), "proj" ), 1, false, cam->projection().data() );
			gl->UniformMatrix4fv( gl->GetUniformLocation( planeprog.Object(), "model" ), 1, false, model.data() );
			gl->ActiveTexture( GL_TEXTURE0 );
			gl->BindTexture( GL_TEXTURE_2D, tex->object() );
			gl->Uniform1i( gl->GetUniformLocation( planeprog.Object(), "tex" ), 0 );
			gl->DrawArrays( GL_TRIANGLES, 0, p->NumElements() );
		gl->BindVertexArray(0);
	}

	void RenderSprite3D( Sprite3D* sprite, const cml::matrix44f_c& model )
	{
		cml::vector2f f = sprite->CurrentFrame();
		cml::vector2f s = sprite->FrameSize();
		gl->BindVertexArray( sprite->GetQuad().GetVAO() );
			gl->UniformMatrix4fv( gl->GetUniformLocation( quadprog.Object(), "model" ), 1, false, model.data() );
			gl->Uniform2f( gl->GetUniformLocation( quadprog.Object(), "frame" ), f[0], f[1] );
			gl->Uniform2f( gl->GetUniformLocation( quadprog.Object(), "size" ), s[0], s[1] );
			gl->ActiveTexture( GL_TEXTURE0 );
			gl->BindTexture( GL_TEXTURE_2D, sprite->GetTex()->object() );
			gl->Uniform1i( gl->GetUniformLocation( quadprog.Object(), "tex" ), 0 );
			gl->DrawArrays( GL_TRIANGLE_STRIP, 0, sprite->GetQuad().NumElements() );
		gl->BindVertexArray(0);
	}

	void RenderEntity( Entity* ent )
	{
		ent->SetupFrame( -cam->position());
		gl->UseProgram( quadprog.Object() );
		Sprite3D* sprite = ent->GetSprite();
		sprite->SetCurrentFrame(ent->framex, ent->framey);
		//cml::vector2f f = sprite->CurrentFrame();
		RenderSprite3D( sprite, ent->Model() );
	}

	uint32_t timer = 0;

	void RenderFinish( SDL_Window* mainWindow, uint32_t delta )
	{
		gl->BindFramebuffer(GL_FRAMEBUFFER, 0);
		gl->BindVertexArray(postvao);
		gl->Disable(GL_DEPTH_TEST);
		gl->UseProgram(postprog.Object());

		if( timer >= 100 )
		{
			timer = delta;
			for( int i = 0; i < 256; i++ )
			{
				array[i] = float(rng.uniform(0,3))/10.f;
			}
		}
		else
		{
			timer += delta;
		}
		gl->Uniform1fv( gl->GetUniformLocation( postprog.Object(), "scanarray" ), 256, array );
		gl->Uniform1f( gl->GetUniformLocation( postprog.Object(), "time" ), SDL_GetTicks() );

		gl->ActiveTexture(GL_TEXTURE0);
		gl->BindTexture(GL_TEXTURE_2D, texColorBuffer);

		gl->DrawArrays( GL_QUADS, 0, 4 );
		gl->BindVertexArray(0);
		SDL_GL_SwapWindow(mainWindow);
	}

	void RenderBlocks( Map& map, tdogl::Texture* tex, int num )
	{
		GLuint shaderprogram = blockprog.Object();
		gl->ActiveTexture(GL_TEXTURE0);
		gl->BindTexture(GL_TEXTURE_2D, tex->object());
		gl->Uniform1i( gl->GetUniformLocation( shaderprogram, "tex" ), 0 );
		GLvoid* cpu_model_buffer = ( (GLvoid*) map.GetModelsBuffer(num) );
		gl->BindBuffer(GL_ARRAY_BUFFER, block.GetVBO(1));
		gl->BufferData( GL_ARRAY_BUFFER, sizeof(cml::matrix44f_c) * map.GetModelsNum(num), cpu_model_buffer, GL_STATIC_DRAW);
		gl->DrawArraysInstanced( GL_TRIANGLES, 0, block.NumElements(), map.GetModelsNum(num) );
	}

	void SetupRender() {
		// SETUP MVP MATRICES
		BindPostFBO();
		gl->Enable(GL_DEPTH_TEST);
		RenderClear();
	}

	void RenderFloor( tdogl::Texture* t ) {
		RenderBigPlane( t, 2.f, 0.f, 90.f );
	}

	void RenderRoof( tdogl::Texture* t ) {
		RenderBigPlane( t, -2.f, 300.f, -90.f );
	}

	void RenderBigPlane( tdogl::Texture* t, float y, float z, float rotation ) {
		cml::matrix44f_c model = cml::identity<4>();
		cml::matrix_rotation_world_x( model, cml::rad(rotation) );
		cml::matrix_set_translation( model, 0.f, y, z );
		RenderPlane( &plane, model, t );
	}

	void Dispose()
	{
		quadprog.Dispose( gl );
		blockprog.Dispose( gl );
		block.Dispose( gl );
		plane.Dispose(gl);
		gl->DeleteBuffers(1,&postvbo);
		Font::Dispose();
	}

};

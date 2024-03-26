
#include "renderer.h"

Renderer::Renderer(Render::Context *gl, std::shared_ptr<Camera> cam, int winWidth, int winHeight)
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
                GL_TEXTURE_2D, 0, GL_RGB, this->ww, this->wh, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
				);

	gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gl->FramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
				);

	gl->GenRenderbuffers(1, &rboDepthStencil);
	gl->BindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    gl->RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->ww, this->wh);


	gl->FramebufferRenderbuffer(
				GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil
				);

	gl->BindFramebuffer( GL_FRAMEBUFFER, 0);

	allocPostQuad();

	plane.Prepare(gl,300,300,4,4);

}

void Renderer::renderText(const char *text, float x, float y, cml::vector4f color, float sx_, float sy_)
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

void Renderer::bindPostFBO()
{
	gl->BindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
}

void Renderer::allocPostQuad()
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

void Renderer::renderPostQuad()
{
	gl->BindVertexArray(0);
}

void Renderer::useDefaultFBO() { gl->BindFramebuffer( GL_FRAMEBUFFER, 0 ); }

void Renderer::useCreatedFBO() { gl->BindFramebuffer( GL_FRAMEBUFFER, frameBuffer ); }

void Renderer::renderClear()
{
	gl->ClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
	gl->Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::bindVP(GLuint shaderprogram)
{
	gl->UniformMatrix4fv( gl->GetUniformLocation( shaderprogram, "view" ), 1, false, this->cam->view().data() );
	gl->UniformMatrix4fv( gl->GetUniformLocation( shaderprogram, "proj" ), 1, false, this->cam->projection().data() );
}

void Renderer::bindVisionRange(GLuint shaderprogram)
{
	gl->Uniform1f( gl->GetUniformLocation( shaderprogram, "range" ), PLAYER_VISION_RANGE );
}

void Renderer::RenderMap(Scene& scene)
{
	GLuint shaderprogram = blockprog.Object();
	gl->UseProgram( shaderprogram );
	bindVP( shaderprogram );
	bindVisionRange( shaderprogram );

	GLuint VAO = block.GetVAO();
	gl->BindVertexArray(VAO);
    for( int i = 0; i < scene.NumTypeTiles(); i++ )
    {
        RenderBlocks( scene, i );
    }
	gl->BindVertexArray(0);
}

void Renderer::BatchSprite3D()
{
	GLuint shaderprogram = quadprog.Object();
	gl->UseProgram( shaderprogram );
	bindVP( shaderprogram );
	bindVisionRange( shaderprogram );
}

void Renderer::RenderPlane(Plane *p, const cml::matrix44f_c &model, tdogl::Texture *tex)
{
	GLuint shaderprogram = planeprog.Object();
	gl->UseProgram( shaderprogram );
	bindVP( shaderprogram );
	bindVisionRange( shaderprogram );

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

void Renderer::RenderSprite3D(Sprite3D *sprite, const cml::matrix44f_c &model)
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

void Renderer::RenderEntity(Entity *ent)
{
	ent->SetupFrame( -cam->position());
	gl->UseProgram( quadprog.Object() );
	Sprite3D* sprite = ent->GetSprite();
	sprite->SetCurrentFrame(ent->framex, ent->framey);
	//cml::vector2f f = sprite->CurrentFrame();
	RenderSprite3D( sprite, ent->Model() );
}

void Renderer::Update()
{
	cam->DecayShake(0.9f);
	m_redScreen *= 0.9f;
	m_greenScreen *= 0.9f;
	m_orangeScreen *= 0.9f;
}

void Renderer::RenderFinish(SDL_Window *mainWindow, uint32_t delta)
{
	static uint32_t timer = 0;

	gl->BindFramebuffer(GL_FRAMEBUFFER, 0);
	gl->BindVertexArray(postvao);
	gl->Disable(GL_DEPTH_TEST);
	gl->UseProgram(postprog.Object());

	if( timer >= 100 )
	{
		timer = delta;
		for( int i = 0; i < 256; i++ )
		{
			float val = rng.uniform() / 4.f;
			array[i] = val;
		}
	}
	else
	{
		timer += delta;
	}
	gl->Uniform1fv( gl->GetUniformLocation( postprog.Object(), "scanarray" ), 256, array );
	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "time"), SDL_GetTicks());
	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "health"), m_health);

	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "paintimer"), m_redScreen);
	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "hptimer"), m_greenScreen);
	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "ammotimer"), m_orangeScreen);
	gl->Uniform1f(gl->GetUniformLocation(postprog.Object(), "slowdown"), m_slowdown);

	gl->ActiveTexture(GL_TEXTURE0);
	gl->BindTexture(GL_TEXTURE_2D, texColorBuffer);

	gl->DrawArrays( GL_QUADS, 0, 4 );
	gl->BindVertexArray(0);
	SDL_GL_SwapWindow(mainWindow);
}

void Renderer::RenderBlocks(Scene& scene, int i)
{
	GLuint shaderprogram = blockprog.Object();
	gl->ActiveTexture(GL_TEXTURE0);
    gl->BindTexture(GL_TEXTURE_2D, scene.getTextureForTile(i)->object());
	gl->Uniform1i( gl->GetUniformLocation( shaderprogram, "tex" ), 0 );
	gl->BindBuffer(GL_ARRAY_BUFFER, block.GetVBO(1));
    gl->BufferData( GL_ARRAY_BUFFER, sizeof(cml::matrix44f_c) * scene.getModelsNum(i), scene.getModelsBuffer(i), GL_STATIC_DRAW);
    gl->DrawArraysInstanced( GL_TRIANGLES, 0, block.NumElements(), scene.getModelsNum(i) );
}

void Renderer::SetupRender() {
	// SETUP MVP MATRICES
	bindPostFBO();
	gl->Enable(GL_DEPTH_TEST);
	renderClear();
}

void Renderer::RenderFloor(tdogl::Texture *t) {
	RenderBigPlane( t, 2.f, 0.f, 90.f );
}

void Renderer::RenderRoof(tdogl::Texture *t) {
	RenderBigPlane( t, -2.f, 300.f, -90.f );
}

void Renderer::RenderBigPlane(tdogl::Texture *t, float y, float z, float rotation) {
	cml::matrix44f_c model = cml::identity<4>();
	cml::matrix_rotation_world_x( model, cml::rad(rotation) );
	cml::matrix_set_translation( model, 0.f, y, z );
	RenderPlane( &plane, model, t );
}

void Renderer::Dispose()
{
	quadprog.Dispose( gl );
	blockprog.Dispose( gl );
	block.Dispose( gl );
	plane.Dispose(gl);
	gl->DeleteBuffers(1,&postvbo);
}


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

	inline void bindVP( GLuint shaderprogram );
	inline void bindVisionRange( GLuint shaderprogram );

public:

	void prepare( Render::Context* gl, Camera* cam, int winWidth, int winHeight );

	void renderText( const char* text, float x, float y, cml::vector4f color = cml::vectorf(1.f,0.f,0.f,1.f), float sx_ = 1.f, float sy_ = 1.f );

	void bindPostFBO();
	void allocPostQuad();
	void renderPostQuad();

	void useDefaultFBO();
	void useCreatedFBO();
	void renderClear();

	void RenderMap( Map& map, tdogl::Texture* tex1, tdogl::Texture* tex2, tdogl::Texture* tex3 );
	void BatchSprite3D();


	void RenderPlane( Plane* p, const cml::matrix44f_c& model, tdogl::Texture* tex );
	void RenderSprite3D( Sprite3D* sprite, const cml::matrix44f_c& model );
	void RenderEntity( Entity* ent );

	void RenderFinish( SDL_Window* mainWindow, uint32_t delta );
	void RenderBlocks( Map& map, tdogl::Texture* tex, int num );
	void SetupRender();

	void RenderFloor( tdogl::Texture* t );
	void RenderRoof( tdogl::Texture* t );
	void RenderBigPlane( tdogl::Texture* t, float y, float z, float rotation );

	void Dispose();

};

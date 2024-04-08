
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
#include "scene.h"
#include "camera.h"
#include "font.h"
#include "../entity/entitymanager.h"


class Renderer
{

public:

	void SetClearColor(const cml::vector3f& clearColor)
	{
		m_clearColor = clearColor;
	}

	void SetSlowdown(float slowdown)
	{
		m_slowdown = slowdown;
	}

	void SetPlayerHealth(float playerHealth)
	{
		m_health = playerHealth > 1.0f ? 1.0f : playerHealth;
	}

	void AddShake(float shake)
	{
		cam->AddShake(shake);
	}

	void AddRedScreen(float rs)
	{
		m_redScreen += rs;
	}

	void AddGreenScreen(float rs)
	{
		m_greenScreen += rs;
	}

	void AddOrangeScreen(float rs)
	{
		m_orangeScreen += rs;
	}

	Renderer( Render::Context* gl, std::shared_ptr<Camera> cam, int winWidth, int winHeight, SDL_Window* window );

	void renderText( const char* text, float x, float y, cml::vector4f color = cml::vectorf(1.f,0.f,0.f,1.f), float sx_ = 1.f, float sy_ = 1.f );

	void Dispose();

	void BatchSprite3D();
	void useDefaultFBO();
	void useCreatedFBO();

	void SceneRender(float viewer_angle, Scene& map, const std::shared_ptr<EntityManager>& emanager)
	{
		RenderMap(map);
		RenderEntities(emanager, viewer_angle);
	}

	void RenderFinish();
	void RenderBegin();
	void Update();

	void RenderSprite3D(Sprite3D* sprite, const cml::matrix44f_c& model);

private:
	void RenderMap(Scene& map);

	void RenderEntities(std::shared_ptr<EntityManager> emanager, float playerAngle)
	{
		BatchSprite3D();

		for (size_t i = 0; i < emanager->NumEntities(); i++)
		{
			Entity* e = emanager->GetEntity(i);
			e->PhysicStep();
			e->ClearVelocity();
			e->SetAngleY(cml::rad(180 + playerAngle));
			RenderEntity(e);
		}
	}




	void allocPostQuad();
	void renderPostQuad();




	void RenderPlane(Plane* p, const cml::matrix44f_c& model, tdogl::Texture* tex);
	void RenderEntity(Entity* ent);

	void RenderPostFX();
	void RenderPostFXSimple();

	void RenderBlocks(Scene& scene, int i);

	void RenderFloor(tdogl::Texture* t);
	void RenderRoof(tdogl::Texture* t);
	void RenderBigPlane(tdogl::Texture* t, float y, float z, float rotation);









	const float PLAYER_VISION_RANGE = 17.f;

	float sprite3d_fog_range;
	float cube_fog_range;

	Render::Context* gl;
	Program quadprog, blockprog, planeprog, postprog, fontprog, simplepostprog;

	// usado para posteffects
	GLuint frameBuffer, texColorBuffer, rboDepthStencil, postvao, postvbo;
	GLfloat array[256];
	RNG rng;

	Block block;

	GLuint attribute_coord, uniform_tex, uniform_color;
	GLuint fontvao, fontvbo;
	int ww, wh;
	std::shared_ptr<Camera> cam;
	Plane plane;
	Font default_font;
	Font small_font;

	inline void bindVP(GLuint shaderprogram);
	inline void bindVisionRange(GLuint shaderprogram);

	float m_health = 0;
	float m_redScreen = 0.0f;
	float m_greenScreen = 0.0f;
	float m_orangeScreen = 0.0f;
	float m_slowdown = 0.0f;

	SDL_Window* window;

	cml::vector3f m_clearColor = { 0.0f, 0.05f, 0.1f };

	bool m_useCreatedFBO = true;
};


#pragma once

#include <SDL.h>
#include "../render/sdlcontext.h"
#include "../render/renderer.h"
#include "../physics/physics.h"
#include "../entity/entitymanager.h"


class Engine
{
public:

	Engine(Render::Context* gl, int winWidth, int winHeight)
		:
		m_gl(gl),
		m_winWidth(winWidth),
		m_winHeight(winHeight)
	{

	}

	void SetSlowDown(float slowdown)
	{
		m_slowDown = slowdown;
		m_renderer->SetSlowdown(slowdown);
	}

	const std::shared_ptr<Renderer>& renderer()
	{
		return m_renderer;
	}

	const std::shared_ptr<EntityManager>& emanager()
	{
		return m_emanager;
	}

	const std::shared_ptr<Physics> physics()
	{
		return m_physics;
	}

	const std::shared_ptr<Camera>& cam()
	{
		return m_cam;
	}

	std::shared_ptr<Transform> sceneRoot()
	{
		return m_sceneRoot;
	}

	Render::Context* gl()
	{
		return m_gl;
	}

	void SetGL(Render::Context* gl)
	{
		m_gl = gl;
	}

	void SetSlowdown(float slowdown)
	{
		m_slowDown = slowdown;
	}

	float GetSlowdown()
	{
		return m_slowDown;
	}

	void LoadScene(Matrix2D tilemap)
	{
		m_scene = std::make_shared<Scene>();
		m_scene->tilemap(tilemap);

		loadCubesForScene();
		m_emanager->ClearAllEntities();
	}

	void SetSceneTextures(std::vector<tdogl::Texture*> texturesForTiles, tdogl::Texture* floorTexture, tdogl::Texture* roofTexture)
	{
		for (int i = 0; i < texturesForTiles.size(); i++)
		{
			m_scene->setTextureForTile(i+1, texturesForTiles[i]);
		}

		m_scene->setFloorTexture(floorTexture);
		m_scene->setRoofTexture(roofTexture);
	}

	void loadCubesForScene()
	{
		auto tilemap = m_scene->tilemap();
		for (int i = 0; i < tilemap.Cols(); i++)
		{
			for (int j = 0; j < tilemap.Rows(); j++)
			{
				if (tilemap.Get(i, j) != 0)
					m_physics->AddCubeBody(-i * 2, -j * 2, 0);
			}
		}
	}

	const std::shared_ptr<Scene> GetScene()
	{
		return m_scene;
	}

	void sceneRender(float viewer_angle)
	{
		m_renderer->SetupRender();
		m_renderer->RenderFloor(m_scene->floorTex());
		m_renderer->RenderRoof(m_scene->roofTex());
		m_renderer->RenderMap(*m_scene);
		m_renderer->BatchSprite3D();
		m_emanager->RenderEntities(viewer_angle);
	}

	void Reset(b2ContactListener* contactListener)
	{
		m_slowDown = 1.0f;
		m_cam = std::make_shared<Camera>();
		m_renderer = std::make_shared<Renderer>(m_gl, m_cam, m_winWidth, m_winHeight);
		m_emanager = std::make_shared<EntityManager>(m_renderer);
		m_physics = std::make_shared<Physics>(contactListener);
		m_cam->position(cml::vector3f(0, 0, 0));
		m_cam->horizontalAngle(90);
		m_sceneRoot = std::make_shared<Transform>();
	}

	void PostUpdate()
	{
		// Clean dead entities
		m_sceneRoot->UpdateClean();
		m_emanager->ClearDeadEntities();
	}

	void PreUpdate(uint32_t delta)
	{
		// Update everything
		m_sceneRoot->Update(Transform(), delta * m_slowDown);
		m_physics->Step(m_slowDown);
	}



private:
	float m_slowDown = 1.0f;

	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<EntityManager> m_emanager;
	std::shared_ptr<Physics> m_physics;
	std::shared_ptr<Camera> m_cam;
	std::shared_ptr<Transform> m_sceneRoot;
	Render::Context* m_gl;
	std::shared_ptr<Scene> m_scene;

	int m_winWidth = 0;
	int m_winHeight = 0;


};














class SDLGLApp {

private:
	bool running;

    SDL_Window* mainWindow;
    SDL_GLContext mainGLContext; // en sdlcontext?
    int winWidth, winHeight;

    uint32_t sdlwinflags;

    std::vector<std::string> m_args;

    std::shared_ptr<Engine> m_engine;

protected:

	virtual void Setup(const std::vector<std::string>& args) = 0 ;
	virtual void Update(uint32_t delta) = 0 ;
	virtual void Render() = 0 ;
	virtual void Cleanup() = 0 ;
	virtual void HandleEvent(SDL_Event& event) = 0 ;

    const std::vector<std::string>& GetArgs() const
    {
        return m_args;
    }

    const std::shared_ptr<Engine>& engine()
    {
        return m_engine;
    }

	void Stop()
	{ running=false; }


    Render::Context* GL()
    {
        return m_engine->gl();
    }

    virtual void PostUpdate() = 0;

    virtual void PreUpdate(uint32_t delta) = 0;

public:
	SDLGLApp ( int width, int height, uint32_t sdlwinflags=(SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL) );
    virtual ~SDLGLApp () = 0 ;

	int Exec(int argc, char** argv);


};



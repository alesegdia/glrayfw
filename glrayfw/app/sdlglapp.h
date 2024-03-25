
#pragma once

#include <SDL.h>
#include "../render/sdlcontext.h"
#include "../render/renderer.h"
#include "../physics/physics.h"
#include "../entity/entitymanager.h"

class Engine
{
public:

    void SetSlowDown(float slowdown)
    {
        m_slowDown = slowdown;
        m_renderer.SetSlowdown(slowdown);
    }
    Renderer& renderer()
    {
        return m_renderer;
    }

    EntityManager& emanager()
    {
        return m_emanager;
    }

    Physics& physics()
    {
        return m_physics;
    }

    Camera& cam()
    {
        return m_cam;
    }

    Transform& sceneRoot()
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

    void loadScene(Scene* scene)
    {
        m_scene = scene;
        m_physics.Cleanup();
        m_physics.Init();
        loadCubesForScene(m_scene);
        m_emanager.ClearAllEntities();
    }

    void loadCubesForScene(Scene* scn)
    {
        auto tilemap = scn->tilemap();
        for (int i = 0; i < tilemap.Cols(); i++)
        {
            for (int j = 0; j < tilemap.Rows(); j++)
            {
                if (tilemap.Get(i, j) != 0)
                    m_physics.AddCubeBody(-i * 2, -j * 2, 0);
            }
        }
    }

    Scene* scene()
    {
        return m_scene;
    }

    void sceneRender(float viewer_angle)
    {
        m_renderer.SetupRender();
        m_renderer.RenderFloor(m_scene->floorTex());
        m_renderer.RenderRoof(m_scene->roofTex());
        m_renderer.RenderMap(*m_scene);
        m_renderer.BatchSprite3D();
        m_emanager.RenderEntities(viewer_angle);
    }



private:
    float m_slowDown = 1.0f;

    Renderer m_renderer;
    EntityManager m_emanager;
    Physics m_physics;
    Camera m_cam;
    Transform m_sceneRoot;
    Render::Context* m_gl;
    Scene* m_scene;


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

    void PostUpdate()
    {
        // Clean dead entities
        m_engine->sceneRoot().UpdateClean();
        m_engine->emanager().ClearDeadEntities();
    }

    void PreUpdate(int delta)
    {
        // Update everything
        m_engine->sceneRoot().Update(Transform(), delta * m_engine->GetSlowdown());
        m_engine->physics().Step(m_engine->GetSlowdown());
    }

public:
	SDLGLApp ( int width, int height, uint32_t sdlwinflags=(SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL) );
    virtual ~SDLGLApp () = 0 ;

	int Exec(int argc, char** argv);


};



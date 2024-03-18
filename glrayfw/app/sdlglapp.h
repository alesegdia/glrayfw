
#pragma once

#include <SDL.h>
#include "../render/sdlcontext.h"
#include "../render/renderer.h"
#include "../physics/physics.h"
#include "../entity/entitymanager.h"

class SDLGLApp {

private:
	bool running;
    Scene* m_scene;

    SDL_Window* mainWindow;
    SDL_GLContext mainGLContext; // en sdlcontext?
    Render::SDL::Context* gl;
    int winWidth, winHeight;

    uint32_t sdlwinflags;

    Renderer m_renderer;
    EntityManager emanager;
    Physics m_physics;
    Camera m_cam;
    Transform m_sceneRoot;


protected:

	virtual void Setup(int argc, char** argv) = 0 ;
	virtual void Update(uint32_t delta) = 0 ;
	virtual void Render() = 0 ;
	virtual void Cleanup() = 0 ;
	virtual void HandleEvent(SDL_Event& event) = 0 ;

	void Stop()
	{ running=false; }

    Physics& physics()
    {
        return m_physics;
    }

    EntityManager& entityManager()
    {
        return emanager;
    }

    void loadScene(Scene* scene)
    {
        m_scene = scene;
        m_physics.Cleanup();
        m_physics.Init();
        loadCubesForScene(m_scene);
        emanager.ClearAllEntities();
    }

    void loadCubesForScene(Scene* scn)
    {
        auto tilemap = scn->tilemap();
        for(int i = 0; i < tilemap.Cols(); i++ )
        {
            for( int j = 0; j < tilemap.Rows(); j++ )
            {
                if( tilemap.Get(i,j) != 0 )
                    m_physics.AddCubeBody(-i*2,-j*2, 0);
            }
        }
    }

    Render::SDL::Context* GL()
    {
        return gl;
    }

    void PostUpdate()
    {
        // Clean dead entities
        this->m_sceneRoot.UpdateClean();
        entityManager().ClearDeadEntities();
    }

    void PreUpdate(int delta)
    {
        SDL_WarpMouseInWindow( NULL, 400, 300 );

        // Update everything
        m_sceneRoot.Update(Transform(), delta);
        physics().Step();
    }

    Transform& sceneRoot()
    {
        return m_sceneRoot;
    }

    Camera& cam()
    {
        return m_cam;
    }

    void sceneRender(float viewer_angle)
    {
        m_renderer.SetupRender();
        m_renderer.RenderFloor(m_scene->floorTex());
        m_renderer.RenderRoof(m_scene->roofTex());
        m_renderer.RenderMap( *m_scene );
        m_renderer.BatchSprite3D();
        emanager.RenderEntities( viewer_angle );

    }

    Renderer& renderer()
    {
        return m_renderer;
    }

public:
	SDLGLApp ( int width, int height, uint32_t sdlwinflags=(SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL) );
    virtual ~SDLGLApp () = 0 ;

	int Exec(int argc, char** argv);


};



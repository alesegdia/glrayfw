
#pragma once

#include <SDL2/SDL.h>
#include "../render/sdlcontext.h"
#include "../render/renderer.h"
#include "../physics/physics.h"
#include "../entity/entitymanager.h"

class SDLGLApp {

private:
	bool running;

protected:

	SDL_Window* mainWindow;
	SDL_GLContext mainGLContext; // en sdlcontext?
	Render::SDL::Context* gl;
	int winWidth, winHeight;

	uint32_t sdlwinflags;

    Renderer renderer;
    EntityManager emanager;
    Physics physics;
    Camera cam;

	virtual void Setup(int argc, char** argv) = 0 ;
	virtual void Update(uint32_t delta) = 0 ;
	virtual void Render() = 0 ;
	virtual void Cleanup() = 0 ;
	virtual void HandleEvent(SDL_Event& event) = 0 ;

	void Stop()
	{ running=false; }


public:
	SDLGLApp ( int width, int height, uint32_t sdlwinflags=(SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL) );
    virtual ~SDLGLApp () = 0 ;

	int Exec(int argc, char** argv);


};




#pragma once

#include <SDL2/SDL.h>
#include "../render/sdlcontext.h"

class SDLGLApp {

private:
	bool running;

protected:

	SDL_Window* mainWindow;
	SDL_GLContext mainGLContext; // en sdlcontext?
	Render::SDL::Context* gl;
	int winWidth, winHeight;

	uint32_t sdlwinflags;

	virtual void Setup(int argc, char** argv) = 0 ;
	virtual void Update(uint32_t delta) = 0 ;
	virtual void Render() = 0 ;
	virtual void Cleanup() = 0 ;
	virtual void HandleEvent(SDL_Event& event) = 0 ;

	void Stop()
	{ running=false; }


public:
	SDLGLApp ( int width, int height, uint32_t sdlwinflags=(SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL) );
	virtual ~SDLGLApp ();

	int Exec(int argc, char** argv);


};



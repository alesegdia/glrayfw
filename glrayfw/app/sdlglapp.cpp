#include "sdlglapp.h"
#include "../core/fpscounter.h"

#include <iostream>

SDLGLApp::SDLGLApp ( int width, int height, uint32_t sdlwinflags ) {
	running=true;
	mainWindow=NULL;
	winWidth=width;
	winHeight=height;
	this->sdlwinflags=sdlwinflags;
}

SDLGLApp::~SDLGLApp()
{
	delete gl;
}

int SDLGLApp::Exec(int argc, char** argv)
{

	mainWindow = SDL_CreateWindow(
		"SDL2 test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		winWidth, winHeight, sdlwinflags);

	if(!mainWindow) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -2;
	}

	mainGLContext = SDL_GL_CreateContext(mainWindow);
	gl = new Render::SDL::Context( mainGLContext, Render::Context::Profile::Core );
	gl->MakeCurrent( mainWindow );
	SDL_GL_SetSwapInterval(0);

	Setup(argc,argv);

	SDL_Event event;
	const int TIME_STEP = 20;
	int currtime = SDL_GetTicks();
	int delta;

	FPSCounter<Uint32> fpsc;
	Uint32 prevfps = 0;
	while(running) {
		int prevtime = currtime;
		currtime = SDL_GetTicks();
		fpsc.Tick(currtime);
		delta += (currtime - prevtime);
		if( fpsc.GetFPS() != prevfps )
		{
			prevfps = fpsc.GetFPS();
			char buffer[10];
			sprintf(buffer, "%d fps", fpsc.GetFPS() );
			SDL_SetWindowTitle( mainWindow, buffer );
		}

		while(SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		int i = 0;
		bool was_updated;
		while( delta >= TIME_STEP ) {
			was_updated = true;
			i++;
			Update(TIME_STEP);
			delta -= TIME_STEP;
		}
		if( was_updated ) Render();
	}

	Cleanup();

	gl->Cleanup();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

	return 0;
}


void SDLGLApp::Setup(int argc, char** argv)
{

}

void SDLGLApp::Update(uint32_t delta)
{

}

void SDLGLApp::Render()
{

}

void SDLGLApp::HandleEvent(SDL_Event& event)
{

}

void SDLGLApp::Cleanup()
{

}

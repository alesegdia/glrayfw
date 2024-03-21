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
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    SDL_DisplayMode current;

    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

        int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

        if(should_be_zero != 0)
        {
            SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
        }
        else
        {
            SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);
        }
    }

    //winWidth = current.w;
    //winHeight = current.h;

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
    //SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

	gl = new Render::SDL::Context( mainGLContext, Render::Context::Profile::Core );
	gl->MakeCurrent( mainWindow );
    SDL_GL_SetSwapInterval(1);
    //SDL_GetWindowSize(mainWindow, &(this->winWidth), &(this->winHeight));

	//SDL_SetWindowMouseGrab(mainWindow, SDL_TRUE);
	//SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
    //SDL_SetWindowSize(mainWindow, current.w, current.h);

    SDL_ShowCursor(0);
    m_renderer.prepare( gl, &m_cam, winWidth, winHeight );
    emanager.Prepare(&m_renderer);
    m_physics.Init();
    m_cam.position( cml::vector3f(0,0,0) );
    m_cam.horizontalAngle( 90 );

	const unsigned char* vendor;
	vendor = gl->GetString(GL_VENDOR);
	std::cout << "GL vendor: " << vendor << std::endl;

	const unsigned char* renderer;
	renderer = gl->GetString(GL_RENDERER);
	std::cout << "GL renderer: " << renderer << std::endl;

	const unsigned char* version;
	version = gl->GetString(GL_VERSION);
	std::cout << "GL version: " << version << std::endl;

	const unsigned char* shading;
	shading = gl->GetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "GL shading lang version: " << shading << std::endl;

	Setup(argc,argv);

	SDL_Event event;
    const int TIME_STEP = 1000/60;
	int currtime = SDL_GetTicks();
	int delta = 0 ;

	FPSCounter<int> fpsc;
	int prevfps = 0;
	while(running) {
		int prevtime = currtime;
		currtime = SDL_GetTicks();
		fpsc.Tick(currtime);
		delta += (currtime - prevtime);
		if( fpsc.GetFPS() != prevfps )
		{
			// commented cause it was giving a runtime error
			/*
			prevfps = fpsc.GetFPS();
			char buffer[10];
			sprintf(buffer, "%d fps", fpsc.GetFPS() );
			SDL_SetWindowTitle( mainWindow, buffer );
			*/
		}

		while(SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		int i = 0;
		bool was_updated = false;
		auto origDelta = delta;
		while( delta >= TIME_STEP ) {
			was_updated = true;
			i++;
            PreUpdate(TIME_STEP);
			Update(TIME_STEP);
            PostUpdate();
			delta -= TIME_STEP;
		}
        if( was_updated )
        {
            Render();
            m_renderer.RenderFinish( mainWindow, origDelta );
        }
	}

	Cleanup();
    m_renderer.Dispose( );
    emanager.ClearAllEntities();
    m_physics.Cleanup();

	gl->Cleanup();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

	return 0;
}



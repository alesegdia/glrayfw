#include "sdlglapp.h"
#include "../core/fpscounter.h"
#include "glrayfw/core/core.h"
#include <iostream>

SDLGLApp::SDLGLApp ( int width, int height, uint32_t sdlwinflags ) {
	running=true;
	mainWindow=NULL;
	winWidth=width;
	winHeight=height;
	this->sdlwinflags=sdlwinflags;
	m_engine = std::make_shared<Engine>();
}

SDLGLApp::~SDLGLApp()
{
	delete m_engine->gl();
}

int SDLGLApp::Exec(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    SDL_DisplayMode current;

    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

        int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

        if(should_be_zero != 0)
        {
			Logger::getInstance().log(LogLevel::ERR, "Could not get display mode for video display #", i, ": ", SDL_GetError());
        }
        else
        {
			Logger::getInstance().log(LogLevel::INFO, "Display #", i, ": current display mode is ", current.w, "x", current.h, "px @ ", current.refresh_rate, "hz.");
        }
    }

    //winWidth = current.w;
    //winHeight = current.h;

    mainWindow = SDL_CreateWindow(
		"SDL2 test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
        winWidth, winHeight, sdlwinflags);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(NULL, 400, 300);

	if(!mainWindow) {
		Logger::getInstance().log(LogLevel::ERR, "Window could not be created! SDL_Error: ", SDL_GetError() );
		return -2;
	}

    mainGLContext = SDL_GL_CreateContext(mainWindow);
    //SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);

	auto sdlgl = new Render::SDL::Context( mainGLContext, Render::Context::Profile::Core );
	m_engine->SetGL(sdlgl);
	sdlgl->MakeCurrent(mainWindow);
	SDL_GL_SetSwapInterval(1);
    //SDL_GetWindowSize(mainWindow, &(this->winWidth), &(this->winHeight));

	//SDL_SetWindowMouseGrab(mainWindow, SDL_TRUE);
	//SDL_CaptureMouse(SDL_TRUE);
    //SDL_SetWindowSize(mainWindow, current.w, current.h);

    SDL_ShowCursor(0);
    m_engine->renderer().prepare(sdlgl, &m_engine->cam(), winWidth, winHeight);
    m_engine->emanager().Prepare(&m_engine->renderer());
    m_engine->physics().Init();
    m_engine->cam().position( cml::vector3f(0,0,0) );
    m_engine->cam().horizontalAngle( 90 );

	const unsigned char* vendor;
	vendor = sdlgl->GetString(GL_VENDOR);
	Logger::getInstance().log(LogLevel::INFO, "GL vendor: ", vendor);

	const unsigned char* renderer;
	renderer = sdlgl->GetString(GL_RENDERER);
	Logger::getInstance().log(LogLevel::INFO, "GL renderer: ", renderer);

	const unsigned char* version;
	version = sdlgl->GetString(GL_VERSION);
	Logger::getInstance().log(LogLevel::INFO, "GL version: ", version);

	const unsigned char* shading;
	shading = sdlgl->GetString(GL_SHADING_LANGUAGE_VERSION);
	Logger::getInstance().log(LogLevel::INFO, "GLSL version: ", shading);

	
	for (int i = 0; i < argc; i++)
	{
		m_args.push_back(argv[i]);
	}
	Setup(m_args);

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
            m_engine->renderer().RenderFinish(mainWindow, origDelta);
        }
	}

	Cleanup();
    m_engine->renderer().Dispose( );
    m_engine->emanager().ClearAllEntities();
    m_engine->physics().Cleanup();

	sdlgl->Cleanup();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

	return 0;
}




#pragma once

#include <SDL.h>
#include "glrayfw/render/sdlcontext.h"
#include "glrayfw/app/engine.h"


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



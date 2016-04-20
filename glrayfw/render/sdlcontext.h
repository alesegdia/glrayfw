
#pragma once
#include <stdint.h>
#include <GL/glcorearb.h>
#include <SDL2/SDL.h>

#include "context.h"

namespace Render {

namespace SDL {
class Context : public Render::Context {

private:

  SDL_GLContext ctx;


public:

	Context( SDL_GLContext c, Render::Context::Profile prof ) :
		Render::Context(prof),
		ctx(c)
  {}
  virtual ~Context();


public:

  using Render::Context::GetProc;
  bool GetProc(void*&p,const char*name)
	{ return (p = (void*)SDL_GL_GetProcAddress(name)); }

  void GetVersion(uint8_t &major, uint8_t &minor, Render::Context::Profile&profile);
  bool CheckExtension(const char*name);
  bool MakeCurrent( SDL_Window* w );

  	// elimina el contexto
  void Cleanup()
  { SDL_GL_DeleteContext( ctx ); }

public:
};

}
}

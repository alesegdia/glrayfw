
#include "sdlcontext.h"

namespace Render {
namespace SDL {

Context::~Context()
{

}

bool Context::MakeCurrent( SDL_Window* w )
{
	SDL_GL_MakeCurrent( w, ctx );
	return Render::Context::MakeCurrent();
}

void Context::GetVersion(uint8_t &major, uint8_t &minor, Render::Context::Profile&profile)
{
  int value;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);	major = value;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);	minor = value;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &value);
  if ( (major == 3 && minor >= 1) || major > 3)
    profile = SDL_GL_ExtensionSupported("GL_ARB_compatibility")?Profile::Compatibility:Profile::Core;
  else
    profile = Profile::Compatibility;
}

bool Context::CheckExtension(const char*name)
{
  return SDL_GL_ExtensionSupported(name);
}


}
}

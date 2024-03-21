
#pragma once

#include "glcorearb.h"
#include <cstdint>

namespace Render {

/*
typedef cml::matrix44f_c	Matrix44f;
typedef cml::vector2f		Vector2;
typedef cml::vector3f		Vector3;
typedef cml::vector4f 		Vector4;
typedef cml::quaternionf_p	Quaternion;
*/

class Context {

public:

  enum class Profile {
    Core,
    Compatibility
  };

  Context( Render::Context::Profile prof )
  	  : profile(prof)
  {}
  virtual ~Context(){}


  const Profile profile;
  static Context* current;

  PFNGLENABLEPROC Enable;
  PFNGLDISABLEPROC Disable;
  PFNGLBINDBUFFERPROC BindBuffer;
  PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
  PFNGLGENBUFFERSPROC GenBuffers;
  PFNGLBUFFERDATAPROC BufferData;
  PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
  PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
  PFNGLCREATESHADERPROC CreateShader;
  PFNGLSHADERSOURCEPROC ShaderSource;
  PFNGLFRONTFACEPROC FrontFace;
  PFNGLGETERRORPROC GetError;
  PFNGLCOMPILESHADERPROC CompileShader;
  PFNGLGETSHADERIVPROC GetShaderiv;
  PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
  PFNGLATTACHSHADERPROC AttachShader;
  PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
  PFNGLLINKPROGRAMPROC LinkProgram;
  PFNGLUSEPROGRAMPROC UseProgram;
  PFNGLGETPROGRAMIVPROC GetProgramiv;
  PFNGLCLEARCOLORPROC ClearColor;
  PFNGLCLEARPROC Clear;
  PFNGLDRAWARRAYSPROC DrawArrays;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
  PFNGLDETACHSHADERPROC DetachShader;
  PFNGLDELETEPROGRAMPROC DeleteProgram;
  PFNGLDELETESHADERPROC DeleteShader;
  PFNGLDELETEBUFFERSPROC DeleteBuffers;
  PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
  PFNGLCREATEPROGRAMPROC CreateProgram;
  PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
  PFNGLBINDVERTEXARRAYPROC BindVertexArray;
  PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
  PFNGLUNIFORM1FVPROC Uniform1fv;
  PFNGLUNIFORM1FPROC Uniform1f;
  PFNGLUNIFORM2FPROC Uniform2f;
  PFNGLUNIFORM3FPROC Uniform3f;
  PFNGLUNIFORM4FPROC Uniform4f;
  PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
  PFNGLCULLFACEPROC CullFace;
  PFNGLBLENDFUNCPROC BlendFunc;
  PFNGLUNIFORM1IPROC Uniform1i;

  PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
  PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
  PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
  PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
  PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
  PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
  PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;

  PFNGLACTIVETEXTUREPROC ActiveTexture;
  PFNGLBINDTEXTUREPROC BindTexture;

  PFNGLPIXELSTOREIPROC PixelStorei;
  PFNGLTEXPARAMETERIPROC TexParameteri;
  PFNGLTEXIMAGE2DPROC TexImage2D;
  PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
  PFNGLGENTEXTURESPROC GenTextures;
  PFNGLDELETETEXTURESPROC DeleteTextures;

  PFNGLDEPTHFUNCPROC DepthFunc;

  PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
  PFNGLVERTEXATTRIBDIVISORPROC VertexAttribDivisor;
  PFNGLDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;

  PFNGLGETSTRINGPROC GetString;


protected:

  virtual bool GetProc(void*&p,const char*name) = 0;
  template <typename T> bool GetProc(T&p, const char*name)
  	{return GetProc((void*&)p, name);}

  virtual void GetVersion(uint8_t &major, uint8_t &minor, Render::Context::Profile&profile) = 0;

  virtual bool CheckExtension(const char*name) = 0;

  bool MakeCurrent();

public:
};


}



#include "context.h"

namespace Render{


bool Context::MakeCurrent()
{
  current = this;
  return
    GetProc( BindBuffer, "glBindBuffer" ) &&
    GetProc( BindVertexArray, "glBindVertexArray" ) &&
    GetProc( GenVertexArrays, "glGenVertexArrays" ) &&
    GetProc( GenBuffers, "glGenBuffers" ) &&
    GetProc( DepthFunc, "glDepthFunc" ) &&
    GetProc( BufferData, "glBufferData" ) &&
    GetProc( PixelStorei, "glPixelStorei" ) &&
    GetProc( VertexAttribPointer, "glVertexAttribPointer" ) &&
    GetProc( GetError, "glGetError" ) &&
    GetProc( EnableVertexAttribArray, "glEnableVertexAttribArray" ) &&
    GetProc( CreateShader, "glCreateShader" ) &&
    GetProc( CreateProgram, "glCreateProgram" ) &&
    GetProc( GetProgramInfoLog, "glGetProgramInfoLog" ) &&
    GetProc( ShaderSource, "glShaderSource" ) &&
    GetProc( CompileShader, "glCompileShader" ) &&
    GetProc( GetShaderiv, "glGetShaderiv" ) &&
    GetProc( GetShaderInfoLog, "glGetShaderInfoLog" ) &&
    GetProc( AttachShader, "glAttachShader" ) &&
    GetProc( BindAttribLocation, "glBindAttribLocation" ) &&
    GetProc( LinkProgram, "glLinkProgram" ) &&
    GetProc( UseProgram, "glUseProgram" ) &&
    GetProc( GetProgramiv, "glGetProgramiv" ) &&
    GetProc( ClearColor, "glClearColor" ) &&
    GetProc( Clear, "glClear" ) &&
    GetProc( DrawArrays, "glDrawArrays" ) &&
    GetProc( DisableVertexAttribArray, "glDisableVertexAttribArray" ) &&
    GetProc( DetachShader, "glDetachShader" ) &&
    GetProc( DeleteProgram, "glDeleteProgram" ) &&
    GetProc( DeleteShader, "glDeleteShader" ) &&
    GetProc( DeleteBuffers, "glDeleteBuffers" ) &&
    GetProc( UniformMatrix4fv, "glUniformMatrix4fv" ) &&
    GetProc( Uniform1fv, "glUniform1fv" ) &&
    GetProc( Uniform1f, "glUniform1f" ) &&
    GetProc( Uniform2f, "glUniform2f" ) &&
    GetProc( Uniform3f, "glUniform3f" ) &&
    GetProc( Uniform4f, "glUniform4f" ) &&
    GetProc( GetUniformLocation, "glGetUniformLocation" ) &&
    GetProc( CullFace, "glCullFace" ) &&
    GetProc( BlendFunc, "glBlendFunc" ) &&
    GetProc( FrontFace, "glFrontFace" ) &&
    GetProc( Enable, "glEnable" ) &&
    GetProc( Disable, "glDisable" ) &&
    GetProc( TexParameteri, "glTexParameteri" ) &&
    GetProc( TexImage2D, "glTexImage2D" ) &&
    GetProc( TexSubImage2D, "glTexSubImage2D" ) &&
    GetProc( GenTextures, "glGenTextures" ) &&
    GetProc( Uniform1i, "glUniform1i" ) &&
    GetProc( ActiveTexture, "glActiveTexture" ) &&
    GetProc( BindTexture, "glBindTexture" ) &&
    GetProc( DeleteTextures, "glDeleteTextures" ) &&
    GetProc( GetAttribLocation, "glGetAttribLocation" ) &&
    GetProc( VertexAttribDivisor, "glVertexAttribDivisor" ) &&
    GetProc( DrawArraysInstanced, "glDrawArraysInstanced" ) &&


    GetProc( GenFramebuffers, "glGenFramebuffers" ) &&
    GetProc( BindFramebuffer, "glBindFramebuffer" ) &&
    GetProc( FramebufferTexture2D, "glFramebufferTexture2D" ) &&
    GetProc( GenRenderbuffers, "glGenRenderbuffers" ) &&
    GetProc( BindRenderbuffer, "glBindRenderbuffer" ) &&
    GetProc( RenderbufferStorage, "glRenderbufferStorage" ) &&
    GetProc( FramebufferRenderbuffer, "glFramebufferRenderbuffer" ) &&

    GetProc(DeleteVertexArrays, "glDeleteVertexArrays") &&
    GetProc(GetString, "glGetString");
}

}

Render::Context* Render::Context::current;



#pragma once

#include "Texture.h"
#include "quad.h"

class Sprite3D
{

private:

	float angle;
	tdogl::Texture* tex;
	Quad quad;
	cml::vector2f size;
	cml::vector2f /* i? */ current_frame, num_frames;


public:

	/*
	class AnimData
	{
		float 
	}
	*/

	void SetAngle( float angle )
	{
		this->angle = angle;
	}

	void Prepare( Render::Context* gl, tdogl::Texture* tex, int frames_x = 1, int frames_y = 1 )
	{
		num_frames[0] = frames_x;
		num_frames[1] = frames_y;
		current_frame[0] = 0;
		current_frame[1] = 0;
		size[0] = 1.f/((float)frames_x);
		size[1] = 1.f/((float)frames_y);
		this->tex = tex;
		quad.Prepare( gl, 2.f, 2.f, size[0], size[1]);
	}

	void Dispose( Render::Context* gl )
	{
		quad.Dispose( gl );
	}

	void SetCurrentFrame( float x, float y )
	{
		current_frame[0] = x;
		current_frame[1] = y;
	}

	const cml::vector2f& CurrentFrame()
	{
		return current_frame;
	}

	const cml::vector2f& FrameSize()
	{
		return size;
	}

	tdogl::Texture* GetTex()
	{
		return tex;
	}

	Quad& GetQuad()
	{
		return quad;
	}

};

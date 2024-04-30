

#pragma once

#include "Texture.h"
#include "quad.h"

enum class Side : int
{
	Left = 0,
	Right,
	Front,
	Back,
	Max,
	None = -1
};


class Sprite3D
{

private:

	float angle;
	tdogl::Texture* tex;
	Quad quad;
	cml::vector2f size;
	cml::vector2i current_frame, num_frames;


public:

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

	void SetCurrentFrame( int x, int y )
	{
		current_frame[0] = x;
		current_frame[1] = y;
	}

	const cml::vector2i& CurrentFrame()
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

class AnimData
{
public:
	void SetCurrentSide(Side side)
	{
		currentSide = side;
	}

	Side GetCurrentSide()
	{
		return currentSide;
	}

	cml::vector2i currentFrame;
	uint32_t timer = 0;

private:
	Side currentSide = Side::None;
};

class Animation
{
public:
	void SetLoop(bool loop)
	{
		m_loop = loop;
	}

	void SetFrameDuration(uint32_t frameDuration)
	{
		m_frameDuration = frameDuration;
	}

	void AddFrame(const cml::vector2i& frame)
	{
		m_frames.push_back(frame);
	}

	void Update(uint32_t delta, AnimData& animData)
	{
		if (animData.timer < GetAnimationDuration())
		{
			animData.timer += delta;
			if (animData.timer < GetAnimationDuration())
			{
				auto currentFrameIndex = animData.timer / m_frameDuration;
				animData.currentFrame = m_frames[currentFrameIndex];
			}
		}
		else if (m_loop)
		{
			animData.timer -= GetAnimationDuration();
		}
	}

private:
	uint32_t GetAnimationDuration()
	{
		return m_frames.size() * m_frameDuration;
	}

	std::vector<cml::vector2i> m_frames;
	uint32_t m_frameDuration;
	bool m_loop = true;

};

struct AnimationPack
{
public:
	void SetSideAnimation(Side side, std::shared_ptr<Animation> anim)
	{
		sideAnimations[size_t(side)] = anim;
	}

	void Step(uint32_t delta, AnimData& animData)
	{
		auto currentSide = animData.GetCurrentSide();
		if (currentSide != Side::None)
		{
			GetCurrentAnimation(currentSide)->Update(delta, animData);
		}
	}

private:

	const std::shared_ptr<Animation>& GetCurrentAnimation(Side side)
	{
		return sideAnimations[size_t(side)];
	}

	std::shared_ptr<Animation> sideAnimations[size_t(Side::Max)];

};




#pragma once

#include "entity.h"

class Pawn : public Entity
{

public:

	void SetRowInSpritesheet(int row)
	{
		framey = row;
	}

	// offset de ángulo y dirección de movimiento
	cml::vector2f rotation_offset, move_direction;

	void PushBack(int amount)
	{
		auto forward = GetForward();
		auto backward = cml::rotate_vector_2D(forward, M_PI);
		pushback = { backward[0] * amount, backward[1] * amount };
	}

	void ApplyLinearVelocity()
	{
		body->SetLinearVelocity(b2Vec2(move_direction[0] + pushback[0], move_direction[1] + pushback[1]));
	}

	cml::vector2f pushback;

};

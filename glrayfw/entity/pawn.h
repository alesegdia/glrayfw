

#pragma once

#include "entity.h"

class Pawn : public Entity
{

public:

	// offset de ángulo y dirección de movimiento
	cml::vector2f rotation_offset, move_direction;

	void ApplyLinearVelocity()
	{
		body->SetLinearVelocity(b2Vec2(move_direction[0],move_direction[1]));
	}

	void SetupFrame( const cml::vector3f& viewerPos ) {
		cml::vector3f actor2pl = transform.position - viewerPos;
		cml::vector3f rotactor = cml::rotate_vector( cml::vector3f(1,0,0), cml::vector3f(0,-1,0), cml::rad(180.f)+transform.logic_angle );
		float datAngle = 180 + cml::deg(cml::signed_angle_2D( cml::vector2f(actor2pl[0],actor2pl[2]), cml::vector2f(rotactor[0],rotactor[2]) ));
		//float deltangle = playerAngle - actor->GetAngleY();
		//while( deltangle < 0 ) deltangle += 360;
		int q = (((int)(datAngle+45)) % 360) / 90;
		int corr[] = { 2,1,3,0 };
		framex = corr[q]; framey = 2;
	}

};

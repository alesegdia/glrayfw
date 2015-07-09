#pragma once

#include "../render/sprite3d.h"
#include "../core/transform.h"
#include "../physics/layers.h"

#include <Box2D/Box2D.h>

class EntityController;

class Entity
{
public:
	enum Type
	{
		BULLET,
		MOB,
		PLAYER,
		PICKUP
	};

protected:

	Sprite3D* sprite;
	b2Body* body = NULL;

	bool isAlive;
	Entity::Type type;


public:

	Transform transform;
	EntityController* controller;
	int framex, framey;

	virtual void SetupFrame( const cml::vector3f& viewerPos ) {
		framex = 0; framey = 0;
	}

	virtual ~Entity();

	Entity()
	{
		isAlive = true;
		controller = NULL;
		type = Type::MOB;
		this->transform.entity = this;
	}

	const char* TypeString()
	{
		switch( type )
		{
			case Type::MOB: return "MOB";
			case Type::PLAYER: return "PLAYER";
			case Type::BULLET: return "BULLET";
		}
	}

	void ClearVelocity()
	{
		if( body ) {
			body->SetLinearVelocity(b2Vec2(0,0));
		}
	}

	void SetController( EntityController* controller );

	void Step( uint32_t delta );

	void SetType( Entity::Type t )
	{
		type = t;
	}

	Entity::Type GetType()
	{
		return type;
	}

	void CollisionEnter( Entity* other )
	{
		//printf("%p (%s): START COLLISION WITH %p (%s)\n", this, TypeString(), other, other->TypeString());
		//fflush(0);
	}

	void CollisionExit( Entity* other )
	{
		//printf("%p (%s): END COLLISION WITH %p (%s)\n", this, TypeString(), other, other->TypeString());
		//fflush(0);
	}

	void Cleanup()
	{
		if( body ) {
			body->GetWorld()->DestroyBody( body );
		}
	}

	void Die()
	{
		isAlive = false;
	}

	bool IsAlive()
	{
		return isAlive;
	}

	void SetPhysicBody( b2Body* body )
	{
		this->body = body;
		this->body->SetUserData(this);
	}

	b2Body* GetPhysicBody()
	{
		return body;
	}

	void SetSprite( Sprite3D* sprite )
	{
		this->sprite = sprite;
	}

	cml::matrix44f_c Model()
	{
		return transform.Model();
	}

	void SetAngleY( float angle )
	{
		transform.rotation[0] = angle;
	}
	
	void SetLogicAngleY( float angle )
	{
		transform.logic_angle = angle;
	}

	float GetAngleY()
	{
		return transform.rotation[0];
	}
	
	float GetQuadAngleY()
	{
		return transform.local_rotation[0];
	}
	
	void SetQuadAngleY( float angle )
	{
		transform.local_rotation[0] = angle;
	}


	void OffsetRotationY( float delta_angle )
	{
		transform.rotation[0] += delta_angle;
	}

	void Prepare( Sprite3D* sprite )
	{
		this->sprite = sprite;
	}

	Transform& GetTransform()
	{
		return transform;
	}

	void PhysicStep()
	{
		if( this->body ) {
			transform.position[0] = -this->body->GetPosition().x;
			transform.position[2] = -this->body->GetPosition().y;
		}
	}

	Sprite3D* GetSprite()
	{
		return sprite;
	}

};

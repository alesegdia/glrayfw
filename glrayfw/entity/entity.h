#pragma once

#include "glrayfw/render/sprite3d.h"
#include "glrayfw/core/transform.h"
#include "glrayfw/physics/layers.h"

#include <Box2D/Box2D.h>

#include <functional>

class EntityController;

class Entity
{
public:
	enum Type
	{
		BULLET,
		MOB,
		PLAYER,
		PICKUP,
		PORTAL,
		SPAWNER
	};

protected:

	Sprite3D* sprite;
	b2Body* body = NULL;

	bool isAlive;
	Entity::Type type;


public:

	std::string name = "UNSET";
	Transform transform;
	int framex, framey;
	std::function<void(Entity*)> onDie;
	std::shared_ptr<Animation> anim = nullptr;

	bool relativeToPlayerSprite = false;

	void SetupFrameAbsolute() {
		framex = 0;
		framey = 0;
	}

	void SetupFrameRelativeToPlayerSprite(const cml::vector3f& viewerPos) {
		cml::vector3f actor2pl = transform.position - viewerPos;
		cml::vector3f rotactor = cml::rotate_vector(cml::vector3f(1, 0, 0), cml::vector3f(0, -1, 0), cml::rad(180.f) + transform.logic_angle);
		float datAngle = 180 + cml::deg(cml::signed_angle_2D(cml::vector2f(actor2pl[0], actor2pl[2]), cml::vector2f(rotactor[0], rotactor[2])));
		int q = (((int)(datAngle + 45)) % 360) / 90;
		int corr[] = { 2,1,3,0 };
		framex = corr[q];
	}

	void SetupFrame(const cml::vector3f& viewerPos)
	{
		if (relativeToPlayerSprite)
		{
			SetupFrameRelativeToPlayerSprite(viewerPos);
		}
		else
		{
			SetupFrameAbsolute();
		}
	}

	virtual ~Entity();

	Entity()
	{
		isAlive = true;
		type = Type::MOB;
		this->transform.entity = this;
		framex = framey = 0;
		sprite = nullptr;
	}

	const char* TypeString()
	{
		switch( type )
		{
			case Type::MOB: return "MOB";
			case Type::PLAYER: return "PLAYER";
			case Type::BULLET: return "BULLET";
		default: return nullptr;
		}
	}

	void ClearVelocity()
	{
		if( body ) {
			body->SetLinearVelocity(b2Vec2(0,0));
		}
	}

	void AddController( const std::shared_ptr<EntityController>& controller );

	void Step( uint32_t delta );

	template <typename T>
	std::shared_ptr<T> GetController()
	{
		for (auto ctrl : controllers)
		{
			std::shared_ptr<T> ctrlDerived = std::dynamic_pointer_cast<T>(ctrl);
			if (ctrlDerived != nullptr)
			{
				return std::move(ctrlDerived);
			}
		}
		return nullptr;
	}

	void SetType( Entity::Type t )
	{
		type = t;
	}

	Entity::Type GetType()
	{
		return type;
	}

	virtual void CollisionEnter( Entity* )
	{

	}

	virtual void CollisionExit( Entity* )
	{

	}

	void Cleanup()
	{
		if( body ) {
			body->GetWorld()->DestroyBody( body );
		}
	}

	void Die()
	{
		if (onDie && isAlive)
		{
			onDie(this);
		}
		isAlive = false;
	}

	bool IsAlive()
	{
		return isAlive;
	}

	void SetPhysicBody( b2Body* body )
	{
		this->body = body;
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

	cml::vector2f GetForward()
	{
		return cml::rotate_vector_2D(cml::vector2f(0.f, 1.f), transform.logic_angle);
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

	void DoLogicAngleAdd(float angle)
	{
		transform.logic_angle += cml::rad(angle);
	}

private:
	std::vector<std::shared_ptr<EntityController>> controllers;


};

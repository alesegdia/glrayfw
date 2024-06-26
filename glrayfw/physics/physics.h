
#pragma once

#include <Box2D/Box2D.h>

#include "layers.h"
#include "glrayfw/core/random.h"
#include <cstdint>

class Player;


class AnyBodyInAreaQueryCallback : public b2QueryCallback
{
public:

	bool ReportFixture(b2Fixture* f)
	{
		m_anyBodyDetected = true;
		return true;
	}

	bool IsAnyBodyDetected()
	{
		return m_anyBodyDetected;
	}

private:
	bool m_anyBodyDetected = false;
};


class Physics
{

private:

	const float timeStep = 1.0f / 60.0f;
	const int32 velocityIterations = 6;
	const int32 positionIterations = 2;

	b2World* world;



public:

	static const uint16_t PLAYER_MASK = CollisionLayer::PICKUP | CollisionLayer::ENEMY_BULLET | CollisionLayer::MAP | CollisionLayer::ENEMY | CollisionLayer::PORTAL;
	static const uint16_t MOB_MASK = CollisionLayer::ALLY_BULLET | CollisionLayer::MAP | CollisionLayer::ENEMY | CollisionLayer::PLAYER;
	static const uint16_t ABULLET_MASK = CollisionLayer::MAP | CollisionLayer::ENEMY;
	static const uint16_t EBULLET_MASK = CollisionLayer::MAP | CollisionLayer::PLAYER; // | CollisionLayer::ALLY
	static const uint16_t PICKUP_MASK = CollisionLayer::PLAYER; // | CollisionLayer::ALLY
	static const uint16_t PORTAL_MASK = CollisionLayer::PLAYER; // | CollisionLayer::ALLY
	static const uint16_t ENEMY_UNDMG_MASK = CollisionLayer::PLAYER | CollisionLayer::ENEMY; // | CollisionLayer::ALLY

	~Physics()
	{
		Cleanup();
	}

    Physics(b2ContactListener* listener);
	void Cleanup();
	b2Body* CreateBulletBody( float x, float y, CollisionLayer category, uint16_t mask, uintptr_t userData);
	b2Body* CreateSphereBody( float x, float y, uintptr_t userData, CollisionLayer category = CollisionLayer::ENEMY, uint16_t mask = MOB_MASK, bool isDynamic = true );
	void AddCubeBody( float x, float y, uintptr_t userData);
	void Step(float factor);
	bool IsAnyBodyDetected(const b2AABB& area)
	{
		AnyBodyInAreaQueryCallback cb;
		world->QueryAABB(&cb, area);
		return cb.IsAnyBodyDetected();
	}

};

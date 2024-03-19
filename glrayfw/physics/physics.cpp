
#include "physics.h"
#include "layers.h"
#include <cstdint>

void Physics::Init( )
{
    world = new b2World(b2Vec2(0.f,0.f));
}

void Physics::Cleanup()
{
	delete world;
}

b2Body* Physics::CreateBulletBody( float x, float y, CollisionLayer category, uint16_t mask, uintptr_t userData)
{
	// BODY CONF
	b2BodyDef bodyDef;
	bodyDef.position.Set( x, y );
	bodyDef.bullet = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData.pointer = userData;
	b2Body* body = world->CreateBody(&bodyDef);

	// SHAPE CONF
	b2CircleShape shape;
	shape.m_p.Set(0,0);
	shape.m_radius = 0.2f;

	// FIXTURE CONF
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = category;
	fixtureDef.filter.maskBits = mask;
	body->CreateFixture( &fixtureDef );

	body->SetGravityScale(0);

	return body;
}

b2Body* Physics::CreateSphereBody( float x, float y, uintptr_t userData, CollisionLayer category, uint16_t mask )
{
	// BODY CONF
	b2BodyDef bodyDef;
	bodyDef.position.Set( x, y );
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData.pointer = userData;
	b2Body* body = world->CreateBody(&bodyDef);

	// SHAPE CONF
	b2CircleShape shape;
	shape.m_p.Set(0,0);
	shape.m_radius = 0.7f;

	// FIXTURE CONF
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.filter.categoryBits = category;
	fixtureDef.filter.maskBits = mask;
	body->CreateFixture( &fixtureDef );

	body->SetGravityScale(0);

	return body;
}

void Physics::AddCubeBody( float x, float y, uintptr_t userData)
{
	// BODY CONF
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set( x, y );
	bodyDef.userData.pointer = userData;
	b2Body* body = world->CreateBody(&bodyDef);

	// SHAPE CONF
	b2PolygonShape shape;
	shape.SetAsBox(1,1);

	// FIXTURE CONF
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.f;
	fixtureDef.filter.categoryBits = CollisionLayer::MAP;
	fixtureDef.filter.maskBits = CollisionLayer::ALLY_BULLET | CollisionLayer::ENEMY | CollisionLayer::ENEMY_BULLET | CollisionLayer::PLAYER;
	body->CreateFixture( &fixtureDef );

}

void Physics::Step()
{
	world->Step(timeStep, velocityIterations, positionIterations);
}

class MyQueryCallback : public b2QueryCallback
{
	public:
		static int numbodies;
		bool ReportFixture(b2Fixture*)
		{
			numbodies++;
			return true;
		}
};

int MyQueryCallback::numbodies = 0;

/*
void Physics::Stress(Player* p)
{
	MyQueryCallback::numbodies = 0;
	for( int i = 0; i < 100; i++ )
	{
		b2AABB aabb;
		float x, y;
		x = p->transform.position[0];
		y = p->transform.position[2];
		aabb.lowerBound = b2Vec2(-x,-y);
		aabb.upperBound = b2Vec2(-x+rng.uniform(100,500),-y+rng.uniform(100,500));
		MyQueryCallback mycb;
		world->QueryAABB( &mycb, aabb );
	}
	printf("KERI! %d\n", MyQueryCallback::numbodies);
}
*/

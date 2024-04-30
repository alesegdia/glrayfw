

#include "entity.h"
#include "controller/entitycontroller.h"

Entity::~Entity() {
	// if( controller ) delete controller;
}

void Entity::AddController( const std::shared_ptr<EntityController>& controller )
{
	controllers.push_back(controller);
}

void Entity::Step( uint32_t delta )
{
	if (animationPack != nullptr)
	{
		animationPack->Step(delta, animData);
		auto frame = animData.currentFrame;
		framex = frame[0];
		framey = frame[1];
		std::cout << framex << ", " << framey << std::endl;
	}
	for (auto& ctrl : controllers)
	{
		ctrl->DoStep(this, delta);
	}
}


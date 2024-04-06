

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
	for (auto& ctrl : controllers)
	{
		ctrl->DoStep(this, delta);
	}
}


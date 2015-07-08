

#include "entity.h"
#include "controller/entitycontroller.h"

Entity::~Entity() {
	if( controller ) delete controller;
}

void Entity::SetController( EntityController* controller )
{
	this->controller = controller;
}

void Entity::Step( uint32_t delta )
{
	if( this->controller )
	{
		this->controller->Step( this, delta );
	}
}


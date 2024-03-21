
#pragma once

#include <cstdint>

class Entity;

class EntityController
{

protected:


public:

	virtual ~EntityController() {}
	virtual void Step( Entity* e, uint32_t delta ) = 0 ;

};

template <typename T>
class TypedEntityController : public EntityController
{
public:
	virtual ~TypedEntityController()
	{

	}

	void Step(Entity* e, uint32_t delta) override
	{
		T* t = dynamic_cast<T*>(e);
		assert(t != nullptr);
		Step(t, delta);
	}

	virtual void Step(T* entity, uint32_t delta) = 0;

};

#pragma once

#include <cstdint>
#include <bitset>


class Entity;


class EntityController
{

protected:


public:

	virtual ~EntityController() {}

	virtual void Step(Entity* e, uint32_t delta) = 0;

	void DoStep(Entity* e, uint32_t delta)
	{
		if (m_dirty)
		{
			Awake(e);
			m_dirty = false;
		}
		Step(e, delta);
	}

	virtual void Awake( Entity* e )
	{

	}

private:
	bool m_dirty = true;
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
		auto derived = GetDerivedEntity(e);
		Step(derived, delta);
	}

	virtual void Step(T* entity, uint32_t delta) = 0;
	virtual void Awake(T* entity)
	{

	}

	virtual void Awake(Entity* e)
	{
		auto derived = GetDerivedEntity(e);
		Awake(derived);
	}

private:
	T* GetDerivedEntity(Entity* e)
	{
		T* t = dynamic_cast<T*>(e);
		assert(t != nullptr);
		return t;
	}

};
#pragma once

#include "../core/dynamicarray.h"

class Entity;

class EntityManager {
public:
	virtual ~EntityManager ();

	EntityManager();

	void AddEntity(Entity* e);
	void ClearDeadEntities();
	void ClearAllEntities();
	size_t NumEntities() { return entities.Size(); }
	Entity* GetEntity(int i) { return entities[i]; }
	void PreUpdate();

private:

	DynamicArray<Entity*> entities;
	std::vector<Entity*> dirtyEntities;

};


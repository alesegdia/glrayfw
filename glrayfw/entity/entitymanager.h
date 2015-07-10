#pragma once

#include "../core/dynamicarray.h"
#include "../render/renderer.h"

class Entity;

class EntityManager {
public:
	EntityManager ();
	virtual ~EntityManager ();

	void Prepare(Renderer* renderer);

	void RenderEntities( float playerAngle );
	void AddEntity(Entity* e);
	void ClearDeadEntities();
	void ClearAllEntities();
	size_t NumEntities() { return entities.Size(); }
	Entity* GetEntity(int i) { return entities[i]; }

private:

	Renderer* renderer;
	DynamicArray<Entity*> entities;

};


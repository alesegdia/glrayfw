
#include "entity.h"
#include "entitymanager.h"

EntityManager::~EntityManager() {
	ClearAllEntities();
}

EntityManager::EntityManager() {
}

void EntityManager::AddEntity(Entity* e) {
	this->entities.Add(e);
	this->dirtyEntities.push_back(e);
}

void EntityManager::PreUpdate()
{

}

void EntityManager::ClearDeadEntities() {
	DynamicArray<Entity*>& l = this->entities;
	for( size_t i = 0; i < l.Size(); i++ )
	{
		if( !l[i]->IsAlive() )
		{
			l[i]->Cleanup();
			//if( l[i]->controller != NULL ) delete l[i]->controller;
			//l[i]->GetPhysicBody()->GetWorld()->DestroyBody(l[i]->GetPhysicBody());
			delete l[i];
			l[i] = l[l.Size()-1];
			l.RemoveLast(); // dealloc!! se olvida?
			i--;
		}
	}
}

void EntityManager::ClearAllEntities() {

	for( size_t i = 0; i < entities.Size(); i++ )
	{
		if (entities[i])
		{
			delete entities[i];
			entities[i] = nullptr;
		}
	}
	entities.Clear();
}

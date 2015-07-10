
#include "entity.h"
#include "entitymanager.h"

EntityManager::EntityManager() {

}

EntityManager::~EntityManager() {

}

void EntityManager::Prepare( Renderer* renderer ) {
	this->renderer = renderer;
}

void EntityManager::AddEntity(Entity* e) {
	this->entities.Add(e);
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

void EntityManager::RenderEntities( float playerAngle ) {
	for( size_t i = 0; i < entities.Size(); i++ )
	{
		entities[i]->PhysicStep();
		entities[i]->ClearVelocity();
		entities[i]->SetAngleY( cml::rad(180 + playerAngle) );
		renderer->RenderEntity( entities[i] );
	}
}

void EntityManager::ClearAllEntities() {

	for( size_t i = 0; i < entities.Size(); i++ )
	{
		delete entities[i];
	}
}

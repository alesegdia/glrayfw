
#pragma once

#include <cml/cml.h>
#include "../core/unordereddynamicarray.h"

class Entity;

class Transform
{

public:

	float logic_angle = 0.f;
	UnorderedDynamicArray<Transform*> children;
	cml::vector3f position;
	cml::vector3f rotation;
	cml::vector3f local_position;
	cml::vector3f local_rotation;
	cml::matrix44f_c world;
	cml::matrix44f_c worldFinal;
	cml::matrix44f_c local;
	Entity* entity;

	void UpdateClean(bool force=false);
	Transform();
	~Transform()
	{
		children.Clear();
	}

	void Update( const Transform& parent, uint32_t delta );

	void AddChild( Transform* child );

	cml::matrix44f_c Model();

};

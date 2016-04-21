
#include "transform.h"
#include "../entity/entity.h"

void Transform::Update(const Transform& parent, uint32_t delta)
{
	cml::matrix44f_c worldtrans,
					 localrot,
					 invrot,
					 localtrans;

	worldtrans = localrot = localtrans = invrot = worldtrans = cml::identity<4>();

	// cml::vector3f loc = local_position;

	cml::matrix_rotation_euler( localrot, local_rotation[0], 0.f, 0.f, cml::EulerOrder::euler_order_yxz );
	cml::matrix_rotation_euler( invrot, -local_rotation[0], 0.f, 0.f, cml::EulerOrder::euler_order_yxz );
	// cml::matrix_rotation_euler( worldrot2, -rotation[0], 0.f, 0.f, cml::EulerOrder::euler_order_yxz );

	cml::matrix_set_translation( localtrans, local_position );
	cml::matrix_set_translation( worldtrans, position );

	world = parent.world * worldtrans * localrot * localtrans * invrot;

	cml::matrix44f_c worldrot = cml::identity<4>();
	cml::matrix_rotation_euler( worldrot, rotation[0], 0.f, 0.f, cml::EulerOrder::euler_order_yxz );
	worldFinal = world * worldrot;

	if( this->entity )
		this->entity->Step( delta );

	for( size_t i = 0; i < this->children.Size(); i++ )
	{
		// peta porque estamos actualizando un hijo que se ha eliminado (una bala probablemente, hija del nodo raiz)
		children[i]->Update(*this, delta);
	}
}

void Transform::UpdateClean(bool force)
{
	for( size_t i = 0; i < this->children.Size(); i++ )
	{
		if( force || !children[i]->entity->IsAlive() )
		{
			children[i]->UpdateClean(true);
			if( force && children[i]->entity != NULL )
			{
				children[i]->entity->Die();
			}
			children.Remove(i);
			i--;
		}
	}
}



Transform::Transform()
{
	world = local = cml::identity<4>();
	position = rotation = local_position = local_rotation = cml::vector3f(0,0,0);
	this->entity = NULL;
}

void Transform::AddChild(Transform* child)
{
	this->children.Add(child);
}

cml::matrix44f_c Transform::Model()
{
	return worldFinal;
}

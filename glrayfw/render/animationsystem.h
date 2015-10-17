#pragma once

#include "../entity/entitymanager.h"

class AnimationSystem {
public:
	AnimationSystem ( EntityManager* eMgr );
	virtual ~AnimationSystem ();

	void process()
	{
		for( size_t i = 0; i < eMgr->NumEntities(); i++ )
		{
			Entity* e = eMgr->GetEntity(i);
			if( e->HasAnimation() )
			{
				e->AnimationStep();
			}
		}
	}

private:
	/* data */
};


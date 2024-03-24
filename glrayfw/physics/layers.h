
#pragma once

enum CollisionLayer
{
	BOUNDARY		= 0x0001,
	ALLY_BULLET		= 0x0002,
	PLAYER			= 0x0004,
	ENEMY_BULLET	= 0x0008,
	ENEMY			= 0x0010,
	MAP				= 0x0020,
	PICKUP			= 0x0040,
	PORTAL			= 0x0080
};

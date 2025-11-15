#pragma once

//#define MASTER_SERVER "pmserver.ir"
#define MASTER_SERVER "sv1.plusmaster.pro"
#define BUILDNUMBER_STR "1"
#define BUILDNUMBER 1
#define MP
#define IW5

//#define NoAuth

typedef enum GameType
{
	GAME_TYPE_MP = 1,
	GAME_TYPE_SP = 2,
	GAME_TYPE_DEDI = 3,
	GAME_TYPE_ERROR = -1
};

class Global
{
public:
	class Game
	{
	public:
		static GameType	Type;
	};
};
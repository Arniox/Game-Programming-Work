#ifndef __USER_H__
#define __USER_H__

//Locals
#include "Player.h"

class User:
	public Player
{
public:
	User();
	~User();

	void CreateUser(IniParser* iniParser, BackBuffer* m_pBackBuffer, double x, double y);
	void ProcessUser(InputHandler* inputHandler, double deltaTime);
	
};

#endif


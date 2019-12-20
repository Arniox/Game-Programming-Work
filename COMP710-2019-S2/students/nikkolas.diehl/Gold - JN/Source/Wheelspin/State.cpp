#include "State.h"
#include "game.h"
#include "IniParser.h"

State::State()
{
	m_pInput = Game::GetInputManager();
}

State::~State()
{
}

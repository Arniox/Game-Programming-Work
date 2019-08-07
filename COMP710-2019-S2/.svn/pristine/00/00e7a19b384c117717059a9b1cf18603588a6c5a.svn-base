#include "PlayerShip.h"

PlayerShip::PlayerShip()
{
}
PlayerShip::~PlayerShip()
{
}

void 
PlayerShip::Process(float deltaTime, int screenWidth, int screenHeight, bool moveRight, bool moveLeft) {
	// SS04.4: Update player...
	float playerX = m_x;
	float playerY = m_y;

	if (moveLeft && !(playerX > (screenWidth - 35))) {
		m_x = playerX += 3;
	}
	if (moveRight && !(playerX < 10)) {
		m_x = playerX -= 3;
	}
}

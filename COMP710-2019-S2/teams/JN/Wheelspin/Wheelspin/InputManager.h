#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <SDL.h>
#include "InputDef.h"

#include <memory>

class Vector2;

class InputManager
{
public:
	InputManager();
	~InputManager();
	bool Initialise();
	static InputManager* GetInstance();

	void Proccess();
	void HandleAxis(const SDL_Event &event);
	void ConstrainAxis(Sint16 & axisValue, bool stick);
	bool MousePressed(MouseID id);
	bool KeyDown(int id);
	bool KeyUp(int id);
	bool JoyButtonDown(int id);
	bool JoyButtonUp(int id);
	bool JoyHatDown(int id);
	float LeftTriggerValue();
	float RightTriggerValue();
	float LeftJoyHorizValue();
	float LeftJoyVertValue();
	float RightJoyHorizValue();
	float RightJoyVertValue();
	bool JoyControl() const;

private:
	InputManager(const InputManager& InputManager);
	InputManager& operator=(const InputManager& InputManager);

public:
	int m_mouseX;
	int m_mouseY;

	std::unique_ptr<Vector2> GetMousePos();
	
private:
	const uint8_t* m_keyboard;
	uint32_t m_mouse;
	bool m_joyControlMode;
	bool m_mouseButtonsDown[MOUSE_SIZE];
	bool m_mouseButtonsUp[MOUSE_SIZE];
	bool m_keysDown[KEY_SIZE];
	bool m_keysUp[KEY_SIZE];
	bool m_joyButtonsDown[JOY_BUTTON_SIZE];
	bool m_joyButtonsUp[JOY_BUTTON_SIZE];
	bool m_joyHatDown[JOY_HAT_SIZE];
	Sint16 m_joyStickDeadZone;
	Sint16 m_triggerDeadZone;
	float m_triggerMax;
	Sint16 m_leftTrigger;
	Sint16 m_rightTrigger;
	Sint16 m_leftJoyHoriz;
	Sint16 m_leftJoyVert;
	Sint16 m_rightJoyHoriz;
	Sint16 m_rightJoyVert;

protected:
	SDL_Joystick* m_pGameController;
	static InputManager* sm_pInstance;
};

#endif // __INPUT_MANAGER_H__

//Locals
#include "InputManager.h"
#include "logmanager.h"
#include "game.h"
#include "IniParser.h"
#include "Vector2.h"

//Libraries
#include <string>

InputManager::InputManager()
: m_mouseX(0)
, m_mouseY(0)
, m_joyStickDeadZone(0)
, m_triggerDeadZone(0)
, m_leftTrigger(SDL_MIN_SINT16)
, m_rightTrigger(SDL_MIN_SINT16)
, m_leftJoyHoriz(0)
, m_leftJoyVert(0)
, m_rightJoyHoriz(0)
, m_rightJoyVert(0)
, m_triggerMax(0.0f)
, m_joyControlMode(false)
{
	int i;
	for (i = 0; i < MOUSE_SIZE; ++i)
	{
		m_mouseButtonsDown[i] = false;
		m_mouseButtonsUp[i] = false;
	}

	for (i = 0; i < KEY_SIZE; ++i)
	{
		m_keysDown[i] = false;
		m_keysUp[i] = false;
	}

	for (i = 0; i < JOY_BUTTON_SIZE; ++i)
	{
		m_joyButtonsDown[i] = false;
		m_joyButtonsUp[i] = false;
	}

	for (i = 0; i < JOY_HAT_SIZE; ++i)
	{
		m_joyHatDown[i] = false;
	}
	
	m_joyStickDeadZone = static_cast<Sint16>(Game::GetIniFile()->GetValueAsInt("Controls", "joyStickDeadZone"));
	m_triggerDeadZone = SDL_MIN_SINT16 + static_cast<Sint16>(Game::GetIniFile()->GetValueAsInt("Controls", "triggerDeadZone"));
	m_triggerMax = static_cast<float>((SDL_MAX_SINT16 * 2));
}

InputManager::~InputManager()
{

}

bool
InputManager::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);

	if (!m_pGameController)
	{
		LogManager::Debug("No controller detected!");
	}

	return (true);
}

void
InputManager::Proccess()
{
	int i;
	for (i = 0; i < MOUSE_SIZE; ++i)
	{
		m_mouseButtonsDown[i] = false;
		m_mouseButtonsUp[i] = false;
	}

	for (i = 0; i < KEY_SIZE; ++i)
	{
		m_keysDown[i] = false;
		m_keysUp[i] = false;
	}

	for (i = 0; i < JOY_BUTTON_SIZE; ++i)
	{
		m_joyButtonsDown[i] = false;
		m_joyButtonsUp[i] = false;
	}

	for (i = 0; i < JOY_HAT_SIZE; ++i)
	{
		m_joyHatDown[i] = false;
	}

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::GetInstance()->Quit();
			break;
		case SDL_KEYDOWN:
			this->m_keysDown[event.key.keysym.scancode] = true;
			m_joyControlMode = false;
			break;
		case SDL_KEYUP:
			this->m_keysUp[event.key.keysym.scancode] = true;
			break;
		case SDL_MOUSEMOTION:
			this->m_mouseX = event.motion.x;
			this->m_mouseY = event.motion.y;
			m_joyControlMode = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				this->m_mouseButtonsDown[MB1] = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				this->m_mouseButtonsDown[MB2] = true;
			m_joyControlMode = false;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				this->m_mouseButtonsUp[MB1] = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				this->m_mouseButtonsUp[MB2] = true;
			break;
		case SDL_JOYBUTTONDOWN:
			this->m_joyButtonsDown[event.jbutton.button] = true;
			m_joyControlMode = true;
			break;
		case SDL_JOYBUTTONUP:
			this->m_joyButtonsUp[event.jbutton.button] = true;
			m_joyControlMode = true;
			break;
		case SDL_JOYHATMOTION:
			this->m_joyHatDown[event.jhat.value] = true;
			m_joyControlMode = true;
			break;
		case SDL_JOYAXISMOTION:
			HandleAxis(event);
			m_joyControlMode = true;
			break;
		default:
			break;
		}
	}
}

void
InputManager::HandleAxis(const SDL_Event &event)
{
	Sint16 value = event.jaxis.value;

	switch (event.jaxis.axis)
	{
	case 0:
		ConstrainAxis(value, true);
		m_leftJoyHoriz = value;
		break;
	case 1:
		ConstrainAxis(value, true);
		m_leftJoyVert = value;
		break;
	case 2:
		ConstrainAxis(value, false);
		m_leftTrigger = value;
		break;
	case 3:
		ConstrainAxis(value, true);
		m_rightJoyHoriz = value;
		break;
	case 4:
		ConstrainAxis(value, true);
		m_rightJoyVert = value;
		break;
	case 5:
		ConstrainAxis(value, false);
		m_rightTrigger = value;
		break;
	default:
		break;
	}
}

void
InputManager::ConstrainAxis(Sint16 &axisValue, bool stick)
{
	if (stick)
	{
		if (!(axisValue > m_joyStickDeadZone || axisValue < -m_joyStickDeadZone))
			axisValue = 0;
		else if (axisValue == SDL_MIN_SINT16)
			axisValue = SDL_MIN_SINT16 + 1;
	}
	else
	{
		if (!(axisValue > m_triggerDeadZone))
			axisValue = SDL_MIN_SINT16;
	}
}

bool
InputManager::MousePressed(MouseID id)
{
	return this->m_mouseButtonsDown[id];
}

bool
InputManager::KeyDown(int id)
{
	if (id > KEY_SIZE || id < 0)
		return false;
	return m_keysDown[id];
}

bool
InputManager::KeyUp(int id)
{
	if (id > KEY_SIZE || id < 0)
		return false;
	return m_keysUp[id];
}

bool
InputManager::JoyButtonDown(int id)
{
	if (id > JOY_BUTTON_SIZE || id < 0)
		return false;
	return m_joyButtonsDown[id];
}

bool
InputManager::JoyButtonUp(int id)
{
	if (id > JOY_BUTTON_SIZE || id < 0)
		return false;
	return m_joyButtonsUp[id];
}

bool
InputManager::JoyHatDown(int id)
{
	if (id > JOY_HAT_SIZE || id < 0)
		return false;
	return m_joyHatDown[id];
}

float
InputManager::LeftTriggerValue()
{
	if (m_leftTrigger != SDL_MIN_SINT16)
	{
		return static_cast<float>(m_leftTrigger + static_cast<float>(SDL_MAX_SINT16)) / m_triggerMax;
	}
	return 0.0f;
	
}

float
InputManager::RightTriggerValue()
{
	if (m_rightTrigger != SDL_MIN_SINT16)
	{
		return static_cast<float>(m_rightTrigger + static_cast<float>(SDL_MAX_SINT16)) / m_triggerMax;
	}
	return 0.0f;
}

float
InputManager::LeftJoyHorizValue()
{
	if (m_leftJoyHoriz != 0)
	{
		return static_cast<float>(static_cast<float>(m_leftJoyHoriz) / static_cast<float>(SDL_MAX_SINT16));
	}
	return 0.0f;
}

float
InputManager::LeftJoyVertValue()
{
	if (m_leftJoyVert != 0)
	{
		return static_cast<float>(static_cast<float>(m_leftJoyVert) / static_cast<float>(SDL_MAX_SINT16));
	}
	return 0.0f;
}

float
InputManager::RightJoyHorizValue()
{
	if (m_rightJoyHoriz != 0)
	{
		return static_cast<float>(static_cast<float>(m_rightJoyHoriz) / static_cast<float>(SDL_MAX_SINT16));
	}
	return 0.0f;
}

float
InputManager::RightJoyVertValue()
{
	if (m_rightJoyVert != 0)
	{
		return static_cast<float>(static_cast<float>(m_rightJoyVert) / static_cast<float>(SDL_MAX_SINT16));
	}
	return 0.0f;
}

bool InputManager::JoyControl() const
{
	return m_joyControlMode;
}

std::unique_ptr<Vector2> 
InputManager::GetMousePos()
{
	std::unique_ptr<Vector2> mousePos = std::make_unique<Vector2>();

	mousePos->x = m_mouseX;
	mousePos->y = m_mouseY;

	return mousePos;
}

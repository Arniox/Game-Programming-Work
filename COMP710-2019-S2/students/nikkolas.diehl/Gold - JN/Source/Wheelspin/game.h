// COMP710 GP 2D Framework 2019
#ifndef __GAME_H__
#define __GAME_H__

// Forward Declarations
class BackBuffer;
class InputManager;
class IniParser;
class SceneManager;
class FontManager;
class SoundManager;

class Camera;

class Game
{
	//Member Methods:
public:
	static Game* GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	static BackBuffer* GetBackBuffer();
	static InputManager* GetInputManager();
	static SceneManager* GetSceneManager();
	static IniParser* GetIniFile();
	static FontManager* GetFontManager();
	static SoundManager* GetSoundManager();
	static Camera* GetCamera();

	static int GetScreenWidth();
	static int GetScreenHeight();
	static int GetFPS();

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();
	
	// Member Data :
protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputManager* m_pInputManager;
	SceneManager* m_pSceneManager;
	IniParser* m_pIniFile;
	FontManager* m_pFontManager;
	SoundManager* m_pSoundManager;
	Camera* m_pCamera;

	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	__int64 m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
	int m_totalFramesPassed;
	
private:
	int m_screenWidth;
	int m_screenHeight;
	bool m_fullScreen;
};

#endif // __GAME_H__

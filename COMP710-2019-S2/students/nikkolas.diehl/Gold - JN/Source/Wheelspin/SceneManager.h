#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

//Forward Declerations
class GUI;

//Locals
#include "State.h"
#include "fmod.hpp"

//Libraries
#include <vector>

class BackBuffer;
class IniParser;
class FontManager;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Draw and process
	void Initialise(SCENES startScene);

	void Draw(BackBuffer& backBuffer);
	void Process(float deltaTime);

	//Change state functionality
	void ChangeState();
	void SetNextState(SCENES scene);
	void ProcessControls();
	
private:
	void InitFMOD(IniParser* pIniParser);
	void UnloadFMOD();

private:
	//Reference copies
	IniParser* m_pIniFile;
	BackBuffer* m_pBackBuffer;
	State* m_pCurrentState;
	FontManager* m_pFontManager;
	GUI* m_pStatsGUI;

	SCENES m_sceneID;
	SCENES m_nextSceneID;	
};

#endif


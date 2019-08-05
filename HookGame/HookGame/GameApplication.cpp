#include "GameApplication.h"

#include "HookGame.h"
#include "HookUIState.h"
#include "HookGameMainMenuState.h"

#include <SpelSylt/FileHandling/ConfigReader.h>

using namespace SpelSylt;

void CGameApplication::SetUpWindow()
{
	CConfigReader CfgReader;
	CfgReader.ReadConfigFile("HookGame.cfg");

	const unsigned int WindowW = CfgReader.GetAsUint("WinW");
	const unsigned int WindowH = CfgReader.GetAsUint("WinH");

	CApplication::SetWindowTitle("Game");
	CApplication::CreateWindow(WindowW, WindowH);
}

void CGameApplication::PushStartUpStates()
{
	CApplication::PushState(std::make_shared<HookGameMainMenuState>(1600, 900));
}
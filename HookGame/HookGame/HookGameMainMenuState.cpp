#include "HookGameMainMenuState.h"

#include <SpelSylt/UI/Button.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/Contexts/GameContext.h>

#include "GameMessages.h"
#include "HookGame.h"

HookGameMainMenuState::HookGameMainMenuState(unsigned int aWidth, unsigned int aHeight) :
	UI::UIState(aWidth, aHeight, "UI/Layouts/hookGameMenu_layout.xml")
{
	UIState::SetStateFlags(StateFlags::NONE);
}

void HookGameMainMenuState::OnInit(SGameContext & InGameContext)
{
	if (auto btn = myLayout->GetButton("start_game_btn")) {
		btn->SetCallback([this, &InGameContext](UI::Button& button) {
			InGameContext.MessageQueue.DispatchEvent<StartGame>();
			CState::PopAndPushNew(std::make_shared<HookGame>());
		});
	}
	if (auto btn = myLayout->GetButton("quit_game_btn")) {
		btn->SetCallback([this, &InGameContext](UI::Button& button) {
			CState::PopAll();
		});
	}
}

void HookGameMainMenuState::OnUpdate(SGameContext & InGameContext)
{
}

void HookGameMainMenuState::OnRender(sf::RenderTarget & InTarget)
{
}

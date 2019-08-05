#pragma once
#include <SpelSylt/UI/Base/UIState.h>

class HookGameMainMenuState : public UI::UIState
{
public:
	HookGameMainMenuState(unsigned int aWidth, unsigned int aHeight);
	virtual ~HookGameMainMenuState() = default;

protected:
	virtual void OnInit(SGameContext& InGameContext) override;
	virtual void OnUpdate(SGameContext& InGameContext) override;
	virtual void OnRender(sf::RenderTarget& InTarget) override;
};
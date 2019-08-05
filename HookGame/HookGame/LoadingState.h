#pragma once
#include <SpelSylt/UI/Base/UIState.h>
#include <SpelSylt/Utility/TimedEvent.h>

#include <SpelSylt/Rendering/Sprite/Sprite.h>
#include <SpelSylt/Rendering/Sprite/Texture.h>

class LoadingState : public UI::UIState
{
public:
	LoadingState(unsigned width, unsigned height);

	virtual void OnInit(SGameContext& InGameContext) override;
	virtual void OnUpdate(SGameContext& InGameContext) override;
	virtual void OnRender(sf::RenderTarget& InTarget) override;
private:
	float Rotation;
	SS::CTexture Texture;
	SS::CSprite Sprite;

	CTimedEvent myLoadingTimer;
};
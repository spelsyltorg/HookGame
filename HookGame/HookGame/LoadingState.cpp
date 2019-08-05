#include "LoadingState.h"
#include "HookGame.h"
#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Utility/Time/Time.h>

#include <SpelSylt/FileHandling/Asset/AssetTypes/TextureAsset.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>

LoadingState::LoadingState(unsigned width, unsigned height) : 
	UI::UIState(width, height, "UI/Layouts/loading_layout.xml")
	, Rotation(0.f)
{
	CState::SetStateFlags(StateFlags::NONE);
}

void LoadingState::OnInit(SGameContext & InGameContext)
{
	Texture = InGameContext.AssetManager.GetAsset<STextureAsset>("Graphics/Sprites/LoadingSprite.png", Synchronous);
	Sprite.SetTextureAsset(Texture.Get());
	Sprite.setScale(0.25f, 0.25f);
	Sprite.setOrigin((float)Texture.Get().GetSize().x * 0.5f, (float)Texture.Get().GetSize().y * 0.5f);
	Sprite.setPosition(1500.f, 800.f);
}

void LoadingState::OnUpdate(SGameContext & InGameContext)
{
	if (InGameContext.AssetManager.GetAsset<STextureAsset>("Graphics/Sprites/Floor.png").GetLoadStatus() == ELoadRequestStatus::Done)
	{
		CState::Pop();
	}
	
	Rotation += 5000.f * InGameContext.Time.GetDeltaTime();
	Sprite.setRotation(Rotation);

	myLoadingTimer.Update(InGameContext.Time.GetDeltaTime());
}

void LoadingState::OnRender(sf::RenderTarget & InTarget)
{
	InTarget.draw(Sprite);
}

#include "Game/HookGame.h"
#include "SFML/Graphics/RenderWindow.hpp"

#include "SpelSylt/Math/CommonMath.h"
#include "SpelSylt/Utility/Input/InputManager.h"

#include "SpelSylt/Rendering/RenderQueue.h"

#include <iostream>

#include <SpelSylt/FileHandling/Asset/Assets.h>
#include <SpelSylt/FileHandling/Asset/AssetManager.h>

#include <SpelSylt/Contexts/GameContext.h>
#include <SpelSylt/Contexts/RenderingContext.h>
#include <SpelSylt/Messaging/MessageQueue.h>
#include <SpelSylt/FileHandling/Loading/LoaderInterface.h>
#include <SpelSylt/Utility/Time/TimeGetter.h>

#include "SpelSylt/Messaging/Messages/AudioMessages.h"

#include "LoadingState.h"

HookGame::HookGame()
	: myIsGrounded(true)
{
}

HookGame::~HookGame()
{
}

using namespace SpelSylt;

void HookGame::OnInit(SGameContext& InGameContext)
{
	CState::Push(std::make_shared<LoadingState>(1600.f, 900.f));

	myPlayerPos = sf::Vector2f(800.f, 900.f);

	for (int i = 0; i < 100; ++i)
	{
		myHookPoints.emplace_back(sf::Vector2f(200.f + (float)(rand() % 1200), 500.f - i * 75.f));
	}

	InGameContext.MessageQueue.DispatchEvent<SMusicMessage>("HookGameOst", false);

	SS::CAssetManager& AssetManager = InGameContext.AssetManager;

	PlayerStandingAnimation = AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/CharacterStanding.anmbndl");
	PlayerSwingingAnimation = AssetManager.GetAsset<SS::SAnimationAsset>("Graphics/Animations/CharacterSwinging.anmbndl");
	myPlayerTexture = AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/Player.png");
	myPlayerHookedTexture = AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/Player_hooked.png");
	myRopeTexture = AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/Rope.png");
	myRopeTexture.Get().setRepeated(true);
	myHookPointTexture = AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/Hookpoint.png");
	myFloorTexture = AssetManager.GetAsset<SS::STextureAsset>("Graphics/Sprites/Floor.png");
	myFloorTexture.Get().setRepeated(true);

	myRope.SetTextureAsset(myRopeTexture.Get());

}

void HookGame::OnUpdate(SGameContext& InGameContext)
{
	const float dt = InGameContext.Time.GetDeltaTime();
	PlayerStandingAnimation.Tick(dt);
	PlayerSwingingAnimation.Tick(dt);

	myRope.setOrigin(myRopeTexture.Get().GetSize().x * 0.5f, 0.f);
	myPlayer.setOrigin(myPlayerTexture.Get().GetSize().x * 0.5f, myPlayerTexture.Get().GetSize().y * 1.f);

	Anchor = myPlayerPos;
	Anchor.y -= 100.f;

	if (!myIsHooked)
	{
		for (auto point : myHookPoints)
		{
			if (point.y <= Anchor.y)
			{
				if (Math::Length(myHookPoint - Anchor) > Math::Length(point - Anchor))
					myHookPoint = point;
			}
		}
	}

	auto& im = InGameContext.Input;
	if (im.IsKeyPressed(EKeyCode::Up))
	{
		if (!myIsHooked)
		{
			InGameContext.MessageQueue.DispatchEvent<SSoundMessage>("HookNoise");
			myRopeLength = Math::Length(myHookPoint - Anchor);
			myRotation = myTargetRotation;
		}
		myIsHooked = !myIsHooked;
	}

	myVelocity.y += 2500.f * dt;

	if ((myIsGrounded || myIsHooked) && im.IsKeyPressed(EKeyCode::Space))
	{
		myVelocity.y = -1000.f;
		myIsHooked = false;
	}

	if (!myIsHooked)
	{
		if (im.IsKeyDown(EKeyCode::Left))
			myVelocity.x = -200.f;
		if (im.IsKeyDown(EKeyCode::Right))
			myVelocity.x = 200.f;
		if (myIsGrounded && !im.IsKeyDown(EKeyCode::Left) && !im.IsKeyDown(EKeyCode::Right))
			myVelocity.x = 0.f;

		myPlayerPos += myVelocity * dt;
		if (!myIsGrounded)
		{
			myRotation += (myVelocity.x < 0.f ? 1.f : -1.f) * 720.f * InGameContext.Time.GetDeltaTime();
		}
		else
		{
			myRotation = 0.f;
			myTargetRotation = 0.f;
		}
	}
	else
	{
		sf::Vector2f p = Anchor + myVelocity * dt;
		if (Math::Length(p - myHookPoint) > myRopeLength)
		{
			sf::Vector2f restrainedP = (myHookPoint + myRopeLength * Math::GetNormalized(p - myHookPoint));
			myVelocity = (restrainedP - Anchor) / dt;
			myPlayerPos = restrainedP;
			myPlayerPos.y += 100.f;
		}
		else
		{
			myPlayerPos += myVelocity * dt;
		}
	}

	myPlayerPos.y = Math::Min(myPlayerPos.y, 900.f);
	myIsGrounded = myPlayerPos.y == 900.f;

	myPlayer.setPosition(myPlayerPos);
	
	sf::Vector2f pToAnchor = Anchor - myHookPoint;
	myRotation = Math::ToDegrees(atan2f(-pToAnchor.x, pToAnchor.y));;

	sf::Vector2f PlayerStandingAnimFloatSize(PlayerStandingAnimation.GetFrameSize());
	PlayerStandingAnimation.setOrigin(PlayerStandingAnimFloatSize.x * 0.5f, 0.f);//PlayerStandingAnimFloatSize.y);
	PlayerStandingAnimation.setRotation(myRotation);

	sf::Vector2f PlayerSwingingAnimFloatSize(PlayerSwingingAnimation.GetFrameSize());
	PlayerSwingingAnimation.setOrigin(PlayerSwingingAnimFloatSize.x * 0.5f, 0.f);// PlayerSwingingAnimFloatSize.y);
	PlayerSwingingAnimation.setRotation(myRotation);
	
	PlayerStandingAnimation.setPosition(Anchor);
	PlayerSwingingAnimation.setPosition(Anchor);

	if (myVelocity.x < 0.f)
	{
		PlayerSwingingAnimation.setScale(-1.f, 1.f);
	}
	else
	{
		PlayerSwingingAnimation.setScale(1.f, 1.f);
	}

	GetCamera().setCenter(myPlayerPos.x, myPlayerPos.y);
}

void HookGame::OnRender(CRenderQueue& InRenderQueue)
{
	sf::Vector2f pToAnchor = Anchor - myHookPoint;
	myRope.setPosition(myHookPoint);
	float rot = Math::ToDegrees(atan2f(-pToAnchor.x, pToAnchor.y));
	//if (myIsHooked) myTargetRotation = -rot;
	myRope.setRotation(rot);
	myRope.setTextureRect(sf::IntRect(0, 0, myRopeTexture.Get().GetSize().x, 10 + (Math::Length(pToAnchor) / myRopeTexture.Get().GetSize().y) * (myRopeTexture.Get().GetSize().y)));
	
	//SS::CTexture texture(myIsHooked ? myPlayerHookedTexture : myPlayerTexture);
	//myPlayer.SetTextureAsset(texture.Get());
	//myPlayer.setOrigin(texture.Get().GetSize().x * 0.5f, myPlayer.getOrigin().y);
	//myPlayer.setRotation(myRotation);
	
	SS::CSprite floor;
	floor.setPosition(-1000.f, 900.f);
	floor.SetTextureAsset(myFloorTexture.Get());
	floor.setTextureRect({ 0, 0, 5000, 1000 });
	InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(floor));
	if (myIsHooked) InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(myRope));
	for (auto hookPoint : myHookPoints)
	{
		SS::CSprite s;
		s.setOrigin(myHookPointTexture.Get().GetSize().x * 0.5f, myHookPointTexture.Get().getSize().y * 0.5f);
		s.SetTextureAsset(myHookPointTexture.Get());
		s.setPosition(hookPoint);
		InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteRenderCommand(s));
	}
	
	if (!myIsHooked)
	{
		InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteAnimationRenderCommand(PlayerStandingAnimation));
	}
	else
	{
		InRenderQueue.Enqueue(ERenderLayer::Game, SSpriteAnimationRenderCommand(PlayerSwingingAnimation));
	}
}

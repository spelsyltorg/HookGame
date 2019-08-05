#pragma once
#include "SpelSylt/GameState2D.h"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/View.hpp"

#include "SpelSylt/Rendering/Sprite/Texture.h"
#include "SpelSylt/Rendering/Animation/SpriteAnimation.h"

namespace sf {
	class RenderTarget;
}

namespace SpelSylt {
	class CRenderQueue;
}


class HookGame : public SpelSylt::GameState2D
{
public:
	HookGame();
	~HookGame();

	//Begin CState
	virtual void OnInit(SpelSylt::SGameContext& InGameContext) override;
	virtual void OnUpdate(SpelSylt::SGameContext& InGameContext) override;
	virtual void OnRender(SpelSylt::CRenderQueue& InRenderQueue) override;
	//End CState
private:

	sf::Vector2f myPlayerPos;
	sf::Vector2f myVelocity;
	bool myIsGrounded;
	bool myIsHooked = false;
	float myRopeLength = 300.f;

	std::vector<sf::Vector2f> myHookPoints;
	sf::Vector2f myHookPoint;
	sf::Vector2f Anchor;

	float myRotation = 0.f;
	float myTargetRotation = 0.f;

	SS::CSprite myPlayer;
	SS::CSprite myRope;

	SS::CSpriteAnimation PlayerStandingAnimation;
	SS::CSpriteAnimation PlayerSwingingAnimation;

	SS::CTexture myPlayerTexture;
	SS::CTexture myPlayerHookedTexture;
	SS::CTexture myRopeTexture;
	SS::CTexture myHookPointTexture;
	SS::CTexture myFloorTexture;
};

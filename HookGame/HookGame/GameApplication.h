#pragma once
#include <SpelSylt/Application.h>

#include <SpelSylt/Messaging/Subscribing/Subscriptions.h>

class CGameApplication final
	: public SpelSylt::CApplication
{
private:
	virtual void SetUpWindow() override;
	virtual void PushStartUpStates() override;

	SS::CSubscriptions Subs;
};
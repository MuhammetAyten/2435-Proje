#pragma once
#include "stdafx.h"

class Discord {
private:
	int64_t StartTime;
	DiscordEventHandlers handlers;
	DiscordRichPresence discordPresence;
public:
	Discord();
	void Initialize();
	void Initialize(std::string DiscordTokenID);
	void Update(bool inGame = false);
};

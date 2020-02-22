#pragma once
#include "discord.h"
#include <chrono>
#include <SFML/System.hpp>
discord::Core* core{};

void discRes(discord::Result res) {
	if (res != discord::Result::Ok)
	{
		printf("discord error : %i \n", res);
	}
}

class Disc
{
public:
	Disc();
	sf::Time chrono;
	sf::Clock clock;
	std::string activitystr = "doing nothing";

	discord::Activity activity{};
	void init() {
		auto result = core->Create(680426302049615894, DiscordCreateFlags_Default, &core);
		activity.SetType(discord::ActivityType::Playing);
		activity.SetState("playing speed lazer");
		activity.SetDetails(activitystr.c_str());

		activity.GetAssets().SetLargeImage("speedlazerlogo");
		core->ActivityManager().UpdateActivity(activity, &discRes);
		clock.restart();
	}
	void update() {
		::core->RunCallbacks();
		if ((int)clock.getElapsedTime().asSeconds() % 20 == 1)
		{
			activity.SetType(discord::ActivityType::Playing);
			activity.SetState("playing speed lazer");
			activity.SetDetails(activitystr.c_str());
			core->ActivityManager().UpdateActivity(activity, &discRes);
		}
	}
	~Disc();

private:
};

Disc::Disc()
{
}

Disc::~Disc()
{
}

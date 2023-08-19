#include "Discord.h"

void Discord::Shutdown()
{
    Discord_Shutdown();
    DiscordInit = false;
}

void Discord::Init()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("574280916214480896", &Handle, 1, NULL);
    DiscordInit = true;
}

void Discord::Update(std::string state, std::string details, int64_t time, std::string LImage, std::string SImage, std::string TLImage, std::string TSImage)
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = state.c_str();
    discordPresence.details = details.c_str();
    discordPresence.startTimestamp = time;
    discordPresence.endTimestamp = NULL;
    discordPresence.largeImageKey = LImage.c_str();
    discordPresence.largeImageText = TLImage.c_str();
    discordPresence.smallImageKey = SImage.c_str();
    discordPresence.smallImageText = TSImage.c_str();
    Discord_UpdatePresence(&discordPresence);
}
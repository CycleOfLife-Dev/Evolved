#pragma once
#include <discord_register.h>
#include <discord_rpc.h>
#include <Windows.h>
#include <chrono>
#include <string>

static bool DiscordInit = false;

class Discord {
public:
    void Init();
    void Shutdown();
    void Update(std::string state, std::string details, int64_t time, std::string LImage = "", std::string SImage = "", std::string TLImage = "", std::string  TSImage = "");
};

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

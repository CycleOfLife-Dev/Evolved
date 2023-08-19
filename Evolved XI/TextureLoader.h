#pragma once
#include "Types.h"
#include "XML/tinyxml.h"

#pragma region LoadFiles
PDIRECT3DTEXTURE9 Texture(std::string filename)
{

	return ImageDrawList.find(filename)->second;
}

PDIRECT3DTEXTURE9 ClubsTexture(std::string filename)
{

	return ClanDrawList.find(filename)->second;
}

PDIRECT3DTEXTURE9 MapsTexture(std::string filename)
{

	return MapTexture.find(filename)->second;
}

PDIRECT3DTEXTURE9 ModesTexture(std::string filename)
{

	return ModeTexture.find(filename)->second;
}

PDIRECT3DTEXTURE9 CPlayerListTexture(std::string filename, int Type)
{
	if (Type == 1)
	{
		auto it = PlayerRankTexture.find(filename);
		if (it != PlayerRankTexture.end())
			return it->second;
	}
	else if (Type == 2)
	{
		auto it = PlayerNameTagTexture.find(filename);
		if (it != PlayerNameTagTexture.end())
			return it->second;
	}
	else if (Type == 3)
	{
		auto it = PlayerClanRankIconTexture.find(filename);
		if (it != PlayerClanRankIconTexture.end())
			return it->second;
	}

	auto it = PlayerLevelTexture.find(filename);
	if (it != PlayerLevelTexture.end())
		return it->second;

	return NULL;
}

bool FileExistsA(const char* szFile)
{
	return (GetFileAttributesA(szFile) != INVALID_FILE_ATTRIBUTES);
}

void LoadResourceFiles()
{
	if (!FileExistsA("UI\\Resources.cfg"))
	{
		HANDLE explorer;
		explorer = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
		TerminateProcess(explorer, 1);
		return;
	}

	TiXmlDocument doc("UI\\Resources.cfg");

	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);

	TiXmlNode* it = hDoc.FirstChild("GUI").ToNode()->FirstChild()->ToElement();

	for (it; it; it = it->NextSiblingElement())
	{
		//printf("Texture Name %s\n", it->ToElement()[0].Attribute("Name"));

		if (it->ValueTStr() == "UI")
			LoadUIResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Level")
			LoadLevelsResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Rank")
			LoadRankResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Clan")
			LoadClubesource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Item")
			LoadItemsResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Map")
			MapResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "Mode")
			ModeResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));		
		if (it->ValueTStr() == "ClubIconRank")
			LoadClubRankIconResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "GameMode")
			LoadGameModeResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
		if (it->ValueTStr() == "ActivatedGameMode")
			LoadActivatedGameModeResource.insert(std::pair<std::string, std::string>(it->ToElement()[0].Attribute("Name"), it->ToElement()[0].Attribute("ResName")));
	}
}

int StdToInt(std::string str)
{
	if (str.empty())
		return 0;
	return std::atoi(str.c_str());
}

void LoadMapInfo()
{
	if (!FileExistsA("UI\\MapInfo.cfg"))
	{
		ExitProcess(0);
	}

	TiXmlDocument doc("UI\\MapInfo.cfg");

	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);

	TiXmlNode* it = hDoc.FirstChild("Room").ToNode()->FirstChild()->ToElement();

	for (it; it; it = it->NextSiblingElement())
	{
		MapInfoStruct* mapinfo = new MapInfoStruct();
		mapinfo->Name = it->ToElement()[0].Attribute("MapName");
		mapinfo->Id = StdToInt(it->ToElement()[0].Attribute("GameMode"));

		PDIRECT3DTEXTURE9 imgtexture = NULL;
		std::string texture = it->ToElement()[0].Attribute("Texture");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + texture).c_str(), &imgtexture);

		mapinfo->Texture = imgtexture;

		PDIRECT3DTEXTURE9 ActivatedTimgtexture = NULL;
		std::string activatedtexture = it->ToElement()[0].Attribute("Activated");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + activatedtexture).c_str(), &ActivatedTimgtexture);

		mapinfo->ActivatedTexture = ActivatedTimgtexture;


		PDIRECT3DTEXTURE9 hover = NULL;

		std::string hovertexture = it->ToElement()[0].Attribute("Hover");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + hovertexture).c_str(), &hover);

		mapinfo->HoverTexture = hover;

		MapInfo.insert(std::pair<unsigned int, MapInfoStruct*>(StdToInt(it->ToElement()[0].Attribute("MapId")), mapinfo));
	}
}

void LoadGameModeInfo()
{
	if (!FileExistsA("UI\\GameModeInfo.cfg"))
	{
		ExitProcess(0);
	}

	TiXmlDocument doc("UI\\GameModeInfo.cfg");

	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);

	TiXmlNode* it = hDoc.FirstChild("Mode").ToNode()->FirstChild()->ToElement();

	for (it; it; it = it->NextSiblingElement())
	{
		GameModeInfoStruct* gamemode = new GameModeInfoStruct();
		gamemode->Name = it->ToElement()[0].Attribute("Name");

		PDIRECT3DTEXTURE9 imgtexture = NULL;
		std::string texture = it->ToElement()[0].Attribute("Texture");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + texture).c_str(), &imgtexture);

		gamemode->Texture = imgtexture;


		PDIRECT3DTEXTURE9 ActivatedTimgtexture = NULL;
		std::string activatedtexture = it->ToElement()[0].Attribute("Activated");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + activatedtexture).c_str(), &ActivatedTimgtexture);

		gamemode->ActivatedTexture = ActivatedTimgtexture;

		PDIRECT3DTEXTURE9 hover = NULL;

		std::string hovertexture = it->ToElement()[0].Attribute("Hover");
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + hovertexture).c_str(), &hover);

		gamemode->HoverTexture = hover;

		GameModesInfo.insert(std::pair<unsigned int, GameModeInfoStruct*>(StdToInt(it->ToElement()[0].Attribute("Id")), gamemode));
	}
}

PDIRECT3DTEXTURE9 LoadTextrueEx(std::string filename)
{
	if (LoadUIResource.size() <= 0 || LoadItemsResource.size() <= 0)
	{
		HANDLE explorer;
		explorer = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
		TerminateProcess(explorer, 1);
	}

	auto Imgit = ImageDrawList.find(filename);
	if (Imgit != ImageDrawList.end())
		return Imgit->second;

	PDIRECT3DTEXTURE9 imgtexture = NULL;
	std::string cyptedfilename;

	auto Resource = LoadUIResource.find(filename);
	if (Resource != LoadUIResource.end())
		cyptedfilename = Resource->second;

	if (cyptedfilename.empty() || cyptedfilename == "")
	{
		Resource = LoadItemsResource.find(filename);
		if (Resource != LoadItemsResource.end())
			cyptedfilename = Resource->second;
	}

	if (cyptedfilename.empty() || cyptedfilename == "")
		return NULL;

	D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);

	ImageDrawList.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));

	return imgtexture;
}

void LoadTextrue(std::string filename, std::string cyptedfilename, int ListCount)
{
	PDIRECT3DTEXTURE9 imgtexture = NULL;

	if (ListCount == 0)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);

		ClanDrawList.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 1)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		ImageDrawList.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 2)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		PlayerLevelTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 3)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		PlayerRankTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 4)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		PlayerNameTagTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 5)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		MapTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 6)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		ModeTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 7)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		PlayerClanRankIconTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	/*else if (ListCount == 8)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		GameModeTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}
	else if (ListCount == 9)
	{
		D3DXCreateTextureFromFileA(pD3DDevice, ("UI\\" + cyptedfilename).c_str(), &imgtexture);
		ActivatedGameModeTexture.insert(std::pair<std::string, PDIRECT3DTEXTURE9>(filename, imgtexture));
	}*/
}

ImFont* LoadFontTextrue(std::string filename, float size)
{
	LegionIO& io = Legion::GetIO();
	ImFont* font = NULL;

	font = Legion::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), size);
	
	if (font == NULL)
		return io.Fonts->AddFontDefault();

	return Legion::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), size);
}

void LoadTexture()
{
	for (std::pair<std::string, std::string> it : LoadClubesource)
		LoadTextrue(it.first, it.second, 0);
	for (std::pair<std::string, std::string> it : LoadLevelsResource)
		LoadTextrue(it.first, it.second, 2);
	for (std::pair<std::string, std::string> it : LoadRankResource)
		LoadTextrue(it.first, it.second, 3);
	for (std::pair<std::string, std::string> it : MapResource)
		LoadTextrue(it.first, it.second, 5);
	for (std::pair<std::string, std::string> it : ModeResource)
		LoadTextrue(it.first, it.second, 6);
	for (std::pair<std::string, std::string> it : LoadClubRankIconResource)
		LoadTextrue(it.first, it.second, 7);
	/*for (std::pair<std::string, std::string> it : LoadGameModeResource)
		LoadTextrue(it.first, it.second, 8);
	for (std::pair<std::string, std::string> it : LoadActivatedGameModeResource)
		LoadTextrue(it.first, it.second, 9);*/
}
#pragma endregion
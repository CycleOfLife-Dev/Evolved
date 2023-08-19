#pragma once
#include "stdafx.h"
#include "Types.h"
#include "Addresses.h"
#include "Utils.h"
#include "TextureLoader.h"
#include "Crash Dumper.h"
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <iostream>
#include "encrypt.h"
#include <regex>
#include "cSound.h"
#include "SurrenderVoice.h"

Memory Mem;

void S4Gui(S4_Screen S4_screen)
{
	Show_Screen((uintptr_t*)*reinterpret_cast<uintptr_t*>(Show_Screen_Instance), static_cast<unsigned int>(S4_screen));
}

std::string EncryptDecrypt(std::string Data, bool nEncrypt)
{
	/*if (Data.empty())
		return NULL;*/

	if (nEncrypt == true)
	{
		return encrypt(Data, PlayerEnDecKey);
	}
	else
	{
		return decrypt(Data, PlayerEnDecKey);
	}
}

std::string GetValue(uintptr_t ptr, bool IsEncrypt = false)
{
	return EncryptDecrypt(Mem.ReadStr(ptr), IsEncrypt);
}

void SendInfoToServer(std::string Key, std::string Value, std::string Name)
{
	try
	{
		/*if (Key.empty() || Value.empty() || Name.empty())
			return;*/

		std::string KeyValue = EncryptDecrypt(Key, true);
		std::string value = EncryptDecrypt(Value, true);
		std::string NameValue = EncryptDecrypt(Name, true);

	/*	if (KeyValue.empty() || value.empty() || NameValue.empty())
			return;*/

		SendInfo_Req(reinterpret_cast<uintptr_t*>(GameServer_Instance), &KeyValue, &value, &NameValue);
	}
	catch (std::exception ex)
	{

	}
	catch (...) {}
}

#pragma region SMessageBox

void CustomMessageBox(std::string CMessage, int CButtons, Msgbox CMessageType)
{
	Message = CMessage;
	Buttons = CButtons;
	MessageType = CMessageType;
	CMessageBoxUI = true;
}

void SMsgBox(std::string msg)
{
	CustomMessageBox(msg, 1, Msgbox::CNone);
}

#pragma endregion

void ExchangeCleanUp()
{
	ExchangePlayer1NickName = "";
	ExchangePlayer2NickName = "";
	ExchangeItemInfo = "";
	ExchangeItemInfo2 = "";
	ExchangeItemInfo3 = "";
	ExchangeP2ItemInfo = "";
	ExchangeP2ItemInfo2 = "";
	ExchangeP2ItemInfo3 = "";
	ExchangeP1Texture = NULL;
	ExchangeP1Texture2 = NULL;
	ExchangeP1Texture3 = NULL;
	ExchangeP2Texture = NULL;
	ExchangeP2Texture2 = NULL;
	ExchangeP2Texture3 = NULL;
	PlayerExc = "LOCK";
	Player2Exc = "LOCK";
	S4_ItemList.clear();
	S4_ItemList2.clear();

	IsExchanged = false;
	IsExcConfirm = false;
	IsExcConfirmed = false;
	IsExcLocked = false;
	IsExcLocked2 = false;
	ExchangeBlockPlayer = false;
	ExchangeBlockPlayer2 = false;
	memset(ExcFindPlayer, 0, sizeof ExcFindPlayer);
	memset(P2PEN, 0, sizeof P2PEN);
	memset(P1PEN, 0, sizeof P1PEN);
	memset(P2LP, 0, sizeof P2LP);
	memset(P1LP, 0, sizeof P1LP);
}

#pragma region Room Information
void RoomCleanUp()
{
#pragma region Reset Room Option
#pragma region Block Custom Rules	
	BlockPS = false;
	BlockCS = false;
	BlockDG = false;
	BlockBAT = false;
	BlockBR = false;
	BlockTB = false;
	BlockIB = false;
	BlockFist = false;
	BlockKatan = false;
	BlockEXO = false;
	BlockSigmaBlade = false;
	BlockVitalShock = false;
	BlockRevolver = false;
	BlockHeavyMachineGun = false;
	BlockLightMachineGun = false;
	BlockTurret = false;
	BlockHealWeapons = false;
	BlockShockWeapons = false;
	BlockSentry = false;
	BlockSenty = false;
	BlockMineGun = false;
	BlockEarthbomber = false;
	BlockLightningBomber = false;
	BlockRocketLauncher = false;
	BlockCannonade = false;
	BlockRailGun = false;
	BlockSniper = false;
	BlockAirGun = false;
	BlockDualMagnum = false;
	BlockBurstShotgun = false;
	BlockSemiRifle = false;
	BlockSmashRifle = false;
	BlockAssaultRifle = false;
	BlockBubblRifle = false;
	BlockGaussRifle = false;
	BlockHandGun = false;
	BlockSubmachineGun = false;
	//Skills
	BlockHalfHPMastery = false;
	BlockHPMastery = false;
	BlockSkillMastery = false;
	BlockDualMastery = false;
	BlockMetallic = false;
	BlockUniqueDualMastery = false;
	BlockAnchoring = false;
	BlockFlying = false;
	BlockInvisible = false;
	BlockDetect = false;
	BlockShield = false;
	BlockBlock = false;
#pragma endregion

#pragma region Allow Custom Rules
	AllowPS = false;
	AllowCS = false;
	AllowDG = false;
	AllowBAT = false;
	AllowBR = false;
	AllowTB = false;
	AllowIB = false;
	AllowFist = false;
	AllowKatan = false;
	AllowEXO = false;
	AllowSigmaBlade = false;
	AllowVitalShock = false;
	AllowRevolver = false;
	AllowHeavyMachineGun = false;
	AllowLightMachineGun = false;
	AllowTurret = false;
	AllowHealWeapons = false;
	AllowShockWeapons = false;
	AllowSentry = false;
	AllowSenty = false;
	AllowMineGun = false;
	AllowEarthbomber = false;
	AllowLightningBomber = false;
	AllowRocketLauncher = false;
	AllowCannonade = false;
	AllowRailGun = false;
	AllowSniper = false;
	AllowAirGun = false;
	AllowDualMagnum = false;
	AllowBurstShotgun = false;
	AllowSemiRifle = false;
	AllowSmashRifle = false;
	AllowAssaultRifle = false;
	AllowBubblRifle = false;
	AllowGaussRifle = false;
	AllowHandGun = false;
	AllowSubmachineGun = false;
	//Skills
	AllowHalfHPMastery = false;
	AllowHPMastery = false;
	AllowSkillMastery = false;
	AllowDualMastery = false;
	AllowMetallic = false;
	AllowUniqueDualMastery = false;
	AllowAnchoring = false;
	AllowFlying = false;
	AllowInvisible = false;
	AllowDetect = false;
	AllowShield = false;
	AllowBlock = false;
#pragma endregion

#pragma region AllowPerTeam Custom Rules
	AllowPerTeamPS = false;
	AllowPerTeamCS = false;
	AllowPerTeamDG = false;
	AllowPerTeamBAT = false;
	AllowPerTeamBR = false;
	AllowPerTeamTB = false;
	AllowPerTeamIB = false;
	AllowPerTeamFist = false;
	AllowPerTeamKatan = false;
	AllowPerTeamEXO = false;
	AllowPerTeamSigmaBlade = false;
	AllowPerTeamVitalShock = false;
	AllowPerTeamRevolver = false;
	AllowPerTeamHeavyMachineGun = false;
	AllowPerTeamLightMachineGun = false;
	AllowPerTeamTurret = false;
	AllowPerTeamHealWeapons = false;
	AllowPerTeamShockWeapons = false;
	AllowPerTeamSentry = false;
	AllowPerTeamSenty = false;
	AllowPerTeamMineGun = false;
	AllowPerTeamEarthbomber = false;
	AllowPerTeamLightningBomber = false;
	AllowPerTeamRocketLauncher = false;
	AllowPerTeamCannonade = false;
	AllowPerTeamRailGun = false;
	AllowPerTeamSniper = false;
	AllowPerTeamAirGun = false;
	AllowPerTeamDualMagnum = false;
	AllowPerTeamBurstShotgun = false;
	AllowPerTeamSemiRifle = false;
	AllowPerTeamSmashRifle = false;
	AllowPerTeamAssaultRifle = false;
	AllowPerTeamBubblRifle = false;
	AllowPerTeamGaussRifle = false;
	AllowPerTeamHandGun = false;
	AllowPerTeamSubmachineGun = false;
	//Skills
	AllowPerTeamHalfHPMastery = false;
	AllowPerTeamHPMastery = false;
	AllowPerTeamSkillMastery = false;
	AllowPerTeamDualMastery = false;
	AllowPerTeamMetallic = false;
	AllowPerTeamUniqueDualMastery = false;
	AllowPerTeamAnchoring = false;
	AllowPerTeamFlying = false;
	AllowPerTeamInvisible = false;
	AllowPerTeamDetect = false;
	AllowPerTeamShield = false;
	AllowPerTeamBlock = false;
#pragma endregion

	option = "";
	Blockroption = "";
	Allowroption = "";
	AllowPerTeamroption = "";

	HP = 100;
	SP = 100;
	Ping = 1000;
	Heal = 0;

	AutoSit = false;
	SVoteLock = false;
	ShowTeamHP = false;
	Respawn = false;
	NoMacro = false;
	NoClassSystem = false;
	NoEnchant = false;
	NoRebirth = false;
	InsideStates = false;
	ChangeProfile = false;
	RoomMapFilter = false;
	CharactersProfileUI = false;
	CharactersProfileHalfTimeUI = false;
	CSurrenderSystemUI = false;
	CSurrendeMessageUI = false;
	CRoomOptionClient = false;
	CAfkPlayerSystemUI = false;
	ActiveFilter = false;
	RandomSwapFilter = false;
	ESCMenu = false;
	MapFilters.clear();
	std::vector<std::string>().swap(MapFilters);
	AfkPlayerKick_Timer->stop();
#pragma endregion
}

void RoomClientOpton(std::string option, std::string filter)
{
	Blockroption = "";
	Allowroption = "";
	AllowPerTeamroption = "";
	MapFilters.clear();
	std::vector<std::string>().swap(MapFilters);

	CRoomOptionClient = true;
	if (!option.empty())
	{
		for (std::string element : Mem.Split(option, '-'))
		{
			if (element.empty())
				continue;

			if (element == "ShowTeamHP Yes")
			{
				ShowTeamHP = true;
				Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x01", 4);
			}
			else if (element == "ShowTeamHP No")
			{
				ShowTeamHP = false;
				Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x00", 4);
			}

			if (element == "Respawn Yes")
			{
				Respawn = true;
				Mem.WriteASM(InfRespawn, (DWORD)"\x00", 1);
			}
			else if (element == "Respawn No")
			{
				Respawn = false;
				Mem.WriteASM(InfRespawn, (DWORD)"\x04", 1);
			}

			if (element == "NoClassSystem Yes")
			{
				NoClassSystem = true;
			}
			else if (element == "NoClassSystem No")
			{
				NoClassSystem = false;
			}

			if (element == "NoEnchant Yes")
			{
				NoEnchant = true;
			}
			else if (element == "NoEnchant No")
			{
				NoEnchant = false;
			}

			if (element == "InsideStates Yes")
			{
				InsideStates = true;
			}
			else if (element == "InsideStates No")
			{
				InsideStates = false;
			}

			if (element == "NoRebirth Yes")
			{
				NoRebirth = true;
			}
			else if (element == "NoRebirth No")
			{
				NoRebirth = false;
			}

			if (element == "AutoSit Yes")
			{
				//Mem.WriteASM(CharBlock_Offset, (DWORD)"\x71", 1);
				AutoSit = true;
			}
			else if (element == "AutoSit No")
			{
				//Mem.WriteASM(CharBlock_Offset, (DWORD)"\x74", 1);
				AutoSit = false;
			}

			if (element == "ChangeProfile Yes")
			{
				ChangeProfile = true;
			}
			else if (element == "ChangeProfile No")
			{
				ChangeProfile = false;
			}

			if (element == "Active Filter Yes")
			{
				ActiveFilter = true;
			}
			else if (element == "Active Filter No")
			{
				ActiveFilter = false;
			}

			if (element == "Random Swap Filter Yes")
			{
				RandomSwapFilter = true;
			}
			else if (element == "Random Swap Filter No")
			{
				RandomSwapFilter = false;
			}

			if (element.find("HP [") != std::string::npos)
			{
				HP = Mem.StdToFloat(Mem.get_str_between_str(element, "HP [", "]"));
			}

			if (element.find("SP ") != std::string::npos)
			{
				SP = Mem.StdToFloat(Mem.get_str_between_str(element, "SP [", "]"));
			}

			if (element.find("Heal ") != std::string::npos)
			{
				Heal = Mem.StdToFloat(Mem.get_str_between_str(element, "Heal [", "]"));
			}

			if (element.find("Ping ") != std::string::npos)
			{
				Ping = Mem.StdToFloat(Mem.get_str_between_str(element, "Ping [", "]"));
			}

			if (element == "BlockPS Yes")
			{
				Blockroption += "Plasma Sword\n";
			}

			if (element == "BlockCS Yes")
			{
				Blockroption += "Counter Sword\n";
			}

			if (element == "BlockDG Yes")
			{
				Blockroption += "Spy Dagger\n";
			}

			if (element == "BlockBAT Yes")
			{
				Blockroption += "Bat\n";
			}

			if (element == "BlockBR Yes")
			{
				Blockroption += "Breaker\n";
			}

			if (element == "BlockTB Yes")
			{
				Blockroption += "Twin Blade\n";
			}

			if (element == "BlockIB Yes")
			{
				Blockroption += "Iron Boots\n";
			}

			if (element == "BlockFist Yes")
			{
				Blockroption += "Fist\n";
			}

			if (element == "BlockKatan Yes")
			{
				Blockroption += "Katan\n";
			}

			if (element == "BlockEXO Yes")
			{
				Blockroption += "Exo\n";
			}

			if (element == "BlockSigmaBlade Yes")
			{
				Blockroption += "Sigma Blade\n";
			}

			if (element == "BlockVitalShock Yes")
			{
				Blockroption += "Vital Shock\n";
			}

			if (element == "BlockRevolver Yes")
			{
				Blockroption += "Revolver\n";
			}

			if (element == "BlockHeavyMachineGun Yes")
			{
				Blockroption += "Heavy Machine Gun\n";
			}

			if (element == "BlockLightMachineGun Yes")
			{
				Blockroption += "Light Machine Gun\n";
			}

			if (element == "BlockTurret Yes")
			{
				Blockroption += "Turret\n";
			}

			if (element == "BlockHealWeapons Yes")
			{
				Blockroption += "Heal Weapons\n";
			}

			if (element == "BlockShockWeapons Yes")
			{
				Blockroption += "Shock Weapons\n";
			}

			if (element == "BlockSentry Yes")
			{
				Blockroption += "Sentry\n";
			}

			if (element == "BlockSenty Yes")
			{
				Blockroption += "Senty\n";
			}

			if (element == "BlockMineGun Yes")
			{
				Blockroption += "Mine Gun\n";
			}

			if (element == "BlockEarthbomber Yes")
			{
				Blockroption += "Earth bomber\n";
			}

			if (element == "BlockLightningBomber Yes")
			{
				Blockroption += "Lightning Bomber\n";
			}

			if (element == "BlockRocketLauncher Yes")
			{
				Blockroption += "Rocket Launcher\n";
			}

			if (element == "BlockCannonade Yes")
			{
				Blockroption += "Cannonade\n";
			}

			if (element == "BlockRailGun Yes")
			{
				Blockroption += "Rail Gun\n";
			}

			if (element == "BlockSniper Yes")
			{
				Blockroption += "Sniper\n";
			}

			if (element == "BlockAirGun Yes")
			{
				Blockroption += "Air Gun\n";
			}

			if (element == "BlockDualMagnum Yes")
			{
				Blockroption += "Dual Magnum\n";
			}

			if (element == "BlockBurstShotgun Yes")
			{
				Blockroption += "Burst Shotgun\n";
			}

			if (element == "BlockSemiRifle Yes")
			{
				Blockroption += "Semi Rifle\n";
			}

			if (element == "BlockSmashRifle Yes")
			{
				Blockroption += "Smash Rifle\n";
			}

			if (element == "BlockAssaultRifle Yes")
			{
				Blockroption += "Assault Rifle\n";
			}

			if (element == "BlockBubblRifle Yes")
			{
				Blockroption += "Bubbl Rifle\n";
			}

			if (element == "BlockGaussRifle Yes")
			{
				Blockroption += "Gauss Rifle\n";
			}

			if (element == "BlockHandGun Yes")
			{
				Blockroption += "Hand Gun\n";
			}

			if (element == "BlockSubmachineGun Yes")
			{
				Blockroption += "Submachine Gun\n";
			}

			if (element == "BlockHalfHPMastery Yes")
			{
				Blockroption += "Half HP Mastery Skill\n";
			}

			if (element == "BlockHPMastery Yes")
			{
				Blockroption += "HP Mastery Skill\n";
			}

			if (element == "BlockSkillMastery Yes")
			{
				Blockroption += "Skill Mastery Skill\n";
			}

			if (element == "BlockDualMastery Yes")
			{
				Blockroption += "Dual Mastery Skills\n";
			}

			if (element == "BlockMetallic Yes")
			{
				Blockroption += "Metallic Skills\n";
			}

			if (element == "BlockUniqueDualMastery Yes")
			{
				Blockroption += "Unique Dual Mastery Skill\n";
			}

			if (element == "BlockAnchoring Yes")
			{
				Blockroption += "Anchoring Skills\n";
			}

			if (element == "BlockFlying Yes")
			{
				Blockroption += "Flying Skills\n";
			}

			if (element == "BlockInvisible Yes")
			{
				Blockroption += "Invisible Skills\n";
			}

			if (element == "BlockDetect Yes")
			{
				Blockroption += "Detect Skills\n";
			}

			if (element == "BlockShield Yes")
			{
				Blockroption += "Shield Skills\n";
			}

			if (element == "BlockBlock Yes")
			{
				Blockroption += "Block Skills";
			}

			//Allow
			if (element == "AllowPS Yes")
			{
				Allowroption += "Plasma Sword\n";
			}

			if (element == "AllowCS Yes")
			{
				Allowroption += "Counter Sword\n";
			}

			if (element == "AllowDG Yes")
			{
				Allowroption += "Spy Dagger\n";
			}

			if (element == "AllowBAT Yes")
			{
				Allowroption += "Bat\n";
			}

			if (element == "AllowBR Yes")
			{
				Allowroption += "Breaker\n";
			}

			if (element == "AllowTB Yes")
			{
				Allowroption += "Twin Blade\n";
			}

			if (element == "AllowIB Yes")
			{
				Allowroption += "Iron Boots\n";
			}

			if (element == "AllowFist Yes")
			{
				Allowroption += "Fist\n";
			}

			if (element == "AllowKatan Yes")
			{
				Allowroption += "Katan\n";
			}

			if (element == "AllowEXO Yes")
			{
				Allowroption += "Exo\n";
			}

			if (element == "AllowSigmaBlade Yes")
			{
				Allowroption += "Sigma Blade\n";
			}

			if (element == "AllowVitalShock Yes")
			{
				Allowroption += "Vital Shock\n";
			}

			if (element == "AllowRevolver Yes")
			{
				Allowroption += "Revolver\n";
			}

			if (element == "AllowHeavyMachineGun Yes")
			{
				Allowroption += "Heavy Machine Gun\n";
			}

			if (element == "AllowLightMachineGun Yes")
			{
				Allowroption += "Light Machine Gun\n";
			}

			if (element == "AllowTurret Yes")
			{
				Allowroption += "Turret\n";
			}

			if (element == "AllowHealWeapons Yes")
			{
				Allowroption += "Heal Weapons\n";
			}

			if (element == "AllowShockWeapons Yes")
			{
				Allowroption += "Shock Weapons\n";
			}

			if (element == "AllowSentry Yes")
			{
				Allowroption += "Sentry\n";
			}

			if (element == "AllowSenty Yes")
			{
				Allowroption += "Senty\n";
			}

			if (element == "AllowMineGun Yes")
			{
				Allowroption += "Mine Gun\n";
			}

			if (element == "AllowEarthbomber Yes")
			{
				Allowroption += "Earth bomber\n";
			}

			if (element == "AllowLightningBomber Yes")
			{
				Allowroption += "Lightning Bomber\n";
			}

			if (element == "AllowRocketLauncher Yes")
			{
				Allowroption += "Rocket Launcher\n";
			}

			if (element == "AllowCannonade Yes")
			{
				Allowroption += "Cannonade\n";
			}

			if (element == "AllowRailGun Yes")
			{
				Allowroption += "Rail Gun\n";
			}

			if (element == "AllowSniper Yes")
			{
				Allowroption += "Sniper\n";
			}

			if (element == "AllowAirGun Yes")
			{
				Allowroption += "Air Gun\n";
			}

			if (element == "AllowDualMagnum Yes")
			{
				Allowroption += "Dual Magnum\n";
			}

			if (element == "AllowBurstShotgun Yes")
			{
				Allowroption += "Burst Shotgun\n";
			}

			if (element == "AllowSemiRifle Yes")
			{
				Allowroption += "Semi Rifle\n";
			}

			if (element == "AllowSmashRifle Yes")
			{
				Allowroption += "Smash Rifle\n";
			}

			if (element == "AllowAssaultRifle Yes")
			{
				Allowroption += "Assault Rifle\n";
			}

			if (element == "AllowBubblRifle Yes")
			{
				Allowroption += "Bubbl Rifle\n";
			}

			if (element == "AllowGaussRifle Yes")
			{
				Allowroption += "Gauss Rifle\n";
			}

			if (element == "AllowHandGun Yes")
			{
				Allowroption += "Hand Gun\n";
			}

			if (element == "AllowSubmachineGun Yes")
			{
				Allowroption += "Submachine Gun\n";
			}

			if (element == "AllowHalfHPMastery Yes")
			{
				Allowroption += "Half HP Mastery Skill\n";
			}

			if (element == "AllowHPMastery Yes")
			{
				Allowroption += "HP Mastery Skill\n";
			}

			if (element == "AllowSkillMastery Yes")
			{
				Allowroption += "Skill Mastery Skill\n";
			}

			if (element == "AllowDualMastery Yes")
			{
				Allowroption += "Dual Mastery Skills\n";
			}

			if (element == "AllowMetallic Yes")
			{
				Allowroption += "Metallic Skills\n";
			}

			if (element == "AllowUniqueDualMastery Yes")
			{
				Allowroption += "Unique Dual Mastery Skill\n";
			}

			if (element == "AllowAnchoring Yes")
			{
				Allowroption += "Anchoring Skills\n";
			}

			if (element == "AllowFlying Yes")
			{
				Allowroption += "Flying Skills\n";
			}

			if (element == "AllowInvisible Yes")
			{
				Allowroption += "Invisible Skills\n";
			}

			if (element == "AllowDetect Yes")
			{
				Allowroption += "Detect Skills\n";
			}

			if (element == "AllowShield Yes")
			{
				Allowroption += "Shield Skills\n";
			}

			if (element == "AllowBlock Yes")
			{
				Allowroption += "Block Skills\n";
			}

			//Per Team

			if (element == "AllowPerTeamPS Yes")
			{
				AllowPerTeamroption += "Plasma Sword\n";
			}

			if (element == "AllowPerTeamCS Yes")
			{
				AllowPerTeamroption += "Counter Sword\n";
			}

			if (element == "AllowPerTeamDG Yes")
			{
				AllowPerTeamroption += "Spy Dagger\n";
			}

			if (element == "AllowPerTeamBAT Yes")
			{
				AllowPerTeamroption += "Bat\n";
			}

			if (element == "AllowPerTeamBR Yes")
			{
				AllowPerTeamroption += "Breaker\n";
			}

			if (element == "AllowPerTeamTB Yes")
			{
				AllowPerTeamroption += "Twin Blade\n";
			}

			if (element == "AllowPerTeamIB Yes")
			{
				AllowPerTeamroption += "Iron Boots\n";
			}

			if (element == "AllowPerTeamFist Yes")
			{
				AllowPerTeamroption += "Fist\n";
			}

			if (element == "AllowPerTeamKatan Yes")
			{
				AllowPerTeamroption += "Katan\n";
			}

			if (element == "AllowPerTeamEXO Yes")
			{
				AllowPerTeamroption += "Exo\n";
			}

			if (element == "AllowPerTeamSigmaBlade Yes")
			{
				AllowPerTeamroption += "Sigma Blade\n";
			}

			if (element == "AllowPerTeamVitalShock Yes")
			{
				AllowPerTeamroption += "Vital Shock\n";
			}

			if (element == "AllowPerTeamRevolver Yes")
			{
				AllowPerTeamroption += "Revolver\n";
			}

			if (element == "AllowPerTeamHeavyMachineGun Yes")
			{
				AllowPerTeamroption += "Heavy Machine Gun\n";
			}

			if (element == "AllowPerTeamLightMachineGun Yes")
			{
				AllowPerTeamroption += "Light Machine Gun\n";
			}

			if (element == "AllowPerTeamTurret Yes")
			{
				AllowPerTeamroption += "Turret\n";
			}

			if (element == "AllowPerTeamHealWeapons Yes")
			{
				AllowPerTeamroption += "Heal Weapons\n";
			}

			if (element == "AllowPerTeamShockWeapons Yes")
			{
				AllowPerTeamroption += "Shock Weapons\n";
			}

			if (element == "AllowPerTeamSentry Yes")
			{
				AllowPerTeamroption += "Sentry\n";
			}

			if (element == "AllowPerTeamSenty Yes")
			{
				AllowPerTeamroption += "Senty\n";
			}

			if (element == "AllowPerTeamMineGun Yes")
			{
				AllowPerTeamroption += "Mine Gun\n";
			}

			if (element == "AllowPerTeamEarthbomber Yes")
			{
				AllowPerTeamroption += "Earth bomber\n";
			}

			if (element == "AllowPerTeamLightningBomber Yes")
			{
				AllowPerTeamroption += "Lightning Bomber\n";
			}

			if (element == "AllowPerTeamRocketLauncher Yes")
			{
				AllowPerTeamroption += "Rocket Launcher\n";
			}

			if (element == "AllowPerTeamCannonade Yes")
			{
				AllowPerTeamroption += "Cannonade\n";
			}

			if (element == "AllowPerTeamRailGun Yes")
			{
				AllowPerTeamroption += "Rail Gun\n";
			}

			if (element == "AllowPerTeamSniper Yes")
			{
				AllowPerTeamroption += "Sniper\n";
			}

			if (element == "AllowPerTeamAirGun Yes")
			{
				AllowPerTeamroption += "Air Gun\n";
			}

			if (element == "AllowPerTeamDualMagnum Yes")
			{
				AllowPerTeamroption += "Dual Magnum\n";
			}

			if (element == "AllowPerTeamBurstShotgun Yes")
			{
				AllowPerTeamroption += "Burst Shotgun\n";
			}

			if (element == "AllowPerTeamSemiRifle Yes")
			{
				AllowPerTeamroption += "Semi Rifle\n";
			}

			if (element == "AllowPerTeamSmashRifle Yes")
			{
				AllowPerTeamroption += "Smash Rifle\n";
			}

			if (element == "AllowPerTeamAssaultRifle Yes")
			{
				AllowPerTeamroption += "Assault Rifle\n";
			}

			if (element == "AllowPerTeamBubblRifle Yes")
			{
				AllowPerTeamroption += "Bubbl Rifle\n";
			}

			if (element == "AllowPerTeamGaussRifle Yes")
			{
				AllowPerTeamroption += "Gauss Rifle\n";
			}

			if (element == "AllowPerTeamHandGun Yes")
			{
				AllowPerTeamroption += "Hand Gun\n";
			}

			if (element == "AllowPerTeamSubmachineGun Yes")
			{
				AllowPerTeamroption += "Submachine Gun\n";
			}

			if (element == "AllowPerTeamHalfHPMastery Yes")
			{
				AllowPerTeamroption += "Half HP Mastery Skill\n";
			}

			if (element == "AllowPerTeamHPMastery Yes")
			{
				AllowPerTeamroption += "HP Mastery Skill\n";
			}

			if (element == "AllowPerTeamSkillMastery Yes")
			{
				AllowPerTeamroption += "Skill Mastery Skill\n";
			}

			if (element == "AllowPerTeamDualMastery Yes")
			{
				AllowPerTeamroption += "Dual Mastery Skills\n";
			}

			if (element == "AllowPerTeamMetallic Yes")
			{
				AllowPerTeamroption += "Metallic Skills\n";
			}

			if (element == "AllowPerTeamUniqueDualMastery Yes")
			{
				AllowPerTeamroption += "Unique Dual Mastery Skill\n";
			}

			if (element == "AllowPerTeamAnchoring Yes")
			{
				AllowPerTeamroption += "Anchoring Skills\n";
			}

			if (element == "AllowPerTeamFlying Yes")
			{
				AllowPerTeamroption += "Flying Skills\n";
			}

			if (element == "AllowPerTeamInvisible Yes")
			{
				AllowPerTeamroption += "Invisible Skills\n";
			}

			if (element == "AllowPerTeamDetect Yes")
			{
				AllowPerTeamroption += "Detect Skills\n";
			}

			if (element == "AllowPerTeamShield Yes")
			{
				AllowPerTeamroption += "Shield Skills\n";
			}

			if (element == "AllowPerTeamBlock Yes")
			{
				AllowPerTeamroption += "Block Skills\n";
			}
		}

		if (Blockroption == "")
			Blockroption = "None";
		if (Allowroption == "")
			Allowroption = "None";
		if (AllowPerTeamroption == "")
			AllowPerTeamroption = "None";
	}

	if (!filter.empty())
	{
		for (std::string element : Mem.Split(filter, ','))
		{
			if (element.empty())
				continue;

			MapFilters.push_back(Mem.Split(element, '|')[0] + "|" + Mem.Split(element, '|')[1]);
		}
	}

	//MenuSelected = 4;
	//CMainMenuUI = true;
}
#pragma endregion

void ServerListInfos()
{
	plr_loc = Player_Loc::Server;
	ExchangeCleanUp();
	discord->Update("Server Selection", "", eptime, "evolved", "", "Server Selection");
}

void CleanUp()
{
	CMainMenuUI = false;
	CRoomOptionUI = false;
	CChatSystemUI = false;
	ChatButtonMenuLock = false;
	CRoomOptionClient = false;
	CDailyRewardUI = false;
	CCustomNameTagUI = false;
	CAutoEventUI = false;
	CMessageBoxUI = false;
	CGameRewardsUI = false;
	CXMasEventUI = false;
	CAfkPlayerSystemUI = false;
	CharactersProfileUI = false;
	CharactersProfileHalfTimeUI = false;
	CSurrenderSystemUI = false;
	CSurrendeMessageUI = false;
	RoomMapFilter = false;
}

#pragma region Client Information
void Player_PlayerInfoListAck()
{
	CINIT = true;

	TotalMatchs = (char*)((char*)dwBaseAddr + 0x176EB14);
	Win = (char*)((char*)dwBaseAddr + 0x176EB30);
	Loss = (char*)((char*)dwBaseAddr + 0x176EB4C);
	WinLossPercentage = (char*)((char*)dwBaseAddr + 0x176EB68);

	discord->Update("Loading Screen", "", eptime, "evolved", "", "Loading");
}

_declspec(naked) void Player_PlayerInfoList_Ack_Hook(void)//8B 4D F8 8B 51 04 89 55 E8 8B 45 E8 89 45 E0 8B 4D F8 8B 11 89 55 E4 8B 45 E4 89 45 FC EB 09 8B 4D FC 83 C1 45 89 4D FC 8B 55 FC 3B 55 E0
{
	_asm
	{
		mov ecx, [ebp - 0x08]
		mov edx, [ecx + 0x04]
		pushad
	}
	Player_PlayerInfoListAck();
	__asm
	{
		popad
		jmp Player_PlayerInfoList_Ack_Back;
	}
}

void Room_GameLoadingAck()
{
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
	CGameMenuUI = false;

	AfkPlayerKick_Timer->stop();
	CleanUp();
}

_declspec(naked) void Room_GameLoading_Ack_Hook(void)
{
	_asm
	{
		mov[ebp - 0x38], eax
		mov ecx, [ebp - 0x38]
		pushad
	}
	Room_GameLoadingAck();
	__asm
	{
		popad
		jmp Room_GameLoading_Ack_Back;
	}
}

void  Room_GamePlayCountDownAck()
{
	plr_loc = Player_Loc::LoadingEnd;

	if (plr_loc == Player_Loc::Playing)
		ESCMenu = true;

	plr_loc = Player_Loc::CountDown;
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
	CGameMenuUI = false;
}

_declspec(naked) void Room_GamePlayCountDown_Ack_Hook(void)//89 45 C4 8B 4D C4 8B 11 8B 4D C4 8B 42 2C FF D0 85 C0
{
	_asm
	{
		mov[ebp - 0x3C], eax
		mov ecx, [ebp - 0x3C]
		pushad
	}

	Room_GamePlayCountDownAck();
	__asm
	{
		popad
		jmp Room_GamePlayCountDown_Ack_Back;
	}
}

void PlayerEndter()
{
	Gamestate = Mem.ReadMemP(GameManager, { 0x40, 0x4 });
	Playerstate = Mem.ReadMemP(GameManager, { 0x124, 0x8, 0x3C, 0x8, 0x360 });

	if (Gamestate == 2 && Playerstate == 4 && !CAfkPlayerSystemUI)
	{
		AfkPlayerKick_Timer->reset();
		AfkPlayerKick_Timer->start();
		CAfkPlayerSystemUI = true;
	}

	MapName = Mem.ReadStr(dwBaseAddr + 0x1772774);
	GameRule = (char*)((char*)dwBaseAddr + 0x1772758);
	LargeImage = Mem.StrLowerCases(Mem.StrReplaceV(Mem.StrReplace(MapName), '-', '_'));
	SmallImage = Mem.StrLowerCases(GameRule);

	std::string nickname;
	if (MyNickName.find("{") != std::string::npos)
		nickname = Mem.Split(MyNickName, '}')[1];
	else
		nickname = MyNickName;

	discord->Update(GameRule + " " + MapName + " " + PlayerCount, nickname + " " + GameRuleState + " " + RoomTime, eptime, LargeImage, SmallImage, RoomId, RoomId);
}

_declspec(naked) void EnterPlayer_Ack_Hook(void)// 8B 4D D4 8B 42 20 FF D0 88 45 C1 0F B6 4D C1 85 C9 75 1A
{
	_asm
	{
		mov ecx, [ebp - 0x2C]
		mov eax, [edx + 0x20]
		pushad
	}
	PlayerEndter();
	__asm
	{
		popad
		jmp EnterPlayer_Ack_Back;
	}
}

_declspec(naked) void Game_ChangeState_Ack_Hook(void)// 8B 4D F4 8B 50 7C FF D2 8B 45 10 83 38 04 74 5C 8B 4D F4 8B 51 0C 89 55 E8 8B 45 E8 8B 48 08 89 4D E4
{
	_asm
	{
		mov[ebp - 0x14], eax
		mov ecx, [ebp - 0x14]
		pushad
	}
	PlayerEndter();
	__asm
	{
		popad
		jmp Game_ChangeState_Ack_Back;
	}
}

void Briefing()
{
	plr_loc = Player_Loc::Room;
	CGameMenuUI = false;
	if (CSurrendeMessageUI)
		CSurrendeMessageUI = false;
	Gamestate = Mem.ReadMemP(GameManager, { 0x40, 0x4 });
	Playerstate = Mem.ReadMemP(GameManager, { 0x124, 0x8, 0x3C, 0x8, 0x360 });

	if (Gamestate == 3 || Gamestate == 1)
	{
		CharactersProfileHalfTimeUI = false;
		CharactersProfileUI = false;

		AfkPlayerKick_Timer->stop();
		CAfkPlayerSystemUI = false;
	}

	std::string nickname;
	if (MyNickName.find("{") != std::string::npos)
		nickname = Mem.Split(MyNickName, '}')[1];
	else
		nickname = MyNickName;

	discord->Update(GameRule + " " + MapName + " " + PlayerCount, nickname + " " + GameRuleState + " " + RoomTime, eptime, LargeImage, SmallImage, RoomId, RoomId);

}

_declspec(naked) void Game_BriefingInfo_Ack_Hook(void)// 89 4D E4 83 7D E4 00 75 07 32 C0
{
	_asm
	{
		mov[ebp - 0x38], edx
		mov eax, [ebp - 0x38]
		pushad
	}
	Briefing();
	__asm
	{
		popad
		jmp RevSevInfo_Back;
	}
}

void ChatAck()
{
	RoomId = (char*)((char*)Room_Id_Offset);
	RoomName = (char*)((char*)Room_Name_Offset);
	GameRuleState = "Waiting";

	Gamestate = Mem.ReadMemP(GameManager, { 0x40, 0x4 });
	Playerstate = Mem.ReadMemP(GameManager, { 0x124, 0x8, 0x3C, 0x8, 0x360 });

	if (Gamestate == 2 && Playerstate == 4)
	{
		plr_loc == Player_Loc::Room;
		CGameMenuUI = false;
		AfkPlayerKick_Timer->reset();
		AfkPlayerKick_Timer->start();
		CAfkPlayerSystemUI = true;
	}

	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
	plr_loc = Player_Loc::Room;
	CGameMenuUI = false;
	PlayerCount = (char*)((char*)dwBaseAddr + 0x1772790);
	RoomTime = (char*)((char*)dwBaseAddr + 0x17727C8);
	MapName = Mem.ReadStr(dwBaseAddr + 0x1772774);
	GameRule = (char*)((char*)dwBaseAddr + 0x1772758);
	LargeImage = Mem.StrLowerCases(Mem.StrReplaceV(Mem.StrReplace(MapName), '-', '_'));
	SmallImage = Mem.StrLowerCases(GameRule);
	ESCMenu = false;
	std::string nickname;
	if (MyNickName.find("{") != std::string::npos)
		nickname = Mem.Split(MyNickName, '}')[1];
	else
		nickname = MyNickName;

	discord->Update(GameRule + " " + MapName + " " + PlayerCount, nickname + " " + GameRuleState + " " + RoomTime, eptime, LargeImage, SmallImage, RoomId, RoomId);

}

_declspec(naked) void Chat_Ack_Hook(void)// 55 8B EC 51 89 4D FC B0 01 8B E5 5D C2 0C 00 CC 55 8B EC 6A FF (4)
{
	_asm
	{
		mov[ebp - 0x04], ecx
		mov al, 0x01
		pushad
	}
	ChatAck();
	__asm
	{
		popad
		jmp RevSevInfo_Back;
	}
}

void Game_RefreshGameRuleInfoAck()
{
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
	CGameMenuUI = false;
}

_declspec(naked) void Game_RefreshGameRuleInfo_Ack_Hook(void)// 8B 55 10 8B 02 50 8B 4D F8 8B 11 8B 4D F8 8B 42 7C
{
	_asm
	{
		mov ecx, [ebp - 0x08]
		mov eax, [edx + 0x7C]
		pushad
	}
	Game_RefreshGameRuleInfoAck();
	__asm
	{
		popad
		jmp RevSevInfo_Back;
	}
}

void Game_ChangeSubStateAck()
{
	Playerstate = Mem.ReadMemP(GameManager, { 0x124, 0x8, 0x3C, 0x8, 0x360 });
	int SubGameStats = Mem.ReadMemP(GameManager, { 0x138, 0x8, 0x0, 0x4, 0x10 });
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
	CGameMenuUI = false;
	plr_loc = Player_Loc::Playing;

	GameRuleState = "Playing";

	if (plr_loc != Player_Loc::CountDown && Playerstate == 4 && SubGameStats == 2)
	{
		GameRuleState = "HalfTime";
		plr_loc = Player_Loc::HalfTime;


		CharactersProfileHalfTimeUI = true;

		if (CharactersProfileHalfTimeUI)
		{
			HalfTimeTimer->reset();
			HalfTimeTimer->start();
		}
	}

	std::string nickname;
	if (MyNickName.find("{") != std::string::npos)
		nickname = Mem.Split(MyNickName, '}')[1];
	else
		nickname = MyNickName;

	discord->Update(GameRule + " " + MapName + " " + PlayerCount, nickname + " " + GameRuleState, eptime, LargeImage, SmallImage, RoomId, RoomId);

	if (Playerstate != 4)
	{
		if (SubGameStats == 1)
		{
			plr_loc = Player_Loc::HalfTime;

			CharactersProfileHalfTimeUI = true;

			if (CharactersProfileHalfTimeUI)
			{
				HalfTimeTimer->reset();
				HalfTimeTimer->start();
			}
		}
		else
		{
			plr_loc = Player_Loc::Playing;

			HalfTimeTimer->reset();
			HalfTimeTimer->stop();
			CharactersProfileUI = false;
			CharactersProfileHalfTimeUI = false;
			GameRuleState = "Second Half";
		}
	}

	std::string nickname;
	if (MyNickName.find("{") != std::string::npos)
		nickname = Mem.Split(MyNickName, '}')[1];
	else
		nickname = MyNickName;

	discord->Update(GameRule + " " + MapName + " " + PlayerCount, nickname + " " + GameRuleState, eptime, LargeImage, SmallImage, RoomId, RoomId);
}

_declspec(naked) void Game_ChangeSubState_Ack_Hook(void)// -> 89 4D 90 8B 55 90 8B 42 0C 89 45 C4 83 7D C4 00
{
	_asm
	{
		mov ecx, [ebp - 0x08]
		mov edx, [ecx + 0x0C]
		pushad
	}
	Game_ChangeSubStateAck();
	__asm
	{
		popad
		jmp RevSevInfo_Back;
	}
}

_declspec(naked) void Room_ListInfo_Ack_Hook(void)//55 8B EC 83 EC 08 89 4D F8 8B 45 F8 8B 48 54 89 4D FC 8B 55 FC 8B 45 FC 8B 4A 04 2B 08 C1 F9 02 8B C1 8B E5 5D
{
	_asm
	{
		mov[ebp - 0x04], ecx
		mov edx, [ebp - 0x04]
		pushad
	}
	CRoomOptionUI = false;
	__asm
	{
		popad
		jmp RevSevInfo_Back;
	}
}


std::string ChatMessageColor(Chat_Type type)
{
	std::string color;

	if (type == Chat_Type::All)
		color = "@dddd}";

	if (type == Chat_Type::Clan)
		color = "@116c2b}";

	if (type == Chat_Type::Union)
		color = "@0c4375}";

	/*if (type == Chat_Type::Support)
		color = "@c43d08}";*/

	if (type == Chat_Type::Whisper)
		color = "@9308ac}";

	if (type == Chat_Type::Team)
		color = "@0889c4}";

	return color;
}

std::string RankPointName(unsigned int RankPoint)
{
	std::string Rank;

	if (RankPoint >= 20000)
		Rank = "Legendary";
	else if (RankPoint >= 17800 && RankPoint < 20000)
		Rank = "Berserker III";
	else if (RankPoint >= 16000 && RankPoint < 17800)
		Rank = "Berserker II";
	else if (RankPoint >= 14800 && RankPoint < 16000)
		Rank = "Berserker I";
	else if (RankPoint >= 13800 && RankPoint < 14800)
		Rank = "Assassin III";
	else if (RankPoint >= 12800 && RankPoint < 13800)
		Rank = "Assassin II";
	else if (RankPoint >= 11700 && RankPoint < 12800)
		Rank = "Assassin I";
	else if (RankPoint >= 10700 && RankPoint < 11700)
		Rank = "Predator III";
	else if (RankPoint >= 9500 && RankPoint < 10700)
		Rank = "Predator II";
	else if (RankPoint >= 8300 && RankPoint < 9500)
		Rank = "Predator I";
	else if (RankPoint >= 7300 && RankPoint < 8300)
		Rank = "Diamond III";
	else if (RankPoint >= 6300 && RankPoint < 7300)
		Rank = "Diamond II";
	else if (RankPoint >= 5200 && RankPoint < 6300)
		Rank = "Diamond I";
	else if (RankPoint >= 4300 && RankPoint < 5200)
		Rank = "Platinum III";
	else if (RankPoint >= 3500 && RankPoint < 4300)
		Rank = "Platinum II";
	else if (RankPoint >= 2700 && RankPoint < 3500)
		Rank = "Platinum I";
	else if (RankPoint >= 2000 && RankPoint < 2700)
		Rank = "Gold III";
	else if (RankPoint >= 1300 && RankPoint < 2000)
		Rank = "Gold II";
	else if (RankPoint >= 1100 && RankPoint < 1300)
		Rank = "Gold I";
	else if (RankPoint >= 900 && RankPoint < 1100)
		Rank = "Silver III";
	else if (RankPoint >= 750 && RankPoint < 900)
		Rank = "Silver II";
	else if (RankPoint >= 550 && RankPoint < 750)
		Rank = "Silver I";
	else if (RankPoint >= 400 && RankPoint < 550)
		Rank = "Bronze III";
	else if (RankPoint >= 250 && RankPoint < 400)
		Rank = "Bronze II";
	else if (RankPoint >= 100 && RankPoint < 250)
		Rank = "Bronze I";
	else if (RankPoint >= 0 && RankPoint < 100)
		Rank = "UnRanked";

	return Rank;
}

#pragma region Shop Calc ItemPrice
float ShopDisc()
{
	if (MyRankPoint == 20000)
		return 0.40f;
	else if (MyRankPoint == 20000)
		return 0.30f;
	else if (MyRankPoint == 16000)
		return 0.25f;
	else if (MyRankPoint == 14800)
		return 0.20f;
	else if (MyRankPoint == 13800)
		return 0.15f;

	return 1;
}

void ShopItemPrice()
{
	if (ItemId >= 2000000 && ItemId < 3000000)
		PriceRate = 8;
	else if (ItemId >= 3000000 && ItemId < 4000000)
		PriceRate = 7;
	else if (ItemId < 2000000)
		PriceRate = 6;
	else
		PriceRate = 1;

	if (ShopUsePen)
	{
		if (ShopItemCurrentColor > 0)
		{
			ShopUsePen = false;
			if (Mem.StdToInt(ShopItemMoneyPrice) / PriceRate != 1)
				ShopItemMoneyPrice = std::to_string((Mem.StdToInt(ShopItemMoneyPrice) / PriceRate) * ShopDisc());

			ShopItemMoneyLeft = std::to_string(ShopLP - Mem.StdToInt(ShopItemMoneyPrice)).c_str();
		}
		else
		{
			ShopItemMoneyPrice = std::to_string(Mem.StdToInt(ShopItemMoneyPrice) * PriceRate);
			ShopItemMoneyLeft = std::to_string(ShopPEN - Mem.StdToInt(ShopItemMoneyPrice)).c_str();
		}
	}
	else
	{
		if (Mem.StdToInt(ShopItemMoneyPrice) > 0 && ShopItemCurrentColor == 0)
			ShopItemMoneyPrice = std::to_string((Mem.StdToInt(ShopItemMoneyPrice) / PriceRate) * ShopDisc());
		else
			ShopItemMoneyPrice = std::to_string(Mem.StdToInt(ShopItemMoneyPrice) * ShopDisc());

		ShopItemMoneyLeft = std::to_string(ShopLP - Mem.StdToInt(ShopItemMoneyPrice)).c_str();
	}
}
#pragma endregion

bool ShopSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "ac8b9de9c5")
		{
			ShopItemUse.clear();
			memset(ShopItemEffects, 0, sizeof ShopItemEffects);
			ShopCoupon.clear();
			ShopUsePen = false;

			for (std::string itemuse : Mem.Split(GetValue(Addy + 0x20), '|'))
				ShopItemUse.push_back(itemuse);

			auto Effect = GetValue(Addy + 0x38);
			strcpy_s(ShopItemEffects, Effect.length() + 1, Effect.c_str());
	//		std::copy(Effect.begin(), Effect.end(), ShopItemEffects);
			/*strcpy_s(eff, effect.length() + 1, effect.c_str());
			ShopItemEffects
			for (std::string effect : Mem.Split(GetValue(Addy + 0x38), '|'))
			{
				char eff[100];
		
				strcpy_s(eff, effect.length() + 1, effect.c_str());
				ShopItemEffect.push_back(eff);
			//	ShopItemEffect.push_back(effect.c_str());
			}
			*/
			for (std::string copuon : Mem.Split(GetValue(Addy + 0x50), '|'))
				ShopCoupon.push_back(copuon.c_str());
	
			auto ItemInfo = GetValue(Addy + 0x68);

			std::string ShopitemName = "{c1ccdd}" + Mem.Split(ItemInfo, '|')[0];
			strcpy_s(ShopItemName, ShopitemName.length() + 1, ShopitemName.c_str());

			ShopItemColorMax = Mem.StdToInt(Mem.Split(ItemInfo, '|')[1]);
			ShopPEN = Mem.StdToInt(Mem.Split(ItemInfo, '|')[2]);
			ShopLP = Mem.StdToInt(Mem.Split(ItemInfo, '|')[3]);
			ShopItemMoneyPrice = Mem.Split(ItemInfo, '|')[4];

		//	if (ShopItemCurrentColor > 0)
				ShopItemMoneyLeft = std::to_string(ShopLP - Mem.StdToInt(ShopItemMoneyPrice)).c_str();
	//		else
		//		ShopItemMoneyLeft = std::to_string(ShopPEN - Mem.StdToInt(ShopItemMoneyPrice)).c_str();

				if (ShopItemUse.size() > 0)
				ShopSelectedType = ShopItemUse[0];

			if (ShopItemEffects != NULL)
				ShopSelectedEffect = ShopItemEffects;

			if (ShopCoupon.size() > 0)
				ShopSelectedCoupon = ShopCoupon[0];

			ShopItemTexture = LoadTextrueEx(std::to_string(ItemId) + "_" + std::to_string(ShopItemCurrentColor));

			CGameShopUI = true;
			return true;
		}

		//Search Item
		if (Key == "614440bde9")
		{
			ShopSearchItems.clear();
			SShopItemMoneyPrice = 0;
			std::string list = GetValue(Addy + 0x20);

			if (list.empty())
				return false;

			for (std::string it : Mem.Split(list, ','))
			{
				if (it.empty())
					continue;

				ShopSearchItem* ShopSearchitem = new ShopSearchItem();
				std::string Name = "{c1ccdd}" + Mem.Split(it, '|')[0];
				strcpy_s(ShopSearchitem->Name, Name.length() + 1, Name.c_str());
				ShopSearchitem->Id = std::atol(Mem.Split(it, '|')[1].c_str());
				ShopSearchitem->MaxColor = Mem.StdToInt(Mem.Split(it, '|')[2]);
				ShopSearchitem->Price = Mem.StdToInt(Mem.Split(it, '|')[3]);
				std::string Effect = Mem.Split(it, '|')[4];
				strcpy_s(ShopSearchitem->Effects, Effect.length() + 1, Effect.c_str());
			/*	for (std::string effect : Mem.Split(Mem.Split(it, '|')[4], '|'))
					ShopSearchitem->Effects.push_back(effect);*/
				//for (std::string type : Mem.Split(Mem.Split(it, '|')[5], '|'))
					ShopSearchitem->Type.push_back(Mem.Split(it, '|')[5]);
				ShopSearchitem->IsActive = false;
				ShopSearchitem->Image = LoadTextrueEx(std::to_string(ShopSearchitem->Id) + "_0");
				ShopSearchItems[ShopSearchItems.size() + 1] = ShopSearchitem;
			}

			SShopItemMoneyLeft = Mem.StdToLong(GetValue(Addy + 0x38));
			ShopSearchUI = true;
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Shop System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Shop System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ClanSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "34072a6a72")// at full list
		{
			if (GetValue(Addy + 0x38) == "No Clan")
			{
				IsInClan = false;
				CMainMenuUI = false;
				CRoomOptionUI = false;
				NewClanSystemUI = true;
				return true;
			}

			IsInClan = true;
			
			ClubMemberList.clear();

			std::vector<std::string> info = Mem.Split(GetValue(Addy + 0x20), '-');

			ClanName = info[0];
			ClanLevel = info[1];
			ClanMarkTexture = ClubsTexture(info[2]);
			ClanRank = info[3];
			ClanEXP = Mem.StdToFloat(info[4]);
			TotalClanExp = Mem.StdToFloat(ClanLevel) * 100.0f;
			ClanPoints = info[5];
			ClanBattels = info[6];
			ClanTitle = info[7];
			ClanMessage = info[8];

			strcpy_s(ClanNotice, ClanTitle.length() + 1, ClanTitle.c_str());
			strcpy_s(ClanNoticeMessage, ClanMessage.length() + 1, ClanMessage.c_str());

			auto list = GetValue(Addy + 0x38);

			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				ClanMemberListClass* ClanMemberListclass = new ClanMemberListClass();
				ClanMemberListclass->PlayerId = Mem.StdToInt(Mem.Split(element, '-')[1]);
				std::string Nickname = Mem.Split(element, '-')[2];
				strcpy_s(ClanMemberListclass->Nickname, Nickname.length() + 1, Nickname.c_str());
				ClanMemberListclass->Level = Mem.Split(element, '-')[3];
				std::string Location = Mem.Split(element, '-')[4];
				strcpy_s(ClanMemberListclass->Location, Location.length() + 1, Location.c_str());
				ClanMemberListclass->Rank = Mem.Split(element, '-')[5];
				ClanMemberListclass->Points = Mem.Split(element, '-')[6];
				ClanMemberListclass->Battles = Mem.Split(element, '-')[7];
				ClanMemberListclass->FriendStats = Mem.Split(element, '-')[8];
				ClanMemberListclass->PlayerStatus = Mem.Split(element, '-')[9];

				if (MyPlayerId == ClanMemberListclass->PlayerId)
				{
					MyClanRank = ClanMemberListclass->Rank;
				}

				ClubMemberList[ClubMemberList.size() + 1] = ClanMemberListclass;
			}

			if (ClubMemberList.size() > 0 && !list.empty())
			{
				ClanButtonMenu = 0;
				CMainMenuUI = false;
				NewClanSystemUI = true;
			}
			return true;
		}

		if (Key == "3d52c59e59")// Rank list
		{
			ClanRankList.clear();
			auto list = GetValue(Addy + 0x38);
			if (list.empty())
				return false;
			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				ClanRankListClass* ClanRankListclass = new ClanRankListClass();
				ClanRankListclass->Rank = Mem.Split(element, '-')[0];
				std::string Name = Mem.Split(element, '-')[1];
				strcpy_s(ClanRankListclass->Name, Name.length() + 1, Name.c_str());
				std::string  Leader = Mem.Split(element, '-')[2];
				strcpy_s(ClanRankListclass->Leader, Leader.length() + 1, Leader.c_str());
				ClanRankListclass->Points = Mem.Split(element, '-')[3];
				ClanRankListclass->Level = Mem.Split(element, '-')[4];
				ClanRankListclass->Battles = Mem.Split(element, '-')[5];

				ClanRankList[ClanRankList.size() + 1] = ClanRankListclass;
			}
			ClanButtonMenu = 2;
			return true;
		}

		if (Key == "c69299fe47")// Union list
		{
			ClanUnionList.clear();

			auto list = GetValue(Addy + 0x38);
			if (!list.empty())
			{
				for (std::string element : Mem.Split(list, ','))
				{
					if (element.empty())
						continue;

					ClanUnionListClass* ClanUnionListclass = new ClanUnionListClass();
					std::string ClanName = Mem.Split(element, '-')[0];
					strcpy_s(ClanUnionListclass->ClanName, ClanName.length() + 1, ClanName.c_str());
					ClanUnionListclass->Level = Mem.Split(element, '-')[1];
					std::string Leader = Mem.Split(element, '-')[2];
					strcpy_s(ClanUnionListclass->Leader, Leader.length() + 1, Leader.c_str());
					ClanUnionListclass->Points = Mem.Split(element, '-')[3];
					ClanUnionListclass->Level = Mem.Split(element, '-')[4];
					ClanUnionListclass->Battles = Mem.Split(element, '-')[5];
					ClanUnionListclass->Members = Mem.Split(element, '-')[6];
					ClanUnionList[ClanUnionList.size() + 1] = ClanUnionListclass;
					UnionMaster = Mem.StrToBool(Mem.Split(element, '-')[7]);
				}
			}
			ClanButtonMenu = 1;
			return true;
		}

		if (Key == "ec86108214")// Union list
		{
			auto it = ClanUnionList.find(Mem.StdToInt(GetValue(Addy + 0x20)));
			if (it != ClanUnionList.end())
				ClanUnionList.erase(it);

			CustomMessageBox("You have successfully kick " + GetValue(Addy + 0x38) + "\nout of the union.", 1, Msgbox::CNone);
			return true;
		}

		if (Key == "e3a694cc43")// Union list
		{
			ClanUnionList.clear();
			CustomMessageBox("You have successfully left the union.", 1, Msgbox::CNone);
			return true;
		}

		if (Key == "8c94e906f6")// histrry list
		{
			ClanHistoryList.clear();
			auto list = GetValue(Addy + 0x38);
			if (list.empty())
				return false;
			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				ClanHistoryListClass* ClanHistoryListclass = new ClanHistoryListClass();
			    std::string Name = Mem.Split(element, '|')[0];
				strcpy_s(ClanHistoryListclass->Name, Name.length() + 1, Name.c_str());
				ClanHistoryListclass->Date = Mem.Split(element, '|')[1];
				std::string Status = Mem.Split(element, '|')[2];
				std::string CStatus;
				if (Status == "Won")
					CStatus = "@03fc8c}" + Status;
				else
					CStatus = "@fc033d}" + Status;

				ClanHistoryListclass->Status = CStatus;
				ClanHistoryList[ClanHistoryList.size() + 1] = ClanHistoryListclass;
			}
			ClanButtonMenu = 4;
			return true;
		}

		if (Key == "db3bad8e30")//  clan info
		{
			ClubInfoMemberList.clear();

			std::vector<std::string> info = Mem.Split(GetValue(Addy + 0x20), '-');

			ClanInfoName = info[0];
			ClanInfoLevel = info[1];
			ClanInfoMarkTexture = ClubsTexture(info[2]);
			ClanInfoRank = info[3];
			ClanInfoEXP = Mem.StdToFloat(info[4]);
			TotalClanInfoExp = Mem.StdToFloat(ClanInfoLevel) * 100.0f;
			ClanInfoPoints = info[5];
			ClanInfoBattels = info[6];
			ClanInfoTitle = info[7];
			ClanInfoMessage = info[8];

			auto list = GetValue(Addy + 0x38);

			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				ClanMemberListClass* ClanMemberListclass = new ClanMemberListClass();
				ClanMemberListclass->PlayerId = Mem.StdToInt(Mem.Split(element, '-')[1]);
				std::string Nickname = Mem.Split(element, '-')[2];
				strcpy_s(ClanMemberListclass->Nickname, Nickname.length() + 1, Nickname.c_str());
				ClanMemberListclass->Level = Mem.Split(element, '-')[3];
				std::string Location = Mem.Split(element, '-')[4];
				strcpy_s(ClanMemberListclass->Location, Location.length() + 1, Location.c_str());
				ClanMemberListclass->Rank = Mem.Split(element, '-')[5];
				ClanMemberListclass->Points = Mem.Split(element, '-')[6];
				ClanMemberListclass->Battles = Mem.Split(element, '-')[7];
				ClanMemberListclass->FriendStats = Mem.Split(element, '-')[8];
				ClanMemberListclass->PlayerStatus = Mem.Split(element, '-')[9];
				ClubInfoMemberList[ClubInfoMemberList.size() + 1] = ClanMemberListclass;
			}

			NewClanSystemUI = false;
			NewClanInfoSystemUI = true;
			return true;
		}

		if (Key == "e07b9b00fb")//  clan info
		{
			ClubRequestList.clear();
			auto list = GetValue(Addy + 0x38);
			if (list.empty())
				return false;
			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				PlayerShortInfoListClass* ClanPlayerShortInfoListclass = new PlayerShortInfoListClass();
				ClanPlayerShortInfoListclass->PlayerId = Mem.StdToInt(Mem.Split(element, '-')[0]);
				std::string Name = Mem.Split(element, '-')[1];
				strcpy_s(ClanPlayerShortInfoListclass->Name, Name.length() + 1, Name.c_str());
				ClanPlayerShortInfoListclass->Level = Mem.Split(element, '-')[2];
				ClubRequestList[ClubRequestList.size() + 1] = ClanPlayerShortInfoListclass;
			}
			ClanButtonMenu = 6;
			return true;
		}

		if (Key == "a0171a4e47")//  clan info
		{
			ClubBannedList.clear();
			auto list = GetValue(Addy + 0x38);
			if (list.empty())
				return false;
			for (std::string element : Mem.Split(list, ','))
			{
				if (element.empty())
					continue;

				PlayerShortInfoListClass* ClanPlayerShortInfoListclass = new PlayerShortInfoListClass();
				ClanPlayerShortInfoListclass->PlayerId = Mem.StdToInt(Mem.Split(element, '-')[1]);
				std::string Name = Mem.Split(element, '-')[2];
				strcpy_s(ClanPlayerShortInfoListclass->Name, Name.length() + 1, Name.c_str());
				ClanPlayerShortInfoListclass->Level = Mem.Split(element, '-')[3];
				ClubBannedList[ClubBannedList.size() + 1] = ClanPlayerShortInfoListclass;
			}
			ClanButtonMenu = 7;
			return true;
		}

		if (Key == "a91548d31e")
		{
			CustomMessageBox("Clan logo has changed.", 1, Msgbox::CNone);
			ClanMarkTexture = ClubsTexture(GetValue(Addy + 0x38));
		}

		if (Key == "ef79ad8564")
		{
			CustomMessageBox(GetValue(Addy + 0x38) + " have invited you to union\nWould you like to accept", 2, Msgbox::ClanUnionAccept);
			return true;
		}

		if (Key == "af8b3f180e")
		{
			CustomMessageBox("You have successfully created your clan.", 1, Msgbox::CNone);

			return true;
		}

		if (Key == "5f70aee2d0")
		{
			auto it = ClubMemberList.find(Mem.StdToInt(GetValue(Addy + 0x38)));
			if (it != ClubMemberList.end())
				ClubMemberList.erase(it);
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Clan System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Clan System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ChatSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (plr_loc != Player_Loc::RoomList)
			return false;

		//Chat System
		if (Key == "c4d4c01a43")
		{
			time(&current_time);
			localtime_s(&local_time, &current_time);
			strftime(buffer, sizeof(buffer), "%I:%M %p", &local_time);

			ChatSystemStruc* chatSystemStruc = new ChatSystemStruc();

			Chat_Type type = (Chat_Type)Mem.StdToInt(GetValue(Addy + 0x20));
			
			chatSystemStruc->Time = "@7d7d7d}" + std::string(buffer);
			std::string Nickname;

			if (type == Chat_Type::Whisper)
				Nickname = GetValue(Addy + 0x38);
			else
				Nickname = GetValue(Addy + 0x38) + " :";

			strcpy_s(chatSystemStruc->Nickname, Nickname.length() + 1, Nickname.c_str());
			std::string Message = ChatMessageColor(type) + GetValue(Addy + 0x50);
			strcpy_s(chatSystemStruc->Message, Message.length() + 1, Message.c_str());

			ChatSystemList[ChatSystemList.size() + 1] = chatSystemStruc;
			if (type == Chat_Type::Clan)
				ClanSystemList[ClanSystemList.size() + 1] = chatSystemStruc;

			if (type == Chat_Type::Union)

				ClanUnionSystemList[ClanUnionSystemList.size() + 1] = chatSystemStruc;
			if (type == Chat_Type::Whisper)
			{
				WhisperSystemList[WhisperSystemList.size() + 1] = chatSystemStruc;

				std::string result = std::regex_replace(Nickname, std::regex("From "), "To ");

			     std::regex base_regex("To (.*) : ");


				std::smatch base_match;
				std::string matched;

				if (std::regex_search(result, base_match, base_regex)) {
					// The first sub_match is the whole string; the next
					// sub_match is the first parenthesized expression.
					if (base_match.size() == 2) {
						matched = base_match[1].str();
					}
				}
		
				//std::replace(Nickname.begin(), Nickname.end(), "Form ", ' ');
				std::string final;
				if (Message.find("Form") != std::string::npos)
				final = Mem.StrReplace(matched, "Form", "");
				if (Message.find("To") != std::string::npos)
			     final = Mem.StrReplace(matched, "To", "");

				if (Message.find("To @") != std::string::npos)
					LastWhisperNickname = "To " + Mem.Split(final, '}')[1] + " : ";
				else
					LastWhisperNickname = "To " + Mem.Split(final, '}')[0] + " : ";
			}

			ChatAutotScroll = true;
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Chat System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Chat System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ExchangeSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (plr_loc != Player_Loc::RoomList)
			return false;

		// Exchange Window Target
		if (Key == "b8f78e1ea7")
		{
			ExchangePlayer1NickName = GetValue(Addy + 0x20);
			P1Level = GetValue(Addy + 0x38);
			ExchangePlayer2NickName = GetValue(Addy + 0x50);
			P2Level = GetValue(Addy + 0x68);

			ExchangeBlockPlayer2 = true;
			return true;
		}

		// Exchange Window Player 2
		if (Key == "b978ff916c")
		{
			ExchangePlayer1NickName = GetValue(Addy + 0x20);
			P1Level = GetValue(Addy + 0x38);
			ExchangePlayer2NickName = GetValue(Addy + 0x50);
			P2Level = GetValue(Addy + 0x68);

			CustomMessageBox(ExchangePlayer1NickName + " WANT TO EXCHANGE WITH YOU\n Would you like to accept?", 2, Msgbox::ExchangeAccept);

			ExchangeBlockPlayer = true;
			return true;
		}

		// Exchange Window Accept
		if (Key == "d9d292a1ff" && GetValue(Addy + 0x38) == "Accept")
		{
			IsExchanged = true;
			MenuSelected = 7;
			CMainMenuUI = true;
			return true;
		}

		if (Key == "d9d292a1ff" && GetValue(Addy + 20) == "Refuse")
		{
			CustomMessageBox(GetValue(Addy + 0x38), 1, Msgbox::CNone);
			ExchangeCleanUp();
			return true;
		}

		//Exchange CONFIRMED
		if (Key == "7a88ddce07") // Player 2
		{
			Player2Exc = GetValue(Addy + 0x38);
			return true;
		}

		if (Key == "3625f5531e") // Player
		{
			PlayerExc = GetValue(Addy + 0x38);
			return true;
		}

		//Confirm Exc
		if (Key == "a6ed545798")
		{
			IsExcConfirm = true;
			PlayerExc = "CONFIRM";
			Player2Exc = "CONFIRM";
			return true;
		}

		if (Key == "5f392a2312")
		{
			std::string Value = GetValue(Addy + 0x38);
			strcpy_s(P1PEN, Value.length() + 1, Value.c_str());
			return true;
		}

		if (Key == "c2d6e162ea")
		{
			std::string Value = GetValue(Addy + 0x38);
			strcpy_s(P1LP, Value.length() + 1, Value.c_str());
			return true;
		}

		if (Key == "f9f989eb40")
		{
			std::string Value = GetValue(Addy + 0x38);
			strcpy_s(P2PEN, Value.length() + 1, Value.c_str());
			return true;
		}

		if (Key == "871d315942")
		{
			std::string Value = GetValue(Addy + 0x38);
			strcpy_s(P2LP, Value.length() + 1, Value.c_str());
			return true;
		}


		if (Key == "0c095b72e5")
		{
			CMainMenuUI = false;
			ExchangeCleanUp();
			SMsgBox("Exchange Completed.");
			return true;
		}

		//Exchange P1
		if (GetValue(Addy + 0x68) == "0c82aa2783")
		{
			S4_Item* item = new S4_Item();
			item->unique_id = GetValue(Addy + 0x08);
			item->item_id = GetValue(Addy + 0x20);
			std::string Effect = GetValue(Addy + 0x38);
			strcpy_s(item->Effect, Effect.length() + 1, Effect.c_str());
			item->color_id = GetValue(Addy + 0x50);
		
			item->ExchangeTexture = LoadTextrueEx(GetValue(Addy + 0x20) + "_" + GetValue(Addy + 0x50));
			S4_ItemList2[S4_ItemList2.size() + 1] = item;

			//CExchangeUI = true;
			return true;
		}

		//Exchange P2
		if (GetValue(Addy + 0x68) == "19c490949b")
		{
			S4_Item* item = new S4_Item();
			item->unique_id = GetValue(Addy + 0x08);
			item->item_id = GetValue(Addy + 0x20);
			std::string Effect = GetValue(Addy + 0x38);
			strcpy_s(item->Effect, Effect.length() + 1, Effect.c_str());
			item->color_id = GetValue(Addy + 0x50);
    		item->ExchangeTexture = LoadTextrueEx(GetValue(Addy + 0x20) + "_" + GetValue(Addy + 0x50));
			S4_ItemList[S4_ItemList.size() + 1] = item;

			//CExchangeUI = true;
			return true;
		}

		//Exchange Remove Items
		if (Key == "eafe09a1f8")
		{
			auto P1Item = S4_ItemList.find(1);
			auto P1Item2 = S4_ItemList.find(2);
			auto P1Item3 = S4_ItemList.find(3);
			if (P1Item != S4_ItemList.end())
			{
				if (P1Item->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList.erase(1);
			}

			if (P1Item2 != S4_ItemList.end())
			{
				if (P1Item2->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList.erase(2);
			}

			if (P1Item3 != S4_ItemList.end())
			{
				if (P1Item3->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList.erase(3);
			}
			return true;
		}

		if (Key == "1e720736b4")
		{
			auto P2Item = S4_ItemList2.find(1);
			auto P2Item2 = S4_ItemList2.find(2);
			auto P2Item3 = S4_ItemList2.find(3);

			if (P2Item != S4_ItemList2.end())
			{
				if (P2Item->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList2.erase(1);
			}

			if (P2Item2 != S4_ItemList2.end())
			{
				if (P2Item2->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList2.erase(2);
			}

			if (P2Item3 != S4_ItemList2.end())
			{
				if (P2Item3->second->unique_id == GetValue(Addy + 0x38))
					S4_ItemList2.erase(3);
			}
			return true;
		}

	}
	catch (std::exception & e)
	{
		//LogCrash("Exchange System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Exchange System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ChallengeSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "3c0856c5ec")
		{
			CustomMessageBox(GetValue(Addy + 0x38), 2, Msgbox::ChallengeAccept);
		}
	}
	catch (std::exception& e)
	{
		//LogCrash("Challenge System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Challenge System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool RoomOptionSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
	
		if (Key == "a84177627b")
		{// auto sit
			/*std::string IsActive = GetValue(Addy + 0x38);
			if (IsActive == "0")
				Mem.WriteASM(CharBlock_Offset, (DWORD)"\x71", 1);
			else
				Mem.WriteASM(CharBlock_Offset, (DWORD)"\x74", 1);*/
			return true;
		}

		if (Key == "fa3113b403")
		{
			S4Gui(S4_Screen::Display_RoomInvite);
			return true;
		}

		if (Key == "d73aa46bab")
		{
			if (plr_loc == Player_Loc::Room)
			{
				CRoomOptionClient = false;
				//MenuSelected = 4;
				//CMainMenuUI = true;
			}
			return true;
		}

		if (Key == "cf84b21bd7")
		{
			CRoomOptionClient = true;
			//MenuSelected = 4;
			//CMainMenuUI = true;
			return true;
		}

		if (Key == "9c2a3c057c")
		{
			RoomClientOpton(GetValue(Addy + 0x20), GetValue(Addy + 0x38));
			return true;
		}


		if (Key == "82a3c057c")
		{
			Respawn = false;
			Mem.WriteASM(InfRespawn, (DWORD)"\x04", 1);
			return true;
		}

		if (Key == "a84ec05fne")// not using
		{
			//if (GetValue(Addy + 0x38) == "killer")
			//	SendInfoToServer(std::to_string(PlayerHPValue), "killer", "a84ec05fne");
			//else if (GetValue(Addy + 0x38) == "assist")
			//	SendInfoToServer(std::to_string(PlayerHPValue), "assist", "a84ec05fne");
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Room Option System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Room Option System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool QuestsSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "c90aa346a1")
		{
			QuestSystemInfo.clear();
			auto QuestInfo = Mem.Split(GetValue(Addy + 0x38), ',');
			if (QuestInfo.empty())
				return false;
			for (std::string it : QuestInfo)
			{
				if (it.empty())
					continue;

				QuestSystemStruct* QuestSystemstruct = new QuestSystemStruct();
				std::string Name = Mem.Split(it, '-')[0];
				strcpy_s(QuestSystemstruct->Name, Name.length() + 1, Name.c_str());
				std::string Info = Mem.Split(it, '-')[1];
				strcpy_s(QuestSystemstruct->Info, Info.length() + 1, Info.c_str());
				QuestSystemstruct->Percentage = Mem.Split(it, '-')[2];
				QuestSystemstruct->Percentage2 = Mem.Split(it, '-')[3];
				QuestSystemstruct->Percentage3 = Mem.Split(it, '-')[4];
				QuestSystemstruct->RewardImage = LoadTextrueEx(Mem.Split(it, '-')[5]);
				QuestSystemstruct->Reward2Image = LoadTextrueEx(Mem.Split(it, '-')[6]);
				QuestSystemstruct->IsClaimed = Mem.StrToBool(Mem.Split(it, '-')[7]);

				if (QuestSystemstruct->IsClaimed)
					QuestSystemstruct->ClaimeImage = ClaimMarkTexture;
			/*	if (QuestInfo.size() > 7)
				{*/
			     	Quest3Reward = LoadTextrueEx(Mem.Split(it, '-')[8]);
					CurrentQuestPercentageWeekly = QuestSystemstruct->WeeklyPercentage = Mem.StdToFloat(Mem.Split(it, '-')[9]);
					MaxQuestPercentageWeekly = QuestSystemstruct->WeeklyMaxPercentage = Mem.StdToFloat(Mem.Split(it, '-')[10]);
					QuestSystemstruct->IsWeeklyClaimed = Mem.StrToBool(Mem.Split(it, '-')[11]);


					if (QuestSystemstruct->IsWeeklyClaimed)
						QuestSystemstruct->WeeklyClaimeImage = ClaimMarkTexture;
				//}
				QuestSystemInfo[QuestSystemInfo.size() + 1] = QuestSystemstruct;
			}

			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Quest System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Quest System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool BattleFieldSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "a122217062")
		{
			if (BattleField.size() > 0)
				return true;
			//BattleField.clear();
			//AdvBattleField.clear();
			auto BattleFieldInfo = Mem.Split(GetValue(Addy + 0x38), ',');
			if (BattleFieldInfo.empty())
				return false;

			for (std::string it : BattleFieldInfo)
			{

				if (it.empty())
					continue;

				BattleFieldSystemStruct* battlefield = new BattleFieldSystemStruct();
				std::string Type = Mem.Split(it, '|')[0];
				std::string Image = Mem.Split(it, '|')[1];
				battlefield->RewardImage = LoadTextrueEx(Image);
				BattleFiedlsLevel = Mem.Split(it, '|')[2];
				BattleFiedlsPoints = Mem.Split(it, '|')[3];
				battlefield->Status = Mem.StrToBool(Mem.Split(it, '|')[4]);
				battlefield->IsButtonActive = Mem.StrToBool(Mem.Split(it, '|')[5]);

				if (Type == "0")
					BattleField[BattleField.size() + 1] = battlefield;
				else
					AdvBattleField[AdvBattleField.size() + 1] = battlefield;
			}
			return true;
		}

		if (Key == "e14fcf8f1d")
		{
			BattleFieldTask.clear();
			auto BattleFieldTaslInfo = Mem.Split(GetValue(Addy + 0x38), ',');
			if (BattleFieldTaslInfo.empty())
				return true;
			for (std::string it : BattleFieldTaslInfo)
			{
				if (it.empty())
					continue;

				BattleFieldTaskSystemStruct* battlefield = new BattleFieldTaskSystemStruct();
				std::string Task = Mem.Split(it, '|')[0];
				strcpy_s(battlefield->Task, Task.length() + 1, Task.c_str());
				battlefield->PercentageInfo = Mem.Split(it, '|')[1];
				battlefield->Points = Mem.Split(it, '|')[2];
				battlefield->Status = Mem.StrToBool(Mem.Split(it, '|')[3]);

				BattleFieldTask[BattleFieldTask.size() + 1] = battlefield;
			}

			return true;
		}

		if (Key == "dfe5774aff")
		{
			BattleFiedlsLevel = GetValue(Addy + 0x20);
			BattleFiedlsPoints = GetValue(Addy + 0x38);

			auto it = BattleFieldTask.find(Mem.StdToInt(GetValue(Addy + 0x50)));
			if (it != BattleFieldTask.end())
				it->second->Status = true;

			return true;
		}

		if (Key == "a59ce5472a")
		{
			auto it = BattleField.find(Mem.StdToInt(GetValue(Addy + 0x38)));
			if (it != BattleField.end())
				it->second->Status = true;

			return true;
		}

		if (Key == "a81e4e4091")
		{
			auto it = AdvBattleField.find(Mem.StdToInt(GetValue(Addy + 0x38)));
			if (it != AdvBattleField.end())
				it->second->Status = true;

			return true;
		}
	}
	catch (std::exception& e)
	{
		//LogCrash("BattleField System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("BattleField System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool LoadMapInfo(DWORD Addy, std::string Key)
{
	try
	{

	}
	catch (std::exception& e)
	{
		//LogCrash("BattleField System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("BattleField System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool SurrenderSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "e50b8bf826")
		{
			SVoteLock = false;
			CSurrenderSystemUI = false;
		}

		if (Key == "28fcf2af4c")
		{
			if (plr_loc < Player_Loc::Playing)
				return true;

			SurrenderPlayerName = GetValue(Addy + 0x20);
			SMaxCounter = GetValue(Addy + 0x38);
			SCounter = 10;
			SVotedCounter = "1";
			if (SurrenderPlayerName != MyNickName)
				SVoteLock = false;
			CSurrenderSystemUI = true;
			SVoteTimer->reset();
			SVoteTimer->start();
			return true;
		}

		if (Key == "b442f8f62f")
		{
			if (plr_loc < Player_Loc::Playing)
				return true;

			SVotedCounter = GetValue(Addy + 0x38);
		}

		if (Key == "6f7bb31645")
		{
			if (plr_loc < Player_Loc::Playing)
				return true;

			std::string Surrender = GetValue(Addy + 0x20);
			std::string VoceSurrender = GetValue(Addy + 0x38);
			strcpy_s(SurrenderMessage, Surrender.length() + 1, Surrender.c_str());
			CSurrendeMessageUI = true;
			SVoteLock = false;
			cSound Sound;

			if (VoceSurrender == "0")
				Sound.wav.playsoundmem((LPCSTR)EnemySurrender.data());
			else
				Sound.wav.playsoundmem((LPCSTR)TeamSurrender.data());

			plr_loc = Player_Loc::Room;
			ESCMenu = false;
			return true;
		}

	}
	catch (std::exception & e)
	{
		//LogCrash("Surrender System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Surrender System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool PlayerReportSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		//Player List
		if (Key == "38e755f54f")
		{
			ReportPlayerList.clear();

			auto List = GetValue(Addy + 0x38);

			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				PlayerShortInfoListClass* ReportPlayerstruct = new PlayerShortInfoListClass();
				std::string Name = Mem.Split(element, '|')[0];
				strcpy_s(ReportPlayerstruct->Name, Name.length() + 1, Name.c_str());
				ReportPlayerstruct->Level = Mem.Split(element, '|')[1];
				ReportPlayerstruct->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[2]);
				ReportPlayerList[ReportPlayerList.size() + 1] = ReportPlayerstruct;
			}

			CGameMenuUI = false;
			CPlayerReportUI = true;
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Player Report System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Player Report System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool EventsSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "6dbac98947")
		{
			QAutoEvent = "{b4c91a}" + GetValue(Addy + 0x38);
			CAutoEventUI = true;
			return true;
		}

		if (Key == "18cc1bb625")
		{
			CAutoEventUI = false;
			return true;
		}

		//Daily Event
		if (Key == "3b3b4f8b8a")
		{
			for (int i = 0; i <= atoi(GetValue(Addy + 0x38).c_str()); i++)
			{
				DailyEventList[i] = ClaimMarkTexture;
			}

			DailyEventEnable = true;
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Events System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Events System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

void SurrenderControlReset()
{
	if (VoiceActionSendTimer->elapsedMilliseconds() > 50)
	{
		Mem.WriteASM(WeaponSwitch_Offset, (DWORD)"\x74", 1);
		VoiceActionSendTimer->stop();
	}
}

void Voice(std::string Action, std::string Gender)
{
	/*cSound Sound;

	if (Gender == "Male")
	{
		if (Action == "Go Go Go")
			Sound.wav.playsoundmem((LPCSTR)T1.data());
		else if (Action == "Attack")
			Sound.wav.playsoundmem((LPCSTR)T2.data());
		else if (Action == "Defend")
			Sound.wav.playsoundmem((LPCSTR)T3.data());
		else if (Action == "Wait me")
			Sound.wav.playsoundmem((LPCSTR)T4.data());
		else if (Action == "Run")
			Sound.wav.playsoundmem((LPCSTR)T5.data());
		else if (Action == "Watch your back")
			Sound.wav.playsoundmem((LPCSTR)T6.data());
		else if (Action == "Cover me")
			Sound.wav.playsoundmem((LPCSTR)T7.data());
		else if (Action == "Out of ammo")
			Sound.wav.playsoundmem((LPCSTR)T8.data());
		else if (Action == "Heal me")
			Sound.wav.playsoundmem((LPCSTR)T9.data());
	}

	if (Gender == "Female")
	{
		if (Action == "Go Go Go")
			Sound.wav.playsoundmem((LPCSTR)FT1.data());
		else if (Action == "Attack")
			Sound.wav.playsoundmem((LPCSTR)FT2.data());
		else if (Action == "Defend")
			Sound.wav.playsoundmem((LPCSTR)FT3.data());
		else if (Action == "Wait me")
			Sound.wav.playsoundmem((LPCSTR)FT4.data());
		else if (Action == "Run")
			Sound.wav.playsoundmem((LPCSTR)FT5.data());
		else if (Action == "Watch your back")
			Sound.wav.playsoundmem((LPCSTR)FT6.data());
		else if (Action == "Cover me")
			Sound.wav.playsoundmem((LPCSTR)FT7.data());
		else if (Action == "Out of ammo")
			Sound.wav.playsoundmem((LPCSTR)FT8.data());
		else if (Action == "Heal me")
			Sound.wav.playsoundmem((LPCSTR)FT9.data());
	}

	VoiceActionSendTimer->stop();
	VoiceActionSendTimer->reset();
	VoiceActionSendTimer->start();*/

}

void CustomVoice(std::string Action, std::string Gender)
{
	/*std::string strx = Action;
	std::wstring wstr = std::wstring(strx.begin(), strx.end());

	if (wstr.empty() || Gender.empty())
		return;

	VAction(wstr.c_str(), Gender);*/
}

bool VoiceSystemInfo(DWORD Addy, std::string Key)
{
	try
	{

		if (Key == "5c28a98077")
		{
		//	cSound Sound;

		//	Voice(GetValue(Addy + 0x20), GetValue(Addy + 0x38));
			/*if (Gender == "Male")
			{
				if (Action == "Go Go Go")
					Sound.wav.playsoundmem((LPCSTR)T1.data());
				else if (Action == "Attack")
					Sound.wav.playsoundmem((LPCSTR)T2.data());
				else if (Action == "Defend")
					Sound.wav.playsoundmem((LPCSTR)T3.data());
				else if (Action == "Wait me")
					Sound.wav.playsoundmem((LPCSTR)T4.data());
				else if (Action == "Run")
					Sound.wav.playsoundmem((LPCSTR)T5.data());
				else if (Action == "Watch your back")
					Sound.wav.playsoundmem((LPCSTR)T6.data());
				else if (Action == "Cover me")
					Sound.wav.playsoundmem((LPCSTR)T7.data());
				else if (Action == "Out of ammo")
					Sound.wav.playsoundmem((LPCSTR)T8.data());
				else if (Action == "Heal me")
					Sound.wav.playsoundmem((LPCSTR)T9.data());
			}

			if (Gender == "Female")
			{
				if (Action == "Go Go Go")
					Sound.wav.playsoundmem((LPCSTR)FT1.data());
				else if (Action == "Attack")
					Sound.wav.playsoundmem((LPCSTR)FT2.data());
				else if (Action == "Defend")
					Sound.wav.playsoundmem((LPCSTR)FT3.data());
				else if (Action == "Wait me")
					Sound.wav.playsoundmem((LPCSTR)FT4.data());
				else if (Action == "Run")
					Sound.wav.playsoundmem((LPCSTR)FT5.data());
				else if (Action == "Watch your back")
					Sound.wav.playsoundmem((LPCSTR)FT6.data());
				else if (Action == "Cover me")
					Sound.wav.playsoundmem((LPCSTR)FT7.data());
				else if (Action == "Out of ammo")
					Sound.wav.playsoundmem((LPCSTR)FT8.data());
				else if (Action == "Heal me")
					Sound.wav.playsoundmem((LPCSTR)FT9.data());
			}
			Mem.WriteASM(WeaponSwitch_Offset, (DWORD)"\x74", 1);*/
		}

		if (Key == "594bf2262b")
		{
			/*std::string strx = GetValue(Addy + 0x20);
			std::wstring wstr = std::wstring(strx.begin(), strx.end());
	
			
			BU_Gender = Gender;
			Gender = GetValue(Addy + 0x38);
			VAction(wstr.c_str());
			Gender = BU_Gender;*/
			//CustomVoice(GetValue(Addy + 0x20), GetValue(Addy + 0x38));
		//	ChatSystemStruc* chatSystemStruc = new ChatSystemStruc();

			//	chatSystemStruc->Time = "@7d7d7d}" + std::string(buffer);
		//	chatSystemStruc->Nickname = GetValue(Addy + 0x50);
		//	chatSystemStruc->Message = ChatMessageColor(Chat_Type::Team) + strx; to do

			//RoomChatSystemList[RoomChatSystemList.size() + 1] = chatSystemStruc;
			//TeamSystemList[TeamSystemList.size() + 1] = chatSystemStruc;
			return true;
		}

		if (Key == "6cbcd4d598")
		{
			//Gender = GetValue(Addy + 0x38);
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Voice System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Voice System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ServerCheck(DWORD Addy, std::string Key)
{
	if (Key == "1ae29e6e68")
	{
		std::string value = GetValue(Addy + 0x20);

		if (value != "S4League")
		{
			ExitProcess(0);
			__asm call dwBaseAddr + 0x134748
		}
	}
}

bool AccountInfoSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (plr_loc >= Player_Loc::Playing)
			return false;

		if (Key == "2be29e6e68")
		{
			PlayerHwid = GetValue(Addy + 0x20);

			MyPlayerId = Mem.StdToInt(GetValue(Addy + 0x38));

			AccountInfo = GetValue(Addy + 0x50);

			MyNickName = Mem.Split(AccountInfo, '|')[0];
			MyLevel = Mem.Split(AccountInfo, '|')[1];
			MyRankPoint = Mem.StdToInt(Mem.Split(AccountInfo, '|')[2]);
			MyClanRank = Mem.Split(AccountInfo, '|')[3];

			ClanMarkTexture = ClubsTexture(Mem.Split(AccountInfo, '|')[4]);

			Gender = Mem.Split(AccountInfo, '|')[5];

			if (PlayerHwid.empty())
				SendInfoToServer(PlayerHwid, "NX", "3be29e6e68");
			return true;
		}

		//Player Rank
		if (Key == "e87a1v7nce")
		{
			MyRankPoint = Mem.StdToInt(GetValue(Addy + 0x38));
			RankTexture = CPlayerListTexture(RankPointName(MyRankPoint), 1);
			return true;
		}

		//Custom NameTag
		if (Key == "0a94b514d6" && !GetValue(Addy + 0x38).empty())
		{
			CustomTagItemId = GetValue(Addy + 0x38);
			CCustomNameTagUI = true;
			return true;
		}

		if (Key == "f0151cde46")
		{
			MyLevel = GetValue(Addy + 0x38);
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Account Info System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Account Info System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool CommunitySystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "d002c470d1")
		{
			//FriendsList
			auto FriendsList = GetValue(Addy + 0x20);
			auto DenyList = GetValue(Addy + 0x38);
			auto ClanList = GetValue(Addy + 0x50);
			auto UnionList = GetValue(Addy + 0x68);

			FrindsPlayerList.clear();
			BlockPlayerList.clear();
			ClanPlayerList.clear();
			UnionPlayerList.clear();

			if (FriendsList != "None")
			{
				for (std::string element : Mem.Split(FriendsList, ','))
				{
					if (element.empty())
						continue;

					PlayersShortInfoListClass* it = new PlayersShortInfoListClass();
					it->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(it->Name, Nickname.length() + 1, Nickname.c_str());
					it->Level = Mem.Split(element, '|')[3];
					it->FriendStats = Mem.Split(element, '|')[4];
					it->BlockStats = Mem.Split(element, '|')[5];
					it->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[6])), 1);
					FrindsPlayerList[FrindsPlayerList.size() + 1] = it;
				}
			}

			if (DenyList != "None")
			{
				for (std::string element : Mem.Split(DenyList, ','))
				{
					if (element.empty())
						continue;

					PlayersShortInfoListClass* it = new PlayersShortInfoListClass();
					it->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(it->Name, Nickname.length() + 1, Nickname.c_str());
					it->Level = Mem.Split(element, '|')[3];
					it->FriendStats = Mem.Split(element, '|')[4];
					it->BlockStats = Mem.Split(element, '|')[5];
					it->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[6])), 1);
					BlockPlayerList[BlockPlayerList.size() + 1] = it;
				}
			}
			
			if (ClanList != "None")
			{
				for (std::string element : Mem.Split(ClanList, ','))
				{
					if (element.empty())
						continue;

					PlayerClanShortInfoListClass* it = new PlayerClanShortInfoListClass();
					it->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(it->Name, Nickname.length() + 1, Nickname.c_str());
					it->Level = Mem.Split(element, '|')[3];
					it->FriendStats = Mem.Split(element, '|')[4];
					it->BlockStats = Mem.Split(element, '|')[5];
					it->ClanIcon = ClubsTexture(Mem.Split(element, '|')[6]);
					it->ClanRank = Mem.Split(element, '|')[7];
					it->StaffIcon = CPlayerListTexture(Mem.Split(element, '|')[7], 3);
					it->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[8])), 1);
					ClanPlayerList[ClanPlayerList.size() + 1] = it;
				}
			}

			if (UnionList != "None")
			{
				for (std::string element : Mem.Split(UnionList, ','))
				{
					if (element.empty())
						continue;

					PlayerClanShortInfoListClass* it = new PlayerClanShortInfoListClass();
					it->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(it->Name, Nickname.length() + 1, Nickname.c_str());
					it->Level = Mem.Split(element, '|')[3];
					it->FriendStats = Mem.Split(element, '|')[4];
					it->BlockStats = Mem.Split(element, '|')[5];
					it->ClanIcon = ClubsTexture(Mem.Split(element, '|')[6]);
					it->ClanRank = Mem.Split(element, '|')[7];
					it->StaffIcon = CPlayerListTexture(Mem.Split(element, '|')[7], 3);
					it->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[8])), 1);
					UnionPlayerList[UnionPlayerList.size() + 1] = it;
				}
			}
			CommunityUI = true;
			return true;
		}
	}
	catch (std::exception& e)
	{
		//LogCrash("Player List System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Player List System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool PlayerListSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (plr_loc != Player_Loc::RoomList)
			return false;

		//Player List
		if (Key == "abc288734c")// Channel Enter
		{
			auto element = GetValue(Addy + 0x38);

			unsigned int PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);

			if (MyPlayerId != PlayerId)
			{
				NewPlayerList* ChannelPlayerlist = new NewPlayerList();
				std::string  Nickname = Mem.Split(element, '|')[2];
				strcpy_s(ChannelPlayerlist->Nickname, Nickname.length() + 1, Nickname.c_str());
				ChannelPlayerlist->Level = Mem.Split(element, '|')[3];
				ChannelPlayerlist->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[4])), 1);
				ChannelPlayerlist->FriendStats = Mem.Split(element, '|')[5];
				ChannelPlayerlist->BlockStats = Mem.Split(element, '|')[6];
				ChannelPlayerList[PlayerId] = ChannelPlayerlist;
			}
	
			return true;
		}

		if (Key == "dbc288734c")// Player List
		{
			ChannelPlayerList.clear();
			auto List = GetValue(Addy + 0x38);
		
			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;
			
				unsigned int PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
				if (MyPlayerId != PlayerId)
				{
					NewPlayerList* ChannelPlayerlist = new NewPlayerList();
					std:: string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(ChannelPlayerlist->Nickname, Nickname.length() + 1, Nickname.c_str());
					ChannelPlayerlist->Level = Mem.Split(element, '|')[3];
					ChannelPlayerlist->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[4])), 1);
					ChannelPlayerlist->FriendStats = Mem.Split(element, '|')[5];
					ChannelPlayerlist->BlockStats = Mem.Split(element, '|')[6];
					ChannelPlayerList[PlayerId] = ChannelPlayerlist;
				}
			}
			return true;
		}

		//Clan Member List
		if (Key == "d7f95969a6")// Player List
		{
			ClanMemberList.clear();
			auto List = GetValue(Addy + 0x38);

			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				unsigned int PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);

				if (MyPlayerId != PlayerId)
				{
					NewPlayerList* ClanPlayerlist = new NewPlayerList();
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(ClanPlayerlist->Nickname, Nickname.length() + 1, Nickname.c_str());
					ClanPlayerlist->Level = Mem.Split(element, '|')[3];
					ClanPlayerlist->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[4])), 1);
					ClanPlayerlist->FriendStats = Mem.Split(element, '|')[5];
					ClanPlayerlist->BlockStats = Mem.Split(element, '|')[6];
					ClanPlayerlist->ClanRank = Mem.Split(element, '|')[7];

					ClanMemberList[PlayerId] = ClanPlayerlist;
				}
			}
			return true;
		}

		//Union Member List
		if (Key == "d54ca8cf7f")// Player List
		{
			UnionMemberList.clear();
			auto List = GetValue(Addy + 0x38);
			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				unsigned int PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);

				if (MyPlayerId != PlayerId)
				{
					NewPlayerList* UnionPlayerlist = new NewPlayerList();
					std::string Nickname = Mem.Split(element, '|')[2];
					strcpy_s(UnionPlayerlist->Nickname, Nickname.length() + 1, Nickname.c_str());
					UnionPlayerlist->Level = Mem.Split(element, '|')[3];
					UnionPlayerlist->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[4])), 1);
					UnionPlayerlist->FriendStats = Mem.Split(element, '|')[5];
					UnionPlayerlist->BlockStats = Mem.Split(element, '|')[6];
					UnionPlayerlist->ClanRank = Mem.Split(element, '|')[7];

					UnionMemberList[PlayerId] = UnionPlayerlist;
				}
			}
			return true;
		}

		//Support
		/*if (Key == "59ff1384fb")// Player List
		{
			SupportPlayerList.clear();

			auto List = GetValue(Addy + 0x38);

			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				NewPlayerList* SupportPlayerlist = new NewPlayerList();
				unsigned int PlayerId = Mem.StdToInt(Mem.Split(element, '|')[1]);
				std::string Nickname = Mem.Split(element, '|')[2];
				strcpy_s(SupportPlayerlist->Nickname, Nickname.length() + 1, Nickname.c_str());
				SupportPlayerlist->Level = Mem.Split(element, '|')[3];
				SupportPlayerlist->Rank = CPlayerListTexture(RankPointName(Mem.StdToInt(Mem.Split(element, '|')[4])), 1);
				SupportPlayerlist->FriendStats = Mem.Split(element, '|')[5];
				SupportPlayerlist->BlockStats = Mem.Split(element, '|')[6];
				SupportPlayerList[PlayerId] = SupportPlayerlist;
			}	
			return true;
		}*/

		if (Key == "c1ea32be29")
		{
			ClanMemberList.clear();
			UnionMemberList.clear();
		}

		if (Key == "1d84fbefc7")// player leave to do offline state in friend block list
		{

			auto cit = ClanMemberList.find(Mem.StdToInt(GetValue(Addy + 0x038)));

			if (cit != ClanMemberList.end())
			{
				/*	if (it->first == MyPlayerId)
						return;*/

				ClanMemberList.erase(cit);
				return true;
			}

			auto uit = UnionMemberList.find(Mem.StdToInt(GetValue(Addy + 0x038)));

			if (uit != UnionMemberList.end())
			{
				/*	if (it->first == MyPlayerId)
						return;*/

				UnionMemberList.erase(uit);
				return true;
			}

			auto it = WhisperSystemList.find(Mem.StdToInt(GetValue(Addy + 0x038)));
			if (it != WhisperSystemList.end())
			{
				/*	if (it->first == MyPlayerId)
						return;*/

				WhisperSystemList.erase(it);
				return true;
			}

			/*for (auto friendx : FriendsPlayerList)
			{
				if (friendx.second->PlayerId == Mem.StdToInt(GetValue(Addy + 0x038)))
					FriendsPlayerList.erase(friendx.first);
			}


			for (auto block : BlockPlayerList)
			{
				if (block.second->PlayerId == Mem.StdToInt(GetValue(Addy + 0x038)))
					BlockPlayerList.erase(block.first);
			}

			for (auto unionx : UnionPlayerList)
			{
				if (unionx.second->PlayerId == Mem.StdToInt(GetValue(Addy + 0x038)))
					UnionPlayerList.erase(unionx.first);
			}*/
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Player List System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Player List System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool MessageBoxSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "a54fb77019")
		{

			CustomMessageBox(GetValue(Addy + 0x20), Mem.StdToInt(GetValue(Addy + 0x38)), (Msgbox)Mem.StdToInt(GetValue(Addy + 0x50)));
			return true;
		}

		if (Key == "321a767620" && GetValue(Addy + 0x38) == "Invalid amount of PEN.")
		{
			memset(CPPEN, 0, sizeof CPPEN);
			CustomMessageBox("Invalid amount of PEN.", 1, Msgbox::InvalidAmountPEN);
			return true;
		}

		if (Key == "321a767620" && GetValue(Addy + 0x38) == "Invalid amount of LP.")
		{
			memset(CPLP, 0, sizeof CPLP);
			CustomMessageBox("Invalid amount of LP.", 1, Msgbox::InvalidAmountLP);
			return true;
		}

		if (Key == "431a767620" && GetValue(Addy + 0x38) == "Invalid amount of PEN.")
		{
			memset(P1PEN, 0, sizeof P1PEN);
			CustomMessageBox("Invalid amount of PEN.", 1, Msgbox::InvalidAmountPEN);
			return true;
		}

		if (Key == "431a767620" && GetValue(Addy + 0x38) == "Invalid amount of LP.")
		{
			memset(P1LP, 0, sizeof P1LP);
			CustomMessageBox("Invalid amount of LP.", 1, Msgbox::InvalidAmountLP);
			return true;
		}

		if (Key == "0dabd226d5" && GetValue(Addy + 0x38) == "Invalid amount of PEN.")
		{
			memset(P2PEN, 0, sizeof P2PEN);
			CustomMessageBox("Invalid amount of PEN.", 1, Msgbox::InvalidAmountPEN);
			return true;
		}

		if (Key == "0dabd226d5" && GetValue(Addy + 0x38) == "Invalid amount of LP.")
		{
			memset(P2LP, 0, sizeof P2LP);
			CustomMessageBox("Invalid amount of LP.", 1, Msgbox::InvalidAmountLP);
			return true;
		}

		//S4 MsgBox
		if (Key == "a53e79a219")
		{
			SMsgBox(GetValue(Addy + 0x38));
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("MessageBox System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("MessageBox System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool CharactersProfileSystem(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "ac40cf9c0e")
		{
			ProfileItemInfoList.clear();
			
			auto List = GetValue(Addy + 0x38);

			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				ProfileItemInfo* ProfileItemInfolist = new ProfileItemInfo();
				ProfileItemInfolist->Name = Mem.Split(element, '|')[0];
				ProfileItemInfolist->ItemName[0] = Mem.Split(element, '|')[1];
				ProfileItemInfolist->ImageName[0] = Mem.Split(element, '|')[2];
				ProfileItemInfolist->Effects[0] = Mem.Split(element, '|')[3];

				ProfileItemInfolist->ItemName[1] = Mem.Split(element, '|')[4];
				ProfileItemInfolist->ImageName[1] = Mem.Split(element, '|')[5];
				ProfileItemInfolist->Effects[1] = Mem.Split(element, '|')[6];

				ProfileItemInfolist->ItemName[2] = Mem.Split(element, '|')[7];
				ProfileItemInfolist->ImageName[2] = Mem.Split(element, '|')[8];
				ProfileItemInfolist->Effects[2] = Mem.Split(element, '|')[9];

				ProfileItemInfolist->ItemName[3] = Mem.Split(element, '|')[10];
				ProfileItemInfolist->ImageName[3] = Mem.Split(element, '|')[11];
				ProfileItemInfolist->Effects[3] = Mem.Split(element, '|')[12];

				ProfileItemInfolist->ItemName[4] = Mem.Split(element, '|')[13];
				ProfileItemInfolist->ImageName[4] = Mem.Split(element, '|')[14];
				ProfileItemInfolist->Effects[4] = Mem.Split(element, '|')[15];

				ProfileItemInfolist->ItemName[5] = Mem.Split(element, '|')[16];
				ProfileItemInfolist->ImageName[5] = Mem.Split(element, '|')[17];
				ProfileItemInfolist->Effects[5] = Mem.Split(element, '|')[18];

				ProfileItemInfolist->ItemName[6] = Mem.Split(element, '|')[19];
				ProfileItemInfolist->ImageName[6] = Mem.Split(element, '|')[20];
				ProfileItemInfolist->Effects[6] = Mem.Split(element, '|')[21];

				ProfileItemInfolist->ItemName[7] = Mem.Split(element, '|')[22];
				ProfileItemInfolist->ImageName[7] = Mem.Split(element, '|')[23];
				ProfileItemInfolist->Effects[7] = Mem.Split(element, '|')[24];

				ProfileItemInfolist->ItemName[8] = Mem.Split(element, '|')[25];
				ProfileItemInfolist->ImageName[8] = Mem.Split(element, '|')[26];
				ProfileItemInfolist->Effects[8] = Mem.Split(element, '|')[27];

				ProfileItemInfolist->ItemName[9] = Mem.Split(element, '|')[28];
				ProfileItemInfolist->ImageName[9] = Mem.Split(element, '|')[29];
				ProfileItemInfolist->Effects[9] = Mem.Split(element, '|')[30];

				ProfileItemInfolist->ItemName[10] = Mem.Split(element, '|')[31];
				ProfileItemInfolist->ImageName[10] = Mem.Split(element, '|')[32];
				ProfileItemInfolist->Effects[10] = Mem.Split(element, '|')[33];

				ProfileItemInfolist->ItemName[11] = Mem.Split(element, '|')[34];
				ProfileItemInfolist->ImageName[11] = Mem.Split(element, '|')[35];
				ProfileItemInfolist->Effects[11] = Mem.Split(element, '|')[36];

				ProfileItemInfoList[ProfileItemInfoList.size()] = ProfileItemInfolist;
			}
			CharactersProfileUI = true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Characters Profile System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Characters Profile System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool VoteKickSystemInfo(DWORD Addy, std::string Key)
{
	try
	{
		if (Key == "e3b03e8682")
		{
			VoteKickList.clear();

			auto List = GetValue(Addy + 0x38);

			if (List.empty() || List == "")
				return true;

			for (std::string element : Mem.Split(List, ','))
			{
				if (element.empty())
					continue;

				PlayerShortInfoListClass* PlayerShortInfoListclass = new PlayerShortInfoListClass();
				std::string Name = Mem.Split(element, '|')[0];
				strcpy_s(PlayerShortInfoListclass->Name, Name.length() + 1, Name.c_str());
				PlayerShortInfoListclass->Level = Mem.Split(element, '|')[1];
				PlayerShortInfoListclass->PlayerId = Mem.StdToInt(Mem.Split(element, '|')[2]);
				VoteKickList[VoteKickList.size() + 1] = PlayerShortInfoListclass;
			}

			CGameMenuUI = false;
			VoteKickUI = true;
			return true;
		}
	}
	catch (std::exception & e)
	{
		//LogCrash("Vote Kick System \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
	//	//LogCrash("Vote Kick System \n" + boost::current_exception_diagnostic_information());
	}
	return false;
}

bool ServInfo(DWORD eax)
{
	std::string Key;

	if (Mem.ReadStr(eax + 0x08) == "0ae29e6e68")
	{
		std::string EnDecKey = Mem.ReadStr(eax + 0x38);
		strcpy_s(PlayerEnDecKey, EnDecKey.length() + 1, EnDecKey.c_str());
	}

	Key = GetValue(eax + 0x08, false);

	if (Key.empty())
		return false;
	
	if (ServerCheck(eax, Key))
		return false;
	if (AccountInfoSystemInfo(eax, Key))
		return false;
	if (ShopSystemInfo(eax, Key))
		return false;
	if (VoiceSystemInfo(eax, Key))
		return false;
	if (EventsSystemInfo(eax, Key))
		return false;
	if (PlayerReportSystemInfo(eax, Key))
		return false;
	if (SurrenderSystemInfo(eax, Key))
		return false;
	if (QuestsSystemInfo(eax, Key))
		return false;
	if (BattleFieldSystemInfo(eax, Key))
		return false;
	if (RoomOptionSystemInfo(eax, Key))
		return false;
	if (ChatSystemInfo(eax, Key))
		return false;
	if (ExchangeSystemInfo(eax, Key))
		return false;
	if (ChallengeSystemInfo(eax, Key))
		return false;
	if (ClanSystemInfo(eax, Key))
		return false;
	if (CommunitySystemInfo(eax, Key))
		return false;
	if (PlayerListSystemInfo(eax, Key))
		return false;
	if (MessageBoxSystemInfo(eax, Key))
		return false;
	if (CharactersProfileSystem(eax, Key))
		return false;
	if (VoteKickSystemInfo(eax, Key))
		return false;

	return false;
}
#pragma endregion 

#pragma region ASM
std::string ScreenRaito()
{
	std::string RatioType;
	DWORD RaitoAddy = Mem.FindOffAAddy(Screen_Raito, { 0x30 });
	if (Mem.ReadMem(RaitoAddy) == 0)
		RatioType = "4:3";
	else if (Mem.ReadMem(RaitoAddy) == 1)
		RatioType = "16:9";
	else if (Mem.ReadMem(RaitoAddy) == 2)
		RatioType = "16:10";
	else if (Mem.ReadMem(RaitoAddy) == 3)
		RatioType = "5:3";
	else if (Mem.ReadMem(RaitoAddy) == 4)
		RatioType = "5:4";

	return RatioType;
}

_declspec(naked) void ClientSide(void)
{
	_asm
	{
		mov eax, [ebp + 0x10]
		push eax
		mov[GetRevSevInfo], eax
		pushad
	}

	ServInfo(GetRevSevInfo);

	__asm
	{
		popad
		mov ecx, [ebp - 0x14]
		jmp RevSevInfo_Back;
	}
}

_declspec(naked) void FPSUnLock(void)
{
	_asm
	{
		cvttsd2si edx, xmm0
		add edx, FPSValue
		jmp UnLockFPS_Back;
	}
}

void GUIControl(DWORD GUIStatus)
{
	if (GUIStatus == 2) 
	{
		ServerListInfos();
		CleanUp();
	}

	if (GUIStatus == 5)
	{
		plr_loc = Player_Loc::Channel;

		CChannelUI = true;
		CPlayerListUI = false;

		std::string nickname;
		if (MyNickName.find("{") != std::string::npos)
			nickname = Mem.Split(MyNickName, '}')[1];
		else
			nickname = MyNickName;
		printf("nickname %s\n", nickname.c_str());
		discord->Update(nickname + " - Channels", TotalMatchs + " " + Win + " " + Loss + " " + WinLossPercentage, eptime, "evolved", "", "Channels");
	}
	else
	{
		CChannelUI = false;
	//	CPlayerListUI = false;
	}

	if (GUIStatus == 6 || GUIStatus == 12 || GUIStatus == 13)
	{
		fov = 0;

		plr_loc = Player_Loc::RoomList;
		CChannelUI = false;
		CChatSystemUI = true;
		CPlayerListUI = true;
		RoomCleanUp();
		if (!IsLoadTexture2)
		{
			IsLoadTexture2 = true;
	
			MenuButtonTexture = LoadTextrueEx("Menu Button");
			ButtonTexture = LoadTextrueEx("Button");
			MenuShopButtonTexture = LoadTextrueEx("Shop");
			MenuInventoryButtonTexture = LoadTextrueEx("Inventory");
			MenuCardButtonTexture = LoadTextrueEx("Card");
			MenuSettingButtonTexture = LoadTextrueEx("Settings");
			MenuInviteButtonTexture = LoadTextrueEx("Invite");
			MenuHelpButtonTexture = LoadTextrueEx("Help");
			MenuEnchantsButtonTexture = LoadTextrueEx("Enchants");
			MenuCommunityButtonTexture = LoadTextrueEx("Community");
			MenuRandomShopButtonTexture = LoadTextrueEx("RandomShop");
			MenuMessagesButtonTexture = LoadTextrueEx("Messages");
			MenuMissionButtonTexture = LoadTextrueEx("Mission");
			MenuClanButtonTexture = LoadTextrueEx("Clan");
			MenuActivatedButtonTexture = LoadTextrueEx("Selected Menu Button");
			BackGroundTexture = LoadTextrueEx("Main BackGround");

			EvTextture = LoadTextrueEx("eVolved");

			VoteKickPlayerUITexture = LoadTextrueEx("Vote Kick");
			ClanUITexture = LoadTextrueEx("Clan_System");

			if (DailyEventEnable)
			{
				CDailyRewardUI = true;
				DailyEventEnable = false;
			}
		}

		if (!IsLoadTexture)
		{
			IsLoadTexture = true;
			LoadTexture();

			//Touch Down
			/*auto it = GameModeTexture.find(std::to_string(GameMode));
			if (it != GameModeTexture.end())
			  SelectedGameModeTexture = TDTexture = it->second;
			auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
			if (it2 != ActivatedGameModeTexture.end())
				ASelectedGameModeTexture = ActivatedTDTexture = it2->second;

			//after done with all game mode image do a list to load others modes image like the exmple down there
			//Touch Down Pass
			auto passtd = GameModeTexture.find(std::to_string(21));
			if (passtd != GameModeTexture.end())
				TDPassTexture = passtd->second;
			auto apasstd = ActivatedGameModeTexture.find(std::to_string(21));
			if (apasstd != ActivatedGameModeTexture.end())
				ActivatedTDPassTexture = apasstd->second;
			//Death Match
			auto dm = GameModeTexture.find(std::to_string(1));
			if (dm != GameModeTexture.end())
				DMTexture = dm->second;
			auto adm = ActivatedGameModeTexture.find(std::to_string(1));
			if (adm != ActivatedGameModeTexture.end())
				ActivatedDMTexture = adm->second;
			//Battle Royale
			auto br = GameModeTexture.find(std::to_string(9));
			if (br != GameModeTexture.end())
				BRTexture = br->second;
			auto abr = ActivatedGameModeTexture.find(std::to_string(9));
			if (abr != ActivatedGameModeTexture.end())
				ActivatedBRTexture = abr->second;
			//Captain
			auto captain = GameModeTexture.find(std::to_string(10));
			if (captain != GameModeTexture.end())
				CaptainTexture = captain->second;
			auto acaptain = ActivatedGameModeTexture.find(std::to_string(10));
			if (acaptain != ActivatedGameModeTexture.end())
				ActivatedCaptainTexture = acaptain->second;
			//Chaser
			auto chaser = GameModeTexture.find(std::to_string(8));
			if (chaser != GameModeTexture.end())
				ChaserTexture = chaser->second;
			auto achaser = ActivatedGameModeTexture.find(std::to_string(8));
			if (achaser != ActivatedGameModeTexture.end())
				ActivatedChaserTexture = achaser->second;
			//Warfare
			auto warfare = GameModeTexture.find(std::to_string(15));
			if (warfare != GameModeTexture.end())
				WarfareTexture = warfare->second;
			auto awarfare = ActivatedGameModeTexture.find(std::to_string(15));
			if (awarfare != ActivatedGameModeTexture.end())
				ActivatedWarfareTexture = awarfare->second;
			//Siege
			auto siege = GameModeTexture.find(std::to_string(11));
			if (siege != GameModeTexture.end())
				SiegeTexture = siege->second;
			auto asiege = ActivatedGameModeTexture.find(std::to_string(11));
			if (asiege != ActivatedGameModeTexture.end())
				ActivatedSiegeTexture = asiege->second;
			//Arena
			auto arena = GameModeTexture.find(std::to_string(11));
			if (arena != GameModeTexture.end())
				ArenaTexture = arena->second;
			auto aarena = ActivatedGameModeTexture.find(std::to_string(11));
			if (aarena != ActivatedGameModeTexture.end())
				ActivatedArenaTexture = aarena->second;
		
			// PVE   Arcade/Conquest
			auto pve = GameModeTexture.find(std::to_string(111));
			if (pve != GameModeTexture.end())
				ArcadeTexture = pve->second;
			auto apve = ActivatedGameModeTexture.find(std::to_string(111));
			if (apve != ActivatedGameModeTexture.end())
				ActivatedArcadeTexture = apve->second;*/


			PlayerClanShortInfoListClass* payerclan = new PlayerClanShortInfoListClass();

	
			std::string Nickname = "test";
			strcpy_s(payerclan->Name, Nickname.length() + 1, Nickname.c_str());
			payerclan->Level = "80";
			payerclan->Rank = CPlayerListTexture("Legendary", 1);
			payerclan->StaffIcon = CPlayerListTexture("Master", 3);
			payerclan->ClanIcon = ClubsTexture("logo_50");
			ClanPlayerList[ClanPlayerList.size() + 1] = payerclan;
		}
	

		std::string nickname;
		if (MyNickName.find("{") != std::string::npos)
			nickname = Mem.Split(MyNickName, '}')[1];
		else
			nickname = MyNickName;

		discord->Update(nickname + " - Lobby", TotalMatchs + " " + Win + " " + Loss + " " + WinLossPercentage, eptime, "evolved", "", "Lobby");

	}
	else
	{
		CChatSystemUI = false;
		ChatButtonMenuLock = false;
		CPlayerListUI = false;
	}

	if (GUIStatus == 8)
		InventoryProfileButtonsUI = true;
	else
		InventoryProfileButtonsUI = false;

	if (GUIStatus == 7)
		ShopSearch = true;
	else
		ShopSearch = false;
	
	if (GUIStatus == 10 )
	{

		fov = 0;
	}
	else
	{
		CRoomChatSystemUI = false;
	}

	if (GUIStatus == 10 || GUIStatus == 19 || GUIStatus == 17)
	{
	}
}

_declspec(naked) void GUIStatus_Hook(void)
{
	_asm
	{
		mov ecx, [ebp + 0x08]
		mov[eax + 0x2C], ecx
		mov[GUIStatus], ecx
		pushad
	}
	GUIControl(GUIStatus);
	__asm
	{
		popad
		jmp GUI_Status_Back;
	}
}

void ResetFoveSetting()
{
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);

}

void HideMouse()
{
	Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
}

_declspec(naked) void SettingClose_Hook(void)
{
	_asm
	{
		mov[ebp - 0x10], eax
		mov ecx, [ebp - 0x10]
		pushad
	}
	HideMouse();
	ResetFoveSetting();
	__asm
	{
		popad
		jmp SettingClose_Back;
	}
}

_declspec(naked) void SettingClose2_Hook(void)
{
	_asm
	{
		mov[ebp - 0x18], eax
		mov ecx, [ebp - 0x18]
		pushad
	}
	HideMouse();
	ResetFoveSetting();
	__asm
	{
		popad
		jmp SettingClose2_Back;
	}
}

_declspec(naked) void PlayerCamChange_Hook(void)
{
	_asm
	{
		add eax, 0x01
		mov ecx, [ebp - 0x04]
		pushad
	}
	ResetFoveSetting();
	__asm
	{
		popad
		jmp PlayerCamChange_Back;
	}
}


void PlrLeaveChannel(DWORD PlayerId)
{
	//if (MyPlayerId == PlayerId)
	//	return;

	auto it = ChannelPlayerList.find(PlayerId);
	if (it != ChannelPlayerList.end())
	{
		ChannelPlayerList.erase(it);
	}


}

_declspec(naked) void PlayerLeaveChannel_Hook(void)
{
	_asm
	{
		mov edx, [eax]
		push edx
		mov[PlayerLeaveChannelId], edx
		pushad
	}
	PlrLeaveChannel(PlayerLeaveChannelId);
	__asm
	{
		popad
		mov ecx, [ebp - 0x08]
		jmp PlayerLeaveChannel_Back;
	}
}


_declspec(naked) void ShopButton_Hook(void)
{
	_asm
	{
		mov byte ptr[ebp - 0x04], 00
		pushad
	}
	IsBuyButtonPressed = true;
	//CGameShopUI = true;
	_asm
	{
		popad
		mov ecx, [ebp - 0x18]
		jmp Shop_Buy_Button_Back;
	}
}

void ItemShopInfo(DWORD Id)
{
	if (!IsBuyButtonPressed)
		return;
	ItemId = Id;

	SendInfoToServer(std::to_string(ItemId), "SHP", "ac8b9de9c5");
	IsBuyButtonPressed = false;
}

_declspec(naked) void ShopItemId_Hook(void)
{
	_asm
	{


		mov ecx, 0x000F4240
		mov[ShopItemId], eax

		div ecx


		//div ecx


		pushad
	}

	ItemShopInfo(ShopItemId);
	_asm
	{
		popad

		jmp Read_Shop_ItemId_Back;
	}
}

void ShopItemColor(DWORD Id)
{
//	if (!IsBuyButtonPressed)
	//	return;
	ShopItemCurrentColor = Mem.ReadMem(Id + 0x10);
}

_declspec(naked) void ShopItemColorId_Hook(void)
{
	_asm
	{
		mov[ebp - 0x04], 0xFFFFFFFF
		mov[ShopItemColorId], eax
		mov ecx, [ebp + 0x08]
		pushad
	}
	ShopItemColor(ShopItemColorId);
	_asm
	{
		popad
		push ecx
		jmp Read_Shop_ItemColorId_Back;
	}
}


void PlayerFOV(DWORD ecx)
{
	if (plr_loc == Player_Loc::Playing)
	{
		Mem.WriteMem(ecx + 0x100, AimPos * 10);
		Mem.WriteMem(ecx + 0x104, AimPos * 10);

		if (ActiveCharCam)
		{
			Mem.WriteMem(ecx + 0x108, CharCam * 10);
			Mem.WriteMem(ecx + 0x10C, CharCam * 10);
		}

		Mem.WriteMem(ecx + 0x154, Fov);
		Mem.WriteMem(ecx + 0x158, Fov);
	}
}

_declspec(naked) void PlayerFOV_Hook(void)
{
	_asm
	{
		//mov eax, [edx + 0x00000104]
		movss xmm0, [ecx + 0x00000154]

		mov[PlayerFov_Addy], ecx

		pushad
	}
	PlayerFOV(PlayerFov_Addy);
	_asm
	{
		popad
		ucomiss xmm0, [edx + 0x00000158]

	//	mov[ecx + 0x00000100], eax
		jmp PlayerFov_Back;
	}
}

void roomtitle()
{
	std::string RandomRoomNames[]
		= { "S4 Zen!", "S4 Zen Time!", "Zen Powa!", "Zen" };

	int rnd = rand() % 4;

		strcpy_s(RoomTitle, RandomRoomNames[rnd].length() + 1, RandomRoomNames[rnd].c_str());
}

_declspec(naked) void MakeRoomButton(void)
{
	_asm
	{
		mov[ebp - 0x10], eax
		mov ecx, [ebp - 0x10]
		pushad
	}

	roomtitle();
	CMakingRoomUI = true;
	_asm
	{
		popad
		jmp MakeRoom_Back
	}
}

_declspec(naked) void InRoomMakeRoomButton(void)
{
	_asm
	{
		mov ebp, esp
		push ecx
		mov[ebp - 0x04], ecx
		pushad
	}
	CMakingRoomUI = true;
	_asm
	{
		popad
		jmp  InRoomMakeRoom_Back
	}
}
#pragma endregion 

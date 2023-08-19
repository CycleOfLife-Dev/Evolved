#include "Funcations.h"
#include "Unlinking.h"
#include "detours.h"
#include <sapi.h>
#include <d3d9.h>
#include "Splash.h"

void ActiveS4Stuff()
{
	Mem.WriteASM(Block_Shop_Window, (DWORD)"\xEB", 1);
	Mem.WriteASM(Mem.FindOffAAddy(dwBaseAddr + 0x0173D7A4, { 0x14, 0x0, 0x2C, 0x2A4 }), (DWORD)"\x01", 1);
	Show_Screen = reinterpret_cast<p_Show_Screen>(dwBaseAddr + Show_Screen_Offset);
	Show_Screen_Instance = dwBaseAddr + Show_Screen_Instance_Offset;

	RevSevInfo = dwBaseAddr + RevSevInfo_Offset;
	RevSevInfo_Back = RevSevInfo + 0x7;
	Mem.CreateDetour((PBYTE)RevSevInfo, (PBYTE)ClientSide, 0x7);

	SettingClose = dwBaseAddr + SettingClose_Offset;
	SettingClose_Back = SettingClose + 0x6;
	Mem.CreateDetour((PBYTE)SettingClose, (PBYTE)SettingClose_Hook, 0x6);

	SettingClose2 = dwBaseAddr + SettingClose2_Offset;
	SettingClose2_Back = SettingClose2 + 0x6;
	Mem.CreateDetour((PBYTE)SettingClose2, (PBYTE)SettingClose2_Hook, 0x6);

	PlayerCamChange = dwBaseAddr + PlayerCamChange_Offset;
	PlayerCamChange_Back = PlayerCamChange + 0x6;
	Mem.CreateDetour((PBYTE)PlayerCamChange, (PBYTE)PlayerCamChange_Hook, 0x6);

	GUI_Status = dwBaseAddr + GUI_Status_Offset;
	GUI_Status_Back = GUI_Status + 0x6;
	Mem.CreateDetour((PBYTE)GUI_Status, (PBYTE)GUIStatus_Hook, 0x6);

	PlayerLeaveChannel = dwBaseAddr + PlayerLeaveChannel_Offset;
	PlayerLeaveChannel_Back = PlayerLeaveChannel + 0x6;
	Mem.CreateDetour((PBYTE)PlayerLeaveChannel, (PBYTE)PlayerLeaveChannel_Hook, 0x6);

	Shop_Buy_Button = dwBaseAddr + Shop_Buy_Button_Offset;
	Shop_Buy_Button_Back = Shop_Buy_Button + 0x7;
	Mem.CreateDetour((PBYTE)Shop_Buy_Button, (PBYTE)ShopButton_Hook, 0x7);

	Read_Shop_ItemId = dwBaseAddr + Read_Shop_ItemId_Offset;
	Read_Shop_ItemId_Back = Read_Shop_ItemId + 0x7;
	Mem.CreateDetour((PBYTE)Read_Shop_ItemId, (PBYTE)ShopItemId_Hook, 0x7);

	Read_Shop_ItemColorId = dwBaseAddr + Read_Shop_ItemColorId_Offset;
	Read_Shop_ItemColorId_Back = Read_Shop_ItemColorId + 0x11;
	Mem.CreateDetour((PBYTE)Read_Shop_ItemColorId, (PBYTE)ShopItemColorId_Hook, 0x11);

	PlayerFov = dwBaseAddr + PlayerFov_Offset;
	PlayerFov_Back = PlayerFov + 0x8;
	Mem.CreateDetour((PBYTE)PlayerFov, (PBYTE)PlayerFOV_Hook, 0x8);

	Mem.WriteASM(MakeRoomBtn_Offset, (DWORD)"\x77", 1);

	MakeRoom = dwBaseAddr + MakeRoom_Offset;
	MakeRoom_Back = MakeRoom + 0x6;

	Mem.CreateDetour((void*)MakeRoom, (void*)MakeRoomButton, 0x6);

	Mem.WriteASM(InRoomMakeRoomBtn_Offset, (DWORD)"\x90\x90\x90\x90\x90", 5);

	InRoomMakeRoom = dwBaseAddr + InRoomMakeRoom_Offset;
	InRoomMakeRoom_Back = InRoomMakeRoom + 0x6;

	Mem.CreateDetour((void*)InRoomMakeRoom, (void*)InRoomMakeRoomButton, 0x6);

	//

	Player_PlayerInfoList_Ack = dwBaseAddr + Player_PlayerInfoList_Ack_Offset;
	Player_PlayerInfoList_Ack_Back = Player_PlayerInfoList_Ack + 0x6;

	Mem.CreateDetour((void*)Player_PlayerInfoList_Ack, (void*)Player_PlayerInfoList_Ack_Hook, 0x6);

	Room_GamePlayCountDown_Ack = dwBaseAddr + Room_GamePlayCountDown_Ack_Offset;
	Room_GamePlayCountDown_Ack_Back = Room_GamePlayCountDown_Ack + 0x6;

	Mem.CreateDetour((void*)Room_GamePlayCountDown_Ack, (void*)Room_GamePlayCountDown_Ack_Hook, 0x6);

	EnterPlayer_Ack = dwBaseAddr + EnterPlayer_Ack_Offset;
	EnterPlayer_Ack_Back = EnterPlayer_Ack + 0x6;

	Mem.CreateDetour((void*)EnterPlayer_Ack, (void*)EnterPlayer_Ack_Hook, 0x6);

	Game_BriefingInfo_Ack = dwBaseAddr + Game_BriefingInfo_Ack_Offset;
	Game_BriefingInfo_Ack_Back = Game_BriefingInfo_Ack + 0x6;

	Mem.CreateDetour((void*)Game_BriefingInfo_Ack, (void*)Game_BriefingInfo_Ack_Hook, 0x6);

	Game_ChangeState_Ack = dwBaseAddr + Game_ChangeState_Ack_Offset;
	Game_ChangeState_Ack_Back = Game_ChangeState_Ack + 0x6;

	Mem.CreateDetour((void*)Game_ChangeState_Ack, (void*)Game_ChangeState_Ack_Hook, 0x6);

	InRoomMakeRoom = dwBaseAddr + Room_GameLoading_Ack_Offset;
	Room_GameLoading_Ack_Back = Room_GameLoading_Ack + 0x6;

	Mem.CreateDetour((void*)Room_GameLoading_Ack, (void*)Room_GameLoading_Ack_Hook, 0x6);

	Game_ChangeSubState_Ack = dwBaseAddr + Game_ChangeSubState_Ack_Offset;
	Game_ChangeSubState_Ack_Back = Game_ChangeSubState_Ack + 0x6;

	Mem.CreateDetour((void*)Game_ChangeSubState_Ack, (void*)Game_ChangeSubState_Ack_Hook, 0x6);

	Game_RefreshGameRuleInfo_Ack = dwBaseAddr + Game_RefreshGameRuleInfo_Ack_Offset;
	Game_RefreshGameRuleInfo_Ack_Back = Game_RefreshGameRuleInfo_Ack + 0x6;

	Mem.CreateDetour((void*)Game_RefreshGameRuleInfo_Ack, (void*)Game_RefreshGameRuleInfo_Ack_Hook, 0x6);

	Room_ListInfo_Ack = dwBaseAddr + Room_ListInfo_Ack_Offset;
	Room_ListInfo_Ack_Back = Room_ListInfo_Ack + 0x6;

	Mem.CreateDetour((void*)Room_ListInfo_Ack, (void*)Room_ListInfo_Ack_Hook, 0x6);

	Chat_Ack = dwBaseAddr + Chat_Ack_Offset;
	Chat_Ack_Back = Chat_Ack + 0x5;

	Mem.CreateDetour((void*)Chat_Ack, (void*)Chat_Ack_Hook, 0x5);

	if (RPCActvion)
		discord->Init();

	XINIT = true;
}

void ChatButtonTexture(Chat_Type chat_type)
{
	if (chat_type == Chat_Type::All)
	{
		if (plr_loc < Player_Loc::Room)
		{
			Chat_type = Chat_Type::All;
			ChatMenuName = "All";
			ChatMainButtonTexture = ChatAllButtonTexture;
		}
		else
		{
			RoomChat_type = Chat_Type::All;
			RoomChatMenuName = "All";
			RoomChatMainButtonTexture = ChatAllButtonTexture;
		}
	}

	if (chat_type == Chat_Type::Team)
	{
		if (plr_loc >= Player_Loc::Room)
		{
			RoomChat_type = Chat_Type::Team;
			RoomChatMenuName = "Team";
			RoomChatMainButtonTexture = ChatTeamButtonTexture;

			//	RoomChatBtnMenuName = "All";
			//	RoomChatMenuButtonTexture = ChatAllButtonTexture;
		}
	}

	if (chat_type == Chat_Type::Clan)
	{
		if (plr_loc < Player_Loc::Room)
		{
			Chat_type = Chat_Type::Clan;
			ChatMenuName = "Clan";
			ChatMainButtonTexture = ChatClanButtonTexture;

			ChatBtnMenuName = "All";
			ChatMenuButtonTexture = ChatAllButtonTexture;
		}
		else
		{
			RoomChat_type = Chat_Type::Clan;
			RoomChatMenuName = "Clan";
			RoomChatMainButtonTexture = ChatClanButtonTexture;

			//	RoomChatBtn2MenuName = "All";
				//RoomChatMenuButton2Texture = ChatAllButtonTexture;
		}

	}

	if (chat_type == Chat_Type::Union)
	{
		if (plr_loc < Player_Loc::Room)
		{
			Chat_type = Chat_Type::Union;
			ChatMenuName = "Union";
			ChatMainButtonTexture = ChatUnionButtonTexture;

			ChatBtn2MenuName = "All";
			ChatMenuButton2Texture = ChatAllButtonTexture;
		}
		else
		{
			RoomChat_type = Chat_Type::Union;
			RoomChatMenuName = "Union";
			RoomChatMainButtonTexture = ChatUnionButtonTexture;

			//RoomChatBtn3MenuName = "All";
			//RoomChatMenuButton3Texture = ChatAllButtonTexture;
		}
	}

	if (chat_type == Chat_Type::Whisper)
	{
		if (plr_loc < Player_Loc::Room)
		{
			Chat_type = Chat_Type::Whisper;
			ChatMenuName = "Whisper";
			ChatMainButtonTexture = ChatWhisperButtonTexture;

			ChatBtn3MenuName = "All";
			ChatMenuButton3Texture = ChatAllButtonTexture;
		}
		else
		{
			RoomChat_type = Chat_Type::Whisper;
			RoomChatMenuName = "Whisper";
			RoomChatMainButtonTexture = ChatWhisperButtonTexture;

			//	RoomChatBtn4MenuName = "All";
			//	RoomChatMenuButton4Texture = ChatAllButtonTexture;
		}
	}

	if (plr_loc < Player_Loc::Room)
	{
		if (chat_type == Chat_Type::Support)
		{
			Chat_type = Chat_Type::Support;
			ChatMenuName = "Support";
			ChatMainButtonTexture = ChatSupportButtonTexture;

			ChatBtn4MenuName = "All";
			ChatMenuButton4Texture = ChatAllButtonTexture;
		}

		if (ChatMenuName != "Clan")
		{
			ChatBtnMenuName = "Clan";
			ChatMenuButtonTexture = ChatClanButtonTexture;
		}

		if (ChatMenuName != "Union")
		{
			ChatBtn2MenuName = "Union";
			ChatMenuButton2Texture = ChatUnionButtonTexture;
		}

		if (ChatMenuName != "Whisper")
		{
			ChatBtn3MenuName = "Whisper";
			ChatMenuButton3Texture = ChatWhisperButtonTexture;
		}

		if (ChatMenuName != "Support")
		{
			ChatBtn4MenuName = "Support";
			ChatMenuButton4Texture = ChatSupportButtonTexture;
		}
	}
	/*else if(plr_loc >= Player_Loc::Room)
	{
		if (ChatMenuName != "Team")
		{
			RoomChatBtnMenuName = "Team";
			RoomChatMenuButtonTexture = ChatTeamButtonTexture;
		}

		if (ChatMenuName != "Clan")
		{
			RoomChatBtn2MenuName = "Clan";
			RoomChatMenuButton2Texture = ChatClanButtonTexture;
		}

		if (RoomChatMenuName != "Union")
		{
			RoomChatBtn3MenuName = "Union";
			RoomChatMenuButton3Texture = ChatUnionButtonTexture;
		}

		if (RoomChatMenuName != "Whisper")
		{
			RoomChatBtn4MenuName = "Whisper";
			RoomChatMenuButton4Texture = ChatWhisperButtonTexture;
		}
	}*/
}

#pragma region Custom Active
bool DoubleKeys(int key, int dkey)
{
	static bool is_xdown = false;
	static bool is_xclicked = false;

	static bool is_xdown2 = false;
	static bool is_xclicked2 = false;

	int size = 0;
	if (GetAsyncKeyState(key) & 0x8000)
		size += 1;

	if (GetAsyncKeyState(dkey) & 1)
		size += 1;

	if (size == 2)
	{
		size = 0;
		return true;
	}
	else
	{
		size = 0;
		return false;
	}
}
#pragma region

#pragma region Render
void Render()
{
	try
	{
		/*
		*/

		if (CRoomOptionUI)
		{
			Legion::BeginEx(BackGroundTexture, NULL, NULL, "Main Menu", ImVec2(950, 475), ImVec2(0.5f, 0.5f), &CMainMenuUI, LegionWindowFlags_Custom);
				MainStatus = "Room Option";
				MainMenuButtonName = "SAVE";
				if (plr_loc == Player_Loc::Room)
				{
					Legion::IndentEx(10, 5);
					Legion::CheckboxEx("show team hp", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} SHOW TEAM HP", &ShowTeamHP, CRoomOptionClient);
					Legion::CheckboxEx("NO RESPAWN", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO RESPAWN", &Respawn, CRoomOptionClient);
					Legion::CheckboxEx("MACRO", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO MACRO", &NoMacro, CRoomOptionClient);
					Legion::CheckboxEx("CLASS SYSTE", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO CLASS SYSTEM", &NoClassSystem, CRoomOptionClient);
					Legion::CheckboxEx("ENCHANTS", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO ENCHANTS", &NoEnchant, CRoomOptionClient);
					Legion::CheckboxEx("INSIDE STATES", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} INSIDE STATES", &InsideStates, CRoomOptionClient);
					Legion::CheckboxEx("REBIRTH", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO REBIRTH", &NoRebirth, CRoomOptionClient);
					Legion::CheckboxEx("AUTO SIT", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} AUTO SIT", &AutoSit, CRoomOptionClient);
					Legion::CheckboxEx("SWITCH PROFILE", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ALLOW SWITCH PROFILE", &ChangeProfile, CRoomOptionClient);
					Legion::CheckboxEx("ACTIVE FILTER", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ACTIVE FILTER", &ActiveFilter, CRoomOptionClient);
					Legion::CheckboxEx("RANDOM SWAP FILTER", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} RANDOM SWAP FILTER", &RandomSwapFilter, CRoomOptionClient);
					if (Legion::ImageButtonEx("FILTER", "FILTER", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(145, 25), CalibriFont, 12.4f))
					{
						RoomMapFilter = true;
					}
					Legion::IndentEx(250, -275);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.HP", &HP, 10, 162, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
					Legion::IndentEx(0, 5);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.SP", &SP, 10, 162, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
					Legion::IndentEx(0, 5);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.HEAL", &Heal, 0, 100, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
					Legion::IndentEx(0, 5);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.PING", &Ping, 20, 1000, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
					Legion::IndentEx(-10, 10);
					Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} BLOCKED.");
					Legion::BeginChildV(RBoxTexture, NULL, NULL, "Blocked", ImVec2(153, 178));
					{
						Legion::IndentEx(3, 7);
						Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Blocked G", ImVec2(148, 161));
						if (!CRoomOptionClient)
						{
							if (!AllowPS && !AllowPerTeamPS)
								Legion::CheckboxEx("CHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &BlockPS);
							if (!AllowCS && !AllowPerTeamCS)
								Legion::CheckboxEx("CHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &BlockCS);
							if (!AllowDG && !AllowPerTeamDG)
								Legion::CheckboxEx("CHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &BlockDG);
							if (!AllowBAT && !AllowPerTeamBAT)
								Legion::CheckboxEx("CHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &BlockBAT);
							if (!AllowBR && !AllowPerTeamBR)
								Legion::CheckboxEx("CHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &BlockBR);
							if (!AllowTB && !AllowPerTeamTB)
								Legion::CheckboxEx("CHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &BlockTB);
							if (!AllowIB && !AllowPerTeamIB)
								Legion::CheckboxEx("CHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &BlockIB);
							if (!AllowFist && !AllowPerTeamFist)
								Legion::CheckboxEx("CHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &BlockFist);
							if (!AllowKatan && !AllowPerTeamKatan)
								Legion::CheckboxEx("CHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &BlockKatan);
							if (!AllowEXO && !AllowPerTeamEXO)
								Legion::CheckboxEx("CHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &BlockEXO);
							if (!AllowSigmaBlade && !AllowPerTeamSigmaBlade)
								Legion::CheckboxEx("CHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &BlockSigmaBlade);
							if (!AllowVitalShock && !AllowPerTeamVitalShock)
								Legion::CheckboxEx("CHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &BlockVitalShock);//Melee End
							if (!AllowRevolver && !AllowPerTeamRevolver)
								Legion::CheckboxEx("CHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &BlockRevolver);
							if (!AllowHeavyMachineGun && !AllowPerTeamHeavyMachineGun)
								Legion::CheckboxEx("CHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &BlockHeavyMachineGun);
							if (!AllowLightMachineGun && !AllowPerTeamLightMachineGun)
								Legion::CheckboxEx("CHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &BlockLightMachineGun);
							if (!AllowTurret && !AllowPerTeamTurret)
								Legion::CheckboxEx("CHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &BlockTurret);
							if (!AllowHealWeapons && !AllowPerTeamHealWeapons)
								Legion::CheckboxEx("CHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &BlockHealWeapons);
							if (!AllowShockWeapons && !AllowPerTeamShockWeapons)
								Legion::CheckboxEx("CHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &BlockShockWeapons);
							if (!AllowSentry && !AllowPerTeamSentry)
								Legion::CheckboxEx("CHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &BlockSentry);
							if (!AllowSenty && !AllowPerTeamSenty)
								Legion::CheckboxEx("CHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &BlockSenty);
							if (!AllowMineGun && !AllowPerTeamMineGun)
								Legion::CheckboxEx("CHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &BlockMineGun);
							if (!AllowEarthbomber && !AllowPerTeamEarthbomber)
								Legion::CheckboxEx("CHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &BlockEarthbomber);
							if (!AllowLightningBomber && !AllowPerTeamLightningBomber)
								Legion::CheckboxEx("CHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &BlockLightningBomber);
							if (!AllowRocketLauncher && !AllowPerTeamRocketLauncher)
								Legion::CheckboxEx("CHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &BlockRocketLauncher);
							if (!AllowCannonade && !AllowPerTeamRailGun)
								Legion::CheckboxEx("CHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &BlockCannonade);
							if (!AllowRailGun && !AllowPerTeamRailGun)
								Legion::CheckboxEx("CHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &BlockRailGun);
							if (!AllowSniper && !AllowPerTeamSniper)
								Legion::CheckboxEx("CHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &BlockSniper);
							if (!AllowAirGun && !AllowPerTeamAirGun)
								Legion::CheckboxEx("CHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &BlockAirGun);
							if (!AllowDualMagnum && !AllowPerTeamDualMagnum)
								Legion::CheckboxEx("CHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &BlockDualMagnum);
							if (!AllowBurstShotgun && !AllowPerTeamBurstShotgun)
								Legion::CheckboxEx("CHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &BlockBurstShotgun);
							if (!AllowSemiRifle && !AllowPerTeamSemiRifle)
								Legion::CheckboxEx("CHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &BlockSemiRifle);
							if (!AllowSmashRifle && !AllowPerTeamSmashRifle)
								Legion::CheckboxEx("CHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &BlockSmashRifle);
							if (!AllowAssaultRifle && !AllowPerTeamAssaultRifle)
								Legion::CheckboxEx("CHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &BlockAssaultRifle);
							if (!AllowBubblRifle && !AllowPerTeamBubblRifle)
								Legion::CheckboxEx("CHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &BlockBubblRifle);
							if (!AllowGaussRifle && !AllowPerTeamGaussRifle)
								Legion::CheckboxEx("CHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &BlockGaussRifle);
							if (!AllowHandGun && !AllowPerTeamHandGun)
								Legion::CheckboxEx("CHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &BlockHandGun);
							if (!AllowSubmachineGun && !AllowPerTeamSubmachineGun)
								Legion::CheckboxEx("CHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &BlockSubmachineGun);

							//Skill
							if (!AllowHalfHPMastery && !AllowPerTeamHPMastery)
								Legion::CheckboxEx("CHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &BlockHalfHPMastery);
							if (!AllowHPMastery && !AllowPerTeamHPMastery)
								Legion::CheckboxEx("CHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &BlockHPMastery);
							if (!AllowSkillMastery && !AllowPerTeamSkillMastery)
								Legion::CheckboxEx("CHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &BlockSkillMastery);
							if (!AllowDualMastery && !AllowPerTeamDualMastery)
								Legion::CheckboxEx("CHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &BlockDualMastery);
							if (!AllowMetallic && !AllowPerTeamMetallic)
								Legion::CheckboxEx("CHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &BlockMetallic);
							if (!AllowUniqueDualMastery && !AllowPerTeamDualMastery)
								Legion::CheckboxEx("CHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &BlockUniqueDualMastery);
							if (!AllowAnchoring && !AllowPerTeamAnchoring)
								Legion::CheckboxEx("CHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &BlockAnchoring);
							if (!AllowFlying && !AllowPerTeamFlying)
								Legion::CheckboxEx("CHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &BlockFlying);
							if (!AllowInvisible && !AllowPerTeamInvisible)
								Legion::CheckboxEx("CHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &BlockInvisible);
							if (!AllowDetect && !AllowPerTeamDetect)
								Legion::CheckboxEx("CHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &BlockDetect);
							if (!AllowShield && !AllowPerTeamShield)
								Legion::CheckboxEx("CHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &BlockShield);
							if (!AllowBlock && !AllowPerTeamBlock)
								Legion::CheckboxEx("CHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Block", &BlockBlock);
						}
						else
						{
							Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + Blockroption).c_str());
						}
						Legion::EndChild();
					}Legion::EndChild();
					Legion::IndentEx(153, -198);
					Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ALLOWED.");
					Legion::BeginChildV(RBoxTexture, NULL, NULL, "Allowed", ImVec2(153, 178));
					{
						Legion::IndentEx(3, 7);
						Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Allowed G", ImVec2(148, 160));
						if (!CRoomOptionClient)
						{
							if (!BlockPS)
								Legion::CheckboxEx("ACHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &AllowPS);
							if (!BlockCS)
								Legion::CheckboxEx("ACHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &AllowCS);
							if (!BlockDG)
								Legion::CheckboxEx("ACHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &AllowDG);
							if (!BlockBAT)
								Legion::CheckboxEx("ACHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &AllowBAT);
							if (!BlockBR)
								Legion::CheckboxEx("ACHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &AllowBR);
							if (!BlockTB)
								Legion::CheckboxEx("ACHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &AllowTB);
							if (!BlockIB)
								Legion::CheckboxEx("ACHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &AllowIB);
							if (!BlockFist)
								Legion::CheckboxEx("ACHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &AllowFist);
							if (!BlockKatan)
								Legion::CheckboxEx("ACHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &AllowKatan);
							if (!BlockEXO)
								Legion::CheckboxEx("ACHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &AllowEXO);
							if (!BlockSigmaBlade)
								Legion::CheckboxEx("ACHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &AllowSigmaBlade);
							if (!BlockVitalShock)
								Legion::CheckboxEx("ACHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &AllowVitalShock);//Melee End
							if (!BlockRevolver)
								Legion::CheckboxEx("ACHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &AllowRevolver);
							if (!BlockHeavyMachineGun)
								Legion::CheckboxEx("ACHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &AllowHeavyMachineGun);
							if (!BlockLightMachineGun)
								Legion::CheckboxEx("ACHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &AllowLightMachineGun);
							if (!BlockTurret)
								Legion::CheckboxEx("ACHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &AllowTurret);
							if (!BlockHealWeapons)
								Legion::CheckboxEx("ACHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &AllowHealWeapons);
							if (!BlockShockWeapons)
								Legion::CheckboxEx("ACHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &AllowShockWeapons);
							if (!BlockSentry)
								Legion::CheckboxEx("ACHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &AllowSentry);
							if (!BlockSenty)
								Legion::CheckboxEx("ACHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &AllowSenty);
							if (!BlockMineGun)
								Legion::CheckboxEx("ACHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &AllowMineGun);
							if (!BlockEarthbomber)
								Legion::CheckboxEx("ACHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &AllowEarthbomber);
							if (!BlockLightningBomber)
								Legion::CheckboxEx("ACHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &AllowLightningBomber);
							if (!BlockRocketLauncher)
								Legion::CheckboxEx("ACHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &AllowRocketLauncher);
							if (!BlockCannonade)
								Legion::CheckboxEx("ACHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &AllowCannonade);
							if (!BlockRailGun)
								Legion::CheckboxEx("ACHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &AllowRailGun);
							if (!BlockSniper)
								Legion::CheckboxEx("ACHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &AllowSniper);
							if (!BlockAirGun)
								Legion::CheckboxEx("ACHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &AllowAirGun);
							if (!BlockDualMagnum)
								Legion::CheckboxEx("ACHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &AllowDualMagnum);
							if (!BlockBurstShotgun)
								Legion::CheckboxEx("ACHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &AllowBurstShotgun);
							if (!BlockSemiRifle)
								Legion::CheckboxEx("ACHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &AllowSemiRifle);
							if (!BlockSmashRifle)
								Legion::CheckboxEx("ACHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &AllowSmashRifle);
							if (!BlockAssaultRifle)
								Legion::CheckboxEx("ACHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &AllowAssaultRifle);
							if (!BlockBubblRifle)
								Legion::CheckboxEx("ACHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &AllowBubblRifle);
							if (!BlockGaussRifle)
								Legion::CheckboxEx("ACHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &AllowGaussRifle);
							if (!BlockHandGun)
								Legion::CheckboxEx("ACHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &AllowHandGun);
							if (!BlockSubmachineGun)
								Legion::CheckboxEx("ACHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &AllowSubmachineGun);

							//Skill
							if (!BlockHalfHPMastery)
								Legion::CheckboxEx("ACHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &AllowHalfHPMastery);
							if (!BlockHPMastery)
								Legion::CheckboxEx("ACHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &AllowHPMastery);
							if (!BlockSkillMastery)
								Legion::CheckboxEx("ACHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &AllowSkillMastery);
							if (!BlockDualMastery)
								Legion::CheckboxEx("ACHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &AllowDualMastery);
							if (!BlockMetallic)
								Legion::CheckboxEx("ACHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &AllowMetallic);
							if (!BlockUniqueDualMastery)
								Legion::CheckboxEx("ACHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &AllowUniqueDualMastery);
							if (!BlockAnchoring)
								Legion::CheckboxEx("ACHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &AllowAnchoring);
							if (!BlockFlying)
								Legion::CheckboxEx("ACHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &AllowFlying);
							if (!BlockInvisible)
								Legion::CheckboxEx("ACHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &AllowInvisible);
							if (!BlockDetect)
								Legion::CheckboxEx("ACHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &AllowDetect);
							if (!BlockShield)
								Legion::CheckboxEx("ACHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &AllowShield);
							if (!BlockBlock)
								Legion::CheckboxEx("ACHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Allow", &AllowBlock);
						}
						else
						{
							Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + Allowroption).c_str());
						}
						Legion::EndChild();
					}Legion::EndChild();
					Legion::IndentEx(153, -197);
					Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ALLOWED PER TEAM.");
					Legion::BeginChildV(RBoxTexture, NULL, NULL, "Allowed Per Team", ImVec2(153, 178));
					{
						Legion::IndentEx(3, 7);
						Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Allowed Per Team G", ImVec2(148, 160));
						if (!CRoomOptionClient)
						{
							if (!BlockPS)
								Legion::CheckboxEx("APTCHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &AllowPerTeamPS);
							if (!BlockCS)
								Legion::CheckboxEx("APTCHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &AllowPerTeamCS);
							if (!BlockDG)
								Legion::CheckboxEx("APTCHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &AllowPerTeamDG);
							if (!BlockBAT)
								Legion::CheckboxEx("APTCHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &AllowPerTeamBAT);
							if (!BlockBR)
								Legion::CheckboxEx("APTCHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &AllowPerTeamBR);
							if (!BlockTB)
								Legion::CheckboxEx("APTCHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &AllowPerTeamTB);
							if (!BlockIB)
								Legion::CheckboxEx("APTCHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &AllowPerTeamIB);
							if (!BlockFist)
								Legion::CheckboxEx("APTCHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &AllowPerTeamFist);
							if (!BlockKatan)
								Legion::CheckboxEx("APTCHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &AllowPerTeamKatan);
							if (!BlockEXO)
								Legion::CheckboxEx("APTCHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &AllowPerTeamEXO);
							if (!BlockSigmaBlade)
								Legion::CheckboxEx("APTCHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &AllowPerTeamSigmaBlade);
							if (!BlockVitalShock)
								Legion::CheckboxEx("APTCHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &AllowPerTeamVitalShock);//Melee End
							if (!BlockRevolver)
								Legion::CheckboxEx("APTCHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &AllowPerTeamRevolver);
							if (!BlockHeavyMachineGun)
								Legion::CheckboxEx("APTCHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &AllowPerTeamHeavyMachineGun);
							if (!BlockLightMachineGun)
								Legion::CheckboxEx("APTCHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &AllowPerTeamLightMachineGun);
							if (!BlockTurret)
								Legion::CheckboxEx("APTCHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &AllowPerTeamTurret);
							if (!BlockHealWeapons)
								Legion::CheckboxEx("APTCHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &AllowPerTeamHealWeapons);
							if (!BlockShockWeapons)
								Legion::CheckboxEx("APTCHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &AllowPerTeamShockWeapons);
							if (!BlockSentry)
								Legion::CheckboxEx("APTCHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &AllowPerTeamSentry);
							if (!BlockSenty)
								Legion::CheckboxEx("APTCHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &AllowPerTeamSenty);
							if (!BlockMineGun)
								Legion::CheckboxEx("APTCHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &AllowPerTeamMineGun);
							if (!BlockEarthbomber)
								Legion::CheckboxEx("APTCHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &AllowPerTeamEarthbomber);
							if (!BlockLightningBomber)
								Legion::CheckboxEx("APTCHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &AllowPerTeamLightningBomber);
							if (!BlockRocketLauncher)
								Legion::CheckboxEx("APTCHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &AllowPerTeamRocketLauncher);
							if (!BlockCannonade)
								Legion::CheckboxEx("APTCHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &AllowPerTeamCannonade);
							if (!BlockRailGun)
								Legion::CheckboxEx("APTCHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &AllowPerTeamRailGun);
							if (!BlockSniper)
								Legion::CheckboxEx("APTCHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &AllowPerTeamSniper);
							if (!BlockAirGun)
								Legion::CheckboxEx("APTCHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &AllowPerTeamAirGun);
							if (!BlockDualMagnum)
								Legion::CheckboxEx("APTCHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &AllowPerTeamDualMagnum);
							if (!BlockBurstShotgun)
								Legion::CheckboxEx("APTCHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &AllowPerTeamBurstShotgun);
							if (!BlockSemiRifle)
								Legion::CheckboxEx("APTCHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &AllowPerTeamSemiRifle);
							if (!BlockSmashRifle)
								Legion::CheckboxEx("APTCHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &AllowPerTeamSmashRifle);
							if (!BlockAssaultRifle)
								Legion::CheckboxEx("APTCHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &AllowPerTeamAssaultRifle);
							if (!BlockBubblRifle)
								Legion::CheckboxEx("APTCHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &AllowPerTeamBubblRifle);
							if (!BlockGaussRifle)
								Legion::CheckboxEx("APTCHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &AllowPerTeamGaussRifle);
							if (!BlockHandGun)
								Legion::CheckboxEx("APTCHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &AllowPerTeamHandGun);
							if (!BlockSubmachineGun)
								Legion::CheckboxEx("APTCHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &AllowPerTeamSubmachineGun);

							//Skill
							if (!BlockHalfHPMastery)
								Legion::CheckboxEx("APTCHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &AllowPerTeamHalfHPMastery);
							if (!BlockHPMastery)
								Legion::CheckboxEx("APTCHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &AllowPerTeamHPMastery);
							if (!BlockSkillMastery)
								Legion::CheckboxEx("APTCHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &AllowPerTeamSkillMastery);
							if (!BlockDualMastery)
								Legion::CheckboxEx("APTCHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &AllowPerTeamDualMastery);
							if (!BlockMetallic)
								Legion::CheckboxEx("APTCHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &AllowPerTeamMetallic);
							if (!BlockUniqueDualMastery)
								Legion::CheckboxEx("APTCHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &AllowPerTeamUniqueDualMastery);
							if (!BlockAnchoring)
								Legion::CheckboxEx("APTCHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &AllowPerTeamAnchoring);
							if (!BlockFlying)
								Legion::CheckboxEx("APTCHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &AllowPerTeamFlying);
							if (!BlockInvisible)
								Legion::CheckboxEx("APTCHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &AllowPerTeamInvisible);
							if (!BlockDetect)
								Legion::CheckboxEx("APTCHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &AllowPerTeamDetect);
							if (!BlockShield)
								Legion::CheckboxEx("APTCHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &AllowPerTeamShield);
							if (!BlockBlock)
								Legion::CheckboxEx("APTCHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Block", &AllowPerTeamBlock);
						}
						else
						{
							Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + AllowPerTeamroption).c_str());
						}
						Legion::EndChild();
					}Legion::EndChild();

					// here 
					if (!CRoomOptionClient && plr_loc == Player_Loc::Room)
					{
				
						Legion::IndentEx(800, -45);// and here
						if (Legion::ImageButtonEx("MainMenu BTN", MainMenuButtonName, true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(120, 25), CalibriFont, 12.4f))
						{
							CRoomOptionUI = false;
							if (!CRoomOptionClient)
							{
								option = "";

								if (ShowTeamHP)
								{
									option += "ShowTeamHP Yes-";
									Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x01", 4);
								}
								else
								{
									option += "ShowTeamHP No-";
									Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x00", 4);
								}

								if (Respawn)
								{
									option += "Respawn Yes-";
									Mem.WriteASM(InfRespawn, (DWORD)"\x00", 1);
								}
								else
								{
									option += "Respawn No-";
									Mem.WriteASM(InfRespawn, (DWORD)"\x04", 1);
								}

								if (NoClassSystem)
									option += "NoClassSystem Yes-";
								else
									option += "NoClassSystem No-";

								if (NoEnchant)
									option += "NoEnchant Yes-";
								else
									option += "NoEnchant No-";

								if (InsideStates)
									option += "InsideStates Yes-";
								else
									option += "InsideStates No-";

								if (NoRebirth)
									option += "No Rebirth Yes-";
								else
									option += "No Rebirth No-";

								if (AutoSit)
								{
									option += "AutoSit Yes-";
									//			Mem.WriteASM(CharBlock_Offset, (DWORD)"\x71", 1);
								}
								else
								{
									option += "AutoSit No-";
									//	Mem.WriteASM(CharBlock_Offset, (DWORD)"\x74", 1);
								}

								if (ChangeProfile)
									option += "ChangeProfile Yes-";
								else
									option += "ChangeProfile No-";

								if (ActiveFilter)
									option += "Active Filter Yes-";
								else
									option += "Active Filter No-";

								if (RandomSwapFilter)
									option += "Random Swap Filter Yes-";
								else
									option += "Random Swap Filter No-";

								std::string hp = std::to_string(HP);
								if (HP > 100 || HP < 100)
									option += "HP [" + hp + "]-";
								else
									option += "HP [100]-";

								std::string sp = std::to_string(SP);
								if (SP > 100 || SP < 100)
									option += "SP [" + sp + "]-";
								else
									option += "SP [100]-";

								std::string heal = std::to_string(Heal);
								if (Heal > 0.0f)
									option += "Heal [" + heal + "]-";
								else
									option += "Heal [0.0]-";

								std::string ping = std::to_string(Ping);
								if (Ping < 1000)
									option += "Ping [" + ping + "]-";
								else
									option += "Ping [1000]-";

								//Custom rule
								if (BlockPS)
									option += "BlockPS Yes-";
								else
									option += "BlockPS No-";

								if (BlockCS)
									option += "BlockCS Yes-";
								else
									option += "BlockCS No-";

								if (BlockDG)
									option += "BlockDG Yes-";
								else
									option += "BlockDG No-";

								if (BlockBAT)
									option += "BlockBAT Yes-";
								else
									option += "BlockBAT No-";

								if (BlockBR)
									option += "BlockBR Yes-";
								else
									option += "BlockBR No-";

								if (BlockTB)
									option += "BlockTB Yes-";
								else
									option += "BlockTB No-";

								if (BlockIB)
									option += "BlockIB Yes-";
								else
									option += "BlockIB No-";

								if (BlockFist)
									option += "BlockFist Yes-";
								else
									option += "BlockFist No-";

								if (BlockKatan)
									option += "BlockKatan Yes-";
								else
									option += "BlockKatan No-";

								if (BlockEXO)
									option += "BlockEXO Yes-";
								else
									option += "BlockEXO No-";

								if (BlockSigmaBlade)
									option += "BlockSigmaBlade Yes-";
								else
									option += "BlockSigmaBlade No-";

								if (BlockVitalShock)
									option += "BlockVitalShock Yes-";
								else
									option += "BlockVitalShock No-";

								if (BlockRevolver)
									option += "BlockRevolver Yes-";
								else
									option += "BlockRevolver No-";

								if (BlockHeavyMachineGun)
									option += "BlockHeavyMachineGun Yes-";
								else
									option += "BlockHeavyMachineGun No-";

								if (BlockLightMachineGun)
									option += "BlockLightMachineGun Yes-";
								else
									option += "BlockLightMachineGun No-";

								if (BlockTurret)
									option += "BlockTurret Yes-";
								else
									option += "BlockTurret No-";

								if (BlockHealWeapons)
									option += "BlockHealWeapons Yes-";
								else
									option += "BlockHealWeapons No-";

								if (BlockShockWeapons)
									option += "BlockShockWeapons Yes-";
								else
									option += "BlockShockWeapons No-";

								if (BlockSentry)
									option += "BlockSentry Yes-";
								else
									option += "BlockSentry No-";

								if (BlockSenty)
									option += "BlockSenty Yes-";
								else
									option += "BlockSenty No-";

								if (BlockMineGun)
									option += "BlockMineGun Yes-";
								else
									option += "BlockMineGun No-";

								if (BlockEarthbomber)
									option += "BlockEarthbomber Yes-";
								else
									option += "BlockEarthbomber No-";

								if (BlockLightningBomber)
									option += "BlockLightningBomber Yes-";
								else
									option += "BlockLightningBomber No-";

								if (BlockRocketLauncher)
									option += "BlockRocketLauncher Yes-";
								else
									option += "BlockRocketLauncher No-";

								if (BlockCannonade)
									option += "BlockCannonade Yes-";
								else
									option += "BlockCannonade No-";

								if (BlockRailGun)
									option += "BlockRailGun Yes-";
								else
									option += "BlockRailGun No-";

								if (BlockSniper)
									option += "BlockSniper Yes-";
								else
									option += "BlockSniper No-";

								if (BlockAirGun)
									option += "BlockAirGun Yes-";
								else
									option += "BlockAirGun No-";

								if (BlockDualMagnum)
									option += "BlockDualMagnum Yes-";
								else
									option += "BlockDualMagnum No-";

								if (BlockBurstShotgun)
									option += "BlockBurstShotgun Yes-";
								else
									option += "BlockBurstShotgun No-";

								//Skills Blocked
								if (BlockHalfHPMastery)
									option += "BlockHalfHPMastery Yes-";
								else
									option += "BlockHalfHPMastery No-";

								if (BlockHPMastery)
									option += "BlockHPMastery Yes-";
								else
									option += "BlockHPMastery No-";

								if (BlockSkillMastery)
									option += "BlockSkillMastery Yes-";
								else
									option += "BlockSkillMastery No-";

								if (BlockDualMastery)
									option += "BlockDualMastery Yes-";
								else
									option += "BlockDualMastery No-";

								if (BlockMetallic)
									option += "BlockMetallic Yes-";
								else
									option += "BlockMetallic No-";

								if (BlockUniqueDualMastery)
									option += "BlockUniqueDualMastery Yes-";
								else
									option += "BlockUniqueDualMastery No-";

								if (BlockAnchoring)
									option += "BlockAnchoring Yes-";
								else
									option += "BlockAnchoring No-";

								if (BlockFlying)
									option += "BlockFlying Yes-";
								else
									option += "BlockFlying No-";

								if (BlockInvisible)
									option += "BlockInvisible Yes-";
								else
									option += "BlockInvisible No-";

								if (BlockDetect)
									option += "BlockDetect Yes-";
								else
									option += "BlockDetect No-";

								if (BlockShield)
									option += "BlockShield Yes-";
								else
									option += "BlockShield No-";

								if (BlockBlock)
									option += "BlockBlock Yes-";
								else
									option += "BlockBlock No-";

								if (BlockSemiRifle)
									option += "BlockSemiRifle Yes-";
								else
									option += "BlockSemiRifle No-";

								if (BlockSmashRifle)
									option += "BlockSmashRifle Yes-";
								else
									option += "BlockSmashRifle No-";

								if (BlockAssaultRifle)
									option += "BlockAssaultRifle Yes-";
								else
									option += "BlockAssaultRifle No-";

								if (BlockBubblRifle)
									option += "BlockBubblRifle Yes-";
								else
									option += "BlockBubblRifle No-";

								if (BlockGaussRifle)
									option += "BlockGaussRifle Yes-";
								else
									option += "BlockGaussRifle No-";

								if (BlockHandGun)
									option += "BlockHandGun Yes-";
								else
									option += "BlockHandGun No-";

								if (BlockSubmachineGun)
									option += "BlockSubmachineGun Yes-";
								else
									option += "BlockSubmachineGun No-";

								//Allow
								if (AllowPS)
									option += "AllowPS Yes-";
								else
									option += "AllowPS No-";

								if (AllowCS)
									option += "AllowCS Yes-";
								else
									option += "AllowCS No-";

								if (AllowDG)
									option += "AllowDG Yes-";
								else
									option += "AllowDG No-";

								if (AllowBAT)
									option += "AllowBAT Yes-";
								else
									option += "AllowBAT No-";

								if (AllowBR)
									option += "AllowBR Yes-";
								else
									option += "AllowBR No-";

								if (AllowTB)
									option += "AllowTB Yes-";
								else
									option += "AllowTB No-";

								if (AllowIB)
									option += "AllowIB Yes-";
								else
									option += "AllowIB No-";

								if (AllowFist)
									option += "AllowFist Yes-";
								else
									option += "AllowFist No-";

								if (AllowKatan)
									option += "AllowKatan Yes-";
								else
									option += "AllowKatan No-";

								if (AllowEXO)
									option += "AllowEXO Yes-";
								else
									option += "AllowEXO No-";

								if (AllowSigmaBlade)
									option += "AllowSigmaBlade Yes-";
								else
									option += "AllowSigmaBlade No-";

								if (AllowVitalShock)
									option += "AllowVitalShock Yes-";
								else
									option += "AllowVitalShock No-";

								if (AllowRevolver)
									option += "AllowRevolver Yes-";
								else
									option += "AllowRevolver No-";

								if (AllowHeavyMachineGun)
									option += "AllowHeavyMachineGun Yes-";
								else
									option += "AllowHeavyMachineGun No-";

								if (AllowLightMachineGun)
									option += "AllowLightMachineGun Yes-";
								else
									option += "AllowLightMachineGun No-";

								if (AllowTurret)
									option += "AllowTurret Yes-";
								else
									option += "AllowTurret No-";

								if (AllowHealWeapons)
									option += "AllowHealWeapons Yes-";
								else
									option += "AllowHealWeapons No-";

								if (AllowShockWeapons)
									option += "AllowShockWeapons Yes-";
								else
									option += "AllowShockWeapons No-";

								if (AllowSentry)
									option += "AllowSentry Yes-";
								else
									option += "AllowSentry No-";

								if (AllowSenty)
									option += "AllowSenty Yes-";
								else
									option += "AllowSenty No-";

								if (AllowMineGun)
									option += "AllowMineGun Yes-";
								else
									option += "AllowMineGun No-";

								if (AllowEarthbomber)
									option += "AllowEarthbomber Yes-";
								else
									option += "AllowEarthbomber No-";

								if (AllowLightningBomber)
									option += "AllowLightningBomber Yes-";
								else
									option += "AllowLightningBomber No-";

								if (AllowRocketLauncher)
									option += "AllowRocketLauncher Yes-";
								else
									option += "AllowRocketLauncher No-";

								if (AllowCannonade)
									option += "AllowCannonade Yes-";
								else
									option += "AllowCannonade No-";

								if (AllowRailGun)
									option += "AllowRailGun Yes-";
								else
									option += "AllowRailGun No-";

								if (AllowSniper)
									option += "AllowSniper Yes-";
								else
									option += "AllowSniper No-";

								if (AllowAirGun)
									option += "AllowAirGun Yes-";
								else
									option += "AllowAirGun No-";

								if (AllowDualMagnum)
									option += "AllowDualMagnum Yes-";
								else
									option += "AllowDualMagnum No-";

								if (AllowBurstShotgun)
									option += "AllowBurstShotgun Yes-";
								else
									option += "AllowBurstShotgun No-";

								if (AllowSemiRifle)
									option += "AllowSemiRifle Yes-";
								else
									option += "AllowSemiRifle No-";

								if (AllowSmashRifle)
									option += "AllowSmashRifle Yes-";
								else
									option += "AllowSmashRifle No-";

								if (AllowAssaultRifle)
									option += "AllowAssaultRifle Yes-";
								else
									option += "AllowAssaultRifle No-";

								if (AllowBubblRifle)
									option += "AllowBubblRifle Yes-";
								else
									option += "AllowBubblRifle No-";

								if (AllowGaussRifle)
									option += "AllowGaussRifle Yes-";
								else
									option += "AllowGaussRifle No-";

								if (AllowHandGun)
									option += "AllowHandGun Yes-";
								else
									option += "AllowHandGun No-";

								if (AllowSubmachineGun)
									option += "AllowSubmachineGun Yes-";
								else
									option += "AllowSubmachineGun No-";

								//Skills Allowed
								if (AllowHalfHPMastery)
									option += "AllowHalfHPMastery Yes-";
								else
									option += "AllowHalfHPMastery No-";

								if (AllowHPMastery)
									option += "AllowHPMastery Yes-";
								else
									option += "AllowHPMastery No-";

								if (AllowSkillMastery)
									option += "AllowSkillMastery Yes-";
								else
									option += "AllowSkillMastery No-";

								if (AllowDualMastery)
									option += "AllowDualMastery Yes-";
								else
									option += "AllowDualMastery No-";

								if (AllowMetallic)
									option += "AllowMetallic Yes-";
								else
									option += "AllowMetallic No-";

								if (AllowUniqueDualMastery)
									option += "AllowUniqueDualMastery Yes-";
								else
									option += "AllowUniqueDualMastery No-";

								if (AllowAnchoring)
									option += "AllowAnchoring Yes-";
								else
									option += "AllowAnchoring No-";

								if (AllowFlying)
									option += "AllowFlying Yes-";
								else
									option += "AllowFlying No-";

								if (AllowInvisible)
									option += "AllowInvisible Yes-";
								else
									option += "AllowInvisible No-";

								if (AllowDetect)
									option += "AllowDetect Yes-";
								else
									option += "AllowDetect No-";

								if (AllowShield)
									option += "AllowShield Yes-";
								else
									option += "AllowShield No-";

								if (AllowBlock)
									option += "AllowBlock Yes-";
								else
									option += "AllowBlock No-";

								// Per team
								if (AllowPerTeamPS)
									option += "AllowPerTeamPS Yes-";
								else
									option += "AllowPerTeamPS No-";

								if (AllowPerTeamCS)
									option += "AllowPerTeamCS Yes-";
								else
									option += "AllowPerTeamCS No-";

								if (AllowPerTeamDG)
									option += "AllowPerTeamDG Yes-";
								else
									option += "AllowPerTeamDG No-";

								if (AllowPerTeamBAT)
									option += "AllowPerTeamBAT Yes-";
								else
									option += "AllowPerTeamBAT No-";

								if (AllowPerTeamBR)
									option += "AllowPerTeamBR Yes-";
								else
									option += "AllowPerTeamBR No-";

								if (AllowPerTeamTB)
									option += "AllowPerTeamTB Yes-";
								else
									option += "AllowPerTeamTB No-";

								if (AllowPerTeamIB)
									option += "AllowPerTeamIB Yes-";
								else
									option += "AllowPerTeamIB No-";

								if (AllowPerTeamFist)
									option += "AllowPerTeamFist Yes-";
								else
									option += "AllowPerTeamFist No-";

								if (AllowPerTeamKatan)
									option += "AllowPerTeamKatan Yes-";
								else
									option += "AllowPerTeamKatan No-";

								if (AllowPerTeamEXO)
									option += "AllowPerTeamEXO Yes-";
								else
									option += "AllowPerTeamEXO No-";

								if (AllowPerTeamSigmaBlade)
									option += "AllowPerTeamSigmaBlade Yes-";
								else
									option += "AllowPerTeamSigmaBlade No-";

								if (AllowPerTeamVitalShock)
									option += "AllowPerTeamVitalShock Yes-";
								else
									option += "AllowPerTeamVitalShock No-";

								if (AllowPerTeamRevolver)
									option += "AllowPerTeamRevolver Yes-";
								else
									option += "AllowPerTeamRevolver No-";

								if (AllowPerTeamHeavyMachineGun)
									option += "AllowPerTeamHeavyMachineGun Yes-";
								else
									option += "AllowPerTeamHeavyMachineGun No-";

								if (AllowPerTeamLightMachineGun)
									option += "AllowPerTeamLightMachineGun Yes-";
								else
									option += "AllowPerTeamLightMachineGun No-";

								if (AllowPerTeamTurret)
									option += "AllowPerTeamTurret Yes-";
								else
									option += "AllowPerTeamTurret No-";

								if (AllowPerTeamHealWeapons)
									option += "AllowPerTeamHealWeapons Yes-";
								else
									option += "AllowPerTeamHealWeapons No-";

								if (AllowPerTeamShockWeapons)
									option += "AllowPerTeamShockWeapons Yes-";
								else
									option += "AllowPerTeamShockWeapons No-";

								if (AllowPerTeamSentry)
									option += "AllowPerTeamSentry Yes-";
								else
									option += "AllowPerTeamSentry No-";

								if (AllowPerTeamSenty)
									option += "AllowPerTeamSenty Yes-";
								else
									option += "AllowPerTeamSenty No-";

								if (AllowPerTeamMineGun)
									option += "AllowPerTeamMineGun Yes-";
								else
									option += "AllowPerTeamMineGun No-";

								if (AllowPerTeamEarthbomber)
									option += "AllowPerTeamEarthbomber Yes-";
								else
									option += "AllowPerTeamEarthbomber No-";

								if (AllowPerTeamLightningBomber)
									option += "AllowPerTeamLightningBomber Yes-";
								else
									option += "AllowPerTeamLightningBomber No-";

								if (AllowPerTeamRocketLauncher)
									option += "AllowPerTeamRocketLauncher Yes-";
								else
									option += "AllowPerTeamRocketLauncher No-";

								if (AllowPerTeamCannonade)
									option += "AllowPerTeamCannonade Yes-";
								else
									option += "AllowPerTeamCannonade No-";

								if (AllowPerTeamRailGun)
									option += "AllowPerTeamRailGun Yes-";
								else
									option += "AllowPerTeamRailGun No-";

								if (AllowPerTeamSniper)
									option += "AllowPerTeamSniper Yes-";
								else
									option += "AllowPerTeamSniper No-";

								if (AllowPerTeamAirGun)
									option += "AllowPerTeamAirGun Yes-";
								else
									option += "AllowPerTeamAirGun No-";

								if (AllowPerTeamDualMagnum)
									option += "AllowPerTeamDualMagnum Yes-";
								else
									option += "AllowPerTeamDualMagnum No-";

								if (AllowPerTeamBurstShotgun)
									option += "AllowPerTeamBurstShotgun Yes-";
								else
									option += "AllowPerTeamBurstShotgun No-";

								if (AllowPerTeamSemiRifle)
									option += "AllowPerTeamSemiRifle Yes-";
								else
									option += "AllowPerTeamSemiRifle No-";

								if (AllowPerTeamSmashRifle)
									option += "AllowPerTeamSmashRifle Yes-";
								else
									option += "AllowPerTeamSmashRifle No-";

								if (AllowPerTeamAssaultRifle)
									option += "AllowPerTeamAssaultRifle Yes-";
								else
									option += "AllowPerTeamAssaultRifle No-";

								if (AllowPerTeamBubblRifle)
									option += "AllowPerTeamBubblRifle Yes-";
								else
									option += "AllowPerTeamBubblRifle No-";

								if (AllowPerTeamGaussRifle)
									option += "AllowPerTeamGaussRifle Yes-";
								else
									option += "AllowPerTeamGaussRifle No-";

								if (AllowPerTeamHandGun)
									option += "AllowPerTeamHandGun Yes-";
								else
									option += "AllowPerTeamHandGun No-";

								if (AllowPerTeamSubmachineGun)
									option += "AllowPerTeamSubmachineGun Yes-";
								else
									option += "AllowPerTeamSubmachineGun No-";

								//Skills AllowPerTeamed
								if (AllowPerTeamHalfHPMastery)
									option += "AllowPerTeamHalfHPMastery Yes-";
								else
									option += "AllowPerTeamHalfHPMastery No-";

								if (AllowPerTeamHPMastery)
									option += "AllowPerTeamHPMastery Yes-";
								else
									option += "AllowPerTeamHPMastery No-";

								if (AllowPerTeamSkillMastery)
									option += "AllowPerTeamSkillMastery Yes-";
								else
									option += "AllowPerTeamSkillMastery No-";

								if (AllowPerTeamDualMastery)
									option += "AllowPerTeamDualMastery Yes-";
								else
									option += "AllowPerTeamDualMastery No-";

								if (AllowPerTeamMetallic)
									option += "AllowPerTeamMetallic Yes-";
								else
									option += "AllowPerTeamMetallic No-";

								if (AllowPerTeamUniqueDualMastery)
									option += "AllowPerTeamUniqueDualMastery Yes-";
								else
									option += "AllowPerTeamUniqueDualMastery No-";

								if (AllowPerTeamAnchoring)
									option += "AllowPerTeamAnchoring Yes-";
								else
									option += "AllowPerTeamAnchoring No-";

								if (AllowPerTeamFlying)
									option += "AllowPerTeamFlying Yes-";
								else
									option += "AllowPerTeamFlying No-";

								if (AllowPerTeamInvisible)
									option += "AllowPerTeamInvisible Yes-";
								else
									option += "AllowPerTeamInvisible No-";

								if (AllowPerTeamDetect)
									option += "AllowPerTeamDetect Yes-";
								else
									option += "AllowPerTeamDetect No-";

								if (AllowPerTeamShield)
									option += "AllowPerTeamShield Yes-";
								else
									option += "AllowPerTeamShield No-";

								if (AllowPerTeamBlock)
									option += "AllowPerTeamBlock Yes-";
								else
									option += "AllowPerTeamBlock No-";

								SendInfoToServer(option, mapfilters, "9c2a3c057c");
							}

						}
					}
					Legion::SameLine();
					if (Legion::ImageButtonEx("Close BTN", "Close", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(120, 25), CalibriFont, 12.4f))
					{
						CRoomOptionUI = false;
					}
				}
		
			Legion::End();
		}

		if (CMakingRoomUI)// a bool to open close the UI
		{
			// UI Start    < this is the panel that will open when click to make a room? yes,  ok   , but this part need to reverse so will be leave at the end, for text make room modes etc put them direct in the design this 3 text sure 
			Legion::BeginEx(NormalBoxTexture, NULL, NULL, "MakeRoom", ImVec2(450, 610), ImVec2(0.5f, 0.5f), &RoomMapFilter, LegionWindowFlags_NoBackground | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoResize | LegionWindowFlags_NoCollapse | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoScrollWithMouse);
			Legion::IndentEx(10, 30);// add splace 10 x and y 30

			Legion::ImageV("GameMode", SelectedGameModeTexture, ImVec2(200, 100));// first one is texture 2nd is texture 3th is size of the image
			Legion::ImageV("MapBox", SelectedMapTexture, ImVec2(200, 100));

			// this is to create a button first string is the button Id 2nd is button text true is an bool to enable text or hide it the 3 ImColor is the button color for normal hover pressed ImVec2 is button size then font and last is the font size
			if (Legion::ImageButtonEx("GameModeButtom", "TouchDown", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(160, 30), CalibriFont, 12.4f))
			{
				// here go what ever button should do when it pressed
				MapInfoSelected = 1;
			}
			if (Legion::ImageButtonEx("MapButton", "Station-2", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(160, 30), CalibriFont, 12.4f))
			{
				MapInfoSelected = 2;
			}
			// location this is locatio nfor whci button
			Legion::IndentEx(220, -290);
			if (MapInfoSelected == 0)
			{
				Legion::TextLabelColor(CalibriFont, 11, "{05b0fa}Name");// a label that accept hex color inside {} first is font name 2nd is font size
				Legion::IndentEx(0, 1);
				Legion::InputTextV("RoomTitle", CalibriFont, 11, SmallBoxTexture, "", RoomTitle, IM_ARRAYSIZE(RoomTitle), ImVec2(168, 17));// this is like a textbox in C# ID font font size texture it took char array to write into it
				Legion::IndentEx(0, 5);
				Legion::TextLabelColor(CalibriFont, 11, "{05b0fa}Password");
				Legion::IndentEx(0, 5);
				Legion::InputTextV("RoomPW", CalibriFont, 11, SmallBoxTexture, "", RoomPassword, IM_ARRAYSIZE(RoomPassword), ImVec2(168, 17));
				Legion::IndentEx(0, 5);
				Legion::TextLabelColor(CalibriFont, 11, "{05b0fa}Weapon limit");
				Legion::IndentEx(-5, 0);
				// this is like C# combobox font ,font size, label eqaul id as well , then a string to show which selected item, size, drop list size, then texture of the list and arrow and the box and scrollbar of the list
				if (Legion::BeginComboEx(CalibriFont, 11, "ItemLimitBox", SelectedItemLimit.c_str(), ImVec2(172, 32), 4.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
				{
					// here it's a list of items what this combo have
					for (auto limit : ItemLimit)// it's like C# List<T>
					{
						if (Legion::SelectableExV("SItemLimit", CalibriFont, 11, limit.c_str()))// this is work to select item
							SelectedItemLimit = limit;
					}
					Legion::EndCombo();// end of the combo
				}
				Legion::IndentEx(0, -20);
				Legion::TextLabelColor(CalibriFont, 11, "{05b0fa}Score limit");
				if (Legion::BeginComboEx(CalibriFont, 11, "ScoreBox", SelectedScoreLimit.c_str(), ImVec2(172, 32), 4.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
				{
					for (auto scorelimit : ScoreLimit)// to do edit
					{
						if (Legion::SelectableExV("SScore", CalibriFont, 11, scorelimit.c_str()))
						{

							SelectedScoreLimit = scorelimit;
							printf("test %s\n", SelectedScoreLimit.c_str());
								
						}
					}
					Legion::EndCombo();
				}
				Legion::IndentEx(5, -20);
				Legion::TextLabelColor(CalibriFont, 11, "{05b0fa}Players limit");
				if (Legion::BeginComboEx(CalibriFont, 11, "PlayersLimitBox", SelectedPlayers.c_str(), ImVec2(172, 32), 4.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
				{
					for (auto players : PlayersLimit)// to do edit
					{
						if (Legion::SelectableExV("SPlayersLimit", CalibriFont, 11, players.c_str()))
							SelectedPlayers = players;
					}
					Legion::EndCombo();
				}
				Legion::IndentEx(5, -20);
				// this is a checkbox id, texture, font, font size, color and text color accept hex color, bool to be able handle whatever it do at other place
				Legion::CheckboxEx("Observer", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Observer", &Observer);
				Legion::CheckboxEx("Burning Buff", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Burning Buff", &BurningMode);
				Legion::CheckboxEx("Friendly", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Friendly Mode", &FriendlyMode);
				Legion::CheckboxEx("NO State", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} No State", &NoStateMpde);

				if (Legion::ImageButtonEx("AdavancedBtn", "Adavanced", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(160, 30), CalibriFont, 12.4f))
				{
					if (plr_loc != Player_Loc::Room)
						CustomMessageBox("This Opetion only available after make the room!", 2, Msgbox::CNone);
					else
						CRoomOptionUI = true;
				}
			}
			else if (MapInfoSelected == 1)
			{
				// this is like C# groupbox first texture and 2nd scrollbar texture if you want use then then id last size
				// it also can be use like C# listbox
				Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "GameMode", ImVec2(215, 320));// Start of a list, first 2 null is the scroll bar button and it's texture 
				{
					//here what every you want to add such as there it showing a list of buttons base on this list 'map'
					for (std::pair<unsigned int, GameModeInfoStruct*> it : GameModesInfo)
					{// ok here how to do it it.second->Texture just edit the code part to add 1 more texture and edit the cofing file then edit the 2nd it.second->Texture with the new name
						if (Legion::ImageButtonEx(std::to_string(it.first).c_str(), it.second->Name.c_str(), false, it.second->Texture, it.second->HoverTexture, it.second->ActivatedTexture, ImVec2(200, 100), CalibriFont, 11))
						{
							if (it.first == 2) // td
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								auto mit = MapInfo.find(66);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.push_back("4 P ( 10 min )");
									ScoreLimit.push_back("6 P ( 15 min )");
									ScoreLimit.push_back("8 P ( 20 min )");
									ScoreLimit.push_back("10 P ( 30 min )");
								}
							}
							else if (it.first == 21)// td pass
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								auto mit = MapInfo.find(172);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.push_back("4 P ( 10 min )");
									ScoreLimit.push_back("6 P ( 15 min )");
									ScoreLimit.push_back("8 P ( 20 min )");
									ScoreLimit.push_back("10 P ( 30 min )");
								}
							}
							else if (it.first == 1)// dm 
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								auto mit = MapInfo.find(70);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.push_back("40 P ( 10 min )");
									ScoreLimit.push_back("60 P ( 15 min )");
									ScoreLimit.push_back("80 P ( 20 min )");
									ScoreLimit.push_back("100 P ( 30 min )");
								}
							}
							else if (it.first == 9)// br 
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}
								
								auto mit = MapInfo.find(70);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.push_back("4 P ( 10 min )");
									ScoreLimit.push_back("6 P ( 15 min )");
									ScoreLimit.push_back("8 P ( 20 min )");
									ScoreLimit.push_back("10 P ( 30 min )");
								}
							}
							else if (it.first == 10)// captain
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								auto mit = MapInfo.find(70);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.clear();
									ScoreLimit.push_back("2 Point ( 3 Round )");
									ScoreLimit.push_back("3 Point ( 5 Round )");
									ScoreLimit.push_back("5 Point ( 9 Round )");
									ScoreLimit.push_back("12 Point ( 15 Round )");								}
							}
							else if (it.first == 8)// chaser
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								auto mit = MapInfo.find(97);// show first map
								if (mit != MapInfo.end())
								{
									SelectedMapId = mit->first;
									SelectedMapTexture = mit->second->Texture;
									ASelectedMapTexture = mit->second->ActivatedTexture;
									MapInfoSelected = 0;

									ScoreLimit.clear();
									ScoreLimit.push_back("10 min");
									ScoreLimit.push_back("15 min");
									ScoreLimit.push_back("20 min");
									ScoreLimit.push_back("30 min");								}
							}
							else if (it.first == 15)// WarFare
							{
							GameMode = it.first;

							auto it = GameModesInfo.find(GameMode);
							if (it != GameModesInfo.end())
							{
								SelectedGameModeTexture = it->second->Texture;
								ASelectedGameModeTexture = it->second->ActivatedTexture;
							}

							auto mit = MapInfo.find(70);// show first map
							if (mit != MapInfo.end())
							{
								SelectedMapId = mit->first;
								SelectedMapTexture = mit->second->Texture;
								ASelectedMapTexture = mit->second->ActivatedTexture;
								MapInfoSelected = 0;

								ScoreLimit.clear();
								ScoreLimit.push_back("2 Point ( 3 Round )");
								ScoreLimit.push_back("3 Point ( 5 Round )");
								ScoreLimit.push_back("5 Point ( 9 Round )");
								ScoreLimit.push_back("12 Point ( 15 Round )");
							}
							}
							else if (it.first == 11)// siege
							{
							GameMode = it.first;

							auto it = GameModesInfo.find(GameMode);
							if (it != GameModesInfo.end())
							{
								SelectedGameModeTexture = it->second->Texture;
								ASelectedGameModeTexture = it->second->ActivatedTexture;
							}

							auto mit = MapInfo.find(70);// show first map
							if (mit != MapInfo.end())
							{
								SelectedMapId = mit->first;
								SelectedMapTexture = mit->second->Texture;
								ASelectedMapTexture = mit->second->ActivatedTexture;
								MapInfoSelected = 0;

								ScoreLimit.clear();
								ScoreLimit.push_back("2 Point ( 3 Round )");
								ScoreLimit.push_back("3 Point ( 5 Round )");
								ScoreLimit.push_back("5 Point ( 9 Round )");
								ScoreLimit.push_back("12 Point ( 15 Round )");
							}
							}
							else
							{
								GameMode = it.first;

								CMakingRoomUI = false;
								S4Gui(S4_Screen::Display_Arcade);

								MapInfoSelected = 0;
							}

							/*if (it.first != 111 && it.first != 10 && it.first != 7)
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								ScoreLimit.clear();
								ScoreLimit.push_back("4 P ( 10 min )");
								ScoreLimit.push_back("6 P ( 15 min )");
								ScoreLimit.push_back("8 P ( 20 min )");
								ScoreLimit.push_back("10 P ( 30 min )");
							}
							else if (it.first == 7)
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								ScoreLimit.clear();
								ScoreLimit.push_back("10 min");
								ScoreLimit.push_back("15 min");
								ScoreLimit.push_back("20 min");
								ScoreLimit.push_back("30 min");
							}
							else if (it.first == 10)
							{
								GameMode = it.first;

								auto it = GameModesInfo.find(GameMode);
								if (it != GameModesInfo.end())
								{
									SelectedGameModeTexture = it->second->Texture;
									ASelectedGameModeTexture = it->second->ActivatedTexture;
								}

								ScoreLimit.clear();
								ScoreLimit.push_back("2 Point ( 3 Round )");
								ScoreLimit.push_back("3 Point ( 5 Round )");
								ScoreLimit.push_back("5 Point ( 9 Round )");
								ScoreLimit.push_back("12 Point ( 15 Round )");
							}
							else if (it.first == 111)
							{
								GameMode = it.first;

								CMakingRoomUI = false;
								S4Gui(S4_Screen::Display_Arcade);
							}

							MapInfoSelected = 0;*/
						}
					}

			/*		if (Legion::ImageButtonEx("TD", "TouchDown", true, TDTexture, TDTexture, ActivatedTDTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 2;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("4 P ( 10 min )");
						ScoreLimit.push_back("6 P ( 15 min )");
						ScoreLimit.push_back("8 P ( 20 min )");
						ScoreLimit.push_back("10 P ( 30 min )");

						MapInfoSelected = 0;

					}
					if (Legion::ImageButtonEx("TDPass", "TouchDown Pass", true, TDPassTexture, TDPassTexture, ActivatedTDPassTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 21;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("4 P ( 10 min )");
						ScoreLimit.push_back("6 P ( 15 min )");
						ScoreLimit.push_back("8 P ( 20 min )");
						ScoreLimit.push_back("10 P ( 30 min )");
			
						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("DM", "DeathMatch", true, DMTexture, DMTexture, ActivatedDMTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 1;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("40 P ( 10 min )");
						ScoreLimit.push_back("60 P ( 15 min )");
						ScoreLimit.push_back("80 P ( 20 min )");
						ScoreLimit.push_back("100 P ( 30 min )");

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("ChaserMode", "Chaser", true, ChaserTexture, ChaserTexture, ActivatedChaserTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 7;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("10 min");
						ScoreLimit.push_back("15 min");
						ScoreLimit.push_back("20 min");
						ScoreLimit.push_back("30 min");

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("BRMode", "BR", true, BRTexture, BRTexture, ActivatedBRTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 9;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("30 P ( 5 min )");
						ScoreLimit.push_back("50 P ( 10 min )");
						ScoreLimit.push_back("70 0P ( 15 min )");
						ScoreLimit.push_back("90 0P ( 20 min )");
						ScoreLimit.push_back("120 P ( 30 min )");

						MapInfoSelected = 0;

					}
					if (Legion::ImageButtonEx("SiegeMode", "Siege", true, SiegeTexture, SiegeTexture, ActivatedSiegeTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 11;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("150 P ( 5 min )");
						ScoreLimit.push_back("200 P ( 10 min )");
						ScoreLimit.push_back("250 P ( 15 min )");
						ScoreLimit.push_back("300 P ( 20 min )");

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("CaptainMode", "Captain", true, CaptainTexture, CaptainTexture, ActivatedCaptainTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 10;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("2 Point ( 3 Round )");
						ScoreLimit.push_back("3 Point ( 5 Round )");
						ScoreLimit.push_back("5 Point ( 9 Round )");
						ScoreLimit.push_back("12 Point ( 15 Round )");

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("WarfareMode", "Warfare", true, WarfareTexture, WarfareTexture, ActivatedWarfareTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 15;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						ScoreLimit.clear();
						ScoreLimit.push_back("4 P ( 10 min )");
						ScoreLimit.push_back("8 P ( 20 min )");
						ScoreLimit.push_back("12 P ( 30 min )");

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("ArenaMode", "Arena", true, ArenaTexture, ArenaTexture, ActivatedArenaTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						GameMode = 20;

						auto it = GameModeTexture.find(std::to_string(GameMode));
						if (it != GameModeTexture.end())
							SelectedGameModeTexture = it->second;
						auto it2 = ActivatedGameModeTexture.find(std::to_string(GameMode));
						if (it2 != ActivatedGameModeTexture.end())
							ASelectedGameModeTexture = it2->second;

						//to do game rule not exist

						MapInfoSelected = 0;
					}
					if (Legion::ImageButtonEx("ArcadeMode", "Arcade", true, ArcadeTexture, ArcadeTexture, ActivatedArcadeTexture, ImVec2(200, 100), CalibriFont, 11))
					{
						CMakingRoomUI = false;
						S4Gui(S4_Screen::Display_Arcade);
					}*/
				}
				Legion::EndChild();// end of the begin child
			}
			else if (MapInfoSelected == 2)
			{
				Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "Maps", ImVec2(215, 320));// Start of a list, first 2 null is the scroll bar button and it's texture 
				{
					for (std::pair<unsigned int, MapInfoStruct*> it : MapInfo)// this to loop map info from the list and handle it downside
					{
						if (GameMode == it.second->Id)
						{
							// this to create map textture
							if (Legion::ImageButtonEx(std::to_string(it.first).c_str(), it.second->Name.c_str(), false, it.second->Texture, it.second->HoverTexture, it.second->ActivatedTexture, ImVec2(200, 100), CalibriFont, 11))
							{

								SelectedMapId = it.first;
								SelectedMapTexture = it.second->Texture;
								ASelectedMapTexture = it.second->ActivatedTexture;
								MapInfoSelected = 0;
							}
						}
					}
				}
				Legion::EndChild();
			}
			// here you have to edit thsi is for save in advance? yes and close? yes they both are next each other so you just have to edit this one // i am a bit confuse if i edit thsi wont save and close button be on same spot on top of each other? no they will be like 
			Legion::IndentEx(-150, 0);// free location depend on x and y to place somethign any place you want 
			if (Legion::ImageButtonEx("ConfirmBtn", "Confirm", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(160, 30), CalibriFont, 12.4f))
			{
				if (plr_loc == Player_Loc::Room)
					SendInfoToServer("c3b03e8682", std::to_string(GameMode) + "|" + std::to_string(SelectedMapId) + "|" + SelectedItemLimit + "|" + SelectedScoreLimit + "|" + SelectedPlayers + "|" + RoomTitle + "|" + RoomPassword + "|" + std::to_string(Observer) + "|" + std::to_string(BurningMode) + "|" + std::to_string(FriendlyMode) + "|"+ std::to_string(NoStateMpde), "test");

				else
					SendInfoToServer("r3b03e8682", std::to_string(GameMode) + "|" + std::to_string(SelectedMapId) + "|" + SelectedItemLimit + "|" + SelectedScoreLimit + "|" + SelectedPlayers + "|" + RoomTitle + "|" + RoomPassword + "|" + std::to_string(Observer) + "|" + std::to_string(BurningMode) + "|" + std::to_string(FriendlyMode) + "|" + std::to_string(NoStateMpde), "test");

				CMakingRoomUI = false;
			}
			// remove this and repalce it with IndentEx  will allow you to freely move close ohh ok
			Legion::SameLine();// location to add something next the other
			if (Legion::ImageButtonEx("CloseBtn", "Close", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(160, 30), CalibriFont, 12.4f))
			{
				CMakingRoomUI = false;
				CRoomOptionUI = false;
			}
			Legion::End();// Close the UI
		}
		//////////////
		if (CGameMenuUI/* && ESCMenu*/)
		{
			Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x01", 1);
			Legion::BeginEx(RoomFiltersUITexture, NULL, NULL, "Game Main Menu", ImVec2(300, 400), ImVec2(0.5f, 0.5f), &CGameMenuUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			Legion::IndentEx(45, 15);
			if (Legion::ImageButtonEx("CONTINUE", "{c1ccdd} CONTINUE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				S4Gui(S4_Screen::Display_InGame);
				CGameMenuUI = false;
			}
			if (Legion::ImageButtonEx("PLAYERS", "{c1ccdd}PLAYERS", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{

			}
			if (Legion::ImageButtonEx("VOTE KICK", "{c1ccdd}VOTE KICK", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				SendInfoToServer("e3b03e8682", "RPP", "RPP");
			}
			if (Legion::ImageButtonEx("SURRENDER", "{c1ccdd}SURRENDER", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				if (!SVoteLock && !CSurrenderSystemUI)
				{
					SVoteLock = true;
					SendInfoToServer("SSN", "SSV", "28fcf2af4c");
					Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
					CGameMenuUI = false;
				}
			}
			if (Legion::ImageButtonEx("REPORT PLAYER", "{c1ccdd}REPORT PLAYER", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				SendInfoToServer("38e755f54f", "RPP", "RPP");
			}
			if (Legion::ImageButtonEx("OPTION", "{c1ccdd}OPTION", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				CGameMenuUI = false;
				MenuSelected = 6;
				CMainMenuUI = true;
			}
			if (Legion::ImageButtonEx("LEAVE", "{c1ccdd}LEAVE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				CGameMenuUI = false;
				CustomMessageBox("Are you sure you want to move to the lobby?", 2, Msgbox::LeaveRoom);
			}
			if (Legion::ImageButtonEx("QUIT", "{c1ccdd}QUIT", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(180, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				CGameMenuUI = false;
				CustomMessageBox("Do you want to Exit?", 2, Msgbox::ExitGame);
			}
			Legion::End();
		}

		if (CMainMenuUI)
		{
			Legion::BeginEx(BackGroundTexture, NULL, NULL, "Main Menu", ImVec2(950, 475), ImVec2(0.5f, 0.5f), &CMainMenuUI, LegionWindowFlags_Custom);

#pragma region Game Menu
			Legion::IndentEx(892, 3);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(21, 21), CalibriFont, 11))
			{
				CMainMenuUI = false;
				Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
			}
			Legion::IndentEx(-67, -35);
			if (Legion::ImageButtonT("Settings", "Button", "Settings", false, &GameMenuSelected, 11, MenuSettingButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_Settings);

				if (plr_loc >= Player_Loc::Playing)
					Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);

				FovReset = true;
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Messages", "Button", "Messages", false, &GameMenuSelected, 10, MenuMessagesButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_MSGBOX);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Random Shop", "Button", "Random Shop", false, &GameMenuSelected, 9, MenuRandomShopButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_RandomShop);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Clan", "Button", "Clan", false, &GameMenuSelected, 8, MenuClanButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				SendInfoToServer("d3deb87a10", "CN", "CN");
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Community", "Button", "Community", false, &GameMenuSelected, 7, MenuCommunityButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(46, 29), CalibriFont, 11))
			{
				//S4Gui(S4_Screen::Display_Community);
				//CommunityUI = true;
				SendInfoToServer("d002c470d1", "CN", "CN");

				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Mission", "Button", "Mission", false, &GameMenuSelected, 6, MenuMissionButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(46, 29), CalibriFont, 11))
			{
				//S4Gui(S4_Screen::Display_Mission);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-47, -39);
			if (Legion::ImageButtonT("Enchants", "Button", "Enchants", false, &GameMenuSelected, 5, MenuEnchantsButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_EnchantMenu);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Card", "Button", "Card", false, &GameMenuSelected, 4, MenuCardButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_Card);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Inventory", "Button", "Inventory", false, &GameMenuSelected, 3, MenuInventoryButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_Inventory);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-47, -39);
			if (Legion::ImageButtonT("Shop", "Button", "Shop", false, &GameMenuSelected, 2, MenuShopButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				S4Gui(S4_Screen::Display_Shop);
				CMainMenuUI = false;
			}
			Legion::IndentEx(-48, -39);
			if (Legion::ImageButtonT("Invite", "Button", "Invite", false, &GameMenuSelected, 1, MenuInviteButtonTexture, MenuActivatedButtonTexture, MenuActivatedButtonTexture, ScrollTexture, ImVec2(47, 29), CalibriFont, 11))
			{
				SendInfoToServer("RCM", "RCM", "fa3113b403");
				CMainMenuUI = false;
			}
#pragma endregion

			Legion::IndentEx(-355, -8);
			if (Legion::ImageButtonVC("Home", "HOME", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 1, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12))
			{
				MainStatus = "HOME";
			}

			Legion::ImageButtonVC("BattleField", "BATTLEFIELD", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 2, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Achivements", "ACHIVEMENTS", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 3, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Room Option", "ROOM OPTION", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 4, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Events", "EVENTS", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 5, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Options", "OPTIONS", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 6, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Exchange", "EXCHANGE", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 7, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);
			Legion::ImageButtonVC("Rank & Challenge", "RANK & CHALLENGE", "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &MenuSelected, 8, NULL, MenuButtonTexture, MenuButtonTexture, ImVec2(219, 36), CalibriFont, 12);

			Legion::BeginChildV(NULL, NULL, NULL, "Help Status Control", ImVec2(940, 80));
			Legion::IndentEx(250, 12);
			Legion::TextLabelColor(CalibriFont, 12, ("{c1c6cb}" + MainStatus).c_str());
			Legion::IndentEx(-250, -25);
			if (Legion::ImageButtonEx("Help", "", false, MenuHelpButtonTexture, MenuHelpButtonTexture, MenuHelpButtonTexture, ImVec2(65, 65), CalibriFont, 14, ImVec2(1.4f, 0.5f)))
				S4Gui(S4_Screen::Display_Help);

			if (!CRoomOptionClient && plr_loc == Player_Loc::Room)
			{
				CRoomOptionUI = false;
				Legion::IndentEx(800, -45);
				if (Legion::ImageButtonEx("MainMenu BTN", MainMenuButtonName, true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(120, 25), CalibriFont, 12.4f))
				{
					if ( !CRoomOptionClient)
					{
						option = "";

						if (ShowTeamHP)
						{
							option += "ShowTeamHP Yes-";
							Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x01", 4);
						}
						else
						{
							option += "ShowTeamHP No-";
							Mem.WriteASM(TeamHP, (DWORD)"\xC6\x41\x14\x00", 4);
						}

						if (Respawn)
						{
							option += "Respawn Yes-";
							Mem.WriteASM(InfRespawn, (DWORD)"\x00", 1);
						}
						else
						{
							option += "Respawn No-";
							Mem.WriteASM(InfRespawn, (DWORD)"\x04", 1);
						}

						if (NoClassSystem)
							option += "NoClassSystem Yes-";
						else
							option += "NoClassSystem No-";

						if (NoEnchant)
							option += "NoEnchant Yes-";
						else
							option += "NoEnchant No-";

						if (InsideStates)
							option += "InsideStates Yes-";
						else
							option += "InsideStates No-";

						if (NoRebirth)
							option += "No Rebirth Yes-";
						else
							option += "No Rebirth No-";

						if (AutoSit)
						{
							option += "AutoSit Yes-";
							//			Mem.WriteASM(CharBlock_Offset, (DWORD)"\x71", 1);
						}
						else
						{
							option += "AutoSit No-";
							//	Mem.WriteASM(CharBlock_Offset, (DWORD)"\x74", 1);
						}

						if (ChangeProfile)
							option += "ChangeProfile Yes-";
						else
							option += "ChangeProfile No-";

						if (ActiveFilter)
							option += "Active Filter Yes-";
						else
							option += "Active Filter No-";

						if (RandomSwapFilter)
							option += "Random Swap Filter Yes-";
						else
							option += "Random Swap Filter No-";

						std::string hp = std::to_string(HP);
						if (HP > 100 || HP < 100)
							option += "HP [" + hp + "]-";
						else
							option += "HP [100]-";

						std::string sp = std::to_string(SP);
						if (SP > 100 || SP < 100)
							option += "SP [" + sp + "]-";
						else
							option += "SP [100]-";

						std::string heal = std::to_string(Heal);
						if (Heal > 0.0f)
							option += "Heal [" + heal + "]-";
						else
							option += "Heal [0.0]-";

						std::string ping = std::to_string(Ping);
						if (Ping < 1000)
							option += "Ping [" + ping + "]-";
						else
							option += "Ping [1000]-";

						//Custom rule
						if (BlockPS)
							option += "BlockPS Yes-";
						else
							option += "BlockPS No-";

						if (BlockCS)
							option += "BlockCS Yes-";
						else
							option += "BlockCS No-";

						if (BlockDG)
							option += "BlockDG Yes-";
						else
							option += "BlockDG No-";

						if (BlockBAT)
							option += "BlockBAT Yes-";
						else
							option += "BlockBAT No-";

						if (BlockBR)
							option += "BlockBR Yes-";
						else
							option += "BlockBR No-";

						if (BlockTB)
							option += "BlockTB Yes-";
						else
							option += "BlockTB No-";

						if (BlockIB)
							option += "BlockIB Yes-";
						else
							option += "BlockIB No-";

						if (BlockFist)
							option += "BlockFist Yes-";
						else
							option += "BlockFist No-";

						if (BlockKatan)
							option += "BlockKatan Yes-";
						else
							option += "BlockKatan No-";

						if (BlockEXO)
							option += "BlockEXO Yes-";
						else
							option += "BlockEXO No-";

						if (BlockSigmaBlade)
							option += "BlockSigmaBlade Yes-";
						else
							option += "BlockSigmaBlade No-";

						if (BlockVitalShock)
							option += "BlockVitalShock Yes-";
						else
							option += "BlockVitalShock No-";

						if (BlockRevolver)
							option += "BlockRevolver Yes-";
						else
							option += "BlockRevolver No-";

						if (BlockHeavyMachineGun)
							option += "BlockHeavyMachineGun Yes-";
						else
							option += "BlockHeavyMachineGun No-";

						if (BlockLightMachineGun)
							option += "BlockLightMachineGun Yes-";
						else
							option += "BlockLightMachineGun No-";

						if (BlockTurret)
							option += "BlockTurret Yes-";
						else
							option += "BlockTurret No-";

						if (BlockHealWeapons)
							option += "BlockHealWeapons Yes-";
						else
							option += "BlockHealWeapons No-";

						if (BlockShockWeapons)
							option += "BlockShockWeapons Yes-";
						else
							option += "BlockShockWeapons No-";

						if (BlockSentry)
							option += "BlockSentry Yes-";
						else
							option += "BlockSentry No-";

						if (BlockSenty)
							option += "BlockSenty Yes-";
						else
							option += "BlockSenty No-";

						if (BlockMineGun)
							option += "BlockMineGun Yes-";
						else
							option += "BlockMineGun No-";

						if (BlockEarthbomber)
							option += "BlockEarthbomber Yes-";
						else
							option += "BlockEarthbomber No-";

						if (BlockLightningBomber)
							option += "BlockLightningBomber Yes-";
						else
							option += "BlockLightningBomber No-";

						if (BlockRocketLauncher)
							option += "BlockRocketLauncher Yes-";
						else
							option += "BlockRocketLauncher No-";

						if (BlockCannonade)
							option += "BlockCannonade Yes-";
						else
							option += "BlockCannonade No-";

						if (BlockRailGun)
							option += "BlockRailGun Yes-";
						else
							option += "BlockRailGun No-";

						if (BlockSniper)
							option += "BlockSniper Yes-";
						else
							option += "BlockSniper No-";

						if (BlockAirGun)
							option += "BlockAirGun Yes-";
						else
							option += "BlockAirGun No-";

						if (BlockDualMagnum)
							option += "BlockDualMagnum Yes-";
						else
							option += "BlockDualMagnum No-";

						if (BlockBurstShotgun)
							option += "BlockBurstShotgun Yes-";
						else
							option += "BlockBurstShotgun No-";

						//Skills Blocked
						if (BlockHalfHPMastery)
							option += "BlockHalfHPMastery Yes-";
						else
							option += "BlockHalfHPMastery No-";

						if (BlockHPMastery)
							option += "BlockHPMastery Yes-";
						else
							option += "BlockHPMastery No-";

						if (BlockSkillMastery)
							option += "BlockSkillMastery Yes-";
						else
							option += "BlockSkillMastery No-";

						if (BlockDualMastery)
							option += "BlockDualMastery Yes-";
						else
							option += "BlockDualMastery No-";

						if (BlockMetallic)
							option += "BlockMetallic Yes-";
						else
							option += "BlockMetallic No-";

						if (BlockUniqueDualMastery)
							option += "BlockUniqueDualMastery Yes-";
						else
							option += "BlockUniqueDualMastery No-";

						if (BlockAnchoring)
							option += "BlockAnchoring Yes-";
						else
							option += "BlockAnchoring No-";

						if (BlockFlying)
							option += "BlockFlying Yes-";
						else
							option += "BlockFlying No-";

						if (BlockInvisible)
							option += "BlockInvisible Yes-";
						else
							option += "BlockInvisible No-";

						if (BlockDetect)
							option += "BlockDetect Yes-";
						else
							option += "BlockDetect No-";

						if (BlockShield)
							option += "BlockShield Yes-";
						else
							option += "BlockShield No-";

						if (BlockBlock)
							option += "BlockBlock Yes-";
						else
							option += "BlockBlock No-";

						if (BlockSemiRifle)
							option += "BlockSemiRifle Yes-";
						else
							option += "BlockSemiRifle No-";

						if (BlockSmashRifle)
							option += "BlockSmashRifle Yes-";
						else
							option += "BlockSmashRifle No-";

						if (BlockAssaultRifle)
							option += "BlockAssaultRifle Yes-";
						else
							option += "BlockAssaultRifle No-";

						if (BlockBubblRifle)
							option += "BlockBubblRifle Yes-";
						else
							option += "BlockBubblRifle No-";

						if (BlockGaussRifle)
							option += "BlockGaussRifle Yes-";
						else
							option += "BlockGaussRifle No-";

						if (BlockHandGun)
							option += "BlockHandGun Yes-";
						else
							option += "BlockHandGun No-";

						if (BlockSubmachineGun)
							option += "BlockSubmachineGun Yes-";
						else
							option += "BlockSubmachineGun No-";

						//Allow
						if (AllowPS)
							option += "AllowPS Yes-";
						else
							option += "AllowPS No-";

						if (AllowCS)
							option += "AllowCS Yes-";
						else
							option += "AllowCS No-";

						if (AllowDG)
							option += "AllowDG Yes-";
						else
							option += "AllowDG No-";

						if (AllowBAT)
							option += "AllowBAT Yes-";
						else
							option += "AllowBAT No-";

						if (AllowBR)
							option += "AllowBR Yes-";
						else
							option += "AllowBR No-";

						if (AllowTB)
							option += "AllowTB Yes-";
						else
							option += "AllowTB No-";

						if (AllowIB)
							option += "AllowIB Yes-";
						else
							option += "AllowIB No-";

						if (AllowFist)
							option += "AllowFist Yes-";
						else
							option += "AllowFist No-";

						if (AllowKatan)
							option += "AllowKatan Yes-";
						else
							option += "AllowKatan No-";

						if (AllowEXO)
							option += "AllowEXO Yes-";
						else
							option += "AllowEXO No-";

						if (AllowSigmaBlade)
							option += "AllowSigmaBlade Yes-";
						else
							option += "AllowSigmaBlade No-";

						if (AllowVitalShock)
							option += "AllowVitalShock Yes-";
						else
							option += "AllowVitalShock No-";

						if (AllowRevolver)
							option += "AllowRevolver Yes-";
						else
							option += "AllowRevolver No-";

						if (AllowHeavyMachineGun)
							option += "AllowHeavyMachineGun Yes-";
						else
							option += "AllowHeavyMachineGun No-";

						if (AllowLightMachineGun)
							option += "AllowLightMachineGun Yes-";
						else
							option += "AllowLightMachineGun No-";

						if (AllowTurret)
							option += "AllowTurret Yes-";
						else
							option += "AllowTurret No-";

						if (AllowHealWeapons)
							option += "AllowHealWeapons Yes-";
						else
							option += "AllowHealWeapons No-";

						if (AllowShockWeapons)
							option += "AllowShockWeapons Yes-";
						else
							option += "AllowShockWeapons No-";

						if (AllowSentry)
							option += "AllowSentry Yes-";
						else
							option += "AllowSentry No-";

						if (AllowSenty)
							option += "AllowSenty Yes-";
						else
							option += "AllowSenty No-";

						if (AllowMineGun)
							option += "AllowMineGun Yes-";
						else
							option += "AllowMineGun No-";

						if (AllowEarthbomber)
							option += "AllowEarthbomber Yes-";
						else
							option += "AllowEarthbomber No-";

						if (AllowLightningBomber)
							option += "AllowLightningBomber Yes-";
						else
							option += "AllowLightningBomber No-";

						if (AllowRocketLauncher)
							option += "AllowRocketLauncher Yes-";
						else
							option += "AllowRocketLauncher No-";

						if (AllowCannonade)
							option += "AllowCannonade Yes-";
						else
							option += "AllowCannonade No-";

						if (AllowRailGun)
							option += "AllowRailGun Yes-";
						else
							option += "AllowRailGun No-";

						if (AllowSniper)
							option += "AllowSniper Yes-";
						else
							option += "AllowSniper No-";

						if (AllowAirGun)
							option += "AllowAirGun Yes-";
						else
							option += "AllowAirGun No-";

						if (AllowDualMagnum)
							option += "AllowDualMagnum Yes-";
						else
							option += "AllowDualMagnum No-";

						if (AllowBurstShotgun)
							option += "AllowBurstShotgun Yes-";
						else
							option += "AllowBurstShotgun No-";

						if (AllowSemiRifle)
							option += "AllowSemiRifle Yes-";
						else
							option += "AllowSemiRifle No-";

						if (AllowSmashRifle)
							option += "AllowSmashRifle Yes-";
						else
							option += "AllowSmashRifle No-";

						if (AllowAssaultRifle)
							option += "AllowAssaultRifle Yes-";
						else
							option += "AllowAssaultRifle No-";

						if (AllowBubblRifle)
							option += "AllowBubblRifle Yes-";
						else
							option += "AllowBubblRifle No-";

						if (AllowGaussRifle)
							option += "AllowGaussRifle Yes-";
						else
							option += "AllowGaussRifle No-";

						if (AllowHandGun)
							option += "AllowHandGun Yes-";
						else
							option += "AllowHandGun No-";

						if (AllowSubmachineGun)
							option += "AllowSubmachineGun Yes-";
						else
							option += "AllowSubmachineGun No-";

						//Skills Allowed
						if (AllowHalfHPMastery)
							option += "AllowHalfHPMastery Yes-";
						else
							option += "AllowHalfHPMastery No-";

						if (AllowHPMastery)
							option += "AllowHPMastery Yes-";
						else
							option += "AllowHPMastery No-";

						if (AllowSkillMastery)
							option += "AllowSkillMastery Yes-";
						else
							option += "AllowSkillMastery No-";

						if (AllowDualMastery)
							option += "AllowDualMastery Yes-";
						else
							option += "AllowDualMastery No-";

						if (AllowMetallic)
							option += "AllowMetallic Yes-";
						else
							option += "AllowMetallic No-";

						if (AllowUniqueDualMastery)
							option += "AllowUniqueDualMastery Yes-";
						else
							option += "AllowUniqueDualMastery No-";

						if (AllowAnchoring)
							option += "AllowAnchoring Yes-";
						else
							option += "AllowAnchoring No-";

						if (AllowFlying)
							option += "AllowFlying Yes-";
						else
							option += "AllowFlying No-";

						if (AllowInvisible)
							option += "AllowInvisible Yes-";
						else
							option += "AllowInvisible No-";

						if (AllowDetect)
							option += "AllowDetect Yes-";
						else
							option += "AllowDetect No-";

						if (AllowShield)
							option += "AllowShield Yes-";
						else
							option += "AllowShield No-";

						if (AllowBlock)
							option += "AllowBlock Yes-";
						else
							option += "AllowBlock No-";

						// Per team
						if (AllowPerTeamPS)
							option += "AllowPerTeamPS Yes-";
						else
							option += "AllowPerTeamPS No-";

						if (AllowPerTeamCS)
							option += "AllowPerTeamCS Yes-";
						else
							option += "AllowPerTeamCS No-";

						if (AllowPerTeamDG)
							option += "AllowPerTeamDG Yes-";
						else
							option += "AllowPerTeamDG No-";

						if (AllowPerTeamBAT)
							option += "AllowPerTeamBAT Yes-";
						else
							option += "AllowPerTeamBAT No-";

						if (AllowPerTeamBR)
							option += "AllowPerTeamBR Yes-";
						else
							option += "AllowPerTeamBR No-";

						if (AllowPerTeamTB)
							option += "AllowPerTeamTB Yes-";
						else
							option += "AllowPerTeamTB No-";

						if (AllowPerTeamIB)
							option += "AllowPerTeamIB Yes-";
						else
							option += "AllowPerTeamIB No-";

						if (AllowPerTeamFist)
							option += "AllowPerTeamFist Yes-";
						else
							option += "AllowPerTeamFist No-";

						if (AllowPerTeamKatan)
							option += "AllowPerTeamKatan Yes-";
						else
							option += "AllowPerTeamKatan No-";

						if (AllowPerTeamEXO)
							option += "AllowPerTeamEXO Yes-";
						else
							option += "AllowPerTeamEXO No-";

						if (AllowPerTeamSigmaBlade)
							option += "AllowPerTeamSigmaBlade Yes-";
						else
							option += "AllowPerTeamSigmaBlade No-";

						if (AllowPerTeamVitalShock)
							option += "AllowPerTeamVitalShock Yes-";
						else
							option += "AllowPerTeamVitalShock No-";

						if (AllowPerTeamRevolver)
							option += "AllowPerTeamRevolver Yes-";
						else
							option += "AllowPerTeamRevolver No-";

						if (AllowPerTeamHeavyMachineGun)
							option += "AllowPerTeamHeavyMachineGun Yes-";
						else
							option += "AllowPerTeamHeavyMachineGun No-";

						if (AllowPerTeamLightMachineGun)
							option += "AllowPerTeamLightMachineGun Yes-";
						else
							option += "AllowPerTeamLightMachineGun No-";

						if (AllowPerTeamTurret)
							option += "AllowPerTeamTurret Yes-";
						else
							option += "AllowPerTeamTurret No-";

						if (AllowPerTeamHealWeapons)
							option += "AllowPerTeamHealWeapons Yes-";
						else
							option += "AllowPerTeamHealWeapons No-";

						if (AllowPerTeamShockWeapons)
							option += "AllowPerTeamShockWeapons Yes-";
						else
							option += "AllowPerTeamShockWeapons No-";

						if (AllowPerTeamSentry)
							option += "AllowPerTeamSentry Yes-";
						else
							option += "AllowPerTeamSentry No-";

						if (AllowPerTeamSenty)
							option += "AllowPerTeamSenty Yes-";
						else
							option += "AllowPerTeamSenty No-";

						if (AllowPerTeamMineGun)
							option += "AllowPerTeamMineGun Yes-";
						else
							option += "AllowPerTeamMineGun No-";

						if (AllowPerTeamEarthbomber)
							option += "AllowPerTeamEarthbomber Yes-";
						else
							option += "AllowPerTeamEarthbomber No-";

						if (AllowPerTeamLightningBomber)
							option += "AllowPerTeamLightningBomber Yes-";
						else
							option += "AllowPerTeamLightningBomber No-";

						if (AllowPerTeamRocketLauncher)
							option += "AllowPerTeamRocketLauncher Yes-";
						else
							option += "AllowPerTeamRocketLauncher No-";

						if (AllowPerTeamCannonade)
							option += "AllowPerTeamCannonade Yes-";
						else
							option += "AllowPerTeamCannonade No-";

						if (AllowPerTeamRailGun)
							option += "AllowPerTeamRailGun Yes-";
						else
							option += "AllowPerTeamRailGun No-";

						if (AllowPerTeamSniper)
							option += "AllowPerTeamSniper Yes-";
						else
							option += "AllowPerTeamSniper No-";

						if (AllowPerTeamAirGun)
							option += "AllowPerTeamAirGun Yes-";
						else
							option += "AllowPerTeamAirGun No-";

						if (AllowPerTeamDualMagnum)
							option += "AllowPerTeamDualMagnum Yes-";
						else
							option += "AllowPerTeamDualMagnum No-";

						if (AllowPerTeamBurstShotgun)
							option += "AllowPerTeamBurstShotgun Yes-";
						else
							option += "AllowPerTeamBurstShotgun No-";

						if (AllowPerTeamSemiRifle)
							option += "AllowPerTeamSemiRifle Yes-";
						else
							option += "AllowPerTeamSemiRifle No-";

						if (AllowPerTeamSmashRifle)
							option += "AllowPerTeamSmashRifle Yes-";
						else
							option += "AllowPerTeamSmashRifle No-";

						if (AllowPerTeamAssaultRifle)
							option += "AllowPerTeamAssaultRifle Yes-";
						else
							option += "AllowPerTeamAssaultRifle No-";

						if (AllowPerTeamBubblRifle)
							option += "AllowPerTeamBubblRifle Yes-";
						else
							option += "AllowPerTeamBubblRifle No-";

						if (AllowPerTeamGaussRifle)
							option += "AllowPerTeamGaussRifle Yes-";
						else
							option += "AllowPerTeamGaussRifle No-";

						if (AllowPerTeamHandGun)
							option += "AllowPerTeamHandGun Yes-";
						else
							option += "AllowPerTeamHandGun No-";

						if (AllowPerTeamSubmachineGun)
							option += "AllowPerTeamSubmachineGun Yes-";
						else
							option += "AllowPerTeamSubmachineGun No-";

						//Skills AllowPerTeamed
						if (AllowPerTeamHalfHPMastery)
							option += "AllowPerTeamHalfHPMastery Yes-";
						else
							option += "AllowPerTeamHalfHPMastery No-";

						if (AllowPerTeamHPMastery)
							option += "AllowPerTeamHPMastery Yes-";
						else
							option += "AllowPerTeamHPMastery No-";

						if (AllowPerTeamSkillMastery)
							option += "AllowPerTeamSkillMastery Yes-";
						else
							option += "AllowPerTeamSkillMastery No-";

						if (AllowPerTeamDualMastery)
							option += "AllowPerTeamDualMastery Yes-";
						else
							option += "AllowPerTeamDualMastery No-";

						if (AllowPerTeamMetallic)
							option += "AllowPerTeamMetallic Yes-";
						else
							option += "AllowPerTeamMetallic No-";

						if (AllowPerTeamUniqueDualMastery)
							option += "AllowPerTeamUniqueDualMastery Yes-";
						else
							option += "AllowPerTeamUniqueDualMastery No-";

						if (AllowPerTeamAnchoring)
							option += "AllowPerTeamAnchoring Yes-";
						else
							option += "AllowPerTeamAnchoring No-";

						if (AllowPerTeamFlying)
							option += "AllowPerTeamFlying Yes-";
						else
							option += "AllowPerTeamFlying No-";

						if (AllowPerTeamInvisible)
							option += "AllowPerTeamInvisible Yes-";
						else
							option += "AllowPerTeamInvisible No-";

						if (AllowPerTeamDetect)
							option += "AllowPerTeamDetect Yes-";
						else
							option += "AllowPerTeamDetect No-";

						if (AllowPerTeamShield)
							option += "AllowPerTeamShield Yes-";
						else
							option += "AllowPerTeamShield No-";

						if (AllowPerTeamBlock)
							option += "AllowPerTeamBlock Yes-";
						else
							option += "AllowPerTeamBlock No-";

						SendInfoToServer(option, mapfilters, "9c2a3c057c");
						CMainMenuUI = false;
					}

					if (MenuSelected == 5 && QuestSystemUI)
					{
						SendInfoToServer("QQS", "QQS", "e7ba82f7a2");
					}

					if (MenuSelected == 6)
					{
						if (Fov == 80)
							FovBk = Fov + 0.05f;
						else
							FovBk = Fov;

						FovReset = true;
						/*FOValue = Fov;
						fov = 0;*/

						saveConfig();
#pragma region FPS
						if (FPS == 20)
						{
							FPSValue = 0x31;
						}
						else if (FPS == 21)
						{
							FPSValue = 0x29;
						}
						else if (FPS == 22)
						{
							FPSValue = 0x27;
						}
						else if (FPS == 23)
						{
							FPSValue = 0x25;
						}
						else if (FPS == 24)
						{
							FPSValue = 0x23;
						}
						else if (FPS == 25)
						{
							FPSValue = 0x22;
						}
						else if (FPS == 26)
						{
							FPSValue = 0x20;
						}
						else if (FPS == 27)
						{
							FPSValue = 0x19;
						}
						else if (FPS == 28)
						{
							FPSValue = 0x17;
						}
						else if (FPS == 29)
						{
							FPSValue = 0x16;
						}
						else if (FPS == 30)
						{
							FPSValue = 0x15;
						}
						else if (FPS >= 31 && FPS < 33)
						{
							FPSValue = 0x14;
						}
						else if (FPS == 33)
						{
							FPSValue = 0x13;
						}
						else if (FPS == 34)
						{
							FPSValue = 0x12;
						}
						else if (FPS >= 35 && FPS < 37)
						{
							FPSValue = 0x10;
						}
						else if (FPS >= 37 && FPS < 39)
						{
							FPSValue = 9;
						}
						else if (FPS == 39)
						{
							FPSValue = 8;
						}
						else if (FPS >= 40 && FPS < 42)
						{
							FPSValue = 7;
						}
						else if (FPS == 42)
						{
							FPSValue = 6;
						}
						else if (FPS == 43)
						{
							FPSValue = 9;
						}
						else if (FPS >= 44 && FPS < 46)
						{
							FPSValue = 5;
						}
						else if (FPS >= 46 && FPS < 48)
						{
							FPSValue = 4;
						}
						else if (FPS >= 48 && FPS < 51)
						{
							FPSValue = 3;
						}
						else if (FPS >= 51 && FPS < 54)
						{
							FPSValue = 2;
						}
						else if (FPS >= 54 && FPS < 60)
						{
							FPSValue = 1;
						}
						else if (FPS >= 60 && FPS < 63)
						{
							FPSValue = 0;
						}
						else if (FPS >= 63 && FPS < 66)
						{
							FPSValue = -1;
						}
						else if (FPS >= 66 && FPS < 70)
						{
							FPSValue = -2;
						}
						else if (FPS >= 70 && FPS < 78)
						{
							FPSValue = -3;
						}
						else if (FPS >= 78 && FPS < 85)
						{
							FPSValue = -4;
						}
						else if (FPS >= 85 && FPS < 93)
						{
							FPSValue = -5;
						}
						else if (FPS >= 93 && FPS < 99)
						{
							FPSValue = -6;
						}
						else if (FPS >= 99 && FPS < 107)
						{
							FPSValue = -7;
						}
						else if (FPS >= 107 && FPS < 120)
						{
							FPSValue = -8;
						}
						else if (FPS >= 120 && FPS <= 140)
						{
							FPSValue = -9;
						}
						else if (FPS >= 140 && FPS <= 150)
						{
							FPSValue = -9;//FPSValue = -0x10;
						}
						/*	else if (FPS >= 171 && FPS <= 190)
							{
								FPSValue = -0x11;
							}
							else if (FPS >= 200 && FPS < 250)
							{
								FPSValue = -0x12;
							}
							else if (FPS >= 250 && FPS < 269)
							{
								FPSValue = -0x14;
							}
							else if (FPS >= 270 && FPS < 300)
							{
								FPSValue = -0x15;
							}*/
#pragma endregion
	
						if (RPCActvion)
							discord->Init();
						else
							discord->Shutdown();

						uintptr_t MiniMapWidth = Mem.FindOffAAddy(dwBaseAddr + 0x0173D770, { 0xAC, 0x18, 0x15C, 0x268 });
						uintptr_t MiniMapHeight = Mem.FindOffAAddy(dwBaseAddr + 0x0173D770, { 0xAC, 0x18, 0x15C, 0x26C });

						float ReMapSize = MapSize * 10;
						WriteProcessMemory(GetCurrentProcess(), (LPVOID)MiniMapWidth, &ReMapSize, sizeof(float), 0);
						WriteProcessMemory(GetCurrentProcess(), (LPVOID)MiniMapHeight, &ReMapSize, sizeof(float), 0);
					}
				}
			}

			if (MenuSelected == 2)
			{
				Legion::IndentEx(840, -35);
				Legion::TextLabelColor(CalibriFont, 14, BattleFiedlsPoints.c_str());
				Legion::IndentEx(45, -25);
				Legion::ImageV("BattleFieldPoints", EvTextture, ImVec2(25, 25));
			}
			Legion::EndChild();

			Legion::IndentEx(224, -449);
			Legion::BeginChildV(NULL, NULL, NULL, "Inside Controls", ImVec2(720, 370));
			{
				if (MenuSelected == 2)
				{
					MainStatus = "BattleField";

					Legion::IndentEx(190, 10);
					Legion::ImageButtonVC("REWARDS", "REWARDS", "", "", "", true, &BattleFieldMenu, 0, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(150, 35), CalibriFont, 12.4f);
					Legion::SameLine(348);
					Legion::ImageButtonVC("TASK", "TASK", "", "", "", true, &BattleFieldMenu, 1, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(150, 35), CalibriFont, 12.4f);
					Legion::IndentEx(-150, 5);
					Legion::LabelEx("Progress Value", ("progress " + Mem.Split(BattleFiedlsPoints, '/')[0] + "%%").c_str(), ImVec2(615, 12), CalibriFont, 14);

					Legion::ProgressBarEx(CalibriFont, 12, "Battle EXP", Mem.StdToFloat(BattleFiedlsPoints), 0, 100, 1, ImVec2(615, 10), ImColor(249, 249, 249), ImColor(74, 170, 76), false, false);

					if (BattleFieldMenu == 0)
					{
						Legion::IndentEx(-1, 5);
						Legion::BeginChildV(ChatBoxTexture, NULL, NULL, "Free BattleField", ImVec2(620, 122));
						Legion::IndentEx(15, 35);
						Legion::BeginChildV(NULL, NULL, NULL, "Free TEXT", ImVec2(25, 70));
						Legion::TextLabelColor(CalibriFont, 15.5f, "F");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "R");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "E");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "E");
						Legion::EndChild();
						Legion::IndentEx(23, -108);
						Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "BattleField Free", ImVec2(575, 120), false, LegionWindowFlags_HorizontalScrollbar, false);
						for (std::pair<unsigned int, BattleFieldSystemStruct*> it : BattleField)
						{
							if (Legion::BattleField(("Item" + std::to_string(it.first)).c_str(), CalibriFont, 12, ImageTexture, it.second->RewardImage, ClaimMarkTexture, it.second->Status, &BattleFieldId, it.first, it.second->IsButtonActive, ImVec2(99, 106)))
							{
								if (it.first > Mem.StdToInt(BattleFiedlsLevel))
									return;
								SendInfoToServer(std::to_string(it.first), "BF", "a484640baf");
								//	it.second->Status = true;
							}
							Legion::SameLine(float(94 * it.first));
						}
						Legion::EndChild();
						Legion::EndChild();

						Legion::BeginChildV(ChatBoxTexture, NULL, NULL, "Adv BattleField", ImVec2(620, 122));
						Legion::IndentEx(15, 3);
						Legion::BeginChildV(NULL, NULL, NULL, "Adv TEXT", ImVec2(25, 112));
						Legion::TextLabelColor(CalibriFont, 15.5f, "A");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "D");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "V");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "A");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "N");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "C");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "E");
						Legion::IndentEx(0, -5);
						Legion::TextLabelColor(CalibriFont, 15.5f, "D");
						Legion::EndChild();
						Legion::IndentEx(23, -117);
						Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "BattleField ADv", ImVec2(575, 120), false, LegionWindowFlags_HorizontalScrollbar, false);
						for (std::pair<unsigned int, BattleFieldSystemStruct*> it : AdvBattleField)
						{
							if (Legion::BattleField(("Adv Item" + std::to_string(it.first)).c_str(), CalibriFont, 12, ImageTexture, it.second->RewardImage, ClaimMarkTexture, it.second->Status, &AdvBattleFieldId, it.first, it.second->IsButtonActive, ImVec2(99, 106)))
							{
								if (it.first > Mem.StdToInt(BattleFiedlsLevel))
									return;
								SendInfoToServer(std::to_string(it.first), "ABF", "a77d0a8def");
								//it.second->Status = true;
							}

							Legion::SameLine(float(94 * it.first));
						}
						Legion::EndChild();
						Legion::EndChild();
					}

					if (BattleFieldMenu == 1)
					{
						Legion::IndentEx(-5, 5);
						Legion::ImageButtonEx("LEvel", ("LEVEL " + BattleFiedlsLevel).c_str(), true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(110, 35), CalibriFont, 12.4f);
						Legion::SameLine(154);
						if (Legion::ImageButtonV("Stage 1", "Stage 1", true, &BattleFieldStageMenu, 1, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(118, 35), CalibriFont, 12.4f))
						{
							SendInfoToServer(std::to_string(BattleFieldStageMenu), "STG", "e14fcf8f1d");
						}
						Legion::SameLine(281);
						if (Legion::ImageButtonV("Stage 2", "Stage 2", true, &BattleFieldStageMenu, 2, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(118, 35), CalibriFont, 12.4f))
						{
							SendInfoToServer(std::to_string(BattleFieldStageMenu), "STG", "e14fcf8f1d");
						}
						Legion::SameLine(408);
						if (Legion::ImageButtonV("Stage 3", "Stage 3", true, &BattleFieldStageMenu, 3, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(118, 35), CalibriFont, 12.4f))
						{
							SendInfoToServer(std::to_string(BattleFieldStageMenu), "STG", "e14fcf8f1d");
						}
						Legion::SameLine(535);
						if (Legion::ImageButtonV("Stage 4", "Stage 4", true, &BattleFieldStageMenu, 4, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(74, 170, 76), ImVec2(118, 35), CalibriFont, 12.4f))
						{
							SendInfoToServer(std::to_string(BattleFieldStageMenu), "STG", "e14fcf8f1d");
						}

						Legion::IndentEx(5, 0);

						Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "Task BattleField", ImVec2(630, 210));
						for (std::pair<unsigned int, BattleFieldTaskSystemStruct*> it : BattleFieldTask)
						{
							if (Legion::BattleFieldTask(("Task List" + std::to_string(it.first)).c_str(), it.second->Task, it.second->Points.c_str(), it.second->PercentageInfo.c_str(), CalibriFont, 14, ChatBoxTexture, ChatBoxTexture, EvTextture, ClaimMarkTexture, it.second->Status, &BattleFieldTaskId, it.first, ImVec2(620, 70)))
							{
								SendInfoToServer(std::to_string(it.first), "ADT", "dfe5774aff");
							}
						}
						Legion::EndChild();
					}
				}

				if (MenuSelected == 3)
				{
					MainStatus = "Achivements";
					Legion::BeginChildV(NULL, NULL, NULL, "Arch Rank", ImVec2(640, 60));
					Legion::LabelEx("RANK", "20", ImVec2(50, 10), CalibriFont, 12);
					Legion::IndentEx(0, -8);
					Legion::LabelEx("RANK Type", "RANK", ImVec2(50, 10), CalibriFont, 12);
					Legion::IndentEx(250, -30);
					Legion::LabelEx("ToNextLevel", "TO NEXT LEVEL", ImVec2(85, 10), CalibriFont, 12);
					Legion::IndentEx(10, -5);
					Legion::ProgressBarEx(CalibriFont, 12, "Arch Next Level Progress", 0, 0, 100, 1, ImVec2(75, 13), ImColor(249, 249, 249), ImColor(74, 170, 76), false, false);
					Legion::IndentEx(0, -20);
					Legion::LabelEx("Progress Val", "{707070}100/500", ImVec2(75, 10), CalibriFont, 12);
					Legion::IndentEx(185, -35);
					Legion::ImageExV("Reward 1", ImageTexture, ImageTexture, ImVec2(50, 50));
					Legion::SameLine();
					Legion::ImageExV("Reward 2", ImageTexture, ImageTexture, ImVec2(50, 50));
					Legion::IndentEx(110, -45);
					if (Legion::ImageButtonEx("ACrchClaIM BTN", "CLAIM", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(80, 25), CalibriFont, 12.4f))
					{

					}
					Legion::EndChild();

					Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "Achivements List", ImVec2(660, 300));
					int testx = 0;
					Legion::Achivements("Test", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 50, false, &testx, 1);
					Legion::SameLine();
					Legion::Achivements("Test", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 70, false, &testx, 7);
					Legion::Achivements("Test1", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 99, false, &testx, 2);
					Legion::Achivements("Test2", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 1, false, &testx, 3);
					Legion::Achivements("Test3", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 30, false, &testx, 4);
					Legion::Achivements("Test4", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 80, false, &testx, 5);
					Legion::Achivements("Test5", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 40, false, &testx, 6);
					Legion::EndChild();
					//	
					//	Legion::Achivements("Test", CalibriFont, 12.5f, BBoxTexture, ButtonTexture, ClaimMarkTexture, 50, false, &testx, 1);
				}

				//if (MenuSelected == 4)
				//{
				//	MainStatus = "Room Option";
				//	MainMenuButtonName = "SAVE";
				//	if (plr_loc == Player_Loc::Room)
				//	{
				//		Legion::IndentEx(10, 5);
				//		Legion::CheckboxEx("show team hp", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} SHOW TEAM HP", &ShowTeamHP, CRoomOptionClient);
				//		Legion::CheckboxEx("NO RESPAWN", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO RESPAWN", &Respawn, CRoomOptionClient);
				//		Legion::CheckboxEx("MACRO", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO MACRO", &NoMacro, CRoomOptionClient);
				//		Legion::CheckboxEx("CLASS SYSTE", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO CLASS SYSTEM", &NoClassSystem, CRoomOptionClient);
				//		Legion::CheckboxEx("ENCHANTS", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO ENCHANTS", &NoEnchant, CRoomOptionClient);
				//		Legion::CheckboxEx("INSIDE STATES", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} INSIDE STATES", &InsideStates, CRoomOptionClient);
				//		Legion::CheckboxEx("REBIRTH", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} NO REBIRTH", &NoRebirth, CRoomOptionClient);
				//		Legion::CheckboxEx("AUTO SIT", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} AUTO SIT", &AutoSit, CRoomOptionClient);
				//		Legion::CheckboxEx("SWITCH PROFILE", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ALLOW SWITCH PROFILE", &ChangeProfile, CRoomOptionClient);
				//		Legion::CheckboxEx("ACTIVE FILTER", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ACTIVE FILTER", &ActiveFilter, CRoomOptionClient);
				//		Legion::CheckboxEx("RANDOM SWAP FILTER", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} RANDOM SWAP FILTER", &RandomSwapFilter, CRoomOptionClient);
				//		if (Legion::ImageButtonEx("FILTER", "FILTER", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(145, 25), CalibriFont, 12.4f))
				//		{
				//			RoomMapFilter = true;
				//		}
				//		Legion::IndentEx(250, -275);
				//		Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.HP", &HP, 10, 162, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
				//		Legion::IndentEx(0, 5);
				//		Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.SP", &SP, 10, 162, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
				//		Legion::IndentEx(0, 5);
				//		Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.HEAL", &Heal, 0, 100, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
				//		Legion::IndentEx(0, 5);
				//		Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MAX.PING", &Ping, 20, 1000, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 107, CRoomOptionClient);
				//		Legion::IndentEx(-10, 10);
				//		Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} BLOCKED.");
				//		Legion::BeginChildV(RBoxTexture, NULL, NULL, "Blocked", ImVec2(153, 178));
				//		{
				//			Legion::IndentEx(3, 7);
				//			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Blocked G", ImVec2(148, 161));
				//			if (!CRoomOptionClient)
				//			{
				//				if (!AllowPS && !AllowPerTeamPS)
				//					Legion::CheckboxEx("CHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &BlockPS);
				//				if (!AllowCS && !AllowPerTeamCS)
				//					Legion::CheckboxEx("CHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &BlockCS);
				//				if (!AllowDG && !AllowPerTeamDG)
				//					Legion::CheckboxEx("CHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &BlockDG);
				//				if (!AllowBAT && !AllowPerTeamBAT)
				//					Legion::CheckboxEx("CHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &BlockBAT);
				//				if (!AllowBR && !AllowPerTeamBR)
				//					Legion::CheckboxEx("CHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &BlockBR);
				//				if (!AllowTB && !AllowPerTeamTB)
				//					Legion::CheckboxEx("CHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &BlockTB);
				//				if (!AllowIB && !AllowPerTeamIB)
				//					Legion::CheckboxEx("CHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &BlockIB);
				//				if (!AllowFist && !AllowPerTeamFist)
				//					Legion::CheckboxEx("CHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &BlockFist);
				//				if (!AllowKatan && !AllowPerTeamKatan)
				//					Legion::CheckboxEx("CHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &BlockKatan);
				//				if (!AllowEXO && !AllowPerTeamEXO)
				//					Legion::CheckboxEx("CHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &BlockEXO);
				//				if (!AllowSigmaBlade && !AllowPerTeamSigmaBlade)
				//					Legion::CheckboxEx("CHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &BlockSigmaBlade);
				//				if (!AllowVitalShock && !AllowPerTeamVitalShock)
				//					Legion::CheckboxEx("CHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &BlockVitalShock);//Melee End
				//				if (!AllowRevolver && !AllowPerTeamRevolver)
				//					Legion::CheckboxEx("CHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &BlockRevolver);
				//				if (!AllowHeavyMachineGun && !AllowPerTeamHeavyMachineGun)
				//					Legion::CheckboxEx("CHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &BlockHeavyMachineGun);
				//				if (!AllowLightMachineGun && !AllowPerTeamLightMachineGun)
				//					Legion::CheckboxEx("CHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &BlockLightMachineGun);
				//				if (!AllowTurret && !AllowPerTeamTurret)
				//					Legion::CheckboxEx("CHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &BlockTurret);
				//				if (!AllowHealWeapons && !AllowPerTeamHealWeapons)
				//					Legion::CheckboxEx("CHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &BlockHealWeapons);
				//				if (!AllowShockWeapons && !AllowPerTeamShockWeapons)
				//					Legion::CheckboxEx("CHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &BlockShockWeapons);
				//				if (!AllowSentry && !AllowPerTeamSentry)
				//					Legion::CheckboxEx("CHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &BlockSentry);
				//				if (!AllowSenty && !AllowPerTeamSenty)
				//					Legion::CheckboxEx("CHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &BlockSenty);
				//				if (!AllowMineGun && !AllowPerTeamMineGun)
				//					Legion::CheckboxEx("CHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &BlockMineGun);
				//				if (!AllowEarthbomber && !AllowPerTeamEarthbomber)
				//					Legion::CheckboxEx("CHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &BlockEarthbomber);
				//				if (!AllowLightningBomber && !AllowPerTeamLightningBomber)
				//					Legion::CheckboxEx("CHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &BlockLightningBomber);
				//				if (!AllowRocketLauncher && !AllowPerTeamRocketLauncher)
				//					Legion::CheckboxEx("CHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &BlockRocketLauncher);
				//				if (!AllowCannonade && !AllowPerTeamRailGun)
				//					Legion::CheckboxEx("CHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &BlockCannonade);
				//				if (!AllowRailGun && !AllowPerTeamRailGun)
				//					Legion::CheckboxEx("CHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &BlockRailGun);
				//				if (!AllowSniper && !AllowPerTeamSniper)
				//					Legion::CheckboxEx("CHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &BlockSniper);
				//				if (!AllowAirGun && !AllowPerTeamAirGun)
				//					Legion::CheckboxEx("CHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &BlockAirGun);
				//				if (!AllowDualMagnum && !AllowPerTeamDualMagnum)
				//					Legion::CheckboxEx("CHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &BlockDualMagnum);
				//				if (!AllowBurstShotgun && !AllowPerTeamBurstShotgun)
				//					Legion::CheckboxEx("CHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &BlockBurstShotgun);
				//				if (!AllowSemiRifle && !AllowPerTeamSemiRifle)
				//					Legion::CheckboxEx("CHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &BlockSemiRifle);
				//				if (!AllowSmashRifle && !AllowPerTeamSmashRifle)
				//					Legion::CheckboxEx("CHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &BlockSmashRifle);
				//				if (!AllowAssaultRifle && !AllowPerTeamAssaultRifle)
				//					Legion::CheckboxEx("CHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &BlockAssaultRifle);
				//				if (!AllowBubblRifle && !AllowPerTeamBubblRifle)
				//					Legion::CheckboxEx("CHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &BlockBubblRifle);
				//				if (!AllowGaussRifle && !AllowPerTeamGaussRifle)
				//					Legion::CheckboxEx("CHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &BlockGaussRifle);
				//				if (!AllowHandGun && !AllowPerTeamHandGun)
				//					Legion::CheckboxEx("CHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &BlockHandGun);
				//				if (!AllowSubmachineGun && !AllowPerTeamSubmachineGun)
				//					Legion::CheckboxEx("CHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &BlockSubmachineGun);

				//				//Skill
				//				if (!AllowHalfHPMastery && !AllowPerTeamHPMastery)
				//					Legion::CheckboxEx("CHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &BlockHalfHPMastery);
				//				if (!AllowHPMastery && !AllowPerTeamHPMastery)
				//					Legion::CheckboxEx("CHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &BlockHPMastery);
				//				if (!AllowSkillMastery && !AllowPerTeamSkillMastery)
				//					Legion::CheckboxEx("CHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &BlockSkillMastery);
				//				if (!AllowDualMastery && !AllowPerTeamDualMastery)
				//					Legion::CheckboxEx("CHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &BlockDualMastery);
				//				if (!AllowMetallic && !AllowPerTeamMetallic)
				//					Legion::CheckboxEx("CHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &BlockMetallic);
				//				if (!AllowUniqueDualMastery && !AllowPerTeamDualMastery)
				//					Legion::CheckboxEx("CHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &BlockUniqueDualMastery);
				//				if (!AllowAnchoring && !AllowPerTeamAnchoring)
				//					Legion::CheckboxEx("CHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &BlockAnchoring);
				//				if (!AllowFlying && !AllowPerTeamFlying)
				//					Legion::CheckboxEx("CHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &BlockFlying);
				//				if (!AllowInvisible && !AllowPerTeamInvisible)
				//					Legion::CheckboxEx("CHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &BlockInvisible);
				//				if (!AllowDetect && !AllowPerTeamDetect)
				//					Legion::CheckboxEx("CHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &BlockDetect);
				//				if (!AllowShield && !AllowPerTeamShield)
				//					Legion::CheckboxEx("CHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &BlockShield);
				//				if (!AllowBlock && !AllowPerTeamBlock)
				//					Legion::CheckboxEx("CHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Block", &BlockBlock);
				//			}
				//			else
				//			{
				//				Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + Blockroption).c_str());
				//			}
				//			Legion::EndChild();
				//		}Legion::EndChild();
				//		Legion::IndentEx(153, -198);
				//		Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ALLOWED.");
				//		Legion::BeginChildV(RBoxTexture, NULL, NULL, "Allowed", ImVec2(153, 178));
				//		{
				//			Legion::IndentEx(3, 7);
				//			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Allowed G", ImVec2(148, 160));
				//			if (!CRoomOptionClient)
				//			{
				//				if (!BlockPS)
				//					Legion::CheckboxEx("ACHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &AllowPS);
				//				if (!BlockCS)
				//					Legion::CheckboxEx("ACHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &AllowCS);
				//				if (!BlockDG)
				//					Legion::CheckboxEx("ACHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &AllowDG);
				//				if (!BlockBAT)
				//					Legion::CheckboxEx("ACHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &AllowBAT);
				//				if (!BlockBR)
				//					Legion::CheckboxEx("ACHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &AllowBR);
				//				if (!BlockTB)
				//					Legion::CheckboxEx("ACHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &AllowTB);
				//				if (!BlockIB)
				//					Legion::CheckboxEx("ACHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &AllowIB);
				//				if (!BlockFist)
				//					Legion::CheckboxEx("ACHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &AllowFist);
				//				if (!BlockKatan)
				//					Legion::CheckboxEx("ACHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &AllowKatan);
				//				if (!BlockEXO)
				//					Legion::CheckboxEx("ACHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &AllowEXO);
				//				if (!BlockSigmaBlade)
				//					Legion::CheckboxEx("ACHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &AllowSigmaBlade);
				//				if (!BlockVitalShock)
				//					Legion::CheckboxEx("ACHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &AllowVitalShock);//Melee End
				//				if (!BlockRevolver)
				//					Legion::CheckboxEx("ACHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &AllowRevolver);
				//				if (!BlockHeavyMachineGun)
				//					Legion::CheckboxEx("ACHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &AllowHeavyMachineGun);
				//				if (!BlockLightMachineGun)
				//					Legion::CheckboxEx("ACHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &AllowLightMachineGun);
				//				if (!BlockTurret)
				//					Legion::CheckboxEx("ACHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &AllowTurret);
				//				if (!BlockHealWeapons)
				//					Legion::CheckboxEx("ACHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &AllowHealWeapons);
				//				if (!BlockShockWeapons)
				//					Legion::CheckboxEx("ACHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &AllowShockWeapons);
				//				if (!BlockSentry)
				//					Legion::CheckboxEx("ACHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &AllowSentry);
				//				if (!BlockSenty)
				//					Legion::CheckboxEx("ACHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &AllowSenty);
				//				if (!BlockMineGun)
				//					Legion::CheckboxEx("ACHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &AllowMineGun);
				//				if (!BlockEarthbomber)
				//					Legion::CheckboxEx("ACHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &AllowEarthbomber);
				//				if (!BlockLightningBomber)
				//					Legion::CheckboxEx("ACHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &AllowLightningBomber);
				//				if (!BlockRocketLauncher)
				//					Legion::CheckboxEx("ACHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &AllowRocketLauncher);
				//				if (!BlockCannonade)
				//					Legion::CheckboxEx("ACHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &AllowCannonade);
				//				if (!BlockRailGun)
				//					Legion::CheckboxEx("ACHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &AllowRailGun);
				//				if (!BlockSniper)
				//					Legion::CheckboxEx("ACHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &AllowSniper);
				//				if (!BlockAirGun)
				//					Legion::CheckboxEx("ACHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &AllowAirGun);
				//				if (!BlockDualMagnum)
				//					Legion::CheckboxEx("ACHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &AllowDualMagnum);
				//				if (!BlockBurstShotgun)
				//					Legion::CheckboxEx("ACHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &AllowBurstShotgun);
				//				if (!BlockSemiRifle)
				//					Legion::CheckboxEx("ACHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &AllowSemiRifle);
				//				if (!BlockSmashRifle)
				//					Legion::CheckboxEx("ACHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &AllowSmashRifle);
				//				if (!BlockAssaultRifle)
				//					Legion::CheckboxEx("ACHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &AllowAssaultRifle);
				//				if (!BlockBubblRifle)
				//					Legion::CheckboxEx("ACHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &AllowBubblRifle);
				//				if (!BlockGaussRifle)
				//					Legion::CheckboxEx("ACHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &AllowGaussRifle);
				//				if (!BlockHandGun)
				//					Legion::CheckboxEx("ACHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &AllowHandGun);
				//				if (!BlockSubmachineGun)
				//					Legion::CheckboxEx("ACHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &AllowSubmachineGun);

				//				//Skill
				//				if (!BlockHalfHPMastery)
				//					Legion::CheckboxEx("ACHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &AllowHalfHPMastery);
				//				if (!BlockHPMastery)
				//					Legion::CheckboxEx("ACHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &AllowHPMastery);
				//				if (!BlockSkillMastery)
				//					Legion::CheckboxEx("ACHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &AllowSkillMastery);
				//				if (!BlockDualMastery)
				//					Legion::CheckboxEx("ACHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &AllowDualMastery);
				//				if (!BlockMetallic)
				//					Legion::CheckboxEx("ACHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &AllowMetallic);
				//				if (!BlockUniqueDualMastery)
				//					Legion::CheckboxEx("ACHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &AllowUniqueDualMastery);
				//				if (!BlockAnchoring)
				//					Legion::CheckboxEx("ACHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &AllowAnchoring);
				//				if (!BlockFlying)
				//					Legion::CheckboxEx("ACHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &AllowFlying);
				//				if (!BlockInvisible)
				//					Legion::CheckboxEx("ACHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &AllowInvisible);
				//				if (!BlockDetect)
				//					Legion::CheckboxEx("ACHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &AllowDetect);
				//				if (!BlockShield)
				//					Legion::CheckboxEx("ACHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &AllowShield);
				//				if (!BlockBlock)
				//					Legion::CheckboxEx("ACHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Allow", &AllowBlock);
				//			}
				//			else
				//			{
				//				Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + Allowroption).c_str());
				//			}
				//			Legion::EndChild();
				//		}Legion::EndChild();
				//		Legion::IndentEx(153, -197);
				//		Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ALLOWED PER TEAM.");
				//		Legion::BeginChildV(RBoxTexture, NULL, NULL, "Allowed Per Team", ImVec2(153, 178));
				//		{
				//			Legion::IndentEx(3, 7);
				//			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Allowed Per Team G", ImVec2(148, 160));
				//			if (!CRoomOptionClient)
				//			{
				//				if (!BlockPS)
				//					Legion::CheckboxEx("APTCHK", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Plasma Sword", &AllowPerTeamPS);
				//				if (!BlockCS)
				//					Legion::CheckboxEx("APTCHK1", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Counter Sword", &AllowPerTeamCS);
				//				if (!BlockDG)
				//					Legion::CheckboxEx("APTCHK2", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dagger", &AllowPerTeamDG);
				//				if (!BlockBAT)
				//					Legion::CheckboxEx("APTCHK3", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bat", &AllowPerTeamBAT);
				//				if (!BlockBR)
				//					Legion::CheckboxEx("APTCHK4", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Breaker", &AllowPerTeamBR);
				//				if (!BlockTB)
				//					Legion::CheckboxEx("APTCHK5", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Twin Blade", &AllowPerTeamTB);
				//				if (!BlockIB)
				//					Legion::CheckboxEx("APTCHK6", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Iron Boots", &AllowPerTeamIB);
				//				if (!BlockFist)
				//					Legion::CheckboxEx("APTCHK7", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Fist", &AllowPerTeamFist);
				//				if (!BlockKatan)
				//					Legion::CheckboxEx("APTCHK8", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Katan", &AllowPerTeamKatan);
				//				if (!BlockEXO)
				//					Legion::CheckboxEx("APTCHK9", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Exo", &AllowPerTeamEXO);
				//				if (!BlockSigmaBlade)
				//					Legion::CheckboxEx("APTCHK10", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sigma Blade", &AllowPerTeamSigmaBlade);
				//				if (!BlockVitalShock)
				//					Legion::CheckboxEx("APTCHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Vital Shock", &AllowPerTeamVitalShock);//Melee End
				//				if (!BlockRevolver)
				//					Legion::CheckboxEx("APTCHK12", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Revolver", &AllowPerTeamRevolver);
				//				if (!BlockHeavyMachineGun)
				//					Legion::CheckboxEx("APTCHK13", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heavy Machine Gun", &AllowPerTeamHeavyMachineGun);
				//				if (!BlockLightMachineGun)
				//					Legion::CheckboxEx("APTCHK14", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Light Machine Gun", &AllowPerTeamLightMachineGun);
				//				if (!BlockTurret)
				//					Legion::CheckboxEx("APTCHK15", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Turret", &AllowPerTeamTurret);
				//				if (!BlockHealWeapons)
				//					Legion::CheckboxEx("APTCHK16", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Heal Weapons", &AllowPerTeamHealWeapons);
				//				if (!BlockShockWeapons)
				//					Legion::CheckboxEx("APTCHK17", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shock Weapons", &AllowPerTeamShockWeapons);
				//				if (!BlockSentry)
				//					Legion::CheckboxEx("APTCHK18", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sentry", &AllowPerTeamSentry);
				//				if (!BlockSenty)
				//					Legion::CheckboxEx("APTCHK19", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Senty", &AllowPerTeamSenty);
				//				if (!BlockMineGun)
				//					Legion::CheckboxEx("APTCHK20", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Mine Gun", &AllowPerTeamMineGun);
				//				if (!BlockEarthbomber)
				//					Legion::CheckboxEx("APTCHK21", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Earth Bomber", &AllowPerTeamEarthbomber);
				//				if (!BlockLightningBomber)
				//					Legion::CheckboxEx("APTCHK22", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Lightning Bomber", &AllowPerTeamLightningBomber);
				//				if (!BlockRocketLauncher)
				//					Legion::CheckboxEx("APTCHK23", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rocket Launcher", &AllowPerTeamRocketLauncher);
				//				if (!BlockCannonade)
				//					Legion::CheckboxEx("APTCHK24", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Cannonade", &AllowPerTeamCannonade);
				//				if (!BlockRailGun)
				//					Legion::CheckboxEx("APTCHK25", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Rail Gun", &AllowPerTeamRailGun);
				//				if (!BlockSniper)
				//					Legion::CheckboxEx("APTCHK26", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Sniper", &AllowPerTeamSniper);
				//				if (!BlockAirGun)
				//					Legion::CheckboxEx("APTCHK27", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Air Gun", &AllowPerTeamAirGun);
				//				if (!BlockDualMagnum)
				//					Legion::CheckboxEx("APTCHK28", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Magnum", &AllowPerTeamDualMagnum);
				//				if (!BlockBurstShotgun)
				//					Legion::CheckboxEx("APTCHK29", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shotgun", &AllowPerTeamBurstShotgun);
				//				if (!BlockSemiRifle)
				//					Legion::CheckboxEx("APTCHK30", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Semi Rifle", &AllowPerTeamSemiRifle);
				//				if (!BlockSmashRifle)
				//					Legion::CheckboxEx("APTCHK31", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Smash Rifle", &AllowPerTeamSmashRifle);
				//				if (!BlockAssaultRifle)
				//					Legion::CheckboxEx("APTCHK32", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Assault Rifle", &AllowPerTeamAssaultRifle);
				//				if (!BlockBubblRifle)
				//					Legion::CheckboxEx("APTCHK33", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Bubbl Rifle", &AllowPerTeamBubblRifle);
				//				if (!BlockGaussRifle)
				//					Legion::CheckboxEx("APTCHK34", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Gauss Rifle", &AllowPerTeamGaussRifle);
				//				if (!BlockHandGun)
				//					Legion::CheckboxEx("APTCHK35", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Hand Gun", &AllowPerTeamHandGun);
				//				if (!BlockSubmachineGun)
				//					Legion::CheckboxEx("APTCHK36", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Submachine Gun", &AllowPerTeamSubmachineGun);

				//				//Skill
				//				if (!BlockHalfHPMastery)
				//					Legion::CheckboxEx("APTCHK37", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Half HP Mastery", &AllowPerTeamHalfHPMastery);
				//				if (!BlockHPMastery)
				//					Legion::CheckboxEx("APTCHK38", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} HP Mastery", &AllowPerTeamHPMastery);
				//				if (!BlockSkillMastery)
				//					Legion::CheckboxEx("APTCHK39", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Skill Mastery", &AllowPerTeamSkillMastery);
				//				if (!BlockDualMastery)
				//					Legion::CheckboxEx("APTCHK40", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Dual Mastery", &AllowPerTeamDualMastery);
				//				if (!BlockMetallic)
				//					Legion::CheckboxEx("APTCHK41", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Metallic", &AllowPerTeamMetallic);
				//				if (!BlockUniqueDualMastery)
				//					Legion::CheckboxEx("APTCHK42", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Unique Dual Mastery", &AllowPerTeamUniqueDualMastery);
				//				if (!BlockAnchoring)
				//					Legion::CheckboxEx("APTCHK43", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Anchoring", &AllowPerTeamAnchoring);
				//				if (!BlockFlying)
				//					Legion::CheckboxEx("APTCHK44", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Flying", &AllowPerTeamFlying);
				//				if (!BlockInvisible)
				//					Legion::CheckboxEx("APTCHK45", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Invisible", &AllowPerTeamInvisible);
				//				if (!BlockDetect)
				//					Legion::CheckboxEx("APTCHK46", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Detect", &AllowPerTeamDetect);
				//				if (!BlockShield)
				//					Legion::CheckboxEx("APTCHK47", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Shield", &AllowPerTeamShield);
				//				if (!BlockBlock)
				//					Legion::CheckboxEx("APTCHK48", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} Block", &AllowPerTeamBlock);
				//			}
				//			else
				//			{
				//				Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} " + AllowPerTeamroption).c_str());
				//			}
				//			Legion::EndChild();
				//		}Legion::EndChild();
				//	}
				//}

				if (MenuSelected == 5)
				{
					MainStatus = "Events";

					if (!QuestSystemUI)
					{
						Legion::IndentEx(20, 10);
						if (Legion::ImageButtonEx("Auto Event", "REGISTER AUTO EVENT", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(130, 25), CalibriFont, 12.4f))
							SendInfoToServer("AUE", "AUR", "660f7d1bf2");
						if (Legion::ImageButtonEx("QUESTUI", "QUEST", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(130, 25), CalibriFont, 12.4f))
							QuestSystemUI = true;
					}
					if (QuestSystemUI)
					{
						MainMenuButtonName = "REDEEM";

						Legion::IndentEx(650, 5);
						if (Legion::ImageButtonEx("Event Return", "", false, LeftArrowTexture, LeftArrowTexture, LeftArrowTexture, ImVec2(20, 20), CalibriFont, 14, ImVec2(1.4f, 0.5f)))
							QuestSystemUI = false;

						Legion::IndentEx(-640, -25);
						Legion::TextLabelColor(CalibriFont, 15, "{c1ccdd} QUEST SYSTEM");
						Legion::IndentEx(0, -5);
						Legion::BeginChildV(BBoxTexture, NULL, NULL, "Quest List", ImVec2(198, 290));
						Legion::IndentEx(5, 5);
						for (std::pair<unsigned int, QuestSystemStruct*> it : QuestSystemInfo)
						{
							if (Legion::ImageButtonVC(("Test " + std::to_string(it.first)).c_str(), it.second->Name, "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &QuestButtonMenu, it.first, NULL, NULL, NULL, ImVec2(215, 10), CalibriFont, 12, ImVec2(0, 0)))
							{
								QuestId = it.first;
								QuestInfo = it.second->Info;
								QuestPercentage = it.second->Percentage;
								QuestPercentage2 = it.second->Percentage2;
								QuestPercentage3 = it.second->Percentage3;
								QuestReward = it.second->RewardImage;
								Quest2Reward = it.second->Reward2Image;
							}
						}
						Legion::EndChild();
						Legion::SameLine(205);
						Legion::BeginChildV(BBoxTexture, NULL, NULL, "Quest Info", ImVec2(462, 290));
						{
							Legion::IndentEx(10, 5);
							Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Quest Infos", ImVec2(442, 230));
							Legion::IndentEx(5, 5);
							Legion::TextColorChatBox(CalibriFont, 12, QuestInfo.c_str());
							Legion::EndChild();
							Legion::IndentEx(5, 5);
							Legion::TextLabelColor(CalibriFont, 12, ("{c1ccdd} Progress " + QuestPercentage + " - " + QuestPercentage2 + " - " + QuestPercentage3).c_str());
							Legion::IndentEx(300, -35);
							Legion::ImageExV("Reward", ImageTexture, QuestReward, ImVec2(54, 54));
							Legion::IndentEx(58, -58);
							Legion::ImageExV("Reward2", ImageTexture, Quest2Reward, ImVec2(54, 54));
						}
						Legion::EndChild();
						Legion::IndentEx(150, 13);
						Legion::TextLabelColor(CalibriFont, 15, "{c1ccdd} WEEKLY REWARD");
						Legion::IndentEx(120, -17);
						Legion::ProgressBarEx(CalibriFont, 12, "Weekly Quest", CurrentQuestPercentageWeekly, 0, MaxQuestPercentageWeekly, 1, ImVec2(320, 10), ImColor(249, 249, 249), ImColor(74, 170, 76), false, false);
						Legion::IndentEx(327, -37);
						Legion::ImageExV("Reward3", ImageTexture, Quest3Reward, ImVec2(54, 54));
					}
				}

				if (MenuSelected == 6)
				{
					MainStatus = "Options";
					MainMenuButtonName = "SAVE";

					Legion::IndentEx(10, 2);
					Legion::CheckboxEx("AUTO BOOST", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} AUTO BOOST", &AutoBoost);
					Legion::CheckboxEx("STABLE FPS", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} STABLE FPS", &StableFPS);
					Legion::CheckboxEx("IMPROVE Client", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} IMPROVE Client", &ImproveClient);
					Legion::CheckboxEx("RPC ACTIVATION", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} RPC ACTIVATION", &RPCActvion);
					Legion::CheckboxEx("ACTIVE VOICE ACTION", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ACTIVE VOICE ACTION", &ActiveVAction);
					Legion::CheckboxEx("ACTIVE CHAR CAM", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 12, "{c1ccdd} ACTIVE CHAR CAM", &ActiveCharCam);
					Legion::IndentEx(200, -125);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} FPS Mode", &InputLag, 0, 5, "%1.0f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 260);
					Legion::SameLine(460);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} Char Cam", &CharCam, -24, 24, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 260);// * 10
					Legion::IndentEx(0, -4);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} FOV", &Fov, 50, 100, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 260);
					Legion::SameLine(460);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} Aim Position", &AimPos, 20, 50, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 260);// * 10
					Legion::IndentEx(0, -4);
					Legion::SliderFloatEx(CalibriFont, 12, "{c1ccdd} MiniMap Size", &MapSize, 10, 50, "%1.f", 1, ImVec2(1, 1), ImColor(34, 123, 51, 1), true, 10);// * 10
					Legion::IndentEx(0, -14);
					Legion::CheckboxEx("CUSTOM ACTION", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 11, "{c1ccdd} CUSTOM ACTION", &ActiveCVAction);
					Legion::BeginChildV(NormalBoxTexture, NULL, NULL, "Custom Actions List", ImVec2(350, 255), false, LegionWindowFlags_Modal);
					{
						Legion::IndentEx(5, 5);
						Legion::InputTextWithHintV("Action 1", CalibriFont, 11, NULL, "", "Action 1", Action, IM_ARRAYSIZE(Action), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 2", CalibriFont, 11, NULL, "", "Action 2", Action2, IM_ARRAYSIZE(Action2), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 3", CalibriFont, 11, NULL, "", "Action 3", Action3, IM_ARRAYSIZE(Action3), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 4", CalibriFont, 11, NULL, "", "Action 4", Action4, IM_ARRAYSIZE(Action4), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 5", CalibriFont, 11, NULL, "", "Action 5", Action5, IM_ARRAYSIZE(Action5), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 6", CalibriFont, 11, NULL, "", "Action 6", Action6, IM_ARRAYSIZE(Action6), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 7", CalibriFont, 11, NULL, "", "Action 7", Action7, IM_ARRAYSIZE(Action7), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 8", CalibriFont, 11, NULL, "", "Action 8", Action8, IM_ARRAYSIZE(Action8), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 9", CalibriFont, 11, NULL, "", "Action 9", Action9, IM_ARRAYSIZE(Action9), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 10", CalibriFont, 11, NULL, "", "Action 10", Action10, IM_ARRAYSIZE(Action10), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 11", CalibriFont, 11, NULL, "", "Action 11", Action11, IM_ARRAYSIZE(Action11), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
						Legion::InputTextWithHintV("Action 12", CalibriFont, 11, NULL, "", "Action 12", Action12, IM_ARRAYSIZE(Action12), ImVec2(292, 16), LegionInputTextFlags_CharsHexadecimal);
					}
					Legion::EndChild();
				}

				if (MenuSelected == 7)
				{
					MainStatus = "Exchange";

					if (IsExchanged)
					{
						LegionInputTextFlags P1, P2;
						if (ExchangeBlockPlayer || IsExcLocked)
							P1 = LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal | LegionInputTextFlags_ReadOnly;
						else
							P1 = LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal;
						if (ExchangeBlockPlayer2 || IsExcLocked2)
							P2 = LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal | LegionInputTextFlags_ReadOnly;
						else
							P2 = LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal;

						Legion::IndentEx(50, 35);
						Legion::ImageV("Player Level Img", CPlayerListTexture(P1Level, 0), ImVec2(55, 55));
						Legion::IndentEx(55, -58);
						Legion::TextLabelColor(CalibriFont, 18, ("{c1ccdd}" + ExchangePlayer1NickName).c_str());
						Legion::TextLabelColor(CalibriFont, 14, ("{c1ccdd} Level. " + P1Level).c_str());
						Legion::IndentEx(-55, 35);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} PEN.AMOUNT");
						Legion::InputTextV("Player PEN", CalibriFont, 12, SmallBoxTexture, "", P1PEN, IM_ARRAYSIZE(P1PEN), ImVec2(172, 17), P1);
						Legion::IndentEx(0, 10);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} EV.AMOUNT");
						Legion::InputTextV("Player EV", CalibriFont, 12, SmallBoxTexture, "", P1LP, IM_ARRAYSIZE(P1LP), ImVec2(172, 17), P1);
						Legion::IndentEx(0, 15);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ADD OBJECT");
						Legion::IndentEx(50, 15);

						auto P1Item = S4_ItemList.find(1);
						auto P1Item2 = S4_ItemList.find(2);
						auto P1Item3 = S4_ItemList.find(3);

						if (Legion::ImageButtonEx("Item1 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer && !IsExcLocked)
							{
								if (P1Item != S4_ItemList.end())
								{
									ExchangeP1Texture = NULL;
									ExchangeItemInfo = "";

									SendInfoToServer(ExchangePlayer2NickName, "aa22c8fc24", P1Item->second->unique_id);
									S4_ItemList.erase(1);
								}
							}
						}
						Legion::IndentEx(80, -22);
						if (Legion::ImageButtonEx("Item2 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer && !IsExcLocked)
							{
								if (P1Item2 != S4_ItemList.end())
								{
									ExchangeP1Texture2 = NULL;
									ExchangeItemInfo2 = "";

									SendInfoToServer(ExchangePlayer2NickName, "aa22c8fc24", P1Item2->second->unique_id);
									S4_ItemList.erase(2);
								}
							}
						}
						Legion::IndentEx(80, -22);
						if (Legion::ImageButtonEx("Item3 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer && !IsExcLocked)
							{
								if (P1Item3 != S4_ItemList.end())
								{
									ExchangeP1Texture3 = NULL;
									ExchangeItemInfo3 = "";

									SendInfoToServer(ExchangePlayer2NickName, "aa22c8fc24", P1Item3->second->unique_id);
									S4_ItemList.erase(3);
								}
							}
						}
						Legion::IndentEx(-215, -27);

						if (P1Item != S4_ItemList.end())
						{
							ExchangeP1Texture = P1Item->second->ExchangeTexture;
							ExchangeItemInfo = P1Item->second->Effect;
						}
						else
						{
							ExchangeP1Texture = NULL;
							ExchangeItemInfo = "";
						}

						if (P1Item2 != S4_ItemList.end())
						{
							ExchangeP1Texture2 = P1Item2->second->ExchangeTexture;
							ExchangeItemInfo2 = P1Item2->second->Effect;
						}
						else
						{
							ExchangeP1Texture2 = NULL;
							ExchangeItemInfo2 = "";
						}

						if (P1Item3 != S4_ItemList.end())
						{
							ExchangeP1Texture3 = P1Item3->second->ExchangeTexture;
							ExchangeItemInfo3 = P1Item3->second->Effect;
						}
						else
						{
							ExchangeP1Texture3 = NULL;
							ExchangeItemInfo3 = "";
						}

						Legion::ImageExTipV("Item1", CalibriFont, 12, ExchangeItemInfo, ImageTexture, ExchangeP1Texture, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(80, -84);
						Legion::ImageExTipV("Item2", CalibriFont, 12, ExchangeItemInfo2, ImageTexture, ExchangeP1Texture2, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(80, -84);
						Legion::ImageExTipV("Item3", CalibriFont, 12, ExchangeItemInfo3, ImageTexture, ExchangeP1Texture3, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(-160, 0);
						if (Legion::ImageButtonEx("lock", ("{c1ccdd} " + PlayerExc).c_str(), true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(231, 36), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
						{
							if (!ExchangeBlockPlayer)
							{
								if (!IsExcConfirmed)
								{
									if (IsExcConfirm)
									{
										SendInfoToServer(ExchangePlayer2NickName, "Exc", "e0ddab7762");
										PlayerExc = "CONFIRMED";
										IsExcConfirmed = true;
									}
									else
									{
										if (IsExcLocked)
										{
											SendInfoToServer(ExchangePlayer2NickName, "Exc", "94954aa771");
											PlayerExc = "LOCK";
											IsExcLocked = false;
										}
										else
										{
											if (strlen(P1PEN) == 0)
												P1PEN[0] = '0';
											if (strlen(P1LP) == 0)
												P1LP[0] = '0';

											SendInfoToServer(P1LP, "Exc", "c2d6e162ea");
											SendInfoToServer(P1PEN, "Exc", "5f392a2312");

											SendInfoToServer(ExchangePlayer2NickName, "Exc", "94954aa771");
											PlayerExc = "UNLOCK";
											IsExcLocked = true;
										}
									}
								}
							}
						}

						Legion::IndentEx(310, -326);
						Legion::ImageV("Separat Img", SeparatTexture, ImVec2(2, 306));

						Legion::IndentEx(262, -315);

						Legion::ImageV("Exc Player2 Level", CPlayerListTexture(P2Level, 0), ImVec2(55, 55));
						Legion::IndentEx(-168, -61);
						Legion::LabelEx("ExchangePlayer2NickName", ("{c1ccdd} " + ExchangePlayer2NickName).c_str(), ImVec2(160, 15), CalibriFont, 18, ImVec2(1.0f, 1.0f));
						Legion::IndentEx(113, 0);
						Legion::TextLabelColor(CalibriFont, 14, ("{c1ccdd} Level. " + P2Level).c_str());
						Legion::IndentEx(37, 35);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} PEN.AMOUNT");
						Legion::IndentEx(-90, 0);
						Legion::InputTextV("Player2 PEN", CalibriFont, 12, SmallBoxTexture, "", P2PEN, IM_ARRAYSIZE(P2PEN), ImVec2(172, 17), P2);
						Legion::IndentEx(90, 10);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} EV.AMOUNT");
						Legion::IndentEx(-90, 0);
						Legion::InputTextV("Player2 EVOLVED", CalibriFont, 12, SmallBoxTexture, "", P2LP, IM_ARRAYSIZE(P2LP), ImVec2(172, 17), P2);
						Legion::IndentEx(90, 15);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} ADD OBJECT");
						Legion::IndentEx(-112, 15);

						auto P2Item = S4_ItemList2.find(1);
						auto P2Item2 = S4_ItemList2.find(2);
						auto P2Item3 = S4_ItemList2.find(3);

						if (Legion::ImageButtonEx("P2 Item3 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer2 && !IsExcLocked2)
							{
								if (P2Item3 != S4_ItemList2.end())
								{
									ExchangeP2Texture3 = NULL;
									ExchangeP2ItemInfo3 = "";

									SendInfoToServer(ExchangePlayer1NickName, "aa22c8fc24", P2Item3->second->unique_id);
									S4_ItemList2.erase(3);
								}
							}
						}
						Legion::IndentEx(80, -22);
						if (Legion::ImageButtonEx("P2 Item2 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer2 && !IsExcLocked2)
							{
								if (P2Item2 != S4_ItemList2.end())
								{
									ExchangeP2Texture2 = NULL;
									ExchangeP2ItemInfo2 = "";

									SendInfoToServer(ExchangePlayer1NickName, "aa22c8fc24", P2Item2->second->unique_id);
									S4_ItemList2.erase(2);
								}
							}
						}
						Legion::IndentEx(80, -22);
						if (Legion::ImageButtonEx("P2 Item1 Remove", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(12, 12), CalibriFont, 11))
						{
							if (!ExchangeBlockPlayer2 && !IsExcLocked2)
							{
								if (P2Item != S4_ItemList2.end())
								{
									ExchangeP2Texture = NULL;
									ExchangeP2ItemInfo = "";

									SendInfoToServer(ExchangePlayer1NickName, "aa22c8fc24", P2Item->second->unique_id);
									S4_ItemList2.erase(1);
								}
							}
						}

						if (P2Item != S4_ItemList2.end())
						{
							ExchangeP2Texture = P2Item->second->ExchangeTexture;
							ExchangeP2ItemInfo = P2Item->second->Effect;
						}
						else
						{
							ExchangeP2Texture = NULL;
							ExchangeP2ItemInfo = "";
						}

						if (P2Item2 != S4_ItemList2.end())
						{
							ExchangeP2Texture2 = P2Item2->second->ExchangeTexture;
							ExchangeP2ItemInfo2 = P2Item2->second->Effect;
						}
						else
						{
							ExchangeP2Texture2 = NULL;
							ExchangeP2ItemInfo2 = "";
						}

						if (P2Item3 != S4_ItemList2.end())
						{
							ExchangeP2Texture3 = P2Item3->second->ExchangeTexture;
							ExchangeP2ItemInfo3 = P2Item3->second->Effect;
						}
						else
						{
							ExchangeP2Texture3 = NULL;
							ExchangeP2ItemInfo3 = "";
						}

						Legion::IndentEx(-215, -27);
						Legion::ImageExTipV("P2 Item3", CalibriFont, 12, ExchangeP2ItemInfo3, ImageTexture, ExchangeP2Texture3, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(80, -84);
						Legion::ImageExTipV("P2 Item2", CalibriFont, 12, ExchangeP2ItemInfo2, ImageTexture, ExchangeP2Texture2, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(80, -84);
						Legion::ImageExTipV("P2 Item1", CalibriFont, 12, ExchangeP2ItemInfo, ImageTexture, ExchangeP2Texture, EmptyBoxUITexture, ImVec2(80, 80));
						Legion::IndentEx(-160, 0);
						if (Legion::ImageButtonEx("P2 lock", ("{c1ccdd} " + Player2Exc).c_str(), true, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(231, 36), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
						{
							if (!ExchangeBlockPlayer2)
							{
								if (!IsExcConfirmed)
								{
									if (IsExcConfirm)
									{
										SendInfoToServer(ExchangePlayer1NickName, "Exc", "e0ddab7762");
										Player2Exc = "CONFIRMED";
										IsExcConfirmed = true;
									}
									else
									{
										if (IsExcLocked2)
										{
											SendInfoToServer(ExchangePlayer1NickName, "Exc", "94954aa771");
											Player2Exc = "LOCK";
											IsExcLocked2 = false;
										}
										else
										{
											if (strlen(P2PEN) == 0)
												P2PEN[0] = '0';
											if (strlen(P2LP) == 0)
												P2LP[0] = '0';

											SendInfoToServer(P2LP, "Exc", "871d315942");
											SendInfoToServer(P2PEN, "Exc", "f9f989eb40");

											SendInfoToServer(ExchangePlayer1NickName, "Exc", "94954aa771");
											Player2Exc = "UNLOCK";
											IsExcLocked2 = true;
										}
									}
								}
							}
						}
					}
					else
					{
						Legion::IndentEx(30, 30);
						Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} Find Player.");
						Legion::IndentEx(0, 5);
						Legion::InputTextV("Exchange Player NickName", CalibriFont, 11, SmallBoxTexture, "", ExcFindPlayer, IM_ARRAYSIZE(ExcFindPlayer), ImVec2(172, 17), LegionInputTextFlags_CharsNoBlank);
						Legion::IndentEx(-3, 10);
						if (Legion::ImageButtonEx("Send Exchange Request", "{c1ccdd} Send Request", true, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(167, 30), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
							SendInfoToServer(ExcFindPlayer, "Exc", "f436dea00b");
					}
				}

				if (MenuSelected == 8)
				{
					MainStatus = "Rank & Challenge";
					Legion::IndentEx(45, 45);
					Legion::ImageV("Player Level Img", CPlayerListTexture(MyLevel, 0), ImVec2(55, 55));
					Legion::IndentEx(59, -58);
					Legion::TextLabelColor(CalibriFont, 18, ("{c1ccdd}" + MyNickName).c_str());
					Legion::IndentEx(-2, 0);
					Legion::TextLabelColor(CalibriFont, 15, ("{c1ccdd} Level." + MyLevel).c_str());
					Legion::TextLabelColor(CalibriFont, 14, "{c1ccdd} Rank.");
					Legion::IndentEx(55, -27);
					Legion::ImageV("Rank Img", CPlayerListTexture(RankPointName(MyRankPoint), 1), ImVec2(35, 35));
					Legion::IndentEx(35, -29);
					Legion::TextLabelColor(CalibriFont, 14, ("{16a7bd}" + RankPointName(MyRankPoint)).c_str());
					Legion::IndentEx(-90, 2);
					Legion::TextLabelColor(CalibriFont, 14, ("{c1ccdd} Rank Status.     {ac3232}" + RankStats).c_str());

					if (Legion::ImageButtonEx("Rank Register", RankButton.c_str(), true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(244, 36), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
					{
						if (RankStats == "Not Registered")
						{
							SendInfoToServer("Register", "RNK", "83c07fa40b");
							RankStats = "Registered ";
							RankButton = "Cancel";
						}
						else
						{
							SendInfoToServer("Cancel", "RNK", "83c07fa40b");
							RankStats = "Not Registered";
							RankButton = "{c1ccdd}Register";
						}
					}
					Legion::IndentEx(97, 0);
					Legion::TextLabelColor(CalibriFont, 10, "{15222f}Press to register");
					Legion::IndentEx(-96, 0);
					Legion::TextLabelColor(CalibriFont, 14, "{c1ccdd} Rank Point");
					Legion::IndentEx(209, -18);
					Legion::TextLabelColor(CalibriFont, 14, ("{1499ad} " + std::to_string(MyRankPoint)).c_str());
					Legion::IndentEx(85, -170);
					Legion::ImageV("Separat Img", SeparatTexture, ImVec2(2, 306));
					Legion::IndentEx(55, -300);
					Legion::TextLabelColor(CalibriFont, 18, "{c1ccdd} Challenge");
					Legion::IndentEx(0, 27);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} Find Player.");
					Legion::InputTextV("Challenge Player NickName", CalibriFont, 11, SmallBoxTexture, "", ChalFindPlayer, IM_ARRAYSIZE(ChalFindPlayer), ImVec2(172, 17), LegionInputTextFlags_CharsNoBlank);
					Legion::IndentEx(0, 5);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} PEN.AMOUNT");
					Legion::InputTextV("Challenge Player PEN", CalibriFont, 11, SmallBoxTexture, "", CPPEN, IM_ARRAYSIZE(CPPEN), ImVec2(172, 17), LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal);
					Legion::IndentEx(0, 5);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} EVOLVED.AMOUNT");
					Legion::InputTextV("Challenge Player EVOLVED", CalibriFont, 11, SmallBoxTexture, "", CPLP, IM_ARRAYSIZE(CPLP), ImVec2(172, 17), LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsDecimal);
					Legion::IndentEx(-4, 10);
					if (Legion::ImageButtonEx("Send Challenge Request", "{c1ccdd} Send Request", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(230, 36), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
					{
						std::string pen;
						std::string evolved;
						if (strlen(CPPEN) == 0)
							pen = "0";
						else
							pen = CPPEN;

						if (strlen(CPLP) == 0)
							evolved = "0";
						else
							evolved = CPLP;

						SendInfoToServer(std::string(ChalFindPlayer) + ", want to a challenge for a " + CPPEN + "PEN" + evolved + "EVOLVED", "CHLG", "21d2face68");
					}
				}
			}
			Legion::EndChild();
			Legion::End();
		}

		if (RoomMapFilter)
		{
			Legion::BeginEx(RoomFiltersUITexture, NULL, NULL, "Room Map Filter", ImVec2(300, 460), ImVec2(0.5f, 0.5f), &RoomMapFilter, LegionWindowFlags_NoBackground | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoResize | LegionWindowFlags_NoCollapse | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoScrollWithMouse);
			Legion::IndentEx(18, 7);
			Legion::TextLabelColor(CalibriFont, 12, "MODE");
			Legion::IndentEx(0, -2);
			if (Legion::ImageExS("MODE Image", CalibriFont, 12, "", BBoxTexture, ModesTexture(RoomFilterModes[RoomFilterModeId]), ImVec2(248, 57)))
			{
				if (RoomFilterModeId < 4)
				{
					RoomFilterModeId++;
					RoomFilterMapId = 0;
				}
				else
				{
					RoomFilterModeId = 0;
					RoomFilterMapId = 0;
				}
			}
			Legion::IndentEx(-10, -47);

			if (Legion::ImageEx("MODE Left Arrow", CalibriFont, 13, "", "", false, LeftArrowTexture, NULL, ImVec2(19, 28)))
			{
				if (RoomFilterModeId > 0)
				{
					RoomFilterModeId--;
					RoomFilterMapId = 0;
				}
				else
				{
					RoomFilterModeId = RoomFilterModes.size() - 1;
					RoomFilterMapId = 0;
				}
			}
			Legion::SameLine(265);
			if (Legion::ImageEx("MODE Right Arrow", CalibriFont, 13, "", "", false, RightArrowTexture, NULL, ImVec2(19, 28)))
			{
				if (RoomFilterModeId < 4)
				{
					RoomFilterModeId++;
					RoomFilterMapId = 0;
				}
				else
				{
					RoomFilterModeId = 0;
					RoomFilterMapId = 0;
				}
			}
			Legion::IndentEx(10, 13);
			Legion::TextLabelColor(CalibriFont, 12, "MAP");
			Legion::IndentEx(0, -2);

			std::vector<std::string> Selectedfilter;
			if (RoomFilterModeId == 0)
				Selectedfilter = TouchdownMaps;
			else if (RoomFilterModeId == 1)
				Selectedfilter = DeathmatchMaps;
			else if (RoomFilterModeId == 2)
				Selectedfilter = ChaserMaps;
			else if (RoomFilterModeId == 3)
				Selectedfilter = BattleRoyalMaps;
			else if (RoomFilterModeId == 4)
				Selectedfilter = TouchdownPassMaps;

			if (Legion::ImageExS("map Image", CalibriFont, 13.5f, Selectedfilter[RoomFilterMapId].c_str(), BBoxTexture, MapsTexture(Selectedfilter[RoomFilterMapId]), ImVec2(248, 114)))
			{
				if (RoomFilterMapId < Selectedfilter.size() - 1)
					RoomFilterMapId++;
				else
					RoomFilterMapId = 0;
			}

			Legion::IndentEx(-10, -77);

			if (Legion::ImageEx("map Left Arrow", CalibriFont, 13, "", "", false, LeftArrowTexture, NULL, ImVec2(19, 28)))
			{
				if (RoomFilterMapId > 0)
					RoomFilterMapId--;
				else
					RoomFilterMapId = Selectedfilter.size() - 1;
			}
			Legion::SameLine(265);
			if (Legion::ImageEx("map Right Arrow", CalibriFont, 13, "", "", false, RightArrowTexture, NULL, ImVec2(19, 28)))
			{
				if (RoomFilterMapId < Selectedfilter.size() - 1)
					RoomFilterMapId++;
				else
					RoomFilterMapId = 0;
			}
			Legion::IndentEx(10, 43);
			Legion::TextLabelColor(CalibriFont, 12, "FILTERS");
			Legion::IndentEx(0, -5);
			Legion::BeginChildV(BBoxTexture, NULL, NULL, "Filter Mapss", ImVec2(248, 114));
			Legion::IndentEx(0, 3);
			Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "Filter Se", ImVec2(240, 105));
			Legion::IndentEx(3, 5);

			for (size_t i = 0; i < MapFilters.size(); ++i)
			{
				if (Legion::RoomFilterEx(std::to_string(i).c_str(), MapFilters[i].c_str(), CalibriFont, 12, ImVec2(200, 10)))
					RoomFilterId = i;
			}

			Legion::EndChild();
			Legion::EndChild();
			Legion::IndentEx(0, -3);
			if (!CRoomOptionClient)
			{
				if (Legion::ImageButtonEx("ADD", "{c1ccdd}ADD", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(117, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
				{
					MapFilters.push_back(RoomFilterModes[RoomFilterModeId] + "|" + Selectedfilter[RoomFilterMapId]);
				}
				Legion::SameLine(148);
				if (Legion::ImageButtonEx("REMOVE", "{c1ccdd}REMOVE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(117, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
				{

					MapFilters.erase(MapFilters.begin() + RoomFilterId);
				}
				Legion::IndentEx(0, -5);
			}
			if (Legion::ImageButtonEx("CLOSE", "{c1ccdd}CLOSE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(240, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				if (!CRoomOptionClient)
				{
					mapfilters = "";
					for (size_t i = 0; i < MapFilters.size(); ++i)
					{
						mapfilters += MapFilters[i] + ",";
					}
				}
				RoomMapFilter = false;
			}
			Legion::IndentEx(90, 5);
			Legion::TextLabelColor(CalibriFont, 12, "ROOM FILTER");
			Legion::End();
		}

		if (CGameShopUI)
		{
			Legion::BeginEx(ShopUITexture, NULL, NULL, "Game Shop", ImVec2(500, 400), ImVec2(0.5f, 0.5f), &CGameShopUI, LegionWindowFlags_Custom);
			Legion::IndentEx(464, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(18, 20), CalibriFont, 11))
			{
				IsBuyButtonPressed = false;
				CGameShopUI = false;
			}

			Legion::IndentEx(-435, 10);
			Legion::ImageExShop("Item Image", ImageTexture, ShopItemTexture, ImVec2(110, 110));

			Legion::IndentEx(8, -73);
			if (ShopItemColorMax >= 1 && ShopItemCurrentColor > 0)
				CNLeftArrowTexture = LeftArrowTexture;
			else
				CNLeftArrowTexture = NULL;

			if (Legion::ImageEx("Color Left Arrow", CalibriFont, 13, "", "", false, CNLeftArrowTexture, NULL, ImVec2(19, 28)) && ShopItemColorMax >= 1 && ShopItemCurrentColor > 0)
			{
				ShopItemCurrentColor--;
				ShopItemTexture = LoadTextrueEx(std::to_string(ItemId) + "_" + std::to_string(ShopItemCurrentColor));
			}

			Legion::IndentEx(76, -32);

			if (ShopItemColorMax >= 1 && ShopItemCurrentColor < ShopItemColorMax)
				CNRightArrowTexture = RightArrowTexture;
			else
				CNRightArrowTexture = NULL;

			if (Legion::ImageEx("Color Right Arrow", CalibriFont, 13, "", "", false, CNRightArrowTexture, NULL, ImVec2(19, 28)) && ShopItemColorMax >= 1 && ShopItemCurrentColor < ShopItemColorMax)
			{
				PriceRate = 1;
				ShopItemCurrentColor++;
				ShopItemPrice();
				ShopItemTexture = LoadTextrueEx(std::to_string(ItemId) + "_" + std::to_string(ShopItemCurrentColor));
			}

			Legion::IndentEx(30, -60);

			Legion::TextLabelColor(CalibriFont, 13, ShopItemName);
			Legion::IndentEx(-5, 5);
			if (Legion::BeginComboEx(CalibriFont, 13, "Item Use ComboBox", ShopSelectedType.c_str(), ImVec2(297, 32), 2.6f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
			{
				for (auto it : ShopItemUse)
				{
					if (Legion::SelectableExV("Item", CalibriFont, 13, it.c_str()))
						ShopSelectedType = it;
				}
				Legion::EndCombo();
			}

			Legion::IndentEx(0, -5);
			if (Legion::BeginComboEx(CalibriFont, 13, "Effect ComboBox", ShopSelectedEffect.c_str(), ImVec2(297, 32), 2.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
			{
				/*for (auto shopeffect : ShopItemEffects)
				{*/
				if (Legion::SelectableExV("Effect", CalibriFont, 13, ShopItemEffects))
					ShopSelectedEffect = ShopItemEffects;
				//}
				Legion::EndCombo();
			}

			Legion::IndentEx(5, 7);
			Legion::TextLabelColor(CalibriFont, 13, "{c1ccdd} DISCOUNT COUPON.");
			Legion::IndentEx(-5, 0);
			if (Legion::BeginComboEx(CalibriFont, 13, "DISCOUNT Combobox", ShopSelectedCoupon.c_str(), ImVec2(297, 32), 2.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
			{
				for (auto shopcoupon : ShopCoupon)
				{
					if (Legion::SelectableExV("DISCOUNT", CalibriFont, 13, shopcoupon.c_str()))
						ShopSelectedCoupon = shopcoupon;
				}
				Legion::EndCombo();
			}


			Legion::IndentEx(-90, 30);
			Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} ITEM PRICE.  " + ShopItemMoneyPrice).c_str());
			Legion::IndentEx(0, 10);
			Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} MONEY LEFT.  " + ShopItemMoneyLeft).c_str());
			Legion::IndentEx(260, -35);
			if (Legion::CheckboxEx("use pen", NormalCheckBoxTexture, ActiveCheckBoxTexture, CalibriFont, 13, "{c1ccdd}USE PEN.", &ShopUsePen, false))
			{
				if (ShopItemCurrentColor == 0)
					ShopItemPrice();
			}
			Legion::IndentEx(-220, 40);
			if (Legion::ImageButtonEx("buy", "{c1ccdd} BUY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(312, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				CGameShopUI = false;
				SendInfoToServer(std::to_string(ItemId) + "|" + ShopSelectedEffect + "|" + std::to_string(ShopItemCurrentColor) + "|" + ShopSelectedType, std::to_string(ShopUsePen), "7085925aa1");
			}

			Legion::IndentEx(95, 5);
			Legion::TextLabelColor(CalibriFont, 16, "{c1ccdd} PURCHASE ITEM");
			Legion::End();
		}

		if (ShopSearch)
		{
			ImVec2 ScreenSize = Legion::GetIO().DisplaySize;

			float locx;
			float locy;
			float multiplierx;
			float multipliery;

			if (ScreenSize.x == 800)
			{
				multiplierx = 0.87f;
				multipliery = 0.83f;
			}
			else if (ScreenSize.x == 1024)
			{
				multiplierx = 0.92f;
				multipliery = 0.88f;
			}
			else if (ScreenSize.x == 1152)
			{
				multiplierx = 0.94f;
				multipliery = 0.897f;
			}
			else if (ScreenSize.x == 1176)
			{
				multiplierx = 0.94f;
				multipliery = 0.85f;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 720)
			{
				multiplierx = 0.955f;
				multipliery = 0.868f;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 768)
			{
				multiplierx = 0.955f;
				multipliery = 0.879f;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 800)
			{
				multiplierx = 0.955f;
				multipliery = 0.886f;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 960)
			{
				multiplierx = 0.96f;
				multipliery = 0.9f;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 1024)
			{
				multiplierx = 0.96f;
				multipliery = 0.916f;
			}
			else if (ScreenSize.x == 1360)
			{
				multiplierx = 0.96f;
				multipliery = 0.879f;
			}
			else if (ScreenSize.x == 1366)
			{
				multiplierx = 0.96f;
				multipliery = 0.879f;
			}
			else if (ScreenSize.x == 1440 && ScreenSize.y == 900)
			{
				multiplierx = 0.97f;
				multipliery = 0.896f;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 900)
			{
				multiplierx = 0.98f;
				multipliery = 0.9f;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 1024)
			{
				multiplierx = 0.98f;
				multipliery = 0.916f;
			}
			else if (ScreenSize.x == 1680 && ScreenSize.y == 1050)
			{
				multiplierx = 0.98f;
				multipliery = 0.917f;
			}
			else if (ScreenSize.x == 1920)
			{
				multiplierx = 1;
				multipliery = 0.92f;
			}

			if (ScreenRaito() == "4:3")
			{
				locx = 0.79375f * multiplierx * ScreenSize.x;
				locy = 0.239815f * multipliery * ScreenSize.y;
			}
			else if (ScreenRaito() == "16:9")
			{
				locx = 0.705729f * multiplierx * ScreenSize.x;
				locy = 0.239815f * multipliery * ScreenSize.y;
			}
			else if (ScreenRaito() == "16:10")
			{
				locx = 0.734896f * multiplierx * ScreenSize.x;
				locy = 0.239815f * multipliery * ScreenSize.y;
			}
			else if (ScreenRaito() == "5:3")
			{

				locx = 0.7234375f * multiplierx * ScreenSize.x;
				locy = 0.239815f * multipliery * ScreenSize.y;
			}
			else if (ScreenRaito() == "5:4")
			{
				locx = multiplierx * ScreenSize.x * 0.794791f;
				locy = multipliery * ScreenSize.y * 0.239815f;
			}

			Legion::SetNextWindowPos(ImVec2(locx, locy), 0, ImVec2(0.0f, 0.0f));
			Legion::BeginEx(NULL, NULL, NULL, "Shop Item Search", ImVec2(120, 30), ImVec2(0, 0), &ShopSearch, LegionWindowFlags_Custom);

			if (Legion::InputTextWithHintV("ShopItemSearchTextBox", CalibriFont, 11, ChatTextBoxTexture, "", "Item Name", ShopItemsSearch, IM_ARRAYSIZE(ShopItemsSearch), ImVec2(120, 20), LegionInputTextFlags_EnterReturnsTrue))
			{
				SendInfoToServer("614440bde9", "SIS", ShopItemsSearch);
				//	memset(ShopItemsSearch, 0, sizeof ShopItemsSearch);
			}
			Legion::End();
		}

		if (ShopSearchUI)
		{
			Legion::BeginEx(ShopUITexture, NULL, NULL, "Shop Item Search UI", ImVec2(660, 450), ImVec2(0.5f, 0.5f), &ShopSearchUI, LegionWindowFlags_Custom);
			Legion::IndentEx(624, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(18, 20), CalibriFont, 11))
				ShopSearchUI = false;
			Legion::IndentEx(-624, -8);
			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Item Search Gruop", ImVec2(640, 230));

			int i = 0;
			if (ShopSearchItems.size() > 0)
			{
				do
				{
					i++;
					if (i % 2 != 0)
					{
						Legion::BeginChildV(BBoxTexture, NULL, NULL, ("Item Search " + std::to_string(i)).c_str(), ImVec2(305, 100));
						{
							float EffectComboY = -5;
							Legion::IndentEx(4, 0);
							Legion::ImageExV(("Item Image " + std::to_string(i)).c_str(), ImageTexture, ShopSearchItems[i]->Image, ImVec2(100, 100));
							Legion::IndentEx(4, -100);
							if (Legion::CheckboxExS(("Is Active" + std::to_string(i)).c_str(), NormalCheckBoxTexture, ActiveCheckBoxTexture, &ShopSearchItems[i]->IsActive))
							{
								if (ShopSearchItems[i]->IsActive)
								{
									SShopItemMoneyPrice += (DWORD)(ShopSearchItems[i]->Price * ShopDisc());
									SShopItemMoneyLeft = SShopItemMoneyLeft - SShopItemMoneyPrice;
								}
								else
								{
									SShopItemMoneyLeft = SShopItemMoneyLeft + SShopItemMoneyPrice;
									SShopItemMoneyPrice -= (DWORD)(ShopSearchItems[i]->Price * ShopDisc());
								}
							}
							Legion::IndentEx(2, 38);
							if (ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color > 0)
								CLeftArrowTexture = LeftArrowTexture;
							else
								CLeftArrowTexture = NULL;

							if (Legion::ImageEx(("Color Left Arrow " + std::to_string(i)).c_str(), CalibriFont, 13, "", "", false, CLeftArrowTexture, NULL, ImVec2(12, 19)) && ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color > 0)
							{
								ShopSearchItems[i]->Color--;
								ShopSearchItems[i]->Image = LoadTextrueEx(std::to_string(ShopSearchItems[i]->Id) + "_" + std::to_string(ShopSearchItems[i]->Color));
							}

							Legion::IndentEx(76, -24);

							if (ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color < ShopSearchItems[i]->MaxColor)
								CRightArrowTexture = RightArrowTexture;
							else
								CRightArrowTexture = NULL;

							if (Legion::ImageEx(("Color Right Arrow " + std::to_string(i)).c_str(), CalibriFont, 13, "", "", false, CRightArrowTexture, NULL, ImVec2(12, 19)) && ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color < ShopSearchItems[i]->MaxColor)
							{
								ShopSearchItems[i]->Color++;
								ShopSearchItems[i]->Image = LoadTextrueEx(std::to_string(ShopSearchItems[i]->Id) + "_" + std::to_string(ShopSearchItems[i]->Color));
							}


							Legion::IndentEx(13, -57);
							Legion::TextLabelColor(CalibriFont, 13, ShopSearchItems[i]->Name);
							Legion::IndentEx(-4, -5);
							if (ShopSearchItems[i]->ShopSelectedType.empty() && ShopSearchItems[i]->Type.size() > 0)
							{
								ShopSearchItems[i]->ShopSelectedType = ShopSearchItems[i]->Type[0];
								EffectComboY = -5;
							}
							if (Legion::BeginComboEx(CalibriFont, 13, ("Item Use ComboBox " + std::to_string(i)).c_str(), ShopSearchItems[i]->ShopSelectedType.c_str(), ImVec2(190, 32), 3.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
							{
								for (auto it : ShopSearchItems[i]->Type)
								{
									if (Legion::SelectableExV("Item", CalibriFont, 13, it.c_str()))
										ShopSearchItems[i]->ShopSelectedType = it;
								}

								Legion::EndCombo();
							}
							Legion::IndentEx(0, EffectComboY);
							if (ShopSearchItems[i]->ShopSelectedEffect.empty() && ShopSearchItems[i]->Effects != NULL)
								ShopSearchItems[i]->ShopSelectedEffect = ShopSearchItems[i]->Effects;

							if (Legion::BeginComboEx(CalibriFont, 13, ("Effect ComboBox " + std::to_string(i)).c_str(), ShopSearchItems[i]->ShopSelectedEffect.c_str(), ImVec2(190, 32), 3.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
							{
								//for (auto shopeffect : ShopSearchItems[i]->Effects)
								//{
								if (Legion::SelectableExV("Effect", CalibriFont, 13, ShopSearchItems[i]->Effects))
									ShopSearchItems[i]->ShopSelectedEffect = ShopSearchItems[i]->Effects;
								//	}
								Legion::EndCombo();
							}

						}Legion::EndChild();
					}
					if (i % 2 == 0)
					{
						Legion::SameLine(320);
						Legion::BeginChildV(BBoxTexture, NULL, NULL, ("Item Search " + std::to_string(i)).c_str(), ImVec2(305, 100));
						{
							float EffectComboY = -5;
							Legion::IndentEx(4, 0);
							Legion::ImageExV(("Item Image " + std::to_string(i)).c_str(), ImageTexture, ShopSearchItems[i]->Image, ImVec2(100, 100));
							Legion::IndentEx(4, -100);
							if (Legion::CheckboxExS(("Is Active" + std::to_string(i)).c_str(), NormalCheckBoxTexture, ActiveCheckBoxTexture, &ShopSearchItems[i]->IsActive))
							{
								if (ShopSearchItems[i]->IsActive)
								{
									SShopItemMoneyPrice += ShopSearchItems[i]->Price;
									SShopItemMoneyLeft = SShopItemMoneyLeft - SShopItemMoneyPrice;
								}
								else
								{
									SShopItemMoneyLeft = SShopItemMoneyLeft + SShopItemMoneyPrice;
									SShopItemMoneyPrice -= ShopSearchItems[i]->Price;
								}
							}
							Legion::IndentEx(2, 38);
							if (ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color > 0)
								CLeftArrowTexture = LeftArrowTexture;
							else
								CLeftArrowTexture = NULL;

							if (Legion::ImageEx(("Color Left Arrow " + std::to_string(i)).c_str(), CalibriFont, 13, "", "", false, CLeftArrowTexture, NULL, ImVec2(12, 19)) && ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color > 0)
							{
								ShopSearchItems[i]->Color--;
								ShopSearchItems[i]->Image = LoadTextrueEx(std::to_string(ShopSearchItems[i]->Id) + "_" + std::to_string(ShopSearchItems[i]->Color));
							}

							Legion::IndentEx(76, -24);

							if (ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color < ShopSearchItems[i]->MaxColor)
								CRightArrowTexture = RightArrowTexture;
							else
								CRightArrowTexture = NULL;

							if (Legion::ImageEx(("Color Right Arrow " + std::to_string(i)).c_str(), CalibriFont, 13, "", "", false, CRightArrowTexture, NULL, ImVec2(12, 19)) && ShopSearchItems[i]->MaxColor >= 1 && ShopSearchItems[i]->Color < ShopSearchItems[i]->MaxColor)
							{
								ShopSearchItems[i]->Color++;
								ShopSearchItems[i]->Image = LoadTextrueEx(std::to_string(ShopSearchItems[i]->Id) + "_" + std::to_string(ShopSearchItems[i]->Color));
							}


							Legion::IndentEx(13, -57);
							Legion::TextLabelColor(CalibriFont, 13, ShopSearchItems[i]->Name);
							Legion::IndentEx(-4, -5);
							if (ShopSearchItems[i]->ShopSelectedType.empty() && ShopSearchItems[i]->Type.size() > 0)
							{
								ShopSearchItems[i]->ShopSelectedType = ShopSearchItems[i]->Type[0];
								EffectComboY = -5;
							}
							if (Legion::BeginComboEx(CalibriFont, 13, ("Item Use ComboBox " + std::to_string(i)).c_str(), ShopSearchItems[i]->ShopSelectedType.c_str(), ImVec2(190, 32), 4.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
							{
								for (auto it : ShopSearchItems[i]->Type)
								{
									if (Legion::SelectableExV("Item", CalibriFont, 13, it.c_str()))
										ShopSearchItems[i]->ShopSelectedType = it;
								}

								Legion::EndCombo();
							}
							Legion::IndentEx(0, EffectComboY);
							if (ShopSearchItems[i]->ShopSelectedEffect.empty() && ShopSearchItems[i]->Effects != NULL)
								ShopSearchItems[i]->ShopSelectedEffect = ShopSearchItems[i]->Effects;

							if (Legion::BeginComboEx(CalibriFont, 13, ("Effect ComboBox " + std::to_string(i)).c_str(), ShopSearchItems[i]->ShopSelectedEffect.c_str(), ImVec2(190, 32), 4.8f, SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
							{
								//	for (auto shopeffect : ShopSearchItems[i]->Effects)
								//	{
								if (Legion::SelectableExV("Effect", CalibriFont, 13, ShopSearchItems[i]->Effects))
									ShopSearchItems[i]->ShopSelectedEffect = ShopSearchItems[i]->Effects;
								//	}
								Legion::EndCombo();
							}

						}Legion::EndChild();
					}
				} while (i != ShopSearchItems.size());
			}

			Legion::EndChild();
			Legion::IndentEx(80, 30);
			Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} ITEM PRICE.  " + std::to_string(SShopItemMoneyPrice)).c_str());
			Legion::IndentEx(0, 1);
			Legion::TextLabelColor(CalibriFont, 13, ("{c1ccdd} MONEY LEFT.  " + std::to_string(SShopItemMoneyLeft)).c_str());

			Legion::IndentEx(95, 50);
			if (Legion::ImageButtonEx("buy", "{c1ccdd} BUY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(312, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				SelectedItemSearch = "";
				for (std::pair<unsigned int, ShopSearchItem*> Item : ShopSearchItems)
					if (Item.second->IsActive)
						SelectedItemSearch += std::to_string(Item.second->Id) + "-" + std::to_string(Item.second->Color) + "-" + Item.second->ShopSelectedType + "-" + Item.second->ShopSelectedEffect + ",";

				ShopSearchUI = false;
				SendInfoToServer(SelectedItemSearch, "SIS", "d140a837d7");
			}

			Legion::IndentEx(120, 15);
			Legion::TextLabelColor(CalibriFont, 16, "{c1ccdd} PURCHASE");
			Legion::End();
		}

		if (CCustomNameTagUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Custom NameTag", ImVec2(351, 190), ImVec2(0.5f, 0.5f), &CCustomNameTagUI, LegionWindowFlags_Custom);
			Legion::IndentEx(317, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(15, 17), CalibriFont, 11))
				CCustomNameTagUI = false;
			Legion::IndentEx(-310, 30);
			Legion::TextLabelColor(CalibriFont, 12, "{c1ccdd} YOUR NAMETAG");
			Legion::IndentEx(50, 5);
			Legion::InputTextWithHintV("NameTag", CalibriFont, 11, SmallBoxTexture, "", "GGGG", CCustomNameTag, IM_ARRAYSIZE(CCustomNameTag), ImVec2(229, 17), LegionInputTextFlags_CharsNoBlank);
			Legion::IndentEx(50, 10);
			if (Legion::ImageButtonEx("Custom NameTag Btn", "{c1ccdd} DONE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(116, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				SendInfoToServer(CCustomNameTag, CustomTagItemId, "b9e297784e");
				CCustomNameTagUI = false;
			}
			Legion::IndentEx(-7, 10);
			Legion::TextLabelColor(CalibriFont, 16, "{c1ccdd} CUSTOM NAMETAG");
			Legion::End();
		}

		if (CAutoEventUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Auto Event", ImVec2(351, 190), ImVec2(0.5f, 0.5f), &CAutoEventUI, LegionWindowFlags_Custom);
			Legion::IndentEx(317, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(15, 17), CalibriFont, 11))
				CAutoEventUI = false;
			Legion::IndentEx(-317, -8);
			Legion::BeginChildV(NULL, NULL, NULL, "Auto Event Q", ImVec2(350, 60));
			Legion::TextLabelColorEx(CalibriFont, 12, QAutoEvent.c_str());
			Legion::EndChild();
			Legion::IndentEx(55, 05);
			Legion::InputTextWithHintV("Auto Event", CalibriFont, 11, SmallBoxTexture, "", "Answer", AAutoEvent, IM_ARRAYSIZE(AAutoEvent), ImVec2(229, 17), LegionInputTextFlags_CharsHexadecimal);
			Legion::IndentEx(50, 8);
			if (Legion::ImageButtonEx("Auto Event Btn", "{c1ccdd} DONE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(116, 35), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
			{
				SendInfoToServer(AAutoEvent, "auen", "4c7af53c6e");
				CAutoEventUI = false;
			}
			Legion::IndentEx(13, 10);
			Legion::TextLabelColor(CalibriFont, 16, "{c1ccdd} AUTO EVENT");
			Legion::End();
		}

		if (CDailyRewardUI)
		{
			Legion::BeginEx(DailyEventUITexture, NULL, NULL, "Daily Event", ImVec2(750, 485), ImVec2(0.5f, 0.5f), &CDailyRewardUI, LegionWindowFlags_Custom);
			Legion::IndentEx(712, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 20), CalibriFont, 11))
				CDailyRewardUI = false;

			Legion::IndentEx(-690, 41);
			Legion::ImageV("day1", DailyEventList[1], ImVec2(78, 78));
			Legion::SameLine(117);
			Legion::ImageV("day2", DailyEventList[2], ImVec2(78, 78));
			Legion::SameLine(202);
			Legion::ImageV("day3", DailyEventList[3], ImVec2(78, 78));
			Legion::SameLine(287);
			Legion::ImageV("day4", DailyEventList[4], ImVec2(78, 78));
			Legion::SameLine(372);
			Legion::ImageV("day5", DailyEventList[5], ImVec2(78, 78));
			Legion::SameLine(457);
			Legion::ImageV("day6", DailyEventList[6], ImVec2(78, 78));
			Legion::SameLine(542);
			Legion::ImageV("day7", DailyEventList[7], ImVec2(78, 78));
			Legion::IndentEx(0, 4);
			Legion::ImageV("day8", DailyEventList[8], ImVec2(78, 78));
			Legion::SameLine(117);
			Legion::ImageV("day9", DailyEventList[9], ImVec2(78, 78));
			Legion::SameLine(202);
			Legion::ImageV("day10", DailyEventList[10], ImVec2(78, 78));
			Legion::SameLine(287);
			Legion::ImageV("day11", DailyEventList[11], ImVec2(78, 78));
			Legion::SameLine(372);
			Legion::ImageV("day12", DailyEventList[12], ImVec2(78, 78));
			Legion::SameLine(457);
			Legion::ImageV("day13", DailyEventList[13], ImVec2(78, 78));
			Legion::SameLine(542);
			Legion::ImageV("day14", DailyEventList[14], ImVec2(78, 78));
			Legion::IndentEx(0, 4);
			Legion::ImageV("day15", DailyEventList[15], ImVec2(78, 78));
			Legion::SameLine(117);
			Legion::ImageV("day16", DailyEventList[16], ImVec2(78, 78));
			Legion::SameLine(202);
			Legion::ImageV("day17", DailyEventList[17], ImVec2(78, 78));
			Legion::SameLine(287);
			Legion::ImageV("day18", DailyEventList[18], ImVec2(78, 78));
			Legion::SameLine(372);
			Legion::ImageV("day19", DailyEventList[19], ImVec2(78, 78));
			Legion::SameLine(457);
			Legion::ImageV("day20", DailyEventList[20], ImVec2(78, 78));
			Legion::SameLine(542);
			Legion::ImageV("day21", DailyEventList[21], ImVec2(78, 78));
			Legion::IndentEx(0, 5);
			Legion::ImageV("day22", DailyEventList[22], ImVec2(78, 78));
			Legion::SameLine(117);
			Legion::ImageV("day23", DailyEventList[23], ImVec2(78, 78));
			Legion::SameLine(202);
			Legion::ImageV("day24", DailyEventList[24], ImVec2(78, 78));
			Legion::SameLine(287);
			Legion::ImageV("day25", DailyEventList[25], ImVec2(78, 78));
			Legion::SameLine(372);
			Legion::ImageV("day26", DailyEventList[26], ImVec2(78, 78));
			Legion::SameLine(457);
			Legion::ImageV("day27", DailyEventList[27], ImVec2(78, 78));
			Legion::SameLine(542);
			Legion::ImageV("day28", DailyEventList[28], ImVec2(78, 78));
			Legion::IndentEx(610, -207);
			Legion::ImageV("day29", DailyEventList[29], ImVec2(78, 78));
			Legion::IndentEx(0, 5);
			Legion::ImageV("day30", DailyEventList[30], ImVec2(78, 78));
			Legion::End();
		}

		if (CommunityUI)
		{
			Legion::BeginEx(RoomFiltersUITexture, NULL, NULL, "Community", ImVec2(300, 460), ImVec2(0.5f, 0.5f), &CommunityUI, LegionWindowFlags_NoBackground | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoResize | LegionWindowFlags_NoCollapse | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoScrollWithMouse);
			Legion::IndentEx(264, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				CommunityUI = false;
			Legion::IndentEx(-266, -5);
			Legion::ImageButtonV("FIRENDS", "{c1ccdd}FRIENDS", true, &SelectedCommunity, 0, ImColor(8, 137, 196), ImColor(219, 146, 59), ImColor(219, 146, 59), ImVec2(67, 30), CalibriFont, 12.4f);
			Legion::SameLine(77);
			Legion::ImageButtonV("CLAN", "{c1ccdd}CLAN", true, &SelectedCommunity, 1, ImColor(8, 137, 196), ImColor(219, 146, 59), ImColor(219, 146, 59), ImVec2(67, 30), CalibriFont, 12.4f);
			Legion::SameLine(148);
			if (Legion::ImageButtonV("UNION", "{c1ccdd}UNION", true, &SelectedCommunity, 2, ImColor(8, 137, 196), ImColor(219, 146, 59), ImColor(219, 146, 59), ImVec2(67, 30), CalibriFont, 12.4f))
				SendInfoToServer("abfaf1cda3", "CN", "CN");
			Legion::SameLine(219);
			Legion::ImageButtonV("BLOCK", "{c1ccdd}BLOCK", true, &SelectedCommunity, 3, ImColor(8, 137, 196), ImColor(219, 146, 59), ImColor(219, 146, 59), ImVec2(67, 30), CalibriFont, 12.4f);

			Legion::BeginChildV(ChatBoxTexture, NULL, NULL, "Community List", ImVec2(290, 340));
			Legion::IndentEx(-5, 0);
			Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "Community List2", ImVec2(280, 330));
			Legion::IndentEx(15, 8);
			if (SelectedCommunity == 0)
			{
				SelectedCommunityPId = SelectedFriendCommunityPId;
				for (std::pair<unsigned int, PlayersShortInfoListClass*> it : FrindsPlayerList)
				{
					if (Legion::FriendsPlayer(it.second->Name, CalibriFont, 14, &SelectedFriendCommunityId, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
					{
						SelectedFriendCommunityPId = SelectedCommunityPId = it.second->PlayerId;
						SelectedCommunityNick = it.second->Name;
						SelectedCommunityFriendStats = "Delete a Friend";
						SelectedCommunityBlockStats = it.second->BlockStats;
					}
				}
			}
			else if (SelectedCommunity == 1)
			{
				SelectedCommunityPId = SelectedClanCommunityPId;

				for (std::pair<unsigned int, PlayerClanShortInfoListClass*> it : ClanPlayerList)
				{
					if (Legion::ClanPlayer(it.second->Name, CalibriFont, 14, &SelectedClanCommunityId, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0), it.second->ClanIcon, it.second->Rank, it.second->StaffIcon))
					{
						SelectedClanCommunityPId = SelectedCommunityPId = it.second->PlayerId;
						SelectedCommunityNick = it.second->Name;
						SelectedCommunityFriendStats = it.second->FriendStats;
						SelectedCommunityBlockStats = it.second->BlockStats;
					}
				}
			}
			else if (SelectedCommunity == 2)//Union
			{
				SelectedCommunityPId = SelectedUnionCommunityPId;

				for (std::pair<unsigned int, PlayerClanShortInfoListClass*> it : UnionPlayerList)
				{
					if (Legion::ClanPlayer(it.second->Name, CalibriFont, 14, &SelectedUnionCommunityId, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0), it.second->ClanIcon, it.second->Rank, it.second->StaffIcon))
					{
						SelectedUnionCommunityPId = SelectedCommunityPId = it.second->PlayerId;
						SelectedCommunityNick = it.second->Name;
						SelectedCommunityFriendStats = it.second->FriendStats;
						SelectedCommunityBlockStats = it.second->BlockStats;
					}
				}
			}
			else if (SelectedCommunity == 3)//Block
			{
				SelectedCommunityPId = SelectedBlockCommunityPId;

				for (std::pair<unsigned int, PlayersShortInfoListClass*> it : BlockPlayerList)
				{
					if (Legion::FriendsPlayer(it.second->Name, CalibriFont, 14, &SelectedBlockCommunityId, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
					{
						SelectedBlockCommunityPId = SelectedCommunityPId = it.second->PlayerId;
						SelectedCommunityNick = it.second->Name;
						SelectedCommunityFriendStats = it.second->FriendStats;
						SelectedCommunityBlockStats = "Remove Block";
					}
				}
			}

			if (MyPlayerId != SelectedCommunityPId && SelectedCommunityPId != 0)
			{
				if (Legion::BeginPopupContextWindowV(ContextTexture, NULL, NULL, "Community List Context", LegionMouseButton_Right))
				{
					std::string nickname;
					if (SelectedCommunityNick.find("{") != std::string::npos)
						nickname = Mem.Split(SelectedCommunityNick, '}')[1];
					else
						nickname = SelectedCommunityNick;

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "User Info", NULL))
					{
						UserInfo_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), SelectedCommunityPId, 0);
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Whisper", NULL))
					{
						ChatButtonTexture(Chat_Type::Whisper);

						memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);

						std::string whisper;
						if (SelectedCommunityNick.find("{") != std::string::npos)
							whisper = "To " + Mem.Split(SelectedCommunityNick, '}')[1] + " : "/*+ " : "*/;
						else
							whisper = "To " + Mem.Split(SelectedCommunityNick, '}')[0] + " : "/*+ " : "*/;

						std::copy(whisper.begin(), whisper.end(), ChatSystemMessage);

						LegionInputTextState& edit_state = Legion::GetCurrentContext()->InputTextState;

						int n = edit_state.TextW.size();
						edit_state.Stb.cursor = n; //stb_text_locate_coord(str, x, y);
						edit_state.Stb.select_start = edit_state.Stb.cursor;
						edit_state.Stb.select_end = edit_state.Stb.cursor;
						edit_state.Stb.has_preferred_x = 0;
						edit_state.CursorAnimReset();
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, SelectedCommunityFriendStats.c_str(), NULL))
					{
						if (SelectedCommunityFriendStats == "Add Friend")
						{
							IsClan = 2;
							CustomMessageBox("Would you like to send\n a friend request to " + nickname, 2, Msgbox::FriendRequest);
						}
						else if (SelectedCommunityFriendStats == "Delete a Friend")
						{
							IsClan = 2;
							CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::DeleteFriendRequest);
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, SelectedCommunityBlockStats.c_str(), NULL, false))
					{
						IsClan = 2;

						if (SelectedCommunityBlockStats == "Block Chat")
						{
							CustomMessageBox("Would you like to block\n a  " + nickname, 2, Msgbox::BlockChatReqeust);
						}
						else
						{
							CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::RemoveBlockChatReqeust);
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Send a MSG", NULL))
					{
						uintptr_t MsgName = Mem.FindOffAAddy(dwBaseAddr + 0x0173D7A4, { 0x1C, 0x4, 0x0, 0x0, 0x0, 0x8, 0x14, 0x10, 0x2AC, 0x4 });
						WriteProcessMemory(GetCurrentProcess(), (LPVOID)MsgName, nickname.c_str(), sizeof(nickname), 0);
						S4Gui(S4_Screen::Display_SendMSG);
					}

					if (SelectedCommunity == 1)
					{
						if (MyClanRank == "Master")
						{
							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Change Rank", NULL))
							{
								NewClanRankChangeUI = true;
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Transfer Master", NULL))
							{
								CustomMessageBox("Would you like to trasfer master\n to " + nickname + "?", 2, Msgbox::ClanTransferMaster);
							}
						}
						if (MyClanRank == "Master" || MyClanRank == "CoMaster")
						{
							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Ban", NULL))
							{
								CustomMessageBox("Would you like to ban \n" + nickname + "from the clan?", 2, Msgbox::ClanBan);
							}
						}

						if (MyClanRank == "Master" || MyClanRank == "CoMaster" || MyClanRank == "Staff")
						{
							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Kick", NULL))
							{
								CustomMessageBox("Would you like to kick \n" + nickname + "from the clan?", 2, Msgbox::ClanKick);
							}
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Exchange", NULL))
					{
						strcpy_s(ExcFindPlayer, nickname.length() + 1, nickname.c_str());
						CustomMessageBox("Would you like to start exchange\n with " + nickname + "?", 2, Msgbox::ExchangeRequest);
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Challenge", NULL))
					{
						strcpy_s(ChalFindPlayer, nickname.length() + 1, nickname.c_str());
						MenuSelected = 8;
						CMainMenuUI = true;
					}
					Legion::EndPopup();
				}
			}
			Legion::EndChild();

			Legion::EndChild();

			//	
			Legion::IndentEx(100, 10);
			Legion::TextLabelColor(CalibriFont, 18, "{c1ccdd}COMMUNITY");
			Legion::IndentEx(157, -8);
			if (Legion::ImageButtonEx("ADD Friend", "{c1ccdd}+", true, ImColor(8, 137, 196), ImColor(219, 146, 59), ImColor(219, 146, 59), ImVec2(25, 25), CalibriFont, 18.4f))
			{
				NewAddFriendUI = true;
			}
			Legion::End();
		}

		if (NewAddFriendUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Add Friend", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &NewAddFriendUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewAddFriendUI = false;
			Legion::IndentEx(-195, -15);
			Legion::TextLabelColor(CalibriFont, 18, "{ffff}PLAYER NAME");
			Legion::IndentEx(-80, 9);
			Legion::InputTextV("PLAYER NAME Box", CalibriFont, 11, SmallBoxTexture, "", AddFriendName, IM_ARRAYSIZE(AddFriendName), ImVec2(287, 30));
			Legion::IndentEx(85, 12);
			if (Legion::ImageButtonEx("APPLY Button", "ADD", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				IsClan = 2;
				CustomMessageBox("Would you like to send\n a friend request to " + SelectedCommunityNick, 2, Msgbox::FriendRequest);
				NewAddFriendUI = false;
			}
		}

		if (ChatButtonMenuLock)
		{
			Legion::SetNextWindowPos(ImVec2(ChatBoxPosX, ChatBoxPosY + (ChatBoxHeight - 140)), 0, ImVec2(0.0f, 0.0f));
			Legion::BeginEx(NULL, NULL, NULL, "Chat Box Menu Buttons", ImVec2(100, 111), ImVec2(0, 0), &ChatButtonMenuLock, LegionWindowFlags_Custom);

			if (Legion::ImageButtonV("SUPPORT", ChatBtn4MenuName, true, &ChatButton, 5, ChatMenuButton4Texture, ChatMenuButton4Texture, ChatMenuButton4Texture, ImVec2(50, 23), CalibriFont, 12, ImVec2(0.5f, 0.5f)))
			{
				if (Chat_type == Chat_Type::Support)
					Chat_type = Chat_Type::All;
				else
					Chat_type = Chat_Type::Support;

				ChatButtonTexture(Chat_type);

				Legion::SetKeyboardFocusHere(-1, "Chat System");
				ChatButtonMenuLock = false;
			}
			Legion::IndentEx(0.0f, -8);
			if (Legion::ImageButtonV("WHISPER", ChatBtn3MenuName, true, &ChatButton, 4, ChatMenuButton3Texture, ChatMenuButton3Texture, ChatMenuButton3Texture, ImVec2(50, 23), CalibriFont, 12, ImVec2(0.5f, 0.5f)))
			{
				if (Chat_type == Chat_Type::Whisper)
					Chat_type = Chat_Type::All;
				else
					Chat_type = Chat_Type::Whisper;

				ChatButtonTexture(Chat_type);

				Legion::SetKeyboardFocusHere(-1, "Chat System");
				ChatButtonMenuLock = false;
			}
			Legion::IndentEx(0.0f, -8);
			if (Legion::ImageButtonV("UNION", ChatBtn2MenuName, true, &ChatButton, 3, ChatMenuButton2Texture, ChatMenuButton2Texture, ChatMenuButton2Texture, ImVec2(50, 23), CalibriFont, 12, ImVec2(0.5f, 0.5f)))
			{
				if (Chat_type == Chat_Type::Union)
					Chat_type = Chat_Type::All;
				else
					Chat_type = Chat_Type::Union;

				ChatButtonTexture(Chat_type);

				Legion::SetKeyboardFocusHere(-1, "Chat System");
				ChatButtonMenuLock = false;
			}
			Legion::IndentEx(0.0f, -8);
			if (Legion::ImageButtonV("CLAN", ChatBtnMenuName, true, &ChatButton, 2, ChatMenuButtonTexture, ChatMenuButtonTexture, ChatMenuButtonTexture, ImVec2(50, 23), CalibriFont, 12, ImVec2(0.5f, 0.5f)))
			{
				if (Chat_type == Chat_Type::Clan)
					Chat_type = Chat_Type::All;
				else
					Chat_type = Chat_Type::Clan;

				ChatButtonTexture(Chat_type);

				Legion::SetKeyboardFocusHere(-1, "Chat System");
				ChatButtonMenuLock = false;
			}

			Legion::End();
		}

		if (CChatSystemUI)
		{
			LegionIO& io = Legion::GetIO();
			ImVec2 ScreenSize = io.DisplaySize;

			if (ScreenRaito() == "4:3")
			{
				ChatBoxWidth = ScreenSize.x * 0.6452f;
				ChatBoxHeight = ScreenSize.y * 0.2813f;
				ChatBoxPosX = ScreenSize.x * 0.343f;
				ChatBoxPosY = ScreenSize.y * 0.695370f;
			}
			else if (ScreenRaito() == "16:9")
			{
				ChatBoxWidth = ScreenSize.x * 0.484f;
				ChatBoxHeight = ScreenSize.y * 0.282f;
				ChatBoxPosX = ScreenSize.x * 0.383f;
				ChatBoxPosY = ScreenSize.y * 0.694f;
			}
			else if (ScreenRaito() == "16:10")
			{
				ChatBoxWidth = ScreenSize.x * 0.534f;
				ChatBoxHeight = ScreenSize.y * 0.281f;
				ChatBoxPosX = ScreenSize.x * 0.370f;
				ChatBoxPosY = ScreenSize.y * 0.696f;
			}
			else if (ScreenRaito() == "5:4")
			{
				ChatBoxWidth = ScreenSize.x * 0.643f;
				ChatBoxHeight = ScreenSize.y * 0.281f;
				ChatBoxPosX = ScreenSize.x * 0.343f;
				ChatBoxPosY = ScreenSize.y * 0.695f;
			}
			else if (ScreenRaito() == "5:3")
			{
				ChatBoxWidth = ScreenSize.x * 0.518f;
				ChatBoxHeight = ScreenSize.y * 0.281f;
				ChatBoxPosX = ScreenSize.x * 0.373f;
				ChatBoxPosY = ScreenSize.y * 0.695f;
			}

			Legion::SetNextWindowPos(ImVec2(ChatBoxPosX, ChatBoxPosY), 0, ImVec2(0.0f, 0.0f));
			Legion::BeginEx(NULL, NULL, NULL, "Chat System", ImVec2(ChatBoxWidth, ChatBoxHeight), ImVec2(0, 0), &CChatSystemUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			Legion::IndentEx(-8.0f, -8);
			Legion::BeginChildV(ChatBoxTexture, NULL, NULL, "Chat Box System", ImVec2(ChatBoxWidth - 15, ChatBoxHeight - 25));
			{
				Legion::IndentEx(0, 3);
				Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "ChatBox", ImVec2(ChatBoxWidth - ChatBoxWidth * 0.040f, ChatBoxHeight - ChatBoxHeight * 0.2f));
				Legion::IndentEx(15, 10);
				if (Chat_type == Chat_Type::All)
					for (std::pair<unsigned int, ChatSystemStruc*> it : ChatSystemList)
						Legion::SelectableV(Ebrima, 13, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(ChatBoxWidth - ChatBoxWidth * 0.040f, ChatBoxHeight - ChatBoxHeight * 0.2f));
				else if (Chat_type == Chat_Type::Clan)
					for (std::pair<unsigned int, ChatSystemStruc*> it : ClanSystemList)
						Legion::SelectableV(Ebrima, 13, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(ChatBoxWidth - ChatBoxWidth * 0.029f, ChatBoxHeight - ChatBoxHeight * 0.2f));
				else if (Chat_type == Chat_Type::Union)
					for (std::pair<unsigned int, ChatSystemStruc*> it : ClanUnionSystemList)
						Legion::SelectableV(Ebrima, 13, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(ChatBoxWidth - ChatBoxWidth * 0.029f, ChatBoxHeight - ChatBoxHeight * 0.2f));
				else if (Chat_type == Chat_Type::Whisper)
					for (std::pair<unsigned int, ChatSystemStruc*> it : WhisperSystemList)
						Legion::SelectableV(Ebrima, 13, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(ChatBoxWidth - ChatBoxWidth * 0.029f, ChatBoxHeight - ChatBoxHeight * 0.2f));
				else if (Chat_type == Chat_Type::Support)
					for (std::pair<unsigned int, ChatSystemStruc*> it : SupportSystemList)
						Legion::SelectableV(Ebrima, 13, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(ChatBoxWidth - ChatBoxWidth * 0.029f, ChatBoxHeight - ChatBoxHeight * 0.2f));

				Legion::EndChild();

				if (ChatAutotScroll && Legion::GetScrollY("ChatBox") >= Legion::GetScrollMaxY("ChatBox"))
				{
					//Legion::SetScrollHereY(1.0f);
					Legion::SetScrollHereVY(1.0f, "ChatBox");
					ChatAutotScroll = false;
				}
			}
			Legion::EndChild();


#pragma region HotKeys
			LegionInputTextState& edit_state = Legion::GetCurrentContext()->InputTextState;

			if (GetAsyncKeyState(VK_RETURN) && GetFocus())
			{
				Legion::SetWindowFocus("Chat System");
				Legion::SetKeyboardFocusHere();
			}

			if (GetAsyncKeyState(VK_F1) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Whisper);
				Legion::SetActiveID(0, Legion::GetCurrentWindow());
				if (strlen(ChatSystemMessage) > 0)
				{
					memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}
				std::string normalmsg = "To ";
				if (!LastWhisperNickname.empty())
					std::copy(LastWhisperNickname.begin(), LastWhisperNickname.end(), ChatSystemMessage);
				else
					std::copy(normalmsg.begin(), normalmsg.end(), ChatSystemMessage);

				int n = edit_state.TextW.size();
				edit_state.Stb.cursor = n; //stb_text_locate_coord(str, x, y);
				edit_state.Stb.select_start = edit_state.Stb.cursor;
				edit_state.Stb.select_end = edit_state.Stb.cursor;
				edit_state.Stb.has_preferred_x = 0;
				edit_state.CursorAnimReset();

				//edit_state.Stb.cursor = edit_state.CurLenW;
				Legion::SetWindowFocus("Chat System");
				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F2) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::All);

				if (strlen(ChatSystemMessage) > 0)
				{
					memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F3) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Clan);

				if (strlen(ChatSystemMessage) > 0)
				{
					memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F4) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Union);

				if (strlen(ChatSystemMessage) > 0)
				{
					memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F5) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Support);

				if (strlen(ChatSystemMessage) > 0)
				{
					//	memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Chat System");

				Legion::SetKeyboardFocusHere();
			}
#pragma endregion

			Legion::IndentEx(10, -2);
			int ChatTextBox = Legion::InputTextWithHintExV("ChatTextBox", CalibriFont, 11, ChatTextBoxTexture, "", "....", ChatSystemMessage, IM_ARRAYSIZE(ChatSystemMessage), ImVec2(ChatBoxWidth - 25.8f, 18.5f), ChatMainButtonTexture, ChatMenuName, ImVec2(50, 23), LegionInputTextFlags_EnterReturnsTrue);

			if (ChatTextBox == 3)
				ChatButtonMenuLock ^= 1;
			else if (ChatTextBox == 2)
			{
				if (strlen(ChatSystemMessage) >= 0)
				{
					SendInfoToServer("c4d4c01a43", std::to_string((DWORD)Chat_type), ChatSystemMessage);

					if (strncmp("To ", ChatSystemMessage, 3) == 0 && strstr(ChatSystemMessage, " : "))
						LastWhisperNickname = "To " + Mem.get_str_between_str(ChatSystemMessage, "To ", " : ") + " : ";

					memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);

					if (Chat_type == Chat_Type::Whisper)
					{
						std::copy(LastWhisperNickname.begin(), LastWhisperNickname.end(), ChatSystemMessage);
					}
				}

				if (!Legion::IsAnyItemActive() && !Legion::IsMouseClicked(0))
					Legion::SetKeyboardFocusHere(-1);
			}
			Legion::End();
		}

		if (CPlayerListUI)
		{
			LegionIO& io = Legion::GetIO();
			ImVec2 ScreenSize = io.DisplaySize;
			//	Legion::SetNextWindowPos(ImVec2(ChatBoxPosX + (ChatBoxWidth), ChatBoxPosY + (ChatBoxHeight - 140)), 0, ImVec2(0.0f, 0.0f));
			Legion::SetNextWindowPos(ImVec2(ScreenSize.x * 0.8f, ScreenSize.y * 0.8f), LegionCond_FirstUseEver, ImVec2(0.8f, 0.8f));
			Legion::BeginEx(PlayerListUITexture, NULL, NULL, "Player List", ImVec2(239, ScreenSize.y * 0.5f /*ChatBoxHeight * 3*/), ImVec2(0.0f, 0.0f), &CPlayerListUI, LegionWindowFlags_NoBackground | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoResize | LegionWindowFlags_NoCollapse | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoScrollWithMouse);
			Legion::IndentEx(15, 10);
			Legion::BeginChildV(PlayerListBoxTexture, ScrollTexture, ScrollBarTexture, "Player List Group", ImVec2(195, ScreenSize.y * 0.45f/*ChatBoxHeight * 2.2f*/));

			if (Chat_type != Chat_Type::Support)
			{
				if (!MyNickName.empty())
				{
					if (Legion::PlayerList(MyNickName.c_str(), CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(MyLevel, 0), CPlayerListTexture(RankPointName(MyRankPoint), 1)))
						CSelectedPlayer = MyNickName;
				}
			}

			if (Chat_type == Chat_Type::All)
			{
				if (ChannelPlayerList.size() > 0)
				{
					for (std::pair<unsigned int, NewPlayerList*> it : ChannelPlayerList)
					{
						if (Legion::PlayerList(it.second->Nickname, CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
						{
							CSelectedPlayerId = it.first;
							CSelectedPlayer = it.second->Nickname;
							CSelectedPlayerFriendStats = it.second->FriendStats;
							CSelectedPlayerBlockStats = it.second->BlockStats;
						}
					}
				}
			}

			if (Chat_type == Chat_Type::Clan)
			{
				if (ClanMemberList.size() > 0)
				{
					for (std::pair<unsigned int, NewPlayerList*> it : ClanMemberList)
					{
						if (Legion::PlayerList(it.second->Nickname, CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
						{
							CSelectedPlayerId = it.first;
							CSelectedPlayer = it.second->Nickname;
							CSelectedPlayerFriendStats = it.second->FriendStats;
							CSelectedPlayerBlockStats = it.second->BlockStats;
						}
					}
				}
			}

			if (Chat_type == Chat_Type::Union)
			{
				if (UnionMemberList.size() > 0)
				{
					for (std::pair<unsigned int, NewPlayerList*> it : UnionMemberList)
					{
						if (Legion::PlayerList(it.second->Nickname, CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
						{
							CSelectedPlayerId = it.first;
							CSelectedPlayer = it.second->Nickname;
							CSelectedPlayerFriendStats = it.second->FriendStats;
							CSelectedPlayerBlockStats = it.second->BlockStats;
						}
					}
				}
			}

			if (Chat_type == Chat_Type::Whisper)
			{
				if (WhisperPlayerList.size() > 0)
				{
					for (std::pair<unsigned int, NewPlayerList*> it : WhisperPlayerList)
					{
						if (Legion::PlayerList(it.second->Nickname, CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
						{
							CSelectedPlayerId = it.first;
							CSelectedPlayer = it.second->Nickname;
							CSelectedPlayerFriendStats = it.second->FriendStats;
							CSelectedPlayerBlockStats = it.second->BlockStats;
						}
					}
				}
			}

			if (Chat_type == Chat_Type::Support)
			{
				if (SupportPlayerList.size() > 0)
				{
					for (std::pair<unsigned int, NewPlayerList*> it : SupportPlayerList)
					{
						if (Legion::PlayerList(it.second->Nickname, CalibriFont, 12, PlayerBackGroundTexture, CPlayerListTexture(it.second->Level, 0), it.second->Rank))
						{
							CSelectedPlayerId = it.first;
							CSelectedPlayer = it.second->Nickname;
							CSelectedPlayerFriendStats = it.second->FriendStats;
							CSelectedPlayerBlockStats = it.second->BlockStats;
						}
					}
				}
			}

			if (CSelectedPlayer != MyNickName && !CSelectedPlayer.empty())
			{
				if (Legion::BeginPopupContextWindowV(ContextTexture, NULL, NULL, "Player List Context", LegionMouseButton_Right))
				{
					std::string nickname;
					if (CSelectedPlayer.find("{") != std::string::npos)
						nickname = Mem.Split(CSelectedPlayer, '}')[1];
					else
						nickname = CSelectedPlayer;

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "UserInfo", NULL, false))
					{
						UserInfo_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0);
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Whisper", NULL, false))
					{
						ChatButtonTexture(Chat_Type::Whisper);

						memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);

						std::string whisper;
						if (CSelectedPlayer.find("{") != std::string::npos)
							whisper = "To " + Mem.Split(CSelectedPlayer, '}')[1] + " : "/*+ " : "*/;
						else
							whisper = "To " + Mem.Split(CSelectedPlayer, '}')[0] + " : "/*+ " : "*/;

						// = "To " + CSelectedPlayer + " : ";
						std::copy(whisper.begin(), whisper.end(), ChatSystemMessage);

						LegionInputTextState& edit_state = Legion::GetCurrentContext()->InputTextState;

						int n = edit_state.TextW.size();
						edit_state.Stb.cursor = n; //stb_text_locate_coord(str, x, y);
						edit_state.Stb.select_start = edit_state.Stb.cursor;
						edit_state.Stb.select_end = edit_state.Stb.cursor;
						edit_state.Stb.has_preferred_x = 0;
						edit_state.CursorAnimReset();

						//  to do add auto fouce bool
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, CSelectedPlayerFriendStats.c_str(), NULL))
					{
						if (CSelectedPlayerFriendStats == "Add Friend")
						{
							IsClan = 1;
							CustomMessageBox("Would you like to send\n a friend request to " + nickname, 2, Msgbox::FriendRequest);
						}
						else if (CSelectedPlayerFriendStats == "Delete a Friend")
						{
							IsClan = 1;
							CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::DeleteFriendRequest);
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, CSelectedPlayerBlockStats.c_str(), NULL, false))
					{
						if (CSelectedPlayerBlockStats == "Block Chat")
						{
							CustomMessageBox("Would you like to block\n a  " + nickname, 2, Msgbox::BlockChatReqeust);
						}
						else
						{
							CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::RemoveBlockChatReqeust);
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Send a MSG", NULL, false))
					{
						uintptr_t MsgName = Mem.FindOffAAddy(dwBaseAddr + 0x0173D7A4, { 0x1C, 0x4, 0x0, 0x0, 0x0, 0x8, 0x14, 0x10, 0x2AC, 0x4 });
						WriteProcessMemory(GetCurrentProcess(), (LPVOID)MsgName, nickname.c_str(), sizeof(nickname), 0);
						S4Gui(S4_Screen::Display_SendMSG);
					}

					if (MyClanRank == "Master" || MyClanRank == "CoMaster")
					{
						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Invite", NULL))
						{
							CustomMessageBox("Would you like to invite '" + nickname + "'\n to your clan?", 2, Msgbox::ClanInvite);
						}
					}

					if (MyClanRank == "Master" || MyClanRank == "CoMaster" || MyClanRank == "Staff")
					{
						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Kick", NULL))
						{
							CustomMessageBox("Would you like to kick \n" + nickname + "from the clan?", 2, Msgbox::ClanKick);
						}
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Exchange", NULL))
					{

						strcpy_s(ExcFindPlayer, nickname.length() + 1, nickname.c_str());
						CustomMessageBox("Would you like to start exchange\n with " + nickname + "?", 2, Msgbox::ExchangeRequest);
					}

					if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Challenge", NULL))
					{
						strcpy_s(ChalFindPlayer, nickname.length() + 1, nickname.c_str());
						MenuSelected = 8;
						CMainMenuUI = true;
						//CustomMessageBox("Would you like to challenge " + ClanSelectedPlayerNickName + "?", 2, Msgbox::ChallengeRequst);
					}
				}
			}
			Legion::EndChild();
			Legion::End();
		}

		if (CChannelUI)
		{
			ImVec2 ScreenSize = Legion::GetIO().DisplaySize;

			float sizex;
			float sizey;

			float bsizey;

			if (ScreenSize.x == 800)
			{
				sizex = 549.36f;
				sizey = 90.72f;

				//bsizex = 732.96f;
				bsizey = 43.2f;
			}
			else if (ScreenSize.x == 1024 && ScreenSize.y == 600)
			{
				sizex = 703;
				sizey = 90.72f;

				bsizey = 43;
			}
			else if (ScreenSize.x == 1024 && ScreenSize.y == 768)
			{
				sizex = 703;
				sizey = 116;

				bsizey = 55;
			}
			else if (ScreenSize.x == 1152)
			{
				sizex = 791;
				sizey = 130;

				bsizey = 61;
			}
			else if (ScreenSize.x == 1176)
			{
				sizex = 808;
				sizey = 100;

				bsizey = 48;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 600)
			{
				sizex = 878;
				sizey = 90.72f;

				bsizey = 43;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 720)
			{
				sizex = 878;
				sizey = 108;

				bsizey = 50;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 768)
			{
				sizex = 878;
				sizey = 116;

				bsizey = 55;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 800)
			{
				sizex = 878;
				sizey = 121;

				bsizey = 57;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 960)
			{
				sizex = 878;
				sizey = 145;

				bsizey = 68;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 1024)
			{
				sizex = 878;
				sizey = 154;

				bsizey = 73;
			}
			else if (ScreenSize.x == 1344 && ScreenSize.y == 840)
			{
				sizex = 878;
				sizey = 154;

				bsizey = 73;
			}
			else if (ScreenSize.x == 1360)
			{
				sizex = 934;
				sizey = 116;

				bsizey = 55;
			}
			else if (ScreenSize.x == 1366)
			{
				sizex = 938;
				sizey = 116;

				bsizey = 55;
			}
			else if (ScreenSize.x == 1440 && ScreenSize.y == 900)
			{
				sizex = 988;
				sizey = 136;

				bsizey = 63;
			}
			else if (ScreenSize.x == 1536 && ScreenSize.y == 864)// new
			{
				sizex = 1058;
				sizey = 136;

				bsizey = 63;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 900)
			{
				sizex = 1098;
				sizey = 136;

				bsizey = 63;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 1024)
			{
				sizex = 1098;
				sizey = 154;

				bsizey = 73;
			}
			else if (ScreenSize.x == 1680 && ScreenSize.y == 1050)
			{
				sizex = 1153;
				sizey = 158;

				bsizey = 74;
			}
			else if (ScreenSize.x >= 1920 && ScreenSize.x < 2560)
			{
				sizex = 384;
				sizey = 960;

				//bsizey = 75;
			}
			/*	else if (ScreenSize.x == 1920 && ScreenSize.y == 1048)
				{
					sizex = 1317;
					sizey = 162;

					bsizey = 75;
				}
				else if (ScreenSize.x == 2560 && ScreenSize.y == 1440)
				{
					sizex = 1517;
					sizey = 172;

					bsizey = 80;
				}*/
			else if (ScreenSize.x >= 2560 /*&& ScreenSize.y == 1080*/)
			{
				sizex = 1517;
				sizey = 172;

				bsizey = 80;
			}
			//Legion::BeginEx(NULL, NULL, NULL, "Channel List", ImVec2(sizex + 10, (sizey * 4) + bsizey), ImVec2(0.5f, 0.5f), &CChannelUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			Legion::BeginEx(MainBGTexture, NULL, NULL, "Channel List", ImVec2(ScreenSize.x + 10, ScreenSize.y + 10), ImVec2(0.49f, 0.49f), &CChannelUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);

			//Legion::IndentEx((Legion::GetWindowSize().x / 2) - sizex / 2, ((sizey * 4) + bsizey) * 0.2f);
			//Legion::IndentEx((Legion::GetWindowSize().x / 2) - sizex / 2/*, ((sizey * 1) + bsizey) * 0.1f */);
			//Legion::ImageButtonEx("BORED", "Bored", false, ChannelBoredTexture, ChannelBoredTexture, ChannelBoredTexture, ImVec2(sizex, bsizey), CalibriFont, 12);


			Legion::IndentEx(-26, 0);
			Legion::IndentEx(Legion::GetWindowSize().x / 5);
			if (Legion::ImageButtonEx("CLAN WAR", "CLAN WAR", false, ClanBattelsTexture, ClanBattelsOnTexture, ClanBattelsOnTexture, ImVec2(sizex, sizey), CalibriFont, 12))
				Enter_Channel_Req(4);
			/*Legion::SameLineEx(0, 0);
			//Legion::IndentEx(0, -10);
			if (Legion::ImageButtonEx("COMPETITIVE", "COMPETITIVE", false, CompetitiveTexture, CompetitiveOnTexture, CompetitiveOnTexture, ImVec2(sizex, sizey), CalibriFont, 12))
				Enter_Channel_Req(3); */

				//Legion::IndentEx(-10, 0);
			Legion::SameLineEx(0, 0);
			if (Legion::ImageButtonEx("Free", "Free", false, ChannelFreeTexture, ChannelFreeOnTexture, ChannelFreeOnTexture, ImVec2(sizex, sizey), CalibriFont, 12))
				Enter_Channel_Req(2);

			//Legion::IndentEx(-10, 0);
			Legion::SameLineEx(0, 0);
			if (Legion::ImageButtonEx("RANKED", "RANKED", false, RankChallengeTexture, RankChallengeOnTexture, RankChallengeOnTexture, ImVec2(sizex, sizey), CalibriFont, 12))
				Enter_Channel_Req(5);

			/*Legion::IndentEx(sizex - 100, 20);

			if (Legion::ImageButtonEx("EXIT", "EXIT", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(135, 45), CalibriFont, 16))
				CustomMessageBox("Do you want to Exit?", 2, Msgbox::ExitGame);*/

			Legion::End();
		}

		/*if (CRoomChatSystemUI)
		{

			LegionIO& io = Legion::GetIO();
			ImVec2 ScreenSize = io.DisplaySize;

			if (ScreenSize.x == 800)
			{
				RoomChatBoxWidth = 278;
				RoomChatBoxHeight = 260;
			}
			else if (ScreenSize.x == 1024)
			{
				RoomChatBoxWidth = 356;
				RoomChatBoxHeight = 333;
			}
			else if (ScreenSize.x == 1152)
			{
				RoomChatBoxWidth = 400;
				RoomChatBoxHeight = 375;
			}
			else if (ScreenSize.x == 1176)
			{
				RoomChatBoxWidth = 409;
				RoomChatBoxHeight = 288;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 720)
			{
				RoomChatBoxWidth = 445;
				RoomChatBoxHeight = 312;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 768)
			{
				RoomChatBoxWidth = 445;
				RoomChatBoxHeight = 333;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 800)
			{
				RoomChatBoxWidth = 445;
				RoomChatBoxHeight = 347;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 960)
			{
				RoomChatBoxWidth = 445;
				RoomChatBoxHeight = 416;
			}
			else if (ScreenSize.x == 1280 && ScreenSize.y == 1024)
			{
				RoomChatBoxWidth = 445;
				RoomChatBoxHeight = 444;
			}
			else if (ScreenSize.x == 1360)
			{
				RoomChatBoxWidth = 437;
				RoomChatBoxHeight = 333;
			}
			else if (ScreenSize.x == 1366)
			{
				RoomChatBoxWidth = 475;
				RoomChatBoxHeight = 333;
			}
			else if (ScreenSize.x == 1440 && ScreenSize.y == 900)
			{
				RoomChatBoxWidth = 501;
				RoomChatBoxHeight = 390;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 900)
			{
				RoomChatBoxWidth = 556;
				RoomChatBoxHeight = 390;
			}
			else if (ScreenSize.x == 1600 && ScreenSize.y == 1024)
			{
				RoomChatBoxWidth = 556;
				RoomChatBoxHeight = 444;
			}
			else if (ScreenSize.x == 1680 && ScreenSize.y == 1050)
			{
				RoomChatBoxWidth = 584;
				RoomChatBoxHeight = 455;
			}
			else if (ScreenSize.x == 1920)
			{
				RoomChatBoxWidth = 668;
				RoomChatBoxHeight = 469;
			}

			Legion::BeginEx(NULL, NULL, NULL, "Room Chat System", ImVec2(RoomChatBoxWidth, RoomChatBoxHeight), ImVec2(0.001f, 0.45f), &CRoomChatSystemUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			Legion::IndentEx(-8.0f, -8);
			Legion::BeginChildV(RoomChatBoxTexture, NULL, NULL, "Room Chat Box System", ImVec2(RoomChatBoxWidth - 15, RoomChatBoxHeight - 25));
			{
				Legion::IndentEx(15, 10);
				if (RoomChat_type == Chat_Type::All)
					for (std::pair<unsigned int, ChatSystemStruc*> it : RoomChatSystemList)
						Legion::SelectableV(CalibriFont, 12, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(RoomChatBoxWidth - RoomChatBoxWidth * 0.040f, RoomChatBoxHeight - RoomChatBoxHeight * 0.2f));
				else if (RoomChat_type == Chat_Type::Team)
					for (std::pair<unsigned int, ChatSystemStruc*> it : TeamSystemList)
						Legion::SelectableV(CalibriFont, 12, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(RoomChatBoxWidth - RoomChatBoxWidth * 0.029f, RoomChatBoxHeight - RoomChatBoxHeight * 0.2f));
				else if (RoomChat_type == Chat_Type::Clan)
					for (std::pair<unsigned int, ChatSystemStruc*> it : ClanSystemList)
						Legion::SelectableV(CalibriFont, 12, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(RoomChatBoxWidth - RoomChatBoxWidth * 0.029f, RoomChatBoxHeight - RoomChatBoxHeight * 0.2f));
				else if (RoomChat_type == Chat_Type::Union)
					for (std::pair<unsigned int, ChatSystemStruc*> it : ClanUnionSystemList)
						Legion::SelectableV(CalibriFont, 12, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(RoomChatBoxWidth - RoomChatBoxWidth * 0.029f, RoomChatBoxHeight - RoomChatBoxHeight * 0.2f));
				else if (RoomChat_type == Chat_Type::Whisper)
					for (std::pair<unsigned int, ChatSystemStruc*> it : WhisperSystemList)
						Legion::SelectableV(CalibriFont, 12, it.second->Time.c_str(), it.second->Nickname, it.second->Message, ImVec2(RoomChatBoxWidth - RoomChatBoxWidth * 0.029f, RoomChatBoxHeight - RoomChatBoxHeight * 0.2f));

				if (RoomChatAutotScroll)
				{
					Legion::SetScrollHereVY(1.0f, "ChatBox");
					RoomChatAutotScroll = false;
				}

			}
			Legion::EndChild();

	#pragma region HotKeys
			LegionInputTextState& edit_state = Legion::GetCurrentContext()->InputTextState;

			//if (GetAsyncKeyState(VK_RETURN) && GetFocus())
			//{
				Legion::SetWindowFocus("Room Chat System");
				Legion::SetKeyboardFocusHere();
			//}

			if (GetAsyncKeyState(VK_F1) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Whisper);
				Legion::SetActiveID(0, Legion::GetCurrentWindow());
				if (strlen(RoomChatSystemMessage) > 0)
				{
					memset(RoomChatSystemMessage, 0, sizeof RoomChatSystemMessage);
					edit_state.ClearText();
				}
				std::string normalmsg = "To ";
				if (!LastWhisperNickname.empty())
					std::copy(LastWhisperNickname.begin(), LastWhisperNickname.end(), RoomChatSystemMessage);
				else
					std::copy(normalmsg.begin(), normalmsg.end(), RoomChatSystemMessage);

				Legion::SetWindowFocus("Room Chat System");
				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F2) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::All);

				if (strlen(RoomChatSystemMessage) > 0)
				{
					//	memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Room Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F3) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Team);

				if (strlen(RoomChatSystemMessage) > 0)
				{
					//	memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Room Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F4) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Clan);

				if (strlen(RoomChatSystemMessage) > 0)
				{
					//	memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Room Chat System");

				Legion::SetKeyboardFocusHere();
			}
			else if (GetAsyncKeyState(VK_F5) && GetFocus())
			{
				ChatButtonTexture(Chat_Type::Union);

				if (strlen(RoomChatSystemMessage) > 0)
				{
					//	memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);
					edit_state.ClearText();
				}

				Legion::SetWindowFocus("Room Chat System");

				Legion::SetKeyboardFocusHere();
			}
	#pragma endregion

			Legion::IndentEx(9, 0);

			if (!IsTextBox)
			{
				int ChatTextBox = Legion::InputTextWithHintExV("RoomChatTextBox", CalibriFont, 11, ChatTextBoxTexture, "", "Hints", RoomChatSystemMessage, IM_ARRAYSIZE(RoomChatSystemMessage), ImVec2(ChatBoxWidth - 24.8f, 20), RoomChatMainButtonTexture, RoomChatMenuName, ImVec2(50, 23), LegionInputTextFlags_EnterReturnsTrue);

				if (ChatTextBox == 2)
				{
					if (strlen(RoomChatSystemMessage) >= 0)
					{
						SendInfoToServer("c1b9354a25", std::to_string((DWORD)RoomChat_type), RoomChatSystemMessage);

						if (strncmp("To ", RoomChatSystemMessage, 3) == 0 && strstr(RoomChatSystemMessage, " : "))
							LastWhisperNickname = "To " + Mem.get_str_between_str(RoomChatSystemMessage, "To ", " : ") + " : ";

						memset(RoomChatSystemMessage, 0, sizeof RoomChatSystemMessage);

						if (RoomChat_type == Chat_Type::Whisper)
						{
							std::copy(LastWhisperNickname.begin(), LastWhisperNickname.end(), RoomChatSystemMessage);
						}

					//	IsTextBox = true;
					}

					//if (!Legion::IsAnyItemActive() && !Legion::IsMouseClicked(0))
					Legion::SetKeyboardFocusHere(-1);
				}
			}

			Legion::End();
		}*/

		if (CPlayerReportUI)
		{
			Legion::BeginEx(ReportPlayerUITexture, NULL, NULL, "Player List", ImVec2(234, 369), ImVec2(0.5f, 0.5f), &CPlayerReportUI, LegionWindowFlags_Custom);
			Legion::IndentEx(198, -8);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(21, 21), CalibriFont, 11))
			{
				Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				CPlayerReportUI = false;
			}
			Legion::IndentEx(-229, 17);
			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Player List Group", ImVec2(234, 270));
			Legion::IndentEx(50, 2);
			for (std::pair<unsigned int, PlayerShortInfoListClass*> it : ReportPlayerList)
			{
				if (Legion::ReportPlayer(it.second->Name, CalibriFont, 12, &RPeportPlayerID, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0)))
				{
					RSelectedPlayerId = it.second->PlayerId;
				}
			}
			Legion::EndChild();
			Legion::IndentEx(65, 10);
			if (Legion::ImageButtonEx("REPORT", "REPORT", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(144, 24), CalibriFont, 15))
			{
				CPlayerReportUI = false;
				Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				SendInfoToServer("a1382cd6f7", "RPP", std::to_string(RSelectedPlayerId));
			}
			Legion::End();
		}

		if (CSurrendeMessageUI)
		{
			CSurrenderSystemUI = false;
			SVoteLock = false;
			Legion::BeginEx(NULL, NULL, NULL, "Surrender Button", ImVec2(430, 80), ImVec2(0.5f, 0.2f), &CSurrendeMessageUI, LegionWindowFlags_Custom | LegionWindowFlags_NoFocusOnAppearing);
			Legion::LabelEx("Surrender Message", SurrenderMessage, ImVec2(420, 70), CalibriFont, Legion::GetIO().DisplaySize.x * 0.0138f);
			Legion::End();
		}

		if (CSurrenderSystemUI)
		{
			if (SVoteTimer->elapsedSeconds() < 11.0)
				SCounter = 10 - SVoteTimer->elapsedSeconds();
			if (SCounter <= 0)
			{
				CSurrenderSystemUI = false;
				SVoteLock = false;
				SVoteTimer->stop();
			}

			Legion::BeginEx(EmptyBoxUITexture, NULL, NULL, "Surrender System", ImVec2(285, 128), ImVec2(0.05f, 0.4f), &CSurrenderSystemUI, LegionWindowFlags_Custom);
			Legion::LabelEx("Left Time", ("Time Left " + std::to_string(SCounter) + " SEC").c_str(), ImVec2(275, 20), CalibriFont, 13);
			Legion::LabelEx("Surrender Player", (SurrenderPlayerName + " is suggested to surrender").c_str(), ImVec2(265, 20), CalibriFont, 13);
			Legion::LabelEx("Surrender Players", ("Vote " + SMaxCounter + " people / Surrender : " + SVotedCounter).c_str(), ImVec2(265, 20), CalibriFont, 13);
			Legion::LabelEx("Surrender Info", "Surrender: Y/Refuse: N", ImVec2(265, 20), CalibriFont, 13);

			if (!SVoteLock)
			{
				if (GetAsyncKeyState(0x59) && GetFocus())
				{
					if (!SVoteLock)
					{
						SVoteLock = true;
						SendInfoToServer("true", "SSV", "b442f8f62f");
					}
				}

				if (GetAsyncKeyState(0x4E) && GetFocus())
				{
					if (!SVoteLock)
					{
						SVoteLock = true;
						SendInfoToServer("false", "SSV", "b442f8f62f");
					}
				}

			}
			Legion::End();
		}

		if (CAfkPlayerSystemUI)
		{
			if (AfkPlayerKick_Timer->elapsedSeconds() < 100)
				SCounter = 99 - AfkPlayerKick_Timer->elapsedSeconds();

			if (SCounter > 0 && SCounter < 60)
			{
				Legion::BeginEx(EmptyBoxUITexture, NULL, NULL, "Afk System", ImVec2(350, 20), ImVec2(0.5f, 0.5f), &CAfkPlayerSystemUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
				Legion::IndentEx(-1, -5);
				Legion::LabelEx("Afk Lobby", ("{c1ccdd}You're about to get kick out of room in " + std::to_string(SCounter) + " Sec").c_str(), ImVec2(340, 20), CalibriFont, 16);
				Legion::End();
			}
			if (SCounter <= 0)
			{
				CAfkPlayerSystemUI = false;
				LeaveRoom_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA68), 0);
				AfkPlayerKick_Timer->stop();
			}
		}

		if (AddNewProfileUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Add New Profile", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &AddNewProfileUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				AddNewProfileUI = false;
			Legion::IndentEx(-195, -15);
			Legion::TextLabelColor(CalibriFont, 18, "{ffff}PROFILE NAME");
			Legion::IndentEx(-80, 9);
			Legion::InputTextV("Profile Name Box", CalibriFont, 11, SmallBoxTexture, "", ProfileName, IM_ARRAYSIZE(ProfileName), ImVec2(287, 33), LegionInputTextFlags_Multiline);
			Legion::IndentEx(90, 17);
			if (Legion::ImageButtonEx("Add Button", "SAVE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("a4122dcbb5", "CPN", ProfileName);
				AddNewProfileUI = false;
			}
			Legion::End();
		}

		if (InventoryProfileButtonsUI)
		{
			ImVec2 ScreenSize = Legion::GetIO().DisplaySize;
			float width;
			float height;
			float savelocx;
			float savelocy;

			if (ScreenRaito() == "4:3")
			{
				width = ScreenSize.x * 0.145833f;
				height = ScreenSize.y * 0.0398148f;

				savelocx = ScreenSize.x * 0.01027083f;
				savelocy = ScreenSize.y * 0.853333f;
			}
			else if (ScreenRaito() == "16:9")
			{
				width = ScreenSize.x * 0.109375f;
				height = ScreenSize.y * 0.03148148f;

				savelocx = ScreenSize.x * 0.1355416f;
				savelocy = ScreenSize.y * 0.87592592f;
			}
			else if (ScreenRaito() == "16:10")
			{
				width = ScreenSize.x * 0.1197916f;
				height = ScreenSize.y * 0.037037f;

				savelocx = ScreenSize.x * 0.0927716f;
				savelocy = ScreenSize.y * 0.85692592f;
			}
			else if (ScreenRaito() == "5:3")
			{
				width = ScreenSize.x * 0.1176816f;
				height = ScreenSize.y * 0.037037f;

				savelocx = ScreenSize.x * 0.1070625f;
				savelocy = ScreenSize.y * 0.8559259f;
			}
			else if (ScreenRaito() == "5:4")
			{
				width = ScreenSize.x * 0.145833f;
				height = ScreenSize.y * 0.0398148f;

				savelocx = ScreenSize.x * 0.01027083f;
				savelocy = ScreenSize.y * 0.853333f;
			}

			Legion::SetNextWindowPos(ImVec2(savelocx, savelocy), 0, ImVec2(0.0f, 0.0f));
			Legion::BeginEx(NULL, NULL, NULL, "Inventory Profile", ImVec2(width * 2.2f, height + 15), ImVec2(0, 0), &InventoryProfileButtonsUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			if (Legion::ImageButtonEx("Save Button", "SAVE PROFILE", true, ProfileButtonTexture, ProfileButtonTexture, ProfileButtonTexture, ImVec2(width, height), CalibriFont, 12.4f))
			{
				AddNewProfileUI = true;
			}
			Legion::SameLine(width + 15);
			if (Legion::ImageButtonEx("View Button", "VIEW PROFILES", true, ProfileButtonTexture, ProfileButtonTexture, ProfileButtonTexture, ImVec2(width, height), CalibriFont, 12.4f))
			{
				SendInfoToServer("ac40cf9c0e", "CPV", "CPV");
			}
			Legion::End();
		}

		if (CharactersProfileUI)
		{
			Legion::BeginEx(CharactersProfileUITexture, NULL, NULL, "Characters Profile View", ImVec2(700, 420), ImVec2(0.5f, 0.5f), &CharactersProfileUI, LegionWindowFlags_Custom);
			Legion::IndentEx(664, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				CharactersProfileUI = false;
			Legion::IndentEx(-639, 73);
			Legion::ImageButtonV("CHARACTER 1", "CHARACTER 1", true, &SelectedChar, 0, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(80, 30), CalibriFont, 12.4f);
			Legion::SameLine(125);
			Legion::ImageButtonV("CHARACTER 2", "CHARACTER 2", true, &SelectedChar, 1, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(80, 30), CalibriFont, 12.4f);
			Legion::SameLine(217);
			Legion::ImageButtonV("CHARACTER 3", "CHARACTER 3", true, &SelectedChar, 2, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(80, 30), CalibriFont, 12.4f);

			Legion::IndentEx(0, 30);
			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Profile List", ImVec2(270, 130));
			for (std::pair<unsigned int, ProfileItemInfo*> it : ProfileItemInfoList)
			{
				if (Legion::ImageButtonVC(("Test " + std::to_string(it.first)).c_str(), it.second->Name.c_str(), "{c1c6cb}", "{ff7c47}", "{00bfff}", true, &SelectedCharProfile, it.first, NULL, NULL, NULL, ImVec2(270, 10), CalibriFont, 12))
				{
					CharProfileSelected = it.second->Name;
					CharactersProfileItemsTexture[0] = LoadTextrueEx(it.second->ImageName[0]);
					CharactersProfileItemsTexture[1] = LoadTextrueEx(it.second->ImageName[1]);
					CharactersProfileItemsTexture[2] = LoadTextrueEx(it.second->ImageName[2]);
					CharactersProfileItemsTexture[3] = LoadTextrueEx(it.second->ImageName[3]);
					CharactersProfileItemsTexture[4] = LoadTextrueEx(it.second->ImageName[4]);
					CharactersProfileItemsTexture[5] = LoadTextrueEx(it.second->ImageName[5]);
					CharactersProfileItemsTexture[6] = LoadTextrueEx(it.second->ImageName[6]);
					CharactersProfileItemsTexture[7] = LoadTextrueEx(it.second->ImageName[7]);
					CharactersProfileItemsTexture[8] = LoadTextrueEx(it.second->ImageName[8]);
					CharactersProfileItemsTexture[9] = LoadTextrueEx(it.second->ImageName[9]);
					CharactersProfileItemsTexture[10] = LoadTextrueEx(it.second->ImageName[10]);
					CharactersProfileItemsTexture[11] = LoadTextrueEx(it.second->ImageName[11]);
					std::copy(it.second->ItemName, it.second->ItemName + 12, ItemNames);
					std::copy(it.second->Effects, it.second->Effects + 12, EffectsInfo);
				}
			}
			Legion::EndChild();
			Legion::IndentEx(0, 16);
			if (Legion::ImageButtonEx("EQUIP", "EQUIP", true, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(127, 30), CalibriFont, 14.4f))
			{
				SendInfoToServer("0acd83bc2a", std::to_string(SelectedChar), CharProfileSelected);
			}
			Legion::SameLine(173);
			if (Legion::ImageButtonEx("DELETE", "DELETE", true, ImColor(8, 137, 196), ImColor(8, 148, 212), ImColor(47, 120, 41), ImVec2(127, 30), CalibriFont, 14.4f))
			{
				SendInfoToServer("dbb33d1755", "CPD", CharProfileSelected);
				auto it = ProfileItemInfoList.find(SelectedCharProfile);
				if (it != ProfileItemInfoList.end())
				{
					ProfileItemInfoList.erase(it);
				}
			}
			Legion::IndentEx(517, -300);
			Legion::ImageExTip("Hair", CalibriFont, 11, ("{fcd303}" + ItemNames[0] + "{fffff}\n\n" + EffectsInfo[0]).c_str(), CharactersProfileItemsTexture[0], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 23);
			Legion::ImageExTip("Face", CalibriFont, 11, ("{fcd303}" + ItemNames[1] + "{fffff}\n\n" + EffectsInfo[1]).c_str(), CharactersProfileItemsTexture[1], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 21);
			Legion::ImageExTip("Shirt", CalibriFont, 11, ("{fcd303}" + ItemNames[2] + "{fffff}\n\n" + EffectsInfo[2]).c_str(), CharactersProfileItemsTexture[2], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 21);
			Legion::ImageExTip("Pants", CalibriFont, 11, ("{fcd303}" + ItemNames[3] + "{fffff}\n\n" + EffectsInfo[3]).c_str(), CharactersProfileItemsTexture[3], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(60, -220);
			Legion::ImageExTip("Gloves", CalibriFont, 11, ("{fcd303}" + ItemNames[4] + "{fffff}\n\n" + EffectsInfo[4]).c_str(), CharactersProfileItemsTexture[4], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 22);
			Legion::ImageExTip("Shoes", CalibriFont, 11, ("{fcd303}" + ItemNames[5] + "{fffff}\n\n" + EffectsInfo[5]).c_str(), CharactersProfileItemsTexture[5], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 19);
			Legion::ImageExTip("Acc", CalibriFont, 11, ("{fcd303}" + ItemNames[6] + "{fffff}\n\n" + EffectsInfo[6]).c_str(), CharactersProfileItemsTexture[6], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(0, 19);
			Legion::ImageExTip("Pet", CalibriFont, 11, ("{fcd303}" + ItemNames[7] + "{fffff}\n\n" + EffectsInfo[7]).c_str(), CharactersProfileItemsTexture[7], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(-238, -120);
			Legion::ImageExTip("Weapon1", CalibriFont, 11, ("{fcd303}" + ItemNames[8] + "{fffff}\n\n" + EffectsInfo[8]).c_str(), CharactersProfileItemsTexture[8], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(60, -80);
			Legion::ImageExTip("Weapon2", CalibriFont, 11, ("{fcd303}" + ItemNames[9] + "{fffff}\n\n" + EffectsInfo[9]).c_str(), CharactersProfileItemsTexture[9], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(56, -15);
			Legion::ImageExTip("Weapon3", CalibriFont, 11, ("{fcd303}" + ItemNames[10] + "{fffff}\n\n" + EffectsInfo[10]).c_str(), CharactersProfileItemsTexture[10], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::IndentEx(-55, -10);
			Legion::ImageExTip("Skill", CalibriFont, 11, ("{fcd303}" + ItemNames[11] + "{fffff}\n\n" + EffectsInfo[11]).c_str(), CharactersProfileItemsTexture[11], EmptyBoxUITexture, ImVec2(45, 45));
			Legion::End();
		}

		if (CharactersProfileHalfTimeUI)
		{
			Mem.WriteMemP(GameManager, { 0xAC, 0x28, 0x28, 0x1C, 0xA8 }, 65793);
			Legion::BeginEx(NULL, NULL, NULL, "Characters Profile HalfTime UI", ImVec2(195, 105), ImVec2(0.5f, 0.85f), &InventoryProfileButtonsUI, LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus);
			Legion::IndentEx(26, 5);
			if (Legion::ImageButtonEx("View Button", "CHANGE PROFILE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(120, 34), CalibriFont, 12))
			{
				SendInfoToServer("ac40cf9c0e", "CPV", "CPV");
			}

			if (HalfTimeTimer->elapsedSeconds() < 25)
			{
				HalfTimerCounter = 25 - HalfTimeTimer->elapsedSeconds();
			}

			Legion::IndentEx(-26, 25);
			Legion::LabelEx("HalfTime Time", "THE GAME WILL RESUME IN", ImVec2(134, 20), Ebrima, 13);
			Legion::IndentEx(130, -40);
			Legion::LabelEx("HalfTime Counter", ("{d88f1b}" + std::to_string(HalfTimerCounter)).c_str(), ImVec2(50, 30), Ebrima, 26);
			Legion::IndentEx(20, -35);
			Legion::LabelEx("HalfTime Se", "{d88f1b}S", ImVec2(50, 26), Ebrima, 18);
			Legion::End();
		}

		if (CMessageBoxUI)
		{
			Legion::BeginEx(MessageBoxUITexture, NULL, NULL, "MessageBox", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &CMessageBoxUI, LegionWindowFlags_Custom);

			Legion::IndentEx(5.0f, 20);
			Legion::LabelEx("MSGBOX", Message.c_str(), ImVec2(280, 60), CalibriFont, 13.4f);
			if (Buttons == 1)
			{
				Legion::IndentEx(103.0f, 0);
				if (Legion::ImageButtonEx("Confirm", "Confirm", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(70, 29), CalibriFont, 13.4f))
				{
					CMessageBoxUI = false;
					Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				}
			}

			if (Buttons == 2)
			{
				Legion::IndentEx(70, 10);
				if (Legion::ImageButtonEx("Yes", "Yes", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(70, 29), CalibriFont, 13.4f))
				{
					if (MessageType == Msgbox::FriendRequest)
					{
						if (IsClan == 0)
						{
							AddFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), ClanSelectedPlayerId, 0, &ClanSelectedPlayerNickName);
							auto it = ChannelPlayerList.find(ClanSelectedPlayer);
							if (it != ChannelPlayerList.end())
								it->second->FriendStats = "Remove Friend";

							auto it2 = ClubMemberList.find(ClanSelectedPlayer);
							if (it2 != ClubMemberList.end())
								it2->second->FriendStats = "Remove Friend";

							auto it3 = UnionMemberList.find(ClanSelectedPlayer);
							if (it3 != UnionMemberList.end())
								it3->second->FriendStats = "Remove Friend";

							auto it4 = SupportPlayerList.find(ClanSelectedPlayer);
							if (it4 != SupportPlayerList.end())
								it4->second->FriendStats = "Remove Friend";

							auto it5 = WhisperPlayerList.find(ClanSelectedPlayer);
							if (it5 != WhisperPlayerList.end())
								it5->second->FriendStats = "Remove Friend";

						}
						else if (IsClan == 1)
						{
							AddFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);

							auto it = ChannelPlayerList.find(CSelectedPlayerId);
							if (it != ChannelPlayerList.end())
								it->second->FriendStats = "Remove Friend";

							auto it2 = ClanMemberList.find(CSelectedPlayerId);
							if (it2 != ClanMemberList.end())
								it2->second->FriendStats = "Remove Friend";

							auto it3 = UnionMemberList.find(CSelectedPlayerId);
							if (it3 != UnionMemberList.end())
								it3->second->FriendStats = "Remove Friend";

							auto it4 = SupportPlayerList.find(CSelectedPlayerId);
							if (it4 != SupportPlayerList.end())
								it4->second->FriendStats = "Remove Friend";

							auto it5 = WhisperPlayerList.find(CSelectedPlayerId);
							if (it5 != WhisperPlayerList.end())
								it5->second->FriendStats = "Remove Friend";
						}
						else if (IsClan == 2)
						{
							AddFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), SelectedCommunityPId, 0, &SelectedCommunityNick);
							//to do add friend

							auto CPlr = ChannelPlayerList.find(SelectedCommunityPId);

							PlayersShortInfoListClass* FriendsPlayerlist = new PlayersShortInfoListClass();

							FriendsPlayerlist->Level = CPlr->second->Level;
							std::string NickName = CPlr->second->Nickname;
							strcpy_s(FriendsPlayerlist->Name, NickName.length() + 1, NickName.c_str());
							FriendsPlayerlist->Rank = CPlr->second->Rank;
							FriendsPlayerlist->IsOnline = true;
							FriendsPlayerlist->PlayerId = CPlr->first;
							FriendsPlayerlist->FriendStats = "Remove Friend";
							FriendsPlayerlist->BlockStats = CPlr->second->BlockStats;
							FrindsPlayerList[FrindsPlayerList.size() + 1] = FriendsPlayerlist;

							for (auto it : BlockPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Remove Friend";
								}
							}

							for (auto it : ClanPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Remove Friend";
								}
							}

							for (auto it : UnionPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Remove Friend";
								}
							}
						}
					}
					else if (MessageType == Msgbox::DeleteFriendRequest)
					{
						if (IsClan == 0)
						{
							DeleteFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), ClanSelectedPlayerId, 0, &ClanSelectedPlayerNickName);

							auto it = ChannelPlayerList.find(ClanSelectedPlayer);
							if (it != ChannelPlayerList.end())
								it->second->FriendStats = "Add Friend";

							auto it2 = ClubMemberList.find(ClanSelectedPlayer);
							if (it2 != ClubMemberList.end())
								it2->second->FriendStats = "Add Friend";

							auto it3 = UnionMemberList.find(ClanSelectedPlayer);
							if (it3 != UnionMemberList.end())
								it3->second->FriendStats = "Add Friend";

							auto it4 = SupportPlayerList.find(ClanSelectedPlayer);
							if (it4 != SupportPlayerList.end())
								it4->second->FriendStats = "Add Friend";

							auto it5 = WhisperPlayerList.find(ClanSelectedPlayer);
							if (it5 != WhisperPlayerList.end())
								it5->second->FriendStats = "Add Friend";

							IsClan = false;
						}
						else if (IsClan == 1)
						{
							DeleteFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);

							auto it = ChannelPlayerList.find(CSelectedPlayerId);
							if (it != ChannelPlayerList.end())
								it->second->FriendStats = "Add Friend";

							auto it2 = ClanMemberList.find(CSelectedPlayerId);
							if (it2 != ClanMemberList.end())
								it2->second->FriendStats = "Add Friend";

							auto it3 = UnionMemberList.find(CSelectedPlayerId);
							if (it3 != UnionMemberList.end())
								it3->second->FriendStats = "Add Friend";

							auto it4 = SupportPlayerList.find(CSelectedPlayerId);
							if (it4 != SupportPlayerList.end())
								it4->second->FriendStats = "Add Friend";

							auto it5 = WhisperPlayerList.find(CSelectedPlayerId);
							if (it5 != WhisperPlayerList.end())
								it5->second->FriendStats = "Add Friend";
						}
						else if (IsClan == 2)
						{
							DeleteFriend_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);
							//to do add friend

							for (auto it : FrindsPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									FrindsPlayerList.erase(it.first);
								}
							}

							for (auto it : BlockPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Add Friend";
								}
							}

							for (auto it : ClanPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Add Friend";
								}
							}

							for (auto it : UnionPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->FriendStats = "Add Friend";
								}
							}
						}
					}
					else if (MessageType == Msgbox::BlockChatReqeust)
					{
						BlockChat_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);

						auto it = ChannelPlayerList.find(CSelectedPlayerId);
						if (it != ChannelPlayerList.end())
							it->second->BlockStats = "Remove Block";

						auto it2 = ClanMemberList.find(CSelectedPlayerId);
						if (it2 != ClanMemberList.end())
							it2->second->BlockStats = "Remove Block";

						auto it3 = UnionMemberList.find(CSelectedPlayerId);
						if (it3 != UnionMemberList.end())
							it3->second->BlockStats = "Remove Block";

						auto it4 = SupportPlayerList.find(CSelectedPlayerId);
						if (it4 != SupportPlayerList.end())
							it4->second->BlockStats = "Remove Block";

						auto it5 = WhisperPlayerList.find(CSelectedPlayerId);
						if (it5 != WhisperPlayerList.end())
							it5->second->BlockStats = "Remove Block";

						if (IsClan == 2)
						{
							BlockChat_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);
							//to do add friend

							auto CPlr = ChannelPlayerList.find(SelectedCommunityPId);

							PlayersShortInfoListClass* FriendsPlayerlist = new PlayersShortInfoListClass();

							FriendsPlayerlist->Level = CPlr->second->Level;
							std::string NickName = CPlr->second->Nickname;
							strcpy_s(FriendsPlayerlist->Name, NickName.length() + 1, NickName.c_str());
							FriendsPlayerlist->Rank = CPlr->second->Rank;
							FriendsPlayerlist->IsOnline = true;
							FriendsPlayerlist->PlayerId = CPlr->first;
							FriendsPlayerlist->FriendStats = CPlr->second->FriendStats;
							FriendsPlayerlist->BlockStats = "Remove Block";
							BlockPlayerList[BlockPlayerList.size() + 1] = FriendsPlayerlist;

							for (auto it : BlockPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Remove Block";
								}
							}

							for (auto it : ClanPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Remove Block";
								}
							}

							for (auto it : UnionPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Remove Block";
								}
							}
						}

					}
					else if (MessageType == Msgbox::RemoveBlockChatReqeust)
					{
						RemoveBlockChat_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);

						auto it = ChannelPlayerList.find(CSelectedPlayerId);
						if (it != ChannelPlayerList.end())
							it->second->BlockStats = "Block Chat";

						auto it2 = ClanMemberList.find(CSelectedPlayerId);
						if (it2 != ClanMemberList.end())
							it2->second->BlockStats = "Block Chat";

						auto it3 = UnionMemberList.find(CSelectedPlayerId);
						if (it3 != UnionMemberList.end())
							it3->second->BlockStats = "Block Chat";

						auto it4 = SupportPlayerList.find(CSelectedPlayerId);
						if (it4 != SupportPlayerList.end())
							it4->second->BlockStats = "Block Chat";

						auto it5 = WhisperPlayerList.find(CSelectedPlayerId);
						if (it5 != WhisperPlayerList.end())
							it5->second->BlockStats = "Block Chat";

						if (IsClan == 2)
						{
							BlockChat_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), CSelectedPlayerId, 0, &CSelectedPlayer);
							//to do add friend

							for (auto it : FrindsPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Block Chat";
								}
							}

							for (auto it : BlockPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									BlockPlayerList.erase(it.first);
									it.second->BlockStats = "Block Chat";
								}
							}

							for (auto it : ClanPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Block Chat";
								}
							}

							for (auto it : UnionPlayerList)
							{
								if (it.second->PlayerId == SelectedCommunityPId)
								{
									it.second->BlockStats = "Block Chat";
								}
							}
						}
					}
					else if (MessageType == Msgbox::ClanInvite)
					{
						ClanInvite_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA68), CSelectedPlayerId, 0);
					}
					else if (MessageType == Msgbox::ExchangeRequest)
					{
						SendInfoToServer(ExcFindPlayer, "Exc", "f436dea00b");
					}
					else if (MessageType == Msgbox::ExchangeAccept)
					{
						SendInfoToServer("Accept", "Exc", "cdd70cc68d");
						IsExchanged = true;
						MenuSelected = 7;
						CMainMenuUI = true;
					}
					else if (MessageType == Msgbox::ChallengeRequst)
					{
						MenuSelected = 8;
						CMainMenuUI = true;
					}
					else if (MessageType == Msgbox::ChallengeAccept)
					{
						SendInfoToServer("Accept", "Exc", "18cc1bb625");
					}
					else if (MessageType == Msgbox::LeaveRoom)
					{
						LeaveRoom_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA68), 0);
					}
					else if (MessageType == Msgbox::ClanKick)
					{
						SendInfoToServer("5f70aee2d2", std::to_string(ClanSelectedPlayer), std::to_string(ClanSelectedPlayerId));
					}
					else if (MessageType == Msgbox::ClanBan)
					{
						SendInfoToServer("5d6478c8cb", std::to_string(ClanSelectedPlayer), std::to_string(ClanSelectedPlayerId));
					}
					else if (MessageType == Msgbox::ClanTransferMaster)
					{
						SendInfoToServer("a4f2d3b9fd", "CLN", std::to_string(ClanSelectedPlayerId));
						auto it = ClubMemberList.find(ClanSelectedPlayer);
						auto it2 = ClubMemberList.find(1);
						if (it != ClubMemberList.end())
						{
							it->second->Rank = "Master";
							it2->second->Rank = "Member";
						}
						MyClanRank = "Member";
					}
					else if (MessageType == Msgbox::ClanInviteUnion)
					{
						SendInfoToServer("3ae2cc61d8", "UNI", UnionName);
					}
					else if (MessageType == Msgbox::ClanKickUntion)
					{
						SendInfoToServer("ec86108214", std::to_string(ClanUnionId), UnionName);
					}
					else if (MessageType == Msgbox::ClanLeaveUntion)
					{
						SendInfoToServer("e3a694cc43", "UNI", "LV");
					}
					else if (MessageType == Msgbox::ClanUnionAccept)
					{
						SendInfoToServer("dc3416bb4d", "UNI", "Accept");
					}
					else if (MessageType == Msgbox::ExitGame)
					{
						//TerminateProcess(GetCurrentProcess(), 1);
					//	ExitProcess(0);
						HANDLE explorer;
						explorer = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
						TerminateProcess(explorer, 1);
					}
					CMessageBoxUI = false;
					Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				}
				Legion::SameLine(155);
				if (Legion::ImageButtonEx("No", "No", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(70, 29), CalibriFont, 13.4f))
				{
					if (MessageType == Msgbox::ExchangeAccept)
					{
						SendInfoToServer("Refuse", "Exc", "cdd70cc68d");
					}
					else if (MessageType == Msgbox::ChallengeAccept)
					{
						SendInfoToServer("Refuse", "Exc", "18cc1bb625");
					}
					else if (MessageType == Msgbox::ClanUnionRefuse)
					{
						SendInfoToServer("c6d46435c4", "UNI", "Refuse");
					}
					CMessageBoxUI = false;
					Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				}
			}
			Legion::End();
		}

		if (NewClanSystemUI)
		{
			LegionWindowFlags flags;

			if (NewClanRankChangeUI || NewClanNameChangeUI || NewClanInviteUI || NewCreateClanUI || CMessageBoxUI)
				flags = LegionWindowFlags_Custom | LegionWindowFlags_NoBringToFrontOnFocus;
			else
				flags = LegionWindowFlags_Custom;

			Legion::BeginEx(ClanUITexture, NULL, NULL, "Clan System", ImVec2(600, 400), ImVec2(0.5f, 0.5f), &NewClanSystemUI, flags);
			Legion::IndentEx(568, -12);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewClanSystemUI = false;

			if (!IsInClan)
			{
				Legion::IndentEx(-335, 12);
				Legion::TextLabelColor(CalibriFont, 20, "CLAN SYSTEM");
				if (ClanButtonMenu != 2)// Rank
				{
					Legion::IndentEx(-71, 95);
					if (Legion::ImageButtonEx("CREATE CLAN Button", "CREATE CLAN", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(245, 30), CalibriFont, 12))
					{
						NewCreateClanUI = true;
					}
					if (Legion::ImageButtonEx("CLAN RANK Button", "CLAN RANK", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(245, 30), CalibriFont, 12))
					{
						ClanName = "None";
						ClanLevel = "0";
						ClanRank = "0";
						ClanBattels = "0/0";
						ClanEXP = 0;
						TotalClanExp = 0;
						ClanPoints = "0";
						ClanTitle = "";
						ClanMessage = "";
						ClanMarkTexture = NULL;

						SendInfoToServer("3d52c59e59", "RNK", "DNK");
					}
					Legion::IndentEx(10, 6);
					Legion::InputTextV("ClanSearchTextBox", CalibriFont, 11, ChatTextBoxTexture, "", ClanFindByName, IM_ARRAYSIZE(ClanFindByName), ImVec2(160, 15));
					Legion::IndentEx(160, -26);
					if (Legion::ImageButtonEx("Find Clan Button", "SEARCH", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(76, 25), CalibriFont, 12))
					{
						SendInfoToServer("db3bad8e30", "UNI", ClanFindByName);
					}
				}
				if (ClanButtonMenu == 2)// Rank
				{
					Legion::IndentEx(-219, 25);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "ClanRank List", ImVec2(558, 254));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "       RANK              NAME                LEADER                 POINTS             LEVEL             BATTELS");
					for (std::pair<unsigned int, ClanRankListClass*> it : ClanRankList)
					{
						if (Legion::NewClanRankList(it.second->Rank.c_str(), it.second->Name, it.second->Leader, it.second->Points.c_str(), it.second->Level.c_str(), it.second->Battles.c_str(), ClanSelectionTexture, OnlineTexture, &ClanRankId, it.first, CalibriFont, 15.35f))
						{
							SendInfoToServer("db3bad8e30", "UNI", it.second->Name);
						}
					}
					Legion::EndChild();

					if (Legion::ImageButtonEx("CLAN RANK Button", "CLAN RANK", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(245, 30), CalibriFont, 12))
					{
						NewCreateClanUI = true;
					}
					Legion::IndentEx(260, -30);
					Legion::InputTextV("ClanFinderTextBox", CalibriFont, 11, ChatTextBoxTexture, "", ClanFindByName, IM_ARRAYSIZE(ClanFindByName), ImVec2(160, 15));
					Legion::IndentEx(160, -27);
					if (Legion::ImageButtonEx("Find Clan Button", "SEARCH", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(76, 25), CalibriFont, 12))
					{
						SendInfoToServer("db3bad8e30", "UNI", ClanFindByName);
					}
				}
			}
			if (IsInClan)
			{
				Legion::IndentEx(-566, -5);
				Legion::ImageV("Clan Mark", ClanMarkTexture, ImVec2(50, 50));
				Legion::IndentEx(60.5, -55);
				Legion::TextLabelColor(CalibriFont, 24, ("#" + ClanRank).c_str());
				Legion::IndentEx(81, -21);
				Legion::ProgressBarEx(CalibriFont, 12, "Clan EXP", ClanEXP, 0, TotalClanExp, 1, ImVec2(408, 10), ImColor(249, 249, 249), ImColor(74, 170, 76), false, true);
				Legion::IndentEx(-80, 5);
				Legion::TextLabelColor(CalibriFont, 16, ClanName.c_str());
				Legion::TextLabelColor(CalibriFont, 12, ("LEVEL.{4aaa4c}" + ClanLevel).c_str());
				Legion::SameLine(150);
				Legion::TextLabelColor(CalibriFont, 12, ("POINTS.{4aaa4c}" + ClanPoints).c_str());
				Legion::SameLine(250);
				Legion::TextLabelColor(CalibriFont, 12, ("BATTALS " + ClanBattels).c_str());
				Legion::SameLine(340);
				Legion::TextLabelColor(CalibriFont, 12, ("MEMBERS  " + std::to_string(ClubMemberList.size()) + "/" + std::to_string(((ClanLevel.empty()) ? 1 : std::stoi(ClanLevel)) * 12)).c_str());
				if (ClanButtonMenu == 2)
				{
					Legion::IndentEx(370, -25);
					Legion::InputTextV("clanfindsearchTextBox", CalibriFont, 11, ChatTextBoxTexture, "", ClanFindByName, IM_ARRAYSIZE(ClanFindByName), ImVec2(100, 15), LegionInputTextFlags_EnterReturnsTrue);
					Legion::IndentEx(98, -27);
					if (Legion::ImageButtonEx("clansearch Button", "SEARCH", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(50, 25), CalibriFont, 12))
					{
						SendInfoToServer("db3bad8e30", "UNI", ClanFindByName);
					}
				}
				if (ClanButtonMenu == 0)// Clan
				{
					Legion::IndentEx(-53, 10);
					if (Legion::ImageButtonEx("Clan Notice", ClanTitle.c_str(), true, ListBGTexture, ListBGTexture, ListBGTexture, ImVec2(444, 31), CalibriFont, 15.4f))
					{
						if (MyClanRank != "Master")
							IsNoticeRead = true;

						ClanButtonMenu = 8;
					}
					Legion::IndentEx(449, -41);
					if (Legion::ImageButtonEx("Notice Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 30), CalibriFont, 15.4f))
					{
						if (MyClanRank != "Master")
							IsNoticeRead = true;

						ClanButtonMenu = 8;
					}
					Legion::IndentEx(-446, 3);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(558, 213));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "         NICKNAME        LEVEL              LOCATION                    RANK           POINTS           BATTELS");
					for (std::pair<unsigned int, ClanMemberListClass*> it : ClubMemberList)
					{
						if (Legion::NewClanPlayerList(it.second->Nickname, it.second->Level.c_str(), it.second->Location, it.second->Rank.c_str(), it.second->Points.c_str(), it.second->Battles.c_str(), ClanSelectionTexture, OnlineTexture, OfflineTexture, it.second->PlayerStatus.c_str(), &ClanPlayerId, it.first, CalibriFont, 15.35f))
						{
							ClanSelectedPlayer = it.first;
							ClanSelectedPlayerId = it.second->PlayerId;
							ClanSelectedPlayerNickName = it.second->Nickname;
							ClanSelectedFriendStats = it.second->FriendStats;
							ClanSelectedPlayerRank = it.second->Rank;
						}
					}

					if (MyPlayerId != ClanSelectedPlayerId && ClanSelectedPlayerId != 0)
					{
						if (Legion::BeginPopupContextWindowV(ContextTexture, NULL, NULL, "Clan List Context", LegionMouseButton_Right))
						{
							std::string nickname;
							if (ClanSelectedPlayerNickName.find("{") != std::string::npos)
								nickname = Mem.Split(ClanSelectedPlayerNickName, '}')[1];
							else
								nickname = ClanSelectedPlayerNickName;

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "User Info", NULL))
							{
								UserInfo_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), ClanSelectedPlayerId, 0);
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Whisper", NULL))
							{
								ChatButtonTexture(Chat_Type::Whisper);

								memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);

								std::string whisper;
								if (ClanSelectedPlayerNickName.find("{") != std::string::npos)
									whisper = "To " + Mem.Split(ClanSelectedPlayerNickName, '}')[1] + " : "/*+ " : "*/;
								else
									whisper = "To " + Mem.Split(ClanSelectedPlayerNickName, '}')[0] + " : "/*+ " : "*/;

								std::copy(whisper.begin(), whisper.end(), ChatSystemMessage);

								LegionInputTextState& edit_state = Legion::GetCurrentContext()->InputTextState;

								int n = edit_state.TextW.size();
								edit_state.Stb.cursor = n; //stb_text_locate_coord(str, x, y);
								edit_state.Stb.select_start = edit_state.Stb.cursor;
								edit_state.Stb.select_end = edit_state.Stb.cursor;
								edit_state.Stb.has_preferred_x = 0;
								edit_state.CursorAnimReset();
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, ClanSelectedFriendStats.c_str(), NULL))
							{
								if (ClanSelectedFriendStats == "Add Friend")
								{
									IsClan = 0;
									CustomMessageBox("Would you like to send\n a friend request to " + nickname, 2, Msgbox::FriendRequest);
								}
								else if (ClanSelectedFriendStats == "Delete a Friend")
								{
									IsClan = 0;
									CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::DeleteFriendRequest);
								}
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Send a MSG", NULL))
							{
								uintptr_t MsgName = Mem.FindOffAAddy(dwBaseAddr + 0x0173D7A4, { 0x1C, 0x4, 0x0, 0x0, 0x0, 0x8, 0x14, 0x10, 0x2AC, 0x4 });
								WriteProcessMemory(GetCurrentProcess(), (LPVOID)MsgName, CSelectedPlayer.c_str(), sizeof(CSelectedPlayer), 0);
								S4Gui(S4_Screen::Display_SendMSG);
							}

							if (MyClanRank == "Master")
							{
								if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Change Rank", NULL))
								{
									NewClanRankChangeUI = true;
								}

								if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Transfer Master", NULL))
								{
									CustomMessageBox("Would you like to trasfer master\n to " + nickname + "?", 2, Msgbox::ClanTransferMaster);
								}
							}
							if (MyClanRank == "Master" || MyClanRank == "CoMaster")
							{
								if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Ban", NULL))
								{
									CustomMessageBox("Would you like to ban \n" + nickname + "from the clan?", 2, Msgbox::ClanBan);
								}
							}

							if (MyClanRank == "Master" || MyClanRank == "CoMaster" || MyClanRank == "Staff")
							{
								if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Kick", NULL))
								{
									CustomMessageBox("Would you like to kick \n" + nickname + "from the clan?", 2, Msgbox::ClanKick);
								}
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Exchange", NULL))
							{
								strcpy_s(ExcFindPlayer, nickname.length() + 1, nickname.c_str());
								CustomMessageBox("Would you like to start exchange\n with " + nickname + "?", 2, Msgbox::ExchangeRequest);
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Challenge", NULL))
							{
								strcpy_s(ChalFindPlayer, nickname.length() + 1, nickname.c_str());
								MenuSelected = 8;
								CMainMenuUI = true;
								//CustomMessageBox("Would you like to challenge " + ClanSelectedPlayerNickName + "?", 2, Msgbox::ChallengeRequst);
							}
							Legion::EndPopup();
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 1)//Union
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(558, 254));
					Legion::IndentEx(70, 11);
					Legion::TextLabelColor(CalibriFont, 15, "INFORMATIONS");
					Legion::IndentEx(-50, 35);
					Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "union info", ImVec2(280, 180));
					if (!UnionName.empty())
					{
						Legion::TextLabelColor(CalibriFont, 15, "CLAN.");
						Legion::SameLine(150);
						Legion::LabelEx("Clan Name", UnionName.c_str(), ImVec2(100, 10), CalibriFont, 15, ImVec2(0.5f, 0.5f));
						Legion::TextLabelColor(CalibriFont, 15, "LEADER.");
						Legion::SameLine(150);
						Legion::LabelEx("Clan Leader", UnionLeader.c_str(), ImVec2(100, 10), CalibriFont, 15, ImVec2(0.5f, 0.5f));
						Legion::TextLabelColor(CalibriFont, 15, "POINTS.");
						Legion::SameLine(150);
						Legion::LabelEx("Clan POINTS", ("{4aaa4c}" + UnionPoints).c_str(), ImVec2(100, 10), CalibriFont, 15, ImVec2(0.5f, 0.5f));
						Legion::TextLabelColor(CalibriFont, 15, "BATTELS");
						Legion::SameLine(150);
						Legion::LabelEx("Clan BATTELS", UnionBattels.c_str(), ImVec2(100, 10), CalibriFont, 15, ImVec2(0.5f, 0.5f));
						Legion::TextLabelColor(CalibriFont, 15, "MEMBERS");
						Legion::SameLine(150);
						Legion::LabelEx("Clan MEMBERS", UnionMember.c_str(), ImVec2(100, 10), CalibriFont, 15, ImVec2(0.5f, 0.5f));
					}
					Legion::EndChild();
					Legion::IndentEx(385, -238);
					Legion::TextLabelColor(CalibriFont, 15, "CLANS UNION");
					Legion::IndentEx(-80, 30);
					Legion::BeginChildV(NULL, NULL, ScrollBarTexture, "union List", ImVec2(230, 180));
					for (std::pair<unsigned int, ClanUnionListClass*> it : ClanUnionList)
					{
						if (Legion::NewClanUnionList(it.second->ClanName, it.second->Level.c_str(), ClanSelectionTexture, &ClanUnionId, it.first, CalibriFont, 15.35f))
						{
							UnionName = it.second->ClanName;
							UnionLeader = it.second->Leader;
							UnionPoints = it.second->Points;
							UnionBattels = it.second->Battles;
							UnionMember = it.second->Members;
						}
					}

					if (Legion::BeginPopupContextWindowV(ContextTexture, NULL, NULL, "Union List Menu", LegionMouseButton_Right))
					{

						if (ClanUnionList.size() == 0 || UnionMaster == true)
						{
							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Invite", NULL))
							{
								NewClanInviteUI = true;
							}

							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Kick", NULL))
							{
								CustomMessageBox("Are you sure you want to kick" + UnionName + "\n from union", 2, Msgbox::ClanKickUntion);
							}
						}
						else if (UnionMaster == false)
						{
							if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Leave", NULL))
							{
								CustomMessageBox("Are you sure you want to leave the union!", 2, Msgbox::ClanLeaveUntion);
							}
						}

						Legion::EndPopup();
					}
					Legion::EndChild();
					/*if (Legion::BeginPopupContextItemV(ContextTexture))
					{
						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Invite", NULL, false))
						{
							NewClanInviteUI = true;
						}

						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Kick", NULL, false))
						{
							//MsgBox
							SendInfoToServer("db3bad8e30", "UNI", UnionName);
						}
						Legion::EndPopup();
					}*/
					Legion::EndChild();
				}
				if (ClanButtonMenu == 2)// Rank
				{
					Legion::IndentEx(-519, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "ClanRank List", ImVec2(558, 254));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "       RANK              NAME                LEADER                 POINTS             LEVEL             BATTELS");
					for (std::pair<unsigned int, ClanRankListClass*> it : ClanRankList)
					{
						if (Legion::NewClanRankList(it.second->Rank.c_str(), it.second->Name, it.second->Leader, it.second->Points.c_str(), it.second->Level.c_str(), it.second->Battles.c_str(), ClanSelectionTexture, OnlineTexture, &ClanRankId, it.first, CalibriFont, 15.35f))
						{
							SendInfoToServer("db3bad8e30", "UNI", it.second->Name);
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 4)// History
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "History List", ImVec2(558, 254));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "       NAME                                                        DATE                                                        STATUS");
					for (std::pair<unsigned int, ClanHistoryListClass*> it : ClanHistoryList)
					{
						if (Legion::NewClanHistoryList(it.second->Name, it.second->Date.c_str(), it.second->Status.c_str(), ClanSelectionTexture, &ClanHistoryId, it.first, CalibriFont, 15.35f))
						{
							SendInfoToServer("db3bad8e30", "UNI", it.second->Name);
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 5)
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Shop List", ImVec2(558, 265));
					Legion::IndentEx(15, 3);

					if (MyClanRank == "Master")
						LeaveCloseButton = "CLOSE CLAN";
					else
						LeaveCloseButton = "LEAVE CLAN";

					if (Legion::ImageButtonEx("Leave Close Button", LeaveCloseButton.c_str(), true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
					{
						if (LeaveCloseButton == "CLOSE CLAN" && MyClanRank == "Master")
						{
							SendInfoToServer("d7d6ba5452", "LCC", "CCC");
							NewClanSystemUI = false;
							IsInClan = false;
						}
						else
						{
							SendInfoToServer("5f70aee2d2", "LCC", std::to_string(MyPlayerId));
							NewClanSystemUI = false;
							IsInClan = false;
						}
					}

					if (MyClanRank == "Master")
					{
						if (Legion::ImageButtonEx("CLAN MARK Button", "CLAN MARK", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							ClanButtonMenu = 9;
						}
						if (Legion::ImageButtonEx("CLAN NAME Button", "CLAN NAME", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							NewClanNameChangeUI = true;
						}
					}
					if (MyClanRank == "Master" || MyClanRank == "CoMaster")
					{
						if (Legion::ImageButtonEx("InviteClan Button", "Invite Union", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							NewClanInviteUI = true;
						}
						if (Legion::ImageButtonEx("REQUEST Button", "REQUEST LIST", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							SendInfoToServer("e07b9b00fb", "RQL", "RQL");
						}
						if (Legion::ImageButtonEx("BANNED Button", "BANNED LIST", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							SendInfoToServer("a0171a4e47", "BNL", "BNL");
						}
					}
					if (MyClanRank == "Master")
					{
						if (Legion::ImageButtonEx("REGISTER CLAN WAR Button", "REGISTER CLAN WAR", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(512, 28), CalibriFont, 15.4f))
						{
							SendInfoToServer("c312f20e87", "CLW", "CLW");
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 6)
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(558, 254));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "       Name                                                        Level                                                        STATUS");
					for (std::pair<unsigned int, PlayerShortInfoListClass*> it : ClubRequestList)
					{
						int Status = Legion::NewClanRequestJoinListEx(it.second->Name, it.second->Level.c_str(), ButtonTexture, ButtonTexture, CalibriFont, 15.35f);
						if (Status == 1)
						{
							SendInfoToServer("2325d20599", "AC", std::to_string(it.second->PlayerId));
							ClubRequestList.erase(it.first);
						}

						if (Status == 2)
						{
							SendInfoToServer("682e66dd90", "RJ", std::to_string(it.second->PlayerId));
							ClubRequestList.erase(it.first);
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 7)
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Ban List", ImVec2(558, 254));
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.35f, "       Name                                                        Level                                                        STATUS");
					for (std::pair<unsigned int, PlayerShortInfoListClass*> it : ClubBannedList)
					{
						if (Legion::NewClanBannedPlayerListEx(it.second->Name, it.second->Level.c_str(), ButtonTexture, CalibriFont, 15.35f))
						{
							SendInfoToServer("c580820c97", "UNB", std::to_string(it.second->PlayerId));
							ClubBannedList.erase(it.first);
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 8)
				{
					LegionInputTextFlags_ flag;
					if (IsNoticeRead)
						flag = LegionInputTextFlags_ReadOnly;
					else
						flag = LegionInputTextFlags_None;

					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(558, 267));
					Legion::IndentEx(11, 6);
					Legion::TextLabelColor(CalibriFont, 15.4f, "NOTICE");
					Legion::IndentEx(0, 1);
					Legion::InputTextV("NOTICE Box", CalibriFont, 12, ChatTextBoxTexture, "", ClanNotice, IM_ARRAYSIZE(ClanNotice), ImVec2(534, 15), flag);//here
					Legion::IndentEx(0, 3);
					Legion::TextLabelColor(CalibriFont, 15.4f, "MESSAGE");
					Legion::InputTextV("MESSAGE Box", CalibriFont, 12, BBoxTexture, "", ClanNoticeMessage, IM_ARRAYSIZE(ClanNoticeMessage), ImVec2(534, 147), LegionInputTextFlags_Multiline | flag);
					if (!IsNoticeRead)
					{
						Legion::IndentEx(-3, 3);
						if (Legion::ImageButtonEx("Notice Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(534, 30), CalibriFont, 15.4f))
						{
							SendInfoToServer("ab64a701b0", ClanNotice, ClanNoticeMessage);
						}
					}
					Legion::EndChild();
				}
				if (ClanButtonMenu == 9)
				{
					Legion::IndentEx(-51, 12);
					Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(558, 261));
					Legion::IndentEx(13, 8);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} LOGO");

					if (ClanLogoSelected > 100)
						SelectedClanLogo = "logo_" + std::to_string(ClanLogoSelected - 101);
					else
						SelectedClanLogo = "basic_" + std::to_string(ClanLogoSelected);

					Legion::ImageV("Clan Mark", ClubsTexture(SelectedClanLogo), ImVec2(80, 80));//ClanLogoSelected - 1
					Legion::IndentEx(15, 5);
					if (ClanLogoSelected < 101)
						ClanLogoPrice = "7000";
					else
						ClanLogoPrice = "10000";

					Legion::TextLabelColor(CalibriFont, 12, ("{e4dc44}" + ClanLogoPrice).c_str());
					Legion::IndentEx(33, -17);
					Legion::ImageV("LP Icon", EvTextture, ImVec2(15.12f, 15.12f));
					Legion::IndentEx(-51, 1);
					if (Legion::ImageButtonEx("buy clan logo", "{c1ccdd}BUY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(80, 36), CalibriFont, 14, ImVec2(0.5f, 0.5f)))
					{
						SendInfoToServer(std::to_string(ClanLogoSelected), "CL", "a4ca685f61");
						// Send Secc Message
					}
					Legion::IndentEx(110, -170);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} BASE.");
					{
						Legion::BeginChildV(BBoxTexture, NULL, NULL, "Base Clan Logo", ImVec2(200, 235));
						Legion::IndentEx(0, 8);
						Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "TEST Clan Logo", ImVec2(194, 220));
						Legion::IndentEx(8, 0);
						Legion::RadioButtonEx("basic_0", ClubsTexture("basic_1"), ImVec2(40, 40), &ClanLogoSelected, 1);//first problem look all line in tool = x4 logos so why line 2 only x3 hmm
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_1", ClubsTexture("basic_2"), ImVec2(40, 40), &ClanLogoSelected, 2);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_2", ClubsTexture("basic_3"), ImVec2(40, 40), &ClanLogoSelected, 3);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_3", ClubsTexture("basic_4"), ImVec2(40, 40), &ClanLogoSelected, 4);
						//Line 2
						Legion::RadioButtonEx("basic_4", ClubsTexture("basic_5"), ImVec2(40, 40), &ClanLogoSelected, 5);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_5", ClubsTexture("basic_6"), ImVec2(40, 40), &ClanLogoSelected, 6);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_6", ClubsTexture("basic_7"), ImVec2(40, 40), &ClanLogoSelected, 7);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_7", ClubsTexture("basic_8"), ImVec2(40, 40), &ClanLogoSelected, 8);
						//Line 3
						Legion::RadioButtonEx("basic_9", ClubsTexture("basic_9"), ImVec2(40, 40), &ClanLogoSelected, 9);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_10", ClubsTexture("basic_10"), ImVec2(40, 40), &ClanLogoSelected, 10);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_11w", ClubsTexture("basic_11"), ImVec2(40, 40), &ClanLogoSelected, 11);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_10w", ClubsTexture("basic_12"), ImVec2(40, 40), &ClanLogoSelected, 12);
						//Line 4	
						Legion::RadioButtonEx("basic_11", ClubsTexture("basic_13"), ImVec2(40, 40), &ClanLogoSelected, 13);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_12", ClubsTexture("basic_14"), ImVec2(40, 40), &ClanLogoSelected, 14);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_13", ClubsTexture("basic_15"), ImVec2(40, 40), &ClanLogoSelected, 15);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_14", ClubsTexture("basic_16"), ImVec2(40, 40), &ClanLogoSelected, 16);
						//Line 5
						Legion::RadioButtonEx("basic_15", ClubsTexture("basic_17"), ImVec2(40, 40), &ClanLogoSelected, 17);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_16", ClubsTexture("basic_18"), ImVec2(40, 40), &ClanLogoSelected, 18);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_17", ClubsTexture("basic_19"), ImVec2(40, 40), &ClanLogoSelected, 19);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_18", ClubsTexture("basic_20"), ImVec2(40, 40), &ClanLogoSelected, 20);
						//Line 6
						Legion::RadioButtonEx("basic_19", ClubsTexture("basic_21"), ImVec2(40, 40), &ClanLogoSelected, 21);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_20", ClubsTexture("basic_22"), ImVec2(40, 40), &ClanLogoSelected, 22);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_21", ClubsTexture("basic_23"), ImVec2(40, 40), &ClanLogoSelected, 23);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_22", ClubsTexture("basic_24"), ImVec2(40, 40), &ClanLogoSelected, 24);
						//Line 7
						Legion::RadioButtonEx("basic_23", ClubsTexture("basic_25"), ImVec2(40, 40), &ClanLogoSelected, 25);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_24", ClubsTexture("basic_26"), ImVec2(40, 40), &ClanLogoSelected, 26);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_25", ClubsTexture("basic_27"), ImVec2(40, 40), &ClanLogoSelected, 27);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_26", ClubsTexture("basic_28"), ImVec2(40, 40), &ClanLogoSelected, 28);
						//Line 8
						Legion::RadioButtonEx("basic_27", ClubsTexture("basic_29"), ImVec2(40, 40), &ClanLogoSelected, 29);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_28", ClubsTexture("basic_30"), ImVec2(40, 40), &ClanLogoSelected, 30);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_29", ClubsTexture("basic_31"), ImVec2(40, 40), &ClanLogoSelected, 31);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_30", ClubsTexture("basic_32"), ImVec2(40, 40), &ClanLogoSelected, 32);
						//Line 9
						Legion::RadioButtonEx("basic_31", ClubsTexture("basic_33"), ImVec2(40, 40), &ClanLogoSelected, 33);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_32", ClubsTexture("basic_34"), ImVec2(40, 40), &ClanLogoSelected, 34);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_33", ClubsTexture("basic_35"), ImVec2(40, 40), &ClanLogoSelected, 35);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_34", ClubsTexture("basic_36"), ImVec2(40, 40), &ClanLogoSelected, 36);
						//Line 10
						Legion::RadioButtonEx("basic_35", ClubsTexture("basic_37"), ImVec2(40, 40), &ClanLogoSelected, 37);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_36", ClubsTexture("basic_38"), ImVec2(40, 40), &ClanLogoSelected, 38);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_37", ClubsTexture("basic_39"), ImVec2(40, 40), &ClanLogoSelected, 39);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_38", ClubsTexture("basic_40"), ImVec2(40, 40), &ClanLogoSelected, 40);
						//Line 11
						Legion::RadioButtonEx("basic_39", ClubsTexture("basic_41"), ImVec2(40, 40), &ClanLogoSelected, 41);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_40", ClubsTexture("basic_42"), ImVec2(40, 40), &ClanLogoSelected, 42);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_41", ClubsTexture("basic_43"), ImVec2(40, 40), &ClanLogoSelected, 43);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_42", ClubsTexture("basic_44"), ImVec2(40, 40), &ClanLogoSelected, 44);
						//Line 12
						Legion::RadioButtonEx("basic_43", ClubsTexture("basic_45"), ImVec2(40, 40), &ClanLogoSelected, 45);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_44", ClubsTexture("basic_46"), ImVec2(40, 40), &ClanLogoSelected, 46);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_45", ClubsTexture("basic_47"), ImVec2(40, 40), &ClanLogoSelected, 47);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_46", ClubsTexture("basic_48"), ImVec2(40, 40), &ClanLogoSelected, 48);
						//Line 13
						Legion::RadioButtonEx("basic_47", ClubsTexture("basic_49"), ImVec2(40, 40), &ClanLogoSelected, 49);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_48", ClubsTexture("basic_50"), ImVec2(40, 40), &ClanLogoSelected, 50);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_49", ClubsTexture("basic_51"), ImVec2(40, 40), &ClanLogoSelected, 51);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_50", ClubsTexture("basic_52"), ImVec2(40, 40), &ClanLogoSelected, 52);
						//Line 14
						Legion::RadioButtonEx("basic_51", ClubsTexture("basic_53"), ImVec2(40, 40), &ClanLogoSelected, 53);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_52", ClubsTexture("basic_54"), ImVec2(40, 40), &ClanLogoSelected, 54);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_53", ClubsTexture("basic_55"), ImVec2(40, 40), &ClanLogoSelected, 55);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_54", ClubsTexture("basic_56"), ImVec2(40, 40), &ClanLogoSelected, 56);
						//Line 15
						Legion::RadioButtonEx("basic_56", ClubsTexture("basic_57"), ImVec2(40, 40), &ClanLogoSelected, 57);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_57", ClubsTexture("basic_58"), ImVec2(40, 40), &ClanLogoSelected, 58);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_58", ClubsTexture("basic_59"), ImVec2(40, 40), &ClanLogoSelected, 59);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_59", ClubsTexture("basic_60"), ImVec2(40, 40), &ClanLogoSelected, 60);
						//Line 16
						Legion::RadioButtonEx("basic_60", ClubsTexture("basic_61"), ImVec2(40, 40), &ClanLogoSelected, 61);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_61", ClubsTexture("basic_62"), ImVec2(40, 40), &ClanLogoSelected, 62);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_62", ClubsTexture("basic_63"), ImVec2(40, 40), &ClanLogoSelected, 63);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_63", ClubsTexture("basic_64"), ImVec2(40, 40), &ClanLogoSelected, 64);
						//Line 17
						Legion::RadioButtonEx("basic_64", ClubsTexture("basic_65"), ImVec2(40, 40), &ClanLogoSelected, 65);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_65", ClubsTexture("basic_66"), ImVec2(40, 40), &ClanLogoSelected, 66);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_66", ClubsTexture("basic_67"), ImVec2(40, 40), &ClanLogoSelected, 67);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_67", ClubsTexture("basic_68"), ImVec2(40, 40), &ClanLogoSelected, 68);
						//Line 18
						Legion::RadioButtonEx("basic_68", ClubsTexture("basic_69"), ImVec2(40, 40), &ClanLogoSelected, 69);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_69", ClubsTexture("basic_70"), ImVec2(40, 40), &ClanLogoSelected, 70);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_70", ClubsTexture("basic_71"), ImVec2(40, 40), &ClanLogoSelected, 71);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_71", ClubsTexture("basic_72"), ImVec2(40, 40), &ClanLogoSelected, 72);
						//Line 19
						Legion::RadioButtonEx("basic_72", ClubsTexture("basic_73"), ImVec2(40, 40), &ClanLogoSelected, 73);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_73", ClubsTexture("basic_74"), ImVec2(40, 40), &ClanLogoSelected, 74);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_74", ClubsTexture("basic_75"), ImVec2(40, 40), &ClanLogoSelected, 75);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_75", ClubsTexture("basic_76"), ImVec2(40, 40), &ClanLogoSelected, 76);
						//Line 20
						Legion::RadioButtonEx("basic_76", ClubsTexture("basic_77"), ImVec2(40, 40), &ClanLogoSelected, 77);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_77", ClubsTexture("basic_78"), ImVec2(40, 40), &ClanLogoSelected, 78);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_78", ClubsTexture("basic_79"), ImVec2(40, 40), &ClanLogoSelected, 79);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_79", ClubsTexture("basic_80"), ImVec2(40, 40), &ClanLogoSelected, 80);
						//Line 21
						Legion::RadioButtonEx("basic_80", ClubsTexture("basic_81"), ImVec2(40, 40), &ClanLogoSelected, 81);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_81", ClubsTexture("basic_82"), ImVec2(40, 40), &ClanLogoSelected, 82);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_82", ClubsTexture("basic_83"), ImVec2(40, 40), &ClanLogoSelected, 83);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_83", ClubsTexture("basic_84"), ImVec2(40, 40), &ClanLogoSelected, 84);
						//Line 22
						Legion::RadioButtonEx("basic_84", ClubsTexture("basic_85"), ImVec2(40, 40), &ClanLogoSelected, 85);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_85", ClubsTexture("basic_86"), ImVec2(40, 40), &ClanLogoSelected, 86);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_86", ClubsTexture("basic_87"), ImVec2(40, 40), &ClanLogoSelected, 87);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_87", ClubsTexture("basic_88"), ImVec2(40, 40), &ClanLogoSelected, 88);
						//Line 23
						Legion::RadioButtonEx("basic_88", ClubsTexture("basic_89"), ImVec2(40, 40), &ClanLogoSelected, 89);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_89", ClubsTexture("basic_90"), ImVec2(40, 40), &ClanLogoSelected, 90);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_90", ClubsTexture("basic_91"), ImVec2(40, 40), &ClanLogoSelected, 91);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_91", ClubsTexture("basic_92"), ImVec2(40, 40), &ClanLogoSelected, 92);
						//Line 24
						Legion::RadioButtonEx("basic_92", ClubsTexture("basic_93"), ImVec2(40, 40), &ClanLogoSelected, 93);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_93", ClubsTexture("basic_94"), ImVec2(40, 40), &ClanLogoSelected, 94);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_94", ClubsTexture("basic_95"), ImVec2(40, 40), &ClanLogoSelected, 95);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_95", ClubsTexture("basic_96"), ImVec2(40, 40), &ClanLogoSelected, 96);
						//Line 25
						Legion::RadioButtonEx("basic_96", ClubsTexture("basic_97"), ImVec2(40, 40), &ClanLogoSelected, 97);
						Legion::SameLine(51);
						Legion::RadioButtonEx("basic_97", ClubsTexture("basic_98"), ImVec2(40, 40), &ClanLogoSelected, 98);
						Legion::SameLine(94);
						Legion::RadioButtonEx("basic_98", ClubsTexture("basic_99"), ImVec2(40, 40), &ClanLogoSelected, 99);
						Legion::SameLine(137);
						Legion::RadioButtonEx("basic_99", ClubsTexture("basic_100"), ImVec2(40, 40), &ClanLogoSelected, 100);
						//Line 26
						//Legion::RadioButtonEx("basic_100", ClubsTexture("basic_100"), ImVec2(40, 40), &ClanLogoSelected, 100);

						Legion::EndChild();
						Legion::EndChild();
					}
					Legion::IndentEx(230, -253);
					Legion::TextLabelColor(CalibriFont, 11, "{c1ccdd} PREMIUM.");
					{
						Legion::BeginChildV(BBoxTexture, NULL, NULL, "PREMIUM Clan Logo", ImVec2(200, 235));
						Legion::IndentEx(0, 8);
						Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "PREMIUM LogoS", ImVec2(194, 220));
						Legion::IndentEx(8, 0);
						Legion::RadioButtonEx("logo_0", ClubsTexture("logo_1"), ImVec2(40, 40), &ClanLogoSelected, 102);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_1", ClubsTexture("logo_2"), ImVec2(40, 40), &ClanLogoSelected, 103);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_2", ClubsTexture("logo_3"), ImVec2(40, 40), &ClanLogoSelected, 104);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_3", ClubsTexture("logo_4"), ImVec2(40, 40), &ClanLogoSelected, 105);
						/*Line 2*/
						Legion::RadioButtonEx("logo_4", ClubsTexture("logo_5"), ImVec2(40, 40), &ClanLogoSelected, 106);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_5", ClubsTexture("logo_6"), ImVec2(40, 40), &ClanLogoSelected, 107);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_6", ClubsTexture("logo_7"), ImVec2(40, 40), &ClanLogoSelected, 108);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_7", ClubsTexture("logo_8"), ImVec2(40, 40), &ClanLogoSelected, 109);
						/*Line 3*/
						Legion::RadioButtonEx("logo_8", ClubsTexture("logo_9"), ImVec2(40, 40), &ClanLogoSelected, 110);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_9", ClubsTexture("logo_10"), ImVec2(40, 40), &ClanLogoSelected, 111);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_10", ClubsTexture("logo_11"), ImVec2(40, 40), &ClanLogoSelected, 112);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_11", ClubsTexture("logo_12"), ImVec2(40, 40), &ClanLogoSelected, 113);
						/*Line 4*/
						Legion::RadioButtonEx("logo_12", ClubsTexture("logo_13"), ImVec2(40, 40), &ClanLogoSelected, 114);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_13", ClubsTexture("logo_14"), ImVec2(40, 40), &ClanLogoSelected, 115);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_14", ClubsTexture("logo_15"), ImVec2(40, 40), &ClanLogoSelected, 116);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_15", ClubsTexture("logo_16"), ImVec2(40, 40), &ClanLogoSelected, 117);
						/*Line 5*/
						Legion::RadioButtonEx("logo_16", ClubsTexture("logo_17"), ImVec2(40, 40), &ClanLogoSelected, 118);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_17", ClubsTexture("logo_18"), ImVec2(40, 40), &ClanLogoSelected, 119);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_18", ClubsTexture("logo_19"), ImVec2(40, 40), &ClanLogoSelected, 120);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_19", ClubsTexture("logo_20"), ImVec2(40, 40), &ClanLogoSelected, 121);
						/*Line 6*/
						Legion::RadioButtonEx("logo_20", ClubsTexture("logo_21"), ImVec2(40, 40), &ClanLogoSelected, 122);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_21", ClubsTexture("logo_22"), ImVec2(40, 40), &ClanLogoSelected, 123);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_22", ClubsTexture("logo_23"), ImVec2(40, 40), &ClanLogoSelected, 124);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_23", ClubsTexture("logo_24"), ImVec2(40, 40), &ClanLogoSelected, 125);
						/*Line 7*/
						Legion::RadioButtonEx("logo_24", ClubsTexture("logo_25"), ImVec2(40, 40), &ClanLogoSelected, 126);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_25", ClubsTexture("logo_26"), ImVec2(40, 40), &ClanLogoSelected, 127);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_26", ClubsTexture("logo_27"), ImVec2(40, 40), &ClanLogoSelected, 128);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_27", ClubsTexture("logo_28"), ImVec2(40, 40), &ClanLogoSelected, 129);
						/*Line 8*/
						Legion::RadioButtonEx("logo_28", ClubsTexture("logo_29"), ImVec2(40, 40), &ClanLogoSelected, 130);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_29", ClubsTexture("logo_30"), ImVec2(40, 40), &ClanLogoSelected, 131);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_30", ClubsTexture("logo_31"), ImVec2(40, 40), &ClanLogoSelected, 132);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_31", ClubsTexture("logo_32"), ImVec2(40, 40), &ClanLogoSelected, 133);
						/*Line 9*/
						Legion::RadioButtonEx("logo_32", ClubsTexture("logo_33"), ImVec2(40, 40), &ClanLogoSelected, 134);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_33", ClubsTexture("logo_34"), ImVec2(40, 40), &ClanLogoSelected, 135);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_34", ClubsTexture("logo_35"), ImVec2(40, 40), &ClanLogoSelected, 136);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_35", ClubsTexture("logo_36"), ImVec2(40, 40), &ClanLogoSelected, 137);
						/*Line 10*/
						Legion::RadioButtonEx("logo_36", ClubsTexture("logo_37"), ImVec2(40, 40), &ClanLogoSelected, 138);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_37", ClubsTexture("logo_38"), ImVec2(40, 40), &ClanLogoSelected, 139);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_38", ClubsTexture("logo_39"), ImVec2(40, 40), &ClanLogoSelected, 140);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_39", ClubsTexture("logo_40"), ImVec2(40, 40), &ClanLogoSelected, 141);
						/*Line 11*/
						Legion::RadioButtonEx("logo_40", ClubsTexture("logo_41"), ImVec2(40, 40), &ClanLogoSelected, 142);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_41", ClubsTexture("logo_42"), ImVec2(40, 40), &ClanLogoSelected, 143);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_42", ClubsTexture("logo_43"), ImVec2(40, 40), &ClanLogoSelected, 144);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_43", ClubsTexture("logo_44"), ImVec2(40, 40), &ClanLogoSelected, 145);
						/*Line 12*/
						Legion::RadioButtonEx("logo_44", ClubsTexture("logo_45"), ImVec2(40, 40), &ClanLogoSelected, 146);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_45", ClubsTexture("logo_46"), ImVec2(40, 40), &ClanLogoSelected, 147);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_46", ClubsTexture("logo_47"), ImVec2(40, 40), &ClanLogoSelected, 148);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_47", ClubsTexture("logo_48"), ImVec2(40, 40), &ClanLogoSelected, 149);
						/*Line 13*/
						Legion::RadioButtonEx("logo_48", ClubsTexture("logo_49"), ImVec2(40, 40), &ClanLogoSelected, 150);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_49", ClubsTexture("logo_50"), ImVec2(40, 40), &ClanLogoSelected, 151);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_50", ClubsTexture("logo_51"), ImVec2(40, 40), &ClanLogoSelected, 152);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_51", ClubsTexture("logo_52"), ImVec2(40, 40), &ClanLogoSelected, 153);
						/*Line 14*/
						Legion::RadioButtonEx("logo_52", ClubsTexture("logo_53"), ImVec2(40, 40), &ClanLogoSelected, 154);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_53", ClubsTexture("logo_54"), ImVec2(40, 40), &ClanLogoSelected, 155);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_54", ClubsTexture("logo_55"), ImVec2(40, 40), &ClanLogoSelected, 156);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_55", ClubsTexture("logo_56"), ImVec2(40, 40), &ClanLogoSelected, 157);
						/*Line 15*/
						Legion::RadioButtonEx("logo_56", ClubsTexture("logo_57"), ImVec2(40, 40), &ClanLogoSelected, 158);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_57", ClubsTexture("logo_58"), ImVec2(40, 40), &ClanLogoSelected, 159);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_58", ClubsTexture("logo_59"), ImVec2(40, 40), &ClanLogoSelected, 160);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_59", ClubsTexture("logo_60"), ImVec2(40, 40), &ClanLogoSelected, 161);
						/*Line 16*/
						Legion::RadioButtonEx("logo_60", ClubsTexture("logo_61"), ImVec2(40, 40), &ClanLogoSelected, 162);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_61", ClubsTexture("logo_62"), ImVec2(40, 40), &ClanLogoSelected, 163);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_62", ClubsTexture("logo_63"), ImVec2(40, 40), &ClanLogoSelected, 164);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_63", ClubsTexture("logo_64"), ImVec2(40, 40), &ClanLogoSelected, 165);
						/*Line 17*/
						Legion::RadioButtonEx("logo_64", ClubsTexture("logo_65"), ImVec2(40, 40), &ClanLogoSelected, 166);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_65", ClubsTexture("logo_66"), ImVec2(40, 40), &ClanLogoSelected, 167);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_66", ClubsTexture("logo_67"), ImVec2(40, 40), &ClanLogoSelected, 168);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_67", ClubsTexture("logo_68"), ImVec2(40, 40), &ClanLogoSelected, 169);
						/*Line 18*/
						Legion::RadioButtonEx("logo_68", ClubsTexture("logo_69"), ImVec2(40, 40), &ClanLogoSelected, 170);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_69", ClubsTexture("logo_70"), ImVec2(40, 40), &ClanLogoSelected, 171);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_70", ClubsTexture("logo_71"), ImVec2(40, 40), &ClanLogoSelected, 172);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_71", ClubsTexture("logo_72"), ImVec2(40, 40), &ClanLogoSelected, 173);
						/*Line 19*/
						Legion::RadioButtonEx("logo_72", ClubsTexture("logo_73"), ImVec2(40, 40), &ClanLogoSelected, 174);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_73", ClubsTexture("logo_74"), ImVec2(40, 40), &ClanLogoSelected, 175);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_74", ClubsTexture("logo_75"), ImVec2(40, 40), &ClanLogoSelected, 176);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_75", ClubsTexture("logo_76"), ImVec2(40, 40), &ClanLogoSelected, 177);
						/*Line 20*/
						Legion::RadioButtonEx("logo_76", ClubsTexture("logo_77"), ImVec2(40, 40), &ClanLogoSelected, 178);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_77", ClubsTexture("logo_78"), ImVec2(40, 40), &ClanLogoSelected, 179);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_78", ClubsTexture("logo_79"), ImVec2(40, 40), &ClanLogoSelected, 180);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_79", ClubsTexture("logo_80"), ImVec2(40, 40), &ClanLogoSelected, 181);
						/*Line 21*/
						Legion::RadioButtonEx("logo_80", ClubsTexture("logo_81"), ImVec2(40, 40), &ClanLogoSelected, 182);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_81", ClubsTexture("logo_82"), ImVec2(40, 40), &ClanLogoSelected, 183);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_82", ClubsTexture("logo_83"), ImVec2(40, 40), &ClanLogoSelected, 184);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_83", ClubsTexture("logo_84"), ImVec2(40, 40), &ClanLogoSelected, 185);
						/*Line 22*/
						Legion::RadioButtonEx("logo_84", ClubsTexture("logo_85"), ImVec2(40, 40), &ClanLogoSelected, 186);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_85", ClubsTexture("logo_86"), ImVec2(40, 40), &ClanLogoSelected, 187);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_86", ClubsTexture("logo_87"), ImVec2(40, 40), &ClanLogoSelected, 188);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_87", ClubsTexture("logo_88"), ImVec2(40, 40), &ClanLogoSelected, 189);
						/*Line 23*/
						Legion::RadioButtonEx("logo_88", ClubsTexture("logo_89"), ImVec2(40, 40), &ClanLogoSelected, 190);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_89", ClubsTexture("logo_90"), ImVec2(40, 40), &ClanLogoSelected, 191);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_90", ClubsTexture("logo_91"), ImVec2(40, 40), &ClanLogoSelected, 192);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_91", ClubsTexture("logo_92"), ImVec2(40, 40), &ClanLogoSelected, 193);
						/*Line 24*/
						Legion::RadioButtonEx("logo_92", ClubsTexture("logo_93"), ImVec2(40, 40), &ClanLogoSelected, 194);
						Legion::SameLine(51);
						Legion::RadioButtonEx("logo_93", ClubsTexture("logo_94"), ImVec2(40, 40), &ClanLogoSelected, 195);
						Legion::SameLine(94);
						Legion::RadioButtonEx("logo_94", ClubsTexture("logo_95"), ImVec2(40, 40), &ClanLogoSelected, 196);
						Legion::SameLine(137);
						Legion::RadioButtonEx("logo_95", ClubsTexture("logo_96"), ImVec2(40, 40), &ClanLogoSelected, 197);
						/*Line 25*/
						Legion::RadioButtonEx("logo_96", ClubsTexture("logo_97"), ImVec2(40, 40), &ClanLogoSelected, 198);

						Legion::EndChild();
						Legion::EndChild();
					}
					Legion::EndChild();
				}
				Legion::IndentEx(-3, 0);
				Legion::ImageButtonVC("CLAN BUTTON", "CLAN", "", "", "", true, &ClanButtonMenu, 0, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 29), CalibriFont, 15.4f);
				Legion::SameLine(130);
				if (Legion::ImageButtonEx("UNION Button", "UNION", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 29), CalibriFont, 15.4f))
				{
					SendInfoToServer("c69299fe47", "UNI", "UIN");
				}
				Legion::SameLine(242);
				Legion::ImageButtonVC("MANAGER Button", "MANAGER", "", "", "", true, &ClanButtonMenu, 5, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 29), CalibriFont, 15.4f);
				Legion::SameLine(355);
				if (Legion::ImageButtonEx("RANK Button", "RANK", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 30), CalibriFont, 15.4f))
				{
					SendInfoToServer("3d52c59e59", "RNK", "DNK");
				}
				Legion::SameLine(467);
				if (Legion::ImageButtonEx("HISTORY Button", "HISTORY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(108, 29), CalibriFont, 15.4f))
				{
					SendInfoToServer("8c94e906f6", "HIST", "HIS");
				}
			}
			Legion::End();
		}

		if (NewClanInfoSystemUI)
		{
			Legion::BeginEx(ClanUITexture, NULL, NULL, "Clan System", ImVec2(600, 400), ImVec2(0.5f, 0.5f), &NewClanInfoSystemUI, LegionWindowFlags_Custom);
			Legion::IndentEx(568, -12);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
			{
				NewClanInfoSystemUI = false;
				NewClanSystemUI = true;
			}
			Legion::IndentEx(-566, -5);
			Legion::ImageV("Clan Mark", ClanInfoMarkTexture, ImVec2(50, 50));
			Legion::IndentEx(60.5, -55);
			Legion::TextLabelColor(CalibriFont, 24, ("#" + ClanInfoRank).c_str());
			Legion::IndentEx(81, -21);
			Legion::ProgressBarEx(CalibriFont, 12, "Clan EXP", ClanInfoEXP, 0, TotalClanInfoExp, 1, ImVec2(408, 10), ImColor(249, 249, 249), ImColor(74, 170, 76), false, true);
			Legion::IndentEx(-80, 5);
			Legion::TextLabelColor(CalibriFont, 16, ClanInfoName.c_str());
			Legion::TextLabelColor(CalibriFont, 12, ("LEVEL.{4aaa4c}" + ClanInfoLevel).c_str());
			Legion::SameLine(150);
			Legion::TextLabelColor(CalibriFont, 12, ("POINTS.{4aaa4c}" + ClanInfoPoints).c_str());
			Legion::SameLine(260);
			Legion::TextLabelColor(CalibriFont, 12, ("Battals " + ClanInfoBattels).c_str());
			Legion::SameLine(360);
			Legion::TextLabelColor(CalibriFont, 12, ("Members  " + std::to_string(ClubInfoMemberList.size()) + "/" + std::to_string(((ClanInfoLevel.empty()) ? 1 : std::stoi(ClanInfoLevel)) * 12)).c_str());
			Legion::IndentEx(-51, 10);
			Legion::ImageButtonEx("Clan Notice", ClanInfoTitle.c_str(), true, ListBGTexture, ListBGTexture, ListBGTexture, ImVec2(550, 31), CalibriFont, 15.4f);
			Legion::IndentEx(3, 3);
			Legion::BeginChildV(ListBGTexture, ScrollTexture, ScrollBarTexture, "Clan List", ImVec2(557, 213));
			Legion::IndentEx(0, 3);
			Legion::TextLabelColor(CalibriFont, 15.35f, "         NICKNAME        LEVEL              LOCATION                    RANK           POINTS           BATTELS");
			for (std::pair<unsigned int, ClanMemberListClass*> it : ClubInfoMemberList)
			{
				if (Legion::NewClanPlayerList(it.second->Nickname, it.second->Level.c_str(), it.second->Location, it.second->Rank.c_str(), it.second->Points.c_str(), it.second->Battles.c_str(), ClanSelectionTexture, OnlineTexture, OfflineTexture, it.second->PlayerStatus.c_str(), &ClanPlayerId, it.first, CalibriFont, 15.35f))
				{
					ClanInfoSelectedPlayer = it.first;
					ClanInfoSelectedPlayerId = it.second->PlayerId;
					ClanInfoSelectedPlayerNickName = it.second->Nickname;
					ClanInfoSelectedFriendStats = it.second->FriendStats;
					ClanSelectedPlayerRank = it.second->Rank;
				}

				if (MyPlayerId != ClanSelectedPlayerId)
				{
					std::string nickname;
					if (ClanInfoSelectedPlayerNickName.find("{") != std::string::npos)
						nickname = Mem.Split(ClanInfoSelectedPlayerNickName, '}')[1];
					else
						nickname = ClanInfoSelectedPlayerNickName;

					if (Legion::BeginPopupContextWindowV(ContextTexture, NULL, NULL, "Clan List Context", LegionMouseButton_Right))
					{
						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "User Info", NULL))
						{
							UserInfo_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA2C), ClanSelectedPlayerId, 0);
						}

						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Whisper", NULL))
						{
							ChatButtonTexture(Chat_Type::Whisper);

							memset(ChatSystemMessage, 0, sizeof ChatSystemMessage);

							std::string whisper;
							if (ClanSelectedPlayerNickName.find("{") != std::string::npos)
								whisper = "To " + Mem.Split(ClanSelectedPlayerNickName, '}')[1] + " : "/*+ " : "*/;
							else
								whisper = "To " + Mem.Split(ClanSelectedPlayerNickName, '}')[0] + " : "/*+ " : "*/;

							std::copy(whisper.begin(), whisper.end(), ChatSystemMessage);
						}

						if (Legion::MenuItemEx(CalibriFont, 12, NULL, ClanInfoSelectedFriendStats.c_str(), NULL))
						{
							if (ClanSelectedFriendStats == "Add Friend")
							{
								CustomMessageBox("Would you like to send\n a friend request to " + nickname, 2, Msgbox::FriendRequest);
							}
							else if (ClanSelectedFriendStats == "Delete a Friend")
							{
								CustomMessageBox("Would you like to delete '" + nickname + "'\n from your friend list?", 2, Msgbox::FriendRequest);
							}
						}

						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Exchange", NULL))
						{
							strcpy_s(ExcFindPlayer, nickname.length() + 1, nickname.c_str());
							CustomMessageBox("Would you like to start exchange\n with " + nickname + "?", 2, Msgbox::ExchangeRequest);
						}

						if (Legion::MenuItemEx(CalibriFont, 12, NULL, "Challenge", NULL))
						{
							strcpy_s(ChalFindPlayer, nickname.length() + 1, nickname.c_str());
							MenuSelected = 8;
							CMainMenuUI = true;
						}
						Legion::EndPopup();
					}
				}
			}
			Legion::EndChild();

			if (Legion::ImageButtonEx("REQUEST JOIN Button", "REQUEST JOIN", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(555, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("ec87544526", "UNI", ClanInfoName);
			}
			Legion::End();
		}

		if (NewClanRankChangeUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Clan Change Player Rank", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &NewClanRankChangeUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewClanRankChangeUI = false;
			Legion::IndentEx(-190, -15);
			Legion::TextLabelColor(CalibriFont, 20, "CHANGE RANK");
			Legion::IndentEx(-80, 9);
			Legion::TextLabelColor(CalibriFont, 16, ClanSelectedPlayerNickName.c_str());
			Legion::IndentEx(-10, -1);
			if (Legion::BeginComboV(CalibriFont, 13, "Rank ComboBox", ClanSelectedPlayerRank.c_str(), ImVec2(290, 32), SmallBoxTexture, NormalBoxTexture, ComboArrowTexture, ScrollTexture, ScrollBarTexture))
			{
				for (auto it : ClanRanksList)
				{
					if (Legion::SelectableExV("Item", CalibriFont, 13, it.c_str()))
						ClanSelectedPlayerRank = it;
				}
				Legion::EndCombo();
			}
			Legion::IndentEx(90, -15);
			if (Legion::ImageButtonEx("APPLY Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("c266d62c20", ClanSelectedPlayerRank, std::to_string(ClanSelectedPlayerId));

				auto it = ClubMemberList.find(ClanSelectedPlayer);
				if (it != ClubMemberList.end())
				{
					it->second->Rank = ClanSelectedPlayerRank;
				}

				NewClanRankChangeUI = false;
			}
		}

		if (NewClanNameChangeUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Clan Change Name", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &NewClanNameChangeUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewClanNameChangeUI = false;
			Legion::IndentEx(-195, -15);
			Legion::TextLabelColor(CalibriFont, 18, "{ffff}NEW CLAN NAME");
			Legion::IndentEx(-80, 9);
			Legion::InputTextV("Clan New Name Box", CalibriFont, 11, SmallBoxTexture, "", ClanNewName, IM_ARRAYSIZE(ClanNewName), ImVec2(287, 33), LegionInputTextFlags_Multiline);
			Legion::TextLabelColor(CalibriFont, 12, "PRICE");
			Legion::IndentEx(233, -17);
			Legion::TextLabelColor(CalibriFont, 12, "{b3a20f}15.000");
			Legion::IndentEx(40, -17);
			Legion::ImageV("LP Icon", EvTextture, ImVec2(15.12f, 15.12f));
			Legion::IndentEx(-190, 12);
			if (Legion::ImageButtonEx("APPLY Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("d3a5e89888", "CNN", ClanNewName);
				NewClanNameChangeUI = false;
			}
		}

		if (NewClanInviteUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Clan Union Invite", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &NewClanInviteUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewClanInviteUI = false;
			Legion::IndentEx(-195, -15);
			Legion::TextLabelColor(CalibriFont, 18, "{ffff}INVITE CLAN UNION");
			Legion::IndentEx(-80, 9);
			Legion::InputTextV("Clan Union Name Box", CalibriFont, 11, SmallBoxTexture, "", InvUnionName, IM_ARRAYSIZE(InvUnionName), ImVec2(287, 33), LegionInputTextFlags_Multiline);
			Legion::IndentEx(90, 17);
			if (Legion::ImageButtonEx("APPLY Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("3ae2cc61d8", "UNI", InvUnionName);
				NewClanInviteUI = false;
			}
		}

		if (NewCreateClanUI)
		{
			Legion::BeginEx(ClanFreeUITexture, NULL, NULL, "Clan Change Name", ImVec2(316, 148), ImVec2(0.5f, 0.5f), &NewCreateClanUI, LegionWindowFlags_Custom);
			Legion::IndentEx(282, -10);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(17, 17), CalibriFont, 11))
				NewCreateClanUI = false;
			Legion::IndentEx(-195, -15);
			Legion::TextLabelColor(CalibriFont, 18, "{ffff}CLAN NAME");
			Legion::IndentEx(-80, 9);
			Legion::InputTextV("Clan Name Box", CalibriFont, 11, SmallBoxTexture, "", CreateClanName, IM_ARRAYSIZE(CreateClanName), ImVec2(287, 30));
			Legion::IndentEx(85, 12);
			if (Legion::ImageButtonEx("APPLY Button", "APPLY", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(110, 29), CalibriFont, 15.4f))
			{
				SendInfoToServer("0ac9fe6491", "CNN", CreateClanName);
				NewCreateClanUI = false;
			}
		}


		//if (RoomSearch)
		//{
		//	LegionIO& io = Legion::GetIO();
		//	ImVec2 ScreenSize = io.DisplaySize;
		//	Legion::SetNextWindowPos(ImVec2(ScreenSize.x * 0.8f, ScreenSize.y * 0.9f), 0, ImVec2(0.0f, 0.0f));
		//	Legion::BeginEx(NULL, NULL, NULL, "Shop Item Search", ImVec2(110, 20), ImVec2(0, 0), &ShopSearch, LegionWindowFlags_Custom);

		//	if (Legion::InputTextWithHintV("ShopItemSearchTextBox", CalibriFont, 11, SmallBoxTexture, "", "Item Name", ShopItemsSearch, IM_ARRAYSIZE(ShopItemsSearch), ImVec2(100, 17), LegionInputTextFlags_EnterReturnsTrue))
		//	{
		//		SendInfoToServer("614440bde9", "SIS", ShopItemsSearch);
		//		//	memset(ShopItemsSearch, 0, sizeof ShopItemsSearch);
		//	}
		//	Legion::End();
		//}

		//if (RoomSearchUI)
		//{
		//	Legion::BeginEx(ShopUITexture, NULL, NULL, "Shop Item Search UI", ImVec2(660, 450), ImVec2(0.5f, 0.5f), &ShopSearchUI, LegionWindowFlags_Custom);
		//	Legion::BeginChildV(BBoxTexture, NULL, NULL, "Item Search Gruop", ImVec2(650, 350));
		//	Legion::IndentEx(0, 10);
		//	Legion::BeginChildV(NULL, NULL, NULL, "Item Search Gruop", ImVec2(650, 330));
		//	{


		//	}
		//}

		if (VoteKickUI)
		{
			Legion::BeginEx(VoteKickPlayerUITexture, NULL, NULL, "Player List", ImVec2(541, 360), ImVec2(0.5f, 0.5f), &VoteKickUI, LegionWindowFlags_Custom);
			Legion::IndentEx(505, -8);
			if (Legion::ImageButtonEx("Exit", "Button", false, ExitButtonTexture, ExitButtonTexture, ExitButtonTexture, ImVec2(18, 18), CalibriFont, 11))
			{
				Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);
				VoteKickUI = false;
			}
			Legion::IndentEx(-493, -8);
			Legion::TextLabelColor(CalibriFont, 11, "");//{c1ccdd}SELECT A PLAYER.
			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Vote Kick List", ImVec2(234, 200));
			Legion::IndentEx(0, 3);
			for (std::pair<unsigned int, PlayerShortInfoListClass*> it : VoteKickList)
			{
				if (Legion::VoteKickPlayer(it.second->Name, CalibriFont, 12, &VoteKickId, it.first, ClanSelectionTexture, CPlayerListTexture(it.second->Level, 0)))
				{
					VoteKickSelectedPlayerId = it.second->PlayerId;
				}
			}
			Legion::EndChild();
			Legion::IndentEx(258, -220);
			Legion::TextLabelColor(CalibriFont, 11, "");//{c1ccdd}REASONS.
			Legion::IndentEx(9, 2);
			Legion::BeginChildV(NULL, ScrollTexture, ScrollBarTexture, "Vote Kick Reasons", ImVec2(234, 200));
			Legion::IndentEx(-1, 3);
			for (std::pair<unsigned int, std::string> it : VoteKickReasons)
			{
				if (Legion::VoteKickReasonsPlayer(it.second.c_str(), CalibriFont, 12, &VoteKickRId, it.first, ClanSelectionTexture))
				{
					VoteKickReasonSelected = it.first;
				}
			}
			Legion::EndChild();
			Legion::IndentEx(-80, 30);
			if (Legion::ImageButtonEx("VOTE", "VOTE", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(135, 36), CalibriFont, 15))
			{
				int Reason;
				if (VoteKickReasonSelected > 4)
					Reason = 5;
				else
					Reason = VoteKickReasonSelected;

				KickVote_Req(reinterpret_cast<uintptr_t*>(dwBaseAddr + 0x173DA68), MyPlayerId, 0, VoteKickSelectedPlayerId, 0, Reason);
			}
			Legion::IndentEx(27, 15);
			Legion::TextLabelColor(CalibriFont, 16, "{c1ccdd}VOTE KICK");
			Legion::End();
		}

		if (AlertUI)
		{
			Legion::BeginEx(NULL, NULL, NULL, "Alert System", ImVec2(200, 30), ImVec2(0.5f, 0.3f), &VoteKickUI, LegionWindowFlags_Custom);


			if (AlertEffectTimer->elapsedSeconds() > 2)
			{
				if (AlertOpacity > 0)
				{
					AlertOpacity = AlertOpacity - 0.2f;
					AlertEffectTimer->reset();
				}
				else
				{
					AlertOpacity = 1.0f;
				}
			}
			Legion::TextColored(CalibriFont, 16, ImVec4(27.0f / 255.0f, 184.0f / 255.0f, 245.0f / 255.0f, AlertOpacity), "Warring your HP is low");
			Legion::End();
		}

		if (CActionsUI)
		{
			if (VoiceActionMenuTimer->elapsedSeconds() > 6)
			{
				Mem.WriteASM(WeaponSwitch_Offset, (DWORD)"\x74", 1);

				CActionsUI = false;
			}

			LegionIO& io = Legion::GetIO();
			ImVec2 screen = io.DisplaySize;
			float font_size;
			if (screen.x >= 800 && screen.x <= 1280)
				font_size = 11;
			else if (screen.x > 1280 && screen.x <= 1440)
				font_size = 12;
			else
				font_size = 13;

			Mem.WriteASM(WeaponSwitch_Offset, (DWORD)"\xEB", 1);// to do

			Legion::BeginEx(NULL, NULL, NULL, "Game Actions", ImVec2(200, 200), ImVec2(0.01f, 0.4f), &CActionsUI, LegionWindowFlags_Custom);
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 1.Go Go Go!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 2.Attack!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 3.Defend!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 4.Wait me!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 5.Run!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 6.Watch your back!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 7.Cover me!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 8.Out of ammo!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 9.Heal me!");
			Legion::TextLabelColor(CalibriFont, font_size, "{c29e90} 0.Cancel");
			Legion::End();
		}

		if (CVoiceChatUI)
		{
			Legion::BeginEx(NULL, NULL, NULL, "Voice Chat", ImVec2(366, 50), ImVec2(0.5f, 0.2f), &CVoiceChatUI, LegionWindowFlags_Custom | LegionWindowFlags_NoFocusOnAppearing);
			Legion::LabelEx("Surrender Message", "{ebb434}Voice Chat is activating press F12 to turn it off", ImVec2(366 - 10, 40), CalibriFont, Legion::GetIO().DisplaySize.x * 0.0138f);
			Legion::End();
		}

		//VoiceAction();
		//CustomAction();

		if (!CGameMenuUI && !CMainMenuUI && !CPlayerReportUI && !VoteKickUI && !CMessageBoxUI)
			Mem.WriteASM(Mem.FindOffAAddy(GameManager, { 0xAC, 0x48, 0x8 }), (DWORD)"\x00", 1);

		if (CCustomNameTagUI || CommunityUI || CGameShopUI || ShopSearchUI || CMainMenuUI || CGameMenuUI || NewClanSystemUI || NewClanInfoSystemUI || NewClanRankChangeUI || NewClanNameChangeUI || NewClanInviteUI || NewCreateClanUI || AddNewProfileUI || CharactersProfileUI)
			InputBlock = true;
		else
			InputBlock = false;

	}
	catch (std::exception& e)
	{
		//LogCrash("Main Func \n" + std::string(e.what()) + "\n" + boost::current_exception_diagnostic_information());
	}
	catch (...)
	{
		//LogCrash("Main Func  \n" + boost::current_exception_diagnostic_information());
	}
}
#pragma endregion

HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pReset)
{
	Legion_ImplDX9_InvalidateDeviceObjects();
	Legion_ImplDX9_CreateDeviceObjects();

	return oReset(pDevice, pReset);
}

std::vector<char> LoadSound(const char* filepath)
{
	if (!FileExistsA(filepath))
		return std::vector<char>();

	std::ifstream ifd(filepath, std::ios::binary | std::ios::ate);

	size_t len = (size_t)ifd.tellg();
	ifd.seekg(0, std::ios::beg);
	std::vector<char> buffer;
	buffer.resize(len);
	ifd.read(buffer.data(), len);

	return buffer;
}

HRESULT APIENTRY Hooked_EndScene(IDirect3DDevice9* pDevice)
{

	if (!XINIT)
		ActiveS4Stuff();


	if (!MenuInit)
	{

		Legion::CreateContext();
		LegionIO& io = Legion::GetIO();

		pD3DDevice = pDevice;

		CalibriFont = LoadFontTextrue("UI\\Joan.ttf", 11);
		Ebrima = LoadFontTextrue("UI\\Roboto.ttf", 11);

		Legion_ImplWin32_Init(S4Window);
		Legion_ImplDX9_Init(pDevice);
		Legion_ImplDX9_CreateDeviceObjects();

		ChannelFreeTexture = LoadTextrueEx("FreeChannel");
		ChannelFreeOnTexture = LoadTextrueEx("FreeOnChannel");
		CompetitiveTexture = LoadTextrueEx("CompetivieChannel");
		CompetitiveOnTexture = LoadTextrueEx("CompetivieOnChannel");
		ClanBattelsTexture = LoadTextrueEx("ClanChannel");
		ClanBattelsOnTexture = LoadTextrueEx("ClanOnChannel");
		RankChallengeTexture = LoadTextrueEx("RankChannel");
		RankChallengeOnTexture = LoadTextrueEx("RankonChannel");
		ChannelBoredTexture = LoadTextrueEx("HeaderChannel");
		MainBGTexture = LoadTextrueEx("MainBG");

		ShopUITexture = LoadTextrueEx("ShopUI");
		LeftArrowTexture = LoadTextrueEx("Left Arrow");
		RightArrowTexture = LoadTextrueEx("Right Arrow");
		ClaimMarkTexture = LoadTextrueEx("Claim");
		DailyEventUITexture = LoadTextrueEx("DailyReward");
		SmallBoxTexture = LoadTextrueEx("Small Box");
		ComboArrowTexture = LoadTextrueEx("Combo Arrow");
		ScrollTexture = LoadTextrueEx("Scroll");
		ScrollBarTexture = LoadTextrueEx("ScrollBar");
		ImageTexture = LoadTextrueEx("Image Box");
		ExitButtonTexture = LoadTextrueEx("Exit");
		NormalCheckBoxTexture = LoadTextrueEx("CheckBox");
		ActiveCheckBoxTexture = LoadTextrueEx("Checked");
		NormalBoxTexture = LoadTextrueEx("Box");
		ListBGTexture = LoadTextrueEx("List BG");


		/// <summary>
		/// 
		ProfileButtonTexture = LoadTextrueEx("Profile Buttons");
		PlayerListUITexture = LoadTextrueEx("Player List");
		ContextTexture = LoadTextrueEx("Context");
		ClanSelectionTexture = LoadTextrueEx("Clan Selection");
		CharactersProfileUITexture = LoadTextrueEx("ProfilWindow");
		ClanFreeUITexture = LoadTextrueEx("Clan Free Window");

		BBoxTexture = LoadTextrueEx("BBox");
		RBoxTexture = LoadTextrueEx("RBox");
		MessageBoxUITexture = LoadTextrueEx("MessageBox");
		OnlineTexture = LoadTextrueEx("Online");
		OfflineTexture = LoadTextrueEx("Offline");
		ReportPlayerUITexture = LoadTextrueEx("Player Report");
		RoomFiltersUITexture = LoadTextrueEx("RoomFilters");
		EmptyBoxUITexture = LoadTextrueEx("Empty Box");


		ChatBoxTexture = LoadTextrueEx("Chat Box");
		ChatTextBoxTexture = LoadTextrueEx("Chat TextBox");
		ChatPlayerListButtonTexture = LoadTextrueEx("Chat PlayerList Button");
		PlayerListBoxTexture = LoadTextrueEx("Chat PlayerList");
		PlayerBackGroundTexture = LoadTextrueEx("PlayerList BackGround");
		ChatMainButtonTexture = ChatAllButtonTexture = LoadTextrueEx("Chat All");
		ChatMenuButtonTexture = ChatClanButtonTexture = LoadTextrueEx("Chat Clan");
		ChatMenuButton2Texture = ChatUnionButtonTexture = LoadTextrueEx("Chat Union");
		ChatMenuButton3Texture = ChatWhisperButtonTexture = LoadTextrueEx("Chat Whisper");
		ChatMenuButton4Texture = ChatSupportButtonTexture = LoadTextrueEx("Chat Support");

		EnemySurrender = LoadSound("UI\\EnemySurrender.mp3");
		TeamSurrender = LoadSound("UI\\TeamSurrender.mp3");

		T1 = LoadSound("UI\\T1.mp3");
		T2 = LoadSound("UI\\T2.mp3");
		T3 = LoadSound("UI\\T3.mp3");
		T4 = LoadSound("UI\\T4.mp3");
		T5 = LoadSound("UI\\T5.mp3");
		T6 = LoadSound("UI\\T6.mp3");
		T7 = LoadSound("UI\\T7.mp3");
		T8 = LoadSound("UI\\T8.mp3");
		T9 = LoadSound("UI\\T9.mp3");

		FT1 = LoadSound("UI\\FT1.mp3");
		FT2 = LoadSound("UI\\FT2.mp3");
		FT3 = LoadSound("UI\\FT3.mp3");
		FT4 = LoadSound("UI\\FT4.mp3");
		FT5 = LoadSound("UI\\FT5.mp3");
		FT6 = LoadSound("UI\\FT6.mp3");
		FT7 = LoadSound("UI\\FT7.mp3");
		FT8 = LoadSound("UI\\FT8.mp3");
		FT9 = LoadSound("UI\\FT9.mp3");

		LoadMapInfo();
		LoadGameModeInfo();

		auto it = MapInfo.find(70);
		if (it != MapInfo.end())
		{
			SelectedMapId = it->first;
			SelectedMapTexture = it->second->Texture;
			ASelectedMapTexture = it->second->ActivatedTexture;
			MapInfoSelected = 0;

			ScoreLimit.push_back("4 P ( 10 min )");
			ScoreLimit.push_back("6 P ( 15 min )");
			ScoreLimit.push_back("8 P ( 20 min )");
			ScoreLimit.push_back("10 P ( 30 min )");// this should fix first problem or the 2nd
		}

		auto git = GameModesInfo.find(2);
		if (git != GameModesInfo.end())
		{
			SelectedGameModeTexture = git->second->Texture;
			ASelectedGameModeTexture = git->second->ActivatedTexture;
		}

		MenuInit = true;
	}

	Legion_ImplDX9_NewFrame();
	Legion_ImplWin32_NewFrame();
	Legion::NewFrame();

	Render();

	Legion::EndFrame();
	Legion::Render();
	Legion_ImplDX9_RenderDrawData(Legion::GetDrawData());

	return oEndScene(pDevice);
}

#pragma region WndProc

void ViewMenu()
{
	bool static is_down = false;
	bool static is_clicked = false;

	bool static is_down2 = false;
	bool static is_clicked2 = false;

	bool static is_down3 = false;
	bool static is_clicked3 = false;

	if (GetAsyncKeyState(VK_INSERT))
		InputBlock = false;

	if (plr_loc > Player_Loc::Channel && plr_loc < Player_Loc::Playing)
	{
		if (GetAsyncKeyState(VK_INSERT) && GetFocus())
		{
			is_clicked = false;
			is_down = true;
		}
		else if (!GetAsyncKeyState(VK_INSERT) && is_down && GetFocus())
		{
			is_clicked = true;
			is_down = false;
		}
		else
		{
			is_clicked = false;
			is_down = false;
		}
		if (is_clicked && GetFocus())
		{
			CMainMenuUI = !CMainMenuUI;
		}
	}

	if (plr_loc == Player_Loc::Playing)
	{

		if (GetAsyncKeyState(VK_ESCAPE) && GetFocus())
		{
			is_clicked3 = false;
			is_down3 = true;
		}
		else if (!GetAsyncKeyState(VK_ESCAPE) && is_down3 && GetFocus())
		{
			is_clicked3 = true;
			is_down3 = false;
		}
		else
		{
			is_clicked3 = false;
			is_down3 = false;
		}
		if (is_clicked3 && GetFocus())
		{
			CGameMenuUI = !CGameMenuUI;
		}
	}

	if (plr_loc == Player_Loc::RoomList)
	{

		if (GetAsyncKeyState(VK_F12) && GetFocus())
		{
			is_clicked2 = false;
			is_down2 = true;
		}
		else if (!GetAsyncKeyState(VK_F12) && is_down2 && GetFocus())
		{
			is_clicked2 = true;
			is_down2 = false;
		}
		else
		{
			is_clicked2 = false;
			is_down2 = false;
		}
		if (is_clicked2 && GetFocus())
		{
			CPlayerListUI = !CPlayerListUI;
		}
	}
}

LRESULT CALLBACK WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ViewMenu();

	if (InputBlock && GetFocus())
	{
		Legion_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return 1;
	}

	if (Legion_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return 1;


	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
#pragma endregion

DWORD WINAPI MainThread(LPVOID param)
{
	while (!(S4Window = FindWindowA("S4 Client", NULL))) Sleep(200);


	// loadConfig();

	Mem.WriteMem(FPS_Offset, 8.01282051282051);//5.98659003831418

	if (S4Window)
		oWndProc = (WNDPROC)SetWindowLongPtr(S4Window, GWL_WNDPROC, (LONG_PTR)WndProc);

	if (GetModuleHandleA("d3d9.dll") == NULL)
		return false;

	DWORD* vTable;

	DWORD Adrr = Mem.FindPattern("d3d9.dll", (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");

	if (Adrr == NULL)
	{
		ExitProcess(0);
	}

	if (Adrr)
	{
		Mem.WriteASM((DWORD)&vTable, (Adrr + 2), 4);
		oReset = (Reset_t)DetourFunction((PBYTE)vTable[16], (PBYTE)Hooked_Reset);
		oEndScene = (f_EndScene)DetourFunction((PBYTE)vTable[42], (PBYTE)Hooked_EndScene);

		_endthread();
	}
	return true;
}
//add check later with vps ip so no one else can be inject the dll or use crc but they will be able to use the client so better use vps ip
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		if (!AllocConsole())
		{
			FreeConsole();
			Sleep(100);
			AllocConsole();
		}

			FILE* conin, * conout;
			SetConsoleTitle("Debug");
			freopen_s(&conin, "conin$", "r", stdin);
			freopen_s(&conout, "conout$", "w", stdout);
			freopen_s(&conout, "conout$", "w", stderr);

		Mem.WriteASM(dwBaseAddr + 0x1553470, (DWORD)"\x70", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553498, (DWORD)"\x28", 1);
		Mem.WriteASM(dwBaseAddr + 0x15534C0, (DWORD)"\x68", 1);
		Mem.WriteASM(dwBaseAddr + 0x15534E8, (DWORD)"\x66", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553510, (DWORD)"\x14", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553538, (DWORD)"\xDC", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553560, (DWORD)"\x30", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553588, (DWORD)"\x8D", 1);
		Mem.WriteASM(dwBaseAddr + 0x15535B0, (DWORD)"\xDB", 1);
		Mem.WriteASM(dwBaseAddr + 0x15535D8, (DWORD)"\x69", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553600, (DWORD)"\x43", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553628, (DWORD)"\x11", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553650, (DWORD)"\xE9", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553678, (DWORD)"\xDB", 1);
		Mem.WriteASM(dwBaseAddr + 0x15536A0, (DWORD)"\x37", 1);
		Mem.WriteASM(dwBaseAddr + 0x15536C8, (DWORD)"\x7E", 1);
		Mem.WriteASM(dwBaseAddr + 0x15536F0, (DWORD)"\xE6", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553718, (DWORD)"\x57", 1);
		Mem.WriteASM(dwBaseAddr + 0x1553740, (DWORD)"\x61", 1);

		/*/CSplash splash1(TEXT("Splash.bmp"), RGB(128, 128, 128));
		splash1.ShowSplash();*/
		UnlinkModuleFromPEB(hModule);

		for (int i = 0; i <= 30; i++)
			DailyEventList.push_back(NULL);

		LoadResourceFiles();

		ItemLimit.push_back("Unlimited");
		ItemLimit.push_back("Sword");
		ItemLimit.push_back("Gun Only");

		PlayersLimit.push_back("4");
		PlayersLimit.push_back("8");
		PlayersLimit.push_back("10");
		PlayersLimit.push_back("12");
		PlayersLimit.push_back("16");

		ClanRanksList.push_back("CoMaster");
		ClanRanksList.push_back("Staff");
		ClanRanksList.push_back("Member");

		RoomFilterModes.push_back("Touchdown");
		RoomFilterModes.push_back("Deathmatch");
		RoomFilterModes.push_back("Chaser");
		RoomFilterModes.push_back("BattleRoyal");
		RoomFilterModes.push_back("TouchdownPass");

		TouchdownMaps.push_back("Wonderland");
		TouchdownMaps.push_back("Real Rustygarden");
		TouchdownMaps.push_back("Cyberion");
		TouchdownMaps.push_back("City Square");
		TouchdownMaps.push_back("Station-3");
		TouchdownMaps.push_back("Ice Square");
		TouchdownMaps.push_back("Hyperium");
		TouchdownMaps.push_back("Side-3");
		TouchdownMaps.push_back("Tunnel");
		TouchdownMaps.push_back("Neden-3");
		TouchdownMaps.push_back("Station-2");
		TouchdownMaps.push_back("Station-1");
		TouchdownMaps.push_back("Highway");
		TouchdownMaps.push_back("Old School");
		TouchdownMaps.push_back("Temple-M");
		TouchdownMaps.push_back("Colosseum");

		TouchdownPassMaps.push_back("Hyperium");
		TouchdownPassMaps.push_back("Real Rustygarden");
		TouchdownPassMaps.push_back("Cyberion");
		TouchdownPassMaps.push_back("City Square");
		TouchdownPassMaps.push_back("Wonderland");
		TouchdownPassMaps.push_back("Station-3");
		TouchdownPassMaps.push_back("Ice Square");
		TouchdownPassMaps.push_back("Side-3");
		TouchdownPassMaps.push_back("Tunnel");
		TouchdownPassMaps.push_back("Neden-3");
		TouchdownPassMaps.push_back("Station-2");
		TouchdownPassMaps.push_back("Station-1");
		TouchdownPassMaps.push_back("Highway");
		TouchdownPassMaps.push_back("Old School");
		TouchdownPassMaps.push_back("Temple-M");
		TouchdownPassMaps.push_back("Colosseum");

		DeathmatchMaps.push_back("Bladecity");
		DeathmatchMaps.push_back("Warp-Ship");
		DeathmatchMaps.push_back("Bio-Lab");
		DeathmatchMaps.push_back("Spade A");
		DeathmatchMaps.push_back("Construction");
		DeathmatchMaps.push_back("Neden-J");
		DeathmatchMaps.push_back("Azit");
		DeathmatchMaps.push_back("Azit-EX");
		DeathmatchMaps.push_back("Square");
		DeathmatchMaps.push_back("HoliDay");
		DeathmatchMaps.push_back("Luna-2");
		DeathmatchMaps.push_back("Treasure");
		DeathmatchMaps.push_back("Neden-1");
		DeathmatchMaps.push_back("Neden-2");
		DeathmatchMaps.push_back("Neden-3");
		DeathmatchMaps.push_back("Highway");
		DeathmatchMaps.push_back("Circle-1");
		DeathmatchMaps.push_back("BlockBuster");

		ChaserMaps.push_back("Alice House");
		ChaserMaps.push_back("Grave");
		ChaserMaps.push_back("Nightmare");
		ChaserMaps.push_back("Temple-O");
		ChaserMaps.push_back("Connest-2");
		ChaserMaps.push_back("Circle-2");
		ChaserMaps.push_back("BlockBuster");
		ChaserMaps.push_back("Office");

		BattleRoyalMaps.push_back("Highway");
		BattleRoyalMaps.push_back("Neden-1");
		BattleRoyalMaps.push_back("Bio-Lab");
		BattleRoyalMaps.push_back("Neoniac");
		BattleRoyalMaps.push_back("Galleon");

		CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(MainThread), NULL, 0, 0);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		Legion_ImplDX9_Shutdown();
		Legion_ImplWin32_Shutdown();
		Legion::DestroyContext();
		DestroyWindow(S4Window);

		if (DiscordInit)
			discord->Shutdown();
	}

	return TRUE;
}

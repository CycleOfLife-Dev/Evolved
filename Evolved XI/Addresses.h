#pragma once
#include <Windows.h>
#include <string>

DWORD dwBaseAddr = (DWORD)GetModuleHandle(NULL);

DWORD GameManager = dwBaseAddr + 0x0173D770;

typedef void(__thiscall* p_Show_Screen)(uintptr_t* ThisPtr, unsigned int id);
p_Show_Screen Show_Screen;

DWORD Show_Screen_Offset = 0xC326F0;
DWORD Show_Screen_Instance;
DWORD Show_Screen_Instance_Offset = 0x173D7A4;

DWORD MakeRoom;
DWORD MakeRoom_Offset = 0x8E42E2;
DWORD MakeRoom_Back;

DWORD  InRoomMakeRoom;
DWORD  InRoomMakeRoom_Offset = 0xA443B1;
DWORD  InRoomMakeRoom_Back;

DWORD MakeRoomBtn_Offset = dwBaseAddr + 0x8E42EC;
DWORD InRoomMakeRoomBtn_Offset = dwBaseAddr + 0xA443B7;

DWORD GUIStatus;
DWORD GUI_Status;
DWORD GUI_Status_Offset = 0xC32994;
DWORD GUI_Status_Back;

DWORD SettingClose;
DWORD SettingClose_Offset = 0x8C4D92;
DWORD SettingClose_Back;

DWORD SettingClose2;
DWORD SettingClose2_Offset = 0x8C4CF2;
DWORD SettingClose2_Back;

DWORD PlayerCamChange;
DWORD PlayerCamChange_Offset = 0x385943;
DWORD PlayerCamChange_Back;

DWORD PlayerFov_Addy;
DWORD PlayerFov2_Addy;
DWORD PlayerFov;
DWORD PlayerFov_Offset = 0x383086;// player cam -0x385C58;
DWORD PlayerFov_Back;

DWORD RevSevInfo;
DWORD RevSevInfo_Offset = 0xBA6014; //0xBA6017;
DWORD RevSevInfo_Back;
DWORD GetRevSevInfo;

DWORD CharBlock_Offset = dwBaseAddr + 0xEB8F7;


DWORD Room_Id_Offset = dwBaseAddr + 0x1772800;
DWORD Room_Name_Offset = dwBaseAddr + 0x1772838;
DWORD InfRespawn = dwBaseAddr + 0x24F745;
DWORD TeamHP = dwBaseAddr + 0x4C64DD;
DWORD Screen_Raito = dwBaseAddr + 0x0173D838;
DWORD GameEacMenu_Offset = dwBaseAddr + 0xE4CCB5;
DWORD FPS_Offset = dwBaseAddr + 0x10CEBD8;

DWORD UnLockFPS;
DWORD UnLockFPS_Offset = 0xCB6FE8;;
DWORD UnLockFPS_Back;
DWORD FPSValue = -9;

uintptr_t GameServer_Instance = dwBaseAddr + 0x173DA68;

DWORD Block_Shop_Window = dwBaseAddr + 0x8ECD31;
DWORD Shop_Buy_Button;
DWORD Shop_Buy_Button_Offset = 0x8ECD23;
DWORD Shop_Buy_Button_Back;

DWORD Read_Shop_ItemId;
DWORD Read_Shop_ItemId_Offset = 0xA1955F;
DWORD Read_Shop_ItemId_Back;

DWORD Read_Shop_ItemColorId;
DWORD Read_Shop_ItemColorId_Offset = 0x8EFC15;
DWORD Read_Shop_ItemColorId_Back;

DWORD PlayerLeaveChannelId;
DWORD PlayerLeaveChannel;
DWORD PlayerLeaveChannel_Offset = 0xAB3E29;
DWORD PlayerLeaveChannel_Back;

typedef void(__thiscall* P_LeaveRoom)(uintptr_t* ThisPtr, DWORD Id);
P_LeaveRoom LeaveRoom_Req = reinterpret_cast<P_LeaveRoom>(dwBaseAddr + 0xB84AC0);

typedef void(__thiscall* P_LeaveChannel)();
P_LeaveChannel LeaveChannel_Req = reinterpret_cast<P_LeaveChannel>(dwBaseAddr + 0x9EC4F0);

typedef void(__thiscall* P_LeaveServer)();
P_LeaveServer LeaveServer = reinterpret_cast<P_LeaveServer>(dwBaseAddr + 0x93A7D0);

typedef void(__thiscall* P_RoomInvite_Req)(uintptr_t* ThisPtr, DWORD Id, DWORD PlayerId);
P_RoomInvite_Req RoomInvite_Req = reinterpret_cast<P_RoomInvite_Req>(dwBaseAddr + 0xAB6370);

typedef void(__thiscall* P_UserInfo_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id);
P_UserInfo_Req UserInfo_Req = reinterpret_cast<P_UserInfo_Req>(dwBaseAddr + 0xAB5D80);

typedef void(__thiscall* P_AddFriend_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id, std::string* NickName);
P_AddFriend_Req AddFriend_Req = reinterpret_cast<P_AddFriend_Req>(dwBaseAddr + 0xAB5B30);

typedef void(__thiscall* P_DeleteFriend_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id, std::string* NickName);
P_DeleteFriend_Req DeleteFriend_Req = reinterpret_cast<P_DeleteFriend_Req>(dwBaseAddr + 0xAB5CD0);

typedef void(__thiscall* P_BlockChat_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id, std::string* NickName);
P_BlockChat_Req BlockChat_Req = reinterpret_cast<P_BlockChat_Req>(dwBaseAddr + 0xAB6210);

typedef void(__thiscall* P_RemoveBlockChat_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id, std::string* NickName);
P_RemoveBlockChat_Req RemoveBlockChat_Req = reinterpret_cast<P_BlockChat_Req>(dwBaseAddr + 0xAB62B0);

typedef void(__thiscall* P_ClanInvite_Req)(uintptr_t* ThisPtr, DWORD PlayerId, DWORD Id);
P_ClanInvite_Req ClanInvite_Req = reinterpret_cast<P_ClanInvite_Req>(dwBaseAddr + 0xB87E20);

auto Enter_Channel_Req = (void* (__stdcall*)(int32_t ChannelId))(dwBaseAddr + 0x950DD0);

typedef void(__thiscall* P_KickVote_Req)(uintptr_t* ThisPtr, DWORD MyPlayerId, DWORD unk, DWORD PlayerId, DWORD unk2, DWORD Reason);
P_KickVote_Req KickVote_Req = reinterpret_cast<P_KickVote_Req>(dwBaseAddr + 0xB86E30);

typedef void(__thiscall* P_SendInfo)(uintptr_t* ThisPtr, std::string* Name, std::string* Unk2, std::string* Unk3);
P_SendInfo SendInfo_Req = reinterpret_cast<P_SendInfo>(dwBaseAddr + 0xB88CB0);

DWORD Player_PlayerInfoList_Ack;
DWORD Player_PlayerInfoList_Ack_Offset = 0xAB39CF;
DWORD Player_PlayerInfoList_Ack_Back;

DWORD Room_GamePlayCountDown_Ack;
DWORD Room_GamePlayCountDown_Ack_Offset = 0xBA1723;
DWORD Room_GamePlayCountDown_Ack_Back;

DWORD EnterPlayer_Ack;
DWORD EnterPlayer_Ack_Offset = 0xBDDDAE;
DWORD EnterPlayer_Ack_Back;

DWORD Game_BriefingInfo_Ack;
DWORD Game_BriefingInfo_Ack_Offset = 0x3CBF41;
DWORD Game_BriefingInfo_Ack_Back;

DWORD Room_ListInfo_Ack;
DWORD Room_ListInfo_Ack_Offset = 0xDC643F;
DWORD Room_ListInfo_Ack_Back;

DWORD Chat_Ack;
DWORD Chat_Ack_Offset = 0xBF4DE4;
DWORD Chat_Ack_Back;

DWORD Game_RefreshGameRuleInfo_Ack;
DWORD Game_RefreshGameRuleInfo_Ack_Offset = 0xB901DF;
DWORD Game_RefreshGameRuleInfo_Ack_Back;

DWORD Game_ChangeState_Ack;
DWORD Game_ChangeState_Ack_Offset = 0xB8B57B;
DWORD Game_ChangeState_Ack_Back;

DWORD PlayerLeaveChannel;
DWORD PlayerLeaveChannel_Offset = 0xAB3E29;
DWORD PlayerLeaveChannel_Back;

DWORD Room_GameLoading_Ack;
DWORD Room_GameLoading_Ack_Offset = 0xB9FB8D;
DWORD Room_GameLoading_Ack_Back;

DWORD Game_ChangeSubState_Ack;
DWORD Game_ChangeSubState_Ack_Offset = 0xB8B6A6;
DWORD Game_ChangeSubState_Ack_Back;

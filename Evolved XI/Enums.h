#pragma once
enum class Chat_Type
{
	All = 0,
	Clan,
	Union,
	Whisper,
	Support,
	Team
};

enum class Msgbox
{
	CNone = 0,
	FriendRequest,
	DeleteFriendRequest,
	BlockChatReqeust,
	RemoveBlockChatReqeust,
	ExchangeRequest,
	ChallengeRequst,
	ExchangeAccept,
	ChallengeAccept,
	ClanInvite,
	ClanKick,
	ClanBan,
	ClanTransferMaster,
	ClanInviteUnion,
	ClanKickUntion,
	ClanLeaveUntion,
	ClanUnionAccept,
	ClanUnionRefuse,
	ClanClose,
	InvalidAmountPEN,
	InvalidAmountLP,
	LeaveRoom,
	ExitGame
};

enum class GameState
{
	Waiting = 1,
	Playing = 2,
	Result = 3,
	Loading = 4
};

enum class PlayerStats
{
	Alive = 0,
	Dead = 1,
	Waiting = 2,
	Spectating = 3,
	Lobby = 4
};

enum class Player_Loc
{
	Server = 0,
	Channel,
	RoomList,
	Room,
	LoadingEnd,
	CountDown,
	Playing,
	HalfTime,
};

enum class Player_State
{
	None = 0,
	Waitting,
	Playing,
};


enum class S4_Screen
{
	Display_InGame = 0x19,
	Display_Mission = 0x805A,
	Display_MSGBOX = 0x802A,
	Display_EnchantMenu = 0x0D,
	Display_Shop = 0x7,
	Display_Inventory = 0x8,
	Display_Arcade = 0x8028,
	Display_RandomShop = 0x0C,
	Display_RoomInfo = 0x8051,
	Display_RoomPassword = 0x8035,
	Display_RoomSettings = 0x8057,
	Display_RoomInvite = 0x8060,
	Display_Card = 0x8026,
	Display_Community = 0x8029,
	Display_Clan = 0x0E,
	Display_Settings = 0x8045,
	Display_Daily_Events = 0x8001,
	Display_Events = 0x8055,
	Display_SendMSG = 0x8043,
	Display_Help = 0x802B,
	Display_Server = 0x2,
	Display_Channel = 0x5,
	Display_Lobby = 0x6,
	Display_Room = 0xA,
	Display_Game = 0x13,
	Hide_Shop = 0x8023,
};

enum class MsgBox_Buttons
{
	None = 0x00,
	Confirm = 0x01,
	Cancel = 0x02,
	Confirm_Cancel = 0x03,
	Yes = 0x04,
	Confirm_Yes = 0x05,
	Cancel_Yes = 0x06,
	Confirm_Cancel_Yes = 0x07,
	No = 0x08,
	Confirm_No = 0x09,
	Cancel_No = 0x0A,
	Confirm_Cancel_No = 0x0B,
	Yes_No = 0x0C,
	Confirm_Yes_No = 0x0D,
	Cancel_Yes_No = 0x0E,
};

enum class  MsgBox_Types
{
	None = 0x00,
	DC = 0x01,
	Exit = 0x02,
};

#pragma once
#include "stdafx.h"
#include "Enums.h"
#include "Discord.h"

#pragma region Functions
HWND S4Window;
WNDPROC oWndProc;
Discord* discord;

bool ActiveVAction = true;
bool ActiveCVAction = true;

extern LRESULT Legion_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice);
f_EndScene oEndScene;

typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
Reset_t oReset;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

void VAction(LPCWSTR Action, std::string Gender);

PDIRECT3DTEXTURE9 CPlayerListTexture(std::string filename, int Type);

LPDIRECT3DDEVICE9 pD3DDevice;

#pragma endregion

#pragma region Main Menu
bool CPlayerListUI = false;
bool CRoomChatSystemUI = false;
bool CChatSystemUI = false;
bool CVoteKickSystemUI = false;
bool CSurrendeMessageUI = false;
bool CVoiceChatUI = false;
bool CSurrenderSystemUI = false;
bool ShopSearch = false;
bool ShopSearchUI = false;
bool CMainMenuUI = false;
bool CGameMenuUI = false;
bool CMakingRoomUI = false;
bool CRoomOptionUI = false;
bool CAfkPlayerSystemUI = false;
bool RoomSearch = false;
bool RoomSearchUI = false;
bool QuestSystemUI = false;
bool CPlayerReportUI = false;
bool NewClanSystemUI = false;
bool NewClanInfoSystemUI = false;
bool NewClanRankChangeUI = false;
bool NewClanNameChangeUI = false;
bool NewClanInviteUI = false;
bool NewCreateClanUI = false;
bool ShowAmountOfAmmo = false;
bool AddNewProfileUI = false;
bool InventoryProfileButtonsUI = false;
bool CharactersProfileUI = false;
bool CharactersProfileHalfTimeUI = false;
bool RoomMapFilter = false;
bool VoteKickUI = false;
bool CommunityUI = false;
bool NewAddFriendUI = false;
bool AlertUI = false;
bool testUI = false;
bool IsLoadTexture = false;
bool IsLoadTexture2 = false;
bool IsLoadTexture3 = false;
bool  CClanLogoUI, CDailyRewardUI, CCustomNameTagUI, CAutoEventUI, CChannelUI, CGameShopUI, CGameRewardsUI,
CXMasEventUI, CMessageBoxUI, CActionsUI, CRoomOptionClient = false;
bool DailyEventEnable = false;
int GameMenuSelected = 0;
int MenuSelected = 1;

bool MenuInit, InputBlock = false;
bool CINIT, VINIT, XINIT = false;

std::string MainStatus;
bool IsTimerActivated = false;
const char* MainMenuButtonName;
//IDirect3DDevice9* pD3DDevice;
#pragma endregion

#pragma region Addfriend
char AddFriendName[18];
#pragma endregion

#pragma region Account Info
Player_Loc plr_loc = Player_Loc::Server;
Player_State plr_state = Player_State::Waitting;
int MyPlayerId;
char PlayerEnDecKey[255];
std::string PlayerHwid;
std::string DecryptionKey;
std::string AccountInfo;
std::string MyNickName;
std::string MyLevel;
std::string MyClanRank;

DWORD PlayerAP;
DWORD PlayerPEN;
DWORD PlayerEXP;
DWORD PlayerCurrentEXP;
#pragma endregion

#pragma region Map Info
int GameMode = 2;
int SelectedMapId = 70;
int MapInfoSelected;
std::string SelectedItemLimit = "Unlimited";
std::string SelectedScoreLimit = "4 P ( 10 min )";
std::string SelectedPlayers = "12";
char RoomTitle[12];
char RoomPassword[12];
bool Observer = false;
bool BurningMode = false;
bool FriendlyMode = false;
bool NoStateMpde = false;
#pragma endregion

#pragma region Message Box
std::string Message;
int Buttons;
Msgbox MessageType;
#pragma endregion

#pragma region HalfTime
CTimer* HalfTimeTimer = new CTimer();
int HalfTimerCounter;
#pragma endregion

#pragma region Clan System
bool IsInClan = false;
bool IsNoticeRead = false;

int ClanPlayerId = 0;
int ClanUnionId = 0;
int ClanRankId = 0;
int ClanHistoryId = 0;
int ClanButtonMenu = 0;
std::string ClanName;
std::string ClanLevel;
std::string ClanRank;
std::string ClanBattels;
float ClanEXP;
float TotalClanExp;
std::string ClanPoints;
std::string ClanTitle;
std::string ClanMessage;

std::string LeaveCloseButton;


unsigned int ClanSelectedPlayer;
unsigned int ClanSelectedPlayerId; 
std::string ClanSelectedPlayerNickName;
std::string ClanSelectedPlayerRank;
std::string ClanSelectedPlayerRankId;
std::string ClanSelectedFriendStats;

unsigned int ClanInfoSelectedPlayer;
unsigned int ClanInfoSelectedPlayerId;
std::string ClanInfoSelectedPlayerNickName;
std::string ClanInfoSelectedPlayerRank;
std::string ClanInfoSelectedFriendStats;

int IsClan = 0;
// Union

std::string UnionName;
std::string UnionLeader;
std::string UnionPoints;
std::string UnionBattels;
std::string UnionMember;
bool UnionMaster;
char InvUnionName[16];
char ClanNewName[16];
char CreateClanName[16];
char ClanFindByName[16];
char ClanNotice[50];
char ClanNoticeMessage[255];
//Clan Info

std::string ClanInfoName;
std::string ClanInfoLevel;
std::string ClanInfoRank;
std::string ClanInfoBattels;
float ClanInfoEXP;
float TotalClanInfoExp;
std::string ClanInfoPoints;
std::string ClanInfoTitle;
std::string ClanInfoMessage;
#pragma endregion

#pragma region Rank
unsigned int MyRankPoint;
std::string RankStats = "Not Registered";
std::string RankButton = "{c1ccdd}Register";
#pragma endregion

#pragma region Challenge
char CPLP[10], CPPEN[10], ChalFindPlayer[16];
#pragma endregion

#pragma region Other
int ClanLogoSelected = 1;
std::string ClanLogoPrice;
std::string SelectedClanLogo;
#pragma endregion

#pragma region Exchange
bool IsExcConfirm, IsExcConfirmed, IsExcLocked, IsExcLocked2, ExchangeBlockPlayer, ExchangeBlockPlayer2 = false;

std::string ExchangePlayer1NickName, ExchangePlayer2NickName, P1Level, P2Level;
std::string PlayerExc = "LOCK";
std::string Player2Exc = "LOCK";
const char* ExchangeItemInfo = "";
const char* ExchangeItemInfo2 = "";
const char* ExchangeItemInfo3 = "";
const char* ExchangeP2ItemInfo = "";
const char* ExchangeP2ItemInfo2 = "";
const char* ExchangeP2ItemInfo3 = "";
bool IsExchanged = false;
char P1LP[10], P1PEN[10], P2LP[10], P2PEN[10], ExcFindPlayer[16];
#pragma endregion

#pragma region Community
int SelectedCommunity = 0;
int SelectedFriendCommunityId = 0;
int SelectedClanCommunityId = 0;
int SelectedUnionCommunityId = 0;
int SelectedBlockCommunityId = 0;
unsigned int SelectedCommunityPId = 0;
unsigned int SelectedFriendCommunityPId = 0;
unsigned int SelectedBlockCommunityPId = 0;
unsigned int SelectedClanCommunityPId = 0;
unsigned int SelectedUnionCommunityPId = 0;
std::string SelectedCommunityNick = "";
std::string SelectedCommunityFriendStats = "";
std::string SelectedCommunityBlockStats = "";
#pragma endregion

#pragma region Characters Profile
char ProfileName[20];
int SelectedChar = 0;
int SelectedCharProfile = 0;
std::string CharProfileSelected;
std::string ItemNames[12];
std::string EffectsInfo[12];
#pragma endregion

#pragma region Room / Player Info
int Gamestate;
int Playerstate;
#pragma endregion

#pragma region Player Fov
float Fov = 60.0f;
float fov = 0;
bool FirstInit = false;
bool IsZoomOut;
bool ESCMenu = false;
int FirstInit2 = 0;
#pragma endregion

#pragma region Room Settings
float HP = 100.0f;
float SP = 100.0f;
float Ping = 1000.0f;
float Heal = 0.0f;

bool ShowTeamHP, Respawn, NoMacro, NoEnchant, NoClassSystem, NoRebirth, AutoSit, InsideStates, ChangeProfile, ActiveFilter, RandomSwapFilter = false;

 int RoomFilterId;
unsigned int RoomFilterModeId;
unsigned int RoomFilterMapId;
unsigned int RoomFilterSize;
std::string option, mapfilters, RoomFilterMode, RoomFilterMap, Blockroption, Allowroption, AllowPerTeamroption;

#pragma region Block Custom Rules
bool BlockPS,
BlockCS,
BlockDG,
BlockBAT,
BlockBR,
BlockTB,
BlockIB,
BlockFist,
BlockKatan,
BlockEXO,
BlockSigmaBlade,
BlockVitalShock,
BlockRevolver,
BlockHeavyMachineGun,
BlockLightMachineGun,
BlockTurret,
BlockHealWeapons,
BlockShockWeapons,
BlockSentry,
BlockSenty,
BlockMineGun,
BlockEarthbomber,
BlockLightningBomber,
BlockRocketLauncher,
BlockCannonade,
BlockRailGun,
BlockSniper,
BlockAirGun,
BlockDualMagnum,
BlockBurstShotgun,
BlockSemiRifle,
BlockSmashRifle,
BlockAssaultRifle,
BlockBubblRifle,
BlockGaussRifle,
BlockHandGun,
BlockSubmachineGun,
//Skills
BlockHalfHPMastery,
BlockHPMastery,
BlockSkillMastery,
BlockDualMastery,
BlockMetallic,
BlockUniqueDualMastery,
BlockAnchoring,
BlockFlying,
BlockInvisible,
BlockDetect,
BlockShield,
BlockBlock = false;
#pragma endregion

#pragma region Allow Custom Rules
bool AllowPS,
AllowCS,
AllowDG,
AllowBAT,
AllowBR,
AllowTB,
AllowIB,
AllowFist,
AllowKatan,
AllowEXO,
AllowSigmaBlade,
AllowVitalShock,
AllowRevolver,
AllowHeavyMachineGun,
AllowLightMachineGun,
AllowTurret,
AllowHealWeapons,
AllowShockWeapons,
AllowSentry,
AllowSenty,
AllowMineGun,
AllowEarthbomber,
AllowLightningBomber,
AllowRocketLauncher,
AllowCannonade,
AllowRailGun,
AllowSniper,
AllowAirGun,
AllowDualMagnum,
AllowBurstShotgun,
AllowSemiRifle,
AllowSmashRifle,
AllowAssaultRifle,
AllowBubblRifle,
AllowGaussRifle,
AllowHandGun,
AllowSubmachineGun,
//Skills
AllowHalfHPMastery,
AllowHPMastery,
AllowSkillMastery,
AllowDualMastery,
AllowMetallic,
AllowUniqueDualMastery,
AllowAnchoring,
AllowFlying,
AllowInvisible,
AllowDetect,
AllowShield,
AllowBlock = false;
#pragma endregion

#pragma region AllowPerTeam Custom Rules
bool AllowPerTeamPS,
AllowPerTeamCS,
AllowPerTeamDG,
AllowPerTeamBAT,
AllowPerTeamBR,
AllowPerTeamTB,
AllowPerTeamIB,
AllowPerTeamFist,
AllowPerTeamKatan,
AllowPerTeamEXO,
AllowPerTeamSigmaBlade,
AllowPerTeamVitalShock,
AllowPerTeamRevolver,
AllowPerTeamHeavyMachineGun,
AllowPerTeamLightMachineGun,
AllowPerTeamTurret,
AllowPerTeamHealWeapons,
AllowPerTeamShockWeapons,
AllowPerTeamSentry,
AllowPerTeamSenty,
AllowPerTeamMineGun,
AllowPerTeamEarthbomber,
AllowPerTeamLightningBomber,
AllowPerTeamRocketLauncher,
AllowPerTeamCannonade,
AllowPerTeamRailGun,
AllowPerTeamSniper,
AllowPerTeamAirGun,
AllowPerTeamDualMagnum,
AllowPerTeamBurstShotgun,
AllowPerTeamSemiRifle,
AllowPerTeamSmashRifle,
AllowPerTeamAssaultRifle,
AllowPerTeamBubblRifle,
AllowPerTeamGaussRifle,
AllowPerTeamHandGun,
AllowPerTeamSubmachineGun,
//Skills
AllowPerTeamHalfHPMastery,
AllowPerTeamHPMastery,
AllowPerTeamSkillMastery,
AllowPerTeamDualMastery,
AllowPerTeamMetallic,
AllowPerTeamUniqueDualMastery,
AllowPerTeamAnchoring,
AllowPerTeamFlying,
AllowPerTeamInvisible,
AllowPerTeamDetect,
AllowPerTeamShield,
AllowPerTeamBlock = false;
#pragma endregion
#pragma endregion

#pragma region Game Settings
float FPS = 60.0f;
float InputLag = 1.0f;
float CharCam = 5.0f;
float AimPos = 35.0f;
float MapSize = 15.0f;
bool StableFPS = true, FPSLoad = true, ImproveClient = true;
bool AutoBoost, ExclusiveMode = false;
bool ActiveCharCam = false;

char Action[25];
char Action2[25];
char Action3[25];
char Action4[25];
char Action5[25];
char Action6[25];
char Action7[25];
char Action8[25];
char Action9[25];
char Action10[25];
char Action11[25];
char Action12[25];
#pragma endregion

#pragma region Alert Info
CTimer* AlertEffectTimer = new CTimer();
CTimer* AlertWorkerTimer = new CTimer();
float AlertOpacity = 1.0f;
#pragma endregion

#pragma region Surrender System
char SurrenderMessage[60];
std::string SurrenderPlayerName;
std::string SVotedCounter;
std::string SMaxCounter;
int SCounter = 0;
bool SVoteLock = false;
bool StartedSurrender = false;
CTimer* SVoteTimer = new CTimer();
#pragma endregion

#pragma region Channel Chat
Chat_Type Chat_type = Chat_Type::All;

time_t current_time;
struct tm local_time;
char buffer[30];

int ChatButton = 1;
int ChannelCount = 0;

float ChatBoxWidth, ChatBoxHeight, ChatBoxPosX, ChatBoxPosY, RoomChatBoxWidth, RoomChatBoxHeight;

char ChatSystemMessage[255];
bool ChatButtonMenuLock = false;
const char* ChatMenuName = "All";
const char* ChatBtnMenuName = "Clan";
const char* ChatBtn2MenuName = "Union";
const char* ChatBtn3MenuName = "Whisper";
const char* ChatBtn4MenuName = "Support";
std::string ChannelName;
std::string CSelectedPlayerFriendStats;
std::string CSelectedPlayerBlockStats;
std::string CSelectedPlayer;
unsigned int CSelectedPlayerId;
std::string LastWhisperNickname;

bool ChatAutotScroll = false;
bool RoomChatAutotScroll = false;
bool IsTextBox = false;
#pragma endregion

#pragma region Room Chat
Chat_Type RoomChat_type = Chat_Type::All;
CTimer* RoomChatTimer = new CTimer();;
time_t RoomChat_current_time;
struct tm RoomChat_local_time;
char RoomChat_buffer[30];

char RoomChatSystemMessage[255];
int RoomChatButton = 1;

bool IsFullChat = false;
bool RoomChatButtonMenu = true;
bool RoomChatButtonMenuLock = false;
const char* RoomChatMenuName = "All";
const char* RoomChatBtnMenuName = "Team";
const char* RoomChatBtn2MenuName = "Clan";
const char* RoomChatBtn3MenuName = "Union";
const char* RoomChatBtn4MenuName = "Whisper";

std::string RoomChatType = "/a";
#pragma endregion

#pragma region Vote Kick
int VoteKickId;
int VoteKickRId;
DWORD VoteKickSelectedPlayer;
DWORD VoteKickReasonSelected;
DWORD VoteKickSelectedPlayerId;
#pragma endregion

#pragma region Report Player
int RPeportPlayerID = 0;
unsigned int RSelectedPlayerId;
#pragma endregion

#pragma region Events
char AAutoEvent[30];
std::string QAutoEvent;
int SantaCap, SnowMan, Snow, Tree;

int QuestButtonMenu = 0;
int LastQuestButtonMenu = 0;
unsigned int QuestId;
std::string QuestPercentage;
std::string QuestPercentage2;
std::string QuestPercentage3;
std::string QuestInfo;

float CurrentQuestPercentageWeekly;
float MaxQuestPercentageWeekly;
#pragma endregion

#pragma region BattleField
int BattleFieldId = 0;
int AdvBattleFieldId = 0;
int BattleFieldTaskId = 0;
int BattleFieldMenu = 0;
int BattleFieldStageMenu = 0;
std::string BattleFiedlsLevel = "0";
std::string BattleFiedlsPoints = "0";
#pragma endregion

#pragma region Custom Name Tag
std::string CustomTagItemId;
char CCustomNameTag[6];
#pragma endregion

#pragma region Voice Actions
std::string Gender;
std::string BU_Gender;
HRESULT Voicehr;
CTimer* VoiceActionMenuTimer = new CTimer();
CTimer* VoiceActionSendTimer = new CTimer();
bool VoiceLock = false;
int VoteTimer = 0;
#pragma endregion

#pragma region Shop
bool  IsBuyButtonPressed = false;
DWORD ShopItemId;
DWORD ItemId;
DWORD ShopItemColorId = 0;

char ShopItemName[100];
std::string ShopItemMoneyLeft;
std::string ShopItemMoneyPrice;

std::string ShopSelectedType;
std::string ShopSelectedEffect;
std::string ShopSelectedCoupon;

char ShopItemEffects[100];

unsigned int ShopItemColorMax;
unsigned int ShopItemCurrentColor;
unsigned int PriceRate = 1;

DWORD ShopPEN;
DWORD ShopLP;

bool ShopUsePen = false;
LPDIRECT3DTEXTURE9 ShopItemTexture = NULL;

#pragma endregion

#pragma region Shop Search
DWORD SShopItemMoneyLeft;
DWORD SShopItemMoneyPrice;
char ShopItemsSearch[255];
std::string SelectedItemSearch;
#pragma endregion

#pragma region Lobby Afk
CTimer* AfkPlayerKick_Timer = new CTimer();
int AfkCounter = 60;
#pragma endregion

#pragma region Discord RPC
static bool RPCActvion = false;
static std::string TotalMatchs;
static std::string Win;
static std::string Loss;
static std::string WinLossPercentage;
static std::string GameRuleState;
static std::string GameRule;
static std::string LargeImage;
static std::string SmallImage;
static std::string LargeImageText;
static std::string SmallImageText;
static const char* Timestamps;
static double RoomCurrentTime;

static std::string PlayerCount;
static std::string MaxPlayers;
static std::string MapName;
static std::string RoomId;
static std::string RoomTime;
static std::string RoomName;
#pragma endregion

#pragma region Struct
struct S4_Item
{
	std::string unique_id;
	std::string item_id;
	std::string color_id;
	char Effect[200];
	LPDIRECT3DTEXTURE9 ExchangeTexture;
}; std::map<unsigned int, S4_Item*> S4_ItemList;
std::map<unsigned int, S4_Item*> S4_ItemList2;

struct ChatSystemStruc
{
	char Nickname[38];
	std::string Time;
	char Message[255];
};

struct RoomChatSystemStruc
{
	std::string Time;
	std::string Nickname;
	std::string Message;
};

struct NewPlayerList
{
	char Nickname[26];
	std::string Level;
	LPDIRECT3DTEXTURE9 Rank;
	std::string FriendStats;
	std::string BlockStats;
	std::string ClanRank;
};

struct ClanMemberListClass // To Do in the new clan system
{
	unsigned int PlayerId;
	char Nickname[26];
	std::string Level;
	char Location[80];
	std::string Rank;
	std::string Points;
	std::string Battles;
	std::string FriendStats;
	std::string PlayerStatus;
};

struct ClanUnionListClass // To Do in the new clan system
{
	char ClanName[20];
	std::string Level;
	char Leader[26];
	std::string Points;
	std::string Battles;
	std::string Members;
};

struct ClanRankListClass // To Do in the new clan system
{
	unsigned int ClanId;
	std::string Rank;
	char Name[26];
	char Leader[18];
	std::string Points;
	std::string Level;
	std::string Battles;
};

struct ClanHistoryListClass // To Do in the new clan system
{
	unsigned int ClanId;
	char Name[20];
	std::string Date;
	std::string Status;
};

struct PlayersShortInfoListClass // To Do in the new clan system
{
	unsigned int PlayerId;
	char Name[26];
	bool IsOnline = false;
	std::string Level;
	std::string FriendStats;
	std::string BlockStats;
	LPDIRECT3DTEXTURE9 Rank;
};

struct PlayerShortInfoListClass // To Do in the new clan system
{
	unsigned int PlayerId;
	char Name[26];
	std::string Level;
};


struct PlayerClanShortInfoListClass // To Do in the new clan system
{
	unsigned int PlayerId;
	char Name[26];
	std::string Level;
	char ClanName[20];
	bool IsOnline = false;
	std::string FriendStats;
	std::string BlockStats;
	std::string ClanRank;
	LPDIRECT3DTEXTURE9 Rank;
	LPDIRECT3DTEXTURE9 ClanIcon;
	LPDIRECT3DTEXTURE9 StaffIcon;
};

struct ShopSearchItem
{
	char Name[100];
	DWORD Id;
	uint8_t Color;
	uint8_t MaxColor;
	//std::vector< std::string > Effects;
	char Effects[100];
	DWORD Price;
	std::vector< std::string > Type;
	bool IsActive = false;
	std::string ShopSelectedType;
	std::string ShopSelectedEffect;
	LPDIRECT3DTEXTURE9 Image = NULL;
};

struct QuestSystemStruct
{
	char Name[200];
	char Info[255];
	std::string Percentage;
	std::string Percentage2;
	std::string Percentage3;
	bool IsClaimed;

	LPDIRECT3DTEXTURE9 RewardImage;
	LPDIRECT3DTEXTURE9 Reward2Image;
	LPDIRECT3DTEXTURE9 ClaimeImage;

	float WeeklyPercentage;
	float WeeklyMaxPercentage;
	bool IsWeeklyClaimed;

	LPDIRECT3DTEXTURE9 WeeklyRewardImage;
	LPDIRECT3DTEXTURE9 WeeklyClaimeImage;
};

struct BattleFieldSystemStruct
{
//	std::string Type;
	//std::string Reward;
	//std::string Color;
	LPDIRECT3DTEXTURE9 RewardImage;
	//std::string ItemPeriod;
	//std::string Period;
//	std::string Level;
	bool Status;
	bool IsButtonActive;
};

struct MapInfoStruct
{
	std::string Name;
	int Id;
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 HoverTexture;
	LPDIRECT3DTEXTURE9 ActivatedTexture;
};

struct GameModeInfoStruct
{
	std::string Name;
	LPDIRECT3DTEXTURE9 Texture;
	LPDIRECT3DTEXTURE9 HoverTexture;
	LPDIRECT3DTEXTURE9 ActivatedTexture;
};

struct GameModeRulesStruct
{
	//std::string Name;
	//int Id;
	//LPDIRECT3DTEXTURE9 Texture;
};

struct BattleFieldTaskSystemStruct
{
	char Task[200];
	std::string PercentageInfo;
	std::string Points;
//	std::string Stage;
	bool Status;
};

struct ProfileItemInfo
{
	std::string Name;
	std::string ItemName[12];
	std::string ImageName[12];
	std::string Effects[12];
};
/*struct LoadResource
{
	std::string Chiled;
	std::string Name;
	std::string CryptedName;
};*/
#pragma endregion

#pragma region Vector Map
std::vector< PDIRECT3DTEXTURE9 > DailyEventList;
std::map<std::string, std::string> Configs;
std::map<std::string, std::string> ImageFileList;
std::map<std::string, LPDIRECT3DTEXTURE9> ImageDrawList;
std::map<std::string, LPDIRECT3DTEXTURE9> ClanDrawList;
std::map<std::string, LPDIRECT3DTEXTURE9> PlayerLevelTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> PlayerRankTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> PlayerNameTagTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> PlayerClanRankIconTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> GameModeTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> ActivatedGameModeTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> MapTexture;
std::map<std::string, LPDIRECT3DTEXTURE9> ModeTexture;
std::map<unsigned int, NewPlayerList*> ChannelPlayerList;
std::map<unsigned int, NewPlayerList*> SupportPlayerList;
std::map<unsigned int, NewPlayerList*> ClanMemberList;
std::map<unsigned int, NewPlayerList*> UnionMemberList;
std::map<unsigned int, NewPlayerList*> WhisperPlayerList;
std::map<unsigned int, ChatSystemStruc*> ChatSystemList;
std::map<unsigned int, ChatSystemStruc*> ClanSystemList;
std::map<unsigned int, ChatSystemStruc*> ClanUnionSystemList;
std::map<unsigned int, ChatSystemStruc*> WhisperSystemList;
std::map<unsigned int, ChatSystemStruc*> SupportSystemList;
std::map<unsigned int, ChatSystemStruc*> TeamSystemList;
std::map<unsigned int, ChatSystemStruc*> RoomChatSystemList;
std::vector< std::string > ShopItemUse;
//std::vector< std::vector<char> > ShopItemEffect;
std::vector< std::string > ShopCoupon;
std::vector< std::string > ItemLimit;
std::vector< std::string > ScoreLimit;
std::vector< std::string > PlayersLimit;
std::map<unsigned int, ShopSearchItem*> ShopSearchItems;
std::map<unsigned int, QuestSystemStruct*> QuestSystemInfo;
std::map<unsigned int, BattleFieldSystemStruct*> BattleField;
std::map<unsigned int, BattleFieldSystemStruct*> AdvBattleField;
std::map<unsigned int, BattleFieldTaskSystemStruct*> BattleFieldTask;
std::map<unsigned int, PlayersShortInfoListClass*> FrindsPlayerList;
std::map<unsigned int, PlayersShortInfoListClass*> BlockPlayerList;
std::map<unsigned int, PlayerClanShortInfoListClass*> ClanPlayerList;
std::map<unsigned int, PlayerClanShortInfoListClass*> UnionPlayerList;
std::map<unsigned int, PlayerShortInfoListClass*> ReportPlayerList;
std::map<unsigned int, PlayerShortInfoListClass*> VoteKickList;
std::map<unsigned int, MapInfoStruct*> MapInfo;// this is like a list to hold on map info inside it so we can load them laterok
std::map<unsigned int, GameModeInfoStruct*> GameModesInfo;// this is like a list to hold on map info inside it so we can load them laterok
std::map<unsigned int, MapInfoStruct*> MapInfoSelection;// this is like a list to hold on map info inside it so we can load them laterok
std::map<unsigned int, std::string> VoteKickReasons{ {0, "Hacking/Cheating Bot"}, {1, "Bad Member"}, {2, "Bug Abusing"}, {3,"Afk"}, {4, "Macro Abusing"}, {5, "UnStable Network Lag"}, {6, "Etc"} };
std::map<std::string, std::string> LoadUIResource;
std::map<std::string, std::string> LoadLevelsResource;
std::map<std::string, std::string> LoadRankResource;
std::map<std::string, std::string> LoadClubesource;
std::map<std::string, std::string> LoadClubRankIconResource;
std::map<std::string, std::string> LoadGameModeResource;
std::map<std::string, std::string> LoadActivatedGameModeResource;
std::map<std::string, std::string> LoadItemsResource;
std::map<std::string, std::string> MapResource;
std::map<std::string, std::string> ModeResource;
std::map<unsigned int, ClanMemberListClass*> ClubMemberList;
std::map<unsigned int, ClanUnionListClass*> ClanUnionList;
std::map<unsigned int, ClanRankListClass*> ClanRankList;
std::map<unsigned int, ClanHistoryListClass*> ClanHistoryList;
std::map<unsigned int, ClanMemberListClass*> ClubInfoMemberList;
std::map<unsigned int, PlayerShortInfoListClass*> ClubRequestList;
std::map<unsigned int, PlayerShortInfoListClass*> ClubBannedList;
std::map<unsigned int, ProfileItemInfo*> ProfileItemInfoList;
std::vector< std::string > RoomFilterModes;
std::vector< std::string > TouchdownMaps;
std::vector< std::string > TouchdownPassMaps;
std::vector< std::string > DeathmatchMaps;
std::vector< std::string > ChaserMaps;
std::vector< std::string > BattleRoyalMaps;
std::vector< std::string > MapFilters;
std::vector< std::string > ClanRanksList;
#pragma endregion

#pragma region Texture
#pragma region Font
std::vector<BYTE> FontData;

ImFont* CalibriFont;
ImFont* Ebrima;
ImFont* CalibriFont2;
ImFont* ArialiFont;
ImFont* ArialiBlodFont;
#pragma endregion

#pragma region UI
LPDIRECT3DTEXTURE9 BackGroundTexture = NULL;
LPDIRECT3DTEXTURE9 CGameSettingsUIUITexture = NULL;
LPDIRECT3DTEXTURE9 PlayerListUITexture = NULL;
LPDIRECT3DTEXTURE9 CustomUITexture = NULL;
LPDIRECT3DTEXTURE9 ShopUITexture = NULL;
LPDIRECT3DTEXTURE9 MessageBoxUITexture = NULL;
LPDIRECT3DTEXTURE9 AutoEventUITexture = NULL;
LPDIRECT3DTEXTURE9 DailyEventUITexture = NULL;
LPDIRECT3DTEXTURE9 GameRewardsUITexture = NULL;
LPDIRECT3DTEXTURE9 MacroUITexture = NULL;
LPDIRECT3DTEXTURE9 EmptyBoxUITexture = NULL;
LPDIRECT3DTEXTURE9 ReportPlayerUITexture = NULL;
LPDIRECT3DTEXTURE9 RoomFiltersUITexture = NULL;
LPDIRECT3DTEXTURE9 VoteKickPlayerUITexture = NULL;
LPDIRECT3DTEXTURE9 ContextTexture = NULL;
LPDIRECT3DTEXTURE9 LPIconTexture = NULL;
LPDIRECT3DTEXTURE9 ClanFreeUITexture = NULL;
LPDIRECT3DTEXTURE9 CharactersProfileUITexture = NULL;

LPDIRECT3DTEXTURE9 SelectedGameModeTexture = NULL;
LPDIRECT3DTEXTURE9 SelectedMapTexture = NULL;

LPDIRECT3DTEXTURE9 ASelectedGameModeTexture = NULL;
LPDIRECT3DTEXTURE9 ASelectedMapTexture = NULL;
#pragma endregion

LPDIRECT3DTEXTURE9 CharactersProfileItemsTexture[12] = {};


#pragma region Clan System
LPDIRECT3DTEXTURE9 ClanUITexture = NULL;
LPDIRECT3DTEXTURE9 ClanSelectionTexture = NULL;
LPDIRECT3DTEXTURE9 ListBGTexture = NULL;
LPDIRECT3DTEXTURE9 OfflineTexture = NULL;
LPDIRECT3DTEXTURE9 OnlineTexture = NULL;
LPDIRECT3DTEXTURE9 CScrollTexture = NULL;
LPDIRECT3DTEXTURE9 ClanMarkTexture = NULL;
LPDIRECT3DTEXTURE9 ClanInfoMarkTexture = NULL;
#pragma endregion

#pragma region Buttons
LPDIRECT3DTEXTURE9 RoomFilterMapTexture = NULL;
LPDIRECT3DTEXTURE9 RoomFilterModeTexture = NULL;
LPDIRECT3DTEXTURE9 ImageTexture = NULL;
LPDIRECT3DTEXTURE9 SmallBoxTexture = NULL;
LPDIRECT3DTEXTURE9 ComboArrowTexture = NULL;
LPDIRECT3DTEXTURE9 LeftArrowTexture = NULL;
LPDIRECT3DTEXTURE9 RightArrowTexture = NULL;
LPDIRECT3DTEXTURE9 CLeftArrowTexture = NULL;
LPDIRECT3DTEXTURE9 CRightArrowTexture = NULL;
LPDIRECT3DTEXTURE9 CNLeftArrowTexture = NULL;
LPDIRECT3DTEXTURE9 CNRightArrowTexture = NULL;
LPDIRECT3DTEXTURE9 RLeftArrowTexture = NULL;
LPDIRECT3DTEXTURE9 RRightArrowTexture = NULL;
LPDIRECT3DTEXTURE9 RMLeftArrowTexture = NULL;
LPDIRECT3DTEXTURE9 RMRightArrowTexture = NULL;
LPDIRECT3DTEXTURE9 GameMsgBoxButtonTexture = NULL;
LPDIRECT3DTEXTURE9 GameMsgBoxButtonActiveTexture = NULL;
LPDIRECT3DTEXTURE9 NormalControlsTexture = NULL;
LPDIRECT3DTEXTURE9 ActiveControlsTexture = NULL;
LPDIRECT3DTEXTURE9 NormalBoxTexture = NULL;
LPDIRECT3DTEXTURE9 BBoxTexture = NULL;
LPDIRECT3DTEXTURE9 RBoxTexture = NULL;
LPDIRECT3DTEXTURE9 SeparatTexture = NULL;
LPDIRECT3DTEXTURE9 MenuInActiveButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuActiveButtonTexture = NULL;
LPDIRECT3DTEXTURE9 NormalCheckBoxTexture = NULL;
LPDIRECT3DTEXTURE9 ActiveCheckBoxTexture = NULL;
LPDIRECT3DTEXTURE9 ButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ExitButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ScrollTexture = NULL;
LPDIRECT3DTEXTURE9 ScrollBarTexture = NULL;
LPDIRECT3DTEXTURE9 ProfileButtonTexture = NULL;


LPDIRECT3DTEXTURE9 TDTexture = NULL;
LPDIRECT3DTEXTURE9 TDPassTexture = NULL;
LPDIRECT3DTEXTURE9 DMTexture = NULL;
LPDIRECT3DTEXTURE9 BRTexture = NULL;
LPDIRECT3DTEXTURE9 ChaserTexture = NULL;
LPDIRECT3DTEXTURE9 CaptainTexture = NULL;
LPDIRECT3DTEXTURE9 SiegeTexture = NULL;
LPDIRECT3DTEXTURE9 ArenaTexture = NULL;
LPDIRECT3DTEXTURE9 ArcadeTexture = NULL;
LPDIRECT3DTEXTURE9 WarfareTexture = NULL;

LPDIRECT3DTEXTURE9 ActivatedTDTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedTDPassTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedDMTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedBRTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedChaserTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedCaptainTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedSiegeTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedArenaTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedArcadeTexture = NULL;
LPDIRECT3DTEXTURE9 ActivatedWarfareTexture = NULL;
#pragma endregion

#pragma region Marks
LPDIRECT3DTEXTURE9 XCrossTexture = NULL;
LPDIRECT3DTEXTURE9 ClaimMarkTexture = NULL;
LPDIRECT3DTEXTURE9 EvTextture = NULL;
#pragma endregion

#pragma region Channel
LPDIRECT3DTEXTURE9 CompetitiveChannelOffTexture = NULL;
LPDIRECT3DTEXTURE9 CompetitiveChannelOnTexture = NULL;
LPDIRECT3DTEXTURE9 FreeChannelOffTexture = NULL;
LPDIRECT3DTEXTURE9 FreeChannelOnTexture = NULL;
LPDIRECT3DTEXTURE9 RankChannelOffTexture = NULL;
LPDIRECT3DTEXTURE9 RankChannelOnTexture = NULL;
LPDIRECT3DTEXTURE9 ClanWarChannelOffTexture = NULL;
LPDIRECT3DTEXTURE9 ClanWarChannelOnTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelBackGroundTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelExitButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelServerButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelServerButtonOnTexture = NULL;
#pragma endregion

#pragma region Exchange
LPDIRECT3DTEXTURE9 ExchangeP1Texture = NULL;
LPDIRECT3DTEXTURE9 ExchangeP1Texture2 = NULL;
LPDIRECT3DTEXTURE9 ExchangeP1Texture3 = NULL;

LPDIRECT3DTEXTURE9 ExchangeP2Texture = NULL;
LPDIRECT3DTEXTURE9 ExchangeP2Texture2 = NULL;
LPDIRECT3DTEXTURE9 ExchangeP2Texture3 = NULL;
#pragma endregion

#pragma region GameReward
LPDIRECT3DTEXTURE9 GameRewardItemTexture = NULL;
LPDIRECT3DTEXTURE9 GameRewardCardTexture = NULL;
LPDIRECT3DTEXTURE9 GameRewardSuppliesTexture = NULL;
LPDIRECT3DTEXTURE9 GameRewardEventTexture = NULL;
#pragma endregion

#pragma region ChatSystem
LPDIRECT3DTEXTURE9 ChatBoxTexture = NULL;
LPDIRECT3DTEXTURE9 ChatTextBoxTexture = NULL;
LPDIRECT3DTEXTURE9 ChatAllButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatClanButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatWhisperButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatSupportButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatUnionButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatTeamButtonTexture = NULL;

LPDIRECT3DTEXTURE9 RoomChatBoxTexture = NULL;
LPDIRECT3DTEXTURE9 RoomChatTextBoxTexture = NULL;

/*LPDIRECT3DTEXTURE9 RoomChatMenuButtonTexture = NULL;
LPDIRECT3DTEXTURE9 RoomChatMenuButton2Texture = NULL;
LPDIRECT3DTEXTURE9 RoomChatMenuButton3Texture = NULL;
LPDIRECT3DTEXTURE9 RoomChatMenuButton4Texture = NULL;*/

LPDIRECT3DTEXTURE9 ChatMenuButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatMenuButton2Texture = NULL;
LPDIRECT3DTEXTURE9 ChatMenuButton3Texture = NULL;
LPDIRECT3DTEXTURE9 ChatMenuButton4Texture = NULL;


LPDIRECT3DTEXTURE9 RoomChatMainButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatMainButtonTexture = NULL;

LPDIRECT3DTEXTURE9 ChatPlayerListButtonTexture = NULL;
LPDIRECT3DTEXTURE9 ChatPlayerListActiveButtonTexture = NULL;
#pragma endregion

#pragma region Player List
LPDIRECT3DTEXTURE9 PlayerListBoxTexture = NULL;
LPDIRECT3DTEXTURE9 CPlayerLevelTexture = NULL;
LPDIRECT3DTEXTURE9 CPlayerRankTexture = NULL;
LPDIRECT3DTEXTURE9 PlayerBackGroundTexture = NULL;
LPDIRECT3DTEXTURE9 CPlayerNameTagTexture = NULL;
#pragma endregion

#pragma region Game Menu
LPDIRECT3DTEXTURE9 MenuShopButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuInventoryButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuCardButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuSettingButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuHelpButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuEnchantsButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuCommunityButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuRandomShopButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuMessagesButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuMissionButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuClanButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuInviteButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MenuActivatedButtonTexture = NULL;
LPDIRECT3DTEXTURE9 MainBGTexture = NULL;
#pragma endregion

LPDIRECT3DTEXTURE9 TestTexture = NULL;
LPDIRECT3DTEXTURE9 RankTexture = NULL;
LPDIRECT3DTEXTURE9 QuestReward = NULL;
LPDIRECT3DTEXTURE9 Quest2Reward = NULL;
LPDIRECT3DTEXTURE9 Quest3Reward = NULL;

LPDIRECT3DTEXTURE9 ChannelListUITexture = NULL;
LPDIRECT3DTEXTURE9 ChannelBoredTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelFreeTexture = NULL;
LPDIRECT3DTEXTURE9 ChannelFreeOnTexture = NULL;
LPDIRECT3DTEXTURE9 CompetitiveTexture = NULL;
LPDIRECT3DTEXTURE9 CompetitiveOnTexture = NULL;
LPDIRECT3DTEXTURE9 ClanBattelsTexture = NULL;
LPDIRECT3DTEXTURE9 ClanBattelsOnTexture = NULL;
LPDIRECT3DTEXTURE9 RankChallengeTexture = NULL;
LPDIRECT3DTEXTURE9 RankChallengeOnTexture = NULL;
#pragma endregion

#pragma region Save Load Confing
void loadConfig()
{
	std::ifstream fin("_resources\\2560369bed2d17e1");
	std::string line;
	if (fin.is_open())
	{
		while (getline(fin, line)) {
			/*line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());*/
			if (line[0] == '#' || line.empty())
				continue;

			std::string Key = (line.substr(0, line.find("=")));
			std::string Value(line.substr(line.find("=") + 1));

			if (Key == "Auto Boost")
				AutoBoost = std::stoi(Value);
			if (Key == "Stable Improve FPS")
				StableFPS = std::stoi(Value);
			if (Key == "FPS Load")
				FPSLoad = std::stoi(Value);
			if (Key == "Improve Client")
				ImproveClient = std::stoi(Value);
			if (Key == "Active Char Cam")
				ActiveCharCam = std::stoi(Value);
			if (Key == "Discord RPC")
				RPCActvion = std::stoi(Value);
			if (Key == "FPS")
				FPS = std::stof(Value);
			if (Key == "Activate Voice Actions")
				ActiveVAction = std::stoi(Value);
			if (Key == "Custom Actions")
				ActiveCVAction = std::stoi(Value);
			if (Key == "FOV")
				Fov = std::stof(Value);
			if (Key == "Char Cam")
				CharCam = std::stof(Value);
			if (Key == "Aim Position")
				AimPos = std::stof(Value);
			if (Key == "Mini Map")
				MapSize = std::stof(Value);

			if (Key == "Action")
				strcpy_s(Action, Value.length() + 1, Value.c_str());
			if (Key == "Action 2")
				strcpy_s(Action2, Value.length() + 1, Value.c_str());
			if (Key == "Action 3")
				strcpy_s(Action3, Value.length() + 1, Value.c_str());
			if (Key == "Action 4")
				strcpy_s(Action4, Value.length() + 1, Value.c_str());
			if (Key == "Action 5")
				strcpy_s(Action5, Value.length() + 1, Value.c_str());
			if (Key == "Action 6")
				strcpy_s(Action6, Value.length() + 1, Value.c_str());
			if (Key == "Action 7")
				strcpy_s(Action7, Value.length() + 1, Value.c_str());
			if (Key == "Action 8")
				strcpy_s(Action8, Value.length() + 1, Value.c_str());
			if (Key == "Action 9")
				strcpy_s(Action9, Value.length() + 1, Value.c_str());
			if (Key == "Action 10")
				strcpy_s(Action10, Value.length() + 1, Value.c_str());
			if (Key == "Action 11")
				strcpy_s(Action11, Value.length() + 1, Value.c_str());
			if (Key == "Action 12")
				strcpy_s(Action12, Value.length() + 1, Value.c_str());

			Configs.insert(std::pair<std::string, std::string>(Key, Value));
		}
	}
}

void saveConfig() {
	std::ofstream myfile("_resources\\2560369bed2d17e1");
	if (myfile.is_open())
	{
		auto cFPS = Configs.find("FPS");
		if (cFPS != Configs.end())
			cFPS->second = std::to_string(FPS);
		else
			Configs.insert(std::pair<std::string, std::string>("FPS", std::to_string(FPS)));

		auto cFOV = Configs.find("FOV");
		if (cFOV != Configs.end())
			cFOV->second = std::to_string(Fov);
		else
			Configs.insert(std::pair<std::string, std::string>("FOV", std::to_string(Fov)));

		auto cMiniMap = Configs.find("Mini Map");
		if (cMiniMap != Configs.end())
			cMiniMap->second = std::to_string(MapSize);
		else
			Configs.insert(std::pair<std::string, std::string>("Mini Map", std::to_string(MapSize)));

		auto cCharCam = Configs.find("Char Cam");
		if (cCharCam != Configs.end())
			cCharCam->second = std::to_string(CharCam);
		else
			Configs.insert(std::pair<std::string, std::string>("Char Cam", std::to_string(CharCam)));

		auto cAimPosition = Configs.find("Aim Position");
		if (cAimPosition != Configs.end())
			cAimPosition->second = std::to_string(AimPos);
		else
			Configs.insert(std::pair<std::string, std::string>("Aim Position", std::to_string(AimPos)));

		auto cActiveCharCam = Configs.find("Active Char Cam");
		if (cActiveCharCam != Configs.end())
			cActiveCharCam->second = std::to_string(ActiveCharCam);
		else
			Configs.insert(std::pair<std::string, std::string>("Active Char Cam", std::to_string(ActiveCharCam)));

		auto cStableFPS = Configs.find("Stable Improve FPS");
		if (cStableFPS != Configs.end())
			cStableFPS->second = std::to_string(StableFPS);
		else
			Configs.insert(std::pair<std::string, std::string>("Stable Improve FPS", std::to_string(StableFPS)));

		auto cFPSLoad = Configs.find("FPS Load");
		if (cFPSLoad != Configs.end())
			cFPSLoad->second = std::to_string(FPSLoad);
		else
			Configs.insert(std::pair<std::string, std::string>("FPS Load", std::to_string(FPSLoad)));

		auto cImproveClient = Configs.find("Improve Client");
		if (cImproveClient != Configs.end())
			cImproveClient->second = std::to_string(ImproveClient);
		else
			Configs.insert(std::pair<std::string, std::string>("Improve Client", std::to_string(ImproveClient)));

		auto cAutoBoost = Configs.find("Auto Boost");
		if (cAutoBoost != Configs.end())
			cAutoBoost->second = std::to_string(AutoBoost);
		else
			Configs.insert(std::pair<std::string, std::string>("Auto Boost", std::to_string(AutoBoost)));

		auto cActiveVAction = Configs.find("Activate Voice Actions");
		if (cActiveVAction != Configs.end())
			cActiveVAction->second = std::to_string(ActiveVAction);
		else
			Configs.insert(std::pair<std::string, std::string>("Activate Voice Actions", std::to_string(ActiveVAction)));

		auto cActiveCVAction = Configs.find("Custom Actions");
		if (cActiveCVAction != Configs.end())
			cActiveCVAction->second = std::to_string(ActiveCVAction);
		else
			Configs.insert(std::pair<std::string, std::string>("Custom Actions", std::to_string(ActiveCVAction)));

		auto cRPCActvion = Configs.find("Discord RPC");
		if (cRPCActvion != Configs.end())
			cRPCActvion->second = std::to_string(RPCActvion);
		else
			Configs.insert(std::pair<std::string, std::string>("Discord RPC", std::to_string(RPCActvion)));


		auto cAction = Configs.find("Action");
		if (cAction != Configs.end())
			cAction->second = Action;
		else
			Configs.insert(std::pair<std::string, std::string>("Action", Action));

		auto cAction2 = Configs.find("Action 2");
		if (cAction2 != Configs.end())
			cAction2->second = Action2;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 2", Action2));

		auto cAction3 = Configs.find("Action 3");
		if (cAction3 != Configs.end())
			cAction3->second = Action3;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 3", Action3));

		auto cAction4 = Configs.find("Action 4");
		if (cAction4 != Configs.end())
			cAction4->second = Action4;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 4", Action4));

		auto cAction5 = Configs.find("Action 5");
		if (cAction5 != Configs.end())
			cAction5->second = Action5;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 5", Action5));

		auto cAction6 = Configs.find("Action 6");
		if (cAction6 != Configs.end())
			cAction6->second = Action6;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 6", Action6));

		auto cAction7 = Configs.find("Action 7");
		if (cAction7 != Configs.end())
			cAction7->second = Action7;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 7", Action7));

		auto cAction8 = Configs.find("Action 8");
		if (cAction8 != Configs.end())
			cAction8->second = Action8;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 8", Action8));

		auto cAction9 = Configs.find("Action 9");
		if (cAction9 != Configs.end())
			cAction9->second = Action9;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 9", Action9));

		auto cAction10 = Configs.find("Action 10");
		if (cAction10 != Configs.end())
			cAction10->second = Action10;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 10", Action10));

		auto cAction11 = Configs.find("Action 11");
		if (cAction11 != Configs.end())
			cAction11->second = Action11;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 11", Action11));

		auto cAction12 = Configs.find("Action 12");
		if (cAction12 != Configs.end())
			cAction12->second = Action12;
		else
			Configs.insert(std::pair<std::string, std::string>("Action 12", Action12));

		for (std::pair<std::string, std::string> it : Configs)
		{
			myfile << it.first + "=" + it.second + "\n";
		}
		myfile.close();
	}
}
#pragma endregion

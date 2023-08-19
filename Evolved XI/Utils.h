#pragma once
class Memory
{
public:
	void WriteASM(DWORD dwAddy, DWORD dwASM, int len);
	unsigned int ReadMemP(uintptr_t ptr, std::vector<unsigned int> offsets);
	std::string ReadMemPStr(uintptr_t ptr, std::vector<unsigned int> offsets);
	unsigned int ReadMem(uintptr_t ptr);
	float ReadMemF(uintptr_t ptr);
	void WriteMemP(uintptr_t ptr, std::vector<unsigned int> offsets, int Value);
	BYTE ReadMemB(uintptr_t ptr);
	void WriteMem(uintptr_t ptr, int Value);
	void WriteMem(uintptr_t ptr, float Value);
	void WriteMem(uintptr_t ptr, bool Value);
	void WriteMem(uintptr_t ptr, double Value);
	std::string ReadStr(uintptr_t ptr);
	template <class T> T MemRead(uintptr_t Addr);
	VOID CreateDetour(void* toHook, void* ourFunct, int len);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, const char* szMask);
	uintptr_t FindOffAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	std::string StrReplaceV(std::string text, char toreplace, char replace);
	std::string StrReplace(std::string text);
	std::string StrReplace(std::string text, std::string first, std::string last);
	std::string StrLowerCases(std::string data);
	bool StrToBool(std::string var);
	bool is_number(const std::string& s);
	std::string get_str_between_str(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
	std::vector<std::string> Split(const std::string& s, char delimiter);
	bool StrStartWith(std::string str, std::string check);
	char* StrToChar(std::string str, char* Value);
	static int TimerCounter;
	bool CountDownTimer();
	int StdToInt(std::string str);
	float StdToFloat(std::string str);
	long StdToLong(std::string str);
};

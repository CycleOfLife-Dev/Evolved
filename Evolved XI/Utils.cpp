#include "stdafx.h"
#include "Utils.h"

#pragma region Pattern
DWORD WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD Memory::FindPattern(std::string moduleName, BYTE* Mask, const char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	if (Address == NULL)
		return 0;
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

#pragma endregion

#pragma region ReadWriteMemory
uintptr_t Memory::FindOffAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(GetCurrentProcess(), (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

unsigned int Memory::ReadMemP(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	unsigned int value;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)Memory::FindOffAAddy(ptr, offsets), &value, sizeof(unsigned int), 0);
	return value;
}

std::string Memory::ReadMemPStr(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	std::string value;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)Memory::FindOffAAddy(ptr, offsets), &value, sizeof(value), NULL);
	return value;
}

unsigned int Memory::ReadMem(uintptr_t ptr)
{
	unsigned int value;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)ptr, &value, sizeof(unsigned int), 0);
	return value;
}

float Memory::ReadMemF(uintptr_t ptr)
{
	float value;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)ptr, &value, sizeof(float), 0);
	return value;
}

BYTE Memory::ReadMemB(uintptr_t ptr)
{
	unsigned int value;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)ptr, &value, sizeof(BYTE), 0);
	return value;
}

template <class T> T Memory::MemRead(uintptr_t ptr)
{
	T value;
	ReadProcessMemory(GetCurrentProcess(), (LPCVOID)ptr, &value, sizeof(value), NULL);
	return value;
}

std::string Memory::ReadStr(uintptr_t ptr)
{
	std::string value;
	unsigned int Size = 0;
	ReadProcessMemory(GetCurrentProcess(), (BYTE*)(ptr + 0x10), &Size, sizeof(Size), 0);

	if (Size > 15)
		value = (char*)((char*)FindOffAAddy(ptr, { 0x00 }));
	else
		ReadProcessMemory(GetCurrentProcess(), (BYTE*)(ptr), (LPVOID)&value, sizeof(value), NULL);

	return value;
}

void Memory::WriteMemP(uintptr_t ptr, std::vector<unsigned int> offsets, int Value)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)FindOffAAddy(ptr, offsets), &Value, sizeof(Value), NULL);
}


void Memory::WriteMem(uintptr_t ptr, int Value)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)ptr, &Value, sizeof(Value), NULL);
}

void Memory::WriteMem(uintptr_t ptr, float Value)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)ptr, &Value, sizeof(Value), NULL);
}

void Memory::WriteMem(uintptr_t ptr, double Value)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)ptr, &Value, sizeof(Value), NULL);
}

void Memory::WriteMem(uintptr_t ptr, bool Value)
{
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)ptr, &Value, sizeof(Value), NULL);
}

void Memory::WriteASM(DWORD dwAddy, DWORD dwASM, int len)
{
	unsigned long Protection;
	VirtualProtect((void*)dwAddy, len, PAGE_EXECUTE_READWRITE, &Protection);
	memcpy((void*)dwAddy, (const void*)dwASM, len);
	VirtualProtect((void*)dwAddy, len, Protection, &Protection);
}
#pragma endregion

#pragma region String Stuffs

int Memory::StdToInt(std::string str)
{
	if (str.empty())
		return 0;
	return std::atoi(str.c_str());
}

float Memory::StdToFloat(std::string str)
{
	if (str.empty())
		return 0;
	return (float)std::atof(str.c_str());
}

long Memory::StdToLong(std::string str)
{
	if (str.empty())
		return 0;
	return std::atol(str.c_str());
}

bool Memory::StrStartWith(std::string str, std::string check)
{
	if (str.rfind(check, 0) == 0) {
		return true;
	}
	return false;
}

char* Memory::StrToChar(std::string str, char* Value)
{
	strcpy_s(Value, str.length() + 1, str.c_str());
	return Value;
}
std::string retrieveString(char* buf, size_t  max)
{

	size_t len = 0;
	while ((len < max) && (buf[len] != '\0')) {
		len++;
	}

	return std::string(buf, len);
}

std::vector<std::string> Memory::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string Memory::get_str_between_str(const std::string& s,
	const std::string& start_delim,
	const std::string& stop_delim)
{
	unsigned first_delim_pos = s.find(start_delim);
	unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
	unsigned last_delim_pos = s.find(stop_delim);

	return s.substr(end_pos_of_first_delim,
		last_delim_pos - end_pos_of_first_delim);
}

std::string Memory::StrLowerCases(std::string data)
{
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::tolower(c);
		});

	return data;
}

std::string Memory::StrReplace(std::string text, std::string first, std::string last)
{
	std::size_t pos = text.find(first);
	if (pos != std::string::npos) {
		text.replace(pos, first.length(), last);
	}
	return text;
}

std::string Memory::StrReplace(std::string text)
{
	std::replace(text.begin(), text.end(), ' ', '_');
	return text;
}

std::string Memory::StrReplaceV(std::string text, char toreplace, char replace)
{
	std::replace(text.begin(), text.end(), toreplace, replace);
	return text;
}

bool Memory::StrToBool(std::string var)
{
	 if (Memory::StrLowerCases(var) == "true")
		 return true;   
	 else if (Memory::StrLowerCases(var) == "false")
		 return false;

	 return false;
}

bool Memory::is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
#pragma endregion

int Memory::TimerCounter = 0;
bool Memory::CountDownTimer()
{
	while (TimerCounter >= 1)
	{
		TimerCounter--;

		if (TimerCounter <= 1)
			return true;
		else
			return false;
		//Sleep(1);
	}
	return false;
}

#pragma region Hook
VOID Memory::CreateDetour(void* toHook, void* ourFunct, int len)
{
	if (len < 5) {
		return;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
}
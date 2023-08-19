#pragma once
#include "stdafx.h"
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp")

time_t dump_time;
struct tm dump_local_time;
char dumpname[30];

// Custom minidump callback 
BOOL CALLBACK MyMiniDumpCallback(PVOID pParam, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput)
{
	BOOL bRet = FALSE;

	// Check parameters 

	if (pInput == 0)
		return FALSE;

	if (pOutput == 0)
		return FALSE;

	// Process the callbacks 
	switch (pInput->CallbackType)
	{
	case IncludeModuleCallback:
	{
		// Include the module into the dump 
		bRet = TRUE;
	}
	break;

	case IncludeThreadCallback:
	{
		// Include the thread into the dump 
		bRet = TRUE;
	}
	break;

	case ModuleCallback:
	{
		// Does the module have ModuleReferencedByMemory flag set ? 
		if (!(pOutput->ModuleWriteFlags & ModuleReferencedByMemory)) {
			// No, it does not - exclude it 
			// wprintf(L"Excluding module: %s \n", pInput->Module.FullPath);
			pOutput->ModuleWriteFlags &= (~ModuleWriteModule);
		}
		bRet = TRUE;
	}
	break;

	case ThreadCallback:
	{
		// Include all thread information into the minidump 
		bRet = TRUE;
	}
	break;

	case ThreadExCallback:
	{
		// Include this information 
		bRet = TRUE;
	}
	break;

	case MemoryCallback:
	{
		// We do not include any information here -> return FALSE 
		bRet = FALSE;
	}
	break;

	case CancelCallback:
	{
		/*if (!MemoryCallbackCalled)
		{
			// Continue receiving CancelCallback callbacks
			pOutput->Cancel = FALSE;
			pOutput->CheckCancel = TRUE;
		}
		else
		{
			// No cancel callbacks anymore
			pOutput->Cancel = FALSE;
			pOutput->CheckCancel = FALSE;
		}*/
	}
	break;
	}

	return bRet;
}

bool CreateMiniDump(EXCEPTION_POINTERS* pExceptionInfo)
{
	time(&dump_time);
	localtime_s(&dump_local_time, &dump_time);
	strftime(dumpname, sizeof(dumpname), "%Y%m%d_%H%M%S.dmp", &dump_local_time);

	std::string path = "Dump/Legion_" ;
	path += dumpname;

	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		//	printf("Exception dump file is not created. Error code: %u Path: %s", GetLastError(), szDumpPath);
		return false;
	}

	// Create the minidump 
	MINIDUMP_EXCEPTION_INFORMATION mdei;
	mdei.ThreadId = GetCurrentThreadId();
	mdei.ExceptionPointers = pExceptionInfo;
	mdei.ClientPointers = FALSE;

	MINIDUMP_CALLBACK_INFORMATION mci;
	mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)MyMiniDumpCallback;
	mci.CallbackParam = 0;

	MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory);
	
	bool rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (pExceptionInfo != 0) ? &mdei : 0, 0, &mci);

	// Close the file 
	CloseHandle(hFile);
	return true;
}

LONG WINAPI ExceptionFilterSeh(EXCEPTION_POINTERS* pExceptionInfo)
{
	// Custom handler
//	CExceptionHandlers::OnExceptionThrowed(pExceptionInfo);

	// Minidump handler
	if (pExceptionInfo && pExceptionInfo->ExceptionRecord)
	{
		if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
		{
			HANDLE hThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(CreateMiniDump), pExceptionInfo, 0, NULL);
			if (hThread == NULL)
			{
				WaitForSingleObject(hThread, INFINITE);
				CloseHandle(hThread);
			}
		}
		else
		{
			CreateMiniDump(pExceptionInfo);
		}

		//	printf("Seh exception triggered. Code: %p", pExceptionInfo->ExceptionRecord->ExceptionCode);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

bool InitMiniDump()
{

	if (SetUnhandledExceptionFilter(ExceptionFilterSeh))
	{
		//printf( "Mini dump generator Exception handler is succesfully created!");
		return true;
	}
	//printf("Mini dump generator Exception handler is NOT created!");
	return false;
}


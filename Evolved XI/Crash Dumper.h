#pragma once
#include "MiniDump.h"

void LogCrash(std::string error)
{
	if (CreateDirectory("Crash", NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		time(&dump_time);
		localtime_s(&dump_local_time, &dump_time);
		strftime(dumpname, sizeof(dumpname), "%Y%m%d_%H%M%S.txt", &dump_local_time);

		std::string path = "Crash/Crash_";
		path += dumpname;

		std::ofstream file;
		file.open(path);
		file << error;
		file.close();
	}
}
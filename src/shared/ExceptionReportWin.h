#pragma once

#include "Common.h"
#include <fstream>
#include <list>
#include <sstream>
#include <DbgHelp.h>


class ExceptionReportWin
{
	HANDLE mProcess;
	std::ofstream mCrashFile;
	LPEXCEPTION_POINTERS mInfo;
	STACKFRAME64 mCurStackFrame;

	std::list<std::string> mParameters;
	std::list<std::string> mLocals;
	std::list<std::string> mGlobals;

	void writeCppExceptionInfo();
	void writeExceptionType();
	void writeCallStackSimple();
	void writeCallStackDetail();

	template<typename T>
	std::string convertValueDirect(uint32 base, uint32 offset) {
		std::stringstream strm;
		strm << *(T*)(base + offset);
		return strm.str();
	}

	template<>
	std::string convertValueDirect<wchar_t*>(uint32 base, uint32 offset) {
		wchar_t* str = *(wchar_t**)(base + offset);
		if(str == nullptr)
			return "nullptr";

		std::stringstream strm;
		strm << "\"" << toAnsi(str) << "\"";
		return strm.str();
	}

	template<>
	std::string convertValueDirect<char*>(uint32 base, uint32 offset) {
		char* str = *(char**)(base + offset);
		if(str == nullptr)
			return "nullptr";

		std::stringstream strm;
		strm << "\"" << str << "\"";
		return strm.str();
	}

	std::string getBasicTypeName(enum BasicType type, uint64 len);
	std::string getBasicTypeValue(enum BasicType type, uint64 len, uint32 base, uint32 offset);
	std::string getBasicTypeValuePtr(enum BasicType type, uint64 len, uint32 base, uint32 offset);
	void addFrameSymbol(void* sym, ULONG size);

	static BOOL WINAPI enumSymbolsCallback(void* symbol, ULONG size, PVOID param);

	std::string getFrameString(DWORD64 address, std::string& function);
	bool getSourceLine(DWORD64 address, std::string& line);
	std::string toAnsi(const std::wstring& str);

public:
	ExceptionReportWin();
	~ExceptionReportWin();

	void recordExceptionInfo(LPEXCEPTION_POINTERS exp);
};
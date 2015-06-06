#include "Common.h"
#include "ExceptionReportWin.h"
#include "revision.h"
#include "revision_nr.h"

#include <iomanip>
#include "cvconst.h"

ExceptionReportWin::ExceptionReportWin() {
	mProcess = GetCurrentProcess();
	SymInitialize(mProcess, nullptr, TRUE);
}

ExceptionReportWin::~ExceptionReportWin() {
	SymCleanup(mProcess);
}

std::string ExceptionReportWin::toAnsi(const std::wstring& str) {
	uint32 numBytes = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0, nullptr, nullptr);
	if(numBytes == 0)
		return "";

	std::vector<char> ansi(numBytes);
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.length(), ansi.data(), numBytes, nullptr, nullptr);
	ansi.push_back('\0');
	return ansi.data();
}

void ExceptionReportWin::recordExceptionInfo(LPEXCEPTION_POINTERS ep) {
	time_t t = time(nullptr);
	tm* tm = localtime(&t);
	std::stringstream fileName;
	fileName << "Crashes\\Exception_" << std::setw(2) << std::setfill('0') << tm->tm_mday << "_" << std::setw(2) << std::setfill('0') << tm->tm_mon + 1 << "_" << tm->tm_year + 1900
		<< "_" << std::setw(2) << std::setfill('0') << tm->tm_hour << "_" << std::setw(2) << std::setfill('0') << tm->tm_min << "_" << std::setw(2) << std::setfill('0') << tm->tm_sec << ".txt";

	mCrashFile.open(fileName.str().c_str(), std::ios::out);
	mCrashFile << "Revision: " << REVISION_DATE << " - " << REVISION_TIME << " - " REVISION_NR << " - " REVISION_ID << std::endl;
	mCrashFile << "Time: " << std::setw(2) << std::setfill('0') << tm->tm_hour << ":" << std::setw(2) << std::setfill('0') << tm->tm_min << std::setw(1) << std::endl;
	mCrashFile << "------------------------------------------------" << std::endl;
	mCrashFile << std::endl;

	mInfo = ep;

	SymRefreshModuleList(mProcess);

	writeExceptionType();
	mCrashFile << std::endl << std::endl;
	writeCallStackSimple();
	mCrashFile << std::endl;
	mCrashFile << "------------------------------------------------" << std::endl;
	mCrashFile << std::endl;

	writeCallStackDetail();
	mCrashFile << std::endl;

	mInfo = nullptr;
	mCrashFile.close();
}

void ExceptionReportWin::writeExceptionType() {
	if(mInfo == nullptr)
		return;

	mCrashFile << "Exception: " << (LPVOID)mInfo->ExceptionRecord->ExceptionCode << " - ";
#define SC(code) \
	case code:\
		mCrashFile << #code;\
		break

	switch(mInfo->ExceptionRecord->ExceptionCode) {
		SC(EXCEPTION_ACCESS_VIOLATION);
		SC(EXCEPTION_BREAKPOINT);
		SC(EXCEPTION_SINGLE_STEP);
		SC(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
		SC(EXCEPTION_FLT_DENORMAL_OPERAND);
		SC(EXCEPTION_FLT_DIVIDE_BY_ZERO);
		SC(EXCEPTION_FLT_INEXACT_RESULT);
		SC(EXCEPTION_FLT_INVALID_OPERATION);
		SC(EXCEPTION_FLT_OVERFLOW);
		SC(EXCEPTION_FLT_STACK_CHECK);
		SC(EXCEPTION_FLT_UNDERFLOW);
		SC(EXCEPTION_INT_DIVIDE_BY_ZERO);
		SC(EXCEPTION_INT_OVERFLOW);
		SC(EXCEPTION_PRIV_INSTRUCTION);
		SC(EXCEPTION_IN_PAGE_ERROR);
		SC(EXCEPTION_ILLEGAL_INSTRUCTION);
		SC(EXCEPTION_NONCONTINUABLE);
		SC(EXCEPTION_STACK_OVERFLOW);
		SC(EXCEPTION_INVALID_DISPOSITION);
		SC(EXCEPTION_GUARD_PAGE);
		SC(EXCEPTION_INVALID_HANDLE);

	case 0xE06D7363:
		writeCppExceptionInfo();
		break;
	}

	switch(mInfo->ExceptionRecord->ExceptionCode) {
	case EXCEPTION_ACCESS_VIOLATION:
	case EXCEPTION_IN_PAGE_ERROR:
		{
			mCrashFile << std::endl << "Additional information: ";
			mCrashFile << "The thread " << GetCurrentThreadId() << " attempted to ";
			if(mInfo->ExceptionRecord->ExceptionInformation[0] == 0)
				mCrashFile << "read";
			else if(mInfo->ExceptionRecord->ExceptionInformation[0] == 1)
				mCrashFile << "write";
			else if(mInfo->ExceptionRecord->ExceptionInformation[0] == 8)
				mCrashFile << "execute";

			mCrashFile << " protected memory at 0x";
			mCrashFile << std::setw(8) << std::setfill('0') << std::uppercase << std::hex;
			mCrashFile << mInfo->ExceptionRecord->ExceptionInformation[1] << std::endl;
			mCrashFile << std::nouppercase << std::setw(1);
		}
		break;
	}
}

void ExceptionReportWin::writeCppExceptionInfo() {
	static const std::type_info& stdInfo = typeid(std::exception);
	static const std::type_info& charInfo = typeid(char*);
	static const std::type_info& ccharInfo = typeid(const char*);
	static const std::type_info& wcharInfo = typeid(wchar_t*);
	static const std::type_info& cwcharInfo = typeid(const wchar_t*);

	std::stringstream errMsgStrm;

	std::type_info* lastInfo = nullptr;
	std::type_info* baseType = nullptr;
		
	LPEXCEPTION_POINTERS info = mInfo;
	LPBYTE pBaseObj = (LPBYTE)info->ExceptionRecord->ExceptionInformation[1];
	_CatchableTypeArray* typeArray = ((_ThrowInfo*)info->ExceptionRecord->ExceptionInformation[2])->pCatchableTypeArray;

	for(uint32 i = 0; i < (uint32)typeArray->nCatchableTypes; ++i)
	{
		_CatchableType& type = *(typeArray->arrayOfCatchableTypes[i]);
		std::type_info* tinfo = (std::type_info*)type.pType;
		if(i == 0)
			baseType = tinfo;

		if(*tinfo == stdInfo)
		{
			std::exception* pException = (std::exception*)(pBaseObj + type.thisDisplacement.mdisp);
			errMsgStrm << baseType->name() << "::what() = " << pException->what();
		}

		if(*tinfo == charInfo || *tinfo == ccharInfo) {
			const char** msg = (const char**)(pBaseObj + type.thisDisplacement.mdisp);
			errMsgStrm << "string \"" << (msg != nullptr ? *msg : "null") << "\"";
			break;
		}

		if(*tinfo == wcharInfo || *tinfo == wcharInfo) {
			const wchar_t** msg = (const wchar_t**)(pBaseObj + type.thisDisplacement.mdisp);
			errMsgStrm << "string \"" << toAnsi((msg != nullptr ? *msg : L"null")) << "\"";
			break;
		}

		if(i == (uint32)(typeArray->nCatchableTypes - 1)) {
			lastInfo = tinfo;
		}
	}
}

#define SET_ADDR(field, addr) field.Mode = AddrModeFlat; field.Offset = addr

void ExceptionReportWin::writeCallStackSimple() {
	CONTEXT ctx = *mInfo->ContextRecord;

	STACKFRAME64 frame = { 0 };
	SET_ADDR(frame.AddrPC, ctx.Eip);
	SET_ADDR(frame.AddrFrame, ctx.Ebp);
	SET_ADDR(frame.AddrStack, ctx.Esp);

	mCrashFile << "Stacktrace simple:" << std::endl;
	mCrashFile << "Address\t\tFunction" << std::endl;

	while(StackWalk64(IMAGE_FILE_MACHINE_I386, mProcess, GetCurrentThread(), &frame, (LPVOID)&ctx, 
		nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {

		DWORD64 frameAddr = frame.AddrPC.Offset;
		std::string unusedFunc;
		std::string str = getFrameString(frameAddr, unusedFunc);
		
		mCrashFile << (LPVOID)frameAddr << "\t" << str << std::endl;
	}
}

void ExceptionReportWin::writeCallStackDetail() {
	CONTEXT ctx = *mInfo->ContextRecord;

	STACKFRAME64 frame = { 0 };
	SET_ADDR(frame.AddrPC, ctx.Eip);
	SET_ADDR(frame.AddrFrame, ctx.Ebp);
	SET_ADDR(frame.AddrStack, ctx.Esp);

	mCrashFile << "Stacktrace extended:" << std::endl;
	while(StackWalk64(IMAGE_FILE_MACHINE_I386, mProcess, GetCurrentThread(), &frame, (LPVOID)&ctx, 
		nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {

		DWORD64 frameAddr = frame.AddrPC.Offset;

		std::string function;
		getFrameString(frameAddr, function);
		
		IMAGEHLP_STACK_FRAME curFrame;
		curFrame.InstructionOffset = frameAddr;
		mParameters.clear();
		mLocals.clear();
		mGlobals.clear();

		mCurStackFrame = frame;

		SymSetContext(mProcess, &curFrame, nullptr);
		SymEnumSymbols(mProcess, 0, nullptr, (PSYM_ENUMERATESYMBOLS_CALLBACK)&ExceptionReportWin::enumSymbolsCallback, this);

		mCrashFile << (LPVOID)frameAddr << "\t" << function << "(";
		uint32 i = 0;
		for(auto& param : mParameters) {
			if(i != 0)
				mCrashFile << ", ";
			mCrashFile << param;
			++i;
		}

		mCrashFile << ")" << std::endl;
		
		std::string code;
		if(getSourceLine(frameAddr, code)) {
			mCrashFile << "Sourcecode:" << std::endl << code << std::endl;
		}

		mCrashFile << "Locals:" << std::endl;
		for(auto& loc : mLocals) {
			mCrashFile << loc << std::endl;
		}
		if(mLocals.size() == 0)
			mCrashFile << "(none)" << std::endl;
		
		mCrashFile << std::endl;
		mCrashFile << "Globals:" << std::endl;
		for(auto& glob : mGlobals)
			mCrashFile << glob << std::endl;

		if(mGlobals.size() == 0)
			mCrashFile << "(none)" << std::endl;

		mCrashFile << std::endl;
		mCrashFile << std::endl;
	}
}

#undef SET_ADDR

BOOL ExceptionReportWin::enumSymbolsCallback(void* sym, ULONG size, PVOID context) {
	((ExceptionReportWin*)context)->addFrameSymbol(sym, size);
	return TRUE;
}

void ExceptionReportWin::addFrameSymbol(void* symp, ULONG size) {
	PSYMBOL_INFO sym = (PSYMBOL_INFO)symp;

	if(sym == nullptr || size == 0)
		return;

	enum SymTagEnum tag = (enum SymTagEnum)0;
	if(SymGetTypeInfo(mProcess, sym->ModBase, sym->TypeIndex, TI_GET_SYMTAG, &tag) == FALSE)
		return;

	std::stringstream symString;

	switch(tag) {
	case SymTagBaseType:
		{
			BasicType subType = (BasicType)0;
			if(!SymGetTypeInfo(mProcess, sym->ModBase, sym->TypeIndex, TI_GET_BASETYPE, &subType))
				return;

			uint64 typeLen = 0;
			if(!SymGetTypeInfo(mProcess, sym->ModBase, sym->TypeIndex, TI_GET_LENGTH, &typeLen))
				return;

			symString << getBasicTypeName(subType, typeLen) << " " << sym->Name << " = " << getBasicTypeValue(subType, typeLen, mCurStackFrame.AddrFrame.Offset, sym->Address);
		}
		break;

	case SymTagUDT:
		{
			std::string type;
			LPWSTR symName = NULL;
			BOOL ret = SymGetTypeInfo(mProcess, sym->ModBase, sym->TypeIndex, TI_GET_SYMNAME, &symName);
			if(ret == TRUE) {
				type = toAnsi(symName);
				LocalFree(symName);
			} else {
				type = "UDT";
			}

			symString << type << " " << sym->Name << " = { complex }";
		}
		break;

	case SymTagPointerType:
		{
			DWORD subType = 0;
			if(!SymGetTypeInfo(mProcess, sym->ModBase, sym->TypeIndex, TI_GET_TYPE, &subType))
				return;

			if(!SymGetTypeInfo(mProcess, sym->ModBase, subType, TI_GET_SYMTAG, &tag))
				return;

			std::stringstream strm;
			LPVOID ptr = *(LPVOID*)(mCurStackFrame.AddrFrame.Offset + sym->Address);
			strm << "0x" << std::hex << std::setfill('0') << std::setw(8) << ptr;
			std::string value = strm.str();
			std::string type = "";

			switch(tag)
			{
			case SymTagUDT:
				{
					LPWSTR symName = NULL;
					BOOL ret = SymGetTypeInfo(mProcess, sym->ModBase, subType, TI_GET_SYMNAME, &symName);
					if(ret == TRUE) {
						type = toAnsi(symName);
						type += "*";
						LocalFree(symName);
					} else {
						type = "UnkPointer*";
					}
				}
				break;

			case SymTagBaseType:
				{
					uint64 typeLen = 0;
					BasicType baseType = (BasicType)0;
					SymGetTypeInfo(mProcess, sym->ModBase, subType, TI_GET_BASETYPE, &baseType);
					SymGetTypeInfo(mProcess, sym->ModBase, subType, TI_GET_LENGTH, &typeLen);
					type = getBasicTypeName(baseType, typeLen) + "*";
					value = getBasicTypeValuePtr(baseType, typeLen, mCurStackFrame.AddrFrame.Offset, sym->Address);
				}
				break;
			}

			symString << type << " " << sym->Name << " = " << value;
		}
		break;

	default:
		return;
	}

	if((sym->Flags & SYMFLAG_PARAMETER) != 0)
		mParameters.push_back(symString.str());
	else if((sym->Flags & SYMFLAG_LOCAL) != 0)
		mLocals.push_back(symString.str());
	else
		mGlobals.push_back(symString.str());
}

std::string ExceptionReportWin::getBasicTypeValue(BasicType type, uint64 len, uint32 base, uint32 offset) {
	switch(type)
	{
	case btFloat:
		return convertValueDirect<float>(base, offset);
	case btVoid:
		return "void";
	case btInt:
		switch(len)
		{
		case sizeof(int32):
			return convertValueDirect<int32>(base, offset);
		case sizeof(int64):
			return convertValueDirect<int64>(base, offset);
		case sizeof(int16):
			return convertValueDirect<int16>(base, offset);
			break;
		}
		break;
	case btUInt:
		switch(len)
		{
		case sizeof(uint32):
			return convertValueDirect<uint32>(base, offset);
		case sizeof(uint64):
			return convertValueDirect<uint64>(base, offset);
		case sizeof(uint16):
			return convertValueDirect<uint16>(base, offset);
		}
		break;
	case btChar:
		return convertValueDirect<char>(base, offset);
	case btBool:
		return convertValueDirect<bool>(base, offset);
	case btLong:
		return convertValueDirect<long>(base, offset);
	case btULong:
		return convertValueDirect<unsigned long>(base, offset);
	case btWChar:
		return convertValueDirect<wchar_t>(base, offset);
	case btBCD:
		return "BCD";
	case btCurrency:
		return "Currency";
	case btVariant:
		return "VARIANT";
	case btComplex:
		return "Complex";
	case btBit:
		return "BIT";
	case btBSTR:
		return convertValueDirect<wchar_t*>(base, offset);
	case btHresult:
		return convertValueDirect<HRESULT>(base, offset);

	default:
		{
			std::stringstream strm;
			strm << type;
			return strm.str();
		}
		break;
	}

	std::stringstream strm;
	strm << type;
	return strm.str();
}

std::string ExceptionReportWin::getBasicTypeValuePtr(BasicType type, uint64 len, uint32 base, uint32 offset) {
	switch(type)
	{
	case btFloat:
		return convertValueDirect<float*>(base, offset);
	case btVoid:
		return convertValueDirect<void*>(base, offset);
	case btInt:
		switch(len)
		{
		case sizeof(int32):
			return convertValueDirect<int32*>(base, offset);
		case sizeof(int64):
			return convertValueDirect<int64*>(base, offset);
		case sizeof(int16):
			return convertValueDirect<int16*>(base, offset);
			break;
		}
		break;
	case btUInt:
		switch(len)
		{
		case sizeof(uint32):
			return convertValueDirect<uint32*>(base, offset);
		case sizeof(uint64):
			return convertValueDirect<uint64*>(base, offset);
		case sizeof(uint16):
			return convertValueDirect<uint16*>(base, offset);
		}
		break;
	case btChar:
		return convertValueDirect<char*>(base, offset);
	case btBool:
		return convertValueDirect<bool*>(base, offset);
	case btLong:
		return convertValueDirect<long*>(base, offset);
	case btULong:
		return convertValueDirect<unsigned long*>(base, offset);
	case btWChar:
		return convertValueDirect<wchar_t*>(base, offset);
	case btBCD:
		return "BCD";
	case btCurrency:
		return "Currency";
	case btVariant:
		return "VARIANT";
	case btComplex:
		return "Complex";
	case btBit:
		return "BIT";
	case btBSTR:
		return convertValueDirect<wchar_t**>(base, offset);
	case btHresult:
		return convertValueDirect<HRESULT*>(base, offset);

	default:
		{
			std::stringstream strm;
			strm << type;
			return strm.str();
		}
		break;
	}

	std::stringstream strm;
	strm << type;
	return strm.str();
}

std::string ExceptionReportWin::getBasicTypeName(BasicType type, uint64 len) {
	switch(type)
			{
			case btFloat:
				return "float";
				break;
			case btVoid:
				return "void";
				break;
			case btInt:
				switch(len)
				{
				case sizeof(int32):
					return "int32";
					break;
				case sizeof(int64):
					return "int64";
					break;
				case sizeof(int16):
					return "int16";
					break;
				}
				break;
			case btUInt:
				switch(len)
				{
				case sizeof(uint32):
					return "uint32";
					break;
				case sizeof(uint64):
					return "uint64";
					break;
				case sizeof(uint16):
					return "uint16";
					break;
				}
				break;
			case btChar:
				return "char";
				break;
			case btBool:
				return "bool";
				break;
			case btLong:
				return "int64";
				break;
			case btULong:
				return "uint64";
				break;
			case btWChar:
				return "wchar_t";
			case btBCD:
				return "BCD";
			case btCurrency:
				return "Currency";
			case btVariant:
				return "VARIANT";
			case btComplex:
				return "Complex";
			case btBit:
				return "BIT";
			case btBSTR:
				return "BSTR";
			case btHresult:
				return "HRESULT";

			default:
				{
					std::stringstream strm;
					strm << type;
					return strm.str();
				}
				break;
			}

			std::stringstream strm;
			strm << type;
			return strm.str();
}

std::string ExceptionReportWin::getFrameString(DWORD64 address, std::string& function) {
	DWORD64 displace = 0;
	std::vector<uint8> buffer(sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME * sizeof(wchar_t));
	PIMAGEHLP_SYMBOL64 sym = (PIMAGEHLP_SYMBOL64)buffer.data();
	memset(sym, 0, buffer.size());

	sym->Size = buffer.size();
	sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOLW64);
	sym->MaxNameLength = MAX_SYM_NAME;

	if(SymGetSymFromAddr64(mProcess, address, &displace, sym)) {
		std::stringstream strm;
		strm << sym->Name;
		function = sym->Name;
		
		IMAGEHLP_LINE64 line = { 0 };
		line.SizeOfStruct = sizeof(line);
		DWORD disp = 0;
		if(SymGetLineFromAddr64(mProcess, address, &disp, &line)) {
			LPSTR file = _strdup(line.FileName);
			strm << " @ " << file << " line " << line.LineNumber;
			free(file);
		} else {
			strm << " + " << (address - sym->Address);
		}

		return strm.str();
	}

	std::stringstream strm;
	strm << std::setw(8) << std::setfill('0') << std::hex << address;
	return strm.str();
}

bool ExceptionReportWin::getSourceLine(DWORD64 address, std::string& outLine) {
	DWORD64 displace = 0;
	std::vector<uint8> buffer(sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME * sizeof(wchar_t));
	PIMAGEHLP_SYMBOL64 sym = (PIMAGEHLP_SYMBOL64)buffer.data();
	memset(sym, 0, buffer.size());

	sym->Size = buffer.size();
	sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOLW64);
	sym->MaxNameLength = MAX_SYM_NAME;

	if(SymGetSymFromAddr64(mProcess, address, &displace, sym)) {		
		IMAGEHLP_LINE64 line = { 0 };
		line.SizeOfStruct = sizeof(line);
		DWORD disp = 0;
		if(SymGetLineFromAddr64(mProcess, address, &disp, &line)) {
			std::ifstream ifile(line.FileName, std::ios::in);
			if(ifile.is_open() == false)
				return false;

			uint32 curLine = 0;
			std::string lineText;
			bool lineFound = false;
			std::stringstream lineStream;
			do {
				if(!std::getline(ifile, lineText))
					break;

				++curLine;
				if(curLine == line.LineNumber - 1 || curLine == line.LineNumber + 1) {
					lineFound = true;
					lineStream << "\t" << lineText << std::endl;
				} else if(curLine == line.LineNumber) {
					lineFound = true;
					lineStream << "\t" << lineText << " <----" << std::endl;
				}

			} while(curLine < line.LineNumber + 1);

			if(lineFound)
				outLine = lineStream.str();

			ifile.close();
			return lineFound;
		} else {
			return false;
		}
	}

	return false;
}
#include "framework.h"
#include <dbghelp.h>
#include "dbghelper.h"
#include <tlhelp32.h>
#include "singleton.h"
#include <DbgEng.h>

dbghelper* dbghelper::instance = nullptr;

dbghelper::dbghelper(): m_pSymInitialize(nullptr), m_pSymLoadModule64(nullptr), m_pSymSetOptions(nullptr)
{
	m_dllHandle = LoadLibrary("dbghelp.dll");
}

dbghelper::~dbghelper()
{
	if (m_dllHandle)
	{
		FreeLibrary(m_dllHandle);
	}
}

dbghelper* dbghelper::getInstance()
{
	if (!dbghelper::instance)
	{
		dbghelper::instance = new dbghelper();
	}
	return dbghelper::instance;
}

DWORD WINAPI dbghelper::ThreadFunc(LPVOID p)
{
	DEBUG_EVENT event;
	WaitForDebugEvent(&event, INFINITE);
	return 1;
}

BOOL dbghelper::RegisterCallBack(HANDLE hProcess, ULONG actionCode, PVOID callbackData, PVOID userContext)
{
	char szTemp[500];
	sprintf_s(szTemp, 500, "%X", actionCode);
	OutputDebugString(szTemp);
	return TRUE;
}

void dbghelper::attachProcess(UINT processId)
{
	_SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
	if (hProcess != NULL)
	{
		m_processId = processId;
		if (!_SymInitialize(hProcess, NULL, TRUE))
		{
			// SymInitialize failed
			int error = GetLastError();
			printf("SymInitialize returned error : %d\n", error);
			return;
		}
		BOOL ret = DebugActiveProcess(processId);
		ret = _SymRegisterCallback(hProcess, dbghelper::RegisterCallBack, (PVOID)this);
		ret = getModuleList(hProcess);
		startDebug();
		CloseHandle(hProcess);
	}
	
}

void dbghelper::startDebug()
{
	HANDLE  hThread;
	DWORD  threadId;
	hThread = CreateThread(NULL, 0, dbghelper::ThreadFunc, LPVOID(this), 0, &threadId);
}

BOOL dbghelper::getModuleList(HANDLE hProcess)
{
	HANDLE hMoudleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_processId);
	if (hMoudleSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error.\n");
		return FALSE;
	}
	MODULEENTRY32 module32;
	memset(&module32, 0, sizeof(module32));
	module32.dwSize = sizeof(module32);
	BOOL bM = Module32First(hMoudleSnap, &module32);
	while (bM)
	{
		_SymLoadModule64(hProcess, 0, module32.szExePath, module32.szModule, (DWORD64)module32.modBaseAddr, module32.modBaseSize);
		bM = Module32Next(hMoudleSnap, &module32);
		char szTemp[500] = {0};
		sprintf_s(szTemp, 500, "%s loaded!\n", module32.szModule);
		OutputDebugString(szTemp);
	}
	return TRUE;
}

DWORD dbghelper::_SymSetOptions(DWORD SymOptions)
{
	if (!m_pSymSetOptions)
	{
		m_pSymSetOptions = (SymSetOptionsFunc)GetProcAddress(m_dllHandle, "SymSetOptions");
	}
	return m_pSymSetOptions(SymOptions);
}

BOOL dbghelper::_SymInitialize(HANDLE hProces, PCSTR UserSearchPath, BOOL fInvadeProcess)
{
	if (!m_pSymInitialize)
	{
		m_pSymInitialize = (SymInitializeFunc)GetProcAddress(m_dllHandle, "SymInitialize");
	}
	return m_pSymInitialize(hProces, UserSearchPath, fInvadeProcess);
}

DWORD64 dbghelper::_SymLoadModule64(HANDLE hProcess, HANDLE hFile, PCSTR ImageName, PCSTR ModuleName, DWORD64 BaseOfDll, DWORD SizeOfDll)
{
	if (!m_pSymLoadModule64)
	{
		m_pSymLoadModule64 = (SymLoadModule64Func)GetProcAddress(m_dllHandle, "SymLoadModule64");
	}
	return m_pSymLoadModule64(hProcess, hFile, ImageName, ModuleName, BaseOfDll, SizeOfDll);
}

BOOL dbghelper::_SymRegisterCallback(HANDLE hProcess, PSYMBOL_REGISTERED_CALLBACK CallbackFunction, PVOID UserContext)
{
	if (!m_pSymRegisterCallback) {
		m_pSymRegisterCallback = (SymRegisterCallbackFunc)GetProcAddress(m_dllHandle, "SymRegisterCallback");
	}
	return m_pSymRegisterCallback(hProcess, CallbackFunction, UserContext);
}

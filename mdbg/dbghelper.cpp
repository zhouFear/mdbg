#include "framework.h"
#include <dbghelp.h>
#include "dbghelper.h"
#include <tlhelp32.h>

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
	MODULEENTRY32 moudle32;
	memset(&moudle32, 0, sizeof(moudle32));
	moudle32.dwSize = sizeof(moudle32);
	BOOL bM = Module32First(hMoudleSnap, &moudle32);
	while (bM)
	{
		_SymLoadModule64(hProcess, 0, moudle32.szExePath, );
	}
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

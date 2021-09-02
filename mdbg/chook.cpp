#include "CHook.h"
#include <xstring>

const std::string dllname = "F:\\mycode\\debug\\git\\mdbg\\mdbg\\Debug\\hookd.dll";
CHook::CHook()
{

}

CHook::~CHook()
{

}

bool CHook::hookTheProc(int id)
{
	hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 1, id);
	LPTHREAD_START_ROUTINE  pfun;
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	LPVOID p = VirtualAllocEx(hProcess, NULL, dllname.length(), MEM_COMMIT, PAGE_READWRITE);
	if (!p)
	{
		return false;
	}
	WriteProcessMemory(hProcess, p, dllname.c_str(), dllname.length(), NULL);
	pfun = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));
	HANDLE hRt = CreateRemoteThread(hProcess, NULL, 0, pfun, p, CREATE_SUSPENDED, 0);
	if (hRt == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(hRt);
	CloseHandle(hProcess);
	return true;
}

bool CHook::_ajustTokenPrivileges()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp;
	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		return false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME,&luid))
	{
		return false;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	if (!AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		return false;
	}
	return true;
}

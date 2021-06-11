#pragma once
#include "framework.h"
#include <map>
class dbghelper
{
	typedef DWORD (_stdcall *SymSetOptionsFunc)(_In_ DWORD   SymOptions);
	typedef BOOL(_stdcall *SymInitializeFunc)(
		_In_ HANDLE hProcess,
		_In_opt_ PCSTR UserSearchPath,
		_In_ BOOL fInvadeProcess
		);
	typedef DWORD64 (_stdcall *SymLoadModule64Func)(
			_In_ HANDLE hProcess,
			_In_opt_ HANDLE hFile,
			_In_opt_ PCSTR ImageName,
			_In_opt_ PCSTR ModuleName,
			_In_ DWORD64 BaseOfDll,
			_In_ DWORD SizeOfDll
		);
private:
	dbghelper();
	~dbghelper();
public:
	static dbghelper* getInstance();
	static DWORD WINAPI  ThreadFunc(LPVOID p);

	void attachProcess(UINT processId);
	void startDebug();

	BOOL getModuleList(HANDLE hProcess);
private:
	DWORD _SymSetOptions(DWORD SymOptions);
	BOOL _SymInitialize(HANDLE hProces, PCSTR UserSearchPath, BOOL fInvadeProcess);
	DWORD64 _SymLoadModule64(_In_ HANDLE hProcess,
		_In_opt_ HANDLE hFile,
		_In_opt_ PCSTR ImageName,
		_In_opt_ PCSTR ModuleName,
		_In_ DWORD64 BaseOfDll,
		_In_ DWORD SizeOfDll);
private:
	HMODULE m_dllHandle;
	static dbghelper* instance;
	std::map<DWORD, tstring> m_moduleList;
	UINT m_processId;

	SymSetOptionsFunc m_pSymSetOptions;
	SymInitializeFunc m_pSymInitialize;
	SymLoadModule64Func m_pSymLoadModule64;
};

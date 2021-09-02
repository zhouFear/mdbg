#pragma once
#include <Windows.h>
class CHook
{
public:
	CHook();
	~CHook();

public:
	bool hookTheProc(int id);
	bool _ajustTokenPrivileges();

private:
	HANDLE hProcess;
};


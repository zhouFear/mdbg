#pragma once
#include "framework.h"
class consoleMgr
{
private:
	consoleMgr();
	~consoleMgr() {};
public:
	static consoleMgr* getInstance();
	BOOL createConsoleWindow();
	void write(std::string content);
private:
	HANDLE fpDebugOut;
	HANDLE fpDebugIn;
};


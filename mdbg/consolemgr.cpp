#include "consoleMgr.h"
#include <locale.h>
consoleMgr::consoleMgr():fpDebugIn(NULL),fpDebugOut(NULL)
{

}

consoleMgr* consoleMgr::getInstance()
{
	static consoleMgr inst;
	return &inst;
}

BOOL consoleMgr::createConsoleWindow()
{
	if (!AllocConsole()) MessageBox(NULL, _T("控制台生成失败。"), NULL, 0);
	SetConsoleTitle(_T("Debug Window"));
	fpDebugOut = GetStdHandle(STD_OUTPUT_HANDLE);
	fpDebugIn = GetStdHandle(STD_INPUT_HANDLE);
	// _tfreopen_s(&fpDebugOut, _T("CONOUT$"), _T("w"), stdout);
	// _tfreopen_s(&fpDebugIn, _T("CONIN$"), _T("r"), stdin);
	// _tsetlocale(LC_ALL, _T("chs"));
	return TRUE;
}

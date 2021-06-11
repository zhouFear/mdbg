#pragma once
#include "framework.h"
#include "resource.h"
#include "mprocesshandler.h"

#define MAX_LOADSTRING 100
class application
{
public:
	application(_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);
	~application();

public:
	BOOL run();
private:
	void _init();
	void _createConsoleWindow();
	BOOL _createWnd();
	ATOM _registerClass();
	UINT _winmsgloop();
public:
	static LRESULT CALLBACK _myWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK _ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE m_inst;
	LPWSTR    m_cmdLine;
	HWND      m_hwnd;
	int       m_nCmdShow;
	WCHAR szWindowClass[MAX_LOADSTRING];
	mprocesshandler* m_pProcessHandler;
};


#include "application.h"
#include <stdio.h>
#include <locale.h>

#define SAFE_CLOSE(f) if(f) fclose(f);

application::application(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow): m_inst(hInstance), m_cmdLine(lpCmdLine), m_nCmdShow(nCmdShow),m_hwnd(NULL), m_pProcessHandler(NULL)
{
	LoadStringW(m_inst, IDC_MDBG, szWindowClass, MAX_LOADSTRING);
	m_pProcessHandler = new mprocesshandler();
}

application::~application()
{

}

BOOL application::run()
{
	_init();
	ATOM ret = _registerClass();
	int err = GetLastError();
	if (!_createWnd())
	{
		return FALSE;
	}
	return _winmsgloop();
}

void application::_init()
{
	// 初始化
}

void application::_createConsoleWindow()
{
	FILE* fpDebugOut = NULL;
	FILE* fpDebugIn = NULL;
	if (!AllocConsole()) MessageBox(NULL, _T("控制台生成失败。"), NULL, 0);
	SetConsoleTitle(_T("Debug Window"));
	_tfreopen_s(&fpDebugOut, _T("CONOUT$"), _T("w"), stdout);
	_tfreopen_s(&fpDebugIn, _T("CONIN$"), _T("r"), stdin);
	_tsetlocale(LC_ALL, _T("chs"));

	SAFE_CLOSE(fpDebugOut);
	SAFE_CLOSE(fpDebugIn);
}

BOOL application::_createWnd()
{
	WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
	LoadStringW(m_inst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_inst, this);

	if (!hWnd)
	{
		int err = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, m_nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

ATOM application::_registerClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = application::_myWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_inst;
	wcex.hIcon = LoadIcon(m_inst, MAKEINTRESOURCE(IDI_MDBG));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MDBG);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

UINT application::_winmsgloop()
{
	HACCEL hAccelTable = LoadAccelerators(m_inst, MAKEINTRESOURCE(IDC_MDBG));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK application::_myWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		application* pThis = reinterpret_cast<application*>(lParam);
		SetWindowLongPtr(hWnd, GWL_USERDATA, lParam);
	}
	case WM_COMMAND:
	{
		application* pThis = reinterpret_cast<application*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(pThis->m_inst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, application::About);
			break;
		case IDM_EXIT:

			DestroyWindow(hWnd);
			break;
		case ID_OPENCONSOLE:
			pThis->_createConsoleWindow();
			break;
		case ID_ATTACHPROCESS:
			DialogBoxParam(pThis->m_inst, MAKEINTRESOURCE(IDD_DIALOG_PROCESS), hWnd, mprocesshandler::_ProcessDlgProc, LPARAM(pThis->m_pProcessHandler));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK application::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK application::_ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

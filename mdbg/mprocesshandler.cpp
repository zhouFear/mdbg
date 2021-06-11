#include "mprocesshandler.h"
#include <stdio.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include "resource.h"
#include "dbghelper.h"

mprocesshandler::mprocesshandler()
{

}

mprocesshandler::~mprocesshandler()
{

}

void mprocesshandler::getProcessList(HWND dlgHwnd)
{
	//PROCESSENTRY32结构体，保存进程具体信息
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	//获得系统进程快照的句柄
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot error.\n");
		return;
	}
	BOOL bProcess = Process32First(hProcessSnap, &pe32);
	HWND hwndList = GetDlgItem(dlgHwnd, IDC_LIST_PROCESS);
	while (bProcess)
	{
		ProcessInfo info;
		info.th32ProcessID = pe32.th32ProcessID;
		info.setFileName(pe32.szExeFile);
		char szText[MAX_PATH] = { 0 };
		wsprintf(szText, "%s[%d]", info.fileName.c_str(), info.th32ProcessID);
		int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,
			(LPARAM)szText);
		SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)pe32.th32ProcessID);
		bProcess = Process32Next(hProcessSnap, &pe32);
	}
	CloseHandle(hProcessSnap);
	SetFocus(hwndList);
}


INT_PTR CALLBACK mprocesshandler::_ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		mprocesshandler* pThis = reinterpret_cast<mprocesshandler*>(lParam);
		SetWindowLongPtr(hDlg, GWL_USERDATA, lParam);
		pThis->getProcessList(hDlg);
	}
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		case IDC_LIST_PROCESS:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(hDlg, IDC_LIST_PROCESS);
				// Get selected index.
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				// Get item data.
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);
				/*mprocesshandler* pThis = reinterpret_cast<mprocesshandler*>(GetWindowLongPtr(hDlg, GWL_USERDATA));
				pThis->attachProcess(i);*/
				dbghelper::getInstance()->attachProcess(i);
				return TRUE;
			}
			default:
				break;
			}
		}
		default:
			break;
		}
		
		break;
	}
	return (INT_PTR)FALSE;
}

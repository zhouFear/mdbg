#ifndef mprocesshandler_h__
#define mprocesshandler_h__
#include "framework.h"
#include <list>


class ProcessDlg
{
	typedef struct proceeInfo
	{
		UINT th32ProcessID;
		tstring filePath;
		tstring fileName;

		void setFileName(tstring path) {
			filePath = path;
			tstring::size_type pos = path.find_last_of(L'\\');
			if (pos != tstring::npos)
			{
				fileName = path.substr(pos);
			}
			else {
				fileName = path;
			}
		}
	} ProcessInfo;
public:
	ProcessDlg();
	~ProcessDlg();

public:
	void GetProcessList(HWND dlgHwnd);
	static INT_PTR CALLBACK ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
#endif // mprocesshandler_h__


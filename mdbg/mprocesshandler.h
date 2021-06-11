#ifndef mprocesshandler_h__
#define mprocesshandler_h__
#include "framework.h"
#include <list>

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

class mprocesshandler
{
public:
	mprocesshandler();
	~mprocesshandler();

public:
	void getProcessList(HWND dlgHwnd);
	static INT_PTR CALLBACK _ProcessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
private:
	// std::list<ProcessInfo> m_prcList;
};
#endif // mprocesshandler_h__


﻿// mdbg.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "mdbg.h"
#include "application.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    application app(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return app.run();
}

#include <afxwin.h>

class CMainFrame : public CFrameWnd {
public:
    CMainFrame() {
        Create(NULL, _T("MFC Application Tutorial"));
        WriteToFile(_T("test.txt"), _T("Hello, MFC File Handling!\n"));
        CString content = ReadFromFile(_T("test.txt"));
        MessageBox(content, _T("File Content"), MB_OK);
    }

    void WriteToFile(const CString& fileName, const CString& text) {
        CFile file;
        if (file.Open(fileName, CFile::modeCreate | CFile::modeWrite)) {
            file.Write((LPCTSTR)text, text.GetLength() * sizeof(TCHAR));
            file.Close();
        } else {
            MessageBox(_T("Failed to write to file."), _T("Error"), MB_ICONERROR);
        }
    }

    CString ReadFromFile(const CString& fileName) {
        CFile file;
        CList<CString> ls;
        CString content;
        if (file.Open(fileName, CFile::modeRead)) {
            ULONGLONG fileSize = file.GetLength();
            // 分配一个足够大的缓冲区来存储文件内容
            wchar_t* buffer = content.GetBufferSetLength((int)fileSize);
            file.Read(buffer, (UINT)fileSize * sizeof(wchar_t)); // 确保按 wchar_t 的大小读取
            content.ReleaseBuffer();
            file.Close();
        } else {
            MessageBox(_T("Failed to read from file."), _T("Error"), MB_ICONERROR);
        }
        return content;
    }

};

class CMyApp : public CWinApp {
public:
    BOOL InitInstance() override {
        m_pMainWnd = new CMainFrame();
        m_pMainWnd->ShowWindow(SW_NORMAL);
        m_pMainWnd->UpdateWindow();
        return TRUE;
    }
};

CMyApp theApp;

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//    return theApp.Run();
//}

// 应用程序入口点
//int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
//{
//    // 初始化 MFC 库
//    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
//    {
//        // 初始化失败
//        return FALSE;
//    }
//
//    // 创建应用程序对象
//    CMyApp myApp;
//
//    // 初始化应用程序对象
//    if (!myApp.InitInstance())
//    {
//        // 初始化失败
//        return FALSE;
//    }
//
//    // 运行应用程序消息循环
//    return myApp.Run();
//}

int AFXAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      _In_ LPTSTR lpCmdLine, int nCmdShow)
{
    ASSERT(hPrevInstance == NULL);

    int nReturnCode = -1;
    CWinThread* pThread = AfxGetThread();
    CWinApp* pApp = AfxGetApp();

    // AFX internal initialization
    if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
        goto InitFailure;

    // App global initializations (rare)
    if (pApp != NULL && !pApp->InitApplication())
        goto InitFailure;

    // Perform specific initializations
    if (!pThread->InitInstance())
    {
        if (pThread->m_pMainWnd != NULL)
        {
            TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
            pThread->m_pMainWnd->DestroyWindow();
        }
        nReturnCode = pThread->ExitInstance();
        goto InitFailure;
    }
    nReturnCode = pThread->Run();

    InitFailure:
#ifdef _DEBUG
    // Check for missing AfxLockTempMap calls
    if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
    {
        TRACE(traceAppMsg, 0, "Warning: Temp map lock count non-zero (%ld).\n",
              AfxGetModuleThreadState()->m_nTempMapLock);
    }
    AfxLockTempMaps();
    AfxUnlockTempMaps(-1);
#endif

    AfxWinTerm();
    return nReturnCode;
}
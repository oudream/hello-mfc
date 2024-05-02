#include "mfc.h"


MyApp app;

BOOL MyApp::InitInstance()
{
    MyFrame* frame = new MyFrame;
    frame->ShowWindow(SW_SHOWNORMAL);
    frame->UpdateWindow();

    m_pMainWnd = frame;

    return TRUE;
}

#define WM_UPDATE_MESSAGE_BOX (WM_APP + 1)

BEGIN_MESSAGE_MAP(MyFrame, CFrameWnd)
                    ON_WM_CREATE()
                    ON_BN_CLICKED(1001, &MyFrame::OnBnClickedConnect)
                    ON_BN_CLICKED(1002, &MyFrame::OnBnClickedDisconnect)
                    ON_MESSAGE(WM_UPDATE_MESSAGE_BOX, &MyFrame::OnUpdateMessageBox)
END_MESSAGE_MAP()

LRESULT MyFrame::OnUpdateMessageBox(WPARAM wParam, LPARAM lParam)
{
    CString* pStr = reinterpret_cast<CString*>(lParam);
//    m_messageBox.SetWindowText(*pStr);

    // 确保我们在 UI 线程中操作 UI 控件
    if (m_messageBox.GetSafeHwnd())
    {
        int nLength = m_messageBox.GetWindowTextLength();
        m_messageBox.SetSel(nLength, nLength);  // 将插入点移至文本末尾
        m_messageBox.ReplaceSel(*pStr);           // 插入文本
        m_messageBox.ReplaceSel(_T("\r\n"));    // 添加新行
    }

    delete pStr;  // 释放传递过来的字符串

    return 0;
}

MyFrame* pFrame = nullptr;

MyFrame::MyFrame() {
    pFrame = reinterpret_cast<MyFrame*>(this);
    Create(NULL, TEXT("mfc"));
    m_bConnected = false;
}

int MyFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_ipAddressBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(10, 10, 200, 35), this, 1002);
    m_ipAddressBox.SetWindowTextW(TEXT("127.0.0.1"));
    m_portBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(10, 50, 200, 75), this, 1003);
    m_portBox.SetWindowTextW(TEXT("1234"));
    m_messageBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE, CRect(10, 90, 200, 200), this, 1004);
    m_connectButton.Create(TEXT("Connect"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(210, 10, 310, 35), this, 1001);

    return 0;
}

UINT AFX_CDECL BackgroundThread(LPVOID pParam)
{
    SOCKET sockHandle = (SOCKET)pParam;
    CAsyncSocket socket;
    socket.Attach(sockHandle);

    char buffer[1024];
    int bytesRead;

    while (true)
    {
        int bytesRead = socket.Receive(buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            CString* pStrReceived = new CString(buffer);
            pFrame->PostMessage(WM_UPDATE_MESSAGE_BOX, 0, reinterpret_cast<LPARAM>(pStrReceived));
        }
        else if (bytesRead == SOCKET_ERROR) {
            int nError = socket.GetLastError();
            if (nError == WSAEWOULDBLOCK) {
                // 非阻塞模式下没有数据可读
            } else {
                break;
                // 处理其他错误
            }
        }
    }

    // 断开连接并清理
    socket.Close();
    return 0;
}



void MyFrame::OnBnClickedConnect()
{
    if (m_bConnected) return;

    CString ipAddress, port;
    m_ipAddressBox.GetWindowText(ipAddress);
    m_portBox.GetWindowText(port);
    int portNumber = _ttoi(port);

    if (!AfxSocketInit())
    {
        AfxMessageBox(TEXT("Socket library initialization failed!"));
        return;
    }

    if (!m_clientSocket.Create())
    {
        AfxMessageBox(TEXT("Socket creation failed!"));
        return;
    }

    if (!m_clientSocket.Connect(ipAddress, portNumber))
    {
        m_messageBox.SetWindowText(TEXT("Failed to connect"));
        return;
    }

    m_messageBox.SetWindowText(TEXT("Connected to server"));

    m_bConnected = true;

    // 套接字已连接，现在从这个套接字分离出句柄
    SOCKET sockHandle = m_clientSocket.Detach();
    // 将句柄传递给后台线程处理
    m_pReceiverThread = AfxBeginThread(BackgroundThread, (LPVOID)sockHandle);

}

void MyFrame::OnBnClickedDisconnect()
{
    if (!m_bConnected) return;

    m_clientSocket.Close();
    m_bConnected = false;
    m_messageBox.SetWindowText(TEXT("Disconnected"));
}

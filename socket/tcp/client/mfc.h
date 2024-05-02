#ifndef MFC_DEMO_MFC_H
#define MFC_DEMO_MFC_H

#include <afxwin.h>
#include <winsock2.h>
#include <afxsock.h>  // MFC socket extensions

#pragma comment(lib, "ws2_32.lib")  // Link with WinSock library

class MyApp:public CWinApp {
public:
    virtual BOOL InitInstance();
};

class MyFrame : public CFrameWnd
{
public:
    MyFrame();
    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedDisconnect();

private:
    CEdit m_ipAddressBox;
    CEdit m_portBox;
    CEdit m_messageBox;
    CButton m_connectButton;
    CButton m_disconnectButton;
    CSocket m_clientSocket;
    CWinThread* m_pReceiverThread;
    bool m_bConnected;

DECLARE_MESSAGE_MAP()

    int OnCreate(LPCREATESTRUCT lpCreateStruct);

    LRESULT OnUpdateMessageBox(WPARAM wParam, LPARAM lParam);

};


#endif //MFC_DEMO_MFC_H

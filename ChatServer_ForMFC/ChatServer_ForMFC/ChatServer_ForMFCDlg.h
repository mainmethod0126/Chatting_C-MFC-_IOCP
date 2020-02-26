
// ChatServer_ForMFCDlg.h: 헤더 파일
//

#pragma once

#include <vector>
#include <string>
#include <mswsock.h>
#include "WS2tcpip.h"

#define MAX_CLIENT_COUNT 100

#define ACCEPT_OK 0
#define ACCEPT_FAIL 1


unsigned int __stdcall WorkThread(LPVOID lpParam);
static HANDLE g_hIOCP;
static OVERLAPPED m_readOverlappedStruct;
static SOCKET acceptCandidateSocket;





// CChatServerForMFCDlg 대화 상자
class CChatServerForMFCDlg : public CDialogEx
{


// 생성입니다.
public:
	CChatServerForMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_FORMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


private:
	SOCKET m_hListenSocket;


	wchar_t	m_rgClient_IPs[MAX_CLIENT_COUNT][24];


	std::vector<HANDLE> vecWorkThreadPool;

	/*static unsigned WINAPI WorkThread;*/

	void initialize();

	bool m_isReadOverlapped;

	// AcceptEx 함수 포인터
	LPFN_ACCEPTEX lpAcceptEx = NULL;

	

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListEvent;
	
	bool AcceptOverlapped(SOCKET& acceptCandidateSocket, std::string& errorText);

	bool SystemLog(int dwSystemLog);

};

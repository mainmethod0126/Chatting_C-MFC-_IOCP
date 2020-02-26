
// ChatServer_ForMFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ChatServer_ForMFC.h"
#include "ChatServer_ForMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatServerForMFCDlg 대화 상자



CChatServerForMFCDlg::CChatServerForMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_FORMFC_DIALOG, pParent)
{
	m_hListenSocket = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CChatServerForMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENT, m_ListEvent);
}

BEGIN_MESSAGE_MAP(CChatServerForMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CChatServerForMFCDlg 메시지 처리기

BOOL CChatServerForMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	initialize();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerForMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatServerForMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatServerForMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatServerForMFCDlg::initialize()
{
	WSADATA w;
	WSAStartup(MAKEWORD(2, 2), &w);

	SYSTEM_INFO stSystemInfo;
	GetSystemInfo(&stSystemInfo);

	m_hListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	//m_hListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in srv_Addr;
	srv_Addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &srv_Addr.sin_addr.s_addr);
	srv_Addr.sin_port = htons(20001);
	bind(m_hListenSocket, (LPSOCKADDR)&srv_Addr, sizeof(srv_Addr));
	listen(m_hListenSocket, 5);

	// 생성
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, stSystemInfo.dwNumberOfProcessors);
	// iocp 에 리슨 소켓 등록

	bool bCreateIoCompletionPortResult = CreateIoCompletionPort((HANDLE)m_hListenSocket, g_hIOCP, 0, 0);
	// CPU 갯수를 가져오기 위한 system 변수 생성 및 정보 가져오기

	// CPU 갯수 만큼의 WorkThread 생성
	// dwNumberOfProcessors CPU 갯수 확인
	for (int i = 0; i < stSystemInfo.dwNumberOfProcessors; i++)
	{
		HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, WorkThread, NULL, 0, NULL);
		vecWorkThreadPool.push_back(hThreadHandle);
	}
	
	std::string errorText;

	acceptCandidateSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	// Listen Socket 에서 Client의 비동기 Accpet를 걸어둔다
	AcceptOverlapped(acceptCandidateSocket, errorText);

	m_isReadOverlapped = true;

	//IOCP 입출력 과정에서 송수신 된 데디엍의 크기정보를 저장할 변수
	DWORD readBuf;
	PULONG_PTR lpCompletionKey = nullptr;

}

unsigned int __stdcall WorkThread(LPVOID lpParam)
{
	DWORD dwReadBuf = 0;
	ULONG_PTR lpCompletionKey = 0;


	BOOL r = GetQueuedCompletionStatus(g_hIOCP, &dwReadBuf, &lpCompletionKey, (LPOVERLAPPED*)&m_readOverlappedStruct, INFINITE);
	if (!r)
	{
		TRACE(_T("연결실패"));
	}

	// llisten Socket으로부터의 완료 수신이라면
	if (lpCompletionKey == 0)
	{
		// 수신한 버퍼의 사이즈 0 이면 연결 끊긴거
		if (dwReadBuf == 0)
		{
			// close handle
			closesocket(acceptCandidateSocket);
		}
		else
		{
			// 생성된 remoteClientSocket을 iocp에 등록해준다
			CreateIoCompletionPort((HANDLE)acceptCandidateSocket, g_hIOCP, (ULONG_PTR)&acceptCandidateSocket, 0);
		}
	}
	// Listen Socket이 아닌 remoteSocket
	else
	{
		if (dwReadBuf == 0)
		{

		}
		else
		{
			// 수신 버퍼에서 읽어와야함
			// WSARecive 써야함
			/*WSARecv();*/
			
		}
	}


	return 0;
}


bool CChatServerForMFCDlg::AcceptOverlapped(SOCKET& acceptCandidateSocket, std::string& errorText)
{
	if (lpAcceptEx == NULL)
	{
		DWORD bytes;

		// AcceptEx는 여타 소켓함수와 달리 직접 호출하는 것이 아니고,
		// 함수 포인터를 먼저 가져온 다음 호출할 수 있다. 그것을 여기서 한다.
		WSAIoctl(
			m_hListenSocket,					// Server ListenSocket (소켓을 식별하는 기술자)
			SIO_GET_EXTENSION_FUNCTION_POINTER, // 함수가 수행하는 연산의 제어코드
			&UUID(WSAID_ACCEPTEX),				// 입력 버퍼에 대한 포인터
			sizeof(UUID),						// 입력 버퍼의 크기
			&lpAcceptEx,						// 출력 버퍼에 대한 포인터 
			sizeof(lpAcceptEx),					// 출력 버퍼의 크기
			&bytes,								// 출력시 실제로 받아내는 바이트 수치를 포인트 함
			NULL,								// WSAOVERLAPPED 구조체의 포인터 (비동기 소켓에서는 무시됨)
			NULL								// 연산이 완료 되었을 때 호출될 완료루틴의 포인터 (넌 오버랩 소켓에서는 무시)
		);

		// 위 WSAloctl()함수에서 AcceptEx의 함수 포인터를 가져오는걸 실패했을 경우에 대한 예외처리
		if (lpAcceptEx == NULL)
		{
			//
			TRACE(_T("lpAcceptEx : NULL"));
		}
	}

	// 여기에는 accept된 소켓의 로컬주소와 리모트주소가 채워집니다만 본 예제에서 독자들에게 가르쳐줄 범위를 벗어나므로 그냥 버립니다.
	char ignored[200];
	DWORD ignored2 = 0;

	bool ret = AcceptEx(
		m_hListenSocket,				//ListenSocket
		acceptCandidateSocket,			//ClinetSocket
		&ignored,						//
		0,
		50,
		50,
		&ignored2,
		&m_readOverlappedStruct
	);

	return ret;
}


bool CChatServerForMFCDlg::SystemLog(int dwSystemLog) 
{
	switch (dwSystemLog)
	{
	case ACCEPT_OK:
	{
		m_ListEvent.SetWindowTextW(_T("Client Connectied!"));
		break;
	}

	default:
		break;
	}

	return 0;
}

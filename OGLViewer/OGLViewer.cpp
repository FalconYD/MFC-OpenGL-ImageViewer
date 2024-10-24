// OGLViewer.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "OGLViewer.h"
#define DLLEXPORT
#include "libOGLViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// COGLViewerApp

BEGIN_MESSAGE_MAP(COGLViewerApp, CWinApp)
END_MESSAGE_MAP()


// COGLViewerApp 생성

COGLViewerApp::COGLViewerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	mc_pImageViewer = new ImageViewer();
}

COGLViewerApp::~COGLViewerApp()
{
	if (mc_pImageViewer != nullptr)
	{
		mc_pImageViewer->DestroyWindow();
		delete (ImageViewer*)mc_pImageViewer;
		mc_pImageViewer = nullptr;
	}
}


// 유일한 COGLViewerApp 개체입니다.

COGLViewerApp theApp;


// COGLViewerApp 초기화

BOOL COGLViewerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

DLLTYPE void _libInitCtrl(CWnd* parentWnd, int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	theApp.m_ID = id;
	theApp.m_pParentCwd = parentWnd;
	RECT rect;
	parentWnd->GetDlgItem(id)->GetWindowRect(&rect);
	parentWnd->ScreenToClient(&rect);

	theApp.mc_pImageViewer->Create(WS_CHILD | WS_VISIBLE, rect, parentWnd, id);
}

DLLTYPE void _libOpenImage(const char* strpath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//cv::Mat matTest = cv::imread(strpath);
	theApp.mc_pImageViewer->LoadImg(strpath);
}
//DLLTYPE void _libFinalCtrl()
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	theApp.mc_pImageViewer->DestroyWindow();
//}

DLLTYPE void _libOnSize(UINT Type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (theApp.m_pParentCwd == nullptr) return;
	//RECT rect;
	//theApp.m_pParentCwd->GetDlgItem(theApp.m_ID)->GetWindowRect(&rect);
	//theApp.m_pParentCwd->ScreenToClient(&rect);

	theApp.mc_pImageViewer->OnSize(theApp.m_ID);
	//theApp.mc_pImageViewer->OnSize(Type, rect.right-rect.left, rect.bottom - rect.top);
}

DLLTYPE void _libSetImage(cv::Mat matImg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.mc_pImageViewer->SetImg(matImg);
}

// OGLViewer.h : OGLViewer DLL의 주 헤더 파일
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

// COGLViewerApp
// 이 클래스 구현에 대해서는 OGLViewer.cpp를 참조하세요.
//

struct ST_CONTROLS
{
	int nID;
	CWnd* pWnd;
	ImageViewer* pImageViewer;
};

class COGLViewerApp : public CWinApp
{
public:
	COGLViewerApp();
	~COGLViewerApp();
// 재정의입니다.
public:
	virtual BOOL InitInstance();

	auto InitGLFW() -> void;
	auto InitGLEW() -> void;

	DECLARE_MESSAGE_MAP()

public:
	std::map<int, ST_CONTROLS> m_mapControls;

private:
	std::thread m_threadUpdate;
	bool bWork = false;
};

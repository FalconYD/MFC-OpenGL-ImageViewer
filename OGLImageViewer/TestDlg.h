#pragma once
#include "afxdialogex.h"


// TestDlg 대화 상자

class TestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TestDlg)

public:
	TestDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TestDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};

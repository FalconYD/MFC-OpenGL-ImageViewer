// TestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "OGLImageViewer.h"
#include "afxdialogex.h"
#include "TestDlg.h"


// TestDlg 대화 상자

IMPLEMENT_DYNAMIC(TestDlg, CDialogEx)

TestDlg::TestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

TestDlg::~TestDlg()
{
}

void TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TestDlg, CDialogEx)
END_MESSAGE_MAP()


// TestDlg 메시지 처리기

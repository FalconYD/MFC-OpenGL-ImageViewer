#include "pch.h"
#include "GLFont.h"
#include "GLImage.h"
#include "GLWindow.h"
#include "ImageViewer.h"

ImageViewer* ImageViewer::g_pImgView = nullptr;

BEGIN_MESSAGE_MAP(ImageViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

ImageViewer::ImageViewer()
{
	g_pImgView = this;
	m_bDestroy = false;

	m_pGL = nullptr;
	m_pGLImage = nullptr;
	m_pGLText = nullptr;
	m_pGLShape = nullptr;
}

ImageViewer::~ImageViewer()
{
	g_pImgView = nullptr;
}

BOOL ImageViewer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pParentWnd = pParentWnd;
	BOOL result;
	static CString className = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	m_clientRect = CRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	result = CWnd::CreateEx(/*WS_EX_CLIENTEDGE*/ // strong (default) border
		WS_EX_STATICEDGE,	// lightweight border
		className, NULL, dwStyle,
		m_clientRect.left, m_clientRect.top, m_clientRect.Width(), m_clientRect.Height(),
		pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID);

	m_nID = nID;
	if (result != 0)
	{
		m_pGL = new GLWindow();
		m_pGL->Init(pParentWnd, nID);
		double dHalfWidth = m_pGL->GetClientRect().Width() / 2.f;
		double dHalfHeight = m_pGL->GetClientRect().Height() / 2.f;

		m_pGLImage = new GLImage();
		m_pGLImage->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height());
		m_pGL->Add((GLBase*)m_pGLImage);
		
		m_pGLText = new GLFont();
		m_pGLText->SetFontName("C:\\Windows\\Fonts\\Arial.ttf");
		m_pGLText->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height());
		
		m_pGLText->SetText("POS", "Pos : %.2f, %.2f", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 40), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbPointCb);
		
		m_pGLText->SetText("SCALE", "Scale : %.2lf x", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 20), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, cbScale);
		
		//m_pGLText->SetText("FRAME", "Test2", static_cast<float>( - dHalfWidth + 5 + 100), static_cast<float>( - dHalfHeight + 5), 0.4f, glm::vec3(1.0, 0.0, 1.0));
		//m_pGLText->BindTextFunc("FrameRate : %.2lf", cbFrameRate);
		
		m_pGLText->SetText("WH", "WH : %.2lf, %.2lf", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbWidthHeight);
		
		m_pGL->Add((GLBase*)m_pGLText);

		//m_pGLShape = new GLShape();
		//m_pGLShape->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height());
		//m_pGL->Add((GLBase*)m_pGLShape);
		
		SetTimer(EN_TIMER, 1, nullptr);
	}


	return result;
}

void ImageViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	//dc.FillSolidRect(0, 0, 100, 100, RGB(255, 255, 255));
	//m_pGL->UpdateDraw();
}


void ImageViewer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(EN_TIMER);
	switch (nIDEvent)
	{
	case EN_TIMER:
		if (!m_bDestroy)
		{
			m_pGL->UpdateDraw();
			SetTimer(EN_TIMER, 1, nullptr);
		}
		break;
	}
	//CWnd::OnTimer(nIDEvent);
}

void ImageViewer::LoadImg(std::string strfilename)
{
	m_pGLImage->LoadImg(strfilename);
}

double ImageViewer::cbFrameRate()
{
	return g_pImgView->m_pGLImage->GetFrameRate();
}

cv::Point2d ImageViewer::cbPointCb()
{
	return g_pImgView->m_pGLImage->GetNowPoint();
}

double ImageViewer::cbScale()
{
	return g_pImgView->m_pGLImage->GetScale();
}

cv::Point2d ImageViewer::cbWidthHeight()
{
	return g_pImgView->m_pGLImage->GetImageSize();
}


void ImageViewer::OnDestroy()
{
	CWnd::OnDestroy();
	m_bDestroy = true;
	KillTimer(EN_TIMER);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pGL != nullptr)
	{
		m_pGL->Final();
		delete (GLWindow*)m_pGL;
		m_pGL = nullptr;
	}

	if (m_pGLText != nullptr)
	{
		delete (GLFont*)m_pGLText;
		m_pGLText = nullptr;
	}

	if (m_pGLImage != nullptr)
	{
		delete (GLImage*)m_pGLImage;
		m_pGLImage = nullptr;
	}
}


void ImageViewer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pGL != nullptr)
		m_pGL->OnSize(cx, cy);
}

void ImageViewer::OnSize(int id)
{
	RECT rect;
	m_pParentWnd->GetDlgItem(m_nID)->GetWindowRect(&rect);
	m_pParentWnd->ScreenToClient(&rect);
	this->SetWindowPos(nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
}

void ImageViewer::SetImg(cv::Mat matSrc)
{
	m_pGLImage->SetImage(matSrc);
}

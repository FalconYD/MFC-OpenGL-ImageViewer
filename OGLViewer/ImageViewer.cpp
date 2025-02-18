#include "pch.h"
#include "GLFont.h"
#include "GLImage.h"
#include "GLWindow.h"
#include "ImageViewer.h"

#include "OGLViewer.h"

constexpr int C_ID_BN_OPEN = 2001;
constexpr int C_ID_BN_SAVE = 2002;
constexpr int C_ID_BN_FIT = 2003;
constexpr int C_ID_BN_1X = 2004;
constexpr int C_ID_BN_IN = 2005;
constexpr int C_ID_BN_OUT = 2006;
constexpr int C_ID_BN_RULER = 2007;
constexpr int C_ID_BN_DRAWMODE = 2008;

extern COGLViewerApp theApp;
ImageViewer* ImageViewer::g_pImgView = nullptr;

BEGIN_MESSAGE_MAP(ImageViewer, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(C_ID_BN_OPEN, OnClickOpen)
	ON_BN_CLICKED(C_ID_BN_SAVE, OnClickSave)
	ON_BN_CLICKED(C_ID_BN_FIT, OnClickFit)
	ON_BN_CLICKED(C_ID_BN_1X, OnClick1x)
	ON_BN_CLICKED(C_ID_BN_IN, OnClickIn)
	ON_BN_CLICKED(C_ID_BN_OUT, OnClickOut)
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
	FinalCtrl();
}
auto ImageViewer::SelectGLWindow() -> void
{
	if (m_pGL != nullptr)
	{
		m_pGL->Select();
	}
}

auto ImageViewer::GetGLWindow() -> GLFWwindow*
{
	if (m_pGL == nullptr)
		return nullptr;
	else
		return m_pGL->GetWindowContext();
}

auto ImageViewer::InitCtrl()->void
{
	/** 컨트롤 배치 하부로 이동. Taeroo-kgseon 2024.12.31 17:48:48 */
	CRect rectTemp;
	int nCtrlOffset;
	CRect client;
	client = m_clientRect;
	client.right -= client.left;
	client.bottom -= client.top;
	client.left = 0;
	client.top = 0;

	m_rectInfoView = client;
	m_rectInfoView.top = client.bottom - INFO_HEIGHT;
	m_rectInfoView.bottom = client.bottom;

	m_rectBtnCtrl = client;
	m_rectBtnCtrl.top = m_rectInfoView.top - BTN_HEIGHT;
	m_rectBtnCtrl.bottom = m_rectInfoView.top;

	m_rectImageView = client;
	m_rectImageView.bottom = m_rectBtnCtrl.top;

	//rectTemp
	nCtrlOffset = static_cast<int>(m_rectBtnCtrl.Width() / 6);
	//nCtrlOffset = m_rectBtnCtrl.Width() / 7;
	rectTemp = m_rectBtnCtrl;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bnOpen = new CButton();
	m_bnOpen->Create(_T("Open"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_OPEN);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bnSave = new CButton();
	m_bnSave->Create(_T("Save"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_SAVE);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bn1X = new CButton();
	m_bn1X->Create(_T("1X"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_1X);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bnFit = new CButton();
	m_bnFit->Create(_T("Fit"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_FIT);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bnIn = new CButton();
	m_bnIn->Create(_T("In"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_IN);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_bnOut = new CButton();
	m_bnOut->Create(_T("Out"), WS_CHILD | WS_VISIBLE | SS_CENTER, rectTemp, this, C_ID_BN_OUT);


	nCtrlOffset = m_rectInfoView.Width() / 4;
	rectTemp = m_rectInfoView;
	rectTemp.right = rectTemp.left + nCtrlOffset;
	m_stPos = new CStatic();
	m_stPos->Create(_T("X:0000, Y:0000"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, rectTemp, this);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset + 20;
	m_stColor = new CStatic();
	m_stColor->Create(_T("R:000, G:000, B:000"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, rectTemp, this);

	rectTemp.left = rectTemp.right;
	rectTemp.right = rectTemp.left + nCtrlOffset - 20;
	m_stScale = new CStatic();
	m_stScale->Create(_T("Scale:x1.00"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, rectTemp, this);

	rectTemp.left = rectTemp.right;
	//rectTemp.right = rectTemp.left + nCtrlOffset;
	rectTemp.right = m_rectBtnCtrl.right;
	m_stImgInfo = new CStatic();
	m_stImgInfo->Create(_T("W:0 H:0"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER, rectTemp, this);

}
auto ImageViewer::FinalCtrl()->void
{
	if (m_bnOpen != nullptr)
	{
		m_bnOpen->DestroyWindow();
		delete m_bnOpen;
		m_bnOpen = nullptr;
	}
	if (m_bnSave != nullptr)
	{
		m_bnSave->DestroyWindow();
		delete m_bnSave;
		m_bnSave = nullptr;
	}
	if (m_bn1X != nullptr)
	{
		m_bn1X->DestroyWindow();
		delete m_bn1X;
		m_bn1X = nullptr;
	}
	if (m_bnFit != nullptr)
	{
		m_bnFit->DestroyWindow();
		delete m_bnFit;
		m_bnFit = nullptr;
	}
	if (m_bnIn != nullptr)
	{
		m_bnIn->DestroyWindow();
		delete m_bnIn;
		m_bnIn = nullptr;
	}
	if (m_bnOut != nullptr)
	{
		m_bnOut->DestroyWindow();
		delete m_bnOut;
		m_bnOut = nullptr;
	}

	if (m_stPos != nullptr)
	{
		m_stPos->DestroyWindow();
		delete m_stPos;
		m_stPos = nullptr;
	}
	if (m_stColor != nullptr)
	{
		m_stColor->DestroyWindow();
		delete m_stColor;
		m_stColor = nullptr;
	}
	if (m_stScale != nullptr)
	{
		m_stScale->DestroyWindow();
		delete m_stScale;
		m_stScale = nullptr;
	}
	if (m_stImgInfo != nullptr)
	{
		m_stImgInfo->DestroyWindow();
		delete m_stImgInfo;
		m_stImgInfo = nullptr;
	}
}

BOOL ImageViewer::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pParentWnd = pParentWnd;
	m_nID = nID;
	BOOL result;
	static CString className = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	m_clientRect = rect;

	result = CWnd::CreateEx(/*WS_EX_CLIENTEDGE*/ // strong (default) border
		WS_EX_STATICEDGE,	// lightweight border
		className, NULL, dwStyle,
		m_clientRect.left, m_clientRect.top, m_clientRect.Width(), m_clientRect.Height(),
		pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID);

	if (result != 0)
	{
		InitCtrl();
		m_pGL = new GLWindow();
		m_pGL->Init(pParentWnd, nID, m_rectImageView);

		//double dHalfWidth = m_pGL->GetClientRect().Width() / 2.f;
		//double dHalfHeight = m_pGL->GetClientRect().Height() / 2.f;
		//
		//
		//m_pGLImage = new GLImage();
		//m_pGLImage->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
		//m_pGL->Add((GLBase*)m_pGLImage);


		//m_pGLText = new GLFont();
		//m_pGLText->SetFontName("C:\\Windows\\Fonts\\Arial.ttf");
		//m_pGLText->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
		//
		//m_pGLText->SetText("POS", "Pos : %.2f, %.2f", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 40), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbPointCb);
		//
		//m_pGLText->SetText("SCALE", "Scale : %.2lf x", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 20), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, cbScale);
		//
		//m_pGLText->SetText("WH", "WH : %.2lf, %.2lf", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbWidthHeight);

		//m_pGL->Add((GLBase*)m_pGLText);


		//m_pGLShape = new GLShape();
		//m_pGLShape->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
		//m_pGLShape->AddRectangle(-100, -100, 100, 100, glm::vec3(1,0,0), 1);
		//m_pGLShape->AddCircle(-50, -50, 100, 100, glm::vec3(0,1,0), 1);
		//m_pGL->Add((GLBase*)m_pGLShape);

		//SetTimer(EN_TIMER, 1, nullptr);
	}

	return result;
}


auto ImageViewer::GLLoad() -> void
{
	double dHalfWidth = m_pGL->GetClientRect().Width() / 2.f;
	double dHalfHeight = m_pGL->GetClientRect().Height() / 2.f;

	m_pGL->Select();
	m_pGLImage = new GLImage(m_pGL);
	m_pGLImage->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
	m_pGL->Add((GLBase*)m_pGLImage);

	//m_pGLText = new GLFont();
	//m_pGLText->SetFontName("C:\\Windows\\Fonts\\Arial.ttf");
	//m_pGLText->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
	//
	//m_pGLText->SetText("POS", "Pos : %.2f, %.2f", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 40), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbPointCb);
	//
	//m_pGLText->SetText("SCALE", "Scale : %.2lf x", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5 + 20), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, cbScale);
	//
	//m_pGLText->SetText("WH", "WH : %.2lf, %.2lf", static_cast<float>(-dHalfWidth + 5), static_cast<float>(-dHalfHeight + 5), 0.4f, glm::vec3(1.0, 0.0, 1.0), EN_TEXTSTYLE::EN_BINDING, nullptr, cbWidthHeight);
	//
	//m_pGL->Add((GLBase*)m_pGLText);
	//
	//
	//m_pGLShape = new GLShape();
	//m_pGLShape->Init(m_pGL->GetClientRect().Width(), m_pGL->GetClientRect().Height(), m_nID);
	//m_pGLShape->AddRectangle(-100, -100, 100, 100, glm::vec3(1,0,0), 1);
	//m_pGLShape->AddCircle(-50, -50, 100, 100, glm::vec3(0,1,0), 1);
	//m_pGL->Add((GLBase*)m_pGLShape);

	SetTimer(EN_TIMER, 17, nullptr);
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
	//KillTimer(EN_TIMER);
	switch (nIDEvent)
	{
	case EN_TIMER:
		if (!m_bDestroy)
		{
			m_pGL->UpdateDraw(m_nID);

			if (m_nWidth != m_pGLImage->GetImageWidth() || m_nHeight != m_pGLImage->GetImageHeight())
			{
				m_nWidth = m_pGLImage->GetImageWidth();
				m_nHeight = m_pGLImage->GetImageHeight();
				m_stImgInfo->SetWindowText(std::format(_T("W:{} H:{}"), m_nWidth, m_nHeight).c_str());
			}

			if (m_dScale != m_pGLImage->GetScale())
			{
				m_dScale = m_pGLImage->GetScale();
				m_stScale->SetWindowText(std::format(_T("Scale : {:.2f}x"), m_dScale).c_str());
			}

			if (m_pntCurr != m_pGLImage->GetNowPoint())
			{
				m_pntCurr = m_pGLImage->GetNowPoint();
				m_stPos->SetWindowText(std::format(_T("X: {:.1f}, Y: {:.1f}"), m_pntCurr.x, m_pntCurr.y).c_str());
			}

			if (m_vecRGB != m_pGLImage->GetPixel(static_cast<int>(m_pntCurr.x), static_cast<int>(m_pntCurr.y)))
			{
				m_vecRGB = m_pGLImage->GetPixel(static_cast<int>(m_pntCurr.x), static_cast<int>(m_pntCurr.y));
				m_stColor->SetWindowText(std::format(_T("R: {} G: {} B: {}"), m_vecRGB[2], m_vecRGB[1], m_vecRGB[0]).c_str());
			}
			//glfwPollEvents();
		}
		break;
	}
	//CWnd::OnTimer(nIDEvent);
}

void ImageViewer::UpdateDraw()
{
	m_pGL->UpdateDraw(1000);
}

void ImageViewer::LoadImg(std::string strfilename)
{
	m_pGLImage->LoadImg(strfilename);
	//m_pGLShape->UpdateImageSize(m_pGLImage->GetImageWidth(), m_pGLImage->GetImageHeight());
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

	if (m_pGLShape != nullptr)
	{
		delete (GLShape*)m_pGLShape;
		m_pGLShape = nullptr;
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

void ImageViewer::OnClickOpen()
{
	CFileDialog dlg(TRUE, NULL,NULL, OFN_EXPLORER,_T("Image File|*.bmp;*.jpg;*.jpeg;*.tiff;*.png|"));
	if (dlg.DoModal() == TRUE)
	{
		m_pGLImage->LoadImg(CT2A(dlg.GetPathName()).m_psz);
	}
}
void ImageViewer::OnClickSave()
{
	CFileDialog dlg(FALSE, NULL, NULL, OFN_EXPLORER, _T("Image File|*.bmp;*.jpg;*.jpeg;*.tiff;*.png|"));
	if (dlg.DoModal() == TRUE)
	{
		m_pGLImage->SaveImg(CT2A(dlg.GetPathName()).m_psz);
	}
}
void ImageViewer::OnClick1x()
{
	m_pGLImage->Scale1x();
}
void ImageViewer::OnClickFit()
{
	m_pGLImage->ScaleFit();
}
void ImageViewer::OnClickIn()
{
	m_pGLImage->ScaleZoomIn();
}
void ImageViewer::OnClickOut()
{
	m_pGLImage->ScaleZoomOut();
}
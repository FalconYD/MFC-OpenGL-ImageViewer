#pragma once
#include "GLFont.h"
#include "GLImage.h"
#include "GLShape.h"
#include "GLWindow.h"


class ImageViewer : public CWnd
{
protected:
	DECLARE_MESSAGE_MAP()

	enum {EN_TIMER};

private:
	CWnd* m_pParentWnd;
	int m_nID;
	GLWindow* m_pGL;
	GLImage* m_pGLImage;
	GLFont* m_pGLText;
	GLShape* m_pGLShape;

	CRect m_clientRect;

	const int BTN_HEIGHT = 70;
	const int INFO_HEIGHT = 20;

	CRect m_rectInfoView;
	CRect m_rectBtnCtrl;
	CRect m_rectImageView;

	CButton* m_bnOpen;
	CButton* m_bnSave;
	CButton* m_bn1X;
	CButton* m_bnFit;
	CButton* m_bnIn;
	CButton* m_bnOut;

	CStatic* m_stPos;
	CStatic* m_stColor;
	CStatic* m_stScale;
	CStatic* m_stImgInfo;

	static ImageViewer* g_pImgView;
	bool m_bDestroy;

	int m_nWidth;
	int m_nHeight;
	double m_dScale;
	cv::Point2d m_pntCurr;
	cv::Vec3b m_vecRGB;
public:
	ImageViewer();
	virtual ~ImageViewer();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = NULL);
	auto InitCtrl() -> void;
	auto FinalCtrl() -> void;
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	auto GLLoad() -> void;
	auto SelectGLWindow() -> void;
	auto GetGLWindow() -> GLFWwindow*;

	void LoadImg(std::string strfilename);

	void SetImg(cv::Mat matSrc);

	void OnSize(int id);

	void UpdateDraw();

	static double cbScale();
	static cv::Point2d cbWidthHeight();
	static double cbFrameRate();
	static cv::Point2d cbPointCb();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickOpen();
	afx_msg void OnClickSave();
	afx_msg void OnClick1x();
	afx_msg void OnClickFit();
	afx_msg void OnClickIn();
	afx_msg void OnClickOut();
};
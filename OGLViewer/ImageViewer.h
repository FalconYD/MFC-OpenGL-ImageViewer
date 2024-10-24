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

	static ImageViewer* g_pImgView;
	bool m_bDestroy;
public:
	ImageViewer();
	virtual ~ImageViewer();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = NULL);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void LoadImg(std::string strfilename);

	void SetImg(cv::Mat matSrc);

	void OnSize(int id);

	static double cbScale();
	static cv::Point2d cbWidthHeight();
	static double cbFrameRate();
	static cv::Point2d cbPointCb();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

// MFCOpenGLView.h : CMFCOpenGLView 类的接口
//

#pragma once


class CMFCOpenGLView : public CView
{
protected: // 仅从序列化创建
	CMFCOpenGLView();
	DECLARE_DYNCREATE(CMFCOpenGLView)

// 特性
public:
	CMFCOpenGLDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMFCOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	HGLRC m_hRC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // MFCOpenGLView.cpp 中的调试版本
inline CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLDoc*>(m_pDocument); }
#endif



// MFCOpenGLView.cpp : CMFCOpenGLView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCOpenGL.h"
#endif

#include "MFCOpenGLDoc.h"
#include "MFCOpenGLView.h"

#include "glut.h"
#include "GL/GL.H"
#include "GL/GLU.H"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCOpenGLView

IMPLEMENT_DYNCREATE(CMFCOpenGLView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenGLView, CView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CMFCOpenGLView 构造/析构

CMFCOpenGLView::CMFCOpenGLView()
{
    // TODO: 在此处添加构造代码

}

CMFCOpenGLView::~CMFCOpenGLView()
{
}

BOOL CMFCOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 在此处通过修改
    //  CREATESTRUCT cs 来修改窗口类或样式

    return CView::PreCreateWindow(cs);
}

// CMFCOpenGLView 绘制

void CMFCOpenGLView::OnDraw(CDC* pDC)
{
    CMFCOpenGLDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO: add draw code for native data here
    // ////////////////////////////////////////////////////////////////
    // New codes begin:
    CClientDC dc(this);
    wglMakeCurrent(dc.m_hDC, m_hRC);

    glClearColor(0,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glColor3f(1.0, 0.0, 0.0);

    glutWireTeapot(2.0);

    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
    // New codes end.
    // //////////////////////////////////////////////////////////////// 

}

void CMFCOpenGLView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CMFCOpenGLView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCOpenGLView 诊断

#ifdef _DEBUG
void CMFCOpenGLView::AssertValid() const
{
    CView::AssertValid();
}

void CMFCOpenGLView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const // 非调试版本是内联的
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenGLDoc)));
    return (CMFCOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCOpenGLView 消息处理程序


int CMFCOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // The PIXELFORMATDESCRIPTOR structure describes
    //      the pixel format of a drawing surface.
    PIXELFORMATDESCRIPTOR pfd =
    { 
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
        1,                              // version number 
        PFD_DRAW_TO_WINDOW |    // support window 
        PFD_SUPPORT_OPENGL |    // support OpenGL 
        PFD_DOUBLEBUFFER,   // double buffered
        PFD_TYPE_RGBA,
        24,                     // 24-bit color depth 
        0, 0, 0, 0, 0, 0,      // color bits ignored 
        0,                      // no alpha buffer 
        0,                      // shift bit ignored 
        0,                      // no accumulation buffer 
        0, 0, 0, 0,             // accum bits ignored 
        32,                     // 32-bit z-buffer (depth)
        0,                      // no stencil buffer 
        0,                      // no auxiliary buffer 
        PFD_MAIN_PLANE,        // main layer 
        0,                      // reserved 
        0, 0, 0                 // layer masks ignored 
    }; 
    
    CClientDC dc(this);
    // Get the best available match of pixel format for the device context
    // In other words, if this computer doesn't support features that I
    // asked for, try to get the next best thing.  i.e. 16-bit color mode
    // instead of 24-bit color mode.
    int pixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);

    // Set the pixel format to the best pixel format I can get (see above)
    // and if that operation fails, bring up a message box that tells the
    // user the error.
    if (!SetPixelFormat(dc.m_hDC, pixelFormat, &pfd))
    {
        MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( )"),
            _T("Application Error", MB_ICONERROR));
    }

    // Creates an OpenGL rendering context so that OpenGL knows how to draw
    // to this view. You can't use OpenGL in MFC without using the handle
    // that this function returns
    m_hRC = wglCreateContext(dc.m_hDC);

    // New codes end.
    // ////////////////////////////////////////////////////////////////


    return 0;
}


void CMFCOpenGLView::OnDestroy()
{
    CView::OnDestroy();

    // TODO: Add your message handler code here
    // ////////////////////////////////////////////////////////////////
    // New codes begin:

    // Set : a specified OpenGL rendering context ==> NULL
    // Set : current rendering context ==> NULL
    wglMakeCurrent(NULL, NULL);

    // Delete the handle to an OpenGL rendering context 
    wglDeleteContext(m_hRC);
    // New codes end.
    // //////////////////////////////////////////////////////////////// 

}


void CMFCOpenGLView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    // ////////////////////////////////////////////////////////////////
    // New codes begin:

    CClientDC dc(this);
    wglMakeCurrent(dc.m_hDC, m_hRC);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double d=10;
    double n=100;

    glOrtho(-cx/n, cx/n, -cy/n, cy/n, -d, d);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, cx, cy);
    glShadeModel(GL_FLAT); // Set shading mode
    wglMakeCurrent(NULL, NULL);

    // New codes end.
    // //////////////////////////////////////////////////////////////// 
}

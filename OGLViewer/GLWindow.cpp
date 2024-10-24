#include "pch.h"
#include "GLWindow.h"

GLWindow* GLWindow::gWin = nullptr;

GLWindow::GLWindow()
{
	m_pParent = nullptr;
	m_window = 0;
	gWin = this;
}
GLWindow::~GLWindow()
{
	gWin = nullptr;
}

int GLWindow::Init(CWnd* pParent, UINT nID)
{
	m_pParent = pParent;
	if (!glfwInit())
		return -1;

	//glfwDefaultWindowHints();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_pParent->GetDlgItem(nID)->GetClientRect(&m_Clientrect);
	m_window = glfwCreateWindow(m_Clientrect.Width(), m_Clientrect.Height(), "GLWindow", NULL, NULL);
	glfwSetCursorPosCallback(m_window, &MouseMoveCallBack);
	glfwSetMouseButtonCallback(m_window, &MouseButtonCallBack);
	glfwSetScrollCallback(m_window, &MouseScrollCallBack);
	glfwSetKeyCallback(m_window, &KeyboardCallBack);
	HWND hwNative = glfwGetWin32Window(m_window);

	::SetParent(hwNative, m_pParent->GetDlgItem(nID)->GetSafeHwnd());

	long style = GetWindowLong(hwNative, GWL_STYLE);
	style &= ~WS_POPUP; // remove popup style
	style |= WS_CHILDWINDOW; // add childwindow style
	SetWindowLong(hwNative, GWL_STYLE, style);

	::ShowWindow(hwNative, SW_SHOW);
	//glfwSetWindowUserPointer(window, GetDlgItem(IDC_OPENGL));

	glfwMakeContextCurrent(m_window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	GLenum en = glewInit();
	if (en != GLEW_OK) {
		return -1;
	}
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return 0;
}

int GLWindow::Final()
{
	for (int i = 0; i < base.size(); i++)
	{
		base[i]->Final();
	}
	base.clear();
	glfwDestroyWindow(m_window);
	glfwTerminate();
	
	return 0;
}

void GLWindow::UpdateDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < base.size(); i++)
	{
		base[i]->Draw();
	}

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}


void GLWindow::Add(GLBase* child)
{
	base.push_back(child);
}

void GLWindow::Remove(GLBase* child)
{
	base.clear();
	//if (base.size() > 0)
	//{
	//	//base.erase()
	//}
	//if (base.begin() != base.end())
	//{
	//	base.erase(base.begin());
	//}
}


void GLWindow::MouseButtonCallBack(GLFWwindow* win, int button, int action, int mods)
{
	for (int i = 0; i < gWin->base.size(); i++)
	{
		gWin->base[i]->MouseButton(win, button, action, mods);
	}
	//gWin->UpdateDraw();
}

void GLWindow::MouseMoveCallBack(GLFWwindow* win, double xpos, double ypos)
{
	for (int i = 0; i < gWin->base.size(); i++)
	{
		gWin->base[i]->MouseMove(win, xpos, ypos);
	}
	//gWin->UpdateDraw();
}

void GLWindow::MouseScrollCallBack(GLFWwindow* win, double xpos, double ypos)
{
	for (int i = 0; i < gWin->base.size(); i++)
	{
		gWin->base[i]->MouseScroll(win, xpos, ypos);
	}
	//gWin->UpdateDraw();
}

void GLWindow::KeyboardCallBack(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	for (int i = 0; i < gWin->base.size(); i++)
	{
		gWin->base[i]->KeyboardCallback(win, key, scancode, action, mods);
	}
	//gWin->UpdateDraw();
}

void GLWindow::OnSize(int width, int height)
{
	if (gWin == nullptr) return;

	
	for (int i = 0; i < gWin->base.size(); i++)
	{
		gWin->base[i]->OnSize(width, height);
	}

	glfwSetWindowSize(m_window, width, height);
	glViewport(0, 0, width, height);
}

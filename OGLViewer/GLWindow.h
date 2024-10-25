#pragma once
class GLWindow
{
public:
	GLWindow();
	~GLWindow();
private:
	std::vector<GLBase*> base;
	CWnd* m_pParent;

	GLFWwindow* m_window;
	CRect m_Clientrect;
public:
	int Init(CWnd* pParent, UINT nID);
	int Final();
	void UpdateDraw();
	void Add(GLBase* child);
	void Remove(GLBase* child);

	void OnSize(int width, int height);
	//void RemoveChild(GLBase* child);

	CRect GetClientRect() const { return m_Clientrect; };

private:
	static GLWindow* gWin;
	static void MouseButtonCallBack(GLFWwindow* win, int button, int action, int mods);
	static void MouseMoveCallBack(GLFWwindow* win, double xpos, double ypos);
	static void MouseScrollCallBack(GLFWwindow* win, double xpos, double ypos);
	static void KeyboardCallBack(GLFWwindow* win, int key, int scancode, int action, int mods);
};


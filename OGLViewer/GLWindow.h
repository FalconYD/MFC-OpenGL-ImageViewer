#pragma once
class GLWindow
{
public:
	GLWindow();
	~GLWindow();
private:
	std::vector<GLBase*> base;
	CWnd* m_pParent;

	int m_nID;
	GLFWwindow* m_window;
	CRect m_Clientrect;
public:
	auto GetWindowContext() -> GLFWwindow* { return m_window; };
	int Init(CWnd* pParent, UINT nID, CRect rect);
	int Final();
	void UpdateDraw(int id);
	void Add(GLBase* child);
	void Remove(GLBase* child);
	auto Select() -> void;
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


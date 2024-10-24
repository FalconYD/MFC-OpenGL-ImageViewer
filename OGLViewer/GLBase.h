#pragma once
class GLBase
{
public:
	virtual void Init(int clientwidth, int clientheight) = 0;
	virtual void Final() = 0;

	virtual void Draw() = 0;

	virtual void OnSize(int width, int height) = 0;

	virtual void MouseMove(GLFWwindow* win, double xpos, double ypos) = 0;
	virtual void MouseButton(GLFWwindow* win, int button, int action, int mods) = 0;
	virtual void MouseScroll(GLFWwindow* win, double xpos, double ypos) = 0;
	virtual void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods) = 0;
};
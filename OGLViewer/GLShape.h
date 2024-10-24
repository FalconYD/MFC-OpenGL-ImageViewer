#pragma once
//https://stackoverflow.com/questions/3484260/opengl-line-width
class GLShape : GLBase
{
public:
	GLShape();
	~GLShape();


private:
	int m_clientwidth;
	int m_clientheight;

	GLuint m_programID;

	GLuint m_locmvp;
	GLuint m_locres;
	GLuint m_locthi;

	glm::mat4 m_Projection;
	std::vector<glm::vec4> varray1;
	std::vector<glm::vec4> varray2;

	GLuint ssbo1;
	GLuint vao;
	GLuint ssbo2;

public:
	virtual void Init(int clientwidth, int clientheight);
	virtual void Final();
	virtual void Draw();
	virtual void OnSize(int width, int height);

	virtual void MouseMove(GLFWwindow* win, double xpos, double ypos);
	virtual void MouseButton(GLFWwindow* win, int button, int action, int mods);
	virtual void MouseScroll(GLFWwindow* win, double xpos, double ypos);
	virtual void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
};


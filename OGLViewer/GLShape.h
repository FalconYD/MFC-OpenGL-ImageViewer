#pragma once
//https://stackoverflow.com/questions/3484260/opengl-line-width
enum EN_SHAPEDEF
{
	EN_SHAPE_RECT = 0,
	EN_SHAPE_CIRCLE
};

struct ST_SHAPE
{
	EN_SHAPEDEF shape;
	std::vector<glm::vec4> vertex;
	float thick;
	glm::vec3 color;
	GLuint ssbo;
	bool moveable;
	ST_SHAPE(EN_SHAPEDEF sh, std::vector<glm::vec4> vec, float th, glm::vec3 clr)
	{
		shape = sh;
		vertex = vec;
		thick = th;
		color = clr;
		ssbo = 0;
		moveable = false;
	}
};

class GLShape : GLBase
{
public:
	GLShape();
	~GLShape();


private:
	int m_clientwidth;
	int m_clientheight;

	int m_imagewidth;
	int m_imageheight;

	GLuint m_programID;

	GLuint m_locmvp;
	GLuint m_locres;
	GLuint m_locthi;

	GLuint m_shapecolor;

	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_Model;
	glm::mat4 m_mvp;

	glm::vec3 m_move;
	glm::vec3 m_zoom;
	glm::mat4 m_translate;
	glm::mat4 m_scale;

	glm::vec3 m_prevPoint;
	glm::vec3 m_currPoint;
	glm::vec3 m_movePoint;
	EN_MOUSEMODE m_enMouseMode;

	std::vector<ST_SHAPE> vshapes;

	GLuint vao;

	float getfitscale();
	float getimgposx(float x);
	float getimgposy(float y);
public:
	void UpdateImageSize(int width, int height);
	void AddRectangle(float x, float y, float w, float h, glm::vec3 color, float thick);
	void AddCircle(float x, float y, float elSizeX, float elSizeY, glm::vec3 color, float thick);

	void SetRectangle(float x, float y, float w, float h, glm::vec3 color, float thick);

	virtual void Init(int clientwidth, int clientheight);
	virtual void Final();
	virtual void Draw();
	virtual void OnSize(int width, int height);

	virtual void MouseMove(GLFWwindow* win, double xpos, double ypos);
	virtual void MouseButton(GLFWwindow* win, int button, int action, int mods);
	virtual void MouseScroll(GLFWwindow* win, double xpos, double ypos);
	virtual void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
};


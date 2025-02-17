#pragma once
#include "GLWindow.h"

class GLImage : GLBase
{
public:
	GLImage(GLWindow* parent);
	~GLImage();
private:
	GLWindow* m_parent;
	int m_nID;
	GLuint m_texture;
	GLuint m_textureid;
	GLuint m_vertexbuffer;
	GLuint m_colorbuffer;
	GLuint m_uvbuffer;
	
	GLuint m_programID;
	GLuint m_VertexArrayID;

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

	GLuint m_MatrixID;

	std::vector<glm::vec3> m_vecData2;
	std::vector<glm::vec3> m_vecColor2;
	std::vector<glm::vec2> m_vecUv2;

	int m_clientwidth;
	int m_clientheight;
	EN_MOUSEMODE m_enMouseMode;
	cv::Mat m_matImg;

	double m_dPrevFrameTime;
	double m_dCurrFrameTime;
	int m_nFPSUpdate;
	double m_dFrameRate;


	inline float getfitscale();
	inline float getimgposy(float y);
	inline float getimgposx(float x);
public:
	virtual void Init(int clientwidth, int clientheight, int id);
	virtual void Final();
	virtual void Draw();
	virtual void OnSize(int width, int height);
	double GetFrameRate() const;
	cv::Point2d GetNowPoint();
	cv::Point2d GetImageSize();
	double GetScale();

	void SetImage(cv::Mat matSrc);

	void LoadImg(std::string strfilename);

	int GetImageWidth() const { return m_matImg.cols; }
	int GetImageHeight() const { return m_matImg.rows; }

	virtual void MouseMove(GLFWwindow* win, double xpos, double ypos);
	virtual void MouseButton(GLFWwindow* win, int button, int action, int mods);
	virtual void MouseScroll(GLFWwindow* win, double xpos, double ypos);
	virtual void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
};

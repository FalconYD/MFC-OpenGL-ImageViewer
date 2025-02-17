#include "pch.h"
#include "GLImage.h"

GLImage::GLImage(GLWindow* parent)
{
	m_parent = parent;
	m_zoom = glm::vec3(1.f, 1.f, 1.f);

	m_vecUv2.clear();
	m_vecUv2.push_back(glm::vec2(0, 1));
	m_vecUv2.push_back(glm::vec2(0, 0));
	m_vecUv2.push_back(glm::vec2(1, 1));

	m_vecUv2.push_back(glm::vec2(1, 1));
	m_vecUv2.push_back(glm::vec2(0, 0));
	m_vecUv2.push_back(glm::vec2(1, 0));

	m_vecData2.clear();
	m_vecData2.push_back(glm::vec3(static_cast<float>(-0), static_cast<float>(+0), 0));
	m_vecData2.push_back(glm::vec3(static_cast<float>(-0), static_cast<float>(-0), 0));
	m_vecData2.push_back(glm::vec3(static_cast<float>(+0), static_cast<float>(+0), 0));

	m_vecData2.push_back(glm::vec3(static_cast<float>(+0), static_cast<float>(+0), 0));
	m_vecData2.push_back(glm::vec3(static_cast<float>(-0), static_cast<float>(-0), 0));
	m_vecData2.push_back(glm::vec3(static_cast<float>(+0), static_cast<float>(-0), 0));
}

GLImage::~GLImage()
{
	m_matImg.release();
	m_vecData2.clear();
	m_vecUv2.clear();
}

void GLImage::Init(int clientwidth, int clientheight, int id)
{
	m_nID = id;
	m_clientwidth = clientwidth;
	m_clientheight = clientheight;
	std::string VertexShaderCode =
		R"(
#version 460 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
out vec2 UV;
uniform mat4 MVP;
void main() 
{
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	UV = vertexUV;
}
	)";

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode =
		R"(
#version 460 core
in vec2 UV;
out vec3 color;
uniform sampler2D myTextureSampler;
void main() {
	color = texture(myTextureSampler, UV).rgb;
}
	)";

	//VertexShaderCode = GLUtil::Replace(VertexShaderCode, "MVP", std::format("MVP{}", m_nID));
	//VertexShaderCode = GLUtil::Replace(VertexShaderCode, "UV", std::format("UV{}", m_nID));
	//VertexShaderCode = GLUtil::Replace(VertexShaderCode, "vertexPosition_modelspace", std::format("vertexPosition_modelspace{}", m_nID));
	//VertexShaderCode = GLUtil::Replace(VertexShaderCode, "location = 0)", std::format("location = {})", m_nID - 1000));
	//VertexShaderCode = GLUtil::Replace(VertexShaderCode, "location = 1)", std::format("location = {})", (m_nID - 1000) + 1));
	//
	//FragmentShaderCode = GLUtil::Replace(FragmentShaderCode, "UV", std::format("UV{}", m_nID));
	//FragmentShaderCode = GLUtil::Replace(FragmentShaderCode, "myTextureSampler", std::format("myTextureSampler{}", m_nID));
	
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);

	m_programID = GLUtil::LoadShader(VertexShaderCode, FragmentShaderCode);
	m_MatrixID = glGetUniformLocation(m_programID, "MVP");
	//m_MatrixID = glGetUniformLocation(m_programID, std::format("MVP{}", m_nID).c_str());
	//m_Projection = glm::perspective(glm::radians(45.0f), (float)rect.Width() / (float)rect.Height(), 0.1f, 100000.0f);
	//m_Projection = glm::ortho(0.f, (float)rect.Width(), (float)rect.Height(), 0.f, 0.01f, 100000.f);
	//m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), 0.01f, 100000.f);
	m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), -10.f, 10.f);
	m_View = glm::lookAt(
		//glm::vec3(4, 3, 3),// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 1),// Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0),// and looks at the origin
		glm::vec3(0, 1, 0)// Head is up (set to 0,-1,0 to look upside-down)
	);

	m_Model = glm::mat4(1.0f);
	m_mvp = m_Projection * m_View * m_Model; // Remember, matrix multiplication is the other way around

	//if (m_dib.IsValid())
	//{
	//	glGenTextures(1, &m_texture);
	//
	//	glBindTexture(GL_TEXTURE_2D, m_texture);
	//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_dib.GetWidth(), m_dib.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, m_dib.GetDIBitsAddr());
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_dib.GetWidth(), m_dib.GetHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, m_dib.GetMatAddr());
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	m_textureid = glGetUniformLocation(m_programID, "myTextureSampler");
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
	
	glGenTextures(1, &m_texture);

	m_textureid = glGetUniformLocation(m_programID, "myTextureSampler");
	//m_textureid = glGetUniformLocation(m_programID, std::format("myTextureSampler{}", m_nID).c_str());

	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3(0, 0, 0)) * m_vecData2.size(), m_vecData2.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2(0, 0)) * m_vecUv2.size(), m_vecUv2.data(), GL_STATIC_DRAW);
	auto isprg = glIsProgram(m_programID);
}

void GLImage::Final()
{
	glDeleteTextures(1, &m_texture);
	glDeleteBuffers(1, &m_colorbuffer);
	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteBuffers(1, &m_uvbuffer);
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_VertexArrayID);

	//m_matImg.release();
}

void GLImage::Draw()
{
	//auto context = wglGetCurrentContext();
	auto isprogram = glIsProgram(m_programID);
	if (isprogram == 0)
	{
		Init(m_clientwidth, m_clientheight, m_nID);
		
		OutputDebugStringA("---------------------------------isprogram Created.\n ");
		GLUtil::GetGLError();
	}
	isprogram = glIsProgram(m_programID);
	m_dCurrFrameTime = glfwGetTime();
	m_nFPSUpdate++;
	if (m_dCurrFrameTime - m_dPrevFrameTime > 1.0)
	{
		//	wchar_t buff[MAX_PATH] = { 0, };
		//	//swprintf_s(buff, L"%.2lf fps", 1 / (m_dCurrFrameTime - m_dPrevFrameTime));
		//	swprintf_s(buff, L"%.2lf fps", m_nFPSUpdate / (m_dCurrFrameTime - m_dPrevFrameTime));
		//	SetDlgItemText(IDC_STATIC_FRAMETIME, buff);
		m_dFrameRate = m_nFPSUpdate / (m_dCurrFrameTime - m_dPrevFrameTime);
		m_nFPSUpdate = 0;
		m_dPrevFrameTime = m_dCurrFrameTime;
	}
	//---------------------------------------------------------------------------
	//m_tball->Update();
	//m_View = m_tCam.m_viewMatr;
	m_Model = m_translate * m_scale * glm::mat4(1.0f);
	m_mvp = m_Projection * m_View * m_Model;
	//---------------------------------------------------------------------------
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//m_Text1.RenderText("Test2", 100, 100, 1.0, glm::vec3(1, 0, 0));
	// 
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Use our shader
	glUseProgram(m_programID);
	//glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &m_mvp[0][0]);
	glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, glm::value_ptr(m_mvp));
	
	glBindVertexArray(m_VertexArrayID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(m_textureid, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
	glVertexAttribPointer(
		1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Draw the triangle !
	
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vecData2.size() * 3));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_DEPTH_TEST);
}

void GLImage::OnSize(int width, int height)
{
	m_clientwidth = width;
	m_clientheight = height;

	//m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), 0.01f, 100000.f);
	m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), -10.f, 10.f);
	
	m_mvp = m_Projection * m_View * m_Model; // Remember, matrix multiplication is the other way around
}

void GLImage::MouseMove(GLFWwindow* win, double xpos, double ypos)
{
	m_movePoint = glm::vec3(xpos, ypos, 0);
	switch (m_enMouseMode)
	{
	case EN_MOUSEMODE::EN_PRESS:
	{
		//CString strMsg;
		//strMsg.Format(_T("%.2lf, %.2lf\n"), xpos, ypos);
		//OutputDebugString(strMsg);

		m_currPoint = glm::vec3(xpos, ypos, 0);
		m_move += m_currPoint - m_prevPoint;
		m_prevPoint = m_currPoint;



		m_translate = glm::translate(glm::mat4(), m_move);
	}
	break;
	}
}

void GLImage::MouseButton(GLFWwindow* win, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (button)
		{
		case 0:
		{
			double xpos, ypos;
			glfwGetCursorPos(win, &xpos, &ypos);
			m_prevPoint = glm::vec3(xpos, ypos, 0);
			m_enMouseMode = EN_MOUSEMODE::EN_PRESS;
		}
		break;
		case 1:
			m_move = glm::vec3(0, 0, 0);
			m_translate = glm::translate(glm::mat4(), m_move);

			m_zoom = glm::vec3(1, 1, 1);
			m_scale = glm::scale(m_zoom);
			break;
		case 2:
		{
			m_move = glm::vec3(0, 0, 0);
			m_translate = glm::translate(glm::mat4(), m_move);

			float fitscale = getfitscale();
			m_zoom = glm::vec3(fitscale, fitscale, 1);
			m_scale = glm::scale(m_zoom);
		}
		break;
		}
	}
	if (action == GLFW_RELEASE)
	{
		m_enMouseMode = EN_MOUSEMODE::EN_RELEASE;
	}
}

void GLImage::MouseScroll(GLFWwindow* win, double xpos, double ypos)
{
	if (ypos != 0.0f)
	{
		float prevx = getimgposx(m_movePoint.x);
		float prevy = getimgposy(m_movePoint.y);
		if (ypos > 0)
		{
			m_zoom *= 1 / 0.9;
		}
		else
		{
			m_zoom *= 0.9;
		}
		m_scale = glm::scale(m_zoom);

		float currx = getimgposx(m_movePoint.x);
		float curry = getimgposy(m_movePoint.y);
		m_move.x += (currx - prevx) * m_zoom.x;
		m_move.y += (curry - prevy) * m_zoom.y;
		m_translate = glm::translate(glm::mat4(), m_move);
	}
}

void GLImage::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

	}

	if (action == GLFW_RELEASE) {

	}
}

float GLImage::getimgposx(float x)
{
	return (x - ((m_clientwidth - m_matImg.cols * m_zoom.x) / 2.f) - m_move.x) / m_zoom.x;
	//return x;
}

float GLImage::getimgposy(float y)
{
	return (y - ((m_clientheight - m_matImg.rows * m_zoom.y) / 2.f) - m_move.y) / m_zoom.y;
	//return y;
}

float GLImage::getfitscale()
{
	float xScale = m_clientwidth / (float)m_matImg.cols;
	float yScale = m_clientheight / (float)m_matImg.rows;
	float fitscale = xScale < yScale ? xScale : yScale;
	return fitscale;
}

void GLImage::LoadImg(std::string strfilename)
{
	cv::imread(strfilename, cv::ImreadModes::IMREAD_UNCHANGED).copyTo(m_matImg);

	if (!m_matImg.empty())
	{
		switch (m_matImg.channels())
		{
		case 1:
			cv::cvtColor(m_matImg, m_matImg, cv::ColorConversionCodes::COLOR_GRAY2BGR);
			break;
		case 4:
			cv::cvtColor(m_matImg, m_matImg, cv::ColorConversionCodes::COLOR_BGRA2BGR);
			break;
		}

		//Final();
		double dHalfWidth = m_matImg.cols / 2.0;
		double dHalfHeight = m_matImg.rows / 2.0;

		glfwMakeContextCurrent(m_parent->GetWindowContext());
		m_vecData2[0] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(+dHalfHeight), 0);
		m_vecData2[1] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(-dHalfHeight), 0);
		m_vecData2[2] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(+dHalfHeight), 0);

		m_vecData2[3] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(+dHalfHeight), 0);
		m_vecData2[4] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(-dHalfHeight), 0);
		m_vecData2[5] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(-dHalfHeight), 0);


		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3(0, 0, 0)) * m_vecData2.size(), m_vecData2.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_matImg.cols, m_matImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_matImg.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

double GLImage::GetFrameRate() const
{
	return m_dFrameRate;
}

cv::Point2d GLImage::GetNowPoint()
{
	return cv::Point2d(getimgposx(m_movePoint.x), getimgposy(m_movePoint.y));
}

cv::Point2d GLImage::GetImageSize()
{
	return cv::Point2d(m_matImg.cols, m_matImg.rows);
}

double GLImage::GetScale()
{
	return m_scale[0].x;
}

void GLImage::SetImage(cv::Mat matSrc)
{
	if (matSrc.empty()) return;
	matSrc.copyTo(m_matImg);
	double dHalfWidth = m_matImg.cols / 2.0;
	double dHalfHeight = m_matImg.rows / 2.0;

	m_vecData2[0] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(+dHalfHeight), 0);
	m_vecData2[1] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(-dHalfHeight), 0);
	m_vecData2[2] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(+dHalfHeight), 0);

	m_vecData2[3] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(+dHalfHeight), 0);
	m_vecData2[4] = glm::vec3(static_cast<float>(-dHalfWidth), static_cast<float>(-dHalfHeight), 0);
	m_vecData2[5] = glm::vec3(static_cast<float>(+dHalfWidth), static_cast<float>(-dHalfHeight), 0);
	glfwMakeContextCurrent(m_parent->GetWindowContext());
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3(0, 0, 0)) * m_vecData2.size(), m_vecData2.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_matImg.cols, m_matImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_matImg.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

}

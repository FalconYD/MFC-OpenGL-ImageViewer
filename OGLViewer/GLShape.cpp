#include "pch.h"
#include "GLShape.h"

GLShape::GLShape()
{
    m_zoom = glm::vec3(1.0);
    m_imagewidth = 0;
    m_imageheight = 0;
}

GLShape::~GLShape()
{

}

void GLShape::Init(int clientwidth, int clientheight, int id)
{
	m_clientwidth = clientwidth;
	m_clientheight = clientheight;
    std::string VertexShaderCode = R"(
#version 460

layout(std430, binding = 0) buffer TVertex
{
   vec4 vertex[]; 
};

uniform mat4  u_mvp;
uniform vec2  u_resolution;
uniform float u_thickness;

void main()
{
    int line_i = gl_VertexID / 6;
    int tri_i  = gl_VertexID % 6;

    vec4 va[4];
    for (int i=0; i<4; ++i)
    {
        va[i] = u_mvp * vertex[line_i+i];
        va[i].xyz /= va[i].w;
        va[i].xy = (va[i].xy + 1.0) * 0.5 * u_resolution;
    }

    vec2 v_line  = normalize(va[2].xy - va[1].xy);
    vec2 nv_line = vec2(-v_line.y, v_line.x);

    vec4 pos;
    if (tri_i == 0 || tri_i == 1 || tri_i == 3)
    {
        vec2 v_pred  = normalize(va[1].xy - va[0].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_pred.y, v_pred.x));

        pos = va[1];
        pos.xy += v_miter * u_thickness * (tri_i == 1 ? -0.5 : 0.5) / dot(v_miter, nv_line);
    }
    else
    {
        vec2 v_succ  = normalize(va[3].xy - va[2].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_succ.y, v_succ.x));

        pos = va[2];
        pos.xy += v_miter * u_thickness * (tri_i == 5 ? 0.5 : -0.5) / dot(v_miter, nv_line);
    }

    pos.xy = pos.xy / u_resolution * 2.0 - 1.0;
    pos.xyz *= pos.w;
    gl_Position = pos;
}
)";

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = R"(
#version 330

out vec4 fragColor;
uniform vec3 shapeColor;
void main()
{
    fragColor = vec4(shapeColor, 1.0);
}
)";
    m_programID = GLUtil::LoadShader(VertexShaderCode, FragmentShaderCode);
    m_locmvp = glGetUniformLocation(m_programID, "u_mvp");
    m_locres = glGetUniformLocation(m_programID, "u_resolution");
    m_locthi = glGetUniformLocation(m_programID, "u_thickness");

    m_shapecolor = glGetUniformLocation(m_programID, "shapeColor");

    m_scale = glm::scale(m_zoom);
    m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), -10.f, 100.f);
    m_View = glm::lookAt(
        //glm::vec3(4, 3, 3),// Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 1),// Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0),// and looks at the origin
        glm::vec3(0, 1, 0)// Head is up (set to 0,-1,0 to look upside-down)
    );

    m_Model = glm::mat4(1.0f);
    m_mvp = m_Projection * m_View * m_Model; // Remember, matrix multiplication is the other way around


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLUtil::GetGLError();
}

void GLShape::Final()
{
    for (int i = 0; i < vshapes.size(); i++)
    {
        glDeleteBuffers(1, &vshapes[i].ssbo);
    }
    glDeleteProgram(m_programID);
}

void GLShape::Draw()
{
    m_Model = m_translate * m_scale * glm::mat4(1.0f);
    m_mvp = m_Projection * m_Model;

    glUseProgram(m_programID);
    glUniform2f(m_locres, m_clientwidth, m_clientheight);
    glUniformMatrix4fv(m_locmvp, 1, GL_FALSE, glm::value_ptr(m_mvp));
    glBindVertexArray(vao);

    GLsizei N;
    for (int i = 0; i < vshapes.size(); i++)
    {
        glUniform1f(m_locthi, vshapes[i].thick * m_zoom.x);
        glUniform3f(m_shapecolor, vshapes[i].color.x, vshapes[i].color.y, vshapes[i].color.z);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vshapes[i].ssbo);
        N = (GLsizei)vshapes[i].vertex.size() - 2;
        glDrawArrays(GL_TRIANGLES, 0, 6 * (N - 1));

    }

    glBindVertexArray(0);
}

void GLShape::OnSize(int width, int height)
{
    m_clientwidth = width;
    m_clientheight = height;

    m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), -10.f, 10.f);

    m_mvp = m_Projection * m_View * m_Model; // Remember, matrix multiplication is the other way around
}

void GLShape::MouseMove(GLFWwindow* win, double xpos, double ypos)
{
    m_movePoint = glm::vec3(xpos, ypos, 0);
    switch (m_enMouseMode)
    {
    case EN_MOUSEMODE::EN_PRESS:
    {

        m_currPoint = glm::vec3(xpos, ypos, 0);
        m_move += m_currPoint - m_prevPoint;
        m_prevPoint = m_currPoint;

        m_translate = glm::translate(glm::mat4(), m_move);

        //SetRectangle();
    }
    break;
    }
}

void GLShape::MouseButton(GLFWwindow* win, int button, int action, int mods)
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

void GLShape::MouseScroll(GLFWwindow* win, double xpos, double ypos)
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

        for (int i = 0; i < vshapes.size(); i++)
        {
            vshapes[i].thick = 1.0 * m_zoom.x;
        }
    }
}

void GLShape::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {

    }

    if (action == GLFW_RELEASE) {

    }
}

void GLShape::AddRectangle(float x, float y, float w, float h, glm::vec3 color, float thick)
{
    float l = x;
    float t = y;
    float r = x+w;
    float b = y+h;
    //glm::vec4 p0(-RECTX, -RECTY, 0.0f, 1.0f);
    //glm::vec4 p1(RECTX, -RECTY, 0.0f, 1.0f);
    //glm::vec4 p2(RECTX, RECTY, 0.0f, 1.0f);
    //glm::vec4 p3(-RECTX, RECTY, 0.0f, 1.0f);
    glm::vec4 p0(l, t, -1.0f, 1.0f);
    glm::vec4 p1(r, t, -1.0f, 1.0f);
    glm::vec4 p2(r, b, -1.0f, 1.0f);
    glm::vec4 p3(l, b, -1.0f, 1.0f);
    ST_SHAPE stShape(EN_SHAPEDEF::EN_SHAPE_RECT, { p3, p0, p1, p2, p3, p0, p1 }, thick, color);
    glGenBuffers(1, &stShape.ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, stShape.ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, stShape.vertex.size() * sizeof(*stShape.vertex.data()), stShape.vertex.data(), GL_STATIC_DRAW);

    vshapes.emplace_back(stShape);
}

void GLShape::SetRectangle(float x, float y, float w, float h, glm::vec3 color, float thick)
{
    float l = x;
    float t = y;
    float r = x + w;
    float b = y + h;

    for (int i = 0; i < vshapes.size(); i++)
    {
        switch (vshapes[i].shape)
        {
        case EN_SHAPE_RECT:
            //vshapes[i].moveable = true;
            glm::vec4 p0(l, t, -1.0f, 1.0f);
            glm::vec4 p1(r, t, -1.0f, 1.0f);
            glm::vec4 p2(r, b, -1.0f, 1.0f);
            glm::vec4 p3(l, b, -1.0f, 1.0f);

            vshapes[i].vertex[0] = p3;
            vshapes[i].vertex[1] = p0;
            vshapes[i].vertex[2] = p1;
            vshapes[i].vertex[3] = p2;
            vshapes[i].vertex[4] = p3;
            vshapes[i].vertex[5] = p0;
            vshapes[i].vertex[6] = p1;

            vshapes[i].color = color;
            vshapes[i].thick = thick;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, vshapes[i].ssbo);
            glBufferData(GL_SHADER_STORAGE_BUFFER, vshapes[i].vertex.size() * sizeof(*vshapes[i].vertex.data()), vshapes[i].vertex.data(), GL_STATIC_DRAW);
            break;
        }
    }
}

void GLShape::AddCircle(float x, float y, float elSizeX, float elSizeY, glm::vec3 color, float thick)
{
    std::vector<glm::vec4> vec;
    for (int u = -8; u <= 368; u += 8)
    {
        double a = u * CV_PI / 180.0;
        double c = std::cos(a);
        double s = std::sin(a);
        vec.emplace_back(glm::vec4(elSizeX / 2.f * (float)c + x, elSizeY / 2.f * (float)s + y, -1.0f, 1.0f));
    }
    ST_SHAPE stShape(EN_SHAPEDEF::EN_SHAPE_CIRCLE, vec, thick, color);
    glGenBuffers(1, &stShape.ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, stShape.ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, stShape.vertex.size() * sizeof(*stShape.vertex.data()), stShape.vertex.data(), GL_STATIC_DRAW);

    vshapes.emplace_back(stShape);
}

void GLShape::UpdateImageSize(int width, int height)
{
    m_imagewidth = width;
    m_imageheight = height;
}

float GLShape::getfitscale()
{
    float xScale = m_clientwidth / (float)m_imagewidth;
    float yScale = m_clientheight / (float)m_imageheight;
    float fitscale = xScale < yScale ? xScale : yScale;
    return fitscale;
}

float GLShape::getimgposx(float x)
{
    return (x - ((m_clientwidth - m_imagewidth * m_zoom.x) / 2.f) - m_move.x) / m_zoom.x;
    //return x;
}

float GLShape::getimgposy(float y)
{
    return (y - ((m_clientheight - m_imageheight * m_zoom.y) / 2.f) - m_move.y) / m_zoom.y;
    //return y;
}
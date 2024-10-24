#include "pch.h"
#include "GLShape.h"

GLShape::GLShape()
{

}

GLShape::~GLShape()
{

}

void GLShape::Init(int clientwidth, int clientheight)
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
#version 460

out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0);
}
)";
    m_programID = GLUtil::LoadShader(VertexShaderCode, FragmentShaderCode);
    m_locmvp = glGetUniformLocation(m_programID, "u_mvp");
    m_locres = glGetUniformLocation(m_programID, "u_resolution");
    m_locthi = glGetUniformLocation(m_programID, "u_thickness");

    m_Projection = glm::ortho(-(float)(m_clientwidth / 2.0), (float)(m_clientwidth / 2.0), (float)(m_clientheight / 2.0), -(float)(m_clientheight / 2.0), -10.f, 10.f);
    
    GLushort pattern = 0x18ff;
    GLfloat  factor = 2.0f;

    glm::vec4 p0(-1.0f, -1.0f, 0.0f, 1.0f);
    glm::vec4 p1(1.0f, -1.0f, 0.0f, 1.0f);
    glm::vec4 p2(1.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 p3(-1.0f, 1.0f, 0.0f, 1.0f);
    varray1 = { p3, p0, p1, p2, p3, p0, p1 };
    
    glUseProgram(m_programID);

    glGenBuffers(1, &ssbo1);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo1);
    glBufferData(GL_SHADER_STORAGE_BUFFER, varray1.size() * sizeof(*varray1.data()), varray1.data(), GL_STATIC_DRAW);

    
    //for (int u = -8; u <= 368; u += 8)
    //{
    //    double a = u * CV_PI / 180.0;
    //    double c = std::cos(a), s = std::sin(a);
    //    varray2.emplace_back(glm::vec4((float)c, (float)s, 0.0f, 1.0f));
    //}
    //
    //
    //glGenBuffers(1, &ssbo2);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, varray2.size() * sizeof(*varray2.data()), varray2.data(), GL_STATIC_DRAW);
    
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    GLUtil::GetGLError();
}

void GLShape::Final()
{
}

void GLShape::Draw()
{
    int w, h;
    //glfwGetFramebufferSize(window, &w, &h);
    //if (w != vpSize[0] || h != vpSize[1])
    //{
    //    vpSize[0] = w; vpSize[1] = h;
    //    glViewport(0, 0, vpSize[0], vpSize[1]);
    //    float aspect = (float)w / (float)h;
    //    project = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);
    //    glUniform2f(loc_res, (float)w, (float)h);
    //}
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_programID);
    glUniform1f(m_locthi, 20.0);
    glBindVertexArray(vao);
    glm::mat4 modelview1(1.0f);
    modelview1 = glm::translate(modelview1, glm::vec3(-0.6f, 0.0f, 0.0f));
    modelview1 = glm::scale(modelview1, glm::vec3(0.5f, 0.5f, 1.0f));
    glm::mat4 mvp1 = m_Projection * modelview1;
    
    //glUniformMatrix4fv(m_locmvp, 1, GL_FALSE, glm::value_ptr(mvp1));
    glUniformMatrix4fv(m_locmvp, 1, GL_FALSE, &mvp1[0][0]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo1);
    GLsizei N1 = (GLsizei)varray1.size() - 2;
    glDrawArrays(GL_TRIANGLES, 0, 6 * (N1 - 1));
    //glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(varray1.size() * 3));
    
    
    //glm::mat4 modelview2(1.0f);
    //modelview2 = glm::translate(modelview2, glm::vec3(0.6f, 0.0f, 0.0f));
    //modelview2 = glm::scale(modelview2, glm::vec3(0.5f, 0.5f, 1.0f));
    //glm::mat4 mvp2 = m_Projection * modelview2;
    //
    //glUniformMatrix4fv(m_locmvp, 1, GL_FALSE, &mvp2[0][0]);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo2);
    //GLsizei N2 = (GLsizei)varray2.size() - 2;
    //glDrawArrays(GL_TRIANGLES, 0, 6 * (N2 - 1));
}

void GLShape::OnSize(int width, int height)
{
    m_clientwidth = width;
    m_clientheight = height;
}

void GLShape::MouseMove(GLFWwindow* win, double xpos, double ypos)
{
}

void GLShape::MouseButton(GLFWwindow* win, int button, int action, int mods)
{
}

void GLShape::MouseScroll(GLFWwindow* win, double xpos, double ypos)
{
}

void GLShape::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
}

#include "pch.h"
#include "GLBase.h"
#include "GLFont.h"


GLFont::GLFont()
{
}

GLFont::~GLFont()
{
    Final();
}

void GLFont::SetFontName(std::string fontname)
{
    this->fontname = fontname;
}

void GLFont::Init(int clientwidth, int clientHeight)
{
    this->clientwidth = clientwidth;
    this->clientheight = clientHeight;
    CreateFontFT(fontname, clientwidth, clientheight);
}

void GLFont::Final()
{
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);

    vecTextList.clear();
}

void GLFont::SetText(std::string key, std::string text, float x, float y, float scale, glm::vec3 color, EN_TEXTSTYLE enStyle, cbBindDouble cbdb, cbBindPoint cbpnt)
{
    
    if (mapText.find(key) == mapText.end())
    {
        mapText.insert(std::make_pair(key, vecTextList.size()));
        vecTextList.push_back(std::make_tuple(text, x, y, scale, color, enStyle, cbdb, cbpnt));
    }
    else
    {
        std::get<0>(vecTextList[mapText[key]]) = text;
        std::get<1>(vecTextList[mapText[key]]) = x;
        std::get<2>(vecTextList[mapText[key]]) = y;
        std::get<3>(vecTextList[mapText[key]]) = scale;
        std::get<4>(vecTextList[mapText[key]]) = color;
        std::get<5>(vecTextList[mapText[key]]) = enStyle;
        std::get<6>(vecTextList[mapText[key]]) = cbdb;
        std::get<7>(vecTextList[mapText[key]]) = cbpnt;
    }
}

void GLFont::Draw()
{
    for (int i = 0; i < vecTextList.size(); i++)
    {
        switch (std::get<5>(vecTextList[i]))
        {
        case EN_TEXTSTYLE::EN_STATICTEXT:
            RenderText(std::get<0>(vecTextList[i]), std::get<1>(vecTextList[i]), std::get<2>(vecTextList[i]), std::get<3>(vecTextList[i]), std::get<4>(vecTextList[i]));
            break;
        case EN_TEXTSTYLE::EN_BINDING:
        {
            if (std::get<6>(vecTextList[i]) != nullptr)
            {
                double dData = std::get<6>(vecTextList[i])();
                RenderText(GLUtil::CFormat(std::get<0>(vecTextList[i]), dData), std::get<1>(vecTextList[i]), std::get<2>(vecTextList[i]), std::get<3>(vecTextList[i]), std::get<4>(vecTextList[i]));
            }
            if (std::get<7>(vecTextList[i]) != nullptr)
            {
                cv::Point2d pntData = std::get<7>(vecTextList[i])();
                RenderText(GLUtil::CFormat(std::get<0>(vecTextList[i]), pntData.x, pntData.y), std::get<1>(vecTextList[i]), std::get<2>(vecTextList[i]), std::get<3>(vecTextList[i]), std::get<4>(vecTextList[i]));
            }
        }
            break;
        }
    }
}

void GLFont::OnSize(int width, int height)
{
    this->clientwidth = width;
    this->clientheight = height;
    projection = glm::ortho(-static_cast<float>(width / 2.0), static_cast<float>(width / 2.0), -static_cast<float>(height / 2.0), static_cast<float>(height / 2.0), -10.f, 10.f);
}

void GLFont::MouseMove(GLFWwindow* win, double xpos, double ypos)
{

}

void GLFont::MouseButton(GLFWwindow* win, int button, int action, int mods)
{
}

void GLFont::MouseScroll(GLFWwindow* win, double xpos, double ypos)
{
}

void GLFont::KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
}

int GLFont::CreateFontFT(std::string font_name, int width, int height)
{
    std::string VertexShaderCode =
        "\
#version 330 core\n\
layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n\
out vec2 TexCoords;\n\
\n\
uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n\
    TexCoords = vertex.zw;\n\
}\n\
	";

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode =
        "\
#version 330 core\n\
in vec2 TexCoords;\n\
out vec4 color;\n\
\n\
uniform sampler2D text;\n\
uniform vec3 textColor;\n\
\n\
void main()\n\
{    \n\
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n\
    color = vec4(textColor, 1.0) * sampled;\n\
}  \n\
	";

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //return 0;
    Shader = GLUtil::LoadShader(VertexShaderCode, FragmentShaderCode);
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");
    //projection = glm::ortho(-static_cast<float>(width / 2.0), static_cast<float>(width / 2.0), -static_cast<float>(height / 2.0), static_cast<float>(height / 2.0), 0.001f, 100000.f);
    projection = glm::ortho(-static_cast<float>(width / 2.0), static_cast<float>(width / 2.0), -static_cast<float>(height / 2.0), static_cast<float>(height / 2.0), -10.f, 10.f);
    Matrix = glGetUniformLocation(Shader, "projection");
    TextColor = glGetUniformLocation(Shader, "textColor");
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");
    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        TRACE("ERROR::FREETYPE: Could not init FreeType Library");
        return -1;
    }

    FT_Face face;

    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        TRACE("ERROR::FREETYPE: Failed to load font");
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                TRACE("ERROR::FREETYTPE: Failed to load Glyph");
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    return 0;
}

void GLFont::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    //s.Use();
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glUseProgram(Shader);

    glUniformMatrix4fv(Matrix, 1, GL_FALSE, &projection[0][0]);
    glUniform3f(TextColor, color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");
    // iterate through all characters
    //float minx = x + clientwidth, miny = y + clientheight;
    float maxx = x, maxy = y;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //minx = std::min(minx, xpos + w);
        maxx = std::max(maxx, xpos + w);
        maxy = std::max(maxy, ypos + h);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableVertexAttribArray(0);
    //OutputDebugStringA((char*)glewGetErrorString(glGetError())); OutputDebugStringA("\n");

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void GLFont::BindTextFunc(std::string key, std::string strFormat)
{
    if (mapText.find(key) != mapText.end())
    {
        std::get<0>(vecTextList[mapText[key]]) = strFormat;
    }
}
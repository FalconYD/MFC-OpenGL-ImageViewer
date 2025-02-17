#pragma once
#include "pch.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

typedef double (*cbBindDouble)();
typedef cv::Point2d (*cbBindPoint)();


enum class EN_TEXTSTYLE { EN_STATICTEXT, EN_BINDING };
class GLFont : GLBase
{
public:
    GLFont();
    ~GLFont();

private:
    GLuint Shader;
    GLuint VAO;
    GLuint VBO;
    GLuint Matrix;
    GLuint TextColor;
    glm::mat4 projection;
    std::map<GLchar, Character> Characters;
    std::string fontname;
    int clientwidth;
    int clientheight;

    int CreateFontFT(std::string font_name, int width, int height);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

    std::string BindFormat;

    std::map<std::string, int> mapText;
    // text, x, y, scale, color, style, cbfunc, cbfunc2
    std::vector<std::tuple<std::string, float, float, float, glm::vec3, EN_TEXTSTYLE, cbBindDouble, cbBindPoint>> vecTextList;
public:
    void BindTextFunc(std::string key, std::string strFormat);
    void SetFontName(std::string fontname);
    void SetText(std::string key, std::string text, float x, float y, float scale, glm::vec3 color, EN_TEXTSTYLE enStyle = EN_TEXTSTYLE::EN_STATICTEXT, cbBindDouble cbdb = nullptr, cbBindPoint cbpnt = nullptr);

    virtual void Init(int clientwidth, int clientheight, int id);
    virtual void Final();
    virtual void Draw();
    virtual void OnSize(int width, int height);
    virtual void MouseMove(GLFWwindow* win, double xpos, double ypos);
    virtual void MouseButton(GLFWwindow* win, int button, int action, int mods);
    virtual void MouseScroll(GLFWwindow* win, double xpos, double ypos);
    virtual void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
};

#pragma once
#include "pch.h"

enum class EN_MOUSEMODE {
    EN_NONE = 0,
    EN_PRESS,
    EN_RELEASE
};

class GLUtil
{
public:
	static GLuint LoadShader(std::string vertexcode, std::string fragmentcode)
	{
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        GLint Result = GL_FALSE;
        int InfoLogLength;


        char const* VertexSourcePointer = vertexcode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            TRACE("%s\n", &VertexShaderErrorMessage[0]);
        }

        char const* FragmentSourcePointer = fragmentcode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            TRACE("%s\n", &FragmentShaderErrorMessage[0]);
        }



        // Link the program
        TRACE("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            TRACE("%s\n", &ProgramErrorMessage[0]);
        }

        glDetachShader(ProgramID, VertexShaderID);
        glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        GLUtil::GetGLError();
        //Shader = ProgramID;
        return ProgramID;
	};

    template<typename ... Args>
    static std::string CFormat(const std::string& format, Args... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
        if (size_s <= 0) { throw std::runtime_error("Format error"); }
        auto size = static_cast<size_t>(size_s);
        //std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args ...);
        //std::string strTest;
        //strTest.reserve(size);
        //std::snprintf(strTest.data(), size, format.c_str(), args ...);
        return std::string(buf.get(), buf.get()+size-1);
        //return std::string(buf.get());
    }

    //static std::string string_format(const std::string fmt, ...) {
    //    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    //    std::string str;
    //    va_list ap;
    //    while (1) {     // Maximum two passes on a POSIX system...
    //        str.resize(size);
    //        va_start(ap, fmt);
    //        int n = vsnprintf((char*)str.data(), size, fmt.c_str(), ap);
    //        va_end(ap);
    //        if (n > -1 && n < size) {  // Everything worked
    //            str.resize(n);
    //            return str;
    //        }
    //        if (n > -1)  // Needed size returned
    //            size = n + 1;   // For null char
    //        else
    //            size *= 2;      // Guess at a larger size (OS specific)
    //    }
    //    return str;
    //}

    static std::string Replace(std::string& src, std::string org, std::string replace)
    {
        int idx = src.find(org);
        std::string strRtn;
        strRtn = src;
        while (0 <= idx)
        {
            strRtn = src.replace(idx, org.length(), replace);
            idx = strRtn.find(org, idx+ replace.length());
        }
        return strRtn;
    }

    static std::string GetGLError()
    {
        auto err = glGetError();
        std::string strOut((char*)glewGetErrorString(err));
        OutputDebugStringA(std::format("[{}] {}\n", err, strOut).c_str());
        return strOut;
        //return "";
    }
};
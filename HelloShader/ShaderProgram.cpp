#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

    ShaderProgram::ShaderProgram():mHandle(0)
    {

    }
        
    
    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(mHandle);
    }

    bool ShaderProgram::loadShader(const char* vsFilename, const char * fsFilename)
    {
        std::string vsString = fileToString(vsFilename);
        std::string fsString = fileToString(fsFilename);

        const GLchar* vsSourcePtr = vsString.c_str();
        const GLchar* fsSourcePtr = fsString.c_str();


        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        

        glShaderSource(vs, 1, &vsSourcePtr, NULL);
        glShaderSource(fs,1,&fsSourcePtr,NULL);

        glCompileShader(vs);
        checkCompileErrors(vs, VERTEX);

        glCompileShader(fs);
        checkCompileErrors(vs, FRAGMENT);

        mHandle = glCreateProgram();
        glAttachShader(mHandle, vs);
        glAttachShader(mHandle, fs);
        glLinkProgram(mHandle);

        checkCompileErrors(mHandle, PROGRAM);

        glDeleteShader(vs);
        glDeleteShader(fs);
        return true;
    }
        
    
    void ShaderProgram::use()
    {
        if(mHandle > 0)
        {
            glUseProgram(mHandle);
        }
    }

    std::string ShaderProgram::fileToString(const std::string& filename)
    {
        std::stringstream ss;
        std::ifstream file;

        try
        {
            file.open(filename, std::ios::in);

            if(!file.fail())
            {
                ss << file.rdbuf();
            }
            file.close();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        return ss.str();
    }
    
    
    void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
    {
        GLint result;
        GLint length = 0;
         
        if(type == PROGRAM)
        {
            glGetProgramiv(mHandle,GL_LINK_STATUS, &result);
            if(result == GL_FALSE)
            {
                glGetProgramiv(mHandle,GL_INFO_LOG_LENGTH,&length);
                std::string errorLog(length, ' ');
                glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
                std::cerr << "Error! Program failed to link. " << errorLog <<'/n';
            }
        }
        else
        {
            glGetShaderiv(shader,GL_COMPILE_STATUS, &result);
            if(result == GL_FALSE)
            {
                glGetProgramiv(shader,GL_INFO_LOG_LENGTH,&length);
                std::string errorLog(length, ' ');
                glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
                std::cerr << "Error! Shader failed to compile. " << errorLog <<'/n';
            }
        }
    }
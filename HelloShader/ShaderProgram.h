#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <GL/glew.h>


class ShaderProgram
{
    public:
        ShaderProgram();
        ~ShaderProgram();

        enum ShaderType
        {
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

        bool loadShader(const char* vsFilename, const char * fsFilename);
        void use();
    private:

        std::string fileToString(const std::string& filename);
        void checkCompileErrors(GLuint shader, ShaderType type);
        
        GLuint mHandle;

};

#endif
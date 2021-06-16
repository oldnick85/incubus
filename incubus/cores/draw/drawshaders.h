#ifndef DRAWSHADERS_H
#define DRAWSHADERS_H

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#define GLFW_STATIC
#include <GLFW/glfw3.h>

class CDrawShaders
{
public:
    CDrawShaders()
    {

    }

    ~CDrawShaders()
    {
        if (m_shader_program != -1)
            glDeleteProgram(m_shader_program);
    }

    void SetShaders(const GLchar* vertex_shader_source, const GLchar* fragment_shader_source)
    {
        m_vertex_shader_source = vertex_shader_source;
        m_fragment_shader_source = fragment_shader_source;
    }

    GLuint GetShadersProgram()
    {
        if (m_shader_program == -1)
            CreateShader();
        return m_shader_program;
    }

    void BeginShadersProgram()
    {
        if (m_shader_program == -1)
            CreateShader();
        glUseProgram(m_shader_program);
    }

    void EndShadersProgram()
    {
        glUseProgram(0);
    }

private:
    void CreateShader()
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &m_vertex_shader_source, NULL);
        glCompileShader(vertexShader);
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &m_fragment_shader_source, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        m_shader_program = glCreateProgram();
        glAttachShader(m_shader_program, vertexShader);
        glAttachShader(m_shader_program, fragmentShader);
        glLinkProgram(m_shader_program);
        glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
        if (not success)
        {
            glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        assert(m_shader_program != -1);
    }

    const GLchar* m_vertex_shader_source;
    const GLchar* m_fragment_shader_source;

    GLuint m_shader_program = -1;
};

#endif // DRAWSHADERS_H

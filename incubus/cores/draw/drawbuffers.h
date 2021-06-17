#ifndef DRAWBUFFERS_H
#define DRAWBUFFERS_H

#include <list>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#define GLFW_STATIC
#include <GLFW/glfw3.h>

class CDrawBuffers
{
public:

    struct sAttribPointer
    {
        sAttribPointer(const uint idx, const uint sz, const uint strd, const uint pnt)
            : index(idx), size(sz), stride(strd), pointer(pnt) {}

        uint index = 0;
        uint size = 3;
        uint stride = 3;
        uint pointer = 0;
    };

    using AttribPointers_t = std::list<sAttribPointer>;

    CDrawBuffers()
    {
        m_attrib_pointers.push_back(sAttribPointer(0, 3, 3, 0));
    }

    CDrawBuffers(const AttribPointers_t &ap)
        : m_attrib_pointers(ap)
    {

    }

    ~CDrawBuffers()
    {
        DeleteBuffers();
    }

    void DeleteBuffers()
    {
        if (VAO != -1)
        {
            glDeleteVertexArrays(1, &VAO);
            VAO = -1;
        }
        if (VBO != -1)
        {
            glDeleteBuffers(1, &VBO);
            VBO = -1;
        }
        if (EBO != -1)
        {
            glDeleteBuffers(1, &EBO);
            EBO = -1;
        }
    }

    void MakeBuffers(GLfloat* vertices_data, size_t size)
    {
        DeleteBuffers();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), vertices_data, GL_STATIC_DRAW);
        for (auto &ap : m_attrib_pointers)
        {
            glVertexAttribPointer(ap.index, ap.size, GL_FLOAT, GL_FALSE, ap.stride * sizeof(GLfloat), (GLvoid*)(ap.pointer * sizeof(GLfloat)));
            glEnableVertexAttribArray(ap.index);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return;
    }

    void BeginVertexBuffer()
    {
        glBindVertexArray(VAO);
    }

    void EndVertexBuffer()
    {
        glBindVertexArray(0);
    }

private:
    GLuint VBO = -1;
    GLuint VAO = -1;
    GLuint EBO = -1;

    AttribPointers_t m_attrib_pointers;
};

#endif // DRAWBUFFERS_H

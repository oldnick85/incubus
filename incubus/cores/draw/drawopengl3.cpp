#include "drawopengl3.h"

bool CDrawOpenGL3::Init()
{
    m_cur_transform = glm::mat4(1.0);
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a GLFWwindow object that we can use for GLFW's functions
    m_window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK) {printf("Init FAILED\n"); return false;}

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    //glEnable(GL_DEPTH_TEST); // enable depth-testing
    //glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    {
        // Build and compile our shader program
        // Vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // Check for compile time errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        // Fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // Check for compile time errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        // Link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // Check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (not success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    {
        GLuint simple_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(simple_vertex_shader, 1, &m_simple_vertex_shader_source, NULL);
        glCompileShader(simple_vertex_shader);
        // Check for compile time errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(simple_vertex_shader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(simple_vertex_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        // Fragment shader
        GLuint simple_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(simple_fragment_shader, 1, &m_simple_fragment_shader_source, NULL);
        glCompileShader(simple_fragment_shader);
        // Check for compile time errors
        glGetShaderiv(simple_fragment_shader, GL_COMPILE_STATUS, &success);
        if (not success)
        {
            glGetShaderInfoLog(simple_fragment_shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        // Link shaders
        m_simple_shader_program = glCreateProgram();
        glAttachShader(m_simple_shader_program, simple_vertex_shader);
        glAttachShader(m_simple_shader_program, simple_fragment_shader);
        glLinkProgram(m_simple_shader_program);
        // Check for linking errors
        glGetProgramiv(m_simple_shader_program, GL_LINK_STATUS, &success);
        if (not success)
        {
            glGetProgramInfoLog(m_simple_shader_program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            assert(false);
        }
        glDeleteShader(simple_vertex_shader);
        glDeleteShader(simple_fragment_shader);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    LoadTextures();

    glfwMakeContextCurrent(nullptr);

    return true;
}

bool CDrawOpenGL3::ShouldCloseWindow()
{
    return glfwWindowShouldClose(m_window);
}

void CDrawOpenGL3::Clear()
{
    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return;
    // Draw our first triangle
    glUseProgram(shaderProgram);

    GLfloat timeValue = glfwGetTime();
    GLfloat color_val = (sin(timeValue/1.0) / 2) + 0.5;
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, color_val, color_val, color_val, 1.0f);

    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    // Get matrix's uniform location and set matrix
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CDrawOpenGL3::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

void CDrawOpenGL3::Release()
{
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
}

void CDrawOpenGL3::ContextHold()
{
    glfwMakeContextCurrent(m_window);
}

void CDrawOpenGL3::ContextRelease()
{
    glfwMakeContextCurrent(nullptr);
}

void CDrawOpenGL3::GetCursorPos(double &x, double &y)
{
    glfwGetCursorPos(m_window, &x, &y);
}

int CDrawOpenGL3::GetMouseButton(int button)
{
    return glfwGetMouseButton(m_window, button);
}

int CDrawOpenGL3::GetKey(int key)
{
    return glfwGetKey(m_window, key);
}

void CDrawOpenGL3::DrawPrimitives2(const GLenum type, const std::vector<std::array<Real_t, 2>> &points)
{
    std::vector<GLfloat> vertices;
    vertices.reserve(points.size()*3);
    for (const auto &p : points)
    {
        vertices.push_back(static_cast<GLfloat>(p[0].Val()));
        vertices.push_back(static_cast<GLfloat>(p[1].Val()));
        vertices.push_back(0.0f);
    }

    glUseProgram(m_simple_shader_program);

    ApplyCurTransform();

    GLuint simple_VAO;
    GLuint simple_VBO;

    glGenVertexArrays(1, &simple_VAO);
    glGenBuffers(1, &simple_VBO);
    glBindVertexArray(simple_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, simple_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(simple_VAO);
    glDrawArrays(type, 0, points.size());
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &simple_VAO);
    glDeleteBuffers(1, &simple_VBO);
}

void CDrawOpenGL3::DrawPoints2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_POINTS, points);}
void CDrawOpenGL3::DrawTriangles2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_TRIANGLES, points);}
void CDrawOpenGL3::DrawTriangleFan2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_TRIANGLE_FAN, points);}
void CDrawOpenGL3::DrawLines2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_LINES, points);}
void CDrawOpenGL3::DrawLineStrip2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_LINE_STRIP, points);}
void CDrawOpenGL3::DrawLineLoop2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_LINE_LOOP, points);}
void CDrawOpenGL3::DrawPolygon2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_POLYGON, points);}
void CDrawOpenGL3::DrawQuads2(const std::vector<std::array<Real_t, 2>> &points) {DrawPrimitives2(GL_QUADS, points);}

void CDrawOpenGL3::PushMatrix()
{
    m_transform_stack.push(m_cur_transform);
}

void CDrawOpenGL3::PopMatrix()
{
    m_cur_transform = m_transform_stack.top();
    m_transform_stack.pop();
}

void CDrawOpenGL3::Translate(const Real_t x, const Real_t y, const Real_t z)
{
    m_cur_transform = glm::translate(m_cur_transform, glm::vec3(x.Val(), y.Val(), z.Val()));
}

void CDrawOpenGL3::ApplyCurTransform()
{
    GLint transformLoc = glGetUniformLocation(m_simple_shader_program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_cur_transform));
}

void CDrawOpenGL3::Scale(const Real_t x, const Real_t y, const Real_t z)
{
    m_cur_transform = glm::scale(m_cur_transform, glm::vec3(x.Val(), y.Val(), z.Val()));
}

uint CDrawOpenGL3::GenLists(const uint size)
{
    return glGenLists(size);
}

void CDrawOpenGL3::NewList(const uint list)
{
    glNewList(list, GL_COMPILE);
}

void CDrawOpenGL3::EndList()
{
    glEndList();
}

void CDrawOpenGL3::CallList(const uint list)
{
    glCallList(list);
}

void CDrawOpenGL3::PointSize(const Real_t sz)
{
    glPointSize(sz.Val());
}

void CDrawOpenGL3::LineWidth(const Real_t w)
{
    glLineWidth(w.Val());
}

void CDrawOpenGL3::EnablePointSmooth()
{
    glEnable(GL_POINT_SMOOTH);
}

void CDrawOpenGL3::EnableLineSmooth()
{
    glEnable(GL_LINE_SMOOTH);
}

void CDrawOpenGL3::Color3(const Real_t r, const Real_t g, const Real_t b)
{
    m_cur_color = {r.Val(), g.Val(), b.Val(), 1.0f};
    GLint vertexColorLocation = glGetUniformLocation(m_simple_shader_program, "ourColor");
    glUniform4f(vertexColorLocation, r.Val(), g.Val(), b.Val(), 1.0f);
}

void CDrawOpenGL3::Color4(const Real_t r, const Real_t g, const Real_t b, const Real_t a)
{
    m_cur_color = {r.Val(), g.Val(), b.Val(), a.Val()};
    GLint vertexColorLocation = glGetUniformLocation(m_simple_shader_program, "ourColor");
    glUniform4f(vertexColorLocation, r.Val(), g.Val(), b.Val(), a.Val());
}

void CDrawOpenGL3::RasterPos2(const Real_t x, const Real_t y)
{
    glRasterPos2d(x.Val(), y.Val());
}

void CDrawOpenGL3::PixelZoom(const Real_t x, const Real_t y)
{
    glPixelZoom(x.Val(), y.Val());
}

void CDrawOpenGL3::DrawPixels(uint width, uint height, const void *pixels )
{
    GLuint texid;

    glGenTextures( 1, &texid );
    glBindTexture( GL_TEXTURE_2D, texid );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); // set 1-byte alignment
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glActiveTexture( GL_TEXTURE0 );

    const float verts[4][2] =
    {
        {-1.0f, -1.0f},
        { 1.0f, -1.0f},
        {-1.0f,  1.0f},
        { 1.0f,  1.0f}
    };

    const float texCoord[4][2] =
    {
        { 0.0f,  0.0f},
        { 1.0f,  0.0f},
        { 0.0f,  1.0f},
        { 1.0f,  1.0f}
    };

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glVertexPointer( 2, GL_FLOAT, sizeof( float ) * 2, verts );
    glTexCoordPointer( 2, GL_FLOAT, sizeof( float ) * 2, texCoord );

    glEnable( GL_TEXTURE_2D );

    glViewport(0, 0, width, height);

    glPushMatrix();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    glPopMatrix();

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    glDeleteTextures( 1, &texid );
}

glm::mat4 CDrawOpenGL3::GetCurrentTransform() const
{
    return m_cur_transform;
}

glm::vec4 CDrawOpenGL3::GetCurrentColor() const
{
    return m_cur_color;
}

uint CDrawOpenGL3::GetFontTexture() const
{
    return GetTexture(txFont);
}

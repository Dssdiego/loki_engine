//
// Created by Diego Santos Seabra on 27/06/21.
//

#if UNIX || __APPLE__
#define GL_GLEXT_PROTOTYPES
#endif

#include "shader.h"
#include "../logging/log.h"

Shader::Shader() { }

Shader::Shader(std::string vertexShaderName, std::string fragmentShaderName)
{

    // Control variables
    GLint isCompiled = 0;
    GLint isLinked = 0;

    //////////////
    /// Vertex ///
    //////////////

    // Vertex source code
    std::fstream vertexSrcFile("assets/shaders/" + vertexShaderName);
    std::string vertexSrc( (std::istreambuf_iterator<char>(vertexSrcFile) ),
                           (std::istreambuf_iterator<char>()    ) );

    // Create empty vertex shader handle
    mVertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send vertex shader source code to GL
    const GLchar* vertexGlSrc = vertexSrc.c_str();
    glShaderSource(mVertexShader, 1, &vertexGlSrc, nullptr);

    // Compile vertex shader
    glCompileShader(mVertexShader);

    // Check for compilation errors
    glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &isCompiled);

    // If the vertex shader didn't compile...
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(mVertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(mVertexShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(mVertexShader);

        Log::error("Error compiling vertex shader");

        return;
    }

    ////////////////
    /// Fragment ///
    ////////////////

    // Read fragment GLSL file
    std::fstream fragmentSrcFile("assets/shaders/" + fragmentShaderName);
    std::string fragmentSrc( (std::istreambuf_iterator<char>(fragmentSrcFile) ),
                             (std::istreambuf_iterator<char>()    ) );

    // Create empty fragment shader handle
    mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send fragment shader source code to GL
    const GLchar* fragmentGlSrc = fragmentSrc.c_str();
    glShaderSource(mFragmentShader, 1, &fragmentGlSrc, nullptr);

    // Compile fragment shader
    glCompileShader(mFragmentShader);

    // Check for errors on fragment shader compilation
    glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &isCompiled);

    // If the fragment shader didn't compile...
    if(isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(mFragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(mFragmentShader, maxLength, &maxLength, &infoLog[0]);

        std::cout << "error: " << &infoLog[0] << std::endl;

        // Don't leak shaders
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);

        Log::error("Error compiling fragment shader");

        return;
    }

    // If the code executed till this point,
    // it means both our shaders compiled successfully

    // Creating a program shader object
    mProgram = glCreateProgram();

    // Attach both our shaders to the program
    glAttachShader(mProgram, mVertexShader);
    glAttachShader(mProgram, mFragmentShader);

    Log::info("Linking vertex and fragment shaders to program");

    // Link our program
    glLinkProgram(mProgram);

    // Check for errors when linking the shader
    glGetProgramiv(mProgram, GL_LINK_STATUS, (int *) &isLinked);

    // If the shader link wasn't successful
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(mProgram, maxLength, &maxLength, &infoLog[0]);

        std::cout << "Shader link error: " << &infoLog[0] << std::endl;

        // We don't need our program anymore
        glDeleteProgram(mProgram);

        // Don't leak shaders...
        glDeleteShader(mVertexShader);
        glDeleteShader(mFragmentShader);

        Log::error("Error linking shader program");

        return;
    }

    // Dettach our shaders (we don't need them anymore)
    glDetachShader(mProgram, mVertexShader);
    glDetachShader(mProgram, mFragmentShader);
}

void Shader::use()
{
    // Use our shader
    glUseProgram(mProgram);
}

void Shader::cleanUp()
{
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragmentShader);
    glDeleteProgram(mProgram);
}

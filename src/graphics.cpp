#include "graphics.hpp"

Graphics::Graphics(SDL_Window* window, WinSize& winSize)
    : window{window},
      context{},
      uniforms{0, 0, 0, 0},
      shader{},
      camera{winSize},
      textures{}
{}

// Close things safely, including SDL windows
// (which is currently the job of the Game class
Graphics::~Graphics()
{
    glDeleteProgram(shader);
    
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);
}


void Graphics::loadShaders(const std::string& vertShaderFilename,
                           const std::string& fragShaderFilename)
{
    std::string line;
    std::string tmpStr;
    const char* tmp;
    int success;
    char info[512] = {};
    GLuint vertShader{};
    GLuint fragShader{};
    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;
    std::stringstream vertData;
    std::stringstream fragData;

    // Read shader files
    vertShaderFile.open(vertShaderFilename);
    fragShaderFile.open(fragShaderFilename);

    // Stream data
    while (getline(vertShaderFile, line))
        vertData << line << '\n';
    
    while (getline(fragShaderFile, line))
        fragData << line << '\n';

    // Create/Source/Compile vertex shader
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    tmpStr = vertData.str();
    tmp = tmpStr.c_str();
    glShaderSource(vertShader, 1, &tmp, NULL);    
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

    // Check shader file
    if (!success)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, info);
        std::cerr << "Error compiling vertex shader: " << info << std::endl;
    }

    // Create/Source/Compile frag shader
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    tmpStr = fragData.str();
    tmp = tmpStr.c_str();
    glShaderSource(fragShader, 1, &tmp, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    // Check shader file
    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, info);
        std::cerr << "Error compiling fragment shader: " << info << std::endl;
    }

    // Link shaders
    shader = glCreateProgram();
    glAttachShader(shader, vertShader);
    glAttachShader(shader, fragShader);
    glBindFragDataLocation(shader, 0, "outputColor");
    glLinkProgram(shader);

    // Check if linking was successful
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, info);
        std::cerr << "Error linking shader: " << info << std::endl;
    }

    postShader();
    
    // We're done with the files here
    vertShaderFile.close();
    fragShaderFile.close();
    
    // Free shaders since we're done with them now
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void Graphics::setupUniforms()
{
    uniforms.model = glGetUniformLocation(shader, "model");
    uniforms.proj = glGetUniformLocation(shader, "projection");
    uniforms.view = glGetUniformLocation(shader, "view");
    uniforms.tex = glGetUniformLocation(shader, "texPos");
}

void Graphics::postShader()
{    
    // Use it!
    useShader();
    setupUniforms();
}

#include "engine.hpp"
#include <SDL3/SDL.h>
#include <fstream>
#include <glad/glad.h>
#include <map>
#include <string>

using namespace std;

std::map<SDL_Keycode, int> match{
    { SDLK_w, 1 }, { SDLK_a, 2 }, { SDLK_s, 3 }, { SDLK_d, 4 }
};

void check_gl_errors()
{
    GLenum num_of_error = glGetError();
    if (num_of_error != GL_NO_ERROR)
    {
        switch (num_of_error)
        {
            case GL_INVALID_ENUM:
                cerr << "Error: GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                cerr << "Error: GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                cerr << "Error: GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                cerr << "Error: GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                cerr << "Error: GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_OVERFLOW:
                cerr << "Error: GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                cerr << "Error: GL_STACK_UNDERFLOW";
                break;
        }
    }
}

void* load_func(const char* name)
{
    SDL_FunctionPointer func_ptr = SDL_GL_GetProcAddress(name);
    return reinterpret_cast<void*>(func_ptr);
}

string get_source(const char* filename)
{
    string   source;
    ifstream input_shader_src(filename);
    if (input_shader_src.good())
    {
        input_shader_src.seekg(0, ios::end);
        input_shader_src.seekg(0, ios::beg);
        source.assign(istreambuf_iterator<char>(input_shader_src),
                      istreambuf_iterator<char>());
        return source;
    }
    else
    {
        cerr << "Something wrong with this file " << filename << endl;
        return "";
    }
}

GLuint load_shader(GLenum shader_type, const char* file)
{
    string      src    = get_source(file);
    const char* source = src.c_str();

    GLuint shader = glCreateShader(shader_type);
    GLint  compiled;
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infolen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
        if (infolen > 1)
        {
            char* error = new char[infolen];
            glGetShaderInfoLog(shader, infolen, NULL, error);
            std::cerr << error << std::endl;
            delete[] error;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

SDL_Window*   window         = nullptr;
SDL_GLContext opengl_context = nullptr;
GLuint        vertex_shader;
GLuint        fragment_shader;
GLuint        program;

static bool is_exist     = false;
int         choose_color = -1;

void Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw runtime_error("Engine could not initialize!");
    }

    window = SDL_CreateWindow("Homework-05-02", 800, 800, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        throw runtime_error("Engine could not create a window!");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    opengl_context = SDL_GL_CreateContext(window);

    if (opengl_context == nullptr)
    {
        throw runtime_error("Engine could not create OpenGL ES context!");
    }

    if (!gladLoadGLES2Loader(load_func))
    {
        throw runtime_error("Engine could not initialize OpenGL ES functions!");
    }
    set_program();
}

bool Engine::read_events()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if (match.contains(Event.key.keysym.sym))
        {
            if (Event.type == SDL_EVENT_KEY_DOWN)
            {
                choose_color = match[Event.key.keysym.sym];
            }
        }
    }
    return true;
}

void Engine::clean()
{
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::set_program()
{
    GLint linked;
    vertex_shader   = load_shader(GL_VERTEX_SHADER, "vertex_shader.txt");
    fragment_shader = load_shader(GL_FRAGMENT_SHADER, "fragment_shader.txt");
    program         = glCreateProgram();
    glAttachShader(program, vertex_shader);

    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infolen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infolen);
        if (infolen > 1)
        {
            char* error = new char[infolen];
            glGetProgramInfoLog(program, infolen, NULL, error);
            std::cerr << error << std::endl;
            delete[] error;
        }
        glDeleteProgram(program);
    }
    else
    {
        glUseProgram(program);
    }
}

void Engine::set_triangle_indexed(vector<float>        Vertex,
                                  vector<unsigned int> indexes,
                                  vector<float>        color)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, Vertex.data());
    check_gl_errors();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, color.data());
    check_gl_errors();
    glEnableVertexAttribArray(0);
    check_gl_errors();
    glEnableVertexAttribArray(1);
    check_gl_errors();
}

void Engine::update(vector<float>        Vertex,
                    vector<unsigned int> indexes,
                    vector<float>        color)
{
    glClear(GL_COLOR_BUFFER_BIT);
    check_gl_errors();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, Vertex.data());
    check_gl_errors();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, color.data());
    check_gl_errors();
    glDrawElements(
        GL_TRIANGLES, indexes.size(), GL_UNSIGNED_INT, indexes.data());
    check_gl_errors();
    SDL_GL_SwapWindow(window);
}

Engine* create_engine()
{
    if (is_exist)
    {
        throw runtime_error("Engine exists!");
    }
    is_exist = true;
    return new Engine();
}

void delete_engine(Engine* engine)
{
    if (is_exist)
    {
        if (engine == nullptr)
        {
            throw runtime_error("Engine is nullptr");
        }
        else
        {
            is_exist = false;
            delete engine;
        }
    }
    else
    {
        throw runtime_error("Engine does not exist!");
    }
}

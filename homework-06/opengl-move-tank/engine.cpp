#include "engine.hpp"
#include "picopng.hpp"
#include <SDL3/SDL.h>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <map>
#include <string>

using namespace std;

void callback_opengl_debug(GLenum                       source,
                           GLenum                       type,
                           GLuint                       id,
                           GLenum                       severity,
                           GLsizei                      length,
                           const GLchar*                message,
                           [[maybe_unused]] const void* userParam);

map<SDL_Keycode, int> match{
    { SDLK_w, 1 }, { SDLK_a, 1 }, { SDLK_s, -1 }, { SDLK_d, -1 }
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

static SDL_Window*   window         = nullptr;
static SDL_GLContext opengl_context = nullptr;
static GLuint        vertex_shader;
static GLuint        fragment_shader;

static GLuint program;

static GLuint texture_handle;
static GLuint texture_handle2;

static bool          is_exist     = false;
static int           choose_color = -1;
static vector<float> my_matrix;

void Engine::init(float w, float h)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw runtime_error("Engine could not initialize!");
    }

    window = SDL_CreateWindow("Homework-06-03", w, h, SDL_WINDOW_OPENGL);

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
    program = set_program("./vertex_shader.txt", "./fragment_shader.txt");

    glUseProgram(program);
    check_gl_errors();
    glEnable(GL_DEBUG_OUTPUT);
    check_gl_errors();
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    check_gl_errors();
    glDebugMessageCallback(callback_opengl_debug, nullptr);
    check_gl_errors();
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    check_gl_errors();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    texture_handle  = set_texture("./MyTank.png");
    texture_handle2 = set_texture("./TankS.png");
}

vector<int> keys_pressed = { 0, 0, 0, 0 };
bool        Engine::read_events()
{
    SDL_Event Event;
    keys_pressed[2] = 0;
    keys_pressed[3] = 0;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if (Event.type == SDL_EVENT_KEY_DOWN)
        {
            if (Event.key.keysym.sym == SDLK_w)
            {
                keys_pressed[0] = 1;
            }
            else if (Event.key.keysym.sym == SDLK_s)
            {
                keys_pressed[1] = 1;
            }
        }
        else if (Event.type == SDL_EVENT_KEY_UP)
        {
            if (Event.key.keysym.sym == SDLK_w)
            {
                keys_pressed[0] = 0;
            }
            else if (Event.key.keysym.sym == SDLK_s)
            {
                keys_pressed[1] = 0;
            }
        }
        if (Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            keys_pressed[2] = 1;
        }
        if (Event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            keys_pressed[3] = Event.wheel.y;
        }
    }
    return true;
}
vector<int> Engine::get_events_buffer()
{
    return keys_pressed;
}

void Engine::clean()
{
    glDeleteProgram(program);
    glDeleteTextures(1, &texture_handle);
    glDeleteTextures(1, &texture_handle2);
    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

unsigned int Engine::set_program(const char* vertex_shader,
                                 const char* fragment_shader)
{
    GLint  linked;
    GLint  vertex_s   = load_shader(GL_VERTEX_SHADER, vertex_shader);
    GLint  fragment_s = load_shader(GL_FRAGMENT_SHADER, fragment_shader);
    GLuint program    = glCreateProgram();

    glAttachShader(program, vertex_s);
    glAttachShader(program, fragment_s);
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
            cerr << error << endl;
            delete[] error;
        }
        glDeleteProgram(program);
        return 0;
    }
    glDeleteShader(vertex_s);
    glDeleteShader(fragment_s);
    return program;
}

void Engine::render_triangle(vector<float> atribute_vertex,
                             vector<float> matrix)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex.data());
    check_gl_errors();
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex.data() + 3);
    check_gl_errors();
    glEnableVertexAttribArray(0);
    check_gl_errors();
    glEnableVertexAttribArray(1);
    check_gl_errors();

    glUniformMatrix3fv(
        glGetUniformLocation(program, "matrix"), 1, GL_TRUE, matrix.data());

    glDrawArrays(GL_TRIANGLE_FAN, 0, atribute_vertex.size() / 6);
    check_gl_errors();
}

void Engine::render_triangle(vector<float> atribute_vertex, bool flag)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data());
    check_gl_errors();
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data() + 3);
    check_gl_errors();
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data() + 6);
    check_gl_errors();
    glEnableVertexAttribArray(0);
    check_gl_errors();
    glEnableVertexAttribArray(1);
    check_gl_errors();
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    check_gl_errors();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    check_gl_errors();
}

void Engine::render_triangle(vector<float> atribute_vertex,
                             vector<float> matrix,
                             bool          flag)
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data());
    check_gl_errors();
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data() + 3);
    check_gl_errors();
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 32, atribute_vertex.data() + 6);
    check_gl_errors();
    glEnableVertexAttribArray(0);
    check_gl_errors();
    glEnableVertexAttribArray(1);
    check_gl_errors();
    glEnableVertexAttribArray(2);

    glUniformMatrix3fv(
        glGetUniformLocation(program, "matrix"), 1, GL_TRUE, matrix.data());

    glActiveTexture(GL_TEXTURE0);

    if (!flag)
    {
        glBindTexture(GL_TEXTURE_2D, texture_handle);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture_handle2);
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Engine::morphing(vector<float> atribute_vertex1,
                      vector<float> atribute_vertex2,
                      vector<float> atribute_vertex3)
{
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex1.data());
    check_gl_errors();
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex1.data() + 3);

    glVertexAttribPointer(
        2, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex2.data());
    check_gl_errors();
    glVertexAttribPointer(
        3, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex2.data() + 3);
    check_gl_errors();

    glVertexAttribPointer(
        4, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex3.data());
    check_gl_errors();
    glVertexAttribPointer(
        5, 3, GL_FLOAT, GL_FALSE, 24, atribute_vertex3.data() + 3);
    check_gl_errors();

    // glUniform1f(glGetUniformLocation(program, "theta"), theta);

    glEnableVertexAttribArray(0);
    check_gl_errors();
    glEnableVertexAttribArray(1);
    check_gl_errors();
    glEnableVertexAttribArray(2);
    check_gl_errors();
    glEnableVertexAttribArray(3);
    check_gl_errors();
    glEnableVertexAttribArray(4);
    check_gl_errors();
    glEnableVertexAttribArray(5);
    check_gl_errors();
    glDrawArrays(GL_TRIANGLE_FAN, 0, atribute_vertex1.size() / 6);
    check_gl_errors();
}

unsigned int Engine::set_texture(const char* path)
{
    vector<byte> info;
    ifstream     png_file(path, ios_base::binary);
    GLuint       texture_handle;
    if (png_file.good())
    {
        png_file.seekg(0, ios_base::end);
        int end_file = png_file.tellg();
        png_file.seekg(0, ios_base::beg);
        info.resize(end_file);
        png_file.read(reinterpret_cast<char*>(info.data()),
                      static_cast<streamsize>(info.size()));
    }
    else
    {
        cerr << "Something wrong with this file: " << path << endl;
        return false;
    }

    vector<byte>  image_pixels;
    unsigned long w = 0, h = 0;

    if (decodePNG(image_pixels, w, h, info.data(), info.size(), false))
    {
        cerr << "ERROR IN DECODING PNG FILE!!" << endl;
        return false;
    }

    glGenTextures(1, &texture_handle);
    check_gl_errors();
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    check_gl_errors();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 w,
                 h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image_pixels.data());
    check_gl_errors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    check_gl_errors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    check_gl_errors();
    return texture_handle;
}

void Engine::display()
{
    SDL_GL_SwapWindow(window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    check_gl_errors();
    glClear(GL_COLOR_BUFFER_BIT);
    check_gl_errors();
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

const char* source_to_strv(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

const char* type_to_strv(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

const char* severity_to_strv(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
    }
    return "unknown";
}

array<char, GL_MAX_DEBUG_MESSAGE_LENGTH> local_log_buff;

void callback_opengl_debug(GLenum                       source,
                           GLenum                       type,
                           GLuint                       id,
                           GLenum                       severity,
                           GLsizei                      length,
                           const GLchar*                message,
                           [[maybe_unused]] const void* userParam)
{
    auto& buff{ local_log_buff };
    int   num_chars = snprintf(buff.data(),
                             buff.size(),
                             "%s %s %d %s %.*s\n",
                             source_to_strv(source),
                             type_to_strv(type),
                             id,
                             severity_to_strv(severity),
                             length,
                             message);

    if (num_chars > 0)
    {
        cerr.write(buff.data(), num_chars);
    }
}

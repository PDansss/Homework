#include "engine.h"
#include <SDL3/SDL.h>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>

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

float mouse_x = -1;
float mouse_y = -1;

float Size  = 1;
float depth = 7;

static bool is_exist     = false;
int         choose_color = -1;

void Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw runtime_error("Engine could not initialize!");
    }

    window = SDL_CreateWindow("gfx-program", 800, 800, SDL_WINDOW_OPENGL);

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
        if (Event.type == SDL_EVENT_KEY_DOWN)
        {
            if (Event.key.keysym.sym == SDLK_w)
            {
                Size += 0.5;
            }
            if (Event.key.keysym.sym == SDLK_s)
            {
                Size -= 0.5;
                if (Size == 0)
                    Size = 0.5;
            }
        }
        if (Event.type == SDL_EVENT_MOUSE_MOTION)
        {
            mouse_x = (2.0f * Event.motion.x) / 800 - 1.0f;
            mouse_y = 1.0f - (2.0f * Event.motion.y) / 800;
        }

        if (Event.type == SDL_EVENT_MOUSE_WHEEL)
        {
            depth += Event.wheel.y;
            if (depth == 0)
                depth = 1;
        }
    }
    return true;
}

void delete_program()
{
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Engine::clean()
{
    delete_program();
    SDL_GL_DeleteContext(opengl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::set_program()
{
    GLint linked;
    vertex_shader   = load_shader(GL_VERTEX_SHADER, "VertexShader.txt");
    fragment_shader = load_shader(GL_FRAGMENT_SHADER, "FragmentShader.txt");
    program         = glCreateProgram();
    glAttachShader(program, vertex_shader);
    check_gl_errors();
    glAttachShader(program, fragment_shader);
    check_gl_errors();
    glLinkProgram(program);
    check_gl_errors();
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    check_gl_errors();
    if (!linked)
    {
        GLint infolen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infolen);
        check_gl_errors();
        if (infolen > 1)
        {
            char* error = new char[infolen];
            glGetProgramInfoLog(program, infolen, NULL, error);
            check_gl_errors();
            cerr << error << endl;
            delete[] error;
        }
        glDeleteProgram(program);
        check_gl_errors();
    }
    else
    {
        glUseProgram(program);
    }
}

void Engine::set_triangle_attributes(vector<float>        Vertex,
                                     vector<unsigned int> indexes,
                                     vector<float>        color)
{
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
    float time = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT);
    check_gl_errors();
    glUniform1f(glGetUniformLocation(program, "time"), time);
    check_gl_errors();
    glUniform1f(glGetUniformLocation(program, "mouse_x"), mouse_x);
    check_gl_errors();
    glUniform1f(glGetUniformLocation(program, "mouse_y"), mouse_y);
    check_gl_errors();
    glUniform1f(glGetUniformLocation(program, "depth"), depth);
    glUniform1f(glGetUniformLocation(program, "size"), Size);
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

game* reload(game*       old,
             const char* library,
             const char* tmp_library,
             void*&      game_library_name);

void clean_game(game* old, void*& game_library_handle);

int main()
{
    Engine* engine = create_engine();
    engine->init();

    const char* library     = "./libGame.so";
    const char* tmp_library = "./tmpGame.so";

    const char* vertex_shader   = "./VertexShader.txt";
    const char* fragment_shader = "./FragmentShader.txt";

    void* game_library_handle = nullptr;
    game* game = reload(nullptr, library, tmp_library, game_library_handle);

    filesystem::file_time_type last_time_write_engine =
        filesystem::last_write_time(library);
    filesystem::file_time_type current_time_write_engine;
    filesystem::file_time_type current_time_write_VS;
    filesystem::file_time_type current_time_write_FS;
    filesystem::file_time_type last_time_write_vertex_shader =
        filesystem::last_write_time(vertex_shader);
    filesystem::file_time_type last_time_write_fragment_shader =
        filesystem::last_write_time(fragment_shader);

    game->init();
    engine->set_triangle_attributes(
        game->get_position(), game->get_indexes(), game->get_color());

    while (true)
    {
        try
        {
            current_time_write_engine = filesystem::last_write_time(library);
            current_time_write_VS = filesystem::last_write_time(vertex_shader);
            current_time_write_FS =
                filesystem::last_write_time(fragment_shader);
        }
        catch (filesystem::filesystem_error& ex)
        {
            cout << ex.what();
            continue;
        }

        if (current_time_write_engine != last_time_write_engine)
        {
            filesystem::file_time_type now_time_write;

            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(400));
                now_time_write = filesystem::last_write_time(library);
                if (current_time_write_engine != now_time_write)
                {
                    current_time_write_engine = now_time_write;
                }
                else
                {
                    break;
                }
            }
            game = reload(game, library, tmp_library, game_library_handle);

            if (game == nullptr)
            {
                cout << "Could not reload! Another attempt.\n";
                continue;
            }
            cout << "Game was reloaded!\n";
            last_time_write_engine = now_time_write;

            game->init();
            engine->set_triangle_attributes(
                game->get_position(), game->get_indexes(), game->get_color());
            mouse_x = -1;
            mouse_y = -1;
            Size    = 1;
            depth   = 7;
        }

        if (current_time_write_VS != last_time_write_vertex_shader ||
            current_time_write_FS != last_time_write_fragment_shader)
        {
            delete_program();
            engine->set_program();
            engine->set_triangle_attributes(
                game->get_position(), game->get_indexes(), game->get_color());
            cout << "Shader was updated!\n";
            last_time_write_vertex_shader =
                filesystem::last_write_time(vertex_shader);
            last_time_write_fragment_shader =
                filesystem::last_write_time(fragment_shader);
        }

        if (!engine->read_events())
            break;

        engine->update(
            game->get_position(), game->get_indexes(), game->get_color());
    }

    clean_game(game, game_library_handle);
    engine->clean();
    delete_engine(engine);
    return 0;
}

void clean_game(game* old, void*& game_library_handle)
{
    if (old != nullptr && game_library_handle != nullptr)
    {
        SDL_FunctionPointer delete_game_ptr =
            SDL_LoadFunction(game_library_handle, "delete_game");
        using func_delete_ptr = decltype(&delete_game);
        auto delete_game = reinterpret_cast<func_delete_ptr>(delete_game_ptr);
        delete_game(old);
    }
}

game* reload(game*       old,
             const char* library,
             const char* tmp_library,
             void*&      game_library_handle)
{
    if (old)
    {
        clean_game(old, game_library_handle);
        SDL_UnloadObject(game_library_handle);
    }

    if (filesystem::exists(tmp_library))
    {
        remove(tmp_library);
    }

    filesystem::copy(library, tmp_library);

    void* library_handle = SDL_LoadObject(tmp_library);

    if (library_handle == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        return nullptr;
    }

    game_library_handle = library_handle;

    SDL_FunctionPointer create_game_ptr =
        SDL_LoadFunction(library_handle, "create_game");

    using func_ptr = decltype(&create_game);

    auto Create_game = reinterpret_cast<func_ptr>(create_game_ptr);

    return Create_game();
}

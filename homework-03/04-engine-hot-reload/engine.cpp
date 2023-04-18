#include "engine.h"
#include <SDL3/SDL.h>
#include <filesystem>
#include <map>

using namespace std;

std::map<SDL_Keycode, int> match{
    { SDLK_w, 1 }, { SDLK_a, 4 }, { SDLK_s, 3 }, { SDLK_d, 2 }
};

SDL_Window* window   = nullptr;
static bool is_exist = false;

void engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw runtime_error("Could not initialize!");
    }

    window = SDL_CreateWindow("Homework-03-04", 300, 300, SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        throw runtime_error("Сould not create a window!");
    }
    cout << "Engine was initialized!\n";
}

int engine::read_events()
{
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_EVENT_QUIT)
        {
            return 0;
        }
        if (match.contains(Event.key.keysym.sym))
        {
            if (Event.type == SDL_EVENT_KEY_DOWN)
            {
                return match[Event.key.keysym.sym];
            }
        }
    }
    return -1;
}

void engine::clean()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

engine* create_engine()
{
    if (is_exist)
    {
        throw runtime_error("Engine exists!");
    }
    is_exist = true;
    cout << "Create Engine!\n";
    return new engine();
}

void delete_engine(engine* engine)
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
            cout << "Engine was deleted!\n";
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
    engine* engine = create_engine();
    engine->init();

    const char* library     = "./libGame.so";
    const char* tmp_library = "./tmpGame.so";

    void* game_library_handle = nullptr;
    game* game = reload(nullptr, library, tmp_library, game_library_handle);

    filesystem::file_time_type last_time_write =
        filesystem::last_write_time(library);

    filesystem::file_time_type current_time_write;

    game->init();
    int direction;

    while (true)
    {
        try
        {
            current_time_write = filesystem::last_write_time(library);
        }
        catch (std::filesystem::filesystem_error& ex)
        {
            cout << ex.what();
            continue;
        }

        if (current_time_write != last_time_write)
        {
            filesystem::file_time_type now_time_write;

            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(400));
                now_time_write = filesystem::last_write_time(library);
                if (current_time_write != now_time_write)
                {
                    current_time_write = now_time_write;
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
            last_time_write = now_time_write;
        }

        direction = engine->read_events();
        if (!direction)
        {
            break;
        }

        game->update(direction);
        game->render();
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

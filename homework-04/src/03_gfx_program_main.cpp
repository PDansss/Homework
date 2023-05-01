#include "03_gfx_program.h"

int main(int, char*[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    int width  = 550;
    int height = 550;

    canvas img(width, height);

    shader      myshader;
    gfx_program MyGFX;
    myshader.create_image(img, 150, width / 2, height / 2, MyGFX);
    myshader.image_update();
    myshader.image_render(img);

    SDL_Window* window =
        SDL_CreateWindow("My gfx program", width, height, SDL_WINDOW_OPENGL);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED);

    void* pixels = img.get_pixels().data();

    bool      run = true;
    SDL_Event Event;

    double time;
    double radius  = 40;
    double mouse_x = 0;
    double mouse_y = 0;
    bool   build   = false;
    while (run)
    {
        time = SDL_GetTicks();
        MyGFX.set_uniforms({ mouse_x, mouse_y, time, radius, build });

        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_EVENT_QUIT)
            {
                run = false;
            }
            if (Event.type == SDL_EVENT_MOUSE_MOTION)
            {
                mouse_x = static_cast<double>(Event.motion.x);
                mouse_y = static_cast<double>(Event.motion.y);
            }
            if (Event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                build = true;
            }
            if (Event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                build = false;
            }
            if (Event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                radius += Event.wheel.y;
                if (radius < 0)
                {
                    radius = 1;
                }
            }
            MyGFX.set_uniforms({ mouse_x, mouse_y, time, radius, build });
        }

        myshader.image_update();
        myshader.image_render(img);

        SDL_Surface* bitmapSurface = SDL_CreateSurfaceFrom(
            pixels, width, height, width * 3, SDL_PIXELFORMAT_RGB24);

        SDL_Texture* texture =
            SDL_CreateTextureFromSurface(renderer, bitmapSurface);

        SDL_DestroySurface(bitmapSurface);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

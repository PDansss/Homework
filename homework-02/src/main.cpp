#include <SDL3/SDL.h>
#include <iostream>

int main()
{
    SDL_version compiled = { 0, 0, 0 };
    SDL_version linked = { 0, 0, 0 };

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << "compiled: " << static_cast<int>(compiled.major) << '.'
              << static_cast<int>(compiled.minor) << '.'
              << static_cast<int>(compiled.patch) << std::endl;

    std::cout << "linked: " << static_cast<int>(linked.major) << '.'
              << static_cast<int>(linked.minor) << '.'
              << static_cast<int>(linked.patch) << std::endl;

    return !std::cout.good();
}

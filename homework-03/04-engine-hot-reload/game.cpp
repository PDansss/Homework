#include "engine.h"
#include <vector>

const char* red    = "\033[31m";
const char* blue   = "\033[34m";
const char* green  = "\033[32m";
const char* yellow = "\033[33m";
const char* reset  = "\033[0m";

class game_realization : public game
{
public:
    void init() { std::cout << "Game was initialized!\n"; }
    void update(int dir)
    {
        direction = dir;
        current_frame++;
        if (current_frame == 5)
        {
            current_frame = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    void render()
    {
        drow_arrows(direction);
        drow_waiting_bar();
    }

private:
    std::vector<const char*> frames = {
        "-    ", "--   ", "---  ", "---- ", "---->"
    };
    int current_frame = 0;
    int direction;

    void drow_arrows(int dir)
    {
        switch (dir)
        {
            case 1:
                current_frame = 4;
                drow_waiting_bar();
                std::cout << std::endl << "          /\\ \n";
                std::cout << "         /  \\ \n";
                std::cout << "        /    \\ \n";
                std::cout << "       /------\\ \n";
                std::cout << "      /        \\ \n";
                std::cout << "     /          \\ \n";
                std::cout << "    /------------\\ \n";
                std::cout << "          | | \n";
                std::cout << "          | | \n" << std::endl;
                break;
            case 2:
                current_frame = 4;
                drow_waiting_bar();
                std::cout << std::endl << "      |\\ \n";
                std::cout << "      | \\ \n";
                std::cout << "      |  \\ \n";
                std::cout << "      |   \\ \n";
                std::cout << "  ____|    \\ \n";
                std::cout << "  ____|    / \n";
                std::cout << "      |   / \n";
                std::cout << "      |  / \n";
                std::cout << "      | / \n";
                std::cout << "      |/ \n" << std ::endl;
                break;
            case 3:
                current_frame = 4;
                drow_waiting_bar();
                std::cout << std::endl << "          | | \n";
                std::cout << "          | | \n";
                std::cout << "    \\------------/ \n";
                std::cout << "     \\          / \n";
                std::cout << "      \\        / \n";
                std::cout << "       \\------/ \n";
                std::cout << "        \\    / \n";
                std::cout << "         \\  / \n";
                std::cout << "          \\/ \n" << std::endl;
                break;
            case 4:
                current_frame = 4;
                drow_waiting_bar();
                std::cout << std::endl << "        /| \n";
                std::cout << "       / | \n";
                std::cout << "      /  | \n";
                std::cout << "     /   | \n";
                std::cout << "    /    |____ \n";
                std::cout << "    \\    |____ \n";
                std::cout << "     \\   | \n";
                std::cout << "      \\  | \n";
                std::cout << "       \\ | \n";
                std::cout << "        \\| \n" << std::endl;
                break;
        }
    }
    void drow_waiting_bar()
    {
        std::cout << "\r" << frames[current_frame] << std::flush;
    }
};

game* create_game()
{
    std::cout << "Game was created!\n";
    return new game_realization();
}
void delete_game(game* game)
{
    delete game;
    std::cout << "Game was deleted!\n";
}

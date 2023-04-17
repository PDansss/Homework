#include <iostream>
#include <thread>

class engine
{
public:
    void init();
    int  read_events();
    void clean();
};
engine* create_engine();
void    delete_engine(engine* engine);

class game
{
public:
    virtual ~game()              = default;
    virtual void init()          = 0;
    virtual void update(int dir) = 0;
    virtual void render()        = 0;
};

extern "C" game* create_game();
extern "C" void  delete_game(game* game);

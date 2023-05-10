#include <iostream>

class Engine
{
public:
    void init();
    bool read_events();
    void clean();
    void update();
};

Engine* create_engine();
void    delete_engine(Engine* engine);

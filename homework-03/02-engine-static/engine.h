#include <iostream>

class Engine
{
public:
    void init();
    bool read_events();
    void clean();
};

Engine* create_engine();
void    delete_engine(Engine* engine);


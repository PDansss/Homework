#include "engine.h"

int main()
{
    Engine* engine = create_engine();
    engine->init();
    while (true)
    {
        if (!engine->read_events())
            break;
    }
    engine->clean();
    delete_engine(engine);
    return 0;
}

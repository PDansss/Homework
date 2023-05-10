#include "engine.hpp"
#include <cmath>

using namespace std;

int main()
{
    Engine* engine = create_engine();
    engine->init();

    vector<float>        Vertexes = { 1.0f,  1.0f,  0.0f, 0.0f,  0.0f,
                                      0.0f,  -1.0f, 1.0f, 0.0f,  -1.0f,
                                      -1.0f, 0.0f,  1.0f, -1.0f, 0.0f };
    vector<unsigned int> Indexes  = { 0, 1, 2, 1, 3, 4 };

    vector<float> col = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    engine->set_triangle_indexed(Vertexes, Indexes, col);

    while (true)
    {
        if (!engine->read_events())
            break;
        engine->update(Vertexes, Indexes, col);
    }

    engine->clean();
    delete_engine(engine);
    return 0;
}

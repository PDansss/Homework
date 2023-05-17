#include "engine.hpp"
#include <cmath>

using namespace std;

int main()
{
    Engine* engine = create_engine();
    engine->init();

    //    vector<float> VER = { 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    //    0.0f,
    //                          0.5f,  -0.5f, 0.0f,
    //                          0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -0.5f, 0.5f, 0.0f,
    //                          0.0f, 1.0f, 1.0f, 0.0f, 0.0f };

    //    vector<float> VER2 = { -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f,
    //                           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    //                           0.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
    //                           0.0f, 1.0f, 1.0f };

    vector<float> VER = { 0.0f,  0.85f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                          0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                          0.85f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };

    vector<float> VER2 = { 0.0f,  0.85f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                           0.85f, 0.85f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                           0.85f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f };

    engine->set_texture("./br.png");
    while (true)
    {
        if (!engine->read_events())
            break;
        engine->render_triangle_with_texture(VER);
        engine->render_triangle_with_texture(VER2);
        engine->display();
    }
    engine->clean();
    delete_engine(engine);
    return 0;
}

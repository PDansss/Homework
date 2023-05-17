#include "engine.hpp"
#include "my_math.hpp"

using namespace std;

int main()
{
    Engine* engine = create_engine();
    float   width = 700, height = 700;
    engine->init(width, height);

    vector<float> ver1 = { -0.1f, 0.2f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                           0.1f,  0.2f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                           0.1f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    vector<float> ver2 = { -0.1f, 0.2f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                           0.1f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                           -0.1f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f };

    vector<int>   events;
    vector<float> matrix;
    vector<float> normolize_matrix;

    bool    is_shoot = false;
    my_math math;

    float dx = 0, dy = 0, dir = 0;

    if (width > height)
    {
        normolize_matrix = math.scaling_matrix(height / width, 1);
    }
    else
    {
        normolize_matrix = math.scaling_matrix(1, width / height);
    }

    while (true)
    {
        is_shoot = false;
        if (!engine->read_events())
            break;
        events = engine->get_events_buffer();

        if (events[0]) // w
        {
            dx += (sin(dir) / 500) * -1;
            dy += (cos(dir) / 500);
        }
        if (events[1]) // s
        {
            dx -= (sin(dir) / 500) * -1;
            dy -= (cos(dir) / 500);
        }

        if (events[2]) // mouse button
        {
            is_shoot = true;
        }

        if (events[3]) // mouse wheel
        {
            dir += 0.07 * events[3];
        }
        vector<float> buff = math.matrix_multiplying(normolize_matrix,
                                                     math.shift_matrix(dx, dy));
        matrix = math.matrix_multiplying(buff, math.rotate_matrix(dir));

        engine->render_triangle(ver1, matrix, is_shoot);
        engine->render_triangle(ver2, matrix, is_shoot);
        engine->display();
    }
    engine->clean();
    delete_engine(engine);
    return 0;
}

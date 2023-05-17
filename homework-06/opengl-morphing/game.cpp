#include "engine.hpp"

using namespace std;

int main()
{
    Engine* engine = create_engine();
    engine->init();

    //    vector<float> VER = { 0.0f,  0.85f, 0.0f, 1.0f, 0.0f, 0.0f,
    //                          0.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    //                          0.85f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };

    //    vector<float> VER2 = { 0.0f,  0.85f, 0.0f, 1.0f, 1.0f, 0.0f,
    //                           0.85f, 0.85f, 0.0f, 1.0f, 0.0f, 1.0f,
    //                           0.85f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f };

    vector<float> circle_vertexes;
    vector<float> deltoid_vertexes;
    vector<float> astroid_vertexes;
    const float   pi                  = 3.1415926535;
    float         radius              = 0.5;
    int           number_of_triangles = 60;

    circle_vertexes.push_back(0);
    circle_vertexes.push_back(0);
    circle_vertexes.push_back(0);

    circle_vertexes.push_back(0);
    circle_vertexes.push_back(0);
    circle_vertexes.push_back(0);

    for (int i = 1; i < number_of_triangles; i += 2)
    {
        float theta = (i - 1) * (2 * pi / number_of_triangles);
        float x     = radius * cos(theta);
        float y     = radius * sin(theta);
        circle_vertexes.push_back(x);
        circle_vertexes.push_back(y);
        circle_vertexes.push_back(0);

        circle_vertexes.push_back(1);
        circle_vertexes.push_back(0);
        circle_vertexes.push_back(0);

        theta = (i + 1) * (2 * pi / number_of_triangles);
        x     = radius * cos(theta);
        y     = radius * sin(theta);
        circle_vertexes.push_back(x);
        circle_vertexes.push_back(y);
        circle_vertexes.push_back(0);

        circle_vertexes.push_back(0);
        circle_vertexes.push_back(0);
        circle_vertexes.push_back(1);
    }
    deltoid_vertexes.push_back(0);
    deltoid_vertexes.push_back(0);
    deltoid_vertexes.push_back(0);

    deltoid_vertexes.push_back(1);
    deltoid_vertexes.push_back(1);
    deltoid_vertexes.push_back(1);

    radius -= 0.3;
    for (int i = 1; i < number_of_triangles; i += 2)
    {
        float theta = (i - 1) * (2 * pi / number_of_triangles);
        float x     = 2 * radius * cos(theta) + radius * cos(2 * theta);
        float y     = 2 * radius * sin(theta) - radius * sin(2 * theta);

        deltoid_vertexes.push_back(x);
        deltoid_vertexes.push_back(y);
        deltoid_vertexes.push_back(0);
        deltoid_vertexes.push_back(1);
        deltoid_vertexes.push_back(0);
        deltoid_vertexes.push_back(1);

        theta = (i + 1) * (2 * pi / number_of_triangles);
        x     = 2 * radius * cos(theta) + radius * cos(2 * theta);
        y     = 2 * radius * sin(theta) - radius * sin(2 * theta);

        deltoid_vertexes.push_back(x);
        deltoid_vertexes.push_back(y);
        deltoid_vertexes.push_back(0);
        deltoid_vertexes.push_back(0);
        deltoid_vertexes.push_back(1);
        deltoid_vertexes.push_back(1);
    }

    radius += 0.3;
    astroid_vertexes.push_back(0);
    astroid_vertexes.push_back(0);
    astroid_vertexes.push_back(0);
    astroid_vertexes.push_back(0.5);
    astroid_vertexes.push_back(0.5);
    astroid_vertexes.push_back(0.5);
    for (int i = 1; i < number_of_triangles; i += 2)
    {
        float theta = (i - 1) * (2 * pi / number_of_triangles);
        float x     = radius * pow(cos(theta), 3);
        float y     = radius * pow(sin(theta), 3);
        astroid_vertexes.push_back(x);
        astroid_vertexes.push_back(y);
        astroid_vertexes.push_back(0);
        astroid_vertexes.push_back(0.5);
        astroid_vertexes.push_back(0.1);
        astroid_vertexes.push_back(0.8);
        theta = (i + 1) * (2 * pi / number_of_triangles);
        x     = radius * pow(cos(theta), 3);
        y     = radius * pow(sin(theta), 3);
        astroid_vertexes.push_back(x);
        astroid_vertexes.push_back(y);
        astroid_vertexes.push_back(0);
        astroid_vertexes.push_back(1);
        astroid_vertexes.push_back(1);
        astroid_vertexes.push_back(1);
    }

    while (true)
    {
        if (!engine->read_events())
            break;
        // engine->render_triangle(VER);
        // engine->render_triangle(VER2);
        // engine->render_triangle(circle_vertexes);
        engine->morphing(circle_vertexes, deltoid_vertexes, astroid_vertexes);
        engine->display();
    }
    engine->clean();
    delete_engine(engine);
    return 0;
}

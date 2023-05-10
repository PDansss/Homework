#include "engine.h"
#include <cmath>

class game_realization : public game
{
public:
    void init()
    {
        const float pi                  = 3.1415926535;
        float       radius              = 0.2;
        int         number_of_triangles = 60;

        Vertexes.push_back(0);
        Vertexes.push_back(0);
        Vertexes.push_back(0);
        col.push_back(0.0);
        col.push_back(1.0);
        col.push_back(0.0);

        //        for (int i = 1; i < number_of_triangles; i += 2)
        //        {
        //            float theta = (i - 1) * (2 * pi / number_of_triangles);
        //            float x     = radius * cos(theta);
        //            float y     = radius * sin(theta);
        //            Vertexes.push_back(x);
        //            Vertexes.push_back(y);
        //            Vertexes.push_back(0);
        //            col.push_back(0.0);
        //            col.push_back(0.0);
        //            col.push_back(1.0);

        //            theta = (i + 1) * (2 * pi / number_of_triangles);
        //            x     = radius * cos(theta);
        //            y     = radius * sin(theta);
        //            Vertexes.push_back(x);
        //            Vertexes.push_back(y);
        //            Vertexes.push_back(0);
        //            col.push_back(1.0);
        //            col.push_back(0.0);
        //            col.push_back(0.0);
        //            Indexes.push_back(0);
        //            Indexes.push_back(Vertexes.size() / 3 - 1);
        //            Indexes.push_back(Vertexes.size() / 3 - 2);
        //        }

        for (int i = 1; i < number_of_triangles; i += 2)
        {
            float theta = (i - 1) * (2 * pi / number_of_triangles);
            float x     = 2 * radius * cos(theta) + radius * cos(2 * theta);
            float y     = 2 * radius * sin(theta) - radius * sin(2 * theta);

            Vertexes.push_back(x);
            Vertexes.push_back(y);
            Vertexes.push_back(0);
            col.push_back(0.0);
            col.push_back(0.0);
            col.push_back(1.0);

            theta = (i + 1) * (2 * pi / number_of_triangles);
            x     = 2 * radius * cos(theta) + radius * cos(2 * theta);
            y     = 2 * radius * sin(theta) - radius * sin(2 * theta);

            Vertexes.push_back(x);
            Vertexes.push_back(y);
            Vertexes.push_back(0);
            col.push_back(1.0);
            col.push_back(0.0);
            col.push_back(0.0);
            Indexes.push_back(0);
            Indexes.push_back(Vertexes.size() / 3 - 1);
            Indexes.push_back(Vertexes.size() / 3 - 2);
        }
    }
    vector<float>        get_position() { return Vertexes; }
    vector<unsigned int> get_indexes() { return Indexes; }
    vector<float>        get_color() { return col; }

private:
    vector<float>        Vertexes;
    vector<unsigned int> Indexes;
    vector<float>        col;
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

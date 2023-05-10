#include <iostream>
#include <vector>

using namespace std;

struct vertex
{
    float x = 0;
    float y = 0;
    float z = 0;
};

class Engine
{
public:
    void init();
    bool read_events();
    void clean();
    void set_program();

    void set_triangle_indexed(vector<float>        Vertex,
                              vector<unsigned int> indexes,
                              vector<float>        color);

    void update(vector<float>        Vertex,
                vector<unsigned int> indexes,
                vector<float>        color);
};

Engine* create_engine();
void    delete_engine(Engine* engine);

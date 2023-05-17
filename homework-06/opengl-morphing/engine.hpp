#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class Engine
{
public:
    void init();
    bool read_events();
    bool set_texture(const char* path);
    void clean();
    void set_program();
    void render_triangle(vector<float> atribute_vertex);
    void render_triangle_with_texture(vector<float> atribute_vertex);
    void morphing(vector<float> atribute_vertex1,
                  vector<float> atribute_vertex2,
                  vector<float> atribute_vertex3);
    void display();
};

Engine* create_engine();
void    delete_engine(Engine* engine);

#include <iostream>
#include <vector>

using namespace std;

class Engine
{
public:
    void         init(float w, float h);
    bool         read_events();
    vector<int>  get_events_buffer();
    unsigned int set_texture(const char* path);
    void         clean();
    unsigned int set_program(const char* vertex_shader,
                             const char* fragment_shader);

    void render_triangle(vector<float> atribute_vertex, bool flag);
    void render_triangle(vector<float> atribute_vertex, vector<float> matrix);

    void render_triangle(vector<float> atribute_vertex,
                         vector<float> matrix,
                         bool          flag);

    void morphing(vector<float> atribute_vertex1,
                  vector<float> atribute_vertex2,
                  vector<float> atribute_vertex3);
    void display();
};

Engine* create_engine();
void    delete_engine(Engine* engine);

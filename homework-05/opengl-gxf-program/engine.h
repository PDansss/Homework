#include <iostream>
#include <thread>
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
    void render_triangle();

    void set_triangle_attributes(vector<float>        Vertex,
                                 vector<unsigned int> indexes,
                                 vector<float>        color);

    void update(vector<float>        Vertex,
                vector<unsigned int> indexes,
                vector<float>        color);
};

class game
{
public:
    virtual ~game()                             = default;
    virtual void                 init()         = 0;
    virtual vector<float>        get_position() = 0;
    virtual vector<unsigned int> get_indexes()  = 0;
    virtual vector<float>        get_color()    = 0;
};

extern "C" game* create_game();
extern "C" void  delete_game(game* game);

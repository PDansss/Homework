#include "02_triangle.h"
#include <SDL3/SDL.h>
#include <cmath>

struct uniforms
{
    double mouse_x = 0;
    double mouse_y = 0;
    double time    = 0;
    double radius  = 0;
    bool   build   = false;
};

class gfx_program
{
public:
    vertex    vertex_shader(const vertex& ver, position pos);
    color_rgb fragment_shader(const vertex& ver);
    void      set_uniforms(uniforms uniform);

private:
    double mouse_x = 0;
    double mouse_y = 0;
    double time    = 0;
    double radius  = 0;
    bool   build   = false;
    int    t       = 1;
};

class shader
{
public:
    void create_image(canvas&      img,
                      double       radius,
                      int          offset_X,
                      int          offset_Y,
                      gfx_program& program)
    {
        myprogram = &program;
        for (int i = 0; i < 1000; ++i)
        {
            double theta = i * (2 * M_PI / 1000);
            int    x = offset_X + static_cast<int>(ceil(radius * cos(theta)));
            int    y = offset_Y + static_cast<int>(ceil(radius * sin(theta)));
            vertexes.push_back({ static_cast<double>(ceil(x)),
                                 static_cast<double>(ceil(y)),
                                 0,
                                 0,
                                 0 });
            start_pos.push_back({ x, y });

            img.set_pixel(x, y, { 255, 0, 0 });
        }

        for (int i = 0; i < 1000; i++)
        {
            double theta = i * (2 * M_PI / 1000);
            int    x = offset_X + static_cast<int>(radius * pow(cos(theta), 3));
            int    y = offset_Y + static_cast<int>(radius * pow(sin(theta), 3));

            vertexes.push_back({ static_cast<double>(ceil(x)),
                                 static_cast<double>(ceil(y)),
                                 0,
                                 255,
                                 0 });
            start_pos.push_back({ x, y });

            img.set_pixel(x, y, { 0, 0, 0 });
        }

        for (int i = 0; i < 1000; i++)
        {
            double theta = 2 * M_PI * i / 1000;
            int    x =
                offset_X + static_cast<int>((radius * sqrt(2) * cos(theta)) /
                                            (1 + pow(sin(theta), 2)));
            int y =
                offset_Y +
                static_cast<int>((radius * sqrt(2) * sin(theta) * cos(theta)) /
                                 (1 + pow(sin(theta), 2)));

            vertexes.push_back({ static_cast<double>(ceil(x)),
                                 static_cast<double>(ceil(y)),
                                 0,
                                 0,
                                 0 });
            start_pos.push_back({ x, y });

            img.set_pixel(x, y, { 0, 0, 255 });
        }
    }

    void image_update()
    {
        for (int i = 0; i < vertexes.size(); i++)
        {
            vertexes[i] = myprogram->vertex_shader(vertexes[i], start_pos[i]);
        }
    }

    void image_render(canvas& img)
    {
        img.set_background_color({ 0, 0, 0 });
        color_rgb f;
        for (int i = 0; i < vertexes.size(); i++)
        {
            color_rgb c       = myprogram->fragment_shader(vertexes[i]);
            f                 = c;
            vertexes[i].red   = static_cast<double>(c.red);
            vertexes[i].green = static_cast<double>(c.green);
            vertexes[i].blue  = static_cast<double>(c.blue);
            img.set_pixel(static_cast<double>(vertexes[i].x),
                          static_cast<int>(vertexes[i].y),
                          { c.red, c.green, c.blue });
        }
    }

private:
    vector<vertex>   vertexes;
    vector<position> start_pos;

    triangle     trian;
    gfx_program* myprogram;
};

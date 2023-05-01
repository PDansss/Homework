#include "03_gfx_program.h"

vertex gfx_program::vertex_shader(const vertex& ver, position pos)
{
    vertex current_vertex = ver;
    double x              = ver.x;
    double y              = ver.y;

    double dx = abs(mouse_x - x);
    double dy = abs(mouse_y - y);

    if (dx * dx + dy * dy < radius * radius)
    {
        if (build)
        {
            current_vertex.x = pos.x;
            current_vertex.y = pos.y;
        }
        else
        {
            current_vertex.x += (rand() % 10) * t;
            current_vertex.y += (rand() % 10) * t;
            t = -t;
        }
    }
    else
    {
        current_vertex.x = x;
        current_vertex.y = y;
    }
    return current_vertex;
}

color_rgb gfx_program::fragment_shader(const vertex& ver)
{
    double    dx      = mouse_x - ver.x;
    double    dy      = mouse_y - ver.y;
    vertex    current = { mouse_x, mouse_y, 255, 255, 255 };
    color_rgb col;
    if (dx * dx + dy * dy < radius * radius)
    {
        col = interpolate_color(
            current, ver, sqrt((dx * dx) + (dy * dy)) / 2 * radius);
        return col;
    }
    return {
        static_cast<uint8_t>(255 * 0.5 * (1 + sin(2 * M_PI * time / 5000))),
        static_cast<uint8_t>(255 * 0.5 * (1 - sin(2 * M_PI * time / 5000))),
        static_cast<uint8_t>(50)
    };
}

void gfx_program::set_uniforms(uniforms uniform)
{
    mouse_x = uniform.mouse_x;
    mouse_y = uniform.mouse_y;
    time    = uniform.time;
    radius  = uniform.radius;
    build   = uniform.build;
}

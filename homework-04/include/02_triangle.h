#pragma once
#include "00_canvas.h"
#include "01_line.h"

struct vertex
{
    double x     = 0;
    double y     = 0;
    double red   = 0;
    double green = 0;
    double blue  = 0;
};

double interpolate(const double v1, const double v2, const double t);

vertex interpolate_vertex(const vertex& v1, const vertex& v2, double t);

color_rgb interpolate_color(const vertex& v1, const vertex& v2, double t);

class triangle
{
public:
    void drow_triangle(canvas&          canvas,
                       vector<position> vertexes,
                       color_rgb        color);

    void drow_triangle_indexed(canvas&          canvas,
                               vector<position> vertexes,
                               vector<int>      indexes,
                               color_rgb        color);

    vector<position> pixels_triangle(position p1, position p2, position p3);

    void drow_rasterize_triangle(canvas&          canvas,
                                 vector<position> vertexes,
                                 color_rgb        color);

    void drow_triangle_interpolated(canvas& canvas, vector<vertex> vertexes);

private:
    line side;
};

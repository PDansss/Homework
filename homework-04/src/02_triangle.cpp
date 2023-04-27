#include "02_triangle.h"

double interpolate(const double v1, const double v2, const double t)
{
    return v1 + (v2 - v1) * t;
}
vertex interpolate_vertex(const vertex& v1, const vertex& v2, double t)
{
    return { interpolate(v1.x, v2.x, t),
             interpolate(v1.y, v2.y, t),
             interpolate(v1.red, v2.red, t),
             interpolate(v1.green, v2.green, t),
             interpolate(v1.blue, v2.blue, t) };
}

color_rgb interpolate_color(const vertex& v1, const vertex& v2, double t)
{
    return { static_cast<uint8_t>(interpolate(v1.red, v2.red, t)),
             static_cast<uint8_t>(interpolate(v1.green, v2.green, t)),
             static_cast<uint8_t>(interpolate(v1.blue, v2.blue, t)) };
}

void triangle::drow_triangle(canvas&          canvas,
                             vector<position> vertexes,
                             color_rgb        color)
{
    int              size = vertexes.size() / 3;
    vector<position> pixels;
    for (int i = 0; i < size; i++)
    {
        pixels = pixels_triangle(
            vertexes.at(i * 3), vertexes.at(i * 3 + 1), vertexes.at(i * 3 + 2));

        for (int j = 0; j < pixels.size(); j++)
        {
            canvas.set_pixel(pixels.at(j).x, pixels.at(j).y, color);
        }
    }
}

void triangle::drow_triangle_indexed(canvas&          canvas,
                                     vector<position> vertexes,
                                     vector<int>      indexes,
                                     color_rgb        color)
{
    int              size = indexes.size() / 3;
    vector<position> pixels;
    for (int i = 0; i < size; i++)
    {
        int index0 = indexes[i * 3];
        int index1 = indexes[i * 3 + 1];
        int index2 = indexes[i * 3 + 2];
        pixels     = pixels_triangle(
            vertexes.at(index0), vertexes.at(index1), vertexes.at(index2));
        for (int j = 0; j < pixels.size(); j++)
        {
            canvas.set_pixel(pixels.at(j).x, pixels.at(j).y, color);
        }
    }
}

vector<position> triangle::pixels_triangle(position p1,
                                           position p2,
                                           position p3)
{
    vector<position> pixels_of_triangle;
    vector<position> pixels_of_line;

    pixels_of_line = side.pixels_line(p1.x, p1.y, p2.x, p2.y);
    pixels_of_triangle.insert(
        pixels_of_triangle.end(), pixels_of_line.begin(), pixels_of_line.end());

    pixels_of_line = side.pixels_line(p1.x, p1.y, p3.x, p3.y);
    pixels_of_triangle.insert(
        pixels_of_triangle.end(), pixels_of_line.begin(), pixels_of_line.end());

    pixels_of_line = side.pixels_line(p2.x, p2.y, p3.x, p3.y);
    pixels_of_triangle.insert(
        pixels_of_triangle.end(), pixels_of_line.begin(), pixels_of_line.end());

    return pixels_of_triangle;
}

void triangle::drow_rasterize_triangle(canvas&          canvas,
                                       vector<position> vertexes,
                                       color_rgb        color)
{
    vector<position*> pi = { &vertexes[0], &vertexes[1], &vertexes[2] };

    sort(pi.begin(),
         pi.end(),
         [](position* left, position* right) { return left->y < right->y; });

    position& top    = *pi[0];
    position& middle = *pi[1];
    position& bottom = *pi[2];

    vector<position> top_middle =
        side.pixels_line(top.x, top.y, middle.x, middle.y);
    vector<position> bottom_middle =
        side.pixels_line(bottom.x, bottom.y, middle.x, middle.y);

    int dx = bottom.x - top.x;
    int dy = bottom.y - top.y;
    int c  = top.y * bottom.x - top.x * bottom.y;

    for (int i = 0; i < top_middle.size(); i++)
    {
        int x = (top_middle[i].y * dx - c) / dy;
        side.drow_line(canvas, top_middle[i], { x, top_middle[i].y }, color);
    }
    for (int i = 0; i < bottom_middle.size(); i++)
    {
        int x = (bottom_middle[i].y * dx - c) / dy;
        side.drow_line(
            canvas, bottom_middle[i], { x, bottom_middle[i].y }, color);
    }
}

void triangle::drow_triangle_interpolated(canvas&        canvas,
                                          vector<vertex> vertexes)
{
    vector<vertex*> pos = { &vertexes[0], &vertexes[1], &vertexes[2] };
    sort(pos.begin(),
         pos.end(),
         [](vertex* left, vertex* right) { return left->y < right->y; });

    vertex& top    = *pos[0];
    vertex& middle = *pos[1];
    vertex& bottom = *pos[2];

    int y = middle.y;

    vertex medium = interpolate_vertex(
        top, bottom, static_cast<double>(y - top.y) / abs(bottom.y - top.y));
    vertex left, right;

    int num_y = abs(top.y - y) + 1;

    for (int i = 0; i < num_y; i++)
    {
        left  = interpolate_vertex(middle, top, static_cast<double>(i) / num_y);
        right = interpolate_vertex(medium, top, static_cast<double>(i) / num_y);
        int num_x = abs(left.x - right.x) + 1;
        for (int j = 0; j < num_x; j++)
        {
            vertex f =
                interpolate_vertex(left, right, static_cast<double>(j) / num_x);
            canvas.set_pixel(static_cast<int>(f.x),
                             static_cast<int>(f.y),
                             { static_cast<uint8_t>(f.red),
                               static_cast<uint8_t>(f.green),
                               static_cast<uint8_t>(f.blue) });
        }
    }

    num_y = abs(bottom.y - y) + 1;
    for (int i = 0; i < num_y; i++)
    {
        left =
            interpolate_vertex(middle, bottom, static_cast<double>(i) / num_y);
        right =
            interpolate_vertex(medium, bottom, static_cast<double>(i) / num_y);
        int num_x = abs(left.x - right.x) + 1;
        for (int j = 0; j <= num_x; j++)
        {
            vertex f =
                interpolate_vertex(left, right, static_cast<double>(j) / num_x);

            canvas.set_pixel(static_cast<int>(f.x),
                             static_cast<int>(f.y),
                             { static_cast<uint8_t>(f.red),
                               static_cast<uint8_t>(f.green),
                               static_cast<uint8_t>(f.blue) });
        }
    }
}

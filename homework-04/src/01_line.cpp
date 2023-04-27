#include "01_line.h"

void line::drow_line(canvas& canvas, position p1, position p2, color_rgb color)
{
    vector<position> pixels = pixels_line(p1.x, p1.y, p2.x, p2.y);
    for_each(pixels.begin(),
             pixels.end(),
             [&](auto& position)
             { canvas.set_pixel(position.x, position.y, color); });
}

vector<position> line::pixels_line(int x0, int y0, int x1, int y1)
{
    vector<position> pixels_of_line;

    auto drow_line_low = [&](int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;

        int y  = y0;
        int yi = 1;

        if (dy < 0)
        {
            yi = -1;
            dy = -dy;
        }

        int D = 2 * dy - dx;
        for (int x = x0; x <= x1; x++)
        {
            pixels_of_line.push_back(position{ x, y });
            if (D > 0)
            {
                y += yi;
                D -= 2 * dx;
            }
            D = D + 2 * dy;
        }
    };

    auto drow_line_high = [&](int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;

        int x  = x0;
        int xi = 1;

        if (dx < 0)
        {
            xi = -1;
            dx = -dx;
        }

        int D = 2 * dx - dy;
        for (int y = y0; y <= y1; y++)
        {
            pixels_of_line.push_back(position{ x, y });
            if (D > 0)
            {
                x += xi;
                D -= 2 * dy;
            }
            D = D + 2 * dx;
        }
    };

    if (abs(x1 - x0) >= abs(y1 - y0))
    {
        if (x0 > x1)
        {
            drow_line_low(x1, y1, x0, y0);
        }
        else
        {
            drow_line_low(x0, y0, x1, y1);
        }
    }
    else
    {
        if (y0 > y1)
        {
            drow_line_high(x1, y1, x0, y0);
        }
        else
        {
            drow_line_high(x0, y0, x1, y1);
        }
    }
    return pixels_of_line;
}

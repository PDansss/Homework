#pragma once
#include "00_canvas.h"

struct position
{
    int x = 0;
    int y = 0;
};

class line
{
public:
    void drow_line(canvas& canvas, position p1, position p2, color_rgb color);
    vector<position> pixels_line(int x0, int y0, int x1, int y1);
};

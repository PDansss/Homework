#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct color_rgb
{
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
};

// color_rgb red   = { 255, 0, 0 };
// color_rgb green = { 0, 255, 0 };
// color_rgb blue  = { 0, 0, 255 };

class canvas
{
public:
    canvas(unsigned int w, unsigned int h);
    void         save_image(const char* filename);
    void         load_image(const char* filename);
    void         set_pixel(int x, int y, color_rgb color);
    void         set_background_color(color_rgb color);
    unsigned int get_width();
    unsigned int get_height();

private:
    unsigned int      width  = 0;
    unsigned int      height = 0;
    vector<color_rgb> pixels;
};

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

class canvas
{
public:
    canvas(unsigned int w, unsigned int h);
    void               save_image(const char* filename);
    void               load_image(const char* filename);
    void               set_pixel(int x, int y, color_rgb color);
    void               set_background_color(color_rgb color);
    vector<color_rgb>& get_pixels();
    unsigned int       get_width();
    unsigned int       get_height();

private:
    unsigned int      width  = 0;
    unsigned int      height = 0;
    vector<color_rgb> pixels;
};

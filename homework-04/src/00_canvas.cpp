#include "00_canvas.h"

canvas::canvas(unsigned int w, unsigned int h)
{
    width  = w;
    height = h;
    pixels.reserve(width * height);
    pixels.resize(width * height);
}

void canvas::save_image(const char* filename)
{
    ofstream MyFile;
    MyFile.open(filename, ios_base::binary);
    MyFile << "P6\n" << width << ' ' << height << '\n' << 255 << '\n';
    MyFile.write((char*)pixels.data(), 3 * pixels.size());
    MyFile.close();
}

void canvas::load_image(const char* filename)
{
    ifstream file_ppm;
    string   header, color;
    file_ppm.open(filename, ios_base::binary);
    char pixels_info = 0;
    file_ppm >> header >> width >> height >> color >> noskipws >> pixels_info;
    if (header != "P6" || color != "255")
    {
        cerr << "Something wrong with this file! Please, check header and "
                "color headers.";
    }
    pixels.reserve(width * height);
    pixels.resize(width * height);
    file_ppm.read(reinterpret_cast<char*>(pixels.data()), 3 * pixels.size());
    file_ppm.close();
}

void canvas::set_pixel(int x, int y, color_rgb color)
{
    unsigned int position = y * width + x;
    color_rgb&   pixel    = pixels.at(position);
    pixel                 = color;
}
void canvas::set_background_color(color_rgb color)
{
    fill(pixels.begin(), pixels.end(), color);
}
unsigned int canvas::get_width()
{
    return width;
}
unsigned int canvas::get_height()
{
    return height;
}

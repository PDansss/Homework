#include "00_canvas.h"
#include <iostream>

int main(int, char**)
{
    canvas img1(500, 500);
    img1.set_background_color({ 233, 120, 40 });
    img1.save_image("My.ppm");
    return 0;
}

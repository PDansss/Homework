#include "01_line.h"

int main(int, char**)
{
    canvas img1(500, 500);
    line   line;
    line.drow_line(img1, { 400, 400 }, { 10, 10 }, { 23, 255, 21 });

    // line.drow_line(img1, { 400, 400 }, { 400, 100 }, { 23, 255, 21 });
    line.drow_line(img1, { 400, 100 }, { 400, 400 }, { 23, 255, 21 });
    img1.save_image("MyLine.ppm");
}

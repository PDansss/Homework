#include "01_line.h"

int main(int, char**)
{
    canvas img1(500, 500);
    
    line   line;
    line.drow_line(img1, { 10, 36 }, { 57, 94 }, { 23, 255, 21 });
    
    img1.save_image("MyLine.ppm");
}

#include "02_triangle.h"

int main(int, char**)
{
    unsigned int width  = 500;
    unsigned int height = 500;
    canvas       img1(width, height);

    triangle trian;
    trian.drow_triangle(
        img1, { { 0, 0 }, { 490, 0 }, { 0, 499 } }, { 255, 255, 0 });
    img1.save_image("simple_triangle.ppm");
    img1.set_background_color({ 0, 0, 0 });

    trian.drow_rasterize_triangle(
        img1, { { { 0, 0 }, { 490, 0 }, { 0, 499 } } }, { 255, 255, 0 });
    img1.save_image("rasterized_triangle.ppm");
    img1.set_background_color({ 0, 0, 0 });

    int number_x_triangle = 30;
    int number_y_triangle = 30;
    int size_x            = (width - 1) / number_x_triangle;
    int size_y            = (height - 1) / number_y_triangle;

    vector<position> Vertexes;
    vector<int>      Indexes;

    for (int i = 0; i <= number_x_triangle; i++)
    {
        for (int j = 0; j <= number_y_triangle; j++)
        {
            Vertexes.push_back({ i * size_x, j * size_y });
        }
    }
    for (int i = 0; i < number_x_triangle; i++)
    {
        for (int j = 0; j < number_y_triangle; j++)
        {
            int index0 = j * (number_y_triangle + 1) + i;
            int index1 = index0 + number_y_triangle + 2;
            int index2 = index1 - 1;
            int index3 = index0 + 1;

            Indexes.push_back(index0);
            Indexes.push_back(index1);
            Indexes.push_back(index2);

            Indexes.push_back(index0);
            Indexes.push_back(index1);
            Indexes.push_back(index3);
        }
    }
    trian.drow_triangle_indexed(img1, Vertexes, Indexes, { 0, 0, 255 });
    img1.save_image("triangle_with_index_and_vertex_buffer.ppm");

    img1.set_background_color({ 0, 0, 0 });

    trian.drow_triangle_interpolated(
        img1,
        { { 0, 0, 255, 0, 0 }, { 0, 499, 0, 255, 0 }, { 499, 0, 0, 0, 255 } });
    img1.save_image("triangle_interpolated.ppm");
    return 0;
}

#include <pybind11/pybind11.h>
#include <iostream>

namespace py = pybind11;

void add_box(float x, float y, float z)
{
    //Add to box data
    std::cout<<"Add Box To "<<x <<","<<y<<","<<z<<","<<std::endl;
}

PYBIND11_MODULE(cpp_renderer, m)
{
    m.def ("add_box", &add_box, "add box tat the given position");
}
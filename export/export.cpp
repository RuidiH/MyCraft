// Third Party
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include the pybindings
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Engine.hpp"

// Standard
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "GameObject.hpp"
#include "Cube.hpp"
#include "TextureManager.hpp"
#include "ShapeComponent.hpp"
#include "Shader.hpp"

namespace py = pybind11;

PYBIND11_MODULE(GameEditor, m)
{
    m.doc() = "Engine as a library";
    py::class_<Engine>(m, "Engine")
        .def(py::init<int, int>(), py::arg("w"), py::arg("h")) // our constructor
        .def("SetupObject", &Engine::SetupObject) 
        .def("MainLoop", &Engine::MainLoop)
        .def("Shutdown", &Engine::Shutdown);
}
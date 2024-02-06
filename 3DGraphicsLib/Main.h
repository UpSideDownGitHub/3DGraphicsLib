#pragma once
#include <glew.h>
#include <Windows.h>
#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glut.h"
# define M_PI 3.14159265358979323846 /* pi */

int main(int argc, char* argv[]);
void init_window(int width, int height);
void init_shaders();
/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "myLogger.h"
#include "myGLFunctions.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>

class Triangle {
public:
    Triangle();
    void    PerformGLInits();
    void    Render();
    void    SetViewport(int width, int height);
    bool    IsInitsDone(){return initsDone;}

private:
    void    RenderTriangle();
    void    AnimateTriangle();

    bool    initsDone;

    // vertex buffer for triangle's vertices, colors
    GLuint  vertexBuffer, colorBuffer;
    // will hold attributed for shader variables
    GLuint  vertexAttribute, colorAttribute;
    GLuint  shaderProgramID;

    float   triangleVertexDelta, triangleSwapRate;

};

#endif //TRIANGLE_H

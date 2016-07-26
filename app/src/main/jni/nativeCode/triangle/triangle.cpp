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

#include "myShader.h"
#include "triangle.h"
#include "myGLFunctions.h"
#include "math.h"

/**
 * Class constructor
 */
Triangle::Triangle() {
    initsDone = false;
}

/**
 * Perform inits and load the triangle's vertices/colors to GLES
 */
void Triangle::PerformGLInits() {

    MyGLInits();

    //specify 3 vertices of the triangle
    static const GLfloat    vertexBufferData[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f
    };

    // Generate a vertex buffer and load the vertices into it
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

    //specify colors of the 3 vertices
    static const GLfloat colorBufferData[] = {
            0.0f, 1.0f, 0.0f, //green
            0.0f, 1.0f, 0.0f, //green
            0.0f, 0.0f, 1.0f  //blue
    };

    // Generate a vertex buffer and load the colors into it
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);

    // shader related setup
    std::string vertexShader    = "shaders/colorTriangle.vsh";
    std::string fragmentShader  = "shaders/colorTriangle.fsh";

    // compile the vertex and fragment shaders, and link them together
    shaderProgramID = LoadShaders(vertexShader, fragmentShader);
    // fetch the locations of "vertexPosition" and "vertexColor" from the shader
    vertexAttribute = GetAttributeLocation(shaderProgramID, "vertexPosition");
    colorAttribute  = GetAttributeLocation(shaderProgramID, "vertexColor");

    // set parameters that control animation of the triangle
    triangleVertexDelta = 0;
    triangleSwapRate = 0.01; // increase this to speedup the animation

    CheckGLError("Triangle::PerformGLInits");
    initsDone = true;
}

/**
 * Render our colorful triangle as per the current vertex coordinates
 */
void Triangle::RenderTriangle() {
    // use the shader
    glUseProgram(shaderProgramID);

    // enable the vertex buffer
    glEnableVertexAttribArray(vertexAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer( vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // enable the color buffer
    glEnableVertexAttribArray(colorAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer( colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

    // Draw the colorful triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(vertexAttribute);
    glDisableVertexAttribArray(colorAttribute);
}

/**
 * Update the vertex positions to animate the triangle
 */
void Triangle::AnimateTriangle() {
    triangleVertexDelta += triangleSwapRate;
    // need to remain in limits of GLES surface coordinates from -1 to +1
    if(triangleVertexDelta > 2.0 || triangleVertexDelta < 0.0) {
        triangleSwapRate = -triangleSwapRate;
    }
    GLfloat vertexBufferData[] = {
            -1.0f+triangleVertexDelta, -1.0f, 0.0f,
            1.0f-triangleVertexDelta, -1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexBufferData), vertexBufferData);
}

/**
 * Render to the display
 */
void Triangle::Render() {

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderTriangle();
    AnimateTriangle();
    CheckGLError("Triangle::Render");

}

/**
 * set the viewport, function is also called when user changes device orientation
 */
void Triangle::SetViewport(int width, int height) {

    glViewport(0, 0, width, height);

    CheckGLError("Triangle::SetViewport");
}

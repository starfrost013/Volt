/* 
    Generic Vertex Shader for 2D Emulated Graphics Hardware

    The gpu_2d shader set is intended for GPUs that do not require hardware acceleration to emulate.
    The primary goal is to simply allow the partial or complete blitting of the display to a texture which is then presented
    to OpenGL at the appropriate time (i.e. when vblank is finished)

    In effect, only a projection matrix is required for this one. Since we do not need to move the camera, we don't need a camera or view matrix.

    Additionally, only a *two-dimensional* vector is needed to hold the coordinates.
*/

#version 430 core

// Input
layout (location = 0) in vec4 vertex;

// Output: The texture coordinate
out vec2 textureCoordinate; 

// texture
uniform mat4 model;
uniform mat4 projection; 

// perform an orthographic projection directly onto 2d space
void main()
{
    textureCoordinate = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

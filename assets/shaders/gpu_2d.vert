/* 
    Generic Vertex Shader for 2D Emulated Graphics Hardware

    The gpu_2d shader set is intended for GPUs that do not require hardware acceleration to emulate.
    The primary goal is to simply allow the partial or complete blitting of the display to a texture which is then presented
    to OpenGL at the appropriate time (i.e. when vblank is finished)

    In effect, only a projection matrix is required for this one. Since we do not need to move the camera, we don't need a camera or view matrix.

    Additionally, only a *two-dimensional* vector is needed to hold the coordinates.
*/

#version 430 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    texCoord = aTexCoord;
}
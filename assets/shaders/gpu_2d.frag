/* 
    Generic Fragment Shader for 2D Emulated Graphics Hardware

    The gpu_2d shader set is intended for GPUs that do not require hardware acceleration to emulate.
    The primary goal is to simply allow the partial or complete blitting of the display to a texture which is then presented
    to OpenGL at the appropriate time (i.e. when vblank is finished)

    In effect, only a projection matrix is required for this one. Since we do not need to move the camera, we don't need a camera or view matrix.

    Additionally, only a *two-dimensional* vector is needed to hold the coordinates.
*/
#version 430 core

in vec2 texCoord;     // frmro vertex shader
out vec4 FragColor;    // Final colour output

uniform sampler2D image; // GPU surface texture

// See what fun processing stuff we can do here!
void main()
{
    FragColor = texture(image, texCoord);
}   
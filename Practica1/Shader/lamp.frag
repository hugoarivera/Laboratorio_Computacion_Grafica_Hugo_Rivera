#version 330 core
out vec4 color;

// Variable para recibir el color desde C++
uniform vec3 lightColor;

void main()
{
    // Aplicamos el color recibido con una opacidad total (1.0f)
    color = vec4(lightColor, 1.0f);
}
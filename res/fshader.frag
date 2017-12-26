#version 330

in vec3 Fcolor;

void main()
{
    gl_FragColor = vec4(Fcolor,1);
}
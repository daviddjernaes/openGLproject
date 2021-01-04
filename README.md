## openGLproject
# Find the robot
A project using light shaders, object files, texture mapping, etc. 

The purpose of this project was for me to gain familiarity with the fundamentals of the Open Graphics Library -- OpenGL.
It is based on a series of tutorials that build all the tools necessary to add vector graphic 3D models, texture maps, and appropriate lighting in the rendered space. The tutorials were developed by Steve Jones from gameinstitute.com   

OpenGL is a set of tools that allows for rendering 2D and 3D vector graphics solely in the GPU, allowing for the CPU to perform other tasks.
In this project the following libraries were used:
* GLFW
* GLEW
* GLM
* stb_image.h, image loader from https://github.com/nothings/stb 

This project includes a handful of features in the final product.
* Multiple point lights as demonstrated in the street lamps. The lamp itself is a 3D model image with texture map that has an associated point light set to approximately the same height as the light bulb, giving the effect of having a soft street lamp quality to them. 
* the fps has a point light that has the feature of acting like a flashlight with an inner and outer cone of separate intensities to mimic how a flashlight's intensity disipates the further from the center it is.
* There is a slight amount of ambient light provided in the overall space. This was used to show the effect of the implemented Blinn-Phong lighting model. This combined with the texture mapping was intended to illustrate the richness of the texture maps on all the objects used in the project. This is especially evident in viewing the robot up close. See the example of this in the video from approximately the middle to the end of the example video.

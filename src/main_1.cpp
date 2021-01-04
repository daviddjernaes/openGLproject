// David Djernaes
// openGL project making use of lighting shaders and 
// various mesh objects to create 3D visual experience
// 

// ConsoleApplicationOpenGL1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "texture2d.h"
#include "Camera.h"
#include "../Mesh.h"

// Global Variables
const char* APP_TITLE = "OpenGL project - explore the space";
int gWindowWidth = 1420;
int gWindowHeight = 768;
GLFWwindow* gWindow = NULL;
bool gWireFrame = false;
bool gFlashlightOn = true;
glm::vec4 gClearColor(0.06f, 0.06f, 0.07f, 1.0f);


FPSCamera fpsCamera(glm::vec3(0.0f, 3.5f, 10.0f)); // , glm::vec3(1.0, 1.0, 1.0));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 10.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;


// Function Prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();


// --------------------
// Main application entry point
// --------------------
int main()
{
 
    if (!initOpenGL())
    {
        std::cerr << "GLFW Initialization Failed...sad" << std::endl;
        return -1;
    }

    glBindVertexArray(0);					// unbind to make sure other code doesn't change it
    
    //shader for the light source
    //ShaderProgram lightShader;
    //lightShader.loadShaders("../shaders/basic.vert", "../shaders/basic.frag");

    //shader for the program enviroment
    ShaderProgram lightingShader;
    lightingShader.loadShaders("../shaders/lighting_dir_spot.vert", "../shaders/lighting_dir_spot.frag");

    // model positions
    glm::vec3 modelPos[] = {
        glm::vec3(-6.5f, 1.0f, -2.5f),  // crate 
        glm::vec3(6.5f, 1.0f, -2.5f),   // woodcrate_diffuse
        glm::vec3(-6.5f, 1.0f, -7.5f),  // woodcrate
        glm::vec3(6.5f, 1.0f, -7.5f),   // crate
        glm::vec3(-12.5f, 1.0f, -8.5f), // crate 
        glm::vec3(12.5f, 1.0f, -8.5f),  // woodcrate_diffuse
        glm::vec3(-12.5f, 1.0f, -10.5f),// woodcrate
        glm::vec3(12.5f, 1.0f, -10.5f), // crate
        glm::vec3(-16.5f, 1.0f, -12.5f),// crate 
        glm::vec3(16.5f, 1.0f, -12.5f), // woodcrate_diffuse
        glm::vec3(-16.5f, 1.0f, -17.5f),// woodcrate
        glm::vec3(16.5f, 1.0f, -17.5f), // crate
        glm::vec3(0.0f, 0.0f, -16.0f),  // robot
        glm::vec3(0.0f, 0.0f, 0.0f),    // floor
        glm::vec3(-6.5f, 1.0f, 2.5f),  // crate 
        glm::vec3(6.5f, 1.0f, 2.5f),   // crate
        glm::vec3(-6.5f, 1.0f, 7.5f),  // crate
        glm::vec3(6.5f, 1.0f, 7.5f),   // crate
        glm::vec3(6.5f, 1.0f, 7.5f),   // crate
        glm::vec3(-12.5f, 1.0f, 8.5f), // crate 
        glm::vec3(12.5f, 1.0f, 8.5f),  // crate
        glm::vec3(12.5f, 1.0f, -0.5f), // woodcrate
        glm::vec3(-16.5f, 1.0f, 12.5f),// woodcrate 
        glm::vec3(16.5f, 1.0f, 12.5f), // woodcrate_diffuse
        glm::vec3(-16.5f, 1.0f, 17.5f),// woodcrate
        glm::vec3(16.5f, 1.0f, 17.5f), // woodcrate
        glm::vec3(-16.5f, 1.0f, 17.5f),// woodcrate
        glm::vec3(16.5f, 1.0f, 17.5f), // woodcrate
        glm::vec3(-37.5f, 0.0f, -30.0f),// lamp post LL1
        glm::vec3(-37.5f, 0.0f, -20.0f),// lamp post LL2
        glm::vec3(-37.5f, 0.0f, -10.0f),// lamp post LL3
        glm::vec3(-37.5f, 0.0f, 0.0f),  // lamp post LL4
        glm::vec3(-7.5f, 0.0f, -18.0f),	// lamp post L1
        glm::vec3(7.5f, 0.0f, -18.0f),	// lamp post R1
        glm::vec3(55.5f, 0.0f, -30.0f),	// lamp post RR1
        glm::vec3(55.5f, 0.0f, -20.0f),	// lamp post RR2
        glm::vec3(55.5f, 0.0f, -10.0f),	// lamp post RR3
        glm::vec3(55.5f, 0.0f, 0.0f),	// lamp post RR4
        glm::vec3(-37.5f, 0.0f, 10.0f),	// lamp post BLL1
        glm::vec3(-37.5f, 0.0f, 20.0f),	// lamp post BLL2
        glm::vec3(-37.5f, 0.0f, 30.0f),	// lamp post BLL3
        glm::vec3(-37.5f, 0.0f, 40.0f),	// lamp post BLL4
        glm::vec3(55.5f, 0.0f, 10.0f),	// lamp post BRR1
        glm::vec3(55.5f, 0.0f, 20.0f),	// lamp post BRR2
        glm::vec3(55.5f, 0.0f, 30.0f),	// lamp post BRR3
        glm::vec3(55.5f, 0.0f, 40.0f),	// lamp post BRR4
        glm::vec3(10.0f, 9.0f, 10.0f)  // warehouse
    };


    //model scaling
    glm::vec3 modelScale[] = {
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // crate 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(1.0f, 1.0f, 1.0f),    // robot***
        glm::vec3(25.0f, 0.0f, 25.0f),  // floor***
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate 
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate 
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate
        glm::vec3(0.9f, 0.9f, 0.9f),    // crate
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse 
        glm::vec3(0.7f, 0.7f, 0.7f),    // woodcrate_diffuse
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post LL1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post LL2
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post LL3
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post LL4
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post L1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post R1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post RR1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post RR2
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post RR3
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post RR4
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BLL1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BLL2
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BLL3
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BLL4
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BRR1
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BRR2
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BRR3
        glm::vec3(2.0f, 2.0f, 2.0f),	// lamp post BRR4
        glm::vec3(0.25f, 0.25f, 0.25f)		// warehouse 
    };

    float lightHeight = 7.6f;

    //Point light positions  //  args: (l/r, up/down, forward/back)
    glm::vec3 pointLightPos[18] = {
        glm::vec3(-37.0f, lightHeight, -30.0f),      // lamp post LL1
        glm::vec3(-37.0f, lightHeight, -20.0f),     // lamp post LL2
        glm::vec3(-37.0f, lightHeight, -10.0f),    // lamp post LL3
        glm::vec3(-37.0f, lightHeight, 0.0f),    // lamp post LL4
        glm::vec3(-7.0f, lightHeight, -18.0f),       // lamp post L1 keep      
        glm::vec3(7.0f, lightHeight, -18.0f),        // lamp post R1  keep
        glm::vec3(55.0f, lightHeight, -30.0f),       // lamp post RR1
        glm::vec3(55.0f, lightHeight, -20.0f),      // lamp post RR2
        glm::vec3(55.0f, lightHeight, -10.0f),     // lamp post RR3
        glm::vec3(55.0f, lightHeight, 0.0f),     // lamp post RR4
        glm::vec3(-37.0f,lightHeight, 10.0f ),     // lamp post BLL1
        glm::vec3(-37.0f,lightHeight, 20.0f),     // lamp post BLL2
        glm::vec3(-37.0f,lightHeight, 30.0f),     // lamp post BLL3
        glm::vec3(-37.0f,lightHeight, 40.0f),     // lamp post BLL4
        glm::vec3(55.0f, lightHeight, 10.0f),       // lamp post BRR1
        glm::vec3(55.0f, lightHeight, 20.0f),      // lamp post BRR2
        glm::vec3(55.0f, lightHeight, 30.0f),      // lamp post BRR3
        glm::vec3(55.0f, lightHeight, 40.0f)       // lamp post BRR4
    };                                      

    // load  Meshes
    const int numModels = 47;
    Mesh mesh[numModels];
    Texture2d texture[numModels];
    
    mesh[0].loadOBJ("../models/crate.obj");
    mesh[1].loadOBJ("../models/woodcrate.obj");
    mesh[2].loadOBJ("../models/woodcrate.obj");
    mesh[3].loadOBJ("../models/crate.obj");    
    mesh[4].loadOBJ("../models/crate.obj");
    mesh[5].loadOBJ("../models/woodcrate.obj");
    mesh[6].loadOBJ("../models/woodcrate.obj");
    mesh[7].loadOBJ("../models/crate.obj");
    mesh[8].loadOBJ("../models/crate.obj");
    mesh[9].loadOBJ("../models/woodcrate.obj");
    mesh[10].loadOBJ("../models/woodcrate.obj");
    mesh[11].loadOBJ("../models/crate.obj");
    mesh[12].loadOBJ("../models/robot.obj");
    mesh[13].loadOBJ("../models/floor.obj");
    mesh[14].loadOBJ("../models/crate.obj");
    mesh[15].loadOBJ("../models/woodcrate.obj");
    mesh[16].loadOBJ("../models/woodcrate.obj");
    mesh[17].loadOBJ("../models/crate.obj");     
    mesh[18].loadOBJ("../models/crate.obj");
    mesh[19].loadOBJ("../models/woodcrate.obj");
    mesh[20].loadOBJ("../models/woodcrate.obj");
    mesh[21].loadOBJ("../models/crate.obj"); 
    mesh[22].loadOBJ("../models/crate.obj");
    mesh[23].loadOBJ("../models/woodcrate.obj");
    mesh[24].loadOBJ("../models/woodcrate.obj");
    mesh[25].loadOBJ("../models/woodcrate.obj");
    mesh[26].loadOBJ("../models/woodcrate.obj");
    mesh[27].loadOBJ("../models/woodcrate.obj");
    mesh[28].loadOBJ("../models/lampPost.obj"); //lamp post
    mesh[29].loadOBJ("../models/lampPost.obj"); 
    mesh[30].loadOBJ("../models/lampPost.obj");
    mesh[31].loadOBJ("../models/lampPost.obj");
    mesh[32].loadOBJ("../models/lampPost.obj");
    mesh[33].loadOBJ("../models/lampPost.obj"); 
    mesh[34].loadOBJ("../models/lampPost.obj");
    mesh[35].loadOBJ("../models/lampPost.obj");
    mesh[36].loadOBJ("../models/lampPost.obj");
    mesh[37].loadOBJ("../models/lampPost.obj"); 
    mesh[38].loadOBJ("../models/lampPost.obj");
    mesh[39].loadOBJ("../models/lampPost.obj");
    mesh[40].loadOBJ("../models/lampPost.obj");
    mesh[41].loadOBJ("../models/lampPost.obj"); 
    mesh[42].loadOBJ("../models/lampPost.obj");
    mesh[43].loadOBJ("../models/lampPost.obj");
    mesh[44].loadOBJ("../models/lampPost.obj");
    mesh[45].loadOBJ("../models/lampPost.obj"); 
    mesh[46].loadOBJ("../models/warehouse.obj");


    texture[0].loadTexture("../textures/crate.jpg", true);
    texture[1].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[2].loadTexture("../textures/woodcrate_diffuse.jpg", true); 
    texture[3].loadTexture("../textures/crate.jpg", true);
    texture[4].loadTexture("../textures/crate.jpg", true);
    texture[5].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[6].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[7].loadTexture("../textures/crate.jpg", true);
    texture[8].loadTexture("../textures/crate.jpg", true);
    texture[9].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[10].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[11].loadTexture("../textures/crate.jpg", true);
    texture[12].loadTexture("../textures/robot_diffuse.jpg", true);
    texture[13].loadTexture("../textures/tile_floor.jpg", true);
    texture[14].loadTexture("../textures/crate.jpg", true); // 
    texture[15].loadTexture("../textures/crate.jpg", true);
    texture[16].loadTexture("../textures/crate.jpg", true);
    texture[17].loadTexture("../textures/crate.jpg", true); // 4
    texture[18].loadTexture("../textures/crate.jpg", true);
    texture[19].loadTexture("../textures/crate.jpg", true);
    texture[20].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[21].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[22].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[23].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[24].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[25].loadTexture("../textures/woodcrate_diffuse.jpg", true); 
    texture[26].loadTexture("../textures/woodcrate_diffuse.jpg", true);
    texture[27].loadTexture("../textures/woodcrate_diffuse.jpg", true); 
    texture[28].loadTexture("../textures/lamp_post_diffuse.png", true); // lamp posts
    texture[29].loadTexture("../textures/lamp_post_diffuse.png", true); 
    texture[30].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[31].loadTexture("../textures/lamp_post_diffuse.png", true); //4
    texture[32].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[33].loadTexture("../textures/lamp_post_diffuse.png", true); 
    texture[34].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[35].loadTexture("../textures/lamp_post_diffuse.png", true); //8
    texture[36].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[37].loadTexture("../textures/lamp_post_diffuse.png", true); 
    texture[38].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[39].loadTexture("../textures/lamp_post_diffuse.png", true); //12
    texture[40].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[41].loadTexture("../textures/lamp_post_diffuse.png", true); 
    texture[42].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[43].loadTexture("../textures/lamp_post_diffuse.png", true); //16
    texture[44].loadTexture("../textures/lamp_post_diffuse.png", true);
    texture[45].loadTexture("../textures/lamp_post_diffuse.png", true); //18
    texture[46].loadTexture("../textures/warehouse_diffuse.jpg", true);

    double lastTime = glfwGetTime();

    // main loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;


        // Poll for and process events
        glfwPollEvents();
        update(deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);

        view = fpsCamera.getViewMatrix();

        projection = glm::perspective(
            glm::radians(fpsCamera.getFOV()), 
            (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

        glm::vec3 viewPos;
        viewPos.x = fpsCamera.getPosition().x;
        viewPos.y = fpsCamera.getPosition().y;
        viewPos.z = fpsCamera.getPosition().z;

       
        lightingShader.use();
        // Pass the matrices to the shader
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("model", glm::mat4(1.0));  // do not need to translate the models so just send the identity matrix
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("viewPos", viewPos);

        // Directional light
        lightingShader.setUniform("sunLight.direction", glm::vec3(0.0f, -0.9f, -0.17f));
        lightingShader.setUniform("sunLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f)); // was 0.1
        lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));		// 0.1 is dark,
        lightingShader.setUniform("sunLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));


        // lamp posts... 18 for this build
        // create string = "pointlights[index].structMemeber" for lightingShader uniform call
        string pointLights = "pointLights[";
        string ambient = "].ambient";
        string diffuse = "].diffuse";
        string specular = "].specular";
        string position = "].position";
        string constant = "].constant";
        string linear = "].linear";
        string exponent = "].exponent";

        glm::vec3 ambient_values = glm::vec3(0.8f, 0.8f, 0.8f);
        glm::vec3 diffuse_values = glm::vec3(0.9373f, 0.9216f, 0.8417f); // warm light
        glm::vec3 specular_values = glm::vec3(1.0f, 1.0f, 1.0f);


        for (int i = 0; i < 18; i++) {
            string index = std::to_string(i);
            const GLchar* temp_ambient = pointLights.append(index + ambient).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_diffuse = pointLights.append(index + diffuse).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_specular = pointLights.append(index + specular).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_position = pointLights.append(index + position).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_constant = pointLights.append(index + constant).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_linear = pointLights.append(index + linear).c_str();
            pointLights = "pointLights[";
            const GLchar* temp_exponent = pointLights.append(index + exponent).c_str();
            pointLights = "pointLights[";

            // do this for each line then 
            lightingShader.setUniform(temp_ambient, glm::vec3(0.8f, 0.8f, 0.8f));
            lightingShader.setUniform(temp_diffuse, glm::vec3(0.9373f, 0.9216f, 0.8417f));
            lightingShader.setUniform(temp_specular, glm::vec3(1.0f, 1.0f, 1.0f));
            lightingShader.setUniform(temp_position, pointLightPos[i]);
            lightingShader.setUniform(temp_constant, 1.0f);
            lightingShader.setUniform(temp_linear, 0.22f);
            lightingShader.setUniform(temp_exponent, 0.2f);

        }
        // end of lamp posts
       


        // Spot light
        glm::vec3 spotlightPos = fpsCamera.getPosition();

        // offset the flash light down a little
        spotlightPos.y -= 0.5f;

        lightingShader.setUniform("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("spotLight.position", spotlightPos);
        lightingShader.setUniform("spotLight.direction", fpsCamera.getLook());
        lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(25.0f)));
        lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(30.0f)));
        lightingShader.setUniform("spotLight.constant", 1.0f);
        lightingShader.setUniform("spotLight.linear", 0.07f);
        lightingShader.setUniform("spotLight.exponent", 0.017f);
        lightingShader.setUniform("spotLight.on", gFlashlightOn);

        // render the scene
        for (int i = 0; i < numModels; i++)
        {
            model = glm::translate(
                glm::mat4(), 
                modelPos[i]) * glm::scale(glm::mat4(), 
                modelScale[i]);
            lightingShader.setUniform("model", model);

            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
            lightingShader.setUniform("material.shininess", 32.0f);

            texture[i].bind(0);
            mesh[i].draw();
            texture[i].unbind(0);
        }

       
        // Swap front and back buffers
        glfwSwapBuffers(gWindow);
        lastTime = currentTime;
    }

    
    // clean up
    glfwTerminate();

    return 0;
}


bool initOpenGL() 
{
    // Intialize GLFW 
    // GLFW is configured.  Must be called before calling any GLFW functions
    if (!glfwInit()) 
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    //  configured window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // forward compatible with newer versions of OpenGL as they become 
    // available but not backward compatible (it will not run on devices 
    // that do not support OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    //const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);


    // initialized window
    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
    if (gWindow == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed" << std::endl;
        return false;
    }

    // Set the required callback functions 
    glfwSetKeyCallback(gWindow, glfw_onKey);
    //glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);
    glfwSetScrollCallback(gWindow, glfw_onMouseScroll);
    glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

    // hides and grabs cursor, unlimited movement
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

    //glClearColor(0.23f, 0.38f, 0.48f, 1.0f);
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        gWireFrame = !gWireFrame;
        if (gWireFrame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        gFlashlightOn = !gFlashlightOn;
    }
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
    gWindowHeight = height;
    gWindowWidth = width;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}



// called by GLFW when the mouse wheel is rotated
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.setFOV((float)fov);
}

// update stuff every frame
void update(double elapsedTime)
{
    // camera orientation
    double mouseX, mouseY;

    // get the current mouse cursor position delta
    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    // rotate the camera the difference in mouse distance from the center of screen
    // multiply the delta by a speed scalar
    fpsCamera.rotate(
        (float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, 
        (float)(gWindowHeight / 2.0 - mouseY)* MOUSE_SENSITIVITY);

    //clamp mouse cursor to center of screen
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    // camera FPS movement

    // Forward / Backward
    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());


    // strafe left / right
    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
    else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());


    // up / down
    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
    else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());

      
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second, and also the average time it takes
// to render one frame.  These stats are appended to the window caption bar.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();  // returns number of seconds since GLFW started, as a double

    elapsedSeconds = currentSeconds - previousSeconds;
    // limit text updates to approx 4 times per seconds
    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << APP_TITLE << "  "
            << "FPS:  " << fps << "  "
            << "Frame Time: " << msPerFrame << " (ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }
    frameCount++;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

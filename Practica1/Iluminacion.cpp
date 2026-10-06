//Practica 8
//Rivera Rodriguez Hugo Abraham
//Fecha de entrega: 05/10/2026
//Número de cuenta: 320291623

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
glm::vec3 lightPos2(-2.0f, 2.0f, -1.0f);
float movelightPos = 0.0f;
float movelightPos2 = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rivera Rodriguez Hugo Abraham", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model Hombre_nieve((char*)"Models/Hombre_nieve.obj");
    Model mountain((char*)"Models/MontanaP6.obj");
    Model snowboard((char*)"Models/Snowboard.obj");
    Model telesilla((char*)"Models/Telesilla.obj");
    Model valla((char*)"Models/Valla.obj");
    Model ski((char*)"Models/ski.obj");
    Model sol((char*)"Models/sol.obj");
    Model luna((char*)"Models/luna.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float cableVertices[] = {
          14.0f,  13.5f, -3.0f,
         -10.0f,  -2.5f, -3.0f
    };

    GLuint cableVAO, cableVBO;
    glGenVertexArrays(1, &cableVAO);
    glGenBuffers(1, &cableVBO);

    glBindVertexArray(cableVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cableVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cableVertices), cableVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        //Se calcula la orbita del sol y la luna
        float timeValue = glfwGetTime() * 0.5f;
        float radius = 8.0f;

        lightPos.x = cos(timeValue) * radius;
        lightPos.y = sin(timeValue) * radius;
        lightPos.z = 0.0f;

        lightPos2.x = -cos(timeValue) * radius;
        lightPos2.y = -sin(timeValue) * radius;
        lightPos2.z = 0.0f;

        //Aqui se hace un cambio de color en el entorno para poder visualizar la altura del sol y la luna
        float skyFactor = (sin(timeValue) + 1.0f) / 2.0f;
        glm::vec3 nightColor(0.05f, 0.05f, 0.1f);
        glm::vec3 dayColor(0.5f, 0.7f, 1.0f);
        glm::vec3 skyColor = glm::mix(nightColor, dayColor, skyFactor);

        glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();

        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x + movelightPos, lightPos.y + movelightPos, lightPos.z + movelightPos);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), lightPos2.x + movelightPos2, lightPos2.y + movelightPos2, lightPos2.z + movelightPos2);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        //Intensidad del sol y la luna
        float sunIntensity = glm::clamp(sin(timeValue), 0.0f, 1.0f);
        float moonIntensity = glm::clamp(-sin(timeValue), 0.0f, 1.0f);
        //Propiedades del sol
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.5f * sunIntensity, 0.3f * sunIntensity, 0.1f * sunIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f * sunIntensity, 0.7f * sunIntensity, 0.2f * sunIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f * sunIntensity, 0.8f * sunIntensity, 0.4f * sunIntensity);
        //Propiedades de la luna
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.05f * moonIntensity, 0.05f * moonIntensity, 0.2f * moonIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.1f * moonIntensity, 0.2f * moonIntensity, 0.6f * moonIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.2f * moonIntensity, 0.3f * moonIntensity, 0.8f * moonIntensity);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.2f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.6f, 0.6f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);

        glBindVertexArray(VAO);



        // Draw the loaded model
        glm::mat4 modelDog(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.9f, 1.08f, 0.8f)); // Posición Derecha
        modelDog = glm::scale(modelDog, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        red_dog.Draw(lightingShader);

        //Segundo modelo
        glm::mat4 modelSnowman(1);
        modelSnowman = glm::translate(modelSnowman, glm::vec3(-1.0f, -0.7f, 2.0f));
        modelSnowman = glm::scale(modelSnowman, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSnowman));
        Hombre_nieve.Draw(lightingShader);

        //Tercer modelo
        glm::mat4 modelMountain(1);
        modelMountain = glm::translate(modelMountain, glm::vec3(0.0f, -2.0f, 0.0f));
        modelMountain = glm::scale(modelMountain, glm::vec3(0.004f, 0.004f, 0.004f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMountain));
        mountain.Draw(lightingShader);

        //Tabla de snowboard
        glm::mat4 modeSnowboard(1);
        modeSnowboard = glm::translate(modeSnowboard, glm::vec3(1.1f, 0.48f, 0.8f));
        modeSnowboard = glm::scale(modeSnowboard, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeSnowboard));
        snowboard.Draw(lightingShader);

        //Telesilla 1
        glm::mat4 modeTelesilla(1);
        modeTelesilla = glm::translate(modeTelesilla, glm::vec3(2.0f, 3.0f, -3.0f));
        modeTelesilla = glm::scale(modeTelesilla, glm::vec3(0.008f, 0.008f, 0.008f));
        modeTelesilla = glm::rotate(modeTelesilla, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeTelesilla));
        telesilla.Draw(lightingShader);

        //Telesilla 2
        glm::mat4 modeTelesilla2(1);
        modeTelesilla2 = glm::translate(modeTelesilla2, glm::vec3(-1.0f, 1.0f, -3.0f));
        modeTelesilla2 = glm::scale(modeTelesilla2, glm::vec3(0.008f, 0.008f, 0.008f));
        modeTelesilla2 = glm::rotate(modeTelesilla2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeTelesilla2));
        telesilla.Draw(lightingShader);

        //Valla de madera
        glm::mat4 modeValla(1);
        modeValla = glm::translate(modeValla, glm::vec3(0.08f, 0.1f, 1.0f));
        modeValla = glm::scale(modeValla, glm::vec3(0.1f, 0.1f, 0.1f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeValla));
        valla.Draw(lightingShader);

        //Skis
        glm::mat4 modeski(1);
        modeski = glm::translate(modeski, glm::vec3(0.2f, 0.25f, 0.4f));
        modeski = glm::scale(modeski, glm::vec3(0.01f, 0.03f, 0.03f));
        modeski = glm::rotate(modeski, glm::radians(285.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeski));
        ski.Draw(lightingShader);

        /*glDrawArrays(GL_TRIANGLES, 0, 36);*/
        

        glBindVertexArray(0);

        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //Sol
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 1.0f, 0.5f, 0.0f);
        glm::mat4 modelLamp1 = glm::mat4(1.0f);
        modelLamp1 = glm::translate(modelLamp1, lightPos + glm::vec3(movelightPos));
        modelLamp1 = glm::scale(modelLamp1, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp1));
        sol.Draw(lampshader);

        //Luna  
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 0.9f, 0.9f, 1.0f);
        glm::mat4 modelLamp2 = glm::mat4(1.0f);
        modelLamp2 = glm::translate(modelLamp2, lightPos2 + glm::vec3(movelightPos2));
        modelLamp2 = glm::scale(modelLamp2, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp2));
        luna.Draw(lampshader);

        //Este es el modelo para el cable de la telesilla
        glUniform3f(glGetUniformLocation(lampshader.Program, "lightColor"), 0.2f, 0.2f, 0.2f);
        glm::mat4 modelCable(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCable));
        glLineWidth(3.0f);
        glBindVertexArray(cableVAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &cableVAO);
    glDeleteBuffers(1, &cableVBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
       
        movelightPos += 0.1f;
    }

    if (keys[GLFW_KEY_L])
    {
        
        movelightPos -= 0.1f;
    }

    if (keys[GLFW_KEY_I])
    {
        movelightPos2 += 0.1f;
    }
    if (keys[GLFW_KEY_K])
    {
        movelightPos2 -= 0.1f;
    }


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}



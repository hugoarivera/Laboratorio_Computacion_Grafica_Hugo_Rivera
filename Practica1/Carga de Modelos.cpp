// Practica 6
// Rivera Rodriguez Hugo Abraham
// Fecha de entrega 15/09/2026
// Número de cuenta: 320291623


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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Rivera Rodriguez Hugo Abraham", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load mo(dels
    Model dog((char*)"Models/RedDog.obj");
    Model mountain((char*)"Models/MontanaP6.obj");
    Model snowboard((char*)"Models/Snowboard.obj");
    Model telesilla((char*)"Models/Telesilla.obj");
    Model valla((char*)"Models/Valla.obj");
    Model hombre_nieve((char*)"Models/Hombre_nieve.obj");
    Model ski((char*)"Models/ski.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    //Esta parte del codigo lo usare para colocar cables en las telesillas
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

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        //Aqui se carga la montaña
        glm::mat4 modelMountain(1);
        modelMountain = glm::translate(modelMountain, glm::vec3(0.0f, -2.0f, 0.0f));
        modelMountain = glm::scale(modelMountain, glm::vec3(0.004f, 0.004f, 0.004f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMountain));
        mountain.Draw(shader);

        //Aqui se carga el snowboard
        glm::mat4 modeSnowboard(1);
        modeSnowboard = glm::translate(modeSnowboard, glm::vec3(1.1f, 0.48f, 0.8f));
        modeSnowboard = glm::scale(modeSnowboard, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeSnowboard));
        snowboard.Draw(shader);

        //Aqui se carga la telesilla
        glm::mat4 modeTelesilla(1);
        modeTelesilla = glm::translate(modeTelesilla, glm::vec3(2.0f, 3.0f, -3.0f));
        modeTelesilla = glm::scale(modeTelesilla, glm::vec3(0.008f, 0.008f, 0.008f));
        modeTelesilla = glm::rotate(modeTelesilla, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeTelesilla));
        telesilla.Draw(shader);

        //Aqui cargare otro modelo de telesilla
        glm::mat4 modeTelesilla2(1);
        modeTelesilla2 = glm::translate(modeTelesilla2, glm::vec3(-1.0f, 1.0f, -3.0f));
        modeTelesilla2 = glm::scale(modeTelesilla2, glm::vec3(0.008f, 0.008f, 0.008f));
        modeTelesilla2 = glm::rotate(modeTelesilla2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeTelesilla2));
        telesilla.Draw(shader);

        //Aqui cargue la valla de madera
        glm::mat4 modeValla(1);
        modeValla = glm::translate(modeValla, glm::vec3(0.08f, 0.1f, 1.0f));
        modeValla = glm::scale(modeValla, glm::vec3(0.1f, 0.1f, 0.1f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeValla));
        valla.Draw(shader);

        //Aqui se carga el hombre de nieve
        glm::mat4 modehombre_nieve(1);
        modehombre_nieve = glm::translate(modehombre_nieve, glm::vec3(-1.0f, -0.7f, 2.0f));
        modehombre_nieve = glm::scale(modehombre_nieve, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modehombre_nieve));
        hombre_nieve.Draw(shader);

        //Aqui se carga el ski
        glm::mat4 modeski(1);
        modeski = glm::translate(modeski, glm::vec3(0.2f, 0.25f, 0.4f));
        modeski = glm::scale(modeski, glm::vec3(0.01f, 0.03f, 0.03f));
        modeski = glm::rotate(modeski, glm::radians(285.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modeski));
        ski.Draw(shader);

        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //dog.Draw(shader);

        //Aqui se carga al perrito
        model = glm::translate(model, glm::vec3(0.9f, 1.08f, 0.8f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        //Aqui coloque el cable del teleferico
        glm::mat4 modelCable(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCable));
        glLineWidth(3.0f);
        glBindVertexArray(cableVAO);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}


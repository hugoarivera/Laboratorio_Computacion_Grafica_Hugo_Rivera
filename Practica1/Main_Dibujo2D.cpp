//Practica 2
//Rivera Rodriguez Hugo Abraham
//Fecha de entrega 24 de Agosto 2026
//Número de cuenta 320291623

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificacin de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificacin de errores de creacion  ventana
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificacin de errores de inicializacin de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	//Aqui cree los vertices que se usaran para crear al gato
	float vertices[] = {
		 0.0f, -0.5f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado en la parte de abajo
		 0.6f, -0.1f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado a la derecha en la cara del gato
		 0.5f,  0.6f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado en la oreja de la derecha
		 0.2f,  0.3f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado abajo de la oreja derecha
		-0.2f,  0.3f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado abajo de la oreja izquierda
		-0.5f,  0.6f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado en la oreja de la izquierda
		-0.6f, -0.1f, 0.0f,    1.0f,0.8f,0.0f, //Es el vertice ubicado a la izquierda en la cara del gato
		-0.2f,  0.1f, 0.0f,    0.0f,0.0f,0.0f, //Sera el vertice que se ubica en el ojo izquierdo
		 0.2f,  0.1f, 0.0f,    0.0f,0.0f,0.0f, //Sera el vertice que se ubica en el ojo derecho
		 0.0f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Sera el vertice que se ubica en la nariz
		-0.2f,  0.0f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del primer bigote de lado izquierdo
		-0.4f,  0.05f,0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del primer bigote de lado izquierdo
		-0.2f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del segundo bigote de lado izquierdo
		-0.4f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del segundo bigote de lado izquierdo
		-0.2f, -0.2f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del tercer bigote de lado izquierdo
		-0.4f, -0.25f,0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del tercer bigote de lado izquierdo
		 0.2f,  0.0f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del primer bigote de lado derecho
		 0.4f,  0.05f,0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del primer bigote de lado derecho
		 0.2f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del segundo bigote de lado derecho
		 0.4f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del segundo bigote de lado derecho
		 0.2f, -0.2f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice del inicio del tercer bigote de lado derecho
		 0.4f, -0.25f,0.0f,    0.0f,0.0f,0.0f, //Es el vertice del final del tercer bigote de lado derecho
		 0.0f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice que se encuentra al inicio de la boca de lado izquierdo
		-0.1f, -0.2f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice que se encuentra al final de la boca de lado izquierdo
		 0.0f, -0.1f, 0.0f,    0.0f,0.0f,0.0f, //Es el vertice que se encuentra al inicio de la boca de lado derecho
		 0.1f, -0.2f, 0.0f,    0.0f,0.0f,0.0f  //Es el vertice que se encuentra al final de la boca de lado derecho
	};
	unsigned int indices[] = {  // note that we start from 0!
		0,1,6, //Aqui estoy uniendo los puntos con triangulos para poder formar la cara del gato
		1,3,6,
		6,3,4,
		1,2,3,
		6,4,5
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);
		
		//Aqui ya mando a imprimir todos mis puntos unidos por triangulos
		glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, 0);

		//Aqui imprimire los puntos que seran los ojos y la nariz
		glPointSize(10.0f);
		//Comienzo desde el vertice 7 y dibuje 3 puntos
		glDrawArrays(GL_POINTS, 7, 3);
		//Aqui se dibujan los bigotes y la parte de la boca con lineas
		glDrawArrays(GL_LINES, 10, 16);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
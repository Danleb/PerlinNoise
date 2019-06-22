#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Noise generator
#include "PerlinNoise/PerlinNoise.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//Размеры окна
const GLuint WIDTH = 1000, HEIGHT = 800;

int main()
{
#pragma region Initializing

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	//Инициализация GLFW
	glfwInit();

	//Настройка 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Создание окна
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Perlin noise generator", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Привязка текущего контекста к созданному окну
	glfwMakeContextCurrent(window);
	//Привязка функции обратного вызова к нажатиям кнопок
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	//Инициализация GLEW, чтобы настроить указатели функций OpenGL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

#pragma endregion

#pragma region Texture creating

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
		//Position  Texcoords
	   -1,  1,		0, 1,
		1,  1,		1, 1,
	   -1, -1,		0, 0,
		1, -1,		1, 0,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);
	GLuint elements[] = {
		0, 1, 2,
		1, 2, 3
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	const char* vertexSource = {
		#include"generated/vertex.glsl"
	};
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
	}

	const char* fragmentSource = {
		#include"generated/fragment.glsl"
	};
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint fragStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragStatus);
	if (fragStatus == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint positionAttrib = glGetAttribLocation(shaderProgram, "positionIn");
	glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(positionAttrib);

	GLint texcoordAttribute = glGetAttribLocation(shaderProgram, "texcoordIn");
	glEnableVertexAttribArray(texcoordAttribute);
	glVertexAttribPointer(texcoordAttribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	//GLuint vao;
	//glGenVertexArrays(1, &vao);

#pragma endregion

	//текстура шума
	GLuint noiseTex;
	glGenTextures(1, &noiseTex);
	glBindTexture(GL_TEXTURE_2D, noiseTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const int textureWidth = 500;
	const int textureHeight = 500;

	Effects::PerlinNoise* perlin = new Effects::PerlinNoise();

#define dyn

#ifdef  dyn
	float* noiseImage = new float[textureHeight * textureWidth * 3];
	const float coef = 5;
	for (size_t u = 0; u < textureHeight; u++)
	{
		for (size_t i = 0; i < textureWidth; i++)
		{
			float x = (float)i / (float)textureWidth;
			float y = (float)u / (float)textureHeight;
			x *= coef;
			y *= coef;
			float val = perlin->GetNoise(x, y, 5, .5f);
			val = (val + 1) / 2;
			float b = val;
			//char b = static_cast<char>(val * 255);

			noiseImage[3 * u * textureWidth + 3 * i + 0] = b;
			noiseImage[3 * u * textureWidth + 3 * i + 1] = b;
			noiseImage[3 * u * textureWidth + 3 * i + 2] = b;
		}
	}
#else
	float noiseImage[] = {
		0,1,0,		    0,1,0,	 0,0,1,
		1,0,1,		    1,0,1,	 0,1,0,
		0.5,0.5,0.5,    1,1,1,	 0,0,1
	};
#endif



	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_FLOAT, noiseImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Игровой цикл
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);




		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);	
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Меняем буферы местами
		glfwSwapBuffers(window);

		//Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions		
		glfwPollEvents();
	}

	//delete[] noiseImage;

	glDeleteTextures(1, &noiseTex);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);

	//Завершение GLFW, очистка ресурсов, захваченных GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//Вызывается, когда какая-либо клавиша нажимается/отпускается
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void DrawPerlinNoise(float* image, int width, int height, float time)
{
	/*Effects::PerlinNoise noiseGenerator(0, 0);

	for (size_t i = 0; i < height; i++)
	{
		for (size_t u = 0; u < width; u++)
		{
			auto index = i * width + u;

			image[index] = noiseGenerator.GetNoise(0, 0);
		}
	}*/
}
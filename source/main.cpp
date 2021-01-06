#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer/shader.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void movementUpdate();

const GLuint WIDTH = 800, HEIGHT = 600;

// TODO: copy shaders in CMake
const GLchar* vertexShaderPath = "../shaders/shader.vs";
const GLchar* fragmentShaderPath = "../shaders/shader.frag";

// TODO: don't use globals
double lastFrameTime = 0;
float deltaTime = 0;
glm::vec3 position;

// Input state
bool isAPressed = false;
bool isDPressed = false;
bool isSPressed = false;
bool isWPressed = false;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Shader shader(vertexShaderPath, fragmentShaderPath);

    GLfloat vertices[] = {
       0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
       0.5f, -0.5f, 0.0f,  1.0f, 1.5f, 1.0f, 1.0f,  1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
      -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    };
    GLuint indices[] = {
            0, 1, 2,
            1, 2, 3
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Vertex color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    int w, h, c;
    unsigned char *image = stbi_load("../resources/test.png", &w, &h, &c, 4);

    GLuint texture;
    glGenTextures(2, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint filtering = GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.Program, "image"), 0);

    GLuint modelUniform = glGetUniformLocation(shader.Program, "model");
    GLuint viewUniform = glGetUniformLocation(shader.Program, "view");
    GLuint projectionUniform = glGetUniformLocation(shader.Program, "projection");

    position = glm::vec3(1.0f, 2.0f, 0);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        movementUpdate();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //view = glm::rotate(view, glm::radians(-(float)glfwGetTime() * 25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 projection;
        projection = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, 0.1f, 10.0f );
        //trans = glm::rotate(trans,(GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Movement
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        isAPressed = true;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        isDPressed = true;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        isSPressed = true;
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        isWPressed = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        isAPressed = false;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        isDPressed = false;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        isSPressed = false;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        isWPressed = false;
}

void movementUpdate()
{
    const float speed = 1.0f;
    if (isAPressed)
        position += glm::vec3(-speed * deltaTime, 0, 0);
    if (isDPressed)
        position += glm::vec3(speed * deltaTime, 0, 0);
    if (isSPressed)
        position += glm::vec3(0, -speed * deltaTime, 0);
    if (isWPressed)
        position += glm::vec3(0, speed * deltaTime, 0);
}
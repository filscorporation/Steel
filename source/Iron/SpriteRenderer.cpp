#include "SpriteRenderer.h"
#include "Shader.h"
#include "Input.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

// TODO: copy shaders in CMake
const GLchar* vertexShaderPath = "../shaders/shader.vs";
const GLchar* fragmentShaderPath = "../shaders/shader.frag";

Shader* shader;
GLuint texture;
GLuint VBO, VAO, EBO;

glm::vec3 position;

void MovementUpdate();

SpriteRenderer::SpriteRenderer()
{
    // TODO: make common rendering object

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

    glGenTextures(2, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint filtering = GL_NEAREST;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    position = glm::vec3(1.0f, 2.0f, 0);

    shader = new Shader(vertexShaderPath, fragmentShaderPath);
    shader->Use();
    glUniform1i(glGetUniformLocation(shader->Program, "image"), 0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SpriteRenderer::OnRender()
{
    GLuint modelUniform = glGetUniformLocation(shader->Program, "model");
    GLuint viewUniform = glGetUniformLocation(shader->Program, "view");
    GLuint projectionUniform = glGetUniformLocation(shader->Program, "projection");

    MovementUpdate();

    // Render
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
}

void MovementUpdate()
{
    const float speed = 1.0f;
    if (Input::IsKeyPressed(A))
        position += glm::vec3(-speed * 0.01f, 0, 0);
    if (Input::IsKeyPressed(D))
        position += glm::vec3(speed * 0.01f, 0, 0);
    if (Input::IsKeyPressed(S))
        position += glm::vec3(0, -speed * 0.01f, 0);
    if (Input::IsKeyPressed(W))
        position += glm::vec3(0, speed * 0.01f, 0);
}
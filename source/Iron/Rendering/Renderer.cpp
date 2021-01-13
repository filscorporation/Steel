#include "Renderer.h"
#include "SpriteRenderer.h"
#include "Shader.h"
#include "../Core/Application.h"
#include "../Core/Log.h"

#include <GLM/gtc/type_ptr.hpp>

// TODO: copy shaders in CMake
const GLchar* vertexShaderPath = "../shaders/shader.vs";
const GLchar* fragmentShaderPath = "../shaders/shader.frag";

Camera* mainCamera;
Shader* shader;
GLuint VBO, VAO, EBO;
GLuint modelUniform;
GLuint viewUniform;
GLuint projectionUniform;

void Renderer::Init(Camera* camera)
{
    Log::LogInfo("Begin renderer init");

    mainCamera = camera;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    shader = new Shader(vertexShaderPath, fragmentShaderPath);
    shader->Use();
    glUniform1i(glGetUniformLocation(shader->Program, "image"), 0);

    Log::LogInfo("Shader created");

    modelUniform = glGetUniformLocation(shader->Program, "model");
    viewUniform = glGetUniformLocation(shader->Program, "view");
    projectionUniform = glGetUniformLocation(shader->Program, "projection");

    Log::LogInfo("Uniforms saved");

    Log::LogInfo("Renderer initialized");
}

void Renderer::Terminate()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::OnBeforeRender()
{
    // Set camera transformations
    glm::mat4 view = mainCamera->GetView();
    glm::mat4 projection = mainCamera->GetProjection();
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::Clear(glm::vec3 color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawQuad(glm::mat4 transformation, GLuint textureID)
{
    float texCoord[] = {
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
    };
    Renderer::DrawQuad(transformation, textureID, texCoord);
}

void Renderer::DrawQuad(glm::mat4 transformation, GLuint textureID, const float *texCoord)
{
    // Create quad
    GLfloat vertices[] = {
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  texCoord[0], texCoord[1],
            0.5f, -0.5f, 0.0f,  1.0f, 1.5f, 1.0f, 1.0f,  texCoord[2], texCoord[3],
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  texCoord[4], texCoord[5],
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  texCoord[6], texCoord[7],
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);
    // Vertex color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Bind texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set quad transformation
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(transformation));

    // Draw quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
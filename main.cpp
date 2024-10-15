#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cmath>
#include "ObjLoader.h"
#include "matriceUtils.h"

// Définition de M_PI si non définie
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Shaders
const std::string vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 color; 

uniform mat4 projection;
uniform mat4 transform;

void main()
{
    gl_Position = projection * transform * vec4(aPos, 1.0);
    color = aColor;
}
)";

const std::string fragmentShaderSrc = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0f);
}
)";

// Création des shaders 0.3647, 0.3686, 0.3725
unsigned int createShader(const std::string& source, GLenum shaderType) {
    unsigned int shader = glCreateShader(shaderType);
    const char* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    return shader;
}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    return shaderProgram;
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "42_SCOP", NULL, NULL);
    if (!window) {
        std::cout << "Error creating window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing GLEW!" << std::endl;
        return -1;
    }

    ObjLoader obj("C:\\Users\\Arthur\\Documents\\model\\42.obj");
    std::vector<float> center = calculateModelCenter(obj.vertex);
    
    // config shader
    unsigned int vertexShader = createShader(vertexShaderSrc, GL_VERTEX_SHADER);
    unsigned int fragmentShader = createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //  VBO, VAO, et EBO
    unsigned int VAO, VBO, colorVBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj.vertex.size() * sizeof(float), obj.vertex.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    std::vector<float> faceColor;
    for (size_t i = 0; i < obj.indices.size(); i++) { // J'utilise r pour que r/g/b ai la meme valeur == nuance de gris
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;
        for (int j = 0; j < 3; ++j) {
            faceColor.push_back(r);
            faceColor.push_back(r);
            faceColor.push_back(r);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, faceColor.size() * sizeof(float), faceColor.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), obj.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.85f, 0.86f, 0.87f, 1.0f);

    float fov = 45.0f * (M_PI / 180.0f);
    float aspectRatio = 800.0f / 600.0f;
    float near = 0.1f, far = 1000.0f;
    float perspectiveMatrix[16];
    calculatePerspectiveMatrix(fov, aspectRatio, near, far, perspectiveMatrix);
   
    glUseProgram(shaderProgram);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, perspectiveMatrix);

    float translationX = 0.0f, translationY = 0.0f, translationZ = -5.0f;
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) translationX -= 0.001f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) translationX += 0.001f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) translationY += 0.001f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) translationY -= 0.001f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) translationZ += 0.001f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) translationZ -= 0.001f;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        float rotationAngle = glfwGetTime();
        float rotationMatrix[16];
        calculateRotationMatrix(rotationAngle, rotationMatrix, 1);

        float translateToOrigin[16], translateBack[16], translationMatrix[16];
        calculateTranslationMatrix(-center[0], -center[1], -center[2], translateToOrigin);
        calculateTranslationMatrix(center[0], center[1], center[2], translateBack);
        calculateTranslationMatrix(translationX, translationY, translationZ, translationMatrix);

        float tempMatrix1[16], tempMatrix2[16], fullTransform[16];
        multiplyMatrices(translateToOrigin, rotationMatrix, tempMatrix1);
        multiplyMatrices(tempMatrix1, translateBack, tempMatrix2);
        multiplyMatrices(tempMatrix2, translationMatrix, fullTransform);


        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, fullTransform);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "paddle.hpp"
#include "ball.hpp"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr glm::vec3 PADDLE_SCALE(1.5f, 1.0f, 0.3f);
constexpr glm::vec3 BALL_SCALE(0.1f, 0.1f, 0.1f);

int main() {
    // setup GLFW and GLAD
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW\n";
        return 1;
    }
    atexit([](){glfwTerminate();});
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SAMPLES, 4);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    #endif

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Pong", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creating GLFW window\n";
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Error initializing GLAD\n";
        return 1;
    }

    // general setup
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); 
    srand(time(nullptr)); 

    // create shaders
    Shader ballShader("shader/shader.vert", "shader/ballShader.frag");
    Shader paddleShader("shader/shader.vert", "shader/paddleShader.frag");
    
    float cubeVertices[] = {
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

    // setup VBO and VAOs
    unsigned int VBO, paddleVAO, ballVAO;
    glGenVertexArrays(1, &paddleVAO);
    glGenVertexArrays(1, &ballVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(paddleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof cubeVertices, cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(ballVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
    glEnableVertexAttribArray(0);

    // set projection and view for vertex shader
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH)/WINDOW_HEIGHT, 0.01f, 100.0f
    );
    ballShader.use();
    ballShader.set("projection", projection);

    paddleShader.use();
    paddleShader.set("projection", projection);

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 7.0f, 6.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    ballShader.use();
    ballShader.set("view", view);

    paddleShader.use();
    paddleShader.set("view", view);
    paddleShader.set("viewPos", glm::vec3(0.0f, 7.0f, 6.0f));

    // initialize structs
    Paddle player;
    player.scale = PADDLE_SCALE;
    player.pos = glm::vec3(0.0f, 0.0f, 3.0f);

    Paddle enemy;
    enemy.scale = PADDLE_SCALE;
    enemy.pos = glm::vec3(0.0f, 0.0f, -player.pos.z);

    Ball ball;

    float dt = 0.0f, lf = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        // calculate dt
        float current = glfwGetTime();
        dt = current - lf;
        lf = current;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // movement and collision
        player.move(
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS, 
            glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS, dt
        );

        float diff = ball.pos.x - enemy.pos.x;
        enemy.move(diff < -0.05f, diff > 0.05f, dt);

        ball.move(player, enemy, dt);


        // drawing
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set light attributes
        paddleShader.use();
        paddleShader.set("light.pos", ball.pos);
        paddleShader.set("light.color", glm::vec3(1.0f, 1.0f, 0.2f));

        // draw player
        glBindVertexArray(paddleVAO);
        paddleShader.set("material.ambient", glm::vec3(0.0f, 0.05f, 0.0f));
        paddleShader.set("material.diffuse", glm::vec3(0.4f, 0.7f, 0.4f));
        paddleShader.set("material.specular", glm::vec3(0.04f, 0.7f, 0.04f));
        paddleShader.set("material.shininess", 0.078125f);
        player.draw(paddleShader, glm::vec3(0.1f, 1.0f, 0.2f));

        // draw enemy
        paddleShader.set("material.ambient", glm::vec3(0.05f, 0.0f, 0.0f));
        paddleShader.set("material.diffuse", glm::vec3(0.7f, 0.4f, 0.4f));
        paddleShader.set("material.specular", glm::vec3(0.7f, 0.04f, 0.04f));
        paddleShader.set("material.shininess", 0.078125f);
        enemy.draw(paddleShader, glm::vec3(1.0f, 0.1f, 0.2f));

        // draw ball
        ballShader.use();
        glBindVertexArray(ballVAO);
        ball.draw(ballShader, glm::vec3(1.0f, 1.0f, 0.2f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
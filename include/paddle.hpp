#pragma once

#include "shader.hpp"

struct Paddle {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 scale =glm::vec3(2.0f, 0.5f, 2.0f);
    float speed = 1.5f;

    void move(bool moveLeft, bool moveRight, float dt);
    void draw(const Shader& shader, glm::vec3 color);
};
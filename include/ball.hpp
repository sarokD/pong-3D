#pragma once

#include "paddle.hpp"

struct Ball {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 dir = Ball::randomDir();
    glm::vec3 scale = glm::vec3(0.1f);
    float speed = 2.0f;

    static glm::vec3 randomDir();

    void move(Paddle& player, Paddle& enemy, float dt);
    void draw(const Shader& shader, glm::vec3 color);
};
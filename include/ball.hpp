#pragma once

#include "paddle.hpp"

struct Ball {
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 dir = glm::vec3(1.0f, 0.0f, 1.0f);
    glm::vec3 scale = glm::vec3(0.1f);
    float speed = 1.75f;

    void move(Paddle& player, Paddle& enemy, float dt);
    void draw(const Shader& shader, glm::vec3 color);
};
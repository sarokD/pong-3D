#include "paddle.hpp"

void Paddle::move(bool moveLeft, bool moveRight, float dt) {
    if (moveLeft) pos.x += speed * dt;
    if (moveRight) pos.x -= speed * dt;
}

void Paddle::draw(const Shader& shader, glm::vec3 color) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    shader.set("model", model);

    // uniform vec3 color;
    shader.set("color", color);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
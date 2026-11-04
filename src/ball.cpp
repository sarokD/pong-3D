#include "ball.hpp"
#include <cmath>

static glm::vec3 randomDir() {
    return glm::vec3(
        ((std::rand() % 2) == 0) ? -1.0f : 1.0f,
        0.0f,
        ((std::rand() % 2) == 0) ? -1.0f : 1.0f
    );
}

static void reset(Ball& ball, Paddle& player, Paddle& enemy) {
    ball.pos = glm::vec3(0);
    ball.dir = randomDir();
    enemy.pos.x = 0;
    player.pos.x = 0;
}

void Ball::move(Paddle& player, Paddle& enemy, float dt) {
    pos += glm::normalize(dir) * speed * dt;
    if (std::abs(pos.x) > 2.75f ) dir.x *= -1;
    if (pos.z >= player.pos.z + scale.z) {
        if (std::abs(pos.x-player.pos.x) < player.scale.x/2.0f) {
            pos.z = player.pos.z;
           dir.z *= -1;
        } else { reset(*this, player, enemy); }
    }
    if (pos.z <= enemy.pos.z + enemy.scale.z) {
        if (std::abs(pos.x-enemy.pos.x) < enemy.scale.x/2.0f) {
            pos.z = enemy.pos.z + enemy.scale.z + 0.01f;
            dir.z *= -1;
        } else { reset(*this, player, enemy); }
    }
}


void Ball::draw(const Shader& shader, glm::vec3 color) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    shader.set("model", model);

    shader.set("color", color);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
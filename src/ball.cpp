#include "ball.hpp"
#include <cmath>

glm::vec3 Ball::randomDir() {
    return glm::vec3(
        ((std::rand() % 2) == 0) ? -1.0f : 1.0f,
        ((std::rand() % 2) == 0) ? -1.0f : 1.0f,
        0.0f
    );
}

static void reset(Ball& ball, Paddle& player, Paddle& enemy) {
    ball.pos = glm::vec3(0);
    ball.dir = Ball::randomDir();
    enemy.pos.x = 0;
    player.pos.x = 0;
}

void Ball::move(Paddle& player, Paddle& enemy, float dt) {
    const float playerFace = player.pos.y - player.scale.y/2.0f - scale.y/2.0f;
    const float enemyFace = enemy.pos.y + enemy.scale.y/2.0f + scale.y/2.0f;

    pos += glm::normalize(dir) * speed * dt;
    if (std::abs(pos.x) > 2.75f ) dir.x *= -1;
    if (pos.y >= playerFace) {
        if (std::abs(pos.x-player.pos.x) < player.scale.x/2.0f) {
            pos.y = playerFace;
           dir.y *= -1;
           dir.x = (pos.x-player.pos.x)/(player.scale.x/2.0f)*1.2f;
        } else if (pos.y >= playerFace + player.scale.y) { reset(*this, player, enemy); }
    }
    if (pos.y <= enemyFace) {
        if (std::abs(pos.x-enemy.pos.x) < enemy.scale.x/2.0f) {
            pos.y = enemyFace;
            dir.y *= -1;
            dir.x = (pos.x-enemy.pos.x)/(enemy.scale.x/2.0f);
        } else if (pos.y <= enemyFace - enemy.scale.y) { reset(*this, player, enemy); }
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
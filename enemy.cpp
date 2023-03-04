#include "LESTA_TANKS.h"
#include <random>

Enemy::Enemy(sf::Vector2i p) : health(100), pos(p), dir(2), sprite(new ISprite("enemy_tank.png", true)) {
    sprite->SetPosition(sf::Vector2f(pos));
}

void Enemy::Update() {
    if(health <= 0)
        return;
    if (toMove > 0.0f)
    {
        toMove -= getDeltaTime();
    }
    else {
        sf::Vector2i n = pos;
        switch (dir) {
            case 0:
                n.y -= PIXEL_SIZE;
                break;
            case 1:
                n.x += PIXEL_SIZE;
                break;
            case 2:
                n.y += PIXEL_SIZE;
                break;
            case 3:
                n.x -= PIXEL_SIZE;
                break;
            default:
                break;
        }
        if (n.x != pos.x || n.y != pos.y) {

            sprite->SetRotation(90.0f * (float)dir);
            toMove = 0.1f;
            if (CheckBounds(pos,n, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), OBSTACLE::ENEMY) == OBSTACLE::NOTHING)
            {
                pos = sf::Vector2i(n);
                sprite->SetPosition(sf::Vector2f(pos));
            }
        }
    }
    if (toShoot > 0.0f)
    {
        toShoot -= getDeltaTime();
    }
    else
    {
        Projectile(pos, dir);
        toShoot = 2.0f;
    }
    if(toChangeDir > 0.0f) {
        toChangeDir -= getDeltaTime();
    }
    else {
        bool changed = false;
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distributionTime(0,300);
        std::uniform_int_distribution<int> distributionDir(0,3);
        toChangeDir = (float)distributionTime(generator) / 100.0f;
        while (!changed) {

            dir = distributionDir(generator);
            sf::Vector2i n = pos;
            switch (dir) {
                case 0:
                    n.y -= PIXEL_SIZE;
                    break;
                case 1:
                    n.x += PIXEL_SIZE;
                    break;
                case 2:
                    n.y += PIXEL_SIZE;
                    break;
                case 3:
                    n.x -= PIXEL_SIZE;
                    break;
                default:
                    break;
            }
            OBSTACLE cb = CheckBounds(pos,n, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), OBSTACLE::ENEMY);
            if(cb != OBSTACLE::END_OF_MAP && cb != OBSTACLE::WALL)
                changed = true;
        }
    }
}

sf::Vector2i Enemy::GetPosition() const {
    return pos;
}

sf::Sprite Enemy::GetSprite() {
    return sprite->GetSprite();
}

void Enemy::GetDamage() {
    health -= 50;
    if(health <= 0) {
        pos = sf::Vector2i(-100,-100);
        sprite->SetPosition(sf::Vector2f(pos));
        RemoveEnemy(*this);
    }
}
#include "LESTA_TANKS.h"

Enemy::Enemy(sf::Vector2i p) {
    pos = p;
    health = 100;
    sprite = new ISprite("enemy_tank.png", true);
    sprite->SetPosition(sf::Vector2f(pos));
    dir = 2;
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
        }
        if (n.x != pos.x || n.y != pos.y) {

            sprite->SetRotation(90.0f * (float)dir);
            toMove = 0.1f;
            if (CheckBounds(pos,n, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), ENEMY) == NOTHING)
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
        toChangeDir = ((float)(rand() % 300) / 100.0f);
        while (!changed) {
            dir = rand() % 4;
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
            }
            int cb = CheckBounds(pos,n, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), ENEMY);
            if(cb != END_OF_MAP && cb != WALL)
                changed = true;
        }
        //CheckDir();
    }
}

/*void Enemy::CheckDir() {
    bool found = false;
    int cb = 0;
    switch (dir) {
        case 0:
            for(int i = pos.y; i > 0; i-=PIXEL_SIZE) {
                cb = CheckBounds(pos,sf::Vector2i(pos.x,i),sf::Vector2i(1,1),ENEMY);
                if(cb > 1) {
                    found = true;
                    break;
                }
            }
            break;
        case 1:
            for(int i = pos.x; i < 800; i+=PIXEL_SIZE) {
                cb = CheckBounds(pos,sf::Vector2i(i,pos.y),sf::Vector2i(1,1),ENEMY);
                if(cb > 1) {
                    found = true;
                    break;
                }
            }
            break;
        case 2:
            for(int i = pos.y; i < 800; i+=PIXEL_SIZE) {
                cb = CheckBounds(pos,sf::Vector2i(pos.x,i),sf::Vector2i(1,1),ENEMY);
                if(cb > 1) {
                    found = true;
                    break;
                }
            }
            break;
        case 3:
            for(int i = pos.x; i > 0; i-=PIXEL_SIZE) {
                cb = CheckBounds(pos,sf::Vector2i(i,pos.y),sf::Vector2i(1,1),ENEMY);
                if(cb > 1) {
                    found = true;
                    break;
                }
            }
            break;
    }
    if(found && (cb == WALL || cb == PLAYER))
    {
        needShot = true;
        Projectile projectile(pos, dir, true);
        toShoot = 0.5f;
    }
}*/

sf::Vector2i Enemy::GetPosition() {
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
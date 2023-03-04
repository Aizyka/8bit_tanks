#include "LESTA_TANKS.h"

Player::Player() : sprite(new ISprite("tank.png", true)), pos(sf::Vector2i(400,600)), dir(0) {
	sprite->SetPosition(sf::Vector2f(pos));
}

void Player::Update() {
    if(health <= 0)
        return;
	if (toMove > 0.0f)
	{
		toMove -= getDeltaTime();
	}
	else {
		int new_x = pos.x;
		int new_y = pos.y;
        switch (GetKey()) {
            case sf::Keyboard::W:
                new_y -= PIXEL_SIZE;
                dir = 0;
                break;
            case sf::Keyboard::A:
                new_x -= PIXEL_SIZE;
                dir = 3;
                break;
            case sf::Keyboard::S:
                new_y += PIXEL_SIZE;
                dir = 2;
                break;
            case sf::Keyboard::D:
                new_x += PIXEL_SIZE;
                dir = 1;
                break;
            default:
                break;
        }
		if (new_x != pos.x || new_y != pos.y) {
            sprite->SetRotation(90.0f * (float)dir);
			toMove = 0.1f;
			if (CheckBounds(pos, sf::Vector2i(new_x, new_y), sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), OBSTACLE::PLAYER) == OBSTACLE::NOTHING)
			{
				pos.x = new_x; pos.y = new_y;
                sprite->SetPosition(sf::Vector2f(pos));
			}
		}
	}
	if (toShoot > 0.0f)
	{
		toShoot -= getDeltaTime();
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Projectile(pos, dir);
			toShoot = 0.5f;
		}
	}
}

sf::Vector2i Player::GetPosition() const {
	return pos;
}

sf::Sprite Player::GetSprite() {
    return sprite->GetSprite();
}

void Player::GetDamage() {
    health -= 50;
    SetString("Health: " + std::to_string(health));
    if(health <= 0) {
        pos = sf::Vector2i(-100,-100);
        sprite->SetPosition(sf::Vector2f(pos));
        SetString("GAME OVER\nPress 'R' to restart");
    }
}

void Player::Reset() {
    health = 100;
    pos = sf::Vector2i(400, 600);
    dir = 0;
    sprite->SetPosition(sf::Vector2f(pos));
    sprite->SetRotation(0);
}
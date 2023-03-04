#include "LESTA_TANKS.h"

Obstacle::Obstacle(sf::Vector2i p, bool breakable) : pos(p), sprite(breakable == true ? make_unique<ISprite>("box.png", true) : make_unique<ISprite>("wall.png", true)), canBreak(breakable) {
	sprite->SetPosition(sf::Vector2f(pos));
}

bool Obstacle::isBreakable() const {
	return canBreak;
}

void Obstacle::Break() {
	pos = sf::Vector2i(-100,-100);
	sprite->SetPosition(sf::Vector2f(pos));
}

void Obstacle::SetPosition(sf::Vector2i p) {
    pos = p;
    sprite->SetPosition(sf::Vector2f(pos));
}

sf::Vector2i Obstacle::GetPosition() const {
	return pos;
}

sf::Sprite Obstacle::GetSprite() const {
	return sprite->GetSprite();
}
#include "LESTA_TANKS.h"

Obstacle::Obstacle(sf::Vector2i p, bool breakable) {
    pos = p;
	if(breakable)
		sprite = new ISprite("box.png", true);
	else
		sprite = new ISprite("wall.png", true);
	sprite->SetPosition(sf::Vector2f(pos));
	canBreak = breakable;
	
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

sf::Vector2i Obstacle::GetPosition() {
	return pos;
}

sf::Sprite Obstacle::GetSprite() {
	return sprite->GetSprite();
}
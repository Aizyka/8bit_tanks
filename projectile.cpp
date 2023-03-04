#include "LESTA_TANKS.h"

Projectile::Projectile(sf::Vector2i p, int dir) {
	int mod_x = 0;
	int mod_y = 0;
	switch (dir) {
        case 0:
            mod_y = -1;
            break;
        case 1:
            mod_x = 1;
            break;
        case 2:
            mod_y = 1;
            break;
        case 3:
            mod_x = -1;
            break;
        default:
            return;
    }
    updateTime = 0.0f;
    mod = sf::Vector2i(mod_x,mod_y);
    pos = sf::Vector2i(p.x+mod_x*PIXEL_SIZE*7, p.y+mod_y*PIXEL_SIZE*7);
	AddProjectile(*this);
}

void Projectile::Update() {
	if (updateTime > 0.0f) {
		updateTime -= getDeltaTime();
		return;
	}
    if(sf::Vector2i newPos(pos.x+mod.x*8,pos.y+mod.y*8); CheckBounds(pos, newPos, sf::Vector2i(1, 1), OBSTACLE::PROJECTILE) == OBSTACLE::NOTHING) {
        pos = newPos;
    }
    else {
        RemoveProjectile(*this);
        return;
    }
	updateTime = 0.05f;
}

sf::Vector2i Projectile::GetPosition() const {
	return pos;
}
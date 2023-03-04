#include <SFML/Graphics.hpp>
#define PIXEL_SIZE 8

typedef enum {
	NOTHING,
	END_OF_MAP,
	WALL,
	PROJECTILE,
	PLAYER,
	ENEMY
} OBSTACLE;

class ISprite {
public:
	void SetPosition(const sf::Vector2f position) {
		sprite.setPosition(position);
	}
	void SetRotation(const float angle) {
		sprite.setRotation(angle);
	}
	void SetColor(const sf::Color color) {
		sprite.setColor(color);
	}
	sf::Sprite GetSprite() {
		return sprite;
	}
	explicit ISprite(const std::string path, const bool center = true) {
		texture.loadFromFile("../resources/images/"+path);
		sprite.setTexture(texture, true);
		sf::FloatRect spriteSize = sprite.getGlobalBounds();
		if (center)
			sprite.setOrigin(spriteSize.width / 2.0f, spriteSize.height / 2.0f);
		sprite.setScale(sf::Vector2f(10, 10));
	}
	ISprite() = default;
private:
	sf::Texture texture;
	sf::Sprite sprite;
};

class Player {
public:
	Player();
	void Update();
	sf::Sprite GetSprite();
	sf::Vector2i GetPosition();
    void GetDamage();
    void Reset();
private:
	int health = 100;
    ISprite* sprite = nullptr;
    sf::Vector2i pos;
    int dir;
    float toMove= 0.0f;
    float toShoot = 0.0f;
};

class Enemy {
public:
    explicit Enemy(sf::Vector2i p);
    void Update();
    sf::Sprite GetSprite();
	sf::Vector2i GetPosition();
    void GetDamage();
private:
    int health;
    ISprite* sprite = nullptr;
    sf::Vector2i pos;
    int dir;
    float toMove= 0.0f;
    float toShoot = 0.0f;
    float toChangeDir = 0.0f;
};

class Obstacle {
public:
	Obstacle(sf::Vector2i pos, bool breakable);
	sf::Vector2i GetPosition();
	sf::Sprite GetSprite();
	bool isBreakable() const;
	void Break();
    void SetPosition(sf::Vector2i pos);
	Obstacle() = default;
private:
	ISprite* sprite;
	bool canBreak;
    sf::Vector2i pos;
};

class Projectile {
public:
	Projectile(sf::Vector2i pos, int dir);
	sf::Vector2i GetPosition();
	void Update();
private:
    sf::Vector2i pos;
    sf::Vector2i mod;
	float updateTime;
};



float getDeltaTime();
void AddProjectile(Projectile projectile);
void RemoveProjectile(Projectile projectile);
void RemoveEnemy(Enemy enemy);
int CheckBounds(sf::Vector2i p, sf::Vector2i n, sf::Vector2i s, OBSTACLE type);
void SetString(const std::string& str);
sf::Keyboard::Key GetKey();
#include "LESTA_TANKS.h"
#include <vector>
using namespace std;

float deltaTime;
bool mustbreakloop = false;
vector<Projectile> projectiles = {};
vector<ISprite> projectileSprites = {};
Obstacle obstacles[11];
Player player;
vector<Enemy> enemies = {};
vector<sf::Keyboard::Key> activeKeys = {};
sf::Text text;
float newSpawn = 3.0f;

void Restart() {
    for (int i = 0; i < 3; i++) {
        obstacles[i].SetPosition(sf::Vector2i(312 + i * PIXEL_SIZE*11, PIXEL_SIZE*50));
    }
    player.Reset();
    enemies.clear();
    projectiles.clear();
    projectileSprites.clear();
    mustbreakloop = true;
    text.setString("Health: 100");
    enemies.emplace_back(Enemy(sf::Vector2i(600,160)));
    enemies.emplace_back(Enemy(sf::Vector2i(200,160)));
}

float getDeltaTime() {
    return deltaTime;
}

void AddProjectile(Projectile projectile) {
    ISprite sprite("projectile.png",true);
    sprite.SetColor(sf::Color::Black);
    projectileSprites.push_back(sprite);
    projectiles.push_back(projectile);
}

void RemoveProjectile(Projectile projectile) {
    int pos = -1;
    for (int i = 0; i < projectiles.size(); i++) {
        if (projectile.GetPosition() == projectiles[i].GetPosition()) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    projectiles.erase(projectiles.begin() + pos);
    projectileSprites.erase(projectileSprites.begin() + pos);
    mustbreakloop = true;
}

void RemoveEnemy(Enemy enemy) {
    int pos = -1;
    for (int i = 0; i < enemies.size(); i++) {
        if (enemy.GetPosition() == enemies[i].GetPosition()) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    enemies.erase(enemies.begin() + pos);
    mustbreakloop = true;
}

int CheckBounds(sf::Vector2i p, sf::Vector2i n, sf::Vector2i s, OBSTACLE type) {
    
    if (n.x - s.x < 0 || n.x + s.x > 800 || n.y - s.y < 0 || n.y + s.y > 800) {
        return END_OF_MAP;
    }
    for (auto & obstacle : obstacles) {
        sf::Vector2i pos = obstacle.GetPosition();
        if ((pos.x == p.x && pos.y == p.y) || pos.x == -100) continue;
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE*5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE*5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (obstacle.isBreakable() && type == PROJECTILE)
                obstacle.Break();
            return WALL;
        }
    }
    {
        int pos1 = -1;
        for (int i = 0; i < projectiles.size(); i++) {
            sf::Vector2i pos = projectiles[i].GetPosition();
            if (pos.x == p.x && pos.y == p.y) {
                continue;
            }
            int pos_diff_x = abs(n.x - pos.x);
            int pos_diff_y = abs(n.y - pos.y);
            if (pos_diff_x <= PIXEL_SIZE && pos_diff_y <= PIXEL_SIZE)
            {
                pos1 = i;
                break;
            }
        }
        if (pos1 != -1) {
            projectiles.erase(projectiles.begin() + pos1);
            projectileSprites.erase(projectileSprites.begin() + pos1);
            mustbreakloop = true;
            return PROJECTILE;
        }
    }
    if(type != PLAYER) {
        sf::Vector2i pos = player.GetPosition();
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE * 5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE * 5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (type == PROJECTILE)
                player.GetDamage();
            return PLAYER;
        }
    }
    for(auto & enemy : enemies) {
        sf::Vector2i pos = enemy.GetPosition();
        if (pos.x == p.x && pos.y == p.y) continue;
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE * 5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE * 5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (type == PROJECTILE)
                enemy.GetDamage();
            return ENEMY;
        }
    }
    return NOTHING;
}

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 800), "TANKI!");
    ISprite background("background.png", false);

    sf::Font font;
    if(!font.loadFromFile("../resources/fonts/arial.ttf")) {
        return 0;
    }
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setString("Health: 100");
    enemies.emplace_back(Enemy(sf::Vector2i(600,160)));
    enemies.emplace_back(Enemy(sf::Vector2i(200,160)));

    sf::View view = window.getDefaultView();
    for (int i = 0; i < 3; i++) {
        obstacles[i] = *new Obstacle(sf::Vector2i(312 + i * PIXEL_SIZE*11, PIXEL_SIZE*50), true);
    }
    for (int i = 3; i < 7; i++) {
        obstacles[i] = *new Obstacle(sf::Vector2i(i > 4 ? PIXEL_SIZE * 6 : PIXEL_SIZE*100 - PIXEL_SIZE*6, i % 2 != 0 ? PIXEL_SIZE * 32.5f : PIXEL_SIZE * 100 - PIXEL_SIZE * 32.5f), false);
    }
    for (int i = 7; i < 11; i++) {
        obstacles[i] = *new Obstacle(sf::Vector2i(PIXEL_SIZE * 50 + (i > 8 ? PIXEL_SIZE * 17.5f : PIXEL_SIZE * -17.5f), i % 2 != 0 ? PIXEL_SIZE * 32.5f : PIXEL_SIZE * 100 - PIXEL_SIZE * 32.5f), false);
    }
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::R)
                        Restart();
                    else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::D)
                        if(find(activeKeys.begin(),activeKeys.end(),event.key.code) == activeKeys.end())
                            activeKeys.push_back(event.key.code);
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::D)
                    {
                        auto pos = find(activeKeys.begin(),activeKeys.end(),event.key.code);
                        if(pos != activeKeys.end())
                            activeKeys.erase(pos);
                    }
                    break;
                default:
                    break;
            }
        }
        player.Update();

        window.clear(sf::Color::White);
        window.draw(background.GetSprite());
        window.draw(player.GetSprite());
        for(auto & enemy : enemies) {
            enemy.Update();
            if (mustbreakloop) {
                mustbreakloop = false;
                break;
            }
            window.draw(enemy.GetSprite());
        }

        for (int i = 0; i < projectiles.size(); i++) {
            projectiles[i].Update();
            if (mustbreakloop) {
                mustbreakloop = false;
                break;
            }
            projectileSprites[i].SetPosition(sf::Vector2f(projectiles[i].GetPosition()));
            window.draw(projectileSprites[i].GetSprite());
        }
        for (auto & obstacle : obstacles) {
            window.draw(obstacle.GetSprite());
        }
        window.draw(text);
        window.display();
        deltaTime = clock.getElapsedTime().asSeconds();
        if(newSpawn > 0) {
            newSpawn -= deltaTime;
        }
        else if(enemies.size() < 4) {
            sf::Vector2i check1(600,160);
            sf::Vector2i check2(200,160);
            if(CheckBounds(check1, check1, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), ENEMY) == NOTHING)
                enemies.emplace_back(Enemy(check1));
            else if(CheckBounds(check2, check2, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), ENEMY) == NOTHING)
                enemies.emplace_back(Enemy(check2));
            newSpawn = 5.0f;
        }
        clock.restart();
    }
    return 0;
}

void SetString(const string& str) {
    text.setString(str);
}

sf::Keyboard::Key GetKey() {
    if(!activeKeys.empty())
        return activeKeys[activeKeys.size()-1];
    return sf::Keyboard::Key::Unknown;
}
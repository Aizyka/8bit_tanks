#include "LESTA_TANKS.h"
using namespace std;
float MainGame::deltaTime;
bool MainGame::MainGame::mustbreakloop = false;
std::vector<Projectile> MainGame::projectiles = {};
std::vector<ISprite> MainGame::projectileSprites = {};
Player MainGame::player;
std::vector<Enemy> MainGame::enemies = {};
std::vector<Obstacle> MainGame::obstacles = {};
std::vector<sf::Keyboard::Key> MainGame::MainGame::activeKeys = {};
sf::Text MainGame::text;
sf::Font MainGame::font;
float MainGame::newSpawn = 3.0f;


void Restart() {
    for (int i = 0; i < 3; i++) {
        MainGame::obstacles[i].SetPosition(sf::Vector2i(312 + i * PIXEL_SIZE*11, PIXEL_SIZE*50));
    }
    MainGame::player.Reset();
    MainGame::enemies.clear();
    MainGame::projectiles.clear();
    MainGame::projectileSprites.clear();
    MainGame::mustbreakloop = true;
    MainGame::text.setString("Health: 100");
    MainGame::enemies.emplace_back(sf::Vector2i(600,160));
    MainGame::enemies.emplace_back(sf::Vector2i(200,160));
}

float getDeltaTime() {
    return MainGame::deltaTime;
}

void AddProjectile(Projectile projectile) {
    ISprite sprite("projectile.png",true);
    sprite.SetColor(sf::Color::Black);
    MainGame::projectileSprites.push_back(sprite);
    MainGame::projectiles.push_back(projectile);
}

void RemoveProjectile(Projectile projectile) {
    int pos = -1;
    for (int i = 0; i < MainGame::projectiles.size(); i++) {
        if (projectile.GetPosition() == MainGame::projectiles[i].GetPosition()) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    MainGame::projectiles.erase(MainGame::projectiles.begin() + pos);
    MainGame::projectileSprites.erase(MainGame::projectileSprites.begin() + pos);
    MainGame::mustbreakloop = true;
}

void RemoveEnemy(Enemy const & enemy) {
    int pos = -1;
    for (int i = 0; i < MainGame::enemies.size(); i++) {
        if (enemy.GetPosition() == MainGame::enemies[i].GetPosition()) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    MainGame::enemies.erase(MainGame::enemies.begin() + pos);
    MainGame::mustbreakloop = true;
}

OBSTACLE CheckObstacles(sf::Vector2i p, sf::Vector2i n, sf::Vector2i s, OBSTACLE type) {
    for (auto & obstacle : MainGame::obstacles) {
        sf::Vector2i pos = obstacle.GetPosition();
        if ((pos.x == p.x && pos.y == p.y) || pos.x == -100) continue;
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE*5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE*5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (obstacle.isBreakable() && type == OBSTACLE::PROJECTILE)
                obstacle.Break();
            return OBSTACLE::WALL;
        }
    }
    return OBSTACLE::NOTHING;
}

OBSTACLE CheckProjectiles(sf::Vector2i p, sf::Vector2i n) {
    int pos1 = -1;
    for (int i = 0; i < MainGame::projectiles.size(); i++) {
        sf::Vector2i pos = MainGame::projectiles[i].GetPosition();
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
        MainGame::projectiles.erase(MainGame::projectiles.begin() + pos1);
        MainGame::projectileSprites.erase(MainGame::projectileSprites.begin() + pos1);
        MainGame::mustbreakloop = true;
        return OBSTACLE::PROJECTILE;
    }
    return OBSTACLE::NOTHING;
}

OBSTACLE CheckPlayer(sf::Vector2i n, sf::Vector2i s, OBSTACLE type) {
    if(type != OBSTACLE::PLAYER) {
        sf::Vector2i pos = MainGame::player.GetPosition();
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE * 5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE * 5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (type == OBSTACLE::PROJECTILE)
                MainGame::player.GetDamage();
            return OBSTACLE::PLAYER;
        }
    }
    return OBSTACLE::NOTHING;
}

OBSTACLE CheckEnemies(sf::Vector2i p, sf::Vector2i n, sf::Vector2i s, OBSTACLE type) {
    for(auto & enemy : MainGame::enemies) {
        sf::Vector2i pos = enemy.GetPosition();
        if (pos.x == p.x && pos.y == p.y) continue;
        int pos_diff_x = abs(n.x - pos.x);
        int pos_diff_y = abs(n.y - pos.y);
        int scale_diff_x = abs(s.x + PIXEL_SIZE * 5);
        int scale_diff_y = abs(s.y + PIXEL_SIZE * 5);
        if (scale_diff_x > pos_diff_x && scale_diff_y > pos_diff_y)
        {
            if (type == OBSTACLE::PROJECTILE)
                enemy.GetDamage();
            return OBSTACLE::ENEMY;
        }
    }
    return OBSTACLE::NOTHING;
}

OBSTACLE CheckBounds(sf::Vector2i p, sf::Vector2i n, sf::Vector2i s, OBSTACLE type) {
    
    if (n.x - s.x < 0 || n.x + s.x > 800 || n.y - s.y < 0 || n.y + s.y > 800) {
        return OBSTACLE::END_OF_MAP;
    }
    OBSTACLE status = CheckObstacles(p,n,s,type);
    if(status != OBSTACLE::NOTHING) return status;
    status = CheckProjectiles(p,n);
    if(status != OBSTACLE::NOTHING) return status;
    status = CheckPlayer(n,s,type);
    if(status != OBSTACLE::NOTHING) return status;
    return CheckEnemies(p,n,s,type);
}

void initText() {
    MainGame::font.loadFromFile("../resources/fonts/arial.ttf");
    MainGame::text.setFont(MainGame::font);
    MainGame::text.setCharacterSize(18);
    MainGame::text.setFillColor(sf::Color::Black);
    MainGame::text.setStyle(sf::Text::Bold);
    MainGame::text.setString("Health: 100");
}

void setup() {
    MainGame::enemies.emplace_back(sf::Vector2i(600,160));
    MainGame::enemies.emplace_back(sf::Vector2i(200,160));
    for (int i = 0; i < 3; i++) {
        MainGame::obstacles.emplace_back(sf::Vector2i(312 + i * PIXEL_SIZE*11, PIXEL_SIZE*50), true);
    }
    for (int i = 3; i < 7; i++) {
        MainGame::obstacles.emplace_back(sf::Vector2i(i > 4 ? PIXEL_SIZE * 6 : PIXEL_SIZE*100 - PIXEL_SIZE*6, i % 2 != 0 ? (int)(PIXEL_SIZE * 32.5f) : (int)(PIXEL_SIZE * 100 - PIXEL_SIZE * 32.5f)), false);
    }
    for (int i = 7; i < 11; i++) {
        MainGame::obstacles.emplace_back(sf::Vector2i(PIXEL_SIZE * 50 + (int)(i > 8 ? (PIXEL_SIZE * 17.5f) : (int)(PIXEL_SIZE * -17.5f)), i % 2 != 0 ? (int)(PIXEL_SIZE * 32.5f) : (int)(PIXEL_SIZE * 100 - PIXEL_SIZE * 32.5f)), false);
    }
}

void pollEvent(sf::RenderWindow& window) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R)
                    Restart();
                else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::D) && find(MainGame::MainGame::activeKeys.begin(),MainGame::MainGame::activeKeys.end(),event.key.code) == MainGame::MainGame::activeKeys.end())
                        MainGame::MainGame::activeKeys.push_back(event.key.code);
                break;
            case sf::Event::KeyReleased:

                if (auto pos = find(MainGame::MainGame::activeKeys.begin(), MainGame::MainGame::activeKeys.end(),
                                    event.key.code); pos != MainGame::MainGame::activeKeys.end())
                    MainGame::MainGame::activeKeys.erase(pos);
                break;
            case sf::Event::Resized:
                window.setSize(sf::Vector2u(800,800));
                break;
            default:
                break;
        }
    }
}

void update(sf::RenderWindow& window) {
    MainGame::player.Update();
    window.draw(MainGame::player.GetSprite());
    for(auto & enemy : MainGame::enemies) {
        enemy.Update();
        if (MainGame::mustbreakloop) {
            MainGame::mustbreakloop = false;
            break;
        }
        window.draw(enemy.GetSprite());
    }

    for (int i = 0; i < MainGame::projectiles.size(); i++) {
        MainGame::projectiles[i].Update();
        if (MainGame::mustbreakloop) {
            MainGame::mustbreakloop = false;
            break;
        }
        MainGame::projectileSprites[i].SetPosition(sf::Vector2f(MainGame::projectiles[i].GetPosition()));
        window.draw(MainGame::projectileSprites[i].GetSprite());
    }
    for (const auto & obstacle : MainGame::obstacles) {
        window.draw(obstacle.GetSprite());
    }
    window.draw(MainGame::text);
}

void updateEnemySpawn() {
    if(MainGame::newSpawn > 0) {
        MainGame::newSpawn -= MainGame::deltaTime;
    }
    else if(MainGame::enemies.size() < 4) {
        sf::Vector2i check1(600,160);
        sf::Vector2i check2(200,160);
        if(CheckBounds(check1, check1, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), OBSTACLE::ENEMY) == OBSTACLE::NOTHING)
            MainGame::enemies.emplace_back(check1);
        else if(CheckBounds(check2, check2, sf::Vector2i(PIXEL_SIZE*5, PIXEL_SIZE*5), OBSTACLE::ENEMY) == OBSTACLE::NOTHING)
            MainGame::enemies.emplace_back(check2);
        MainGame::newSpawn = 5.0f;
    }
}


int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(800, 800), "TANKI!");
    ISprite background("background.png", false);
    initText();
    setup();


    while (window.isOpen()) {
        pollEvent(window);

        window.clear(sf::Color::White);
        window.draw(background.GetSprite());
        update(window);
        window.display();
        MainGame::deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        updateEnemySpawn();
    }
    return 0;
}

void SetString(const string& str) {
    MainGame::text.setString(str);
}

sf::Keyboard::Key GetKey() {
    if(!MainGame::MainGame::activeKeys.empty())
        return MainGame::MainGame::activeKeys[MainGame::MainGame::activeKeys.size()-1];
    return sf::Keyboard::Key::Unknown;
}
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "View.h"
#include "Player.h"
#include "Collider.h"

using namespace sf;

int main()
{
	srand(time(0));

	#pragma region Setup
	// Создание окна и вида
	RenderWindow window(VideoMode(1280, 720), "Game");
	view.reset(FloatRect(0, 0, 512, 288));

	// Создание шрифта и текста
	Font font;
	font.loadFromFile("fonts/default.otf");
	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setScale(0.5f, 0.5f);
	text.setOutlineColor(Color::Black);
	text.setOutlineThickness(2);

	// Создание карты тайлов
	Texture tilesTexture;
	tilesTexture.loadFromFile("img/tiles.png");
	Image level;
	level.loadFromFile("levels/level.png");
	TileMap tileMap(level, tilesTexture, Vector2u(32, 32), Collider(0, 0, 32, 32), Vector2f(1.0f, 1.0f));

	// Создание игрока
	Image playerImg;
	playerImg.loadFromFile("img/player.png");
	Texture playerTexture;
	playerTexture.loadFromImage(playerImg);

	Stats stats(&view, text);

	Player player(Vector2f(32, 32), tileMap.getTileSize(), playerTexture, Collider(5, 10, 22, 22), stats, Vector2f(1.5f, 1.5f));
	player.setScale(Vector2f(1.5f, 1.5f));

	// Анимации
	playerTexture.loadFromImage(playerImg, IntRect(0, 0, 32 * 4, 32));
	player.idleAnim = Animation(playerTexture, 4, true);
	playerTexture.loadFromImage(playerImg, IntRect(0, 32, 32 * 11, 32));
	player.jumpAnim = Animation(playerTexture, 11, false, 1 / 1.5f);
	playerTexture.loadFromImage(playerImg, IntRect(0, 32 * 2, 32 * 5, 32));
	player.attackAnim = Animation(playerTexture, 5, false);
	playerTexture.loadFromImage(playerImg, IntRect(0, 32 * 3, 32 * 4, 32));
	player.hurtAnim = Animation(playerTexture, 4, false);
	playerTexture.loadFromImage(playerImg, IntRect(0, 32 * 4, 32 * 9, 32));
	player.deathAnim = Animation(playerTexture, 9, false);

	// Спавн игрока
	player.setPosition(tileMap.getPlayerSpawnPos(level));

	bool showStats = true;

	Clock clock;
	float deltaTime = 0;
	float tileSpawnTime = 1;
	float tileSpawnTimer = 0;

	#pragma endregion

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		if (player.isAlive == false)
		{
			window.close();
		}

		#pragma region Events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		#pragma endregion

		TileType randomType = static_cast<TileType>(2 + rand() % (TileType::None - 2));

		/*if (tileSpawnTimer >= tileSpawnTime) 
		{
			tileMap.spawnTiles(randomType, 1);
			tileSpawnTimer = 0;
		}
		else
		{
			tileSpawnTimer += deltaTime;
		}*/

		player.update(tileMap, deltaTime);
		clampView(player.getPosition(), tileMap);

		window.clear();

		window.setView(view);
		changeView(deltaTime);

		window.draw(tileMap);
		window.draw(player);

		window.display();
	}

	return 0;
}
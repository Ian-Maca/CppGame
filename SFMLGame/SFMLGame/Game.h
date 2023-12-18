#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


/*
*		//system window graphics - 2 . dll
		Class that acts as a game engine
*/


class Game
{
private:

	//   ----  variables  ----
	// 
	//
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions (one for collision check)
	sf::Vector2i mousePosition;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;
	sf::Text uiText;
	sf::Text endGameText;
	sf::Text getFuckedText;

	//Game logic
	size_t frameCounter;
	unsigned int points;
	unsigned int highScore;
	int health;
	bool bEndGame;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	unsigned int maxEnemies;
	bool bMouseHeld;

	//Game objects
	std::vector<sf::Sprite> enemies;
	sf::Texture enemyTexture;
	sf::Sprite enemySprite;


	//Background Image
	sf::Texture bgTexture;
	sf::Sprite bgSprite;

	//EndGameImage
	sf::Texture endGameBgTexture;
	sf::Sprite endGameBgSprite;

	//Restart button
	sf::Texture restartButtonTexture;
	sf::Sprite restartButtonSprite;

	void initVariables();
	void initWindow();
	void initEnemies();
	void initFonts();
	void initText();

public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool gameEnded() const;

	//Functions
	void callRestartGame();
	void spawnEnemy();
	void updateEnemies();
	void updateMousePositions();
	void pollEvents();
	void update();
	void updateText();
	void drawEndGame(sf::RenderTarget& target);

	void render();
	void renderEnemies(sf::RenderTarget& target);
	void renderText(sf::RenderTarget& target);
	

};

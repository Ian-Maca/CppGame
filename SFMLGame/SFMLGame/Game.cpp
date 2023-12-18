#pragma once
#include "Game.h"

constexpr float max_window_height = 720;
constexpr float max_window_width = 1280;

void Game::initVariables()
{
	this->window = nullptr;

	//game logic
	this->frameCounter = 0;
	this->bEndGame = false;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->bMouseHeld = false;
	this->highScore = 0;

}

void Game::initWindow()
{
	//Main window setup
	this->videoMode.width = max_window_width;	this->videoMode.height = max_window_height;
	this->window = new sf::RenderWindow(this->videoMode, "WHATTHEFUCK", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);

	//Background image setup
	this->bgTexture.loadFromFile("Textures/bgimage.png");
	this->bgSprite.setTexture(this->bgTexture);


	//endGame window setup
	this->endGameBgTexture.loadFromFile("Textures/endgamebg.png");
	this->endGameBgSprite.setTexture(this->endGameBgTexture);


	//Restart button setup
	this->restartButtonTexture.loadFromFile("Textures/restartbutton.png");
	this->restartButtonSprite.setTexture(this->restartButtonTexture);
	this->restartButtonSprite.setPosition(sf::Vector2f(max_window_width / 2.f, max_window_height / 1.2f));

}

void Game::initEnemies()
{
	if (!this->enemyTexture.loadFromFile("Textures/hatdude.png")) 
	{
		std::cout << "ERROR: GAME::initEnemies() --- FAILED TO LOAD TEXTURE!\n";
	}
	this->enemyTexture.setSmooth(false);
	this->enemySprite.setTexture(this->enemyTexture);

}

void Game::initFonts()
{

	if(!this->font.loadFromFile("Fonts/Marlboro.ttf"))
	{
		std::cout << "ERROR: GAME::initFonts() --- FAILED TO LOAD FONT! \n";
	}
	
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(30);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("Default text");

	this->endGameText.setFont(this->font);
	this->endGameText.setCharacterSize(100);
	this->endGameText.setFillColor(sf::Color(30,0,100, 255));
	this->endGameText.setString("Default endgametext");

	this->getFuckedText.setFont(this->font);
	this->getFuckedText.setCharacterSize(80);
	this->getFuckedText.setFillColor(sf::Color::Yellow);
	this->getFuckedText.setString("Default taunt");

}


//Constructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}
//Destructors
Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::gameEnded() const
{
	return this->bEndGame;
}

void Game::callRestartGame()
{
	this->bEndGame = false;
	this->points = 0;
	this->health = 10;
	this->frameCounter = 0;
	this->enemies.clear();
}

void Game::spawnEnemy()
{
	//pushes current enemy onto enemies and sets colors and positions

	this->enemySprite.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemySprite.getPosition().x)), 0.f);

	this->enemies.push_back(this->enemySprite);


}

void Game::updateEnemies()
{
	/*
		@return void

		updates the enemy spawn time
		resets when at timermax
	*/

	//Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies)
	{
		//Timer is going off
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}

	}

	//Move and delete the enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		//Move and rotate with frame scaler
		this->enemies[i].rotate(1.f);
		this->enemies[i].setPosition
		(this->enemies[i].getPosition().x, 
			this->enemies[i].getPosition().y + (3.f + (0.01 * this->frameCounter)));
		

		//Despawn at bottom of screen OR to left or right of screen
		if (this->enemies[i].getPosition().y > this->window->getSize().y || 
			this->enemies[i].getPosition().x < 0						 ||
			this->enemies[i].getPosition().x > this->window->getSize().x) 
		{																	
			this->health -= 1;
			this->enemies.erase(this->enemies.begin() + i);
		}

	}

	//Check if clicked on
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->bMouseHeld == false)
		{
			this->bMouseHeld = true;
			bool bDeleted = false;

			for (size_t i = 0; i < this->enemies.size() && bDeleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//Set bDeleted true and erase enemy
					bDeleted = true;
					this->enemies.erase(this->enemies.begin() + i);

					//Gain points
					this->points += 1;
				}

			}

		}

	}
	else
	{
		this->bMouseHeld = false;
	}

}

void Game::renderEnemies(sf::RenderTarget& target)
{
	target.draw(this->bgSprite);
	//Rendering all the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);

	if (this->health == 0)
	{
		this->drawEndGame(target);
	}
}

void Game::drawEndGame(sf::RenderTarget& target)
{
	std::stringstream str;
	str << "High score: " << this->highScore;
	this->endGameText.setPosition(sf::Vector2f((max_window_width / 4.f), (max_window_height / 2.f)));
	this->endGameText.setString(str.str());
	this->getFuckedText.setString("You smell like rotten dog cum");
	this->getFuckedText.setPosition(sf::Vector2f((max_window_width / 4.f), (max_window_height / 3.f)));

	target.draw(this->endGameBgSprite);
	target.draw(this->endGameText);
	target.draw(this->restartButtonSprite);
	target.draw(this->getFuckedText);
}

void Game::updateText()
{
	this->uiText.setPosition(sf::Vector2f(10.f, 5.f));

	std::stringstream ss;

	ss << "Points: " << this->points << "\n";
	ss << "Health: " << this->health;

	this->uiText.setString(ss.str());
}

void Game::pollEvents()
{

	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
			}
			else if (this->ev.key.code == sf::Keyboard::R && bEndGame)
			{
				this->callRestartGame();
			}
			break;
			
		}
	}
}

void Game::updateMousePositions()
{
	//Update mouse position relative to windows
	//Also update mousePosView(float conversion)
	this->mousePosition = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosition);
}

void Game::update()
{
	this->frameCounter++;
	//Polls events
	this->pollEvents();

	if (this->bEndGame == false)
	{
		//Update mouse positions
		this->updateMousePositions();

		//Update enemy logic
		this->updateEnemies();

		//Update UI Text
		this->updateText();
	}

	//Set position and value of endGameText
	if (this->health <= 0)
	{
		//Set high score
		if (this->highScore < this->points)
		{
			this->highScore = this->points;
		}
		this->bEndGame = true;
	}
}

void Game::render()
{
	/*
	@return void
		-clear old frame
		-render objects
		-display frame in window

		renders game objects
	*/

	this->window->clear();

	//Draws game objects
	this->renderEnemies(*this->window);

	//Draw UI
	this->renderText(*this->window);

	this->window->display();

}
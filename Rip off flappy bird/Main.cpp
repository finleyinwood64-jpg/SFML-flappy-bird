#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
int main()
{
	float restartTimer = 5.f;
	float jumpCooldown = 0.5;
	bool showHitboxes = false;
	float buttonCooldown = 0.25;
	srand((unsigned int)time(0));
	int randomY = rand() % 400 + 100;
	float velocity = 0.f;
	float spawnTimer = 0.f;
	float spawnInterval = 2.f;
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Rip off flappy bird");
	window.setFramerateLimit(165);
	sf::SoundBuffer dieBuffer;
	if (!dieBuffer.loadFromFile("die.mp3"))
	{
		std::cout << "Failed to load die.mp3\n";
	}
	sf::Sound die(dieBuffer);
	sf::SoundBuffer jumpBuffer;
	if (!jumpBuffer.loadFromFile("jump.mp3"))
	{
		std::cout << "Failed to load jump.mp3\n";
	}
	sf::Sound jumpSound(jumpBuffer);
	sf::Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(icon);
	sf::Font mcFont;
	mcFont.openFromFile("Mojangles.ttf");
	int score = 0;
	sf::Text gameOverText(mcFont, "GAME OVER", 60);
	gameOverText.setPosition({ 250, 200 });
	sf::Text restartText(mcFont, "Restart in " + std::to_string(restartTimer), 30);
	restartText.setPosition({ 300,350 });
	restartText.setFillColor(sf::Color::White);
	sf::Text scoreText(mcFont, "Score: " + std::to_string(score), 30);
	sf::Clock clock;
	sf::Texture pillarTexture;
	sf::Texture playerTexture;
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("4622732.png");
	sf::Sprite background1(backgroundTexture);
	sf::Sprite background2(backgroundTexture);
	background1.setPosition({ 0, 0 });
	background1.setScale({ 2.1f, 2.2f });
	background2.setPosition({ 800, 0 });
	background2.setScale({ 2.1f, 2.2f });
	if (!pillarTexture.loadFromFile("greek-culture-pillar-free-png.png"))
	{
		std::cout << "Failed to load pillarSprite\n";
	}
	if (!playerTexture.loadFromFile("playerSprite.png"))
	{
		std::cout << "Failed to load playerSprite\n";
	}
	std::vector <sf::Sprite*> pillars;
	sf::Sprite playerSprite(playerTexture);
	playerSprite.setScale({ 0.1f, 0.1f });
	playerSprite.setPosition({ 400, 300 });
	while (window.isOpen())
	{
		int randomY = rand() % 400;
		float deltaTime = clock.restart().asSeconds();
		buttonCooldown -= deltaTime;
		jumpCooldown -= deltaTime;
		spawnTimer += deltaTime;
		velocity += 500.f * deltaTime;
		sf::FloatRect background1Position = background1.getGlobalBounds();
		sf::FloatRect background2Position = background2.getGlobalBounds();
		if ((background1Position.position.x + background1Position.size.x) <= 0)
		{
			background1.setPosition({ background2Position.position.x + background2Position.size.x -20, 0 });
		}
		if ((background2Position.position.x + background2Position.size.x) <= 0)
		{
			background2.setPosition({ background1Position.position.x + background1Position.size.x -20, 0 });
		}
		float pillarSpeed = 200.f + (score * 10.f);
		float backgroundSpeed = 200.f + (score * 10.f);
		background1.move({ -backgroundSpeed * deltaTime, 0 });
		background2.move({ -backgroundSpeed * deltaTime, 0});
		if (spawnTimer >= spawnInterval)
		{
			spawnTimer = 0;
			sf::Sprite* newPillar = new sf::Sprite(pillarTexture);
			newPillar->setScale({ 0.5f, 0.5f });
			newPillar->setPosition({ 800.f, (float)randomY });
			pillars.push_back(newPillar);
		}
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}
		sf::FloatRect playerPosition = playerSprite.getGlobalBounds();
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && playerPosition.position.y > 0) && jumpCooldown <= 0)
		{
			jumpCooldown = 0.5;
			velocity = -300.f;
			jumpSound.play();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && playerPosition.position.x > 0)
		{
			playerSprite.move({ -200.f * deltaTime, 0 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && (playerPosition.position.x + playerPosition.size.x) < 800)
		{
			playerSprite.move({ 200.f * deltaTime, 0 });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3) && buttonCooldown <= 0)
		{
			showHitboxes = !showHitboxes;
			buttonCooldown = 0.5;
		}
		playerSprite.move({ 0, velocity * deltaTime });
		if ((playerPosition.position.y + playerPosition.size.y) >= 600)
		{
			velocity = 0.f;
			playerSprite.setPosition({ playerPosition.position.x, 599 - playerPosition.size.y });
		}
		if (playerPosition.position.y <= 0)
		{
			velocity = 0.f;
			playerSprite.setPosition({ playerPosition.position.x, 1 });
		}
		for (int i = 0; i < pillars.size(); i++)
		{
			sf::FloatRect pillarBounds = pillars[i]->getGlobalBounds();
			sf::FloatRect smallerPillarBounds
			(
				{ pillarBounds.position.x + 80, pillarBounds.position.y + 10 },
				{ pillarBounds.size.x - 155, pillarBounds.size.y - 40 }
			);
			if (playerSprite.getGlobalBounds().findIntersection(smallerPillarBounds))
			{
				die.play();
				float goingUpSpeed = -100;
				velocity = 170;
				while (window.isOpen())
				{
					if (goingUpSpeed <= 0)
					{
						playerSprite.move({ 0, goingUpSpeed * deltaTime });
						goingUpSpeed += (deltaTime * 170);
					}
					else
					{
						playerSprite.move({ 0, velocity * deltaTime });
						velocity += (deltaTime * 300);
					}
					while (std::optional event = window.pollEvent())
					{
						if (event->is<sf::Event::Closed>())
						{
							window.close();
						}
					}
					deltaTime = clock.restart().asSeconds();
					if (restartTimer > 0)
					{
						restartTimer -= deltaTime;
					}
					if (restartTimer < 0)
					{
						restartTimer = 0;
					}
					std::ostringstream ss;
					ss << std::fixed << std::setprecision(1) << restartTimer;
					restartText.setString("Restart in " + ss.str());
					sf::FloatRect background1Position = background1.getGlobalBounds();
					sf::FloatRect background2Position = background2.getGlobalBounds();
					if ((background1Position.position.x + background1Position.size.x) <= 0)
					{
						background1.setPosition({ background2Position.position.x + background2Position.size.x -20, 0 });
					}
					if ((background2Position.position.x + background2Position.size.x) <= 0)
					{
						background2.setPosition({ background1Position.position.x + background1Position.size.x -20, 0 });
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && restartTimer <= 0)
					{
						for (int i = 0; i < pillars.size(); i++)
						{
							delete pillars[i];
						}
						pillars.clear();
						score = 0;
						scoreText.setString("Score " + std::to_string(score));
						restartTimer = 5.f;
						spawnTimer = 0.f;
						spawnInterval = 2.f;
						velocity = 0.f;
						playerSprite.setPosition({ 400,300 });
						scoreText.setPosition({ 1, 1 });

						break;
					}
					background1.move({ -backgroundSpeed * deltaTime, 0 });
					background2.move({ -backgroundSpeed * deltaTime, 0 });
					scoreText.setPosition({ 300, 300 });
					window.clear();
					window.draw(background1);
					window.draw(background2);
					for (int i = 0; i < pillars.size(); i++)
					{
						window.draw(*pillars[i]);
					}
					window.draw(playerSprite);
					window.draw(gameOverText);
					window.draw(restartText);
					window.draw(scoreText);
					window.display();
				}
				break;
			}
			if (pillars[i]->getPosition().x < 0)
			{
				score += 1;
				scoreText.setString("Score: " + std::to_string(score));
				delete pillars[i];
				pillars.erase(pillars.begin() + i);
				i--;
				continue;
			}
			pillars[i]->move({ -pillarSpeed * deltaTime, 0 });
		}
		sf::RectangleShape hitboxesplayer({ playerPosition.size.x, playerPosition.size.y });
		hitboxesplayer.setPosition({ playerPosition.position.x, playerPosition.position.y });
		hitboxesplayer.setFillColor(sf::Color::Transparent);
		hitboxesplayer.setOutlineColor(sf::Color::Red);
		hitboxesplayer.setOutlineThickness(2);
		window.clear();
		window.draw(background1);
		window.draw(background2);
		window.draw(scoreText);
		for (int i = 0; i < pillars.size(); i++)
		{
			window.draw(*pillars[i]);
			sf::FloatRect pillarBounds = pillars[i]->getGlobalBounds();
			sf::FloatRect smallerPillarBounds
			(
				{ pillarBounds.position.x + 80, pillarBounds.position.y + 10 },
				{ pillarBounds.size.x - 155, pillarBounds.size.y - 30 }
			);
			sf::RectangleShape hitboxesPillar({ smallerPillarBounds.size.x, smallerPillarBounds.size.y });
			hitboxesPillar.setPosition({ smallerPillarBounds.position.x, smallerPillarBounds.position.y });
			hitboxesPillar.setFillColor(sf::Color::Transparent);
			hitboxesPillar.setOutlineColor(sf::Color::Red);
			hitboxesPillar.setOutlineThickness(2);
			if (showHitboxes == true)
			{
				window.draw(hitboxesplayer);
				window.draw(hitboxesPillar);
			}
		}
		window.draw(playerSprite);
		window.display();
	}
}
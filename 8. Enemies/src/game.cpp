#include <game.hpp>
#include <iostream>

namespace ac
{
	const float pi = acos(-1);

	Game::Game(int width, int height, const std::string &capture)
	{
		m_settings.antialiasingLevel = 8;
		m_width = width;
		m_height = height;
		m_capture = capture;
		m_window.create(sf::VideoMode(m_width, m_height), m_capture);
	}

	bool Game::Setup(int n)
	{

		m_n = n;
		srand(time(0));
		m_c = new Circle[m_n];

		if (!m_textureBackground.loadFromFile("assets\\background.jpg"))
		{
			std::cout << "Error while loading background.jpg" << std::endl;
			return false;
		}

		if (!m_font.loadFromFile("assets\\arial.ttf"))
			{
				std::cout << "Error while loading arial.ttf" << std::endl;
				return false;
			}
			m_fpsText.setFont(m_font);
			
		m_spriteBackground.setTexture(m_textureBackground);

		if (!m_ship.Setup(100, 100))
			return false;

		

		for (int i = 0; i < n; i++)
		{
			int r = rand() % 90 + 10;
			int x = rand() % (m_width - 2 * r) + r;
			int y = rand() % (m_height - 2 * r) + r;
			int vx = rand() % 10 + 500;
			int vy = rand() % 10 + 500;

			m_c[i].Setup(x, y, r, vx, vy, n);
			

			for (int j = 0; j < i; j++)
			{
				if ((m_c[i].CheckCollision(m_c[j])))
				{
					r = rand() % 90 + 10;
					x = rand() % (m_width - 2 * r) + r;
					y = rand() % (m_height - 2 * r) + r;
					m_c[i].Setup(x, y, r, vx, vy, n);
					j = -1;
				}
			}
		}
		m_SpaceObjects.push_back(new UFO);
		m_SpaceObjects[0]->Setup(400, 400, 50, 50, "assets//UFO2.png", 0.2f);

		m_SpaceObjects.push_back(new Destroyer);
		m_SpaceObjects[1]->Setup(250, 500, 50, 50, "assets//Destroyer.png", 0.2f);



		m_Enemies.push_back(new Asteroid);
		m_Enemies.push_back(new Asteroid);
		m_Enemies.push_back(new Asteroid);
		m_Enemies.push_back(new Asteroid);
		m_Enemies.push_back(new Asteroid);

		m_Enemies.push_back(new Comet);



	}

	
	void Game::LifeCycle()
	{
		sf::Clock clock;
		sf::Clock bullet_timeout_clock;
		sf::Clock enemies_timeout_clock;

		
		

		m_window.setFramerateLimit(60);
		while (m_window.isOpen() && is_not_game_over)
		{
			float dt = clock.getElapsedTime().asSeconds();
			clock.restart();


			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				m_ship.setVelocity(1000, dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				m_ship.setVelocity(-1000, dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_ship.Rotate(-120, dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_ship.Rotate(120, dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && (bullet_timeout_clock.getElapsedTime().asSeconds() > bullet_timeout))
			{
				m_ship.Attack();
				bullet_timeout_clock.restart();

				
			}

			for (int i = 0; i < m_Enemies.size()-1; i++)
				if ((m_Enemies[i]->Y()>m_height) || (sf::Keyboard::isKeyPressed(sf::Keyboard::L)))
				{
					m_Enemies[i]->Setup(rand() % m_width, -100, 0, rand()%100+100, "assets//asteroid2.png", 0.2f);
				}

			if ((m_Enemies[m_Enemies.size()-1]->Y() > m_height) || (sf::Keyboard::isKeyPressed(sf::Keyboard::L)))
			{
				m_Enemies[m_Enemies.size()-1]->Setup(m_width + 100, -100, -100, 100, "assets//comet.png", 0.2f);
			}





			

			m_fpsText.setString(std::to_string(1 / dt));

			m_ship.Move(dt);

			for (int i=0; i<m_SpaceObjects.size(); i++)
			{
				m_SpaceObjects[i]->Move(dt);
			}

			for (int i = 0; i < m_Enemies.size(); i++)
			{
				m_Enemies[i]->Move(dt);
			}
			


			for (int i = 0; i < m_n; i++)
			{
				if (!m_c[i].TouchBorder(m_width, m_height, dt))
					m_c[i].Move(dt);
			}


			for (int i = 0; i < m_ship.GetNumberOfMaxBullets(); i++)
				for (int j = 0; j < m_n; j++)
					if (m_ship.IsBulletHit(m_c[j], i))
					{
						m_ship.HandleHit(i);
						m_c[j].Deleting();
					}
				

			for (int i = 0; i < m_n; i++)
				for (int j = i + 1; j < m_n; j++)
					if (m_c[i].CheckCollision(m_c[j]))
						m_c[i].HandleCollision(m_c[j], i, j);

			for (int i = 0; i < m_Enemies.size(); i++)				
				if (m_Enemies[i]->CheckCollisionEnemies(m_ship))
					is_not_game_over = false;
				
			
					
	
					
			m_window.clear();
			m_window.draw(m_spriteBackground);

			for (int i = 0; i < m_ship.GetNumberOfMaxBullets(); i++)
				m_window.draw(m_ship.GetBullet(i));
		
				
			m_window.draw(m_ship.Get());

			for (int i = 0; i < m_SpaceObjects.size(); i++)
			{
				m_window.draw(m_SpaceObjects[i]->Get());
			}

			for (int i = 0; i < m_Enemies.size(); i++)
			{
				m_window.draw(m_Enemies[i]->Get());
			}

			m_ship.TouchBorder(m_width, m_height);

			m_window.draw(m_fpsText);
			for (int i = 0; i < m_n; i++)
				m_window.draw(m_c[i].Get());
			m_window.display();
		}

		while (!is_not_game_over)
		{
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window.close();
			}
			m_window.clear();
			
			m_fpsText.setString("Game over!");
			

			m_fpsText.setColor(sf::Color(255, 255, 255));
			m_window.draw(m_fpsText);
			m_window.display();
			
		}

	}

}

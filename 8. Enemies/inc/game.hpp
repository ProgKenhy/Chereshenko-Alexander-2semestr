#pragma once
#include <SFML/Graphics.hpp>
#include <circle.hpp>
#include <Ship.hpp>
#include <bullet.hpp>
#include <SpaceObject.hpp>
#include <UFO.hpp>
#include <Destroyer.hpp>
#include <Enemies.hpp>
#include <comet.hpp>
#include <asteroid.hpp>

namespace ac
{

	class Game 
	{
		int m_width;
		int m_height;
		std::string m_capture;
		Circle *m_c;
		int m_n;
		float bullet_timeout = 0.3f;
		float enemies_timeout = 50;
		bool is_not_game_over = true;


		sf::RenderWindow m_window;
		sf::ContextSettings m_settings;

		sf::Texture m_textureBackground;
		sf::Sprite m_spriteBackground;
		sf::Font m_font;
		sf::Text m_fpsText;
		sf::Text m_gameOver;

		Ship m_ship;

		UFO m_ufo;
		Destroyer m_destroyer;
		Comet m_comet;
		Asteroid m_asteroid;
		std::vector<SpaceObject*> m_SpaceObjects;
		std::vector<Enemies*> m_Enemies;



	public:
		Game(int width, int height, const std::string &capture);
		bool Setup(int n);
		bool TouchBorder(Circle &obj, float dt);
		void LifeCycle();
		int N() { return m_n; }
		~Game() {
			delete[] m_c; 
			for (int i = 0; i < m_SpaceObjects.size(); i++)
				delete m_SpaceObjects[i];
			for (int i = 0; i < m_Enemies.size(); i++)
				delete m_Enemies[i];
		}
	};
}
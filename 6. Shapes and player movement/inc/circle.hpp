#pragma once
#include <SFML/Graphics.hpp>

namespace ac
{
	class Circle
	{
		int m_n;
		float m_r;
		float m_x, m_y;
		float m_vx;
		float m_vy;
		bool *m_col_is_handling;
		sf::CircleShape m_shape;

	public:
		Circle() = default;
		Circle(float x, float y, float r, float vx, float vy, int n);
		void Setup(float x, float y, float r, float vx, float vy, int n);
		void Move(float dt);
		bool Circle::CheckCollision(Circle &c2);
		void HandleCollision(Circle &c2, int i, int j);
		sf::CircleShape Get();
		float X() { return m_x; }
		float Y() { return m_y; }
		void X(float x) { m_x = x; }
		void Y(float y) { m_y = y; }
		float VX() { return m_vx; }
		float VY() { return m_vy; }
		float R() { return m_r; }
		void VX(float vx) { m_vx = vx; }
		void VY(float vy) { m_vy = vy; }
	};
}
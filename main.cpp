#include<iostream>
#include<SFML/Graphics.hpp>
#include<Windows.h>

sf::RenderWindow window({ 1500, 1000 }, "The Prime Spiral", sf::Style::Close);
sf::Event evnt;

class sfLine : public sf::Drawable
{
public:
	sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2, sf::Color color, float thickness) :
		color(color), thickness(thickness)
	{
		sf::Vector2f direction = point2 - point1;
		sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
		sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

		sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

		vertices[0].position = point1 + offset;
		vertices[1].position = point2 + offset;
		vertices[2].position = point2 - offset;
		vertices[3].position = point1 - offset;

		for (int i = 0; i < 4; ++i)
			vertices[i].color = color;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(vertices, 4, sf::Quads);
	}


private:
	sf::Vertex vertices[4];
	float thickness;
	sf::Color color;
};

constexpr size_t TotalNums = 1000;
constexpr float LineThickness = 2.f;
constexpr float LineSize = 20.f;
constexpr float CircleRadius = 5.f;


int prevStepSize = 1;
int StepCount = 1;


bool isPrime(const int& Num)
{
	for (size_t i = 2; i < Num / 2; i++)
	{
		if (Num % i == 0)
			return false;
	}
}


int main()
{
	while (window.isOpen())
	{
		while (window.pollEvent(evnt))
			if (evnt.type == sf::Event::Closed) window.close();


		window.clear();

		int Step = 1;
		int StepSize = 1;
		int StepsToIncrease = 0;

		sf::Vector2f p1{ float(window.getSize().x / 2), float(window.getSize().y / 2) };
		sf::Vector2f p2 = p1;

		float XV = LineSize;
		float YV = 0.f;
		for (size_t i = 0; i < prevStepSize; i++)
		{
			for (size_t k = 0; k < 2; k++)
			{
				for (size_t j = 0; j < StepSize; j++)
				{
					p2.x += XV;
					p2.y += YV;

					window.draw(sfLine({ p1, p2, sf::Color::White, LineThickness }));

					Step++;

					p1 = p2;

					if (isPrime(Step))
					{
						sf::CircleShape s;
						s.setRadius(CircleRadius);
						s.setPosition(p1.x - CircleRadius, p1.y - CircleRadius);
						window.draw(s);
					}
				}

				if (XV > 0.f) { XV = 0.f; YV = -LineSize; continue; }

				if (YV > 0.f) { XV = LineSize; YV = 0.f; continue; }

				if (XV < 0.f) { XV = 0.f; YV = LineSize; continue; }

				if (YV < 0.f) { XV = -LineSize; YV = 0.f; continue; }

			}
		    StepSize++;
	    }
		StepCount += (Step - StepCount);

		if (StepCount <= TotalNums)
		prevStepSize = StepSize;

		window.display();
	}
}
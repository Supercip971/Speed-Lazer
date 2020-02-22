#include "SLgineImport.h"
namespace SL
{
	int globalQuality = 3;
	sf::Clock clock;
	sf::Clock clockFromStart;
	sf::Keyboard::Key lastKeyPress;
	sf::RenderWindow* windowGlobal;
	sf::Time elapsedTime;
	bool ShouldClose = false;
	bool MouseID = false;
	float mouseScrolling = 0;
	void initSLgine(sf::RenderWindow* win) {
		printf("loading SLGINE \n");

		printf("loading clock \n");
		clock.restart();
		clockFromStart.restart();
		windowGlobal = win;
		sf::View v({ 0,0,1200,650 });/*
		windowGlobal->setVerticalSyncEnabled(true);
		windowGlobal->setFramerateLimit(60);*/
		char windowTitle[255] = "Speed Lazer";

		win->setTitle(windowTitle);

		initSLgineDraw(windowGlobal);

		printf("loading finish ! time take: %f second \n", clock.getElapsedTime().asSeconds());
	}

	int& GetQuality() {
		return globalQuality;
	}
	void SetQuality(int qu) {
		//   globalQuality = qu;
	}
	bool IsKeyPressed(sf::Keyboard::Key key) {
		if (sf::Keyboard::isKeyPressed(key))
		{
			lastKeyPress = key;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool IsKeyPressedOnce(sf::Keyboard::Key key) {
		if (sf::Keyboard::isKeyPressed(key))
		{
			if (key != lastKeyPress)
			{
				lastKeyPress = key;
				return true;
			}
			else
			{
				lastKeyPress = key;
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool IsMousePressed(sf::Mouse::Button b) {
		if (sf::Mouse::isButtonPressed(b))
		{
			if (MouseID == false)
			{
				MouseID = true;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			MouseID = false;
			return false;
		}
	}bool IsMouseRelease(sf::Mouse::Button b) {
		auto io = ImGui::GetIO();
		if (io.MouseReleased[b])
		{
			
				return true;
		}
		else
		{
			return false;
		}
	}

	float mouseScroll()
	{
		
		return mouseScrolling;
	}

	sf::Vector2i getMousePosition() {
		POINT p;
		auto i = ImGui::GetIO();
		p.x = i.MousePos.x;
		p.y = i.MousePos.y;
		sf::Vector2i v;
		v.x = p.x;
		v.y = p.y;
		v.y *= 675.f / (float)windowGlobal->getSize().y;
		v.x *= 1200.f / (float)windowGlobal->getSize().x;
		return v;
	}
	void UpdateSLgine() {
		elapsedTime = clock.restart();
		sf::Event event;
		mouseScrolling = 0;
		while (windowGlobal->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			 if (event.type == sf::Event::MouseWheelScrolled) {
			mouseScrolling = event.mouseWheelScroll.delta *10;
			 }
			 else
			 {
			 }


			if (event.type == sf::Event::Closed) {
				ShouldClose = true;
			}
			else
			{
				ShouldClose = false;
			}
		}
	}
	bool windowShouldClose() {
		return ShouldClose;
	}

	float getFps() {
		return  1 / (elapsedTime.asSeconds());
	}

	sf::Time gettime() {
		return  elapsedTime;
	}

	std::string cTStr(float t) {
		std::ostringstream ss;
		ss << t;

		return(ss.str());
	} /// convert a float to a string
	std::string cTStr(int t) {
		std::ostringstream ss;
		ss << t;

		return(ss.str());
	} std::string cTStr(size_t t) {
		std::ostringstream ss;
		ss << t;

		return(ss.str());
	}/// convert a int to a string

	std::random_device r;
	double numCallrnd = 0;
	float GetRandomVal(int min, int max) {
		numCallrnd++;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_dist(min, max);
		int mean = uniform_dist(e1);

		//srand(clockFromStart.getElapsedTime().asMicroseconds()+ gettime().asMicroseconds()+ numCallrnd);
		//if (numCallrnd < 300000)
		//{
		//    clockFromStart.restart();
		//    numCallrnd = 0;
		//}
		return mean
			/*rand() % (max - min) + min*/;
	}

	bool CheckCollisionPointRec(sf::Vector2i pos, sf::Vector2f posob, sf::Vector2f sizeob)
	{
		if (pos.x > posob.x)
		{
			if (pos.y > posob.y)
			{
				if (pos.x < posob.x + sizeob.x)
				{
					if (pos.y < posob.y + sizeob.y)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	bool CheckCollisionCircles(sf::Vector2f center1, float radius1, sf::Vector2f center2, float radius2)
	{
		bool collision = false;

		float dx = center2.x - center1.x;      // X distance between centers
		float dy = center2.y - center1.y;      // Y distance between centers

		float distance = sqrtf(dx * dx + dy * dy); // Distance between centers

		if (distance <= (radius1 + radius2)) collision = true;

		return collision;
	}// get collision circles Vec Float / Vec Float
	bool CheckCollisionCircles(sf::Vector2i center1, float radius1, sf::Vector2i center2, float radius2) {
		bool collision = false;

		float dx = center2.x - center1.x;      // X distance between centers
		float dy = center2.y - center1.y;      // Y distance between centers

		float distance = sqrtf(dx * dx + dy * dy); // Distance between centers

		if (distance <= (radius1 + radius2)) collision = true;

		return collision;
	}
	bool CheckCollisionCircleRec(sf::Vector2f circlePos, float radius, sf::Vector2f recpos, sf::Vector2f recSize)
	{
		int recCenterX = (int)(recpos.x + recSize.x / 2.0f);
		int recCenterY = (int)(recpos.y + recSize.y / 2.0f);

		float dx = (float)fabs(circlePos.x - recCenterX);
		float dy = (float)fabs(circlePos.y - recCenterY);

		if (dx > (recSize.x / 2.0f + radius)) { return false; }
		if (dy > (recSize.y / 2.0f + radius)) { return false; }

		if (dx <= (recSize.x / 2.0f)) { return true; }
		if (dy <= (recSize.y / 2.0f)) { return true; }

		float cornerDistanceSq = (dx - recSize.x / 2.0f) * (dx - recSize.x / 2.0f) +
			(dy - recSize.y / 2.0f) * (dy - recSize.y / 2.0f);

		return (cornerDistanceSq <= (radius * radius));
	}
}
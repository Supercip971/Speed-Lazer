#pragma once

#include "utils.h"
#include "Engine.h"
class GUI {
	Engine* MainEngine;
	GUI(Engine* GuiEngine) {
		MainEngine = GuiEngine;
	}
};
class IObutton {
public:
	Vector2f pos;
	float size;

	sf::Mouse m;
	Engine* eng;
	float ts = 0;
	float t = 0;
	bool IO = false;
	void Draw() {
		if (checkCollisionRecPoint({ pos.x,pos.y,size,size }, m.getPosition(eng->window).x, m.getPosition(eng->window).y))
		{
			eng->DrawRecRounded({ pos.x - 2, pos.y - 2, size + 4, size + 4 }, 0.1, 10, Color(255, 255, 255));

			if (t > 0)
			{
				ts += 2;
				t -= positiving(sin((ts)));
			}
		}
		else
		{
			eng->DrawRecRounded({ pos.x - 1, pos.y - 1, size + 2, size + 2 }, 0.1, 10, Color(90, 90, 90));
			if (t < (255 - 120))
			{
				t += 1;
			}
		}
		if (IO == true)
		{
			eng->DrawRecRounded({ pos.x , pos.y , size , size }, 0.1, 10, Color(200, 10, 10));
		}
		else
		{
			eng->DrawRecRounded({ pos.x , pos.y , size , size }, 0.1, 10, Color(10, 200, 10));
		}
		Event e;
		while (eng->window.pollEvent(e))
		{
			if (e.type == sf::Event::MouseButtonReleased) {
				if (checkCollisionRecPoint({ pos.x,pos.y,size,size }, m.getPosition(eng->window).x, m.getPosition(eng->window).y))
				{
					IO -= 1;
				}
			}
		}
	}
};

class button
{
public:
	Vector2f pos;
	Vector2f size;
	String name;
	Engine* eng;
	sf::Mouse m;
	float ts = 0;
	float t = 0;
	bool clicked = false;
	int textSize = 12;
	void Draw() {
		if (checkCollisionRecPoint({ pos.x,pos.y,size.x,size.y }, m.getPosition(eng->window).x, m.getPosition(eng->window).y))
		{
			eng->DrawRecRounded({ pos.x - 2, pos.y - 2, size.x + 4, size.y + 4 }, 0.1, 10, Color(255, 255, 255));
			eng->DrawRecRounded({ pos.x , pos.y , size.x , size.y }, 0.1, 10, Color(t + 90, t + 90, t + 120));
			if (t > 0)
			{
				ts += 2;
				t -= positiving(sin((ts)));
			}
		}
		else
		{
			eng->DrawRecRounded({ pos.x - 1, pos.y - 1, size.x + 2, size.y + 2 }, 0.1, 10, Color(90, 90, 90));
			eng->DrawRecRounded({ pos.x , pos.y , size.x , size.y }, 0.1, 10, Color(t, t, t));
			if (t < (255 - 120))
			{
				t += 1;
			}
		}
		Text t(name, eng->globalFont, textSize);

		eng->SDrawText({ pos.x + (size.x / 2 - t.getLocalBounds().width / 2), pos.y + (size.y / 2 - textSize / 2) }, textSize, name, Color::White);
		Event e;
		while (eng->window.pollEvent(e))
		{
			if (e.type == sf::Event::MouseButtonReleased) {
				if (checkCollisionRecPoint(rectangle{ pos.x,pos.y,size.x,size.y }, (float)m.getPosition(eng->window).x, (float)m.getPosition(eng->window).y))
				{
					clicked = true;
				}
			}
			else
			{
				clicked = false;
			}
		}
	}
	bool isClicked() {
		return clicked;
	}
};

class Message
{
public:

	String txt = "error : no message";
	Font elementFont;
	Message(String text) {
		txt = text;
	}
	int tclick = 0;
	void send() {
		Engine newWindow(500, 300, "message");

		button but1;
		but1.size = { newWindow.getWindowWidthf() / 2, 20 };
		but1.pos = { newWindow.getWindowWidthf() / 2 - 20, newWindow.getWindowHeightf() - 30 };
		but1.name = "ok";
		but1.eng = &newWindow;
		while (newWindow.window.isOpen()) {
			Event event;
			while (newWindow.window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					logs("closing...", LOG_NORMAL);
					newWindow.window.close();
				}
			}

			newWindow.startDraw();
			newWindow.window.clear(Color(80, 80, 85));
			newWindow.SDrawText({ 10,10 }, 12, txt, Color::Black);
			but1.Draw();
			if (but1.isClicked() && tclick == 0)
			{
				tclick = 1;
				newWindow.window.close();
			}
			else
			{
				tclick = 0;
			}

			newWindow.EndDraw();
		}
	}
};

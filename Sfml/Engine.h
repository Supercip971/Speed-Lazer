#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
#define DEG2RAD (3.14/180.0f)

struct rectangle {
	float x;
	float y;
	float width;
	float height;
};

class CustomShape
{
public:
	ConvexShape shape;
	sf::Vertex* triangle;
	int num = 0;
	int be = 0;
	void start(int b) {
		shape.setPointCount(b);
		triangle = new Vertex[b];
		be = b;
	}
	void setPoint(float x, float y) {
		shape.setPoint(num, { x,y });
		triangle[num].position = { x,y };
		num++;
	}void setPoint(float x, float y, Color col) {
		shape.setPoint(num, { x,y });
		triangle[num].position = { x,y };
		triangle[num].color = col;
		num++;
	}
	void getPoint() {
	}

private:
};

class Engine
{
public:
	Font globalFont;
	sf::RenderWindow window;
	Engine(int width, int height, sf::String title) : window(sf::VideoMode(width, height), "NULL", 7U, ContextSettings(0, 0, 8, 1, 1, ContextSettings::Attribute::Default, false))

	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;

		window.setSize(Vector2u(width, height));
		window.setTitle(title);
	}
	Font GetFont() {
		return globalFont;
	}
	void DrawCircle(int radius, sf::Vector2f pos, Color col) {
		sf::CircleShape cshape(radius);
		cshape.setPosition(pos);
		cshape.setFillColor(col);
		window.draw(cshape);
	}
	sf::RectangleShape rec;
	void DrawRec(float x, float y, float width, float height, Color col) {
		rec.setPosition({ x,y });
		rec.setSize({ width,height });

		rec.setFillColor(col);

		window.draw(rec);
		rec.~RectangleShape();
	}
	//convex.shape.setTexture(sf::Texture::loadFromImage(sf::Image::))
	void SDrawText(Vector2f pos, float size, String texts, Color col) {
		sf::Text text;
		text.setPosition(pos);
		text.setFont(globalFont);

		text.setString(texts);

		text.setCharacterSize(size); // in pixels, not points!

		text.setFillColor(col);

		window.draw(text);
		text.~Text();
	}
	void SDrawText(Vector2f pos, float size, Font font, String texts, Color col) {
		sf::Text text;

		text.setFont(font);

		text.setString(texts);

		text.setCharacterSize(size); // in pixels, not points!

		text.setFillColor(col);

		window.draw(text);
		text.~Text();
	}

	void DrawRecRounded(rectangle rec, float roundness, int segments, Color color)
	{
		// Not a rounded rectangle
		CustomShape convex;

		// resize it to 5 points

		if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1))
		{
			DrawRec(rec.x, rec.y, rec.width, rec.height, color);
			return;
		}

		if (roundness >= 1.0f) roundness = 1.0f;

		// Calculate corner radius
		float radius = (rec.width > rec.height) ? (rec.height * roundness) / 2 : (rec.width * roundness) / 2;
		if (radius <= 0.0f) return;

		// Calculate number of segments to use for the corners
		if (segments < 4)
		{
			// Calculate how many segments we need to draw a smooth circle, taken from https://stackoverflow.com/a/2244088

// Calculate the maximum angle between segments based on the error rate.
			float th = acosf(2 * powf(1 - 0.5 / radius, 2) - 1);
			segments = ceilf(2 * 3.14 / th) / 4;
			if (segments <= 0) segments = 4;
		}

		float stepLength = 90.0f / (float)segments;

		/*  Quick sketch to make sense of all of this (there are 9 parts to draw, also mark the 12 points we'll use below)
		 *  Not my best attempt at ASCII art, just preted it's rounded rectangle :)
		 *     P0                    P1
		 *       ____________________
		 *     /|                    |\
		 *    /1|          2         |3\
		 *P7 /__|____________________|__\ P2
		 *  |   |P8                P9|   |
		 *  | 8 |          9         | 4 |
		 *  | __|____________________|__ |
		 *P6 \  |P11              P10|  / P3
		 *    \7|          6         |5/
		 *     \|____________________|/
		 *     P5                    P4
		 */

		const Vector2f point[12] = { // coordinates of the 12 points that define the rounded rect (the idea here is to make things easier)
			{(float)rec.x + radius, rec.y}, {(float)(rec.x + rec.width) - radius, rec.y}, { rec.x + rec.width, (float)rec.y + radius }, // PO, P1, P2
			{rec.x + rec.width, (float)(rec.y + rec.height) - radius}, {(float)(rec.x + rec.width) - radius, rec.y + rec.height}, // P3, P4
			{(float)rec.x + radius, rec.y + rec.height}, { rec.x, (float)(rec.y + rec.height) - radius}, {rec.x, (float)rec.y + radius}, // P5, P6, P7
			{(float)rec.x + radius, (float)rec.y + radius}, {(float)(rec.x + rec.width) - radius, (float)rec.y + radius}, // P8, P9
			{(float)(rec.x + rec.width) - radius, (float)(rec.y + rec.height) - radius}, {(float)rec.x + radius, (float)(rec.y + rec.height) - radius} // P10, P11
		};

		const Vector2f centers[4] = { point[8], point[9], point[10], point[11] };
		const float angles[4] = { 180.0f, 90.0f, 0.0f, 270.0f };
		convex.start(30 + 12 * segments);
		convex.shape.setFillColor(color);
		int b = 0;
		// Draw all of the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
		for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
		{
			float angle = angles[k];
			const Vector2f center = centers[k];
			for (int i = 0; i < segments; i++)
			{
				convex.setPoint(center.x, center.y);

				convex.setPoint(center.x + sinf(DEG2RAD * angle) * radius, center.y + cosf(DEG2RAD * angle) * radius);

				convex.setPoint(center.x + sinf(DEG2RAD * (angle + stepLength)) * radius, center.y + cosf(DEG2RAD * (angle + stepLength)) * radius);
				angle += stepLength;
			}
		}

		// [2] Upper Rectangle
	  // [2] Upper Rectangle
		convex.setPoint(point[0].x, point[0].y);
		convex.setPoint(point[8].x, point[8].y);
		convex.setPoint(point[9].x, point[9].y);
		convex.setPoint(point[1].x, point[1].y);
		convex.setPoint(point[0].x, point[0].y);
		convex.setPoint(point[9].x, point[9].y);

		// [4] Right Rectangle
		convex.setPoint(point[9].x, point[9].y);
		convex.setPoint(point[10].x, point[10].y);
		convex.setPoint(point[3].x, point[3].y);
		convex.setPoint(point[2].x, point[2].y);
		convex.setPoint(point[9].x, point[9].y);
		convex.setPoint(point[3].x, point[3].y);
		// [6] Bottom Rectangle
		convex.setPoint(point[11].x, point[11].y);
		convex.setPoint(point[5].x, point[5].y);
		convex.setPoint(point[4].x, point[4].y);
		convex.setPoint(point[10].x, point[10].y);
		convex.setPoint(point[11].x, point[11].y);
		convex.setPoint(point[4].x, point[4].y);

		// [8] Left Rectangle
		convex.setPoint(point[7].x, point[7].y);
		convex.setPoint(point[6].x, point[6].y);
		convex.setPoint(point[11].x, point[11].y);
		convex.setPoint(point[8].x, point[8].y);
		convex.setPoint(point[7].x, point[7].y);
		convex.setPoint(point[11].x, point[11].y);

		// [9] Middle Rectangle
		convex.setPoint(point[8].x, point[8].y);
		convex.setPoint(point[11].x, point[11].y);
		convex.setPoint(point[10].x, point[10].y);
		convex.setPoint(point[9].x, point[9].y);
		convex.setPoint(point[8].x, point[8].y);
		convex.setPoint(point[10].x, point[10].y);

		window.draw(convex.shape);
	}

	void DrawRecRoundedGradient(rectangle rec, float roundness, int segments, Color color1, Color color2)
	{
		// Not a rounded rectangle
		CustomShape convex;

		// resize it to 5 points

		if ((roundness <= 0.0f) || (rec.width < 1) || (rec.height < 1))
		{
			DrawRec(rec.x, rec.y, rec.width, rec.height, color1);
			return;
		}

		if (roundness >= 1.0f) roundness = 1.0f;

		// Calculate corner radius
		float radius = (rec.width > rec.height) ? (rec.height * roundness) / 2 : (rec.width * roundness) / 2;
		if (radius <= 0.0f) return;

		// Calculate number of segments to use for the corners
		if (segments < 4)
		{
			// Calculate the maximum angle between segments based on the error rate.
			float th = acosf(2 * powf(1 - 0.5 / radius, 2) - 1);
			segments = ceilf(2 * 3.14 / th) / 4;
			if (segments <= 0) segments = 4;
		}

		float stepLength = 90.0f / (float)segments;

		/*  Quick sketch to make sense of all of this (there are 9 parts to draw, also mark the 12 points we'll use below)
		 *  Not my best attempt at ASCII art, just preted it's rounded rectangle :)
		 *     P0                    P1
		 *       ____________________
		 *     /|                    |\
		 *    /1|          2         |3\
		 *P7 /__|____________________|__\ P2
		 *  |   |P8                P9|   |
		 *  | 8 |          9         | 4 |
		 *  | __|____________________|__ |
		 *P6 \  |P11              P10|  / P3
		 *    \7|          6         |5/
		 *     \|____________________|/
		 *     P5                    P4
		 */

		const Vector2f point[12] = { // coordinates of the 12 points that define the rounded rect (the idea here is to make things easier)
			{(float)rec.x + radius, rec.y}, {(float)(rec.x + rec.width) - radius, rec.y}, { rec.x + rec.width, (float)rec.y + radius }, // PO, P1, P2
			{rec.x + rec.width, (float)(rec.y + rec.height) - radius}, {(float)(rec.x + rec.width) - radius, rec.y + rec.height}, // P3, P4
			{(float)rec.x + radius, rec.y + rec.height}, { rec.x, (float)(rec.y + rec.height) - radius}, {rec.x, (float)rec.y + radius}, // P5, P6, P7
			{(float)rec.x + radius, (float)rec.y + radius}, {(float)(rec.x + rec.width) - radius, (float)rec.y + radius}, // P8, P9
			{(float)(rec.x + rec.width) - radius, (float)(rec.y + rec.height) - radius}, {(float)rec.x + radius, (float)(rec.y + rec.height) - radius} // P10, P11
		};

		const Vector2f centers[4] = { point[8], point[9], point[10], point[11] };
		const float angles[4] = { 180.0f, 90.0f, 0.0f, 270.0f };
		convex.start(30 + 12 * segments);

		// Draw all of the 4 corners: [1] Upper Left Corner, [3] Upper Right Corner, [5] Lower Right Corner, [7] Lower Left Corner
		for (int k = 0; k < 4; ++k) // Hope the compiler is smart enough to unroll this loop
		{
			float angle = angles[k];
			const Vector2f center = centers[k];
			for (int i = 0; i < segments; i++)
			{
				if (k == 0 || k == 1)
				{
					convex.setPoint(center.x, center.y, color1);

					convex.setPoint(center.x + sinf(DEG2RAD * angle) * radius, center.y + cosf(DEG2RAD * angle) * radius, color1);

					convex.setPoint(center.x + sinf(DEG2RAD * (angle + stepLength)) * radius, center.y + cosf(DEG2RAD * (angle + stepLength)) * radius, color1);
				}
				else
				{
					convex.setPoint(center.x, center.y, color2);

					convex.setPoint(center.x + sinf(DEG2RAD * angle) * radius, center.y + cosf(DEG2RAD * angle) * radius, color2);

					convex.setPoint(center.x + sinf(DEG2RAD * (angle + stepLength)) * radius, center.y + cosf(DEG2RAD * (angle + stepLength)) * radius, color2);
				}
				angle += stepLength;
			}
		}

		// [2] Upper Rectangle
	  // [2] Upper Rectangle
		convex.setPoint(point[0].x, point[0].y, color1);
		convex.setPoint(point[8].x, point[8].y, color1);
		convex.setPoint(point[9].x, point[9].y, color1);
		convex.setPoint(point[1].x, point[1].y, color1);
		convex.setPoint(point[0].x, point[0].y, color1);
		convex.setPoint(point[9].x, point[9].y, color1);

		// [4] Right Rectangle
		convex.setPoint(point[9].x, point[9].y, color1);
		convex.setPoint(point[10].x, point[10].y, color2);
		convex.setPoint(point[3].x, point[3].y, color2);
		convex.setPoint(point[2].x, point[2].y, color1);
		convex.setPoint(point[9].x, point[9].y, color1);
		convex.setPoint(point[3].x, point[3].y, color2);
		// [6] Bottom Rectangle
		convex.setPoint(point[11].x, point[11].y, color2);
		convex.setPoint(point[5].x, point[5].y, color2);
		convex.setPoint(point[4].x, point[4].y, color2);
		convex.setPoint(point[10].x, point[10].y, color2);
		convex.setPoint(point[11].x, point[11].y, color2);
		convex.setPoint(point[4].x, point[4].y, color2);

		// [8] Left Rectangle
		convex.setPoint(point[7].x, point[7].y, color1);
		convex.setPoint(point[6].x, point[6].y, color2);
		convex.setPoint(point[11].x, point[11].y, color2);
		convex.setPoint(point[8].x, point[8].y, color1);
		convex.setPoint(point[7].x, point[7].y, color1);
		convex.setPoint(point[11].x, point[11].y, color2);

		// [9] Middle Rectangle
		convex.setPoint(point[8].x, point[8].y, color1);
		convex.setPoint(point[11].x, point[11].y, color2);
		convex.setPoint(point[10].x, point[10].y, color2);
		convex.setPoint(point[9].x, point[9].y, color1);
		convex.setPoint(point[8].x, point[8].y, color1);
		convex.setPoint(point[10].x, point[10].y, color2);
		convex.getPoint();
		window.draw(convex.triangle, convex.num, sf::Triangles);
	}

	int getWindowWidth() {
		return window.getSize().x;
	}int getWindowHeight() {
		return window.getSize().y;
	}float getWindowWidthf() {
		return window.getSize().x;
	}float getWindowHeightf() {
		return window.getSize().y;
	}
	void startDraw() {
		window.clear();
	}
	void EndDraw() {
		window.display();
	}
};

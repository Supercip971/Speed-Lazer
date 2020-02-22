#pragma once
#include <iostream>
#include "Engine.h"
#define LOG_ERROR 2
#define LOG_NORMAL 0
#define LOG_SUCCES 1
#define LOG_WARNING 3

float positiving(float e) {
	return sqrt(e * e);
}

bool checkCollisionRecPoint(rectangle rec, Vector2f pos) {
	bool collision = false;

	if ((pos.x >= rec.x) && (pos.x <= (rec.x + rec.width)) && (pos.y >= rec.y) && (pos.y <= (rec.y + rec.height))) collision = true;

	return collision;
}
bool checkCollisionRecPoint(rectangle rec, Vector2u pos) {
	bool collision = false;

	if ((pos.x >= rec.x) && (pos.x <= (rec.x + rec.width)) && (pos.y >= rec.y) && (pos.y <= (rec.y + rec.height))) collision = true;

	return collision;
}bool checkCollisionRecPoint(rectangle rec, Vector2i posi) {
	Vector2f pos = (Vector2f)posi;
	bool collision = false;

	if ((pos.x >= rec.x) && (pos.x <= (rec.x + rec.width)) && (pos.y >= rec.y) && (pos.y <= (rec.y + rec.height))) collision = true;

	return collision;
}
bool checkCollisionRecPoint(rectangle rec, float posx, float posy) {
	bool collision = false;

	if ((posx >= rec.x) && (posx <= (rec.x + rec.width)) && (posy >= rec.y) && (posy <= (rec.y + rec.height))) collision = true;

	return collision;
}
void logs(std::string log, int logtype = LOG_NORMAL) {
	// you can loop k higher to see more color choices

		// pick the colorattribute k you want
	switch (logtype)
	{
	case LOG_NORMAL:
		std::cout << "[log] " << log << std::endl;
		break;
	case LOG_SUCCES:
		std::cout << "[log] " << log << std::endl;
		break;
	case LOG_WARNING:
		std::cout << "[WARNING] " << log << std::endl;
		break;
	case LOG_ERROR:
		std::cout << "[ERROR] " << log << std::endl;
		break;
	default:
		break;
	}
}
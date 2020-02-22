#pragma once
#include <string>
#include "json.hpp"
#include "SFML/Main.hpp"
#include "SFML/Audio.hpp"
#include <fstream>
#include <iostream>
#include "SLgineImport.h"
enum LevDifficulty
{
	NULLDIF = -1,
	SUPEREASY = 0,
	EASY = 1,
	NORMAL = 2,
	HARD = 3,
	VERYHARD = 4,
	HARDCORE = 5,
	DEATH = 6
};
int parsVer = 1;

std::string GetDifficulty(int l) {
	switch (l)
	{
	case LevDifficulty::NULLDIF:
		return ("null");
		break;
	case LevDifficulty::SUPEREASY:
		return ("difficulty : super easy");
		break;
	case LevDifficulty::EASY:
		return ("difficulty : easy");
		break;
	case LevDifficulty::NORMAL:
		return ("difficulty : normal");
		break;
	case LevDifficulty::HARD:
		return ("difficulty : hard");
		break;
	case LevDifficulty::VERYHARD:
		return ("difficulty : very hard");
		break;
	case LevDifficulty::HARDCORE:
		return ("difficulty : hardcore");
		break;
	case LevDifficulty::DEATH:
		return ("difficulty : death");
		break;
	default:
		break;
	}
}
class Proj
{
public:
	int size = 0;
	int tickcreated = 1;
	sf::Vector2f projpos;
	sf::Vector2f projvel;
	sf::Vector2f acceleration = { 1.f,1.f };
	int id = 0; // 0= global
	sf::Color projCol = sf::Color::White;
	nlohmann::json* part = new nlohmann::json();
	Proj() {
		projpos = { 0,0 };
		projvel = { 0,0 };
	}
	Proj(sf::Vector2f pos, sf::Vector2f vel) {
		projpos = pos;
		projvel = vel;
	}
	Proj(sf::Vector2f posi, sf::Vector2f vel, int sizes) {
		projpos = posi;
		projvel = vel;
		size = sizes;
	}

	void update() {
		projpos += projvel;
		projvel.x *= acceleration.x;
		projvel.y *= acceleration.y;
	}
	bool available = false;

	void Draw(int mapSize) {
		SL::DrawCircle(projpos, size, projCol);
		if (projpos.x - size < 1200 / 2 - mapSize - 1)
		{
			available = false;
		}

		else if (projpos.x + size > (1200 / 2 - mapSize - 1) + mapSize * 2)
		{
			available = false;
		}
	}
};

sf::Texture playertxt = sf::Texture();
bool playertxtinit = false;
class Player
{
public:
	Player();
	~Player();

	SL::Particle prt = SL::Particle();

	SL::ParticleMap* plyrParticle = new SL::ParticleMap();
	SL::ParticleType plyrPartType = SL::PARTICLE_CIRCLE;
	SL::ParticleEmmiter plyrPatEmit = SL::ParticleEmmiter(plyrParticle);

	sf::Vector2f pos = { 0,0 };
	sf::Vector2f Velocity = { 0,0 };

	int time = 0;

	int life = 3;

	sf::Sprite sprite = sf::Sprite();
	void JDraw() {
		plyrParticle->draw();

		sprite.setPosition(pos);
		if (time != 0) {
			sprite.setColor({ 255, sf::Uint8(255 - (time * 2 + 50)),sf::Uint8(255 - (time * 2 + 50)), 255 });
		}
		else
		{
			sprite.setColor(sf::Color::White);
		}

		SL::DrawSprite(sprite);
	}
	void Draw() {
		plyrParticle->draw();

		if (SL::IsKeyPressed(sf::Keyboard::Left))
		{
			Velocity.x -= 6;
			if (SL::IsKeyPressed(sf::Keyboard::LShift))
			{
				Velocity.x += 4;
			}
		}if (SL::IsKeyPressed(sf::Keyboard::Right))
		{
			if (SL::IsKeyPressed(sf::Keyboard::LShift))
			{
				Velocity.x -= 4;
			}
			Velocity.x += 6;
		}

		if (SL::IsKeyPressed(sf::Keyboard::Up))
		{
			Velocity.y -= 6;
			if (SL::IsKeyPressed(sf::Keyboard::LShift))
			{
				Velocity.y += 4;
			}
		}if (SL::IsKeyPressed(sf::Keyboard::Down))
		{
			if (SL::IsKeyPressed(sf::Keyboard::LShift))
			{
				Velocity.y -= 4;
			}
			Velocity.y += 6;
		}
		pos += Velocity;
		Velocity.x *= 0.4;
		Velocity.y *= 0.4;
		sprite.setPosition(pos);
		if (time != 0) {
			sprite.setColor({ 255, sf::Uint8(255 - (time * 2 + 50)),sf::Uint8(255 - (time * 2 + 50)), 255 });
		}
		else
		{
			sprite.setColor(sf::Color::White);
		}

		time -= 1;
		if (time < 0)
		{
			time = 0;
		}
		SL::DrawSprite(sprite);
	}

	void update(int sizeOfMap) {
		Draw();
		while (pos.x < 1200 / 2 - sizeOfMap - 1)
		{
			pos.x += 0.1f;
		}
		while (pos.x + playertxt.getSize().x * 0.3 > (1200 / 2 - sizeOfMap - 1) + sizeOfMap * 2)
		{
			pos.x -= 0.1f;
		}
		plyrParticle->Update();
	}
	void smartupdate(int sizeOfMap) {
		while (pos.x < 1200 / 2 - sizeOfMap - 1)
		{
			pos.x += 0.1f;
		}
		while (pos.x + playertxt.getSize().x * 0.3 > (1200 / 2 - sizeOfMap - 1) + sizeOfMap * 2)
		{
			pos.x -= 0.1f;
		}
		plyrParticle->Update();
	}

	bool CheckWithProj(Proj prj) {
		if (SL::CheckCollisionCircleRec(prj.projpos, prj.size, pos, { playertxt.getSize().x * 0.3f, playertxt.getSize().y * 0.3f }))
		{
			plyrPatEmit.pos = pos;

			for (int i = 0; i < 500; i++)
			{
				plyrPatEmit.particleToEmit = prt;

				prt.direction = { SL::GetRandomVal(-20,20) / 10 + ((prj.projvel.x * (SL::GetRandomVal(-0,20) / 10))),SL::GetRandomVal(-20,20) / 10 + ((prj.projvel.y * (SL::GetRandomVal(-0,20) / 10))) };
				prt.size = SL::GetRandomVal(5, 30);
				prt.lifeTime = SL::GetRandomVal(5, 150);
				plyrPatEmit.numParticleAdded = 1;
				plyrPatEmit.Update();
			}

			plyrPatEmit.numParticleAdded = 0;
			life -= 1;
			time = 100;
			return true;
		}
		else
		{
			return false;
		}
	}

	void reset() {
		pos = { 1200 / 2.f,675 / 2.f };
		life = 3;
		time = 0; Velocity = { 0,0 };
	}
private:
};

Player::Player()
{
	prt.baseSize = 2;
	prt.size = 2;
	prt.pCol = sf::Color::Red;
	prt.direction = { 0.f,0.f };

	prt.lifeTime = 100;
	prt.ptype = SL::PARTICLE_CIRCLE;
	prt.sizeChange = true;
	prt.colorFade = sf::Color::Red;
	plyrPatEmit.numParticleAdded = 1;
	plyrPatEmit.particleToEmit = prt;
	plyrPatEmit.pos = pos;
	plyrPatEmit.map = plyrParticle;
	if (playertxtinit == false)
	{
		playertxtinit = true;
		if (!playertxt.loadFromFile("ressource/player.png"))
		{
			printf("ERROR can't load : ressource/player.png !");
		}
		playertxt.setSmooth(true);
	}

	sprite.setTexture(playertxt);
	sprite.scale({ 0.3,0.3 });
	pos = { 1200 / 2.f,675 / 2.f };
	sprite.setPosition(pos);
	sprite.setRotation(0);
	sprite.setOrigin(0, 0);
}

Player::~Player()
{
}

class dataMap
{
public:

	int numberProj = 500;
	Proj* ProjectileMap = new Proj[numberProj];
	Player plyr = Player();
	int mapSize = 0;
	void Update() {
		for (int i = 0; i < numberProj; i++)
		{
			if (ProjectileMap[i].available)
			{
				ProjectileMap[i].update();
				ProjectileMap[i].Draw(mapSize);
				if (plyr.time == 0)
				{
					plyr.CheckWithProj(ProjectileMap[i]);
				}
			}
		}
		plyr.update(mapSize);
	}void smartUpdate() {
		for (int i = 0; i < numberProj; i++)
		{
			if (ProjectileMap[i].available)
			{
				ProjectileMap[i].update();
			}
		}
		//plyr.smartupdate(mapSize);
	}
	void draw() {
		for (int i = 0; i < numberProj; i++)
		{
			if (ProjectileMap[i].available)
			{
				ProjectileMap[i].Draw(mapSize);
			}
		}plyr.JDraw();
	}
	void spawnProj(Proj proj) {
		for (int i = 0; i < numberProj; i++)
		{
			if (!ProjectileMap[i].available)
			{
				ProjectileMap[i] = proj;
				ProjectileMap[i].available = true;
				return;
			}
		}
	}
	void reset() {
		plyr.reset();
		for (int i = 0; i < 300; i++)
		{
			if (ProjectileMap[i].available)
			{
				ProjectileMap[i] = Proj();
			}
		}
	}

	dataMap();

private:
};

dataMap::dataMap()
{
	plyr = Player();
	for (int i = 0; i < 300; i++)
	{
		ProjectileMap[i] = Proj();
	}
}
class TickLev {
public:

	int size = -1;

	int numPrj = 0;
	Proj* prjtoSpawn;

	TickLev(int numberPrj) {
		if (prjtoSpawn != nullptr)
		{
			delete[]prjtoSpawn;
		}
		prjtoSpawn = new Proj[numberPrj];
	}

	TickLev() {
	}
};
std::fstream levelLstream;
#define NULLDATA -1010101 // i'm a bad coder
class Event {
public:
	int idtriggered = 0;
	Proj dataChanged;

	bool relative = false;
	bool changevel = false;
	bool changePos = false;
	bool changeSize = false;
	bool changeColor = false;
	bool changeAcceleration = false;
	Event() {
		dataChanged.available = true;
		dataChanged.projCol = sf::Color::White;
		dataChanged.projpos = { 0,0 }; // -1 = null
		dataChanged.projvel = { 0,0 };
		dataChanged.size = 0;
		dataChanged.acceleration = { 1.f,1.f };
	}
	void Apply(Proj* prjToApply) {
		if (relative && changePos)
		{
			prjToApply->projpos += dataChanged.projpos;
		}if (relative && changevel)
		{
			prjToApply->projvel += dataChanged.projvel;
		}if (!relative && changevel)
		{
			prjToApply->projvel = dataChanged.projvel;
		}if (changeSize)
		{
			prjToApply->size = dataChanged.size;
		}

		if (changeColor)
		{
			prjToApply->projCol = dataChanged.projCol;
		}if (changeAcceleration)
		{
			prjToApply->acceleration = dataChanged.acceleration;
		}
	}
};
class Background {
public:
	sf::Color down = sf::Color::Red;
	sf::Color up = sf::Color::Blue;
};
template<typename T > T getData(T defaultdat, nlohmann::json* j) {
	try
	{
		return(j->get<T>());
	}
	catch (nlohmann::json::parse_error & e)
	{
		*j = defaultdat;
		return (defaultdat);
	}
	catch (nlohmann::json::type_error & e)
	{
		*j = defaultdat;
		return (defaultdat);
	}
}

class Level
{
public:

	bool threading = false;
	dataMap Map = dataMap();
	dataMap threadMap = dataMap();
	std::string name;
	int difficulty = NULLDIF;
	bool OnlineLev = false;
	float ver = 0.1;
	bool available = false;
	std::string music;
	std::string musicCName;
	std::string musPath;
	int numberOfDeath = 0;
	int numberOfTick = 0;
	bool completed = false;
	bool edit = false;
	bool* updated = new bool(false);
	float size = 400;
	int sizemax = 400;
	int update = 0;
	Background levelBackground;
	std::string pathLevel = "";
	sf::Music* song = new sf::Music();
	nlohmann::json levelL;
	nlohmann::json threadLevelL;
	std::string levelDatastr;
	void threadBegin() {
	}
	void theadEnd() {
	}
	void save() {
		if (threading != true)
		{
			levelLstream.open(pathLevel, std::fstream::binary | std::fstream::out | std::fstream::trunc);;
			levelLstream << levelL;
			levelLstream.close();
			printf("a thread is running wait a moment...");
		}
	}
	void loadFromFile(std::string filePath) {
		pathLevel = filePath;

		//	<< doc.child("mesh").attribute("name").value() << std::endl;
		try
		{
			levelL.~basic_json();
			levelL = nlohmann::json();
			levelLstream.open(pathLevel, std::fstream::in | std::fstream::binary);;

			levelLstream >> levelL;
			levelLstream.close();
			//printf("load : %s", levelL); //This will print the entire json object.)

			// parsing input with a syntax error
			size = getData<int>(100, &levelL["level"]["size"]);

			name = getData<std::string>("no name", &levelL["level"]["name"]);;
			sizemax = getData<int>(100, &levelL["level"]["tickMax"]);;
			difficulty = getData<int>(-1, &levelL["level"]["difficulty"]);;
			ver = getData<int>(0, &levelL["level"]["version"]);;

			//sizemax = doc.child("level").child("song").attribute("levtime").as_int();

			music = getData<std::string>("music name", &levelL["level"]["song"]["songName"]);;
			musicCName = getData<std::string>("music creator", &levelL["level"]["song"]["songCreator"]);;
			musPath = getData<std::string>("music path", &levelL["level"]["song"]["path"]);;
			levelBackground.up = {
				getData<unsigned char>(0,&levelL["level"]["background"]["up"]["r"]),
				getData<unsigned char>(0,&levelL["level"]["background"]["up"]["g"]),
				getData<unsigned char>(0,&levelL["level"]["background"]["up"]["b"]) };
			levelBackground.down = {
				getData<unsigned char>(0,&levelL["level"]["background"]["down"]["r"]),
				getData<unsigned char>(0,&levelL["level"]["background"]["down"]["g"]),
				getData<unsigned char>(0,&levelL["level"]["background"]["down"]["b"]) };
		}
		catch (nlohmann::json::parse_error & e)
		{
			printf(std::string("load result for" + filePath + " json, " + std::string(levelL.dump())).c_str());
			// output exception information
			std::cout << "message: " << e.what() << '\n'
				<< "exception id: " << e.id << '\n'
				<< "byte position of error: " << e.id << std::endl;
		}
		catch (nlohmann::json::type_error & e)
		{
			printf(std::string("load result for" + filePath + " json, " + std::string(levelL.dump())).c_str());
			// output exception information
			std::cout << "message: " << e.what() << '\n'
				<< "exception id: " << e.id << '\n'
				<< "byte position of error: " << e.id << std::endl;
		}

		printf(musPath.c_str());
		song->openFromFile(musPath);
		available = true;

		Map.mapSize = size;

		//	doc.insert_attribute_after("size", pugi::xml_attribute::);*/
	}
	void smartloadFromFile(std::string filePath) {
		pathLevel = filePath;

		//	<< doc.child("mesh").attribute("name").value() << std::endl;
		try
		{
			//printf("load : %s", levelL); //This will print the entire json object.)

			// parsing input with a syntax error
			size = levelL["level"]["size"].get<int>();
			name = levelL["level"]["name"].get<std::string>();;
			sizemax = levelL["level"]["tickMax"].get<int>();;

			difficulty = levelL["level"]["difficulty"].get<int>();
			ver = levelL["level"]["version"].get<int>();
			music = levelL["level"]["song"]["songName"].get<std::string>();
			musicCName = levelL["level"]["song"]["songCreator"].get<std::string>();
			musPath = levelL["level"]["song"]["path"].get<std::string>();
			//sizemax = doc.child("level").child("song").attribute("levtime").as_int();
		}
		catch (nlohmann::json::type_error & e)
		{
			printf(std::string("load result for" + filePath + " json, " + std::string(levelL.dump())).c_str());
			// output exception information
			std::cout << "message: " << e.what() << '\n'
				<< "exception id: " << e.id << '\n'
				<< "byte position of error: " << e.id << std::endl;
		}

		//song->openFromFile(musPath);
		available = true;

		song->openFromFile(musPath); song->stop();
		Map.mapSize = size;

		//	doc.insert_attribute_after("size", pugi::xml_attribute::);*/
	}
	Level() {
	}

	Level(std::string path) {
		loadFromFile(path);
	}

	void draw() {
		Map.draw();

		SL::DrawRectangle({ 0,0 }, 1200, 1200, { 10,0,0, sf::Uint8((Map.plyr.time * 2)) });
	}
	bool Update(int tick) {
		if (edit) {
			Map.reset();
			size = levelL["level"]["size"].get<int>();

			for (size_t i = 0; i < tick; i++)
			{
				if (i < 0) {
					i = 0;
				}
				Map.smartUpdate();
				std::string t = std::string("tick") + SL::cTStr(i);

				try
				{
					int nS = levelL["level"]["data"][t][("levdata")][("size")].get<int>();
					if (nS != 0)
					{
						size = nS;
					}
					Map.mapSize = size;
				}
				catch (nlohmann::json::type_error & e) {}
				try
				{
					Proj j;
					size_t idx = 0;
					while (!levelL["level"]["data"][t][("projList")][idx].is_null())
					{
						if (levelL["level"]["data"][t][("projList")][idx]["exist"] != false)
						{
							levelL["level"]["data"][t][("projList")][idx];
							j.projpos = { levelL["level"]["data"][t][("projList")][idx][("posx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("posy")].get<float>() };
							j.projvel = { levelL["level"]["data"][t][("projList")][idx][("velx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("vely")].get<float>() };
							j.size = levelL["level"]["data"][t][("projList")][idx][("size")].get<int>();
							j.tickcreated = i + 1;
							j.part = &levelL["level"]["data"][t][("projList")][idx];
							try
							{
								j.projCol =
								{
									(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["r"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["g"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["b"].get<unsigned int>()
								};
							}
							catch (nlohmann::json::type_error & e) {
								j.projCol = sf::Color::White;
								levelL["level"]["data"][t][("projList")][idx][("color")]["r"] = 255;
								levelL["level"]["data"][t][("projList")][idx][("color")]["g"] = 255;
								levelL["level"]["data"][t][("projList")][idx][("color")]["b"] = 255;
							}try
							{
								j.id = levelL["level"]["data"][t][("projList")][idx][("id")].get<int>();
							}
							catch (nlohmann::json::type_error & e) {
								levelL["level"]["data"][t][("projList")][idx][("id")] = 0;

								j.id = levelL["level"]["data"][t][("projList")][idx][("id")].get<int>();
							}try
							{
								j.acceleration = { levelL["level"]["data"][t][("projList")][idx][("accx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("accy")].get<float>() };
								;
							}
							catch (nlohmann::json::type_error & e) {
								levelL["level"]["data"][t][("projList")][idx][("accx")] = 1.f;
								levelL["level"]["data"][t][("projList")][idx][("accy")] = 1.f;
								j.acceleration = { levelL["level"]["data"][t][("projList")][idx][("accx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("accy")].get<float>() };
								;
							}
							Map.spawnProj(j);
						}
						idx++;
					}

					idx = 0;
					while (!levelL["level"]["data"][t][("event")][idx].is_null())
					{
						if (levelL["level"]["data"][t][("event")][idx]["exist"] != false)
						{
							Event ev = Event();

							ev.idtriggered = levelL["level"]["data"][t][("event")][idx]["id"].get<int>();;
							ev.changeSize = levelL["level"]["data"][t][("event")][idx]["changeSize"].get<bool>();
							ev.changevel = levelL["level"]["data"][t][("event")][idx]["changeVel"].get<bool>();
							ev.changePos = levelL["level"]["data"][t][("event")][idx]["changePos"].get<bool>();
							ev.changeColor = levelL["level"]["data"][t][("event")][idx]["changeColor"].get<bool>();
							ev.relative = levelL["level"]["data"][t][("event")][idx]["relative"].get<bool>();
							ev.changeAcceleration = levelL["level"]["data"][t][("event")][idx]["changeAcceleration"].get<bool>();
							if (ev.changeSize == true)
							{
								ev.dataChanged.size = levelL["level"]["data"][t][("event")][idx]["size"].get<int>();
							}if (ev.changevel == true)
							{
								ev.dataChanged.projvel = {
									levelL["level"]["data"][t][("event")][idx]["velx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["vely"].get<float>() };
							}if (ev.changePos == true)
							{
								ev.dataChanged.projpos = {
									levelL["level"]["data"][t][("event")][idx]["posx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["posy"].get<float>() };
							}if (ev.changeColor) {
								ev.dataChanged.projCol = {
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["r"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["g"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["b"].get<unsigned int>()
								};
							}if (ev.changeAcceleration) {
								ev.dataChanged.acceleration = {
									levelL["level"]["data"][t][("event")][idx]["accx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["accy"].get<float>() };
							}

							for (int i = 0; i < Map.numberProj; i++)
							{
								if (Map.ProjectileMap[i].id == ev.idtriggered) {
									ev.Apply(&Map.ProjectileMap[i]);
								};
							}
						}
						idx++;
					}
				}
				catch (nlohmann::json::type_error & e) {}
			}

			return false;
		}
		else
		{
			std::string t = std::string("tick") + SL::cTStr(tick);
			Map.Update();

			try
			{
				int nS = levelL["level"]["data"][t][("levdata")][("size")].get<int>();
				if (nS != 0)
				{
					size = nS;
				}
				Map.mapSize = size;
			}
			catch (nlohmann::json::type_error & e) {}
			try
			{
				Proj j;
				size_t idx = 0;
				while (!levelL["level"]["data"][t][("projList")][idx].is_null())
				{
					j.projpos = { levelL["level"]["data"][t][("projList")][idx][("posx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("posy")].get<float>() };
					j.projvel = { levelL["level"]["data"][t][("projList")][idx][("velx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("vely")].get<float>() };
					j.size = levelL["level"]["data"][t][("projList")][idx][("size")].get<int>();
					j.tickcreated = tick;
					j.part = &levelL["level"]["data"][t][("projList")][idx];
					try
					{
						j.projCol =
						{
							(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["r"].get<unsigned int>(),
							(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["g"].get<unsigned int>(),
							(sf::Uint8)levelL["level"]["data"][t][("projList")][idx][("color")]["b"].get<unsigned int>()
						};
					}
					catch (nlohmann::json::type_error & e) {
						j.projCol = sf::Color::White;
						levelL["level"]["data"][t][("projList")][idx][("color")]["r"] = 255;
						levelL["level"]["data"][t][("projList")][idx][("color")]["g"] = 255;
						levelL["level"]["data"][t][("projList")][idx][("color")]["b"] = 255;
					}
					try
					{
						j.id = levelL["level"]["data"][t][("projList")][idx][("id")].get<int>();
					}
					catch (nlohmann::json::type_error & e) {
						levelL["level"]["data"][t][("projList")][idx][("id")] = 0;

						j.id = levelL["level"]["data"][t][("projList")][idx][("id")].get<int>();
					}
					try
					{
						j.acceleration = { levelL["level"]["data"][t][("projList")][idx][("accx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("accy")].get<float>() };
						;
					}
					catch (nlohmann::json::type_error & e) {
						levelL["level"]["data"][t][("projList")][idx][("accx")] = (float)1;
						levelL["level"]["data"][t][("projList")][idx][("accy")] = (float)1;
						j.acceleration = { levelL["level"]["data"][t][("projList")][idx][("accx")].get<float>(),levelL["level"]["data"][t][("projList")][idx][("accy")].get<float>() };
						;
					}
					Map.spawnProj(j);
					idx++;
				}
				idx = 0;
				while (!levelL["level"]["data"][t][("event")][idx].is_null())
				{
					if (levelL["level"]["data"][t][("event")][idx]["exist"] != false)
					{
						Event ev = Event();

						try
						{
							ev.idtriggered = levelL["level"]["data"][t][("event")][idx]["id"].get<int>();;
							ev.changeSize = levelL["level"]["data"][t][("event")][idx]["changeSize"].get<bool>();
							ev.changevel = levelL["level"]["data"][t][("event")][idx]["changeVel"].get<bool>();
							ev.changePos = levelL["level"]["data"][t][("event")][idx]["changePos"].get<bool>();
							ev.changeColor = levelL["level"]["data"][t][("event")][idx]["changeColor"].get<bool>();
							ev.relative = levelL["level"]["data"][t][("event")][idx]["relative"].get<bool>();
							ev.changeAcceleration = levelL["level"]["data"][t][("event")][idx]["changeAcceleration"].get<bool>();
							if (ev.changeSize == true)
							{
								ev.dataChanged.size = levelL["level"]["data"][t][("event")][idx]["size"].get<int>();
							}if (ev.changevel == true)
							{
								ev.dataChanged.projvel = {
									levelL["level"]["data"][t][("event")][idx]["velx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["vely"].get<float>() };
							}if (ev.changePos == true)
							{
								ev.dataChanged.projpos = {
									levelL["level"]["data"][t][("event")][idx]["posx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["posy"].get<float>() };
							}if (ev.changeColor) {
								ev.dataChanged.projCol = {
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["r"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["g"].get<unsigned int>(),
									(sf::Uint8)levelL["level"]["data"][t][("event")][idx]["color"]["b"].get<unsigned int>()
								};
							}
							if (ev.changeAcceleration) {
								ev.dataChanged.acceleration = {
									levelL["level"]["data"][t][("event")][idx]["accx"].get<float>(),
									levelL["level"]["data"][t][("event")][idx]["accy"].get<float>()
								};
							}
						}
						catch (nlohmann::json::type_error & e) {
						}

						for (int i = 0; i < Map.numberProj; i++)
						{
							if (Map.ProjectileMap[i].available)
							{
								if (Map.ProjectileMap[i].id == ev.idtriggered) {
									ev.Apply(&Map.ProjectileMap[i]);
								};
							}
						}
					}
					idx++;
				}
			}
			catch (nlohmann::json::type_error & e) {}

			sf::Time tlo;
			song->setPlayingOffset(sf::seconds((tick / 60.f)));
			if (tick > sizemax)
			{
				Map.reset();
				loadFromFile(pathLevel);
				song->play();
				return true;
			}
			if (Map.plyr.life <= 0)
			{
				Map.reset();
				loadFromFile(pathLevel);
				song->play();
				return true;
			}
			song->setVolume(100 - (Map.plyr.time / 2));

			SL::DrawRectangle({ 0,0 }, 1200, 1200, { 10,0,0, sf::Uint8((Map.plyr.time * 2)) });
			return false;
		}
		*updated = true;
	}

private:
};

Level getLevelLocalHistData(int id, bool load) {
	static Level lToReturn = Level(std::string("ressource/level1.json"));
	lToReturn.name = "NULL";
	lToReturn.difficulty = NULLDIF;
	lToReturn.OnlineLev = false;

	static Level lev1 = Level(std::string("ressource/level1.json"));
	switch (id)
	{
	case 1:
		/*	lToReturn.name = "Bally";
			lToReturn.difficulty = SUPEREASY;
			lToReturn.available = true;
			lToReturn.music = "in dev";
			lToReturn.musicCName = "in dev creator";
			if (load == true)
			{
				lToReturn.loadFromFile(std::string("ressource/level1.xml").c_str());
			}*/
		return lev1;

		break;
	case 2:
		lToReturn.name = "in development";
		lToReturn.difficulty = SUPEREASY;
		lToReturn.available = true;
		lToReturn.music = "in dev";
		lToReturn.musicCName = "in dev creator";

		break;
	default:
		break;
	}
	return lToReturn;
}

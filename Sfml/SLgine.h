#define MAXPARTICLE 4096

namespace SL
{
	void initSLgine(sf::RenderWindow* win);
	int& GetQuality();
	bool IsKeyPressed(sf::Keyboard::Key key);
	bool IsKeyPressedOnce(sf::Keyboard::Key key);

	bool IsMousePressed(sf::Mouse::Button b);
	bool IsMouseRelease(sf::Mouse::Button b);
	float mouseScroll();
	sf::Vector2i getMousePosition();
	void UpdateSLgine();
	bool windowShouldClose();

	float getFps();

	sf::Time gettime();

	std::string cTStr(float t); /// convert a float to a string
	std::string cTStr(int t);/// convert a int to a string
	std::string cTStr(size_t t); 
	/// <summary>
	/// convert a int to a string
	/// </summary>
	float GetRandomVal(int min, int max);

	// -------------------------------------------------- HITBOX
	bool CheckCollisionPointRec(sf::Vector2i pos, sf::Vector2f posob, sf::Vector2f sizeob);

	bool CheckCollisionCircles(sf::Vector2f center1, float radius1, sf::Vector2f center2, float radius2);
	bool CheckCollisionCircles(sf::Vector2i center1, float radius1, sf::Vector2i center2, float radius2);// get collision circles Vec int / Vec int

	bool CheckCollisionCircleRec(sf::Vector2f circlePos, float radius, sf::Vector2f recpos, sf::Vector2f recSize);

	// ---------------------------------------------------- SlgineDraw.cpp -----------------------------------------------
	void initSLgineDraw(sf::RenderWindow* win);/// initialisation

	void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col); /// draw a colored line

	void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col1, sf::Color col2);/// draw a colored gradient line

	void DrawPix(sf::Vector2f pos, sf::Color col1);/// draw a pixel

	void DrawCircle(sf::Vector2f pos, float size, sf::Color crColor);/// draw a circle

	void DrawCircleQ(sf::Vector2f pos, float size, sf::Color crColor, int quality);///  draw circle with custom quality (quality = number of point in the shape)

	void DrawCircleLine(sf::Vector2f pos, float size, sf::Color crColor, int thickness); // draw circle outline

	void DrawCircleLineQ(sf::Vector2f pos, float size, sf::Color crColor, int thickness, int quality); // draw circle outline with custom quality (quality = number of point in the shape)
	
	void DrawCircleG(sf::Vector2f pos, float size, float expend, sf::Color crColor1, sf::Color crColor2);

	void DrawRectangle(sf::Vector2f pos, float sizex, float sizey, sf::Color crColor);/// draw a rectangle

	void DrawRectangleGradient(sf::Vector2f pos, float sizex, float sizey, sf::Color bottomColor, sf::Color upColor);/// draw a rectangle gradient

	void DrawProgressBar(sf::Vector2f pos, float sizex, float sizey, sf::Color crColordisabled, sf::Color crColorprogress, float percent /* 0.1 = 10%*/);/// draw a progress bar

	void DrawTextN(sf::Vector2f pos, float size, sf::Color textCol, std::string text);/// draw text from loaded font
	void DrawTextNI(sf::Vector2i pos, float size, sf::Color textCol, std::string text);

	void DrawTextNC(sf::Vector2f pos, float recSize, float size, sf::Color textCol, std::string text);
	int getTextSize(std::string text, float sizeY);
	/// draw text from loaded font

	///                                            texture drawing
#pragma region texture

	sf::Texture LoadTexture(std::string filePath); /// load a texture (note if error occur it will load a null texture)

	void DrawTexture(sf::Texture textureTodraw, sf::Vector2f pos); /// draw a simple texture

	void DrawTextureA(sf::Texture textureTodraw, sf::Vector2f pos
		, float rotation = 0.f, sf::Vector2f centerP = { 0,0 }
	, float size = 1.f, sf::Color tint = sf::Color::White); /// draw a advance texture

	void DrawTextureC(sf::Texture textureTodraw, sf::Vector2f pos, sf::Color tint = sf::Color::White);/// draw a texture with tint

	void DrawTexture(sf::Texture textureTodraw, sf::Vector2f pos);

	sf::Window& getWindow();

	void DrawSprite(sf::Sprite sptodraw);
#pragma endregion
	///                                             other
#pragma region other
	int getDrawCall();/// get the number of thing draw since the last displayWindow() call

	void ClearWindow(sf::Color bgColor);/// fill the background

	void DisplayWindow();/// display everything draw from the last display

	void renderFromzone(sf::Vector2f pos, float size);

	// -------------------------------------------------------------------------- particle ----------------------------------------------------------------------
	enum ParticleType
	{
		PARTICLE_CIRCLE = 0,
		PARTICLE_RECTANGLE
	};
	class Particle {
	public:
		int number = 0;
		int desizingFactor = 0;
		float lifeTime = 0;
		float life = 0;
		float size = 0;
		float baseSize = 0;
		bool sizeChange = true;
		bool dead = true;
		sf::Color colorFade = sf::Color::Black;
		sf::Color pCol = sf::Color::Red;
		sf::Vector2f pos = { 0,0 };

		sf::Vector2f direction = { 0,0 };

		ParticleType ptype = PARTICLE_CIRCLE;
		void Update();

		void Draw();
	};

	class ParticleMap {
	public:

		int MaxParticle = 1000;
		Particle* p;
		int numberParticle = 0; /// 1 for just security
		void draw();
		ParticleMap();
		void Update();
		void AddParticle(Particle particl);
		int getNumberParticle();
	};
	class ParticleEmmiter {
	public:
		Particle particleToEmit;

		int numParticleAdded = 1;
		sf::Vector2f pos;
		ParticleMap* map;

		void Update();
		ParticleEmmiter(ParticleMap* mapt);
	};

	// init the window
	void initSLgineParticle(sf::RenderWindow* win);

	// GUI
	bool guiButton(sf::Vector2f pos, sf::Vector2f size, std::string text);

	std::string popup(std::string popuptext, std::string secondText, sf::RenderWindow* wnd);

	int popupdiff(std::string popuptext, int prevDiff, sf::RenderWindow* wnd);
}

#ifndef SLGINEDRAW_H
#define SLGINEDRAW_H

#include "SLgineImport.h"
namespace SL
{
	int drawCall = 0;
	sf::Font globalFont;
	sf::Texture errorTexture;
	sf::RenderWindow* windowGlobalDraw;
	// init the window

	void Check() {
		if (drawCall + 200 > 1024)
		{
		}// on laisse de l'espace au cas ou
	}



	const char RadialGradient[] =
		"uniform vec4 color;"
		"uniform vec2 center;"
		"uniform float radius;"
		"uniform float expand;"
		"uniform float windowHeight;"
		"void main(void)"
		"{"
		"vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);"
		"vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;"
		"float r = sqrt(dot(p, p));"
		"if (r < 1.0)"
		"{"
		"gl_FragColor = mix(color, gl_Color, (r - expand) / (1 - expand));"
		"}"
		"else"
		"{"
		"gl_FragColor = gl_Color;"
		"}"
		"}";
	sf::Shader gradient;
	void initSLgineDraw(sf::RenderWindow* win) {
		printf("loading Draw \n");
		if (!globalFont.loadFromFile("ressource/engine/Segoe UI.ttf"))
		{
			printf("! error ! : loading global font error\n");
		}
		else
		{
		}
		if (!errorTexture.loadFromFile("ressource/engine/errortexture.png"))
		{
			printf("! error ! : can't load null texture \n");
		}if (!gradient.loadFromMemory(RadialGradient, sf::Shader::Type::Fragment))
		{
			printf("! error ! : can't load gradient shader\n");
		}
		windowGlobalDraw = win;
	}/// initialisation

	void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col) {
		drawCall++;
		Check();
		sf::Vertex line[] =
		{
			sf::Vertex(p1,col),
			sf::Vertex(p2,col),
		};

		drawCall += 2;
		windowGlobalDraw->draw(line, 2, sf::Lines);
	} /// draw a colored line

	void DrawLine(sf::Vector2f p1, sf::Vector2f p2, sf::Color col1, sf::Color col2) {
		drawCall++;
		Check();
		sf::Vertex line[] =
		{
			sf::Vertex(p1,col1),
			sf::Vertex(p2,col2),
		};
		drawCall += 2;
		// crée une forme vide
		windowGlobalDraw->draw(line, 2, sf::Lines);
	}/// draw a colored gradient line

	void DrawPix(sf::Vector2f pos, sf::Color col1) {
		drawCall++;
		Check();
		sf::Vertex point[] =
		{
			sf::Vertex(pos,col1),
		};

		drawCall += 1;
		// crée une forme vide
		windowGlobalDraw->draw(point, 1, sf::Points);
	}/// draw a pixel

	int nC;

	void DrawCircle(sf::Vector2f pos, float size, sf::Color crColor) {
		sf::CircleShape Circshape(size);
		Circshape.setFillColor(crColor);
		Circshape.move(pos);
		if (GetQuality() == 3)
		{
			Circshape.setPointCount(20);
		}
		else if ((GetQuality() == 2))
		{
			Circshape.setPointCount(13);
		}
		else if ((GetQuality() == 1))
		{
			Circshape.setPointCount(5);
		}

		Circshape.setOrigin(size, size);
		// drawCall += Circshape.getPointCount();

		Check();
		windowGlobalDraw->draw(Circshape);
	}

	void DrawCircleQ(sf::Vector2f pos, float size, sf::Color crColor, int quality)
	{
		sf::CircleShape Circshape(size);
		Circshape.setFillColor(crColor);
		Circshape.move(pos);

		Circshape.setPointCount(quality);

		Circshape.setOrigin(size, size);

		// drawCall += Circshape.getPointCount();
		Check();
		windowGlobalDraw->draw(Circshape);
	}

	void DrawCircleLine(sf::Vector2f pos, float size, sf::Color crColor, int thickness)
	{
		sf::CircleShape Circshape(size);
		Circshape.setOutlineColor(crColor);
		Circshape.setOutlineThickness(thickness);
		Circshape.setFillColor({ 0,0,0,0 });
		Circshape.move(pos);
		if (GetQuality() == 3)
		{
			Circshape.setPointCount(20);
		}
		else if ((GetQuality() == 2))
		{
			Circshape.setPointCount(13);
		}
		else if ((GetQuality() == 1))
		{
			Circshape.setPointCount(5);
		}

		Circshape.setOrigin(size, size);
		// drawCall += Circshape.getPointCount();
		Check();
		windowGlobalDraw->draw(Circshape);
	}
	void DrawCircleLineQ(sf::Vector2f pos, float size, sf::Color crColor, int thickness, int quality)
	{
		sf::CircleShape Circshape(size);
		Circshape.setOutlineColor(crColor);
		Circshape.setOutlineThickness(thickness);
		Circshape.setFillColor({ 0,0,0,0 });
		Circshape.move(pos);

		Circshape.setPointCount(quality);

		Circshape.setOrigin(size, size);
		// drawCall += Circshape.getPointCount();
		Check();
		windowGlobalDraw->draw(Circshape);
	}
	inline float deg2rad(float x) {
		return x * (180 / 3.14);
	}
	void DrawCircleG(sf::Vector2f pos, float size,float expend, sf::Color crColor1,sf::Color crColor2)
	{
		
		
			/*for (int i = 0; i < 360; i += 10)
			{
				rlColor4ub(color1.r, color1.g, color1.b, color1.a);
				rlVertex2f(centerX, centerY);
				rlColor4ub(color2.r, color2.g, color2.b, color2.a);
				rlVertex2f(centerX + sinf(DEG2RAD * i) * radius, centerY + cosf(DEG2RAD * i) * radius);
				rlColor4ub(color2.r, color2.g, color2.b, color2.a);
				rlVertex2f(centerX + sinf(DEG2RAD * (i + 10)) * radius, centerY + cosf(DEG2RAD * (i + 10)) * radius);
			}*/

		gradient.setUniform("windowHeight", static_cast<float>(windowGlobalDraw->getSize().y)); // t
		gradient.setUniform("color", sf::Glsl::Vec4( crColor1));
		gradient.setUniform("center", pos);
		gradient.setUniform("radius", size);
		gradient.setUniform("expand", expend);

		sf::Shader::bind(&gradient);
		SL::DrawCircle(pos, size, crColor2);
		sf::Shader::bind(NULL);


	}
	/// draw a circle

	sf::RectangleShape shapeRec;
	void DrawRectangle(sf::Vector2f pos, float sizex, float sizey, sf::Color crColor) {
		shapeRec = sf::RectangleShape({ sizex, sizey });

		shapeRec.setFillColor(crColor);
		shapeRec.move(pos);
		drawCall += shapeRec.getPointCount();
		windowGlobalDraw->draw(shapeRec);
	}/// draw a rectangle

	void DrawRectangleGradient(sf::Vector2f pos, float sizex, float sizey, sf::Color bottomColor, sf::Color upColor) {
		drawCall += 6;
		Check();

		sf::Vertex rec[] =
		{
			sf::Vertex(pos,upColor),
			sf::Vertex({pos.x + sizex, pos.y},upColor),
			sf::Vertex({pos.x + sizex, pos.y + sizey},bottomColor),
			sf::Vertex({pos.x , pos.y + sizey},bottomColor),
		};
		windowGlobalDraw->draw(rec, 4, sf::Quads);

		//delete( rec);
	}/// draw a rectangle gradient

	void DrawProgressBar(sf::Vector2f pos, float sizex, float sizey, sf::Color crColordisabled, sf::Color crColorprogress, float percent /* 0.1 = 10%*/) {
		drawCall += 12;
		Check();
		DrawRectangle({ pos.x - 5,pos.y - 5 }, sizex + 10, sizey + 10, crColordisabled);
		DrawRectangle(pos, sizex * percent, sizey, crColorprogress);
	}/// draw a progress bar

	void DrawTextN(sf::Vector2f pos, float size, sf::Color textCol, std::string text) {
		drawCall += 6; // nani
		Check();
		sf::Text txt(text, globalFont, 128);
		txt.scale({ size / 128,size / 128 });
		//  txt.setFont(globalFont);
		//  txt.setString(text);
		//  txt.setCharacterSize(size);
		txt.setFillColor(textCol);
		txt.setPosition(pos);
		windowGlobalDraw->draw(txt);
	}/// draw text from loaded font
	void DrawTextNI(sf::Vector2i pos, float size, sf::Color textCol, std::string text) {
		drawCall += 6; // nani
		Check();
		sf::Text txt(text, globalFont, 128);
		txt.scale({ size / 128,size / 128 });
		//  txt.setFont(globalFont);
		//  txt.setString(text);
		//  txt.setCharacterSize(size);
		sf::Vector2f f = { (float)pos.x,(float)pos.y };
		txt.setFillColor(textCol);
		txt.setPosition(f);

		windowGlobalDraw->draw(txt);
	}/// draw text from loaded font

	void DrawTextNC(sf::Vector2f pos, float recSize, float size, sf::Color textCol, std::string text) {
		drawCall += 6; // nani
		Check();
		sf::Text txt(text, globalFont, 128);
		txt.scale({ size / 128,size / 128 });
		//  txt.setFont(globalFont);
		//  txt.setString(text);
		//  txt.setCharacterSize(size);
		sf::Vector2f f = { (float)pos.x,(float)pos.y };
		txt.setFillColor(textCol);
		txt.setPosition(
			pos.x + (recSize - txt.getGlobalBounds().width / 2), pos.y);
		windowGlobalDraw->draw(txt);
	}

	int getTextSize(std::string text, float sizeY) {
		sf::Text txt(text, globalFont, 128);
		txt.scale({ sizeY / 128,sizeY / 128 });
		return txt.getGlobalBounds().width;
	}
	///                                            texture drawing
#pragma region texture

	sf::Texture LoadTexture(std::string filePath) {
		sf::Texture texture;
		if (!texture.loadFromFile(filePath))
		{
			texture = errorTexture;

			printf(std::string("\n! error ! : can't load" + filePath + " texture \n").c_str());
		}
		texture.setSmooth(true);

		return texture;
	} /// load a texture (note if error occur it will load a null texture)

	void DrawTexture(sf::Texture textureTodraw, sf::Vector2f pos) {
		Check();
		drawCall += 6;
		sf::Sprite sprite = sf::Sprite();
		textureTodraw.setSmooth(true);
		sprite.setTexture(textureTodraw);
		sprite.setPosition(pos);

		windowGlobalDraw->draw(sprite);
	} /// draw a simple texture

	sf::Window& getWindow()
	{
		return *windowGlobalDraw;
	}

	void DrawSprite(sf::Sprite sptodraw)
	{
		windowGlobalDraw->draw(sptodraw);
	}

	sf::Sprite sprite;
	void DrawTextureA(sf::Texture textureTodraw, sf::Vector2f pos
		, float rotation, sf::Vector2f centerP
		, float size, sf::Color tint) {
		drawCall += 6;
		Check();
		sprite = sf::Sprite();
		sprite.setTexture(textureTodraw);
		sprite.setPosition(pos);
		sprite.setColor(tint);
		sprite.rotate(rotation);
		sprite.scale({ size,size });
		sprite.setOrigin(centerP);
		windowGlobalDraw->draw(sprite);
	} /// draw a advance texture

	void DrawTextureC(sf::Texture textureTodraw, sf::Vector2f pos, sf::Color tint) {
		drawCall += 6;
		Check();
		textureTodraw.setSmooth(true);
		sprite = sf::Sprite();
		sprite.setTexture(textureTodraw);
		sprite.setPosition(pos);
		sprite.setColor(tint);
		windowGlobalDraw->draw(sprite);
	} /// draw a texture with tint

#pragma endregion
	///                                             other
#pragma region other
	int getDrawCall() {
		return drawCall;
	}/// get the number of thing draw since the last displayWindow() call

	void ClearWindow(sf::Color bgColor) {
		nC = 0;
		drawCall++;

		windowGlobalDraw->clear(bgColor); // fill background with color
	}/// fill the background

	void DisplayWindow() {
		windowGlobalDraw->display();

		drawCall = 0;
	}/// display everything draw from the last display
	sf::Sprite sprt;
	void renderFromzone(sf::Vector2f pos, float size) {
		sf::Texture texture;
		texture.create(float(windowGlobalDraw->getSize().x), float(windowGlobalDraw->getSize().y));
		texture.update(*windowGlobalDraw);
		texture.setSmooth(true);
		SL::ClearWindow(sf::Color::Black);
		DrawTextureA(texture, pos, 0.f, { 0.f,0.f }, (1200 * size / windowGlobalDraw->getSize().x));

		drawCall = 0;
	}/// display everything draw from the last display

#pragma endregion
}
#endif
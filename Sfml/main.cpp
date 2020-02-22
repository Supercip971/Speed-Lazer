#define OPENGL_33
#define THREADING

bool beta = true;
bool alpha = true;
float version = 0.1;
bool debug = true;
bool rainBowPowa = false;
//#define ERROR_HANDLE
#include <SFML/OpenGL.hpp>
#include "levels.h"
#include <thread>
#include <future>
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include <filesystem>
#include <exception>
#include "discPro.h"
std::mutex mu;
bool threadend = false;
void ltoUpdatethread(Level* l, int tick) {
}

class DefaultError : public std::exception
{
public:
	~DefaultError() {
		delete[] err;
	};
	const char* what() const noexcept {
		snprintf(err, 2048, ": %s\n line %d\n file %s\n", " error ", __LINE__, __FILE__);
		printf("error %s \n line : %d \n file : %s", err, __LINE__, __FILE__);
		return err;
	}
private:
	char* err = new char[2048];
};

namespace fs = std::filesystem;
namespace js = nlohmann;

/// important ! credit
// DOS-88 - City Stomper
void ImGuiStyleCall() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 colNorm = ImVec4(0.30f, 0.30f, 0.37f, 1.f);
	ImVec4 colHover = ImVec4(0.40f, 0.40f, 0.49f, 1.f);
	ImVec4 colactive = ImVec4(0.50f, 0.50f, 0.59f, 1.f);

	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.07f, 0.80f);
	/*  style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.02f, 0.80f);

	  style.Colors[ImGuiCol_Button] = colNorm;
	  style.Colors[ImGuiCol_ButtonHovered] = colHover;
	  style.Colors[ImGuiCol_ButtonActive] = colactive;

   style.Colors[ImGuiCol_FrameBg] = colNorm;
	  style.Colors[ImGuiCol_FrameBgHovered] = colHover;
	  style.Colors[ImGuiCol_ButtonActive] = colactive;

	  style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);*/
	style.WindowRounding = 0.f;                    // Background color edit
	style.WindowPadding = { 3.f,3.f };
	style.FrameRounding = 5.f;                    // Background color edit
	style.FrameBorderSize = 1.f;
}
void updateParticleThread(SL::ParticleMap* pm)
{
	// cette fonction démarre lorsque thread.launch() est appelé

	pm->Update();
}   //This function converts HSL (Hue, saturation, lightness) to RGB.
//H goes from 0 to 360.
//S goes from 0 to 1.
//L goes from 0 to 1.
//The RGB values in the returned pointer go from 0 to 255.
//The returned pointer is 3 doubles long, *(ptr)=R, *(ptr+1)=G, *(ptr+2)=B.
#define LOADSTEP Loadpercent += thingToAdd; SL::DrawProgressBar({ 1200 / 2 - 1000 / 2, 600 / 2 - 50 / 2 }, 1000, 50, { 200,200,200,255 }, sf::Color::Black, Loadpercent); SL::DisplayWindow();
sf::Color HSLtoRGB(double H, double S, double L) {
	double* HSLptr;
	HSLptr = new double[3];

	double C = 2 * L * S;
	double Hprime = H / 60.0;
	double a = Hprime - 2 * floor(Hprime / 2.0);
	double X = C * (1 - fabs(a - 1));
	double C_0 = C * 255;
	double X_0 = X * 255;

	if (Hprime >= 0 && Hprime < 1) {
		*(HSLptr + 0) = C_0;
		*(HSLptr + 1) = X_0;
		*(HSLptr + 2) = 0;
	}
	if (Hprime >= 1 && Hprime < 2) {
		*(HSLptr + 0) = X_0;
		*(HSLptr + 1) = C_0;
		*(HSLptr + 2) = 0;
	}
	if (Hprime >= 2 && Hprime < 3) {
		*(HSLptr + 0) = 0;
		*(HSLptr + 1) = C_0;
		*(HSLptr + 2) = X_0;
	}
	if (Hprime >= 3 && Hprime < 4) {
		*(HSLptr + 0) = 0;
		*(HSLptr + 1) = X_0;
		*(HSLptr + 2) = C_0;
	}
	if (Hprime >= 4 && Hprime < 5) {
		*(HSLptr + 0) = X_0;
		*(HSLptr + 1) = 0;
		*(HSLptr + 2) = C_0;
	}
	if (Hprime >= 5 && Hprime < 6) {
		*(HSLptr + 0) = C_0;
		*(HSLptr + 1) = 0;
		*(HSLptr + 2) = X_0;
	}
	sf::Color col = sf::Color::Black;
	col.r = *(HSLptr + 0);
	col.g = *(HSLptr + 1);
	col.b = *(HSLptr + 2);
	return col;
}	//HSLtoRGB
enum MenuModeE
{
	START = 0,
	SELECTION = 1,
	EDITION = 2,
	PLAYING = 3,
	LOADING = 4,
	CREATORSELECT = 5
};
int main()
{
#ifdef ERROR_HANDLE

	try
	{
#endif // ERROR_HANDLE

		float thingToAdd = (100.f / 29) / 100.f;
		float Loadpercent = 0.f;
		int& Globalquality = SL::GetQuality(); //  3 = ultra quality | 2= good quality | 1= performance
		using namespace std::chrono_literals;
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		int scrW = 1200;
		int scrH = 675;
		sf::VideoMode video(scrW, scrH);

		sf::RenderWindow window(video, "loading...", sf::Style::Default, settings);

		SL::initSLgine(&window);
		LOADSTEP
			SL::UpdateSLgine();
		SL::ClearWindow(sf::Color(255, 255, 255, 255));
		LOADSTEP

			ImGui::SFML::Init(window);
		LOADSTEP

			sf::Music menuMusic;
		sf::Music* playMusic = new sf::Music();
		// window.setFramerateLimit(1000000); window.setVerticalSyncEnabled(false);
		LOADSTEP
			SL::DisplayWindow();

		sf::Vector2f playerVel = { 0,0 };

		if (!menuMusic.openFromFile("ressource/song/City-Stomper.wav"))
			printf("ERROR can't load : City Stomper.wav !");
		LOADSTEP
			sf::Texture logoSpeedLazer = SL::LoadTexture("ressource/logow.png");
		sf::Color bgColor;
		float color[3] = { 0.f, 0.f, 0.f };
		int levelTick = 0;
		LOADSTEP
			// to do settings

			LOADSTEP
			// let's use char array as buffer, see next part
			// for instructions on using std::string with ImGui
			//window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
			SL::ParticleMap* pM = new SL::ParticleMap();
		SL::Particle particleBackground;
		int maxPartSetting = pM->MaxParticle;
		particleBackground.baseSize = 30;
		particleBackground.lifeTime = 310;
		particleBackground.pCol = { 10,10,25 ,255 };
		particleBackground.size = 20;
		particleBackground.direction = { 0.f,-3.f };
		particleBackground.ptype = SL::PARTICLE_CIRCLE;
		SL::ParticleEmmiter pE = SL::ParticleEmmiter(pM);
		pE.particleToEmit = particleBackground;
		pE.pos = { -1.f, 700 };
		pE.map = pM;
		LOADSTEP
			int planetSelected = 0;
		int volume = 75;
		int cursorLevel = 0;
		sf::Color rainBowPowacol = sf::Color::Color(0);
		double rainboxpowadoub = 0;
		float menuStart[] = { 0,0,0,0 };
		bool settingopen = false;
		LOADSTEP
			pE.numParticleAdded = 3;
		// Use an atomic flag.
		int tickupdateParticle = 0;
		LOADSTEP
			std::future<void> updateParticleThreadth = std::async(updateParticleThread, pM);
		LOADSTEP
			MenuModeE menuMode = START; // main menu
		menuMusic.setLoop(true);
		menuMusic.play();
		Player mainPlayer = Player();
		sf::Music song;
		Level lToPlay = Level("ressource/level1.json");
		Level lToEdit = Level("ressource/level1.json");
		Level l = Level("ressource/level1.json");
		lToEdit.edit = true;

		LOADSTEP

			// ---------------------------------------------   load user level
			printf("loading user level...\n");
		Level* userLevel = new Level[512];
		const int maxLev = 512;
		int numLev = 0;
		float cursor = 0;
		const fs::path pathToUserLevel{ fs::current_path().string() + "/ressource/userLevel" };
		bool openLevelData = false;
		bool openLevelEditor = false;
		int levelSelect = 0;
		for (const auto& entry : fs::directory_iterator(pathToUserLevel)) {
			const auto filenameStr = entry.path().filename().string();
			if (entry.path().filename().extension().string() == ".json") {
				std::cout << "file: " << filenameStr << '\n';

				if (numLev < maxLev)
				{
					userLevel[numLev] = Level(entry.path().string());
				} numLev++;
			}
		}
		LOADSTEP
			// -------------------- load util texture

			float buttonEditorPadding = 20.f;
		float buttonSize = 32.f;

		LOADSTEP

			sf::Texture editorIcon = SL::LoadTexture("ressource/UIicon/editor.png");
		sf::Sprite editorSprite = sf::Sprite(editorIcon);

		editorSprite.scale({ buttonSize / 1024,buttonSize / 1024 });
		LOADSTEP
			sf::Texture exitIcon = SL::LoadTexture("ressource/UIicon/exit.png");
		sf::Sprite exitSprite = sf::Sprite(exitIcon);

		exitSprite.scale({ buttonSize / 1024,buttonSize / 1024 });
		LOADSTEP

			sf::Texture PlayIcon = SL::LoadTexture("ressource/UIicon/play.png");
		sf::Sprite PlaySprite = sf::Sprite(PlayIcon);
		PlaySprite.scale({ buttonSize / 1024,buttonSize / 1024 });
		LOADSTEP

			sf::Texture SaveIcon = SL::LoadTexture("ressource/UIicon/save.png");
		sf::Sprite SaveSprite = sf::Sprite(SaveIcon);

		SaveSprite.scale({ buttonSize / 512,buttonSize / 512 });

		sf::Texture UploadIcon = SL::LoadTexture("ressource/UIicon/upload.png");
		sf::Sprite UploadSprite = sf::Sprite(UploadIcon);

		UploadSprite.scale({ buttonSize / 512,buttonSize / 512 });

		sf::Texture NewIcon = SL::LoadTexture("ressource/UIicon/new.png");
		sf::Sprite NewSprite = sf::Sprite(NewIcon);

		NewSprite.scale({ buttonSize / 512,buttonSize / 512 });

		sf::Texture RefreshIcon = SL::LoadTexture("ressource/UIicon/refresh.png");
		RefreshIcon.setSmooth(true);
		sf::Sprite RefreshSprite = sf::Sprite(RefreshIcon);

		RefreshSprite.scale({ buttonSize / 512,buttonSize / 512 });
		LOADSTEP
			sf::Texture DiffNullIcon = SL::LoadTexture("ressource/UIicon/difficulty/null.png");

		LOADSTEP
			sf::Texture DiffSupEasyIcon = SL::LoadTexture("ressource/UIicon/difficulty/superEasy.png");

		LOADSTEP
			sf::Texture DiffEasyIcon = SL::LoadTexture("ressource/UIicon/difficulty/Easy.png");

		LOADSTEP
			sf::Texture DiffNormalIcon = SL::LoadTexture("ressource/UIicon/difficulty/Normal.png");

		LOADSTEP
			sf::Texture DiffHardIcon = SL::LoadTexture("ressource/UIicon/difficulty/Hard.png");

		LOADSTEP
			sf::Texture DiffVerHardIcon = SL::LoadTexture("ressource/UIicon/difficulty/VeryHard.png");

		LOADSTEP
			sf::Texture DiffHarCoreIcon = SL::LoadTexture("ressource/UIicon/difficulty/HardCore.png");
		LOADSTEP
			sf::Texture DiffDeathIcon = SL::LoadTexture("ressource/UIicon/difficulty/Death.png");

		LOADSTEP
			sf::Texture SupCypLogo = SL::LoadTexture("ressource/supCypLogoSmall.png");
		sf::Sprite SupCypLogosprt = sf::Sprite(SupCypLogo);
		SupCypLogosprt.setPosition({ 1200 / 2 - 250, 675 / 2 - 250 });
		LOADSTEP
			sf::Texture SpeedLightEngineLogo = SL::LoadTexture("ressource/SpeedLight Engine.png");
		sf::Sprite SpeedLightEnginesprt = sf::Sprite(SpeedLightEngineLogo);
		SpeedLightEnginesprt.setPosition({ 1200 / 2 - 250, 675 / 2 - 250 });

		LOADSTEP
			printf("loading discord \n");

		Disc discordHandle;
		discordHandle.init();

		sf::Shader shad = sf::Shader();
		const std::string shadblurstr = ("ressource/blur.fs");
		if (!shad.loadFromFile(shadblurstr, sf::Shader::Type::Fragment)) {
		
			printf("\n error shader\n");
		}
		std::future<bool> t1;
		for (int i = 0; i < 90; i += 1)
		{
			SL::UpdateSLgine();
			SL::ClearWindow(sf::Color::White);
			window.draw(SupCypLogosprt);
			//  SL::DrawTextureC(SupCypLogo, { 1200 / 2 - 250, 675 / 2 - 250 }, { 255,255,255,  255 });
			SL::DisplayWindow();
		}for (int i = 0; i < 90; i += 1)
		{
			SL::UpdateSLgine();
			SL::ClearWindow(sf::Color::White);
			window.draw(SpeedLightEnginesprt);
			SL::DisplayWindow();
		}

		float oi = 0;
		float oil = 1.f / oi;
		/// Level* levelHist = new Level[2]{ Level("ressource/level1.xml"),Level("ressource/level1.xml") };

		bool paused = false;
#pragma region EDITORDATA
		bool exit = false;
		int tickedit = int(0);
		int tickPlay;
		Proj* projectiletoedit = new Proj();
		bool shouldUpdate = false;
		bool shouldrun = false;
		int update = 0;
		Proj j;
		Event ev;
		bool eventSelect = false;
		nlohmann::json* eventjson = nullptr;
		bool backgroundSetopen = false;
		std::future<bool> editorUpdateThread;
		Level levThreadOthrer;
		std::future_status status;

#pragma endregion
		sf::Texture texturetheadpic;



		while (window.isOpen()) {
			SL::UpdateSLgine();
			ImGuiStyleCall();
			if (menuMode == START)
			{
				menuStart[0] -= 0.05;
				if (menuStart[0] < 0)
				{
					menuStart[0] = 0;
				}

				menuStart[1] -= 0.05;
				if (menuStart[1] < 0)
				{
					menuStart[1] = 0;
				}

				menuStart[2] -= 0.05;
				if (menuStart[2] < 0)
				{
					menuStart[2] = 0;
				}
				menuStart[3] -= 0.05;
				if (menuStart[3] < 0)
				{
					menuStart[3] = 0;
				}
				tickupdateParticle++;
				sf::Event event;

				updateParticleThreadth.wait();
				pE.Update();
				updateParticleThreadth = std::async(updateParticleThread, pM);
			}

			if (SL::windowShouldClose()) {
				window.close();
			}
			ImGui::SFML::Update(window, SL::gettime());

			if (menuMode == START)
			{
				static sf::Vector2f blurdat = { 0,0 };
				discordHandle.activitystr = "main menu";
				ImGui::Begin("debug back");

				ImGui::InputFloat("clamp red", &blurdat.x);
				ImGui::InputFloat("radx", &blurdat.y);
				ImGui::End();
				SL::ClearWindow(
					{ 255,0,0,255 });
				
				
				
			

				SL::ClearWindow(
					{ 10,10,25,255 });
				SL::DrawRectangleGradient({ 0,0 }, 1200, 675, bgColor, { 0,0,0,0 });

				pM->draw();
					SL::DrawRectangle({ scrW / 2.f - 605.f / 2.f, 0.f }, 605.f, scrH, { 255,255,255,255 });
				SL::DrawRectangle({ scrW / 2.f - 600.f / 2.f, 0.f }, 600.f, scrH, sf::Color::Black);

				std::string fp = "fps : ";
				fp += SL::cTStr(SL::getFps());
				SL::DrawTextN({ 0,0 }, 20, sf::Color::White, fp);

				SL::DrawRectangle({ scrW / 2.f - 300,logoSpeedLazer.getSize().y + 0.f }, 600, 45, { 30,35,80,sf::Uint8(menuStart[0] * 255) });
				SL::DrawRectangle({ scrW / 2.f - 300,logoSpeedLazer.getSize().y + 50.f }, 600, 45, { 30,35,80,sf::Uint8(menuStart[1] * 255) });
				SL::DrawRectangle({ scrW / 2.f - 300,logoSpeedLazer.getSize().y + 100.f }, 600, 45, { 30,35,80,sf::Uint8(menuStart[2] * 255) });
				SL::DrawRectangle({ scrW / 2.f - 300,logoSpeedLazer.getSize().y + 150.f }, 600, 45, { 110,35,80,sf::Uint8(menuStart[3] * 255) });
				//  SL::guiButton({ 20,20 }, { 40,10 }, "hello world");
				if (settingopen == true)
				{
					SL::DrawTextN({ scrW / 2.f - 600.f / 2.f, 0.f }, 30, sf::Color::White, "settings :");

					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 0.f }, 40, sf::Color::White, "START");

					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 50.f }, 40, sf::Color::White, "volume : " + SL::cTStr(volume) + "%");

					if (Globalquality == 1)
					{
						SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 100.f }, 40, sf::Color::White, " quality : performance ");
					}if (Globalquality == 2)
					{
						SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 100.f }, 40, sf::Color::White, " quality : good quality");
					}if (Globalquality == 3)
					{
						SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 100.f }, 40, sf::Color::White, " quality : ultra quality");
					}
					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 150.f }, 40, sf::Color::White, "exit setting");

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 0.f }, { 600.f, 45.f })) {
						menuStart[0] += 0.15;
						if (menuStart[0] > 1)
						{
							menuStart[0] = 1;
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 50.f }, { 600.f, 45.f })) {
						menuStart[1] += 0.15;
						if (menuStart[1] > 1)
						{
							menuStart[1] = 1;
						}if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							volume += 25;
							if (volume > 100)
							{
								volume = 0;
							}menuMusic.setVolume(volume);
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 100.f }, { 600.f, 45.f })) {
						menuStart[2] += 0.15;
						if (menuStart[2] > 1)
						{
							menuStart[2] = 1;
						}
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							Globalquality++;
							if (Globalquality == 4)
							{
								Globalquality = 1;
							}

							// update of thing that change with quality --

							pE.numParticleAdded = Globalquality;
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 150.f }, { 600.f, 45.f })) {
						menuStart[3] += 0.15;
						if (menuStart[3] > 1)
						{
							menuStart[3] = 1;
						}
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							settingopen = false;
						}
					}
				}
				else
				{
					if (rainBowPowa == true)
					{
						SL::DrawTextureA(logoSpeedLazer, { scrW / 2.f - 600.f / 2.f, 0.f }, 0, { 0,0 }, 600.f / logoSpeedLazer.getSize().x, rainBowPowacol);
						rainboxpowadoub += 0.1;
						rainBowPowacol = HSLtoRGB(rainboxpowadoub, 1, 1);
						if (rainboxpowadoub > 360)
						{
							rainboxpowadoub = 0;
						}
						rainBowPowacol.a = 255;
					}
					else
					{
						SL::DrawTextureA(logoSpeedLazer, { scrW / 2.f - 600.f / 2.f, 0.f }, 0, { 0,0 }, 600.f / logoSpeedLazer.getSize().x);
					}
					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 0.f }, 40, sf::Color::White, "START");

					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 50.f }, 40, sf::Color::White, "EDITOR (in development)");
					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 100.f }, 40, sf::Color::White, "OPTIONS");
					SL::DrawTextN({ scrW / 2.f - 300 + 10,logoSpeedLazer.getSize().y + 150.f }, 40, sf::Color::White, "QUIT");

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2.f - 600.f / 2.f, 0.f }, { 600.f,(float)logoSpeedLazer.getSize().y })) {
						if (SL::IsKeyPressed(sf::Keyboard::Key::LAlt))
						{
							if (SL::IsMouseRelease(sf::Mouse::Button::Left))
							{
								rainBowPowa = true;
							}
						}
					}

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 0.f }, { 600.f, 45.f })) {
						menuStart[0] += 0.15;
						if (menuStart[0] > 1)
						{
							menuStart[0] = 1;
						}if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = MenuModeE::SELECTION;
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 50.f }, { 600.f, 45.f })) {
						menuStart[1] += 0.15;
						if (menuStart[1] > 1)
						{
							menuStart[1] = 1;
						}if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = MenuModeE::EDITION;
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 100.f }, { 600.f, 45.f })) {
						menuStart[2] += 0.15;
						if (menuStart[2] > 1)
						{
							menuStart[2] = 1;
						}if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							settingopen = true;
						}
					}
					else if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW / 2 - 300.f,logoSpeedLazer.getSize().y + 150.f }, { 600.f, 45.f })) {
						menuStart[3] += 0.15;
						if (menuStart[3] > 1)
						{
							menuStart[3] = 1;
						}
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							break;
						}
					}
				}
				SL::DrawPix((sf::Vector2f)SL::getMousePosition(), sf::Color::White);
				if (beta == true)
				{
					SL::DrawTextN({ scrW / 2.f - 300 + 10,scrH - 22.f }, 20, sf::Color::White, "version : " + SL::cTStr(version) + " beta");
				}
				else
				{
					SL::DrawTextN({ scrW / 2.f - 300 + 10,scrH - 22.f }, 20, sf::Color::White, "version : " + SL::cTStr(version));
				}
			}
			else if (menuMode == SELECTION) {
				discordHandle.activitystr = "main menu";
				SL::ClearWindow(
					{ 10,10,25,255 });
				SL::DrawRectangleGradient({ 0,0 }, 1200, 675, { 3,5,25 }, { 19,1,28 });
				static bool LUSM = false;
				if (!LUSM)
				{
					l = getLevelLocalHistData(planetSelected, true);

					SL::DrawCircleLineQ({ (3000.f) - cursorLevel,scrH / 2.f }, 3000 - 100, sf::Color::White, 2, 200);
					SL::DrawCircleQ({ 100.f - cursorLevel,300 }, 45, sf::Color(200, 230, 70), 64);

					if (SL::CheckCollisionCircles(SL::getMousePosition(), 3, { 100 - cursorLevel,300 }, 45))
					{
						planetSelected = 1;
					} if (planetSelected == 1)
					{
						SL::DrawCircleLineQ({ 100.f - cursorLevel,300.f }, 45, sf::Color(255, 255, 255), 2, 64);
					}
					else
					{
						SL::DrawCircleLineQ({ 100.f - cursorLevel,300.f }, 45, sf::Color(0, 0, 0), 2, 64);
					}

					SL::DrawCircleLineQ({ (3000.f) - cursorLevel,scrH / 2.f }, 3000 - 310, sf::Color::White, 2, 200);

					SL::DrawCircleQ({ 320.f - cursorLevel,610 }, 30, sf::Color(130, 200, 100), 64);

					if (SL::CheckCollisionCircles(SL::getMousePosition(), 3, { 320 - cursorLevel,610 }, 45))
					{
						l = getLevelLocalHistData(planetSelected, true);

						planetSelected = 2;
					} if (planetSelected == 2)
					{
						SL::DrawCircleLineQ({ 320.f - cursorLevel,610 }, 30, sf::Color(255, 255, 255), 2, 64);
					}
					else
					{
						SL::DrawCircleLineQ({ 320.f - cursorLevel,610 }, 30, sf::Color(0, 0, 0), 2, 64);
					}

					SL::DrawRectangle({ 0,0 }, 70, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,0 }, { 70,23 }))
					{
						SL::DrawRectangle({ 0,0 }, 70, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = START;
						}
					}
					SL::DrawRectangle({ 70,0 }, 120, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 70,0 }, { 120,23 }))
					{
						SL::DrawRectangle({ 70,0 }, 120, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							LUSM = true;
						}
					}
					SL::DrawTextN({ 70,0 }, 20, sf::Color::White, "custom level");

					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, "< back");
					SL::DrawRectangle({ scrW - 300.f,0 }, 300, scrH, sf::Color::Black);

					SL::DrawTextNI({ scrW - 300,0 }, 25, sf::Color::White, "planet " + SL::cTStr(planetSelected) + " : " + l.name);
					switch (l.difficulty)
					{
					case LevDifficulty::NULLDIF:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : null");
						break;
					case LevDifficulty::SUPEREASY:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : super easy");
						break;
					case LevDifficulty::EASY:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : easy");
						break;
					case LevDifficulty::NORMAL:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : normal");
						break;
					case LevDifficulty::HARD:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : hard");
						break;
					case LevDifficulty::VERYHARD:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : very hard");
						break;
					case LevDifficulty::HARDCORE:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : hardcore");
						break;
					case LevDifficulty::DEATH:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : death");
						break;
					default:
						break;
					}

					SL::DrawTextNI({ scrW - 300,60 }, 20, sf::Color::White, "music : " + l.music);
					SL::DrawTextNI({ scrW - 300,80 }, 20, sf::Color::White, "music by : " + l.musicCName);

					if (l.completed == false)
					{
						SL::DrawTextNI({ scrW - 300,110 }, 20, sf::Color::White, "level not completed");
					} if (l.completed == true)
					{
						SL::DrawTextNI({ scrW - 300,110 }, 20, sf::Color::Green, "level completed");
					}

					if (l.available == true)
					{
						SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(0, 100, 0), sf::Color(10, 255, 10));

						SL::DrawTextN({ scrW - 280.f + 260 / 4 - 15,scrH - 65.f }, 50, sf::Color::White, "START");
						if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW - 280.f,scrH - 60.f }, { 260, 50 }))
						{
							SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(20, 120, 20), sf::Color(30, 255, 30));
							SL::DrawTextN({ scrW - 280.f + 260 / 4 - 15,scrH - 65.f }, 50, sf::Color::White, "START");

							if (SL::IsMouseRelease(sf::Mouse::Left))
							{
								lToPlay = l;
								menuMode = MenuModeE::PLAYING;
								playMusic->openFromFile(lToPlay.musPath);
								menuMusic.stop();
								playMusic->play();
								lToPlay.song = playMusic;
							}
						}
					}
					else
					{
						SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(100, 100, 100), sf::Color(230, 230, 230));

						SL::DrawTextN({ scrW - 280.f + 260 / 4 - 30 ,scrH - 65.f }, 50, sf::Color::White, "LOCKED");
					}//lToPlay
				}
				else {

				SL::ClearWindow(
					{ 10,10,25,255 });


				SL::DrawRectangleGradient({ 0,0 }, 1200 - 300.f, 675, { 3,5,25 }, { 19,1,28 });

				int lasti = 0;
				for (int i = 0; i < numLev; i++)
				{
					if ((75 + cursor) + (i * 40.f) > 0 && (75 + cursor) + (i * 40.f) < 700)
					{
						SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200 - 300.f, 40, { 0,0,0,100 });
						if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,(75 + cursor) + (i * 40.f) }, { 1200 - 300.f, 40 }))
						{
							SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200 - 300.f, 40, { 10,10,13,60 });
							if (SL::IsMouseRelease(sf::Mouse::Button::Left))
							{

								levelSelect = i; l = userLevel[levelSelect];
							}
						}
						else
						{
							SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200 - 300.f, 40, { 0,0,0,100 });
						} SL::DrawTextN({ 0,(75 + cursor) + (i * 40.f) }, 30, sf::Color::White, userLevel[i].name);
					}
					lasti = i;

				}if (numLev >= 15) {
					cursor += SL::mouseScroll();
					if (cursor > 0)
					{
						cursor -= cursor / 10 + 1;
					}if ((75 + cursor) + (lasti * 40.f) < 23)
					{
						cursor += 2;
					}
				}

				SL::DrawRectangleGradient({ 0,0 }, 1200, 75, { 0,0,0 }, { 0,0,10,255 });
				//  SL::DrawRectangleGradient({ 590,0 }, 20, 1000, { 0,0,0 }, { 0,0,0,100 });

				// back button
				
				RefreshSprite.setPosition({ (1200 - 300.f )- ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding });
				if (SL::CheckCollisionPointRec(SL::getMousePosition(), { (1200 - 300.f) - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, { buttonSize,buttonSize }))
				{
					SL::DrawRectangle({ (1200 - 300.f) - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
					if (SL::IsMouseRelease(sf::Mouse::Button::Left))
					{
						numLev = 0;
						for (int i = 0; i < maxLev; i++)
						{

							userLevel[i] = Level();
						}
						for (const auto& entry : fs::directory_iterator(pathToUserLevel)) {
							const auto filenameStr = entry.path().filename().string();
							if (entry.path().filename().extension().string() == ".json") {
								userLevel[numLev] = Level();
								std::cout << "file: " << filenameStr << '\n';

								if (numLev < maxLev)
								{
									userLevel[numLev] = Level(entry.path().string());
								} numLev++;
							}
						}
					}
				}
				UploadSprite.setPosition({ (1200 - 300.f) - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding });
				if (SL::CheckCollisionPointRec(SL::getMousePosition(), { (1200 - 300.f) - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, { buttonSize, buttonSize }))
				{
					SL::DrawRectangle({ (1200 - 300.f) - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
					if (SL::IsMouseRelease(sf::Mouse::Button::Left))
					{
						system(("explorer " + fs::canonical(pathToUserLevel).string()).c_str());
					}
				}
				window.draw(RefreshSprite);
				window.draw(UploadSprite);

					SL::DrawRectangle({ 0,0 }, 70, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,0 }, { 70, 23 }))
					{
						SL::DrawRectangle({ 0,0 }, 70, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = START;
						}
					}
					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, "< back");

					SL::DrawRectangle({ 70,0 }, 120, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 70,0 }, { 120,75 }))
					{
						SL::DrawRectangle({ 70,0 }, 120, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							LUSM = false;
						}
					}
					SL::DrawTextN({ 70,0 }, 20, sf::Color::White, "official level");

					SL::DrawRectangle({ scrW - 300.f,0 }, 300, scrH, sf::Color::Black);
					SL::DrawTextNI({ scrW - 300,0 }, 25, sf::Color::White, "level : " + l.name);
					switch (l.difficulty)
					{
					case LevDifficulty::NULLDIF:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : null");
						break;
					case LevDifficulty::SUPEREASY:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : super easy");
						break;
					case LevDifficulty::EASY:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : easy");
						break;
					case LevDifficulty::NORMAL:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : normal");
						break;
					case LevDifficulty::HARD:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : hard");
						break;
					case LevDifficulty::VERYHARD:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : very hard");
						break;
					case LevDifficulty::HARDCORE:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : hardcore");
						break;
					case LevDifficulty::DEATH:
						SL::DrawTextNI({ scrW - 300,30 }, 20, sf::Color::White, "difficulty : death");
						break;
					default:
						break;
					}

					SL::DrawTextNI({ scrW - 300,60 }, 20, sf::Color::White, "music : " + l.music);
					SL::DrawTextNI({ scrW - 300,80 }, 20, sf::Color::White, "music by : " + l.musicCName);

					if (l.completed == false)
					{
						SL::DrawTextNI({ scrW - 300,110 }, 20, sf::Color::White, "level not completed");
					} if (l.completed == true)
					{
						SL::DrawTextNI({ scrW - 300,110 }, 20, sf::Color::Green, "level completed");
					}

					if (l.available == true)
					{
						SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(0, 100, 0), sf::Color(10, 255, 10));

						SL::DrawTextN({ scrW - 280.f + 260 / 4 - 15,scrH - 65.f }, 50, sf::Color::White, "START");
						if (SL::CheckCollisionPointRec(SL::getMousePosition(), { scrW - 280.f,scrH - 60.f }, { 260, 50 }))
						{
							SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(20, 120, 20), sf::Color(30, 255, 30));
							SL::DrawTextN({ scrW - 280.f + 260 / 4 - 15,scrH - 65.f }, 50, sf::Color::White, "START");

							if (SL::IsMouseRelease(sf::Mouse::Left))
							{
								lToPlay = l;
								menuMode = MenuModeE::PLAYING;
								playMusic->openFromFile(lToPlay.musPath);
								menuMusic.stop();
								playMusic->play();
								lToPlay.song = playMusic;
							}
						}
					}
					else
					{
						SL::DrawRectangleGradient({ scrW - 280.f,scrH - 60.f }, 260, 50, sf::Color(100, 100, 100), sf::Color(230, 230, 230));

						SL::DrawTextN({ scrW - 280.f + 260 / 4 - 30 ,scrH - 65.f }, 50, sf::Color::White, "LOCKED");
					}//lToPlay
				}
				
			}
			else if (menuMode == MenuModeE::PLAYING) {
				discordHandle.activitystr = "playing level " + lToPlay.name;
				SL::ClearWindow(
					{ 10,10,25,255 });
				SL::DrawRectangleGradient({ 0,0 }, 1200, 675, lToPlay.levelBackground.down, lToPlay.levelBackground.up);

				SL::DrawRectangle({ scrW / 2 - lToPlay.size - 1,0 }, (lToPlay.size * 2) + 2, scrH, sf::Color::White);
				SL::DrawRectangle({ scrW / 2 - lToPlay.size,0 }, lToPlay.size * 2, scrH, sf::Color::Black);

				if (paused == true) {
					lToPlay.draw();
					SL::DrawRectangleGradient({ 0,0 }, 1200, 300, { 0,0,0,0 }, { 0,0,0,200 });
					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, "PAUSED");
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,675 - 30 }, { 1200,30 }))
					{
						SL::DrawRectangle({ 0,675 - 30 }, 1200, 30, { 0,0,0,200 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = MenuModeE::EDITION;
						}
					}
					SL::DrawTextN({ 0,675 - 30 }, 20, sf::Color::White, "EXIT");

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,675 - 60 }, { 1200,30 }))
					{
						SL::DrawRectangle({ 0,675 - 60 }, 1200, 30, { 0,0,0,200 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							paused = false;
						}
					}
					SL::DrawTextN({ 0,675 - 60 }, 20, sf::Color::White, "CONTINUE");
				}
				else
				{
					if (SL::IsKeyPressed(sf::Keyboard::Key::Escape))
					{
						paused = true;
					}
					if (SL::IsKeyPressed(sf::Keyboard::Key::R))
					{
						levelTick = 0;
					}levelTick++;
					if (lToPlay.Update(levelTick)) {
						levelTick = 0;
					}
					std::string fp = "fps : ";
					fp += SL::cTStr(SL::getFps());
					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, fp);
					std::string fpt = "tick : ";

					fpt += SL::cTStr(levelTick);
					SL::DrawTextN({ 0,20 }, 20, sf::Color::White, fpt);
				}
			}
			else if (menuMode == MenuModeE::EDITION) {
				discordHandle.activitystr = "editing level ";
				if (!openLevelData && !openLevelEditor) {
					SL::ClearWindow(
						{ 10,10,25,255 });


					SL::DrawRectangleGradient({ 0,0 }, 1200, 675, { 3,5,25 }, { 19,1,28 });
					
					int lasti = 0;
					for (int i = 0; i < numLev; i++)
					{
						if ((75 + cursor) + (i * 40.f) > 0 && (75 + cursor) + (i * 40.f) < 1200)
						{
							SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200, 40, { 0,0,0,100 });
							if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,(75 + cursor) + (i * 40.f) }, { 1200, 40 }))
							{
								SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200, 40, { 10,10,13,60 });
								if (SL::IsMouseRelease(sf::Mouse::Button::Left))
								{
									openLevelData = true;
									levelSelect = i;
								}
							}
							else
							{
								SL::DrawRectangle({ 0,(75 + cursor) + (i * 40.f) }, 1200, 40, { 0,0,0,100 });
							} SL::DrawTextN({ 0,(75 + cursor) + (i * 40.f) }, 30, sf::Color::White, userLevel[i].name);
						}
						lasti = i;
						
					}if (numLev >= 15) {
						cursor += SL::mouseScroll();
						if (cursor > 0)
						{
							cursor -= cursor / 10 + 1;
						}if ((75 + cursor) + (lasti * 40.f) < 75)
						{
							cursor += 2;
						}
					}
					
					SL::DrawRectangleGradient({ 0,0 }, 1200, 75, { 0,0,0 }, { 0,0,10,255 });
					//  SL::DrawRectangleGradient({ 590,0 }, 20, 1000, { 0,0,0 }, { 0,0,0,100 });
					SL::DrawTextNC({ 0,0 }, 1200 / 2, 50, { 255,255,255 }, "EDITOR");

					// back button
					SL::DrawRectangle({ 0,0 }, 70, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,0 }, { 70,23 }))
					{
						SL::DrawRectangle({ 0,0 }, 70, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = START;
						}
					}
					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, "< back");
					NewSprite.setPosition({ 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding }, { buttonSize,buttonSize }))
					{
						SL::DrawRectangle({ 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							openLevelEditor = false;
							openLevelData = true;
							lToEdit = userLevel[levelSelect];
							int idu = 0;

							while (true)
							{
								std::string levName = "unnamed ";
								levName += SL::cTStr(idu);
								if (!fs::exists(fs::path((pathToUserLevel.string() + "/" + levName + ".json")))) {
									std::ofstream outfile(pathToUserLevel.string() + "/" + levName + ".json");

									outfile << "{" <<
										"}" << std::endl;

									outfile.close();
									break;
								}
								else
								{
									idu++;
								}
							}
							numLev = 0;
							for (const auto& entry : fs::directory_iterator(pathToUserLevel)) {
								userLevel[numLev] = Level();
								const auto filenameStr = entry.path().filename().string();
								if (entry.path().filename().extension().string() == ".json") {
									std::cout << "file: " << filenameStr << '\n';

									if (numLev < maxLev)
									{
										userLevel[numLev] = Level(entry.path().string());
									} numLev++;
								}
							}
						}
					};
					RefreshSprite.setPosition({ 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, { buttonSize,buttonSize }))
					{
						SL::DrawRectangle({ 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							numLev = 0;
							for (int i = 0; i < maxLev; i++)
							{

								userLevel[i] = Level();
							}
							for (const auto& entry : fs::directory_iterator(pathToUserLevel)) {
								const auto filenameStr = entry.path().filename().string();
								if (entry.path().filename().extension().string() == ".json") {
									userLevel[numLev] = Level();
									std::cout << "file: " << filenameStr << '\n';

									if (numLev < maxLev)
									{
										userLevel[numLev] = Level(entry.path().string());
									} numLev++;
								}
							}
						}
					}
					UploadSprite.setPosition({ 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, { buttonSize, buttonSize }))
					{
						SL::DrawRectangle({ 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							system(("explorer " + fs::canonical(pathToUserLevel).string()).c_str());
						}
					}
					window.draw(NewSprite);
					window.draw(RefreshSprite);
					window.draw(UploadSprite);
				}

				else if (openLevelData && !openLevelEditor) {
					static bool DeleteOpen = false;
					static bool deleting = false;
					static bool DeleteClose = false;
					if (DeleteOpen == true)
					{
						ImGui::Begin("DELETE LEVEL", &DeleteOpen);
						ImGui::Text("do you want to delete this level ? you will not be able to go back");
						if (ImGui::Button("Delete"))
						{
							DeleteOpen = false;
							numLev = 0;

							std::remove(userLevel[levelSelect].pathLevel.c_str()); // delete file
							for (int i = 0; i < maxLev; i++)
							{

								userLevel[i] = Level();
							}
							for (const auto& entry : fs::directory_iterator(pathToUserLevel)) {
								const auto filenameStr = entry.path().filename().string();
								if (entry.path().filename().extension().string() == ".json") {
									userLevel[numLev] = Level();

									std::cout << "file: " << filenameStr << '\n';

									if (numLev < maxLev)
									{
										
										userLevel[numLev] = Level(entry.path().string());
									} numLev++;
								}
							}
							openLevelData = false;
							continue;
						}

						if (ImGui::Button("cancel"))
						{
							openLevelData = false;
							DeleteOpen = false;
						}
						ImGui::End();
					}
					SL::ClearWindow(
						{ 10,10,25,255 });

					SL::DrawRectangleGradient({ 0,0 }, 1200, 675, { 3,5,25 }, { 19,1,28 });

					SL::DrawRectangleGradient({ 0,0 }, 1200, 75, { 0,0,0 }, { 0,0,0,100 });
					//  SL::DrawRectangleGradient({ 590,0 }, 20, 1000, { 0,0,0 }, { 0,0,0,100 });
					SL::DrawTextNC({ 0,0 }, 1200 / 2, 50, { 255,255,255 }, userLevel[levelSelect].name);

					// back button
					SL::DrawRectangle({ 0,0 }, 70, 23, sf::Color::Black);
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,0 }, { 70,23 }))
					{
						SL::DrawRectangle({ 0,0 }, 70, 23, { 20,20,23 });
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							openLevelData = false;
						}
					}
					SL::DrawTextN({ 0,0 }, 20, sf::Color::White, "< back");
					// -------- edit button

					editorSprite.setPosition({ 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding }, { buttonSize,buttonSize }))
					{
						SL::DrawRectangle({ 1200 - (buttonSize + buttonEditorPadding),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							openLevelEditor = true;
							openLevelData = false;
							lToEdit = userLevel[levelSelect];
							lToEdit.edit = true;
						}
					}
					PlaySprite.setPosition({ 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, { buttonSize,buttonSize }))
					{
						SL::DrawRectangle({ 1200 - ((buttonSize + buttonEditorPadding) * 2),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							menuMode = MenuModeE::PLAYING;
							lToPlay = userLevel[levelSelect];
						}
					}
					exitSprite.setPosition({ 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, { buttonSize, buttonSize }))
					{
						SL::DrawRectangle({ 1200 - ((buttonSize + buttonEditorPadding) * 3),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							DeleteOpen = true;
						}
					}
					SaveSprite.setPosition({ 1200 - ((buttonSize + buttonEditorPadding) * 4),buttonEditorPadding });
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 1200 - ((buttonSize + buttonEditorPadding) * 4),buttonEditorPadding }, { buttonSize, buttonSize }))
					{
						SL::DrawRectangle({ 1200 - ((buttonSize + buttonEditorPadding) * 4),buttonEditorPadding }, buttonSize, buttonSize, sf::Color(255, 255, 255, 10));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							userLevel[levelSelect].save();
						}
					}
					
					window.draw(editorSprite);
					window.draw(PlaySprite);
					window.draw(exitSprite);
					window.draw(SaveSprite);
					/* SL::DrawTextureA(editorIcon, , 0, { 0,0 }, 64.f / 1024);
					 SL::DrawTextureA(PlayIcon, { 1200 - (64*2),5 }, 0, { 0,0 }, 64.f / 1024);
					 SL::DrawTextureA(exitIcon, { 1200 - (64*3),5 }, 0, { 0,0 }, 64.f / 1024);*/

					 // ------------------- level name -------------------------
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,75 }, { 500,25 }))
					{
						SL::DrawRectangle({ 0,75 }, 500, 25, { 200,200,200,20 });
						SL::DrawTextN({ 0,75 }, 20, sf::Color::White, userLevel[levelSelect].name);
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							userLevel[levelSelect].name = SL::popup("set new level name", userLevel[levelSelect].name, &window);
						}
					}
					else
					{
						SL::DrawTextN({ 0,75 }, 20, sf::Color::White, userLevel[levelSelect].name);
					}

					// ------------------- level difficulty -------------------------

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,100 }, { 500,25 }))
					{
						SL::DrawRectangle({ 0,100 }, 500, 25, { 200,200,200,20 });
						SL::DrawTextN({ 0,100 }, 20, sf::Color::White, GetDifficulty(userLevel[levelSelect].difficulty));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							userLevel[levelSelect].difficulty = SL::popupdiff("set new level difficulty", userLevel[levelSelect].difficulty, &window);
						}
					}
					else
					{
						SL::DrawTextN({ 0,100 }, 20, sf::Color::White, GetDifficulty(userLevel[levelSelect].difficulty));
					}

					// ------------------- level song -------------------------
					SL::DrawTextN({ 0,130 }, 25, sf::Color::White, " song settings :");

					// -------------------- song Name

					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,165 }, { 500,25 }))
					{
						SL::DrawRectangle({ 0,165 }, 500, 25, { 200,200,200,20 });
						SL::DrawTextN({ 0,165 }, 20, sf::Color::White, "song name : " + (userLevel[levelSelect].music));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							userLevel[levelSelect].music = SL::popup("set new level music name", userLevel[levelSelect].music, &window);
						}
					}
					else
					{
						SL::DrawTextN({ 0,165 }, 20, sf::Color::White, "song name : " + (userLevel[levelSelect].music));
					}

					// -------------------- song Creator Name
					if (SL::CheckCollisionPointRec(SL::getMousePosition(), { 0,190 }, { 500,25 }))
					{
						SL::DrawRectangle({ 0,190 }, 500, 25, { 200,200,200,20 });
						SL::DrawTextN({ 0,190 }, 20, sf::Color::White, "song creator name : " + (userLevel[levelSelect].musicCName));
						if (SL::IsMouseRelease(sf::Mouse::Button::Left))
						{
							userLevel[levelSelect].musicCName = SL::popup("set new level music creator name", userLevel[levelSelect].musicCName, &window);
						}
					}
					else
					{
						SL::DrawTextN({ 0,190 }, 20, sf::Color::White, "song creator name : " + (userLevel[levelSelect].musicCName));
					}
				}
				else if (!openLevelData && openLevelEditor) {
					SL::UpdateSLgine();
					int idx = 0;
					ImGui::BeginMainMenuBar();
					if (ImGui::Button("exit")) {
						exit = true;
					};
					if (ImGui::Button("save")) {
						printf("saving...");
						lToEdit.save();
						printf("saved !");
					};
					if (ImGui::Button("background"))
					{
						backgroundSetopen = true;
					}

					std::string fp = "fps : ";
					fp += SL::cTStr(SL::getFps());

					ImGui::SameLine();
					ImGui::Text(fp.c_str());
					ImGui::EndMainMenuBar();
					if (backgroundSetopen)
					{
						ImGui::Begin("background");

						float colup[3] = { lToEdit.levelBackground.up.r / 255.f,lToEdit.levelBackground.up.g / 255.f,lToEdit.levelBackground.up.b / 255.f };
						if (ImGui::ColorPicker3("up", colup))
						{
							lToEdit.levelBackground.up.r = colup[0] * 255.f;
							lToEdit.levelBackground.up.g = colup[1] * 255.f;
							lToEdit.levelBackground.up.b = colup[2] * 255.f;
							lToEdit.levelL["level"]["background"]["up"]["r"] = lToEdit.levelBackground.up.r;
							lToEdit.levelL["level"]["background"]["up"]["g"] = lToEdit.levelBackground.up.g;
							lToEdit.levelL["level"]["background"]["up"]["b"] = lToEdit.levelBackground.up.b;
						}float coldown[3] = { lToEdit.levelBackground.down.r / 255.f,lToEdit.levelBackground.down.g / 255.f,lToEdit.levelBackground.down.b / 255.f };
						if (ImGui::ColorPicker3("down", coldown))
						{
							lToEdit.levelBackground.down.r = coldown[0] * 255.f;
							lToEdit.levelBackground.down.g = coldown[1] * 255.f;
							lToEdit.levelBackground.down.b = coldown[2] * 255.f;
							lToEdit.levelL["level"]["background"]["down"]["r"] = lToEdit.levelBackground.down.r;
							lToEdit.levelL["level"]["background"]["down"]["g"] = lToEdit.levelBackground.down.g;
							lToEdit.levelL["level"]["background"]["down"]["b"] = lToEdit.levelBackground.down.b;
						}

						if (ImGui::Button("close"))
						{
							backgroundSetopen = false;
						}
						ImGui::End();
					}
					if (exit) {
						ImGui::Begin("exit");
						ImGui::Text("warning all content can be lost !");
						if (ImGui::Button("exit without saving")) {
							lToEdit.loadFromFile(lToEdit.pathLevel);
							lToEdit.edit = false;
							exit = false; openLevelEditor = false; openLevelData = true;
						};
						ImGui::SameLine();
						if (ImGui::Button("save and exit")) {
							exit = false;

							lToEdit.edit = false;
							printf("saving...");
							lToEdit.save();
							printf("saved !"); openLevelEditor = false; openLevelData = true;
						};

						ImGui::End();
					}

					ImGui::Begin("time", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
					ImGui::SetWindowPos({ 0,((float)window.getSize().y / 10.f) * 8 });
					ImGui::SetWindowSize({ (float)window.getSize().x,(float)100 });
					ImGui::BeginGroup();

					// slider will fill the space and leave 100 pixels for the label
					ImGui::PushItemWidth((float)window.getSize().x * -0.5);
					if (ImGui::SliderInt("", &tickedit, 0, lToEdit.sizemax)) {
						shouldUpdate = true;
					};
					ImGui::SameLine();
					ImGui::PushItemWidth((float)window.getSize().x * 0.1);
					if (ImGui::InputInt("LevelSize", &lToEdit.sizemax)) {
						lToEdit.levelL["level"]["size"] = lToEdit.sizemax;
						shouldUpdate = true;
					};

					ImGui::SameLine();
					ImGui::PushItemWidth((float)window.getSize().x * 0.1);
					if (shouldrun == false)
					{
						if (ImGui::Button("play")) { shouldrun = true; };
					}
					else
					{
						if (ImGui::Button("stop")) { shouldrun = false; };
					}

					if (ImGui::Button("next event")) {
						int idex = 0;

						for (int i = tickedit + 1; i < lToEdit.sizemax; i++)
						{
							std::string t = std::string("tick") + SL::cTStr(int(i - 1));
							idx = 0;
							while (!lToEdit.levelL["level"]["data"][t][("event")][idx].is_null())

							{
								if (lToEdit.levelL["level"]["data"][t][("event")][idx]["exist"].get<bool>() != false)
								{
									tickedit = i;
									i = lToEdit.sizemax * 2; // security
									shouldUpdate = true;
									break;
								}
							}
						}
					};
					ImGui::SameLine();
					if (ImGui::Button("back event")) {
						int idex = 0;

						for (int i = tickedit - 1; i > 0; i--)
						{
							std::string t = std::string("tick") + SL::cTStr(int(i - 1));
							idx = 0;
							while (!lToEdit.levelL["level"]["data"][t][("event")][idx].is_null())

							{
								if (lToEdit.levelL["level"]["data"][t][("event")][idx]["exist"].get<bool>() != false)
								{
									tickedit = i;
									i -= lToEdit.sizemax * 2; // security
									shouldUpdate = true;
									break;
								}
							}
						}
					};
					ImGui::EndGroup();

					ImGui::End();
					if (SL::IsKeyPressed(sf::Keyboard::Key::Right) && SL::IsKeyPressed(sf::Keyboard::Key::LAlt))
					{
						shouldUpdate = true;
						tickedit += 1;
					}
					else if (SL::IsKeyPressed(sf::Keyboard::Key::Left) && SL::IsKeyPressed(sf::Keyboard::Key::LAlt))
					{
						shouldUpdate = true;
						tickedit -= 1;
					}
					ImGui::Begin("elements :");
					std::string t = std::string("tick") + SL::cTStr(int(tickedit - 1));
					ImGui::Text("projectile :");
					while (!lToEdit.levelL["level"]["data"][t][("projList")][idx].is_null())

					{
						if (lToEdit.levelL["level"]["data"][t][("projList")][idx]["exist"] != false)
						{
							if (ImGui::Selectable(("element " + SL::cTStr(idx)).c_str())) {
								j.projpos = { lToEdit.levelL["level"]["data"][t][("projList")][idx][("posx")].get<float>(),lToEdit.levelL["level"]["data"][t][("projList")][idx][("posy")].get<float>() };
								j.projvel = { lToEdit.levelL["level"]["data"][t][("projList")][idx][("velx")].get<float>(),lToEdit.levelL["level"]["data"][t][("projList")][idx][("vely")].get<float>() };
								j.size = lToEdit.levelL["level"]["data"][t][("projList")][idx][("size")].get<int>();
								j.id = lToEdit.levelL["level"]["data"][t][("projList")][idx][("id")].get<int>();
								j.tickcreated = tickedit - 1;
								j.part = &lToEdit.levelL["level"]["data"][t][("projList")][idx];
								try
								{
									j.projCol =
									{
										(sf::Uint8) lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["r"].get<unsigned int>(),
										(sf::Uint8) lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["g"].get<unsigned int>(),
										(sf::Uint8) lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["b"].get<unsigned int>()
									};
								}
								catch (nlohmann::json::type_error & e) {
									j.projCol = sf::Color::White;
									lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["r"] = 255;
									lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["g"] = 255;
									lToEdit.levelL["level"]["data"][t][("projList")][idx][("color")]["b"] = 255;
								}
								eventSelect = false;
								projectiletoedit = &j;
							} ImGui::SameLine();
							if (ImGui::Button("X"))
							{
								lToEdit.levelL["level"]["data"][t][("projList")][idx]["exist"] = false;
								shouldUpdate = true;
								//     lToEdit.levelL["level"]["data"][t][("projList")].erase(idx);
							}
						}

						idx++;
					}
					if (ImGui::Button("new projectile")) {
						std::string tn = std::string("tick") + SL::cTStr(int(tickedit - 1));
						idx = 0;

						try
						{
							int idxnew = idx + 1;

							nlohmann::json nproj = nlohmann::json();
							nproj[("posx")] = 600.f;
							nproj[("posy")] = 90.f;
							nproj[("velx")] = 0.f;
							nproj[("vely")] = 0.f;
							nproj[("size")] = 5;
							nproj[("id")] = 0;
							nproj[("color")]["r"] = 255;
							nproj[("color")]["g"] = 255;
							nproj[("color")]["b"] = 255;
							nproj["exist"] = true;

							nproj[("accx")] = 1.f;
							nproj[("accy")] = 1.f;
							if (idx == 0)
							{
								int idxnew = idx;
							}
							int elementindex = 0;
							while (true)
							{
								if (lToEdit.levelL["level"]["data"][t][("projList")][elementindex].is_null() ||
									lToEdit.levelL["level"]["data"][t][("projList")][idx]["exist"] == false)
								{
									lToEdit.levelL["level"]["data"][tn][("projList")][elementindex] = nproj;
									shouldUpdate = true;
									break;
								}
								elementindex++;
							}
						}
						catch (nlohmann::json::type_error & e) {
							printf("error json : %s", e.what());
						}
						//   auto new_pos = v.insert(v.end(), v2.begin(), v2.end());
						   //lToEdit.levelL["level"]["data"][t][("projList")][idx + 1] = nproj;
					}

					ImGui::Text("event :");
					idx = 0;
					while (!lToEdit.levelL["level"]["data"][t][("event")][idx].is_null())

					{
						if (lToEdit.levelL["level"]["data"][t][("event")][idx]["exist"].get<bool>() != false)
						{
							try
							{
								if (ImGui::Selectable(("event " + SL::cTStr(idx)).c_str())) {
									eventSelect = true;
									eventjson = &lToEdit.levelL["level"]["data"][t][("event")][idx];
									ev.idtriggered = lToEdit.levelL["level"]["data"][t][("event")][idx]["id"].get<int>();;
									ev.changeSize = lToEdit.levelL["level"]["data"][t][("event")][idx]["changeSize"].get<bool>();
									ev.changevel = lToEdit.levelL["level"]["data"][t][("event")][idx]["changeVel"].get<bool>();
									ev.changePos = lToEdit.levelL["level"]["data"][t][("event")][idx]["changePos"].get<bool>();
									ev.changeColor = lToEdit.levelL["level"]["data"][t][("event")][idx]["changeColor"].get<bool>();

									ev.relative = lToEdit.levelL["level"]["data"][t][("event")][idx]["relative"].get<bool>();

									ev.dataChanged.size = lToEdit.levelL["level"]["data"][t][("event")][idx]["size"].get<int>();

									ev.dataChanged.projvel = {
										lToEdit.levelL["level"]["data"][t][("event")][idx]["velx"].get<float>(),
										lToEdit.levelL["level"]["data"][t][("event")][idx]["vely"].get<float>() };

									ev.dataChanged.projpos = {
										lToEdit.levelL["level"]["data"][t][("event")][idx]["posx"].get<float>(),
										lToEdit.levelL["level"]["data"][t][("event")][idx]["posy"].get<float>() };

									ev.dataChanged.projCol = {
										(sf::Uint8)lToEdit.levelL["level"]["data"][t][("event")][idx]["color"]["r"].get<unsigned int>(),
										(sf::Uint8)lToEdit.levelL["level"]["data"][t][("event")][idx]["color"]["g"].get<unsigned int>(),
										(sf::Uint8)lToEdit.levelL["level"]["data"][t][("event")][idx]["color"]["b"].get<unsigned int>()
									};
									//ev.changeColor
									try
									{
										ev.changeAcceleration = lToEdit.levelL["level"]["data"][t][("event")][idx]["changeAcceleration"].get<bool>();
										ev.dataChanged.acceleration = {
																		 lToEdit.levelL["level"]["data"][t][("event")][idx]["accx"].get<float>(),
																		 lToEdit.levelL["level"]["data"][t][("event")][idx]["accy"].get<float>() };
									}
									catch (nlohmann::json::type_error&)
									{
										lToEdit.levelL["level"]["data"][t][("event")][idx]["changeAcceleration"] = false; ev.changeAcceleration = lToEdit.levelL["level"]["data"][t][("event")][idx]["changeAcceleration"].get<bool>();
										lToEdit.levelL["level"]["data"][t][("event")][idx]["accx"] = (float)1;
										lToEdit.levelL["level"]["data"][t][("event")][idx]["accy"] = (float)1;

										ev.dataChanged.acceleration = {
																		 lToEdit.levelL["level"]["data"][t][("event")][idx]["accx"].get<float>(),
																		 lToEdit.levelL["level"]["data"][t][("event")][idx]["accy"].get<float>() };
									}
								}
							}
							catch (nlohmann::json::type_error & e)
							{
							} ImGui::SameLine();
							if (ImGui::Button("X"))
							{
								lToEdit.levelL["level"]["data"][t][("event")][idx]["exist"] = false;
								shouldUpdate = true;
								//     lToEdit.levelL["level"]["data"][t][("projList")].erase(idx);
							}
						}

						idx++;
					}
					if (ImGui::Button("new event")) {
						std::string tn = std::string("tick") + SL::cTStr(int(tickedit - 1));
						idx = 0;

						try
						{
							int idxnew = idx + 1;

							nlohmann::json nevent = nlohmann::json();
							nevent[("id")] = 0;
							nevent[("posx")] = 0;
							nevent[("posy")] = 0.f;
							nevent[("velx")] = 0.f;
							nevent[("vely")] = 0.f;
							nevent[("size")] = 0;
							nevent[("changeSize")] = false;
							nevent[("changeVel")] = false;
							nevent[("changePos")] = false;
							nevent[("relative")] = false;
							nevent[("changeColor")] = false;
							nevent[("id")] = 0;

							nevent[("color")]["r"] = 255;
							nevent[("color")]["g"] = 255;
							nevent[("color")]["b"] = 255;
							nevent["exist"] = true;
							if (idx == 0)
							{
								int idxnew = idx;
							}
							int elementindex = 0;
							while (true)
							{
								if (lToEdit.levelL["level"]["data"][t][("event")][elementindex].is_null() ||
									lToEdit.levelL["level"]["data"][t][("event")][elementindex]["exist"] == false)
								{
									lToEdit.levelL["level"]["data"][tn][("event")][elementindex] = nevent;
									shouldUpdate = true;
									break;
								}
								elementindex++;
							}
						}
						catch (nlohmann::json::type_error & e) {
							printf("error json : %s", e.what());
						}
						//   auto new_pos = v.insert(v.end(), v2.begin(), v2.end());
						   //lToEdit.levelL["level"]["data"][t][("projList")][idx + 1] = nproj;
					}

					ImGui::End();

					if (projectiletoedit != nullptr && !eventSelect) {
						try
						{
							float dx = projectiletoedit->part->operator[](("posx")).get<float>();
							float dy = projectiletoedit->part->operator[]("posy").get<float>();
							float dsize = projectiletoedit->part->operator[]("size").get<float>();
							float dvelx = projectiletoedit->part->operator[]("velx").get<float>();
							float dvely = projectiletoedit->part->operator[]("vely").get<float>();
							float daccx = projectiletoedit->part->operator[]("accx").get<float>();
							float daccy = projectiletoedit->part->operator[]("accy").get<float>();
							float sdx = projectiletoedit->part->operator[](("posx")).get<float>();
							float sdy = projectiletoedit->part->operator[]("posy").get<float>();
							int did = projectiletoedit->part->operator[]("id").get<int>();
							ImGui::Begin("object property");

							ImGui::PushItemWidth((float)window.getSize().x * 0.05);
							if (ImGui::DragFloat("start x", &dx, 1.f, 0.f, 1200.f)) {
								projectiletoedit->part->operator[]("posx") = dx;
								projectiletoedit->projpos.x += dx - sdx;
								shouldUpdate = true;
							}
							ImGui::SameLine();
							if (ImGui::DragFloat("velocity x", &dvelx))
							{
								projectiletoedit->part->operator[]("velx") = dvelx;
								projectiletoedit->projvel.x = dvelx;
								shouldUpdate = true;
							}

							ImGui::PushItemWidth((float)window.getSize().x * 0.05);
							if (ImGui::DragFloat("start y ", &dy, 1.f, 0.f, 600.f)) {
								projectiletoedit->part->operator[]("posy") = dy;
								projectiletoedit->projpos.y += dy - sdy;
								shouldUpdate = true;
							};
							ImGui::SameLine();
							if (ImGui::DragFloat("velocity y", &dvely))
							{
								projectiletoedit->part->operator[]("vely") = dvely;
								projectiletoedit->projvel.y = dvely;
								shouldUpdate = true;
							}
							if (ImGui::DragFloat("acceleration x", &daccx))
							{
								projectiletoedit->part->operator[]("accx") = daccx;
								projectiletoedit->acceleration.x = daccx;
								shouldUpdate = true;
							}if (ImGui::DragFloat("acceleration y", &daccy))
							{
								projectiletoedit->part->operator[]("accy") = daccy;
								projectiletoedit->acceleration.y = daccy;
								shouldUpdate = true;
							}

							float coltemp[3] = { projectiletoedit->projCol.r / 255.f, projectiletoedit->projCol.g / 255.f,projectiletoedit->projCol.b / 255.f };

							ImGui::PushItemWidth((float)window.getSize().x * 0.1);
							if (ImGui::ColorEdit3("projectile color", coltemp)) {
								projectiletoedit->projCol.r = static_cast<sf::Uint8>(coltemp[0] * 255.f); // real time edit
								projectiletoedit->projCol.g = static_cast<sf::Uint8>(coltemp[1] * 255.f);
								projectiletoedit->projCol.b = static_cast<sf::Uint8>(coltemp[2] * 255.f);
								projectiletoedit->projCol.a = 255;

								projectiletoedit->part->operator[]("color").operator[]("r") = projectiletoedit->projCol.r; // json edit
								projectiletoedit->part->operator[]("color").operator[]("g") = projectiletoedit->projCol.g;
								projectiletoedit->part->operator[]("color").operator[]("b") = projectiletoedit->projCol.b;
							};
							ImGui::PushItemWidth((float)window.getSize().x * 0.1);
							if (ImGui::DragFloat("size ", &dsize, 1.f, 0.f, 600.f)) {
								projectiletoedit->part->operator[]("size") = dsize;
								projectiletoedit->size = dsize;
								shouldUpdate = true;
							};
							if (ImGui::InputInt("id", &did))
							{
								projectiletoedit->part->operator[]("id") = did;
								projectiletoedit->id = did;
								shouldUpdate = true;
							}

							if (ImGui::Button("go to"))
							{
								tickedit = projectiletoedit->tickcreated;
								shouldUpdate = true;
							}

							//     projectiletoedit = &lToEdit.Map.ProjectileMap[i];
							ImGui::End();
						}

						catch (nlohmann::json::type_error & e) {}  // for error handle if there are not tick (file are smaller and can be read
					}
					else if (eventSelect) {
						try
						{
							ImGui::Begin("event property");

							ImGui::SameLine();
							if (ImGui::InputInt("event id", &ev.idtriggered))
							{
								eventjson->operator[]("id") = ev.idtriggered;
								shouldUpdate = true;
							}
							if (ImGui::Checkbox("relative", &ev.relative))
							{
								eventjson->operator[]("relative") = ev.relative;
								shouldUpdate = true;
							}

							if (ImGui::Checkbox("change position", &ev.changePos))
							{
								eventjson->operator[]("changePos") = ev.changePos;
								shouldUpdate = true;
							}

							if (ev.changePos)
							{
								ImGui::Text("position :");
								if (ImGui::InputFloat("x", &ev.dataChanged.projpos.x))
								{
									eventjson->operator[]("posx") = ev.dataChanged.projpos.x;
									shouldUpdate = true;
								}
								if (ImGui::InputFloat("y", &ev.dataChanged.projpos.y))
								{
									eventjson->operator[]("posy") = ev.dataChanged.projpos.y;
									shouldUpdate = true;
								}
							}

							if (ImGui::Checkbox("change velocity", &ev.changevel))
							{
								eventjson->operator[]("changeVel") = ev.changevel;
								shouldUpdate = true;
							}

							if (ev.changevel)
							{
								ImGui::Text("velocity :");
								if (ImGui::InputFloat("x", &ev.dataChanged.projvel.x))
								{
									eventjson->operator[]("velx") = ev.dataChanged.projvel.x;
									shouldUpdate = true;
								}
								if (ImGui::InputFloat("y", &ev.dataChanged.projvel.y))
								{
									eventjson->operator[]("vely") = ev.dataChanged.projvel.y;
									shouldUpdate = true;
								}
							}

							if (ImGui::Checkbox("change size", &ev.changeSize))
							{
								eventjson->operator[]("changeSize") = ev.changeSize;
								shouldUpdate = true;
							}

							if (ev.changeSize)
							{
								ImGui::Text("size :");
								if (ImGui::DragInt("size", &ev.dataChanged.size))
								{
									eventjson->operator[]("size") = ev.dataChanged.size;
									shouldUpdate = true;
								}
							}
							if (ImGui::Checkbox("change acceleration", &ev.changeAcceleration))
							{
								eventjson->operator[]("changeAcceleration") = ev.changeAcceleration;
								shouldUpdate = true;
							}

							if (ev.changeAcceleration)
							{
								ImGui::Text("acceleration :");
								if (ImGui::InputFloat("x", &ev.dataChanged.acceleration.x))
								{
									eventjson->operator[]("accx") = ev.dataChanged.acceleration.x;
									shouldUpdate = true;
								}
								if (ImGui::InputFloat("y", &ev.dataChanged.acceleration.y))
								{
									eventjson->operator[]("accy") = ev.dataChanged.acceleration.y;
									shouldUpdate = true;
								}
							}

							/*  if (ImGui::Checkbox("spawn projectile", &ev.changeSize))
							  {
								  eventjson->operator[]("changeSize") = ev.changeSize;
								  shouldUpdate = true;
							  }*/

							  //     projectiletoedit = &lToEdit.Map.ProjectileMap[i];
							ImGui::End();
						}

						catch (nlohmann::json::type_error & e) {}  // for error handle if there are not tick (file are smaller and can be read
					}

					if (shouldrun == false)
					{
						if (ImGui::GetIO().MouseClicked[0])
						{
							for (int iind = 0; iind < 400; iind++)
							{
								if (lToEdit.Map.ProjectileMap[iind].available)
								{
									if (SL::CheckCollisionCircleRec(lToEdit.Map.ProjectileMap[iind].projpos, lToEdit.Map.ProjectileMap[iind].size + 3, { float(SL::getMousePosition().x - (1200 / 10)) * (1200 / (1200 * 0.8f)),(float)SL::getMousePosition().y * (675 / (675 * 0.8f)) }, { 2,2 })) {
										projectiletoedit = &lToEdit.Map.ProjectileMap[iind];
										eventSelect = false;
										break;
									};
								}
								;
							}
						}
					}if (update == 0 && shouldUpdate)
					{
						editorUpdateThread = std::async(&Level::Update, &lToEdit, tickedit);
						//  editorUpdateThread.share();
						//  editorUpdateThread.detach();
					}if (shouldUpdate)
					{
						status = editorUpdateThread.wait_for(std::chrono::nanoseconds(0));
						++update;
					}

					if (shouldUpdate && editorUpdateThread.valid()) {
						if (editorUpdateThread.wait_for(std::chrono::nanoseconds(0)) == std::future_status::ready)
						{
							editorUpdateThread.wait();

							update = 0;

							shouldUpdate = false;
						}
					}

					SL::DrawRectangleGradient({ 0,0 }, 1200, 675, lToEdit.levelBackground.down, lToEdit.levelBackground.up);

					SL::DrawRectangle({ scrW / 2 - lToEdit.size - 1,0 }, (lToEdit.size * 2) + 2, scrH, sf::Color::White);
					SL::DrawRectangle({ scrW / 2 - lToEdit.size,0 }, lToEdit.size * 2, scrH, sf::Color::Black);

					if (shouldrun == true)
					{
						lToEdit.edit = false;
						tickedit++;
						if (lToEdit.Update(tickedit)) {
							tickedit = 0;
						};
						texturetheadpic.create(float(window.getSize().x), float(window.getSize().y));
						texturetheadpic.update(window);
					}
					else
					{
						if (status == std::future_status::ready || !editorUpdateThread.valid()) {
							lToEdit.edit = true;
							lToEdit.draw();
							if (projectiletoedit != nullptr)
							{
								SL::DrawLine(projectiletoedit->projpos, sf::Vector2f({ projectiletoedit->projvel.x * 20,projectiletoedit->projvel.y * 20 }) + projectiletoedit->projpos, sf::Color::Red);
							}
							texturetheadpic.create(float(window.getSize().x), float(window.getSize().y));
							texturetheadpic.update(window);
						}
					}

					SL::ClearWindow(sf::Color::Black);
					//SL::renderFromzone({1200/10,0}, 0.8f);

					SL::DrawTextureA(texturetheadpic, { 1200 / 10,0 }, 0.f, { 0.f,0.f }, (1200 * 0.8f / window.getSize().x));

					//SL::DisplayWindow();
				};
			};
			if (alpha)
			{
				SL::DrawTextN({ 0,675 - 35 }, 30, { 255,255,255,50 }, "alpha version");
			}
			discordHandle.update();
			ImGui::SFML::Render(window);
			SL::DisplayWindow();
		};
#ifdef ERROR_HANDLE
	}
	catch (std::exception & e)
	{
		printf("an error has occured \n");
		printf(e.what());
		abort();
	}
	catch (...)
	{
		printf("an error has occured %s \n", __LINE__);
		abort();
	}
#endif
	ImGui::SFML::Shutdown();
	return 0;
};
#include "SLgineImport.h"
std::string GetDifficultyGUI(int l) {
	switch (l)
	{
	case -1:
		return ("null");
		break;
	case 0:
		return ("difficulty : super easy");
		break;
	case 1:
		return ("difficulty : easy");
		break;
	case 2:
		return ("difficulty : normal");
		break;
	case 3:
		return ("difficulty : hard");
		break;
	case 4:
		return ("difficulty : very hard");
		break;
	case 5:
		return ("difficulty : hardcore");
		break;
	case 6:
		return ("difficulty : death");
		break;
	default:
		break;
	}
}

namespace SL
{
	bool guiButton(sf::Vector2f pos, sf::Vector2f size, std::string text) {
		auto io = ImGui::GetIO();

		if (SL::CheckCollisionPointRec(SL::getMousePosition(), { pos.x , pos.y - 3 }, { SL::getTextSize(text, size.y) + size.x, size.y + 6 }))
		{
			SL::DrawRectangle({ pos.x , pos.y - 3 }, SL::getTextSize(text, size.y) + size.x, size.y + 6, sf::Color(40, 40, 40, 100));
		}
		else
		{
			SL::DrawRectangle({ pos.x , pos.y - 3 }, SL::getTextSize(text, size.y) + size.x, size.y + 6, sf::Color(20, 20, 20, 100));
		}

		SL::DrawTextNC(pos, size.x, size.y, sf::Color::Black, text);

		if (io.MouseClicked[0])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::string popup(std::string popuptext, std::string secondText, sf::RenderWindow* wnd) {
		ImGuiWindowFlags flg = ImGuiWindowFlags_();

		char* element = new char[1024];
		for (int i = 0; i < 1024; i++)
		{
			element[i] = '\0';
		}
		while (true)
		{
			SL::UpdateSLgine();
			ImGui::SFML::Update(*wnd, SL::gettime());
			ImGui::Begin(popuptext.c_str(), 0, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoMove);
			ImGui::Text(popuptext.c_str());
			ImGui::InputText(secondText.c_str(), element, 1024);
			if (ImGui::Button("ok")) {
				return std::string(element);
			}
			ImGui::End();

			ImGui::SFML::Render(*wnd);

			SL::DisplayWindow();
		}
	}

	int popupdiff(std::string popuptext, int prevDiff, sf::RenderWindow* wnd) {
		ImGuiWindowFlags flg = ImGuiWindowFlags_();

		int* element = new int();
		*element = prevDiff;

		while (true)
		{
			SL::UpdateSLgine();
			ImGui::SFML::Update(*wnd, SL::gettime());
			ImGui::Begin(popuptext.c_str(), 0, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoMove);
			ImGui::SetWindowSize({ 600,100 });
			ImGui::Text(popuptext.c_str());
			ImGui::SliderInt(GetDifficultyGUI(*element).c_str(), element, 0, 6);
			if (ImGui::Button("ok")) {
				return *element;
			}
			ImGui::End();

			ImGui::SFML::Render(*wnd);

			SL::DisplayWindow();
		}
	}
}
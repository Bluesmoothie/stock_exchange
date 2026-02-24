#include "utils/guiUtils.hpp"

namespace	guiUtils {

	void	obligatoryInputPopup(std::string& buff, std::string& message, const char popupName[], const std::string& inputName, std::function<std::string(const std::string&)> callbackFunc) {
		const std::string	inputLabel = "##" + inputName + "Input";

		if (ImGui::BeginPopupModal(popupName, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("%s:", inputName.c_str());
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere(0);
			ImGui::SameLine();
			ImGui::InputText(inputLabel.c_str(), &buff);

			ImGui::Spacing();

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 100.0f) / 2);
			if (ImGui::Button("Ok", ImVec2(100.0f, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				message = callbackFunc(buff);
				if (message == "OK") {
					message.clear();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!message.empty()) {
				ImVec2	text_size = ImGui::CalcTextSize(message.c_str());

				ImGui::PushStyleColor(ImGuiCol_Text, UI_COLOR_RED);
				ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - text_size.x) * 0.5f);
				ImGui::Text("%s", message.c_str());
				ImGui::PopStyleColor(1);

			} else
				ImGui::Text(" ");

			ImGui::EndPopup();
		}

	}

	void	inputPopup(std::string& buff, std::string& message, const char popupName[], const std::string& inputName, std::function<std::string(const std::string&)> callbackFunc) {
		const std::string	inputLabel = "##" + inputName + "Input";

		if (ImGui::BeginPopupModal(popupName, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("%s:", inputName.c_str());
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere(0);
			ImGui::SameLine();
			ImGui::InputText(inputLabel.c_str(), &buff);

			ImGui::Spacing();

			ImGui::SetCursorPosX(20.0f);
			if (ImGui::Button("Ok", ImVec2(100.0f, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				message = callbackFunc(buff);
				if (message == "OK") {
					message.clear();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f - 20.0f);
			if (ImGui::Button("Cancel", ImVec2(100.0f, 0)))
				ImGui::CloseCurrentPopup();

			if (!message.empty()) {
				ImVec2	text_size = ImGui::CalcTextSize(message.c_str());

				ImGui::PushStyleColor(ImGuiCol_Text, UI_COLOR_RED);
				ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - text_size.x) * 0.5f);
				ImGui::Text("%s", message.c_str());
				ImGui::PopStyleColor(1);

			} else
				ImGui::Text(" ");

			ImGui::EndPopup();
		}
	}
}
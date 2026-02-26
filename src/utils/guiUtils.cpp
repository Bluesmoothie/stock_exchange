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

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - UI_BUTTON_WIDTH) / 2);
			if (ImGui::Button("Ok", UI_BUTTON_SIZE) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
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

			ImGui::SetCursorPosX(UI_PADDING_WIDTH);
			if (ImGui::Button("Ok", UI_BUTTON_SIZE) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				message = callbackFunc(buff);
				if (message == "OK") {
					message.clear();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::SameLine(ImGui::GetWindowWidth() - UI_BUTTON_WIDTH - UI_PADDING_WIDTH);
			if (ImGui::Button("Cancel", UI_BUTTON_SIZE))
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

	void	selectableListPopup(const char popupName[], std::vector<std::string>& list, std::vector<std::string>::difference_type& selectedIndex, std::function<void(const std::vector<std::string>::difference_type)> setFunc, bool closeOnOk) {
		const std::string	boxName = std::string("##") + popupName + "Box";
		
		if (ImGui::BeginPopupModal(popupName, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::BeginListBox(boxName.c_str());
			std::vector<std::string>::iterator	it = list.begin();
			std::vector<std::string>::iterator	ite = list.end();
			for (; it != ite; ++it) {
				std::vector<std::string>::difference_type	distance = std::distance(list.begin(), it);
				if (ImGui::Selectable((*it).c_str(), distance == selectedIndex))
					selectedIndex = distance;
			}
			ImGui::EndListBox();

			ImGui::SetCursorPosX(UI_PADDING_WIDTH);
			if (ImGui::Button("Ok", UI_BUTTON_SIZE) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
				setFunc(selectedIndex);
				if (closeOnOk)
					ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine(ImGui::GetWindowWidth() - UI_BUTTON_WIDTH - UI_PADDING_WIDTH);
			if (ImGui::Button("Cancel", UI_BUTTON_SIZE))
				ImGui::CloseCurrentPopup();
	
			ImGui::EndPopup();
		}
	}
}
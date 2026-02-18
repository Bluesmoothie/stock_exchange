#include "classes/stockExchange.hpp"

void	stockExchange::draw(void) {
		ImGui::BeginMainMenuBar(); {
			if (ImGui::BeginMenu("Indices")) {
				ImGui::MenuItem("Add index", "Ctrl+A");
				ImGui::MenuItem("Show indices", "Ctrl+S");
				ImGui::MenuItem("Remove index", "Ctrl+R");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Select index")) {
				if (this->_indices.empty()) {
					ImGui::PushStyleColor(ImGuiCol_Text, color_medium_grey);
					ImGui::MenuItem("Empty...", nullptr, false, false);
					ImGui::PopStyleColor(1);
				}
				ImGui::EndMenu();
			}
		} ImGui::EndMainMenuBar();
}
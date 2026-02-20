#include "classes/stockExchange.hpp"

stockExchange::stockExchange(void) : _api(nullptr), _indices{}, _selectedIndex(_indices.end()), _apiKey(false), _addIndex(false), _showIndices(false), _removeIndex(false) {}

void	stockExchange::draw(void) {
	this->_apiKey = this->_api == nullptr;
	this->_addIndex = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A);
	this->_showIndices = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S);
	this->_removeIndex = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_R);

	ImGui::BeginMainMenuBar(); {
		if (ImGui::BeginMenu("Indices")) {
			this->_addIndex = ImGui::MenuItem("Add index", "Ctrl+A") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A);
			this->_showIndices = ImGui::MenuItem("Show indices", "Ctrl+S") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S);
			this->_removeIndex = ImGui::MenuItem("Remove index", "Ctrl+R") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_R);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Select index")) {
			if (this->_indices.empty()) {
				ImGui::PushStyleColor(ImGuiCol_Text, UI_COLOR_MEDIUM_GREY);
				ImGui::MenuItem("Empty...", nullptr, false, false);
				ImGui::PopStyleColor(1);
			} else {
				std::vector<std::string>::iterator	it = this->_indices.begin();
				std::vector<std::string>::iterator	ite = this->_indices.end();
				for (; it != ite; ++it) {
					if (ImGui::MenuItem((*it).c_str(), nullptr, it == this->_selectedIndex))
						this->_selectedIndex = it;
				}
			}
			ImGui::EndMenu();
		}
	} ImGui::EndMainMenuBar();

	this->drawPopups();
}

void		stockExchange::drawPopups(void) {
	this->apiKeyPopup();
	this->addIndexPopup();
	this->removeIndexPopup();
	this->showIndicesPopup();
}

void	stockExchange::apiKeyPopup(void) {
	static std::string	buff = {};
	static std::string	message = {};

	if (this->_apiKey) {
		ImGui::OpenPopup("Api key");
		this->_apiKey = false;
	}

	if (ImGui::BeginPopupModal("Api key", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Finnhub API key:");
		if (ImGui::IsWindowAppearing())
			ImGui::SetKeyboardFocusHere(0);
		ImGui::SameLine();
		ImGui::InputText("##apiKeyInput", &buff);

		ImGui::Spacing();

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 100.0f) / 2);
		if (ImGui::Button("Add", ImVec2(100.0f, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			message = this->registerApiKey(buff);
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

std::string	stockExchange::registerApiKey(const std::string& p_apiKey) {
	if (this->_api)
		delete this->_api;

	this->_api = new Rivendell::FinnHubAPI(p_apiKey);

	Json::Value*	res = this->_api->StockSymbolLookup("apple");
	std::string		ret;

	if (jsonUtils::isErrorResponse(res))
		ret = jsonUtils::getResponseError(res);
	else
		ret = "OK";

	delete res;
	return ret;
}

void	stockExchange::addIndexPopup(void) {
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_addIndex) {
		ImGui::OpenPopup("Add index");
		this->_addIndex = false;
		message.clear();
		buff.clear();
	}
	
	if (ImGui::BeginPopupModal("Add index", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Index:");
		if (ImGui::IsWindowAppearing())
			ImGui::SetKeyboardFocusHere(0);
		ImGui::SameLine();
		ImGui::InputText("##addIndexInput", &buff);

		ImGui::Spacing();

		ImGui::SetCursorPosX(20.0f);
		if (ImGui::Button("Add", ImVec2(100.0f, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			message = this->addIndex(buff);
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

std::string	stockExchange::addIndex(const std::string& p_index) {
	if (p_index.size() == 0)
		return "Empty text";

	Json::Value*	res = this->_api->StockQuote(p_index);

	if (jsonUtils::isErrorResponse(res)) {
		std::string	ret = jsonUtils::getResponseError(res);
		delete res;
		return ret;
	}

	std::vector<std::string>::iterator	ite = this->_indices.end();
	if (std::find(this->_indices.begin(), ite, p_index) != ite)
		return "Index aleady added";
	
	this->_indices.push_back(p_index);
	return "OK";
}

void	stockExchange::showIndicesPopup(void) {
	bool	dummy = true;

	if (this->_showIndices) {
		ImGui::OpenPopup("Indices list");
		this->_showIndices = false;
	}

	if (ImGui::BeginPopupModal("Indices list", &dummy, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::BeginListBox("##Indices list box");
		std::vector<std::string>::iterator	it = this->_indices.begin();
		std::vector<std::string>::iterator	ite = this->_indices.end();
		for (; it != ite; ++it) {
			if (ImGui::Selectable((*it).c_str(), it == this->_selectedIndex))
				this->_selectedIndex = it;
		}
		ImGui::EndListBox();

		ImGui::EndPopup();
	}
}

void	stockExchange::removeIndexPopup(void) {
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_removeIndex) {
		ImGui::OpenPopup("Remove index");
		this->_removeIndex = false;
		message.clear();
		buff.clear();
	}
	
	if (ImGui::BeginPopupModal("Remove index", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Index:");
		if (ImGui::IsWindowAppearing())
			ImGui::SetKeyboardFocusHere(0);
		ImGui::SameLine();
		ImGui::InputText("##removeIndexInput", &buff);

		ImGui::Spacing();

		ImGui::SetCursorPosX(20.0f);
		if (ImGui::Button("Remove", ImVec2(100.0f, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			message = this->removeIndex(buff);
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

std::string	stockExchange::removeIndex(const std::string& p_index) {
	if (p_index.size() == 0)
		return "Empty text";

	std::vector<std::string>::iterator	ite = this->_indices.end();
	std::vector<std::string>::iterator	it = std::find(this->_indices.begin(), ite, p_index);
	if (it == ite)
		return ("Index not found");
	
	if (it == this->_selectedIndex)
		this->_selectedIndex = this->_indices.end();
	this->_indices.erase(it);
	return ("OK");
}

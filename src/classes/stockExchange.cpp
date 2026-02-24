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
	const char			popupName[] = "Api key";
	static std::string	buff = {};
	static std::string	message = {};

	if (this->_apiKey) {
		ImGui::OpenPopup(popupName);
		this->_apiKey = false;
	}

	obligatoryInputPopup(buff, message, popupName, "Finnhub API key", [this](const std::string& p) -> std::string {return this->registerApiKey(p);});
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
	const char			popupName[] = "Add index";
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_addIndex) {
		ImGui::OpenPopup(popupName);
		this->_addIndex = false;
		message.clear();
		buff.clear();
	}
	
	inputPopup(buff, message, popupName, "Index", [this](const std::string& p) -> std::string {return this->addIndex(p);});
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
	const char			popupName[] = "Remove index";
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_removeIndex) {
		ImGui::OpenPopup(popupName);
		this->_removeIndex = false;
		message.clear();
		buff.clear();
	}
	
	inputPopup(buff, message, popupName, "Index", [this](const std::string& p) -> std::string {return this->removeIndex(p);});
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

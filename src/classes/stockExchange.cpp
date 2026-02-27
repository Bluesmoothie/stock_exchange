#include "classes/stockExchange.hpp"

stockExchange::stockExchange(void) {}

void	stockExchange::draw(void) {
	
	this->drawMenuBar();
	this->drawMainScreen();
	this->drawPopups();
}

void		stockExchange::drawMenuBar(void) {
	this->_popupApiKey = this->_api == nullptr;
	this->_popupSearchIndex = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A);
	this->_popupShowIndices = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S);
	this->_popupRemoveIndex = ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_R);

	ImGui::BeginMainMenuBar(); {
		if (ImGui::BeginMenu("Indices")) {
			this->_popupSearchIndex = ImGui::MenuItem("Search index", "Ctrl+A") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_A);
			this->_popupShowIndices = ImGui::MenuItem("Show indices", "Ctrl+S") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S);
			this->_popupRemoveIndex = ImGui::MenuItem("Remove index", "Ctrl+R") || ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_R);
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
					std::vector<std::string>::difference_type	index = std::distance(this->_indices.begin(), it);
					if (ImGui::MenuItem((*it).c_str(), nullptr, index == this->_selectedIndex))
						this->_selectedIndex = index;
				}
			}
			ImGui::EndMenu();
			this->_menuHeight = ImGui::GetWindowHeight();
		}
	} ImGui::EndMainMenuBar();
}

void		stockExchange::drawMainScreen(void) {
	if (this->_selectedIndex != -1) {

	static std::string	error = {};
	static Json::Value*	quotes = nullptr;

	if (!quotes) {
		quotes = this->_api->StockQuote(this->_indices[this->_selectedIndex]);
		error.clear();
		if (jsonUtils::isErrorResponse(quotes)) {
			error = jsonUtils::getResponseError(quotes);
		}
	}

	ImGui::Dummy(ImVec2(0, this->_menuHeight));
	if (error.size() > 0) {
		ImGui::Text("Error while fetching stock quote: %s", error.c_str());
	} else {
		ImGui::Text("Index: %s", this->_indices[this->_selectedIndex].c_str());
		ImGui::Spacing();
		ImGui::Text("        Current price: %f", (*quotes)["c"].asFloat());
		ImGui::Text("               Change: %f", (*quotes)["d"].asFloat());
		ImGui::Text("       Percent change: %f", (*quotes)["dp"].asFloat());
		ImGui::Text("High price of the day: %f", (*quotes)["h"].asFloat());
		ImGui::Text(" Low price of the day: %f", (*quotes)["l"].asFloat());
		ImGui::Text("Open price of the day: %f", (*quotes)["o"].asFloat());
		ImGui::Text(" Previous close price: %f", (*quotes)["pc"].asFloat());
	}
		
	}
}

void		stockExchange::drawPopups(void) {
	this->apiKeyPopup();
	this->searchIndexPopup();
	this->selectIndexPopup();
	this->removeIndexPopup();
	this->showIndicesPopup();
}

void	stockExchange::apiKeyPopup(void) {
	const char			popupName[] = "Api key";
	static std::string	buff = {};
	static std::string	message = {};

	if (this->_popupApiKey) {
		ImGui::OpenPopup(popupName);
		this->_popupApiKey = false;
	}

	guiUtils::obligatoryInputPopup(buff, message, popupName, "Finnhub API key", [this](const std::string& p) -> std::string {return this->registerApiKey(p);});
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

void	stockExchange::searchIndexPopup(void) {
	const char			popupName[] = "Add index";
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_popupSearchIndex) {
		ImGui::OpenPopup(popupName);
		this->_popupSearchIndex = false;
		message.clear();
		buff.clear();
	}
	
	guiUtils::inputPopup(buff, message, popupName, "Index", [this](const std::string& p) -> std::string {return this->searchIndex(p);});
}

std::string	stockExchange::searchIndex(const std::string& p_index) {
	if (p_index.size() == 0)
		return "Empty text";

	Json::Value*	res = this->_api->StockSymbolLookup(p_index);

	if (jsonUtils::isErrorResponse(res)) {
		std::string	ret = jsonUtils::getResponseError(res);
		delete res;
		return ret;
	}

	if (res->isMember("count") && (*res)["count"].asInt() > 0) {
		this->_popupSelectIndex = true;
		this->_lastResults = res;
		return "Selecting";
	}

	return "Unknow error";
}

void		stockExchange::parseIndexLookup(void) {
	if (this->_lastIndices)
			delete this->_lastIndices;
	this->_lastIndices = new std::vector<std::string>;

	for (int i = 0; i < (*this->_lastResults)["count"].asInt(); i++) {
		this->_lastIndices->push_back((*this->_lastResults)["result"][i]["symbol"].asString());
	}

}

void		stockExchange::selectIndexPopup(void) {
	const char											popupName[] = "Select index";
	static	std::vector<std::string>::difference_type	selectedIndex = 0;

	if (this->_popupSelectIndex) {
		ImGui::OpenPopup(popupName);
		this->_popupSelectIndex = false;
		selectedIndex = 0;
		this->parseIndexLookup();
	}

	guiUtils::selectableListPopup(popupName, (*this->_lastIndices), selectedIndex, [this](const std::vector<std::string>::difference_type& p) -> std::string {return this->addIndex(p);});
}

std::string	stockExchange::addIndex(const std::vector<std::string>::difference_type& p_index) {
	std::vector<std::string>::iterator	ite = this->_indices.end();
	if (std::find(this->_indices.begin(), ite, this->_lastIndices->at(p_index)) != ite)
		return "Index aleady added";
	
	this->_indices.push_back(this->_lastIndices->at(p_index));
	return "OK";
}

void	stockExchange::showIndicesPopup(void) {
	const char											popupName[] = "Indices list";
	static	std::vector<std::string>::difference_type	selectedIndex = this->_selectedIndex;

	if (this->_popupShowIndices) {
		ImGui::OpenPopup(popupName);
		this->_popupShowIndices = false;
	}

	guiUtils::selectableListPopup(popupName, this->_indices, selectedIndex, [this](const std::vector<std::string>::difference_type p) -> void {return this->setSelectedIndex(p);});
}

void	stockExchange::removeIndexPopup(void) {
	const char			popupName[] = "Remove index";
	static std::string	message = {};
	static std::string	buff = {};

	if (this->_popupRemoveIndex) {
		ImGui::OpenPopup(popupName);
		this->_popupRemoveIndex = false;
		message.clear();
		buff.clear();
	}
	
	guiUtils::inputPopup(buff, message, popupName, "Index", [this](const std::string& p) -> std::string {return this->removeIndex(p);});
}

std::string	stockExchange::removeIndex(const std::string& p_index) {
	if (p_index.size() == 0)
		return "Empty text";

	std::vector<std::string>::iterator	ite = this->_indices.end();
	std::vector<std::string>::iterator	it = std::find(this->_indices.begin(), ite, p_index);
	if (it == ite)
		return ("Index not found");
	
	if (std::distance(this->_indices.begin(), it) == this->_selectedIndex)
		this->_selectedIndex = -1;
	this->_indices.erase(it);
	return ("OK");
}

void		stockExchange::setSelectedIndex(const std::vector<std::string>::difference_type p_index) {
	if (p_index >= 0)
		this->_selectedIndex = p_index;
}

#include "classes/save.hpp"

save::save(void) {
	this->saveFile.open("settings.json", std::ifstream::binary);
	if (this->saveFile.is_open()) {
		this->loadSave();
	}
}

void	save::loadSave(void) {
	Json::Value	settings;

	this->saveFile >> settings;
}

void	save::saveSettings(void) {
	Json::Value	settings;

	settings["apiKey"].append(this->_api);
}
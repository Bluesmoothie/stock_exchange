#pragma once

#include "finnhub-api-cpp/FinnHubAPI.h"
#include <fstream>

class save
{
	private:

		std::ifstream	saveFile;

		void	loadSave(void);

	protected:

		Rivendell::FinnHubAPI*						_api = nullptr;

		save(void);

		void	saveSettings(void);
};

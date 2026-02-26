#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "video.hpp"
#include "finnhub-api-cpp/FinnHubAPI.h"
#include "utils/jsonUtils.hpp"
#include "ui.hpp"
#include "utils/guiUtils.hpp"

class stockExchange {
	private:

		Rivendell::FinnHubAPI*						_api;

		std::vector<std::string>					_indices;
		std::vector<std::string>::difference_type	_selectedIndex;

		bool	_apiKey;
		bool	_addIndex;
		bool	_selectIndex;
		bool	_showIndices;
		bool	_removeIndex;

		void		drawMenuBar(void);
		void		drawMainScreen(void);
		void		drawPopups(void);

		void		apiKeyPopup(void);
		std::string	registerApiKey(const std::string& p_apiKey);
		void		addIndexPopup(void);
		std::string	addIndex(const std::string& p_index);
		void		selectIndexPopup(void);
		void		showIndicesPopup(void);
		void		removeIndexPopup(void);
		std::string	removeIndex(const std::string& p_index);

		void		setSelectedIndex(const std::vector<std::string>::difference_type p_index);

	public:

		stockExchange(void);
		
		void	draw(void);
};
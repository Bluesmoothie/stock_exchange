#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "video.hpp"
#include "ui.hpp"
#include "utils/jsonUtils.hpp"
#include "utils/guiUtils.hpp"
#include "finnhub-api-cpp/FinnHubAPI.h"

class stockExchange {
	private:

		Rivendell::FinnHubAPI*						_api = nullptr;

		std::vector<std::string>					_indices{};
		std::vector<std::string>::difference_type	_selectedIndex = -1;
		std::vector<std::string>::difference_type	_oldSelectedIndex = -1;

		Json::Value*								_lastResults = nullptr;
		std::vector<std::string>*					_lastIndices = nullptr;

		float	_menuHeight = 0.0f;


		bool	_popupApiKey = false;
		bool	_popupSearchIndex = false;
		bool	_popupSelectIndex = false;
		bool	_popupShowIndices = false;
		bool	_popupRemoveIndex = false;

		void		drawMenuBar(void);
		void		drawMainScreen(void);
		void		drawPopups(void);

		void			apiKeyPopup(void);
		std::string		registerApiKey(const std::string& p_apiKey);
		void			searchIndexPopup(void);
		std::string		searchIndex(const std::string& p_index);
		void			parseIndexLookup(void);
		void			selectIndexPopup(void);
		std::string		addIndex(const std::vector<std::string>::difference_type& p_index);
		void			showIndicesPopup(void);
		void			removeIndexPopup(void);
		std::string		removeIndex(const std::string& p_index);

		void		setSelectedIndex(const std::vector<std::string>::difference_type p_index);

	public:

		stockExchange(void);
		
		void	draw(void);
};
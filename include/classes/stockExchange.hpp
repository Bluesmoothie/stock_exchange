#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "video.hpp"

class stockExchange {
	private:

		std::vector<std::string>			_indices;
		std::vector<std::string>::iterator	_selectedIndex;

		bool	_addIndex;
		bool	_showIndices;
		bool	_removeIndex;

		void		drawPopups(void);

		void		addIndexPopup(void);
		std::string	addIndex(const std::string& p_index);
		void		showIndicesPopup(void);
		void		removeIndexPopup(void);
		std::string	removeIndex(const std::string& p_index);

	public:

		stockExchange(void);
		
		void	draw(void);
};
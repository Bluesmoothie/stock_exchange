#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
# include <SDL2/SDL_opengles2.h>
#else
# include <SDL2/SDL_opengl.h>
#endif

#include <functional>
#include "ui.hpp"

namespace	guiUtils {
	void	obligatoryInputPopup(std::string& buff, std::string& message, const char popupName[], const std::string& inputName, std::function<std::string(const std::string&)> callbackFunc);
	void	inputPopup(std::string& buff, std::string& message, const char popupName[], const std::string& inputName, std::function<std::string(const std::string&)> callbackFunc);
	void	selectableListPopup(const char popupName[], std::vector<std::string>& list, std::vector<std::string>::difference_type& selectedIndex, std::function<void(const std::vector<std::string>::difference_type)> setFunc, bool closeOnOk = 1);
}
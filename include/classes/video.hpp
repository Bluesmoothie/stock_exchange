#pragma once

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
# include <SDL2/SDL_opengles2.h>
#else
# include <SDL2/SDL_opengl.h>
#endif
#include <string>
#include <functional>

#define TTT_WINDOW_TITLE	"Stock exchange"
#define TTT_WINDOW_WIDTH	1720
#define TTT_WINDOW_HEIGH	880

const ImVec4	clear_color = ImVec4(0.67f, 0.83f, 0.02f, 1.00f);

const ImVec4 	color_red = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
const ImVec4 	color_blue = ImVec4(0.2f, 0.6f, 0.9f, 1.0f);
const ImVec4 	color_white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
const ImVec4 	color_light_grey = ImVec4(0.85f, 0.85f, 0.90f, 1.00f);
const ImVec4	color_medium_grey = ImVec4(0.65f, 0.65f, 0.70f, 1.00f);

class	Video {

private:

	std::function<void()>	_draw_func;

	SDL_Window* 	_window;
	ImGuiIO 		_io;
	SDL_GLContext 	_gl_context;
	ImGuiViewport*	_viewport;

	bool			_initialized;
	bool			_done;

public:

	Video(std::function<void()> p_draw_func);
	~Video(void);

	int		init(void);
	void	deinit(void);

	void	loop(void);
	void	treatEvents(void);
	void	draw(void);
	void	render(void);

};
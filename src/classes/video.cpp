#include "classes/video.hpp"

Video::Video(void) : initialized(false) {}

Video::~Video(void) {

	if (this->initialized)
		this->deinit();
}

int	Video::init(void) {
	// Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    this->window = SDL_CreateWindow(TTT_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)(TTT_WINDOW_WIDTH * main_scale), (int)(TTT_WINDOW_HEIGH * main_scale), window_flags);
    if (this->window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }

    this->gl_context = SDL_GL_CreateContext(this->window);
    if (this->gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_MakeCurrent(this->window, this->gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->io = ImGui::GetIO();
    this->io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    this->io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(this->window, this->gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

	// io.Fonts->AddFontFromFileTTF("fonts/NotoSans-Black.ttf", 36.0f);
	// ImFontConfig config;
	// config.MergeMode = true;
    // io.Fonts->AddFontFromFileTTF("fonts/NotoEmoji-Regular.ttf", 26.0f, &config);

	this->viewport = ImGui::GetMainViewport();

	return 0;
}

void	Video::deinit(void) {
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
	this->initialized = false;
}

void	Video::loop(void) {
	this->done = false;

	while (!this->done) {
		this->treatEvents();

		if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
            continue;
        }

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

		this->draw();

		this->render();
	}
}

void	Video::treatEvents(void) {
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            this->done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(this->window))
            this->done = true;
    }
}

void	Video::render(void) {
	ImGui::Render();
    glViewport(0, 0, (int)this->io.DisplaySize.x, (int)this->io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(this->window);
}

void	Video::draw(void) {

	ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(viewport->WorkSize, ImGuiCond_FirstUseEver);

	if (ImGui::Begin("main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
		
	}
	ImGui::End();
}

#include <string>
#include <string_view>

#include <GL/gl3w.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

#include <imgui/imgui_impl_sdl2.h>
#include "EditorApplication.hpp"

using glm::ivec2;
using cute::shell::WindowHandle;

static constexpr ivec2 DEFAULT_WINDOW_SIZE = {1280, 720};
static const char DEFAULT_WINDOW_TITLE[] = "yaed";

static void showError(const std::string &message) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, DEFAULT_WINDOW_TITLE, message.c_str(), nullptr);
}

static void showSdlError(std::string_view context) {
	showError(fmt::format("{}: {}", context, SDL_GetError()));
}

static bool pollEvents(SDL_Window *window) {
	SDL_Event event;

	bool shouldClose = false;

	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type) {
			case SDL_QUIT:
				shouldClose = true;
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
					shouldClose = true;

			default:
				break;
		}
	}

	return shouldClose;
}

static WindowHandle extractRawHandleFromSdlWindow(SDL_Window *window) {
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;

	SDL_VERSION(&wmInfo.version);

	if (!SDL_GetWindowWMInfo(window, &wmInfo))
		return {};

	return reinterpret_cast<window_handle_t>(wmInfo.info.win.window);
#else
	return {};
#endif
}

int main(int, char **) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		showSdlError("Failed to initialize SDL");
		return 1;
	}

	// enable native ime
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window *window = SDL_CreateWindow(
			DEFAULT_WINDOW_TITLE,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
	);

	if (!window) {
		showSdlError("Failed to create SDL window");
		SDL_Quit();
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (!glContext) {
		showSdlError("Failed to create OpenGL context");

		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	SDL_GL_MakeCurrent(window, glContext);

	// try to enable adaptive vsync, use normal vsync if not available
	if (SDL_GL_SetSwapInterval(-1) == -1)
		SDL_GL_SetSwapInterval(1);

	if (gl3wInit(reinterpret_cast<GL3WGetProcAddressProc>(SDL_GL_GetProcAddress)) != GL3W_OK) {
		showError("Failed to load OpenGL extensions");

		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	ImGui::CreateContext();

	ImGuiIO &imGuiIo = ImGui::GetIO();

	imGuiIo.IniFilename = nullptr;

	imGuiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	imGuiIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#ifdef _WIN32
	imGuiIo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif

	ImGui::StyleColorsLight();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	{
		EditorApplication editor(extractRawHandleFromSdlWindow(window));

		bool windowShown = false;

		while (true) {
			bool shouldClose = pollEvents(window);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();

			ImGui::NewFrame();

			if (!editor.update(shouldClose))
				break;

			// prepare draw data
			ImGui::Render();

			{
				const auto &size = imGuiIo.DisplaySize;
				glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
			}

			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			// actual rendering
			editor.renderOffscreenViews();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (imGuiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				SDL_Window *lastWindow = SDL_GL_GetCurrentWindow();
				SDL_GLContext lastContext = SDL_GL_GetCurrentContext();

				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();

				SDL_GL_MakeCurrent(lastWindow, lastContext);
			}

			SDL_GL_SwapWindow(window);

			// show window after first frame presentation to avoid flicker
			if (!windowShown) {
				SDL_ShowWindow(window);
				windowShown = true;
			}
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
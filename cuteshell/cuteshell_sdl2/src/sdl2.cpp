#include "cute/shell/sdl2.hpp"

#include <sstream>
#include <string>

#include <SDL_syswm.h>

using namespace cute::shell;

template<class T>
static std::string toHex(T value) {
	std::stringstream ss;
	ss << std::hex << value;
	return ss.str();
}

WindowHandle sdl2::getWindowHandle(SDL_Window *window) {
	SDL_SysWMinfo wmInfo;

	SDL_VERSION(&wmInfo.version);

	if (!SDL_GetWindowWMInfo(window, &wmInfo))
		return {};

	switch (wmInfo.subsystem) {
#ifdef SDL_VIDEO_DRIVER_WINDOWS
		case SDL_SYSWM_WINDOWS:
			return reinterpret_cast<WindowHandle>(wmInfo.info.win.window);
#endif
#ifdef SDL_VIDEO_DRIVER_X11
		case SDL_SYSWM_X11:
			return "x11:" + toHex(wmInfo.info.x11.window);
#endif
		default:
			break;
	}

	return {};
}
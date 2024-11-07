//
// Created by liamb on 7/11/2024.
//

#pragma once

#include <iostream>
#include <ostream>
#include <windows.h>
#include "IApplication/IApplication.h"
#include "Launch/EngineLoop/EngineLoop.h"

extern LunarisEngine::IApplication* EntryApplication();

inline void launch_windows() {
	const auto entry_application = EntryApplication();

	std::cout << "Launching Windows" << std::endl;
}

inline void shutdown_windows() {

}

inline int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	launch_windows();
	shutdown_windows();
	return 0;
}
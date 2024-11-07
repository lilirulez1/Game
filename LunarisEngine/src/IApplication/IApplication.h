//
// Created by liamb on 7/11/2024.
//

#pragma once

#include "Engine/Definitions/Definitions.h"

#define EntryApp(x) LunarisEngine::IApplication* EntryApplication() { return new x; }

namespace LunarisEngine {

	class LUNARIS_ENGINE IApplication {
	public:
		virtual ~IApplication() = default;
	};

	IApplication* EntryApplication();

} // LunarisEngine

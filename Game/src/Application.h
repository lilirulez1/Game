//
// Created by liamb on 7/11/2024.
//

#pragma once

#include "IApplication/IApplication.h"
#include "Launch/Windows/LaunchWindows.h"

class Application final : public LunarisEngine::IApplication {
};

EntryApp(Application);
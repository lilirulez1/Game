add_library(LunarisEngine SHARED
        src/Launch/Windows/LaunchWindows.h
        src/IApplication/IApplication.cpp
        src/IApplication/IApplication.h
        src/Engine/Definitions/Definitions.cpp
        src/Engine/Definitions/Definitions.h
        src/Launch/EngineLoop/EngineLoop.cpp
        src/Launch/EngineLoop/EngineLoop.h
)

target_compile_definitions(LunarisEngine PRIVATE BUILD_DLL)
target_include_directories(LunarisEngine PUBLIC src)
cmake_minimum_required(VERSION 3.30)
project(Engine)

set(CMAKE_CXX_STANDARD 26)

include_directories(../Engine)
include_directories(Core/pch)

add_library(Engine SHARED
        Core/pch/pch.cpp
        Core/pch/pch.h
        EngineLoop/EngineLoop.cpp
        EngineLoop/EngineLoop.h
        Util/Util.h
        Core/EngineExport.h
        Core/File/Filesystem/Filesystem.cpp
        Core/File/Filesystem/Filesystem.h
        Core/Memory/StackAllocator/StackAllocator.cpp
        Core/Memory/StackAllocator/StackAllocator.h
        Core/Memory/MemoryUtil.h
        Core/Memory/MemoryManager/MemoryManager.cpp
        Core/Memory/MemoryManager/MemoryManager.h
        Core/Time/StepTimer.h
        Services/Window/WindowService.cpp
        Services/Window/WindowService.h
        Services/Window/Window.cpp
        Services/Window/Window.h
        Services/Service.h
        Core/Memory/PoolAllocator/PoolAllocator.cpp
        Core/Memory/PoolAllocator/PoolAllocator.h
        Application/Application.cpp
        Application/Application.h
        Services/Graphics/GraphicsService.cpp
        Services/Graphics/GraphicsService.h
        Services/Graphics/Renderers/AbstractRenderer2D/AbstractRenderer2D.cpp
        Services/Graphics/Renderers/AbstractRenderer2D/AbstractRenderer2D.h
        Services/Graphics/Renderers/Direct2DRenderer/Direct2DRenderer.cpp
        Services/Graphics/Renderers/Direct2DRenderer/Direct2DRenderer.h
)

target_precompile_headers(Engine PRIVATE Core/pch/pch.h)
target_compile_definitions(Engine PRIVATE BUILD_DLL)
target_link_libraries(Engine PRIVATE dwmapi.lib)
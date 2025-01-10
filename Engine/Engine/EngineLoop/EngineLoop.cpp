//
// Created by liamb on 6/01/2025.
//

#include "pch.h"
#include "EngineLoop.h"
#include "Core/Memory/MemoryManager/MemoryManager.h"
#include "Services/Window/WindowService.h"
#include "Services/Graphics/GraphicsService.h"
#include "Application/Application.h"

Engine::EngineLoop::EngineLoop() {
	m_memoryManager = new Memory::MemoryManager();
	m_windowService = Memory::MemoryManager::NewOnStatic<Services::WindowService>();
	m_graphicsService = Memory::MemoryManager::NewOnStatic<Services::GraphicsService>();

	m_fixedTimer = StepTimer();
	m_fixedTimer.SetFixedTimeStep(true);
	m_fixedTimer.SetTargetElapsedSeconds(1.0f / 60.0f);

	m_variableTimer = StepTimer();
}

Engine::EngineLoop::~EngineLoop() {
	m_windowService->~WindowService();
	m_graphicsService->~GraphicsService();
	delete m_memoryManager;
}

bool Engine::EngineLoop::IsRunning() const {
	return m_isRunning;
}

void Engine::EngineLoop::Run(Application* application) {
	m_application = application;
	Startup();
	while (m_isRunning) {
		Update();
	}
	Shutdown();
}

void Engine::EngineLoop::Exit() {
	m_isRunning = false;
}

void Engine::EngineLoop::Startup() {
	m_isRunning = true;

	m_windowService->Startup();
	m_graphicsService->Startup();
	m_application->Startup();
}

void Engine::EngineLoop::Update() {
	m_fixedTimer.Tick([]() {
		Instance().FixedUpdate();
	});

	m_variableTimer.Tick();
	VariableUpdate();
}

void Engine::EngineLoop::FixedUpdate() const {
	m_application->FixedUpdate();
}

void Engine::EngineLoop::VariableUpdate() const {
	m_graphicsService->Update();
	m_windowService->Update();
	m_application->VariableUpdate();
}

void Engine::EngineLoop::Shutdown() const {
	m_graphicsService->Shutdown();
	m_windowService->Shutdown();
	m_application->Shutdown();
}

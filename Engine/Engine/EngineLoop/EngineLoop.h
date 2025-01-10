//
// Created by liamb on 6/01/2025.
//

#pragma once

#include "Core/EngineExport.h"
#include "Util/Util.h"
#include "Core/Time/StepTimer.h"

namespace Engine {
	class Application;
}

namespace Engine {
	namespace Memory {
		class MemoryManager;
	}

	namespace Services {
		class WindowService;
		class GraphicsService;
	}

	class API EngineLoop final : public Singleton<EngineLoop> {
	public:
		[[nodiscard]] bool IsRunning() const;

		void Run(Application* application);
		void Exit();

	private:
		friend class Singleton;
		EngineLoop();
		~EngineLoop() override;

		bool m_isRunning = false;

		Application* m_application = nullptr;

		Memory::MemoryManager* m_memoryManager = nullptr;
		Services::WindowService* m_windowService = nullptr;
		Services::GraphicsService* m_graphicsService = nullptr;

		StepTimer m_fixedTimer;
		StepTimer m_variableTimer;

		void Startup();
		void Update();
		void FixedUpdate() const;
		void VariableUpdate() const;
		void Shutdown() const;
	};

}

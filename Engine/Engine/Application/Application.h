//
// Created by liamb on 9/01/2025.
//

#pragma once

namespace Engine {

	class Application {
	public:
		virtual ~Application() = default;

		friend class EngineLoop;
	protected:
		virtual void Startup() = 0;
		virtual void FixedUpdate() = 0;
		virtual void VariableUpdate() = 0;
		virtual void Shutdown() = 0;
	};

} // Engine

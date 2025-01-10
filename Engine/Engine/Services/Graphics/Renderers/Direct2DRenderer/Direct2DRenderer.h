//
// Created by liamb on 9/01/2025.
//

#pragma once

#include "Core/EngineExport.h"
#include "Services/Graphics/Renderers/AbstractRenderer2D/AbstractRenderer2D.h"

namespace Engine {

	namespace Memory {
		template <typename T>
		class PoolAllocator;
	}

	class API Direct2DRenderer final : public AbstractRenderer2D {
	public:
		void DoSomething() override;

		template <typename T>
		friend class Memory::PoolAllocator;
	private:
		Direct2DRenderer() = default;
		~Direct2DRenderer() override = default;
	};

} // Engine

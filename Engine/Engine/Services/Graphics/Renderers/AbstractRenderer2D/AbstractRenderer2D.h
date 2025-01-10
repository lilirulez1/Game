//
// Created by liamb on 9/01/2025.
//

#pragma once
#include "Util/Util.h"

namespace Engine {

	namespace Services {
		class GraphicsService;
	}

	class AbstractRenderer2D {
	public:
		virtual ~AbstractRenderer2D() = default;

		friend class Services::GraphicsService;
		template <typename T>
		friend struct Renderer2DProxy;
	private:
		u16 m_magicNumber;

		virtual void DoSomething() = 0;
	};

} // Engine

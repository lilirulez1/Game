//
// Created by liamb on 9/01/2025.
//

#pragma once
#include "Core/Memory/PoolAllocator/PoolAllocator.h"
#include "Renderers/AbstractRenderer2D/AbstractRenderer2D.h"
#include "Services/Service.h"
#include "Util/Util.h"

#define RENDERER2D_COUNT 4

namespace Engine {
	template <typename T>
	struct Renderer2DProxy;
}

namespace Engine::Services {

	class API GraphicsService final : public Singleton<GraphicsService>, public Service {
	public:
		GraphicsService();
		~GraphicsService() override = default;

		void Startup() override;
		void Update() override;
		void Shutdown() override;

		template <typename T>
		Renderer2DProxy<T> CreateRenderer2D();

		template <typename T>
		friend struct Renderer2DProxy;

	private:
		friend class Singleton;

		Memory::PoolAllocator<AbstractRenderer2D> m_renderer2DAllocator;

		template <typename T>
		T* GetRenderer2D(const Renderer2DProxy<T>& renderer2DProxy);
	};

	template <typename T>
	Renderer2DProxy<T> GraphicsService::CreateRenderer2D() {
		static int MAGIC_NUMBER_COUNTER = 1;

		int index;
		T* renderer2D = static_cast<T*>(m_renderer2DAllocator.Get(&index));
		renderer2D->m_magicNumber = MAGIC_NUMBER_COUNTER++;

		Renderer2DProxy<T> renderer2DProxy;
		renderer2DProxy.handle = static_cast<u32>(renderer2D->m_magicNumber) << 16 | index;
		return renderer2DProxy;
	}

	template <typename T>
	T* GraphicsService::GetRenderer2D(const Renderer2DProxy<T>& renderer2DProxy) {
		const u16 index = GetIndexFromHandle(renderer2DProxy);
		const u16 magicNumber = GetMagicFromHandle(renderer2DProxy);
		auto* renderer2D = m_renderer2DAllocator.GetByIndex(index);
		if (renderer2D->m_magicNumber != magicNumber) {
			throw std::runtime_error("Window does not have the correct magic number!");
		}
		return static_cast<T*>(renderer2D);
	}

}

namespace Engine {

	template <typename T>
	struct API Renderer2DProxy final : Proxy {
		void DoSomething() {
			Services::GraphicsService::Instance().GetRenderer2D<T>(*this)->DoSomething();
		}
	};

}
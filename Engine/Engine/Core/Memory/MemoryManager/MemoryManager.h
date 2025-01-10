//
// Created by liamb on 7/01/2025.
//

#pragma once
#include "Util/Util.h"
#include "Core/Memory/StackAllocator/StackAllocator.h"

namespace Engine::Memory {

	class MemoryManager final : public Singleton<MemoryManager> {
	public:
		friend class Singleton;
		MemoryManager()
		: m_staticMemory(10_MB) {};
		~MemoryManager() override = default;

		static void* AllocateOnStatic(size size, u8 alignment);

		template<typename T, typename ... Args>
		static T* NewOnStatic(Args&&...);

		template<typename T>
		static T* NewArrayOnStatic(size capacity, u8 alignment);

	private:
		StackAllocator m_staticMemory;
	};

	template <typename T, typename ... Args>
	T* MemoryManager::NewOnStatic(Args&&... args) {
		return Instance().m_staticMemory.New<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	T* MemoryManager::NewArrayOnStatic(const size capacity, const u8 alignment) {
		return Instance().m_staticMemory.NewArray<T>(capacity, alignment);
	}
}

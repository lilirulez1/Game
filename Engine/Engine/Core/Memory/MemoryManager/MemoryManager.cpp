//
// Created by liamb on 7/01/2025.
//

#include "pch.h"
#include "MemoryManager.h"

namespace Engine::Memory {
	void* MemoryManager::AllocateOnStatic(const size size, const u8 alignment) {
		return Instance().m_staticMemory.Allocate(size, alignment);
	}
}

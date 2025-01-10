//
// Created by liamb on 9/01/2025.
//

#include "pch.h"
#include "GraphicsService.h"

#include "Core/Memory/MemoryManager/MemoryManager.h"
#include "Renderers/AbstractRenderer2D/AbstractRenderer2D.h"

namespace Engine::Services {

	GraphicsService::GraphicsService()
	: m_renderer2DAllocator(Memory::MemoryManager::AllocateOnStatic(sizeof(AbstractRenderer2D) * RENDERER2D_COUNT, alignof(AbstractRenderer2D)), RENDERER2D_COUNT) {
		m_instance = this;
	}

	void GraphicsService::Startup() {

	}

	void GraphicsService::Update() {

	}

	void GraphicsService::Shutdown() {

	}

}

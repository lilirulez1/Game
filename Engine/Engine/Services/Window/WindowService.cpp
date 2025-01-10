//
// Created by liamb on 8/01/2025.
//

#include "pch.h"
#include "WindowService.h"

#include <assert.h>

#include "Window.h"
#include "Core/Memory/MemoryManager/MemoryManager.h"
#include "EngineLoop/EngineLoop.h"

namespace Engine {

	void WindowProxy::Update() {
		if (!IsValid()) return;
		auto* window = Services::WindowService::Instance().GetWindow(*this);
		if (!window->ShouldUpdate()) return;
		window->Update();
		if (window->ShouldClose()) {
			Destroy();
		}
	}

	void WindowProxy::Show() const {
		if (!IsValid()) return;
		Services::WindowService::Instance().GetWindow(*this)->Show();
	}

	void WindowProxy::Hide() const {
		if (!IsValid()) return;
		Services::WindowService::Instance().GetWindow(*this)->Hide();
	}

	void WindowProxy::Resize(const int width, const int height) const {
		if (!IsValid()) return;
		Services::WindowService::Instance().GetWindow(*this)->Resize(width, height);
	}

	void WindowProxy::SetTitle(const std::string& title) const {
		if (!IsValid()) return;
		Services::WindowService::Instance().GetWindow(*this)->SetTitle(title);
	}

	void WindowProxy::SetIcon(const HICON icon) const {
		if (!IsValid()) return;
		Services::WindowService::Instance().GetWindow(*this)->SetIcon(icon);
	}

	LONG WindowProxy::GetStyle() const {
		if (!IsValid()) throw std::runtime_error("WindowProxy::GetStyle() called on an invalid handle");
		return Services::WindowService::Instance().GetWindow(*this)->GetStyle();
	}

	void WindowProxy::SetStyle(const LONG style) const {
		if (!IsValid()) return;
		return Services::WindowService::Instance().GetWindow(*this)->SetStyle(style);
	}

	bool WindowProxy::HasClosed() const {
		return !IsValid();
	}

	void WindowProxy::Destroy() {
		if (!IsValid()) return;

		Services::WindowService::Instance().DestroyWindow(*this);

		handle = 0;
	}

}

namespace Engine::Services {

	WindowService::WindowService()
	: m_windowAllocator(Memory::MemoryManager::AllocateOnStatic(sizeof(Window) * MAX_WINDOWS, alignof(Window)), MAX_WINDOWS) {
		m_instance = this;

		if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {}
	}

	void WindowService::Startup() {
	}

	void WindowService::Update() {
	}

	void WindowService::Shutdown() {
	}

	WindowProxy WindowService::MakeWindow() {
		static int MAGIC_NUMBER_COUNTER = 1;

		int index;
		auto* window = m_windowAllocator.Get(&index);
		window->m_magicNumber = MAGIC_NUMBER_COUNTER++;

		WindowProxy windowProxy;
		windowProxy.handle = static_cast<u32>(window->m_magicNumber) << 16 | index;
		return windowProxy;
	}

	Window* WindowService::GetWindow(const WindowProxy& windowProxy) {
		const u16 index = GetIndexFromHandle(windowProxy);
		const u16 magicNumber = GetMagicFromHandle(windowProxy);
		auto* window = m_windowAllocator.GetByIndex(index);
		if (window->m_magicNumber != magicNumber) {
			throw std::runtime_error("Window does not have the correct magic number!");
		}
		return window;
	}

	void WindowService::DestroyWindow(const WindowProxy& windowProxy) {
		auto* window = GetWindow(windowProxy);
		window->~Window();
		m_windowAllocator.Free(window);
	}

}

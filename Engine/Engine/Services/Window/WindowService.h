//
// Created by liamb on 8/01/2025.
//

#pragma once

#include "Core/Memory/PoolAllocator/PoolAllocator.h"
#include "Util/Util.h"
#include "Services/Service.h"

#define MAX_WINDOWS 4

namespace Engine {
	class Window;

	// Make it easier to add multi-platform support
	struct API WindowProxy final : Proxy {
		void Update();

		void Show() const;
		void Hide() const;

		void Resize(int width, int height) const;
		void SetTitle(const std::string& title) const;
		void SetIcon(HICON icon) const;

		LONG GetStyle() const;
		void SetStyle(LONG style) const;

		bool HasClosed() const;
		void Destroy();
	};

}

namespace Engine::Services {

	class API WindowService final : public Singleton<WindowService>, public Service {
	public:
		WindowService();
		~WindowService() override = default;

		void Startup() override;
		void Update() override;
		void Shutdown() override;

		WindowProxy MakeWindow();

		friend struct WindowProxy;

	private:
		friend class Singleton;

		Memory::PoolAllocator<Window> m_windowAllocator;

		Window* GetWindow(const WindowProxy& windowProxy);
		void DestroyWindow(const WindowProxy& windowProxy);
	};
}

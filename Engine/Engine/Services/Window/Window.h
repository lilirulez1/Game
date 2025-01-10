//
// Created by liamb on 8/01/2025.
//

#pragma once
#include "Util/Util.h"

namespace Engine {

	struct WindowProxy;

	namespace Memory {
		template <typename T>
		class PoolAllocator;
	}

	namespace Services {
		class WindowService;
	}

	class Window {
	public:
		LRESULT ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		template <typename T>
		friend class Memory::PoolAllocator;
		friend class Services::WindowService;
		friend struct WindowProxy;

	private:
		Window();
		~Window();

		u16 m_magicNumber;

		int m_width;
		int m_height;
		std::string m_title;
		bool m_shouldClose;

		HWND m_hWnd = nullptr;
		MSG m_msg{};

		void Show() const { ShowWindow(m_hWnd, SW_SHOW); }
		void Hide() const { CloseWindow(m_hWnd); }
		void Resize(const int width, const int height) { m_width = width, m_height = height; SetWindowPos(m_hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER); }
		void SetTitle(const std::string& title) { m_title = title; SetWindowText(m_hWnd, title.c_str()); }
		void SetIcon(HICON icon) const { SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon)); SendMessage(m_hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon)); }
		LONG GetStyle() const { return GetWindowLong(m_hWnd, GWL_STYLE); }
		void SetStyle(const LONG style) const { SetWindowLong(m_hWnd, GWL_STYLE, style); }
		bool ShouldClose() const { return m_shouldClose; }

		bool ShouldUpdate();
		void Update() const;
	};

}

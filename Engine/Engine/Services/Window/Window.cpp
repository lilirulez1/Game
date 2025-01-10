//
// Created by liamb on 8/01/2025.
//

#include "pch.h"
#include "Window.h"

#include "WindowService.h"
#include <dwmapi.h>

namespace Engine {

	Window::Window()
	: m_width(800), m_height(600), m_title("Window"), m_shouldClose(false) {
		const HINSTANCE hInstance = GetModuleHandle(nullptr);

		WNDCLASSEX windowClass = { };
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = hInstance;
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.lpszClassName = m_title.c_str();
		RegisterClassEx(&windowClass);

		RECT windowRect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		m_hWnd = CreateWindow(
			windowClass.lpszClassName,
			m_title.c_str(),
			WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,
			nullptr,
			hInstance,
			this
		);
	}

	Window::~Window() {
		if (m_hWnd == nullptr) return;

		DestroyWindow(m_hWnd);
	}

	bool Window::ShouldUpdate() {
		return PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE);
	}

	void Window::Update() const {
		if (m_msg.message == WM_QUIT) {
			return;
		}

		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}

	bool IsSystemDarkModeEnabled() {
		HKEY hKey;
		if (LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey); lResult == ERROR_SUCCESS) {
			DWORD dwValue = 0;
			DWORD dwSize = sizeof(dwValue);
			lResult = RegQueryValueEx(hKey, "AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&dwValue), &dwSize);
			RegCloseKey(hKey);

			if (lResult == ERROR_SUCCESS) {
				return dwValue == 0;
			}
		}
		return false;
	}

	LRESULT Window::ProcessMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam) {
		switch (msg) {
			case WM_CREATE: {
				const BOOL darkMode = IsSystemDarkModeEnabled();
				DwmSetWindowAttribute(m_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));

				return 0;
			}
			case WM_SETTINGCHANGE: {
				if (const auto path = reinterpret_cast<char const*>(lParam); strcmp(path, "ImmersiveColorSet") == 0) {
					const BOOL darkMode = IsSystemDarkModeEnabled();
					DwmSetWindowAttribute(m_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
				}

				return 0;
			}
			case WM_PAINT: {
				return 0;
			}
			case WM_SIZE: {
				RECT clientRect = {};
				GetClientRect(m_hWnd, &clientRect);

				m_width = clientRect.right - clientRect.left;
				m_height = clientRect.bottom - clientRect.top;

				return 0;
			}
			case WM_DESTROY:
				m_shouldClose = true;
				PostQuitMessage(0);
				return 0;
			default:
				return DefWindowProc(m_hWnd, msg, wParam, lParam);
		}
	}

	LRESULT Window::WindowProc(HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
		Window* self = nullptr;

		if (uMsg == WM_NCCREATE) {
			const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			self = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
			self->m_hWnd = hWnd;
		} else {
			self = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (self) {
			return self->ProcessMessage(uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

}

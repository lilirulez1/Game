//
// Created by liamb on 6/01/2025.
//

#pragma once

#include <functional>

namespace Engine {

	using byte = uint8_t;
	using size = size_t;

	using  u8 =  uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	constexpr  u8  u8_max = 0xFF;
	constexpr u16 u16_max = 0xFFFF;
	constexpr u32 u32_max = 0xFFFFFFFF;
	constexpr u64 u64_max = 0xFFFFFFFFFFFFFFFF;

	using  i8 =  int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using PtrDiff = std::ptrdiff_t;
	using PtrInt = uintptr_t;

	template <typename... T>
	using Action = std::function<void(T...)>;

	template <typename result, typename... T>
	using Func = std::function<result(T...)>;

	template <typename T>
	class Singleton {
	public:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		static T& Instance() {
			if (m_instance == nullptr) {
				m_instance = new T();
			}
			return *m_instance;
		}

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

		inline static T* m_instance = nullptr;
	};

	struct Proxy {
		Proxy() : handle(0) {}
		u32 handle;

		bool IsValid() const { return handle != 0; }
	};

	static u16 GetIndexFromHandle(const Proxy& proxy) {
		constexpr u32 standardIndexMask = (1 << 16) - 1;
		return proxy.handle & standardIndexMask;
	}

	static u16 GetMagicFromHandle(const Proxy& proxy) {
		constexpr u32 standardIndexMask = (1 << 16) - 1;
		constexpr u32 standardMagicNumberMask = ~standardIndexMask;
		return (proxy.handle & standardMagicNumberMask) >> 16;
	}

}

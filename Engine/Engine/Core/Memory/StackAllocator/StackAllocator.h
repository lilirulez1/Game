//
// Created by liamb on 7/01/2025.
//

#pragma once

#include "Core/Memory/MemoryUtil.h"
#include "Util/Util.h"

namespace Engine::Memory {

	class StackAllocator {
	public:
		using Marker = size;

		StackAllocator() = delete;
		explicit StackAllocator(size stackSize);
		~StackAllocator();

		void* Allocate(size size, u8 alignment);
		void Clear();

		template <typename T, typename... args>
		T* New(args... argList);

		template <typename T>
		T* NewArray(size capacity, u8 alignment);

		void FreeToMarker(Marker marker);
		[[nodiscard]] Marker GetMarker() const;

	private:
		void* m_memory;
		size m_capacity;
		Marker m_top;
		PtrInt m_bottom;
	};

	template <typename T, typename ... args>
	T* StackAllocator::New(args... argList) {
		void* memory = Allocate(sizeof(T), ALIGNMENT);
		return new (memory)T(argList...);
	}

	template <typename T>
	T* StackAllocator::NewArray(const size capacity, const u8 alignment) {
		void* alloc = Allocate(sizeof(T) * capacity, alignment);
		const char* allocAddress = static_cast<char*>(alloc);
		for (int i = 0; i < capacity; ++i) new (allocAddress + i * sizeof(T)) T;
		return static_cast<T*>(alloc);
	}
}

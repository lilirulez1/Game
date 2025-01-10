//
// Created by liamb on 8/01/2025.
//

#pragma once

#include "Core/Memory/MemoryUtil.h"
#include "Util/Util.h"

namespace Engine::Memory {

	template <typename T>
	class PoolAllocator {
	public:
		PoolAllocator() = delete;
		PoolAllocator(void* memory, size count);
		explicit PoolAllocator(size count);
		~PoolAllocator();

		template <typename ... Args>
		T* Get(int* index = nullptr, Args&& ...);
		T* GetByIndex(size index);
		void Free(T*);

	private:
		// Wtf does union mean
		union PoolNode {
			PoolNode* next;
			explicit PoolNode(PoolNode* next) { this->next = next; }
		};

		void InitializeNodes(void* memory, size count);

		bool m_isGlobal;
		PoolNode* m_head;
		void* m_memory;
		size m_capacity;
		size m_chunkSize;
	};

	template <typename T>
	PoolAllocator<T>::PoolAllocator(void* memory, const size count)
	: m_isGlobal(false), m_memory(memory), m_capacity(count), m_chunkSize(sizeof(T)) {
		InitializeNodes(memory, count);
	}

	template <typename T>
	PoolAllocator<T>::PoolAllocator(const size count)
	: m_isGlobal(true), m_memory(std::malloc(sizeof(T) * count)), m_capacity(count), m_chunkSize(sizeof(T)) {
		InitializeNodes(m_memory, count);
	}

	template <typename T>
	PoolAllocator<T>::~PoolAllocator() {
		if (m_isGlobal) {
			std::free(m_memory);
		}
	}

	template <typename T>
	template <typename ... Args>
	T* PoolAllocator<T>::Get(int* index, Args&& ... args) {
		if (m_head == nullptr) {
			throw std::overflow_error("Not enough memory");
		}

		void* toReturn = m_head;

		if (index != nullptr) {
			const PtrInt chunkAddress = reinterpret_cast<PtrInt>(toReturn);
			const PtrInt memoryAddress = reinterpret_cast<PtrInt>(m_memory);
			*index = static_cast<int>((chunkAddress - memoryAddress) / m_chunkSize);
		}

		m_head = m_head->next;
		memset(toReturn, 0x0, sizeof(PoolNode));
		return new (toReturn) T(std::forward<Args>(args)...);
	}

	template <typename T>
	T* PoolAllocator<T>::GetByIndex(const size index) {
		if (index >= m_capacity) {
			throw std::out_of_range("Index out of bounds");
		}

		const PtrInt memoryAddress = reinterpret_cast<PtrInt>(m_memory);
		PtrInt chunkAddress = memoryAddress + (m_chunkSize * index);
		return reinterpret_cast<T*>(chunkAddress);
	}

	template <typename T>
	void PoolAllocator<T>::Free(T* memory) {
		memset(memory, 0x0, m_chunkSize);
		PoolNode* newHead = new (memory) PoolNode{m_head};
		m_head = newHead;
	}

	template <typename T>
	void PoolAllocator<T>::InitializeNodes(void* memory, const size count) {
		m_head = new (memory) PoolNode{nullptr};
		PoolNode* current = m_head;
		PtrInt memoryAddress = reinterpret_cast<PtrInt>(memory);

		for (size i = 1; i < count; ++i) {
			memoryAddress += m_chunkSize;
			PoolNode* node = new (reinterpret_cast<void*>(memoryAddress)) PoolNode{nullptr};
			current->next = node;
			current = current->next;
		}
	}

}

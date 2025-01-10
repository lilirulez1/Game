//
// Created by liamb on 7/01/2025.
//

#include "pch.h"
#include "StackAllocator.h"

namespace Engine::Memory {

	StackAllocator::StackAllocator(const size stackSize)
	: m_memory(std::malloc(stackSize)), m_capacity(stackSize), m_top(0), m_bottom(reinterpret_cast<PtrInt>(m_memory)) {}

	StackAllocator::~StackAllocator() {
		std::free(m_memory);
	}

	void* StackAllocator::Allocate(const size size, const u8 alignment) {
		CheckAlignment(alignment);

		const PtrInt currentAddress = m_bottom + m_top;
		const PtrInt misalignment = currentAddress & (alignment - 1);
		PtrDiff adjustment = alignment - misalignment;
		adjustment = adjustment & (alignment - 1);
		const PtrInt alignedAddress = currentAddress + adjustment;
		const Marker newTop = m_top + size + adjustment;

		if (newTop > m_capacity) {
			throw std::overflow_error("Not enough memory");
		}

		m_top = newTop;

		return reinterpret_cast<void*>(alignedAddress);
	}

	void StackAllocator::Clear() {
		m_top = 0;
	}

	void StackAllocator::FreeToMarker(const Marker marker) {
		m_top = marker;
	}

	StackAllocator::Marker StackAllocator::GetMarker() const {
		return m_top;
	}

}

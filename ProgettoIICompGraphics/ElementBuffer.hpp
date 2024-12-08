#pragma once

#include "SimpleBuffer.hpp"
#include <vector>

/**
 * Class to hold the information of an ElementBuffer (the indices of an object).
 */
class ElementBuffer : public SimpleBuffer {
public:
	/**
	 * Constructor for the element buffer.
	 *
	 * \param indices The indices to save in the GPU buffer.
	 * \param dynamic Flag to check if the data can be overwritten.
	 */
	ElementBuffer(const std::vector<uint32_t>& indices, const bool dynamic = false);
};
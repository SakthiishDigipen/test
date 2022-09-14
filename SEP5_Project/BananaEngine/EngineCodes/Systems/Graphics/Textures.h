#pragma once

#include "GraphicsSystem.h"

namespace BE {

	bool load_image_from_file(VulkanSystem& engine, const char* file, AllocatedImage& outImage);

}
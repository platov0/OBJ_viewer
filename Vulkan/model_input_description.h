#ifndef MODEL_INPUT_DESCRIPTION
#define MODEL_INPUT_DESCRIPTION
#include <vector>
#include "vulkan/vulkan.h"
class ModelInputDescription
{
public:
	void CreateInputDescription()
	{
		binding_description.binding = 0;
		binding_description.stride = 24;
		binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		attribute_descriptions.emplace_back();
		attribute_descriptions.back().binding = 0;
		attribute_descriptions.back().location = 0;
		attribute_descriptions.back().format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions.back().offset = 0;

		attribute_descriptions.emplace_back();
		attribute_descriptions.back().binding = 0;
		attribute_descriptions.back().location = 1;
		attribute_descriptions.back().format = VK_FORMAT_R32G32B32_SFLOAT;
		attribute_descriptions.back().offset = 12;
	}
public:
	VkVertexInputBindingDescription binding_description{};
	std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};
};
#endif // !CORE_INPUT_DESCRIPTION

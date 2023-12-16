#ifndef MODEL_TRANSIT_COMMAND_BUFFERS
#define MODEL_TRANSIT_COMMAND_BUFFERS
#include <vulkan/vulkan.h>
#include <stdexcept>
class ModelTransitCommandBuffer
{
public:
	void CreateCommandBuffers(VkDevice& device, uint32_t graphics_family_index)
	{
		CreateCommandPool(device, graphics_family_index);
		AllocateCommandBuffers(device);
	}
private:
	void CreateCommandPool(VkDevice& device, uint32_t graphics_family_index)
	{
		VkCommandPoolCreateInfo command_pool_info{};
		command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		command_pool_info.queueFamilyIndex = graphics_family_index;
		if (vkCreateCommandPool(device, &command_pool_info, nullptr, &command_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool!");
		}
	}
	void AllocateCommandBuffers(VkDevice& device)
	{
		VkCommandBufferAllocateInfo command_buffer_info{};
		command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		command_buffer_info.commandPool = command_pool;
		command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		command_buffer_info.commandBufferCount = 2;
		if (vkAllocateCommandBuffers(device, &command_buffer_info, command_buffers) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}
public:
	VkCommandBuffer command_buffers[2];
private:
	VkCommandPool command_pool;
};
#endif // !EMPTY_COMMAND_BUFFERS

#ifndef MODEL_VERTEX_BUFFER
#define MODEL_VERTEX_BUFFER
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
class ModelVertexBuffer
{
public:
	void CreateVertexBuffer(VkPhysicalDevice& physical_device, VkDevice& device, VkQueue& graphic_queue, uint32_t graphics_family_index, VkCommandBuffer& transit_command_buffer, const void* data, uint32_t buffer_size)
	{
		CreateBuffer(physical_device, device, buffer_size, staging_buffer, staging_memory, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		CopyDataToStagingBuffer(device, data, buffer_size, staging_memory);
		CreateBuffer(physical_device, device, buffer_size, buffer, buffer_memory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CopyDataToLocalBuffer(device, graphic_queue, graphics_family_index, buffer_size, transit_command_buffer, staging_buffer, buffer);
	}
private:
	void CreateBuffer(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t buffer_size, VkBuffer& used_buffer, VkDeviceMemory& used_buffer_memory, VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_flags)
	{
		VkBufferCreateInfo buffer_create_info{};
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = buffer_size;
		buffer_create_info.usage = usage;
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(device, &buffer_create_info, nullptr, &used_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetBufferMemoryRequirements(device, used_buffer, &mem_requirements);

		VkMemoryAllocateInfo allocate_info{};
		allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocate_info.allocationSize = mem_requirements.size;
		allocate_info.memoryTypeIndex = FindMemoryIndex(physical_device, mem_requirements, memory_flags);
		if (vkAllocateMemory(device, &allocate_info, nullptr, &used_buffer_memory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}
		vkBindBufferMemory(device, used_buffer, used_buffer_memory, 0);
	}
	void CopyDataToStagingBuffer(VkDevice& device, const void* data, uint32_t buffer_size, VkDeviceMemory& used_buffer_memory)
	{
		vkMapMemory(device, used_buffer_memory, 0, buffer_size, 0, &data_vert);
		memcpy(data_vert, data, buffer_size);
		vkUnmapMemory(device, used_buffer_memory);
	}
	void CopyDataToLocalBuffer(VkDevice& device, VkQueue& graphic_queue, uint32_t graphics_family_index, uint32_t buffer_size, VkCommandBuffer& transit_command_buffer, VkBuffer& scr_buffer, VkBuffer& dst_buffer)
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(transit_command_buffer, &begin_info);

		VkBufferCopy copy_region{};
		copy_region.size = buffer_size;
		vkCmdCopyBuffer(transit_command_buffer, scr_buffer, dst_buffer, 1, &copy_region);

		vkEndCommandBuffer(transit_command_buffer);

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &transit_command_buffer;

		vkQueueSubmit(graphic_queue, 1, &submit_info, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphic_queue);
		vkResetCommandBuffer(transit_command_buffer, 0);
	}
	uint32_t FindMemoryIndex(const VkPhysicalDevice& physical_device, VkMemoryRequirements& requirements, VkMemoryPropertyFlags& flags)
	{
		VkPhysicalDeviceMemoryProperties properties{};
		uint32_t type_index;
		vkGetPhysicalDeviceMemoryProperties(physical_device, &properties);
		for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
		{
			if ((requirements.memoryTypeBits & (1 << i)) && (properties.memoryTypes[i].propertyFlags & (flags)) == (flags))
			{
				type_index = i;
				return type_index;
			}
		}
		//return type_index;
	}
public:
	VkBuffer buffer;
	VkDeviceMemory buffer_memory;
private:
	void* data_vert;
	VkBuffer staging_buffer;
	VkDeviceMemory staging_memory;
};
#endif // !VERTEX_BUFFER


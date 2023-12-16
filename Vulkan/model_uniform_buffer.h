#ifndef MODEL_UNIFORM_BUFFER
#define MODEL_UNIFORM_BUFFER
#include <vulkan/vulkan.h>
#include <stdexcept>
class ModelUniformBuffer
{
public:
	void CreateUniformBuffer(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t buffer_size)
	{
		VkBufferCreateInfo buffer_create_info{};
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = buffer_size;
		buffer_create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetBufferMemoryRequirements(device, buffer, &mem_requirements);

		VkMemoryAllocateInfo allocate_info{};
		allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocate_info.allocationSize = mem_requirements.size;
		allocate_info.memoryTypeIndex = FindMemoryIndex(physical_device, mem_requirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(device, &allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}
		vkBindBufferMemory(device, buffer, buffer_memory, 0);
	}
	void UpdateUniformBuffer(VkDevice& device, const void* data, uint32_t size)
	{
		void* data_vert;
		vkMapMemory(device, buffer_memory, 0, size, 0, &data_vert);
		memcpy(data_vert, data, size);
		vkUnmapMemory(device, buffer_memory);
	}
private:
	uint32_t FindMemoryIndex(const VkPhysicalDevice& physical_device, VkMemoryRequirements& requirements, VkMemoryPropertyFlags flags)
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
};
#endif // !UNIFORM_BUFFER

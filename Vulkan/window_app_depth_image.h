#ifndef WINDOW_APP_DEPTH_IMAGE
#define WINDOW_APP_DEPTH_IMAGE
#include <vulkan/vulkan.h>
#include <stdexcept>
class WindowAppDepthImage
{
public:
	void CreateDepthImage(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t width, uint32_t height)
	{
		VkImageCreateInfo image_create_info{};
		image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.extent.width = width;
		image_create_info.extent.height = height;
		image_create_info.extent.depth = 1;
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.format = VK_FORMAT_D32_SFLOAT;
		image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &image_create_info, nullptr, &image) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetImageMemoryRequirements(device, image, &mem_requirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = mem_requirements.size;
		allocInfo.memoryTypeIndex = FindMemoryIndex(physical_device, mem_requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &image_memory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate image memory!");
		}
		vkBindImageMemory(device, image, image_memory, 0);

		VkImageViewCreateInfo view_info{};
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = image;
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = VK_FORMAT_D32_SFLOAT;
		view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &view_info, nullptr, &image_view) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture image view!");
		}
	}
private:
	uint32_t FindMemoryIndex(const VkPhysicalDevice& physical_device, const VkMemoryRequirements& requirements, const VkMemoryPropertyFlags& flags)
	{
		uint32_t type_index;
		VkPhysicalDeviceMemoryProperties properties{};
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
	VkImage image;
	VkImageView image_view;
	VkDeviceMemory image_memory;
};
#endif // !DEPTH_IMAGE


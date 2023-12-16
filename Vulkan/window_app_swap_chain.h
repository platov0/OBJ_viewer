#ifndef WINDOW_APP_SWAP_CHAIN
#define WINDOW_APP_SWAP_CHAIN
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
class WindowAppSwapChain
{
public:
	void CreateSwapChain(VkPhysicalDevice& physical_device, VkDevice& device, VkSurfaceKHR& surface, uint32_t graphics_family_index, uint32_t present_family_index)
	{
		// choose surface format
		uint32_t format_ñount{};
		std::vector<VkSurfaceFormatKHR> surface_formats{};
		VkSurfaceCapabilitiesKHR surface_ñapabilities;
		VkSurfaceFormatKHR surface_format;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_ñapabilities);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_ñount, nullptr);
		if (format_ñount)
		{
			surface_formats.resize(format_ñount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_ñount, surface_formats.data());
		}

		for (auto& surface_format_t : surface_formats)
		{
			surface_format = surface_formats[0];

			if (surface_format_t.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format_t.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				surface_format = surface_format_t;
				break;
			}
		}
		// choose present mode
		std::vector<VkPresentModeKHR> present_modes{};
		VkPresentModeKHR present_mode{};
		uint32_t present_mode_count{};
		//
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
		if (present_mode_count)
		{
			present_modes.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
		}
		for (auto& present_mode_t : present_modes)
		{
			present_mode = VK_PRESENT_MODE_FIFO_KHR;

			if (present_mode_t == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				present_mode = present_mode_t;
				break;
			}
		}
		// choose image count
		VkSwapchainCreateInfoKHR swap_chain_info{};
		uint32_t queue_family_indices[] = { graphics_family_index, present_family_index };

		// create swap chain
		if (graphics_family_index != present_family_index)
		{
			swap_chain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swap_chain_info.queueFamilyIndexCount = 2;
			swap_chain_info.pQueueFamilyIndices = queue_family_indices;
		}
		else
		{
			swap_chain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		swap_chain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swap_chain_info.surface = surface;
		swap_chain_info.minImageCount = 2;
		swap_chain_info.imageFormat = surface_format.format;
		swap_chain_info.imageColorSpace = surface_format.colorSpace;
		swap_chain_info.imageExtent = surface_ñapabilities.currentExtent;
		swap_chain_info.imageArrayLayers = 1;
		swap_chain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swap_chain_info.preTransform = surface_ñapabilities.currentTransform;
		swap_chain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swap_chain_info.presentMode = present_mode;
		swap_chain_info.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(device, &swap_chain_info, nullptr, &swap_chain_handle) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create swap chain!");
		}

		// extract image handles
		vkGetSwapchainImagesKHR(device, swap_chain_handle, &surface_ñapabilities.minImageCount, nullptr);
		swap_chain_images.resize(surface_ñapabilities.minImageCount);
		vkGetSwapchainImagesKHR(device, swap_chain_handle, &surface_ñapabilities.minImageCount, swap_chain_images.data());

		// create image views
		VkImageViewCreateInfo image_view_info{};
		swap_chain_image_views.resize(swap_chain_images.size());

		for (int i = 0; i < swap_chain_images.size(); i++)
		{
			image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_info.image = swap_chain_images[i];
			image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_info.format = surface_format.format;
			image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_info.subresourceRange.baseMipLevel = 0;
			image_view_info.subresourceRange.levelCount = 1;
			image_view_info.subresourceRange.baseArrayLayer = 0;
			image_view_info.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &image_view_info, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create image views!");
			}
		}
	}
public:
	VkSwapchainKHR swap_chain_handle;
	std::vector<VkImage> swap_chain_images;
	std::vector<VkImageView> swap_chain_image_views;
};
#endif // !SWAP_CHAIN


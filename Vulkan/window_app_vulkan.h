#ifndef WINDOW_APP_VULKAN
#define WINDOW_APP_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3_m.h>
#include <vector>
#include <optional>
#include <set>
#include <stdexcept>
class WindowAppVulkan
{
public:
	void InitVulkan(GLFWwindow* window)
	{
		CreateVulkanInstanceAndWindowSurface(window);
		CreateDevicesAndQueues(vulkan_instance, window_surface);
	}
private:
	void CreateVulkanInstanceAndWindowSurface(GLFWwindow* window)
	{
		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName = "Hello Triangle";
		application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pEngineName = "No Engine";
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.apiVersion = VK_API_VERSION_1_3;

		uint32_t glfw_extension_count{};
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		VkInstanceCreateInfo instance_info{};
		const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pApplicationInfo = &application_info;
		instance_info.enabledExtensionCount = glfw_extension_count;
		instance_info.ppEnabledExtensionNames = glfw_extensions;
		instance_info.enabledLayerCount = validation_layers.size();
		instance_info.ppEnabledLayerNames = validation_layers.data();

		if (vkCreateInstance(&instance_info, NULL, &vulkan_instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}

		if (glfwCreateWindowSurface(vulkan_instance, window, nullptr, &window_surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}
	void CreateDevicesAndQueues(VkInstance& vulkan_instance, VkSurfaceKHR& window_surface)
	{
		uint32_t device_count{};
		std::vector<VkPhysicalDevice> physical_devices{};

		vkEnumeratePhysicalDevices(vulkan_instance, &device_count, nullptr);
		physical_devices.resize(device_count);
		vkEnumeratePhysicalDevices(vulkan_instance, &device_count, physical_devices.data());
		physical_device = physical_devices[0];

		// search for suitable queue family indexes
		uint32_t queue_family_count{};
		std::vector<VkQueueFamilyProperties> queue_families{};
		VkBool32 present_support = false;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);
		queue_families.resize(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

		int i{};
		for (auto& queue_family : queue_families)
		{
			if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphics_family_index = i;
			}

			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, window_surface, &present_support);
			if (present_support)
			{
				present_family_index = i;
			}
			if (graphics_family_index.has_value() && present_family_index.has_value())
			{
				break;
			}
			present_support = 0;
			i++;
		}

		// create queue info
		std::set<uint32_t> family_indices{ graphics_family_index.value(), present_family_index.value() };
		std::vector<VkDeviceQueueCreateInfo> queue_infos{};
		float queue_priorities = 1;
		//
		for (uint32_t family_index : family_indices)
		{
			VkDeviceQueueCreateInfo queue_info{};
			queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info.queueFamilyIndex = family_index;
			queue_info.queueCount = 1;
			queue_info.pQueuePriorities = &queue_priorities;

			queue_infos.push_back(queue_info);
		}

		// create logical device
		VkPhysicalDeviceFeatures device_features{};
		device_features.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo device_info{};
		const char* extension_name[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.pQueueCreateInfos = queue_infos.data();
		device_info.queueCreateInfoCount = queue_infos.size();
		device_info.enabledExtensionCount = 1;
		device_info.ppEnabledExtensionNames = extension_name; // add check extension support function
		device_info.pEnabledFeatures = &device_features;

		if (vkCreateDevice(physical_device, &device_info, nullptr, &device) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(device, graphics_family_index.value(), 0, &graphics_queue);
		vkGetDeviceQueue(device, present_family_index.value(), 0, &present_queue);
	}
public:
	VkInstance vulkan_instance;
	VkSurfaceKHR window_surface;
	VkPhysicalDevice physical_device;
	VkDevice device;
	std::optional<uint32_t> graphics_family_index;
	std::optional<uint32_t> present_family_index;
	VkQueue graphics_queue;
	VkQueue present_queue;
};
#endif // !VULKAN_INSTANCE


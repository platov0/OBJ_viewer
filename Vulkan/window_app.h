#ifndef WINDOW_APP
#define WINDOW_APP
#include "window_app_window.h"
#include "window_app_vulkan.h"
#include "window_app_swap_chain.h"
#include "window_app_depth_image.h"
#include "window_app_render_pass.h"
#include "window_app_framebuffer.h"
class WindowApp
{
public:
	void InitWindow(int width, int height)
	{
		window.CreateWindow(width, height);
	}
	void InitVulkan()
	{
		vulkan.InitVulkan(window.window_handle);
	}
	void InitSwapChain()
	{
		swap_chain.CreateSwapChain(vulkan.physical_device, vulkan.device, vulkan.window_surface, vulkan.graphics_family_index.value(), vulkan.present_family_index.value());
	}
	void InitDepthImage(int width, int height)
	{
		depth_image.CreateDepthImage(vulkan.physical_device, vulkan.device, width, height);
	}
	void InitRenderPass()
	{
		render_pass.CreateRenderPass(vulkan.device);
	}
	void InitFramebuffer(int width, int height)
	{
		framebuffer.CreateFramebuffer(vulkan.device, swap_chain.swap_chain_image_views, depth_image.image_view, render_pass.render_pass_handle, width, height);
	}
public:
	WindowAppWindow window;
	WindowAppVulkan vulkan;
	WindowAppSwapChain swap_chain;
	WindowAppDepthImage depth_image;
	WindowAppRenderPass render_pass;
	WindowAppFramebuffer framebuffer;
};
#endif // !WINDOW_APP


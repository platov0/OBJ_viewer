#ifndef WINDOW_APP_FRAMEBUFFER
#define WINDOW_APP_FRAMEBUFFER
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
class WindowAppFramebuffer
{
public:
	void CreateFramebuffer(VkDevice& device, std::vector<VkImageView>& swap_chain_image_views, VkImageView& depth_image_view, VkRenderPass& render_pass, uint32_t width, uint32_t height)
	{
		for (int i = 0; i < 2; i++)
		{
			std::vector<VkImageView> attachments = { swap_chain_image_views[i],depth_image_view };
			VkFramebufferCreateInfo framebuffer_Info{};
			framebuffer_Info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_Info.renderPass = render_pass;
			framebuffer_Info.attachmentCount = 2;
			framebuffer_Info.pAttachments = attachments.data();
			framebuffer_Info.width = width;
			framebuffer_Info.height = height;
			framebuffer_Info.layers = 1;

			framebuffers.emplace_back();
			if (vkCreateFramebuffer(device, &framebuffer_Info, nullptr, &framebuffers.back()) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}
public:
	std::vector<VkFramebuffer> framebuffers;
};
#endif // !FRAMEBUFFER


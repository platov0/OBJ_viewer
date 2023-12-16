#ifndef SHADOW_MAP_COMMAND_BUFFER
#define SHADOW_MAP_COMMAND_BUFFER
#include <stdexcept>
#include <array>
#include "vulkan/vulkan.h"
class ShadowMapCommandBuffer
{
public:
    void CreateCommandBuffer(VkDevice& device, uint32_t graphics_family_index)
    {
        CreateCommandPool(device, graphics_family_index);
        AllocateCommandBuffer(device);
    }
    void RecordCommandBuffer(VkCommandBuffer& command_buffer, VkRenderPass& render_pass, VkFramebuffer& framebuffer, VkPipeline& pipeline, VkPipelineLayout& pipeline_layout, VkBuffer& vertex_buffer, VkBuffer& indices_buffer, VkDescriptorSet& descriptor_set, uint32_t indices_count, uint32_t width, uint32_t height)
    {
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(command_buffer, &begin_info);

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = render_pass;
        render_pass_info.framebuffer = framebuffer;
        render_pass_info.renderArea.offset = { 0, 0 };
        render_pass_info.renderArea.extent = { width,height };

        VkClearValue clear_values{};
        clear_values.color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clear_values.depthStencil = { 1.0f, 0 };

        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clear_values;

        vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)width;
        viewport.height = (float)height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = { width,height };
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        VkBuffer vertexBuffers[] = { vertex_buffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(command_buffer, indices_buffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptor_set, 0, nullptr);

        vkCmdDrawIndexed(command_buffer, indices_count, 1, 0, 0, 0);

        vkCmdEndRenderPass(command_buffer);

        vkEndCommandBuffer(command_buffer);
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
    void AllocateCommandBuffer(VkDevice& device)
    {
        VkCommandBufferAllocateInfo command_buffer_info{};
        command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_info.commandPool = command_pool;
        command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_info.commandBufferCount = 1;
        if (vkAllocateCommandBuffers(device, &command_buffer_info, &command_buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
public:
    VkCommandBuffer command_buffer;
private:
    VkCommandPool command_pool;
};
#endif // !SHADOW_MAP_COMMAND_BUFFER

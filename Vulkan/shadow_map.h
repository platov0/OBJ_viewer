#ifndef SHADOW_MAP
#define SHADOW_MAP
#include "shadow_map_matrices.h"
#include "shadow_map_depth_image.h"
#include "shadow_map_render_pass.h"
#include "shadow_map_framebuffer.h"
#include "shadow_map_uniform_buffer.h"
#include "shadow_map_descriptors.h"
#include "shadow_map_shaders_loader.h"
#include "shadow_map_pipeline.h"
#include "shadow_map_command_buffer.h"
class ShadowMap
{
public:
	void InitDepthImage(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t width, uint32_t height)
	{
		depth_image.CreateDepthImage(physical_device, device, width, height);
	}
	void InitRenderPass(VkDevice& device)
	{
		render_pass.CreateRenderPass(device);
	}
	void InitFramebuffer(VkDevice& device, uint32_t width, uint32_t height)
	{
		framebuffer.CreateFramebuffer(device, depth_image.image_view, render_pass.render_pass_handle, width, height);
	}
	void InitUniformBuffer(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t size)
	{
		uniform_buffer.CreateUniformBuffer(physical_device, device, size);
	}
	void InitDescriptors(VkDevice& device,uint32_t uniform_buffer_size)
	{
		descriptors.CreateDescriptors(device, uniform_buffer.buffer, uniform_buffer_size);
	}
	void InitShaders(std::string vertex_shader_path, std::string fragment_shader_path)
	{
		shaders_loader.LoadVertexShaderCode(vertex_shader_path);
		shaders_loader.LoadFragmentShaderCode(fragment_shader_path);
	}
	void InitPipeline(VkDevice& device, VkVertexInputBindingDescription& binding_description, std::vector<VkVertexInputAttributeDescription>& attribute_descriptions)
	{
		pipeline.CreatePipeline(device, shaders_loader.vertex_shader_code, shaders_loader.fragment_shader_code, binding_description, attribute_descriptions, descriptors.descriptor_set_layout, render_pass.render_pass_handle);
	}
	void InitCommandBuffer(VkDevice& device, uint32_t graphics_family_index)
	{
		command_buffer.CreateCommandBuffer(device, graphics_family_index);
	}

	void UpdateUniformBuffer(VkDevice& device, float ortho_matrix_aspect_ratio, float ortho_matrix_near, float ortho_matrix_far, float scale_factor, float rotate_x, float rotate_y, float rotate_z, float offset_x, float offset_y, float offset_z, float translate_x, float translate_y, float translate_z, float light_pos_x, float light_pos_y, float light_pos_z, float light_view_x, float light_view_y, float light_view_z)
	{
		matrices.CreateMatrices(ortho_matrix_aspect_ratio, ortho_matrix_near, ortho_matrix_far, scale_factor, rotate_x, rotate_y, rotate_z, offset_x, offset_y, offset_z, translate_x, translate_y, translate_z, light_pos_x, light_pos_y, light_pos_z, light_view_x, light_view_y, light_view_z);
		uniform_buffer.UpdateUniformBuffer(device, &matrices.matrix_set, sizeof(matrices.matrix_set));
	}

	void DrawShadowMap(VkDevice& device, VkQueue& graphic_queue, VkBuffer& vertex_buffer, VkBuffer& indices_buffer, uint32_t indices_count, int width, int height, VkFence& fence, uint32_t frame_num)
	{
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &fence);
		vkResetCommandBuffer(command_buffer.command_buffer, 0);
		command_buffer.RecordCommandBuffer(command_buffer.command_buffer, render_pass.render_pass_handle, framebuffer.framebuffers[frame_num], pipeline.pipeline, pipeline.pipeline_layout, vertex_buffer, indices_buffer, descriptors.descriptor_set, indices_count, width, height);

		VkSubmitInfo submit_info{};
		VkPipelineStageFlags wait_stage[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitDstStageMask = wait_stage;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer.command_buffer;
		if (vkQueueSubmit(graphic_queue, 1, &submit_info, fence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}
public:
	ShadowMapMatrices matrices;
	ShadowMapDepthImage depth_image;
	ShadowMapRenderPass render_pass;
	ShadowMapFrambuffer framebuffer;
	ShadowMapUniformBuffer uniform_buffer;
	ShadowMapDescriptors descriptors;
	ShadowMapShadersLoader shaders_loader;
	ShadowMapPipeline pipeline;
	ShadowMapCommandBuffer command_buffer;
};
#endif // !SHADOW_MAP

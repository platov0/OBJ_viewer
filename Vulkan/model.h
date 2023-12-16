#ifndef MODEL
#define MODEL
#include "model_matrices.h"
#include "model_mesh.h"
#include "model_input_description.h"
#include "model_transit_command_buffer.h"
#include "model_vertex_buffer.h"
#include "model_indices_buffer.h"
#include "model_descriptors.h"
#include "model_shaders_loader.h"
#include "model_pipeline.h"
#include "model_uniform_buffer.h"
#include "model_command_buffer.h"
class Model
{
public:
	void InitMesh(std::string mesh_path)
	{
		mesh.CreateMesh(mesh_path);
	}
	void InitInputDescription()
	{
		input_description.CreateInputDescription();
	}
	void InitCommandBuffers(VkDevice& device, uint32_t graphics_family_index)
	{
		transit_command_buffer.CreateCommandBuffers(device, graphics_family_index);
		command_buffer.CreateCommandBuffer(device, graphics_family_index);
	}
	void InitVerticesBuffers(VkPhysicalDevice& physical_device, VkDevice& device, VkQueue& graphic_queue, uint32_t graphics_family_index)
	{
		vertex_buffer.CreateVertexBuffer(physical_device, device, graphic_queue, graphics_family_index, transit_command_buffer.command_buffers[0], mesh.vertices.data(), sizeof(mesh.vertices[0]) * mesh.vertices.size());
		indices_buffer.CreateIndicesBuffer(physical_device, device, graphic_queue, graphics_family_index, transit_command_buffer.command_buffers[1], mesh.indices.data(), sizeof(mesh.indices[0]) * mesh.indices.size());
	}
	void InitUniformBuffer(VkPhysicalDevice& physical_device, VkDevice& device, uint32_t size)
	{
		uniform_buffer.CreateUniformBuffer(physical_device, device, size);
	}
	void InitDescriptors(VkDevice& device, uint32_t uniform_buffer_size, VkImageView& image_view, VkSampler& image_sampler)
	{
		descriptors.CreateDescriptors(device, uniform_buffer.buffer, uniform_buffer_size, image_view, image_sampler);
	}
	void InitShaders(std::string vertex_shader_path, std::string fragment_shader_path)
	{
		shaders_loader.LoadVertexShaderCode(vertex_shader_path);
		shaders_loader.LoadFragmentShaderCode(fragment_shader_path);
	}
	void InitPipeline(VkDevice& device, VkRenderPass& render_pass)
	{
		pipeline.CreatePipeline(device, shaders_loader.vertex_shader_code, shaders_loader.fragment_shader_code, input_description.binding_description, input_description.attribute_descriptions, descriptors.descriptor_set_layout, render_pass);
	}

	void UpdateUniformBuffer(VkDevice& device, glm::mat4& shadow_matrix, glm::mat4& centering, glm::vec3 shadow_vector, float scale_factor, float rotate_x, float rotate_y, float rotate_z, float translate_x, float translate_y, float translate_z)
	{
		matrices.CreateMatrices(shadow_matrix, centering, shadow_vector, scale_factor, rotate_x, rotate_y, rotate_z, translate_x, translate_y, translate_z);
		uniform_buffer.UpdateUniformBuffer(device, &matrices.matrix_set, sizeof(matrices.matrix_set));
	}

	void DrawModel(VkDevice& device, VkSwapchainKHR& swap_chain, VkQueue& graphic_queue, VkRenderPass& render_pass, VkFramebuffer& framebuffer,VkFence& fence,VkSemaphore& image_avaliable_semaphore, VkSemaphore& render_finish_semaphore, uint32_t width, uint32_t height, int frame_num)
	{
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &fence);
		vkResetCommandBuffer(command_buffer.command_buffer, 0);
		vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX, image_avaliable_semaphore, VK_NULL_HANDLE, &image_index);
		command_buffer.RecordCommandBuffer(command_buffer.command_buffer, render_pass, framebuffer, pipeline.pipeline, pipeline.pipeline_layout, vertex_buffer.buffer, indices_buffer.buffer, descriptors.descriptor_set, mesh.indices.size(), width, height);

		VkSubmitInfo submit_info{};
		VkPipelineStageFlags wait_stage[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = &image_avaliable_semaphore;
		submit_info.pWaitDstStageMask = wait_stage;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer.command_buffer;
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &render_finish_semaphore;
		if (vkQueueSubmit(graphic_queue, 1, &submit_info, fence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}
	}
	void PresentModel(VkSwapchainKHR& shap_chain, VkQueue& present_queue, VkSemaphore& render_finish_semaphore)
	{
		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = &render_finish_semaphore;
		present_info.swapchainCount = 1;
		present_info.pSwapchains = &shap_chain;
		present_info.pImageIndices = &image_index;
		vkQueuePresentKHR(present_queue, &present_info);
	}
public:
	ModelMatrices matrices;
	ModelMesh mesh;
	ModelInputDescription input_description;
	ModelTransitCommandBuffer transit_command_buffer;
	ModelVertexBuffer vertex_buffer;
	ModelIndicesBuffer indices_buffer;
	ModelDescriptors descriptors;
	ModelShadersLoader shaders_loader;
	ModelPipeline pipeline;
	ModelUniformBuffer uniform_buffer;
	ModelCommandBuffer command_buffer;
private:
	uint32_t image_index;
};
#endif // !MODEL


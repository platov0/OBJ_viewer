#include "window_app.h"
#include "model.h"
#include "shadow_map.h"
#include "render_sync.h"
#include "keyboard_input.h"
class Render
{
public:
	void InitWindowApp(int window_width, int window_height)
	{
		window.InitWindow(window_width, window_height);
		window.InitVulkan();
		window.InitSwapChain();
		window.InitDepthImage(window_width, window_height);
		window.InitRenderPass();
		window.InitFramebuffer(window_width, window_height);
	}
	void InitScene(std::string mesh_path, std::string shadow_map_vertex_shader_path, std::string shadow_map_fragment_shader_path, std::string model_vertex_shader_path, std::string model_fragment_shader_path, int window_width, int window_height, int shadow_map_width, int shadow_map_height)
	{
		model.InitMesh(mesh_path);
		model.InitInputDescription();

		shadow_map.InitDepthImage(window.vulkan.physical_device, window.vulkan.device, shadow_map_width, shadow_map_height);
		shadow_map.InitRenderPass(window.vulkan.device);
		shadow_map.InitFramebuffer(window.vulkan.device, shadow_map_width, shadow_map_height);
		shadow_map.InitUniformBuffer(window.vulkan.physical_device, window.vulkan.device, sizeof(shadow_map.matrices.matrix_set));
		shadow_map.InitDescriptors(window.vulkan.device, sizeof(shadow_map.matrices.matrix_set));
		shadow_map.InitShaders(shadow_map_vertex_shader_path, shadow_map_fragment_shader_path);
		shadow_map.InitPipeline(window.vulkan.device, model.input_description.binding_description, model.input_description.attribute_descriptions);
		shadow_map.InitCommandBuffer(window.vulkan.device, window.vulkan.graphics_family_index.value());

		model.InitCommandBuffers(window.vulkan.device, window.vulkan.graphics_family_index.value());
		model.InitVerticesBuffers(window.vulkan.physical_device, window.vulkan.device, window.vulkan.graphics_queue, window.vulkan.graphics_family_index.value());
		model.InitUniformBuffer(window.vulkan.physical_device, window.vulkan.device, sizeof(model.matrices.matrix_set));
		model.InitDescriptors(window.vulkan.device, sizeof(model.matrices.matrix_set), shadow_map.depth_image.image_view, shadow_map.depth_image.image_sampler);
		model.InitShaders(model_vertex_shader_path, model_fragment_shader_path);
		model.InitPipeline(window.vulkan.device, window.render_pass.render_pass_handle);
	}
	void InitSyncPrimitives()
	{
		sync.CreateSyncPrimitives(window.vulkan.device);
	}
	void Draw(int window_width, int window_height, int shadow_map_width, int shadow_map_height)
	{
		int current_frame{1};

		while (!glfwWindowShouldClose(window.window.window_handle))
		{
			input.ActivateInput(window.window.window_handle);

			shadow_map.UpdateUniformBuffer(window.vulkan.device, 800.0f / 600.0f, 0.0f, -10.0f, model.mesh.basic_transform.scale, 0, input.rotate_y, 0, model.mesh.basic_transform.x_offset, model.mesh.basic_transform.y_offset, model.mesh.basic_transform.z_offset, 0, 0, -0.5, 0.0f, 1.0f, 0.0f, -0.5f, 0.0f, -1.0f);
			model.UpdateUniformBuffer(window.vulkan.device, shadow_map.matrices.shadow_matrix, shadow_map.matrices.matrix_set.centering, shadow_map.matrices.shadow_vector, model.mesh.basic_transform.scale, 0, input.rotate_y, 0, 0, 0, -0.5f);

			shadow_map.DrawShadowMap(window.vulkan.device, window.vulkan.graphics_queue, model.vertex_buffer.buffer, model.indices_buffer.buffer, model.mesh.indices.size(), shadow_map_width, shadow_map_height, sync.frame[current_frame - 1].fence, 0);
			model.DrawModel(window.vulkan.device, window.swap_chain.swap_chain_handle, window.vulkan.graphics_queue, window.render_pass.render_pass_handle, window.framebuffer.framebuffers[current_frame - 1], sync.frame[current_frame - 1].fence, sync.frame[current_frame - 1].image_avaliable_semaphore, sync.frame[current_frame - 1].render_finish_semaphore, window_width, window_height, 0);
			model.PresentModel(window.swap_chain.swap_chain_handle, window.vulkan.present_queue, sync.frame[current_frame - 1].render_finish_semaphore);

			current_frame %= 2;
			current_frame++;
		}
	}
public:
	WindowApp window;
	Model model;
	ShadowMap shadow_map;
	RenderSync sync;
	KeyboardInput input;
};
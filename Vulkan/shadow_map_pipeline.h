#ifndef SHADOW_MAP_PIPELINE
#define SHADOW_MAP_PIPELINE
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
class ShadowMapPipeline
{
public:
	void CreatePipeline(VkDevice& device, std::vector<char>& vertex_shader_code, std::vector<char>& fragment_shader_code, VkVertexInputBindingDescription& binding_description, std::vector<VkVertexInputAttributeDescription>& attribute_descriptions, VkDescriptorSetLayout& descriptor_set_layout, VkRenderPass& render_pass)
	{
		// create vertex shader module
		VkShaderModuleCreateInfo vert_module_info{};
		VkShaderModule vertex_shader_module{};
		vert_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vert_module_info.codeSize = vertex_shader_code.size();
		vert_module_info.pCode = (const uint32_t*)vertex_shader_code.data();

		if (vkCreateShaderModule(device, &vert_module_info, nullptr, &vertex_shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}

		// create fragment shader module
		VkShaderModuleCreateInfo frag_module_info{};
		VkShaderModule fragment_shader_module{};
		frag_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		frag_module_info.codeSize = fragment_shader_code.size();
		frag_module_info.pCode = (const uint32_t*)fragment_shader_code.data();

		if (vkCreateShaderModule(device, &frag_module_info, nullptr, &fragment_shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}

		// create vertex shared stage
		VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
		vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vert_shader_stage_info.module = vertex_shader_module;
		vert_shader_stage_info.pName = "main";

		// create fragment shared stage
		VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
		frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		frag_shader_stage_info.module = fragment_shader_module;
		frag_shader_stage_info.pName = "main";

		// create vertex input info
		VkPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_info.vertexBindingDescriptionCount = 1;
		vertex_input_info.pVertexBindingDescriptions = &binding_description;
		vertex_input_info.vertexAttributeDescriptionCount = attribute_descriptions.size();
		vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

		// create input assembly
		VkPipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = VK_FALSE;

		// create viewportState
		VkPipelineViewportStateCreateInfo viewport_state{};
		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = 1;
		viewport_state.scissorCount = 1;

		// create rasterizator
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// create multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// color blending
		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo color_blending{};
		color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending.logicOpEnable = VK_FALSE;
		color_blending.logicOp = VK_LOGIC_OP_COPY;
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;
		color_blending.blendConstants[0] = 0.0f;
		color_blending.blendConstants[1] = 0.0f;
		color_blending.blendConstants[2] = 0.0f;
		color_blending.blendConstants[3] = 0.0f;

		// create dynamic state
		VkPipelineDynamicStateCreateInfo dynamic_state{};
		std::vector<VkDynamicState> dynamic_states = { VK_DYNAMIC_STATE_VIEWPORT ,VK_DYNAMIC_STATE_SCISSOR };
		dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.dynamicStateCount = dynamic_states.size();
		dynamic_state.pDynamicStates = dynamic_states.data();

		// create pipeline layout
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 1;
		pipeline_layout_info.pSetLayouts = &descriptor_set_layout;

		if (vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkPipelineDepthStencilStateCreateInfo depth_stencil{};
		depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil.depthTestEnable = VK_TRUE;
		depth_stencil.depthWriteEnable = VK_TRUE;
		depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depth_stencil.depthBoundsTestEnable = VK_FALSE;
		depth_stencil.stencilTestEnable = VK_FALSE;

		// create pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		VkPipelineShaderStageCreateInfo shaderStages[] = { vert_shader_stage_info, frag_shader_stage_info };
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertex_input_info;
		pipelineInfo.pInputAssemblyState = &input_assembly;
		pipelineInfo.pViewportState = &viewport_state;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &color_blending;
		pipelineInfo.pDynamicState = &dynamic_state;
		pipelineInfo.layout = pipeline_layout;
		pipelineInfo.renderPass = render_pass;
		pipelineInfo.subpass = 0;
		pipelineInfo.pDepthStencilState = &depth_stencil;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(device, vertex_shader_module, nullptr);
		vkDestroyShaderModule(device, fragment_shader_module, nullptr);
	}
public:
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;
};
#endif // !SHADOW_MAP_PIPELINE


#ifndef SHADOW_MAP_DESCRIPTORS
#define SHADOW_MAP_DESCRIPTORS
#include <vulkan/vulkan.h>
#include <stdexcept>
class ShadowMapDescriptors
{
public:
	void CreateDescriptors(VkDevice& device, VkBuffer& uniform_buffer, VkDeviceSize uniform_buffer_size)
	{
		CreateDescriptorPool(device);
		CreateDescriptorSetLayouts(device);
		AllocateDescriptorsSet(device);
		UpdateDescriptorSets(device, uniform_buffer, uniform_buffer_size);
	}
private:
	void CreateDescriptorPool(VkDevice& device)
	{
		VkDescriptorPoolSize pool_size;
		pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		pool_size.descriptorCount = 1;

		VkDescriptorPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = 1;
		pool_info.pPoolSizes = &pool_size;
		pool_info.maxSets = 1;

		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	void CreateDescriptorSetLayouts(VkDevice& device)
	{
		VkDescriptorSetLayoutBinding descriptor_set_layout_bindings[1] = {};
		descriptor_set_layout_bindings[0].binding = 0;
		descriptor_set_layout_bindings[0].descriptorCount = 1;
		descriptor_set_layout_bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_set_layout_bindings[0].pImmutableSamplers = nullptr;
		descriptor_set_layout_bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = 1;
		layout_info.pBindings = descriptor_set_layout_bindings;

		if (vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}
	void AllocateDescriptorsSet(VkDevice& device)
	{
		VkDescriptorSetAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = descriptor_pool;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &descriptor_set_layout;

		if (vkAllocateDescriptorSets(device, &alloc_info, &descriptor_set) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
	}
	void UpdateDescriptorSets(VkDevice& device, VkBuffer& uniform_buffer, VkDeviceSize uniform_buffer_size)
	{
		VkDescriptorBufferInfo buffers_info[1] = {};
		buffers_info[0].buffer = uniform_buffer;
		buffers_info[0].offset = 0;
		buffers_info[0].range = uniform_buffer_size;

		VkWriteDescriptorSet write_descriptor_sets[1] = {};
		write_descriptor_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[0].dstSet = descriptor_set;
		write_descriptor_sets[0].dstBinding = 0;
		write_descriptor_sets[0].dstArrayElement = 0;
		write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_sets[0].descriptorCount = 1;
		write_descriptor_sets[0].pBufferInfo = &buffers_info[0];

		vkUpdateDescriptorSets(device, 1, write_descriptor_sets, 0, nullptr);
	}
public:
	VkDescriptorSet descriptor_set;
	VkDescriptorSetLayout descriptor_set_layout;
private:
	VkDescriptorPool descriptor_pool;
};
#endif // !SHADOW_MAP_DESCRIPTORS


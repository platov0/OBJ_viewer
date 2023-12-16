#ifndef MODEL_DESCRIPTORS
#define MODEL_DESCRIPTORS
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>
class ModelDescriptors
{
public:
	void CreateDescriptors(VkDevice& device, VkBuffer& uniform_buffer, VkDeviceSize uniform_buffer_size, VkImageView& image_view, VkSampler& image_sampler)
	{
		CreateDescriptorPool(device);
		CreateDescriptorSetLayouts(device);
		AllocateDescriptorsSet(device);
		UpdateDescriptorSets(device, uniform_buffer, uniform_buffer_size, image_view, image_sampler);
	}
private:
	void CreateDescriptorPool(VkDevice& device)
	{
		std::array<VkDescriptorPoolSize, 2> pool_sizes;
		pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		pool_sizes[0].descriptorCount = 1;
		pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		pool_sizes[1].descriptorCount = 1;

		VkDescriptorPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = 2;
		pool_info.pPoolSizes = pool_sizes.data();
		pool_info.maxSets = 1;

		if (vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	void CreateDescriptorSetLayouts(VkDevice& device)
	{
		VkDescriptorSetLayoutBinding uniform_layout_binding{};
		uniform_layout_binding.binding = 0;
		uniform_layout_binding.descriptorCount = 1;
		uniform_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniform_layout_binding.pImmutableSamplers = nullptr;
		uniform_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding texture_layout_binding{};
		texture_layout_binding.binding = 1;
		texture_layout_binding.descriptorCount = 1;
		texture_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		texture_layout_binding.pImmutableSamplers = nullptr;
		texture_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uniform_layout_binding ,texture_layout_binding };
		VkDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = 2;
		layout_info.pBindings = bindings.data();

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
	void UpdateDescriptorSets(VkDevice& device, VkBuffer& uniform_buffer, VkDeviceSize uniform_buffer_size, VkImageView& image_view,VkSampler& image_sampler)
	{
		VkDescriptorBufferInfo buffers_info{};
		buffers_info.buffer = uniform_buffer;
		buffers_info.offset = 0;
		buffers_info.range = uniform_buffer_size;

		VkDescriptorImageInfo image_info{};
		image_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		image_info.imageView = image_view;
		image_info.sampler = image_sampler;

		VkWriteDescriptorSet write_descriptor_sets[2] = {};
		write_descriptor_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[0].dstSet = descriptor_set;
		write_descriptor_sets[0].dstBinding = 0;
		write_descriptor_sets[0].dstArrayElement = 0;
		write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_sets[0].descriptorCount = 1;
		write_descriptor_sets[0].pBufferInfo = &buffers_info;

		write_descriptor_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[1].dstSet = descriptor_set;
		write_descriptor_sets[1].dstBinding = 1;
		write_descriptor_sets[1].dstArrayElement = 0;
		write_descriptor_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_sets[1].descriptorCount = 1;
		write_descriptor_sets[1].pImageInfo = &image_info;

		vkUpdateDescriptorSets(device, 2, write_descriptor_sets, 0, nullptr);
	}
public:
	VkDescriptorSet descriptor_set;
	VkDescriptorSetLayout descriptor_set_layout;
private:
	VkDescriptorPool descriptor_pool;
};
#endif // !SHADOW_MAP_DESCRIPTORS
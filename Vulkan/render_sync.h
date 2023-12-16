#ifndef RENDER_SYNC
#define RENDER_SYNC
#include "vulkan/vulkan.h"
class RenderSync
{
public:
    void CreateSyncPrimitives(VkDevice& device)
    {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(device, &semaphore_info, nullptr, &frame[0].image_avaliable_semaphore);
        vkCreateSemaphore(device, &semaphore_info, nullptr, &frame[1].image_avaliable_semaphore);
        vkCreateSemaphore(device, &semaphore_info, nullptr, &frame[0].render_finish_semaphore);
        vkCreateSemaphore(device, &semaphore_info, nullptr, &frame[1].render_finish_semaphore);

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &fence_info, nullptr, &frame[0].fence);
        vkCreateFence(device, &fence_info, nullptr, &frame[1].fence);
    }
private:
    struct SyncFrame
    {
        VkSemaphore render_finish_semaphore;
        VkSemaphore image_avaliable_semaphore;
        VkFence fence;
    };
public:
    SyncFrame frame[2];
};
#endif // !SHADOW_MAP_SYNC


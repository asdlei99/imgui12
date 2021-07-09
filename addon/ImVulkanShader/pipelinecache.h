#pragma once
#include "platform.h"
#include <vulkan/vulkan.h>
#include "gpu.h"

namespace ImGui 
{
class VulkanDevice;
class PipelineCachePrivate;
class PipelineCache
{
public:
    explicit PipelineCache(const VulkanDevice* _vkdev);

    virtual ~PipelineCache();

    void clear();

    int get_pipeline(const uint32_t* spv_data, size_t spv_data_size, const std::vector<vk_specialization_type>& specializations,
                    uint32_t local_size_x, uint32_t local_size_y, uint32_t local_size_z,
                    VkShaderModule* shader_module,
                    VkDescriptorSetLayout* descriptorset_layout,
                    VkPipelineLayout* pipeline_layout,
                    VkPipeline* pipeline,
                    VkDescriptorUpdateTemplateKHR* descriptor_update_template,
                    ShaderInfo& shader_info) const;

protected:
    int new_pipeline(VkShaderModule shader_module, const ShaderInfo& shader_info, const std::vector<vk_specialization_type>& specializations,
                    VkDescriptorSetLayout* descriptorset_layout,
                    VkPipelineLayout* pipeline_layout,
                    VkPipeline* pipeline,
                    VkDescriptorUpdateTemplateKHR* descriptor_update_template) const;

protected:
    const VulkanDevice* vkdev;

private:
    PipelineCache(const PipelineCache&);
    PipelineCache& operator=(const PipelineCache&);

private:
    PipelineCachePrivate* const d;
};

} // namespace ImGui


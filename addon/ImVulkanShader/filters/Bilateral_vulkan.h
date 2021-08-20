#pragma once
#include <imgui.h>
#include "gpu.h"
#include "pipeline.h"
#include <imgui_mat.h>

namespace ImGui 
{
class IMGUI_API Bilateral_vulkan
{
public:
    Bilateral_vulkan(int gpu = -1);
    ~Bilateral_vulkan();

    // input CPU Buffer and output to RGBA CPU buffer
    void filter(const ImMat& src, ImMat& dst, int ksz, float sigma_spatial, float sigma_color);
    // input CPU Buffer and output to RGBA GPU buffer
    void filter(const ImMat& src, VkMat& dst, int ksz, float sigma_spatial, float sigma_color);
    // input GPU Buffer and output to RGBA CPU buffer
    void filter(const VkMat& src, ImMat& dst, int ksz, float sigma_spatial, float sigma_color);
    // input GPU Buffer and output to RGBA GPU buffer
    void filter(const VkMat& src, VkMat& dst, int ksz, float sigma_spatial, float sigma_color);

public:
    const VulkanDevice* vkdev {nullptr};
    Pipeline * pipe           {nullptr};
    VkCompute * cmd           {nullptr};
    Option opt;

private:
    std::vector<uint32_t> spirv_data;
    void upload_param(const VkMat& src, VkMat& dst, int ksz, float sigma_spatial, float sigma_color);
};
} // namespace ImGui 
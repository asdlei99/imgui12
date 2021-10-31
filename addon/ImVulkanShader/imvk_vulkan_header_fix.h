#pragma once
#include <vulkan/vulkan.h>

// This header contains new structure and function declearation to fix build with old vulkan sdk

#if VK_HEADER_VERSION < 70
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES (VkStructureType)1000094000
typedef enum VkSubgroupFeatureFlagBits
{
    VK_SUBGROUP_FEATURE_BASIC_BIT = 0x00000001,
    VK_SUBGROUP_FEATURE_VOTE_BIT = 0x00000002,
    VK_SUBGROUP_FEATURE_ARITHMETIC_BIT = 0x00000004,
    VK_SUBGROUP_FEATURE_BALLOT_BIT = 0x00000008,
    VK_SUBGROUP_FEATURE_SHUFFLE_BIT = 0x00000010,
    VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT = 0x00000020,
    VK_SUBGROUP_FEATURE_CLUSTERED_BIT = 0x00000040,
    VK_SUBGROUP_FEATURE_QUAD_BIT = 0x00000080,
    VK_SUBGROUP_FEATURE_PARTITIONED_BIT_NV = 0x00000100,
    VK_SUBGROUP_FEATURE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkSubgroupFeatureFlagBits;
typedef VkFlags VkSubgroupFeatureFlags;
typedef struct VkPhysicalDeviceSubgroupProperties
{
    VkStructureType sType;
    void* pNext;
    uint32_t subgroupSize;
    VkShaderStageFlags supportedStages;
    VkSubgroupFeatureFlags supportedOperations;
    VkBool32 quadOperationsInAllStages;
} VkPhysicalDeviceSubgroupProperties;
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES (VkStructureType)1000168000
#define VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT            (VkStructureType)1000168001
typedef struct VkPhysicalDeviceMaintenance3Properties
{
    VkStructureType sType;
    void* pNext;
    uint32_t maxPerSetDescriptors;
    VkDeviceSize maxMemoryAllocationSize;
} VkPhysicalDeviceMaintenance3Properties;
typedef struct VkDescriptorSetLayoutSupport
{
    VkStructureType sType;
    void* pNext;
    VkBool32 supported;
} VkDescriptorSetLayoutSupport;
typedef VkPhysicalDeviceMaintenance3Properties VkPhysicalDeviceMaintenance3PropertiesKHR;
typedef VkDescriptorSetLayoutSupport VkDescriptorSetLayoutSupportKHR;
typedef void(VKAPI_PTR* PFN_vkGetDescriptorSetLayoutSupportKHR)(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
#endif // VK_HEADER_VERSION < 70

#if VK_HEADER_VERSION < 80
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR (VkStructureType)1000177000
typedef struct VkPhysicalDevice8BitStorageFeaturesKHR
{
    VkStructureType sType;
    void* pNext;
    VkBool32 storageBuffer8BitAccess;
    VkBool32 uniformAndStorageBuffer8BitAccess;
    VkBool32 storagePushConstant8;
} VkPhysicalDevice8BitStorageFeaturesKHR;
#define VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR  (VkStructureType)1000109000
#define VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR    (VkStructureType)1000109001
#define VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2_KHR     (VkStructureType)1000109002
#define VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR      (VkStructureType)1000109003
#define VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2_KHR (VkStructureType)1000109004
#define VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO_KHR        (VkStructureType)1000109005
#define VK_STRUCTURE_TYPE_SUBPASS_END_INFO_KHR          (VkStructureType)1000109006
typedef struct VkAttachmentDescription2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkAttachmentDescriptionFlags flags;
    VkFormat format;
    VkSampleCountFlagBits samples;
    VkAttachmentLoadOp loadOp;
    VkAttachmentStoreOp storeOp;
    VkAttachmentLoadOp stencilLoadOp;
    VkAttachmentStoreOp stencilStoreOp;
    VkImageLayout initialLayout;
    VkImageLayout finalLayout;
} VkAttachmentDescription2KHR;
typedef struct VkAttachmentReference2KHR
{
    VkStructureType sType;
    const void* pNext;
    uint32_t attachment;
    VkImageLayout layout;
    VkImageAspectFlags aspectMask;
} VkAttachmentReference2KHR;
typedef struct VkSubpassDescription2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkSubpassDescriptionFlags flags;
    VkPipelineBindPoint pipelineBindPoint;
    uint32_t viewMask;
    uint32_t inputAttachmentCount;
    const VkAttachmentReference2KHR* pInputAttachments;
    uint32_t colorAttachmentCount;
    const VkAttachmentReference2KHR* pColorAttachments;
    const VkAttachmentReference2KHR* pResolveAttachments;
    const VkAttachmentReference2KHR* pDepthStencilAttachment;
    uint32_t preserveAttachmentCount;
    const uint32_t* pPreserveAttachments;
} VkSubpassDescription2KHR;
typedef struct VkSubpassDependency2KHR
{
    VkStructureType sType;
    const void* pNext;
    uint32_t srcSubpass;
    uint32_t dstSubpass;
    VkPipelineStageFlags srcStageMask;
    VkPipelineStageFlags dstStageMask;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkDependencyFlags dependencyFlags;
    int32_t viewOffset;
} VkSubpassDependency2KHR;
typedef struct VkRenderPassCreateInfo2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkRenderPassCreateFlags flags;
    uint32_t attachmentCount;
    const VkAttachmentDescription2KHR* pAttachments;
    uint32_t subpassCount;
    const VkSubpassDescription2KHR* pSubpasses;
    uint32_t dependencyCount;
    const VkSubpassDependency2KHR* pDependencies;
    uint32_t correlatedViewMaskCount;
    const uint32_t* pCorrelatedViewMasks;
} VkRenderPassCreateInfo2KHR;
typedef struct VkSubpassBeginInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    VkSubpassContents contents;
} VkSubpassBeginInfoKHR;

typedef struct VkSubpassEndInfoKHR
{
    VkStructureType sType;
    const void* pNext;
} VkSubpassEndInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateRenderPass2KHR)(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
typedef void(VKAPI_PTR* PFN_vkCmdBeginRenderPass2KHR)(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo);
typedef void(VKAPI_PTR* PFN_vkCmdNextSubpass2KHR)(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo);
typedef void(VKAPI_PTR* PFN_vkCmdEndRenderPass2KHR)(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo);
#endif // VK_HEADER_VERSION < 80

#if VK_HEADER_VERSION < 95
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR (VkStructureType)1000082000
typedef struct VkPhysicalDeviceFloat16Int8FeaturesKHR
{
    VkStructureType sType;
    void* pNext;
    VkBool32 shaderFloat16;
    VkBool32 shaderInt8;
} VkPhysicalDeviceFloat16Int8FeaturesKHR;
#endif // VK_HEADER_VERSION < 95

#if VK_HEADER_VERSION < 97
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT (VkStructureType)1000237000
typedef struct VkPhysicalDeviceMemoryBudgetPropertiesEXT
{
    VkStructureType sType;
    void* pNext;
    VkDeviceSize heapBudget[VK_MAX_MEMORY_HEAPS];
    VkDeviceSize heapUsage[VK_MAX_MEMORY_HEAPS];
} VkPhysicalDeviceMemoryBudgetPropertiesEXT;
#endif // VK_HEADER_VERSION < 97

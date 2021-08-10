#pragma once
#include <vk_mat_shader.h>

#define SHADER_PARAM \
" \n\
layout (push_constant) uniform parameter \n\
{ \n\
    int w; \n\
    int h; \n\
    int cstep; \n\
\n\
    int format; \n\
\n\
	int xksize;	\n\
    int yksize;	\n\
    int xanchor; \n\
    int yanchor; \n\
} p; \
"

#define SHADER_FILTER_COLUMN_MAIN \
" \n\
const int PATCH_PER_BLOCK = 4;\n\
const int HALO_SIZE = 1; \n\
shared sfpvec4 column_shared[16 * (PATCH_PER_BLOCK + HALO_SIZE * 2)][16]; \n\
void main() \n\
{ \n\
    ivec2 size = ivec2(p.w, p.h); \n\
    int x = int(gl_GlobalInvocationID.x); \n\
    if (x >= size.x) \n\
        return; \n\
    int wy = int(gl_WorkGroupID.y); \n\
    int wsy = int(gl_WorkGroupSize.y); \n\
    int lx = int(gl_LocalInvocationID.x); \n\
    int ly = int(gl_LocalInvocationID.y); \n\
    // 纹理范围的全局起点 \n\
    int yStart = wy * (wsy * PATCH_PER_BLOCK) + ly; \n\
    // 填充每个块的上HALO_SIZE,注意每列最前面的那个块取不到纹理数据 \n\
    if (wy > 0) \n\
    { \n\
        // 填充非第一列的上边 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(x, yStart - (HALO_SIZE - j) * wsy, p.w, p.cstep, p.format); \n\
            column_shared[ly + j * wsy][lx] = rgba; \n\
        } \n\
    } \n\
    else \n\
    { \n\
        // 每列最上边 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            int maxIdy = max(0, yStart - (HALO_SIZE - j) * wsy); \n\
            sfpvec4 rgba = load_src_rgba(x, maxIdy, p.w, p.cstep, p.format); \n\
            column_shared[ly + j * wsy][lx] = rgba; \n\
        } \n\
    } \n\
    // 填充正常加下边扩展的块,注意每列最后面的那个块取不到下HALO_SIZE块纹理数据 \n\
    if (wy + 2 < gl_NumWorkGroups.y) \n\
    { \n\
        // 主要导入的数据,一个线程取行上四个位置数据 \n\
        for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(x, yStart + j * wsy, p.w, p.cstep, p.format); \n\
            int y = ly + (HALO_SIZE + j) * wsy; \n\
            column_shared[y][lx] = rgba; \n\
        } \n\
        // 下边的扩展中,还在纹理中 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(x, yStart + (PATCH_PER_BLOCK + j) * wsy, p.w, p.cstep, p.format); \n\
            int y = ly + (PATCH_PER_BLOCK + HALO_SIZE + j) * wsy; \n\
            column_shared[y][lx] = rgba; \n\
        } \n\
    } \n\
    else \n\
    { \n\
        // 每列最后边的一个块 \n\
        for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
        { \n\
            int minIdy = min(size.y - 1, yStart + j * wsy); \n\
            int y = ly + (HALO_SIZE + j) * wsy; \n\
            sfpvec4 rgba = load_src_rgba(x, minIdy, p.w, p.cstep, p.format); \n\
            column_shared[y][lx] = rgba; \n\
        } \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            int minIdy = min(size.y - 1, yStart + (PATCH_PER_BLOCK + j) * wsy); \n\
            int y = ly + (PATCH_PER_BLOCK+HALO_SIZE + j) * wsy; \n\
            sfpvec4 rgba = load_src_rgba(x, minIdy, p.w, p.cstep, p.format); \n\
            column_shared[y][lx] = rgba; \n\
        } \n\
    } \n\
	memoryBarrierShared(); \n\
	barrier(); \n\
    for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
    { \n\
        int y = yStart + j * wsy; \n\
        if (y < size.y) \n\
        { \n\
            sfpvec4 sum = sfpvec4(0); \n\
            for (int k = 0; k < p.yksize; ++k) \n\
            { \n\
                int yy = ly + (HALO_SIZE + j) * wsy - p.yanchor + k; \n\
                sum = sum + column_shared[yy][lx] * sfp(kernel_data[k]); \n\
            } \n\
            store_dst_rgba(sum, x, y, p.w, p.cstep, p.format); \n\
        } \n\
    } \n\
} \
"

#define SHADER_FILTER_ROW_MAIN \
" \n\
const int PATCH_PER_BLOCK = 4;\n\
const int HALO_SIZE = 1; \n\
shared sfpvec4 row_shared[16][16 * (PATCH_PER_BLOCK + HALO_SIZE * 2)]; \n\
void main() \n\
{ \n\
    ivec2 size = ivec2(p.w, p.h); \n\
    int y = int(min(gl_GlobalInvocationID.y, size.y - 1)); \n\
    // 纹理正常范围的全局起点 \n\
    int wx = int(gl_WorkGroupID.x); \n\
    int wsx = int(gl_WorkGroupSize.x); \n\
    int lx = int(gl_LocalInvocationID.x); \n\
    int ly = int(gl_LocalInvocationID.y); \n\
    int xStart = wx * (wsx * PATCH_PER_BLOCK) + lx; \n\
    // 每个线程组填充HALO_SIZE*gl_WorkGroupSize个数据 \n\
    // 填充每个左边HALO_SIZE,需要注意每行左边是没有纹理数据的 \n\
    if (wx > 0) \n\
    { \n\
        //填充非最左边块的左边 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(xStart - (HALO_SIZE - j) * wsx, y, p.w, p.cstep, p.format); \n\
            row_shared[ly][lx + j * wsx] = rgba; \n\
        } \n\
    } \n\
    else \n\
    { \n\
        // 每行最左边 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            int maxIdx = max(0, xStart - (HALO_SIZE - j) * wsx); \n\
            sfpvec4 rgba = load_src_rgba(maxIdx, y, p.w, p.cstep, p.format); \n\
            row_shared[ly][lx + j * wsx] = rgba; \n\
        } \n\
    } \n\
    // 填充中间与右边HALO_SIZE块,注意每行右边的HALO_SIZE块是没有纹理数据的 \n\
    if (wx + 2 < gl_NumWorkGroups.x) \n\
    { \n\
        // 填充中间块 \n\
        for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(xStart + j * wsx, y, p.w, p.cstep, p.format); \n\
            int x = lx + (HALO_SIZE + j) * wsx; \n\
            row_shared[ly][x] = rgba; \n\
        } \n\
        // 右边的扩展中,还在纹理中 \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            sfpvec4 rgba = load_src_rgba(xStart + (PATCH_PER_BLOCK + j) * wsx, y, p.w, p.cstep, p.format); \n\
            int x = lx + (PATCH_PER_BLOCK+HALO_SIZE + j) * wsx; \n\
            row_shared[ly][x] = rgba; \n\
        } \n\
    } \n\
    else \n\
    { \n\
        // 每行右边的一个块 \n\
        for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
        { \n\
            int minIdx = min(size.x - 1, xStart + j * wsx); \n\
            int x = lx + (HALO_SIZE + j) * wsx; \n\
            sfpvec4 rgba = load_src_rgba(minIdx, y, p.w, p.cstep, p.format); \n\
            row_shared[ly][x] = rgba; \n\
        } \n\
        for (int j = 0; j < HALO_SIZE; ++j) \n\
        { \n\
            int minIdx = min(size.x - 1, xStart + (PATCH_PER_BLOCK + j) * wsx); \n\
            int x = lx + (PATCH_PER_BLOCK + HALO_SIZE + j) * wsx; \n\
            sfpvec4 rgba = load_src_rgba(minIdx, y, p.w, p.cstep, p.format); \n\
            row_shared[ly][x] = rgba; \n\
        } \n\
    } \n\
	memoryBarrierShared(); \n\
	barrier(); \n\
    for (int j = 0; j < PATCH_PER_BLOCK; ++j) \n\
    { \n\
        int x = xStart + j * wsx; \n\
        if (x < size.x && gl_GlobalInvocationID.y < size.y) \n\
        { \n\
            sfpvec4 sum = sfpvec4(0); \n\
            for (int k = 0; k < p.xksize; ++k) \n\
            { \n\
                int xx = lx + (HALO_SIZE + j) * wsx - p.xanchor + k; \n\
                sum = sum + row_shared[ly][xx] * sfp(kernel_data[k]); \n\
            } \n\
            store_dst_rgba(sum, x, y, p.w, p.cstep, p.format); \n\
        } \n\
    } \n\
} \
"

static const char FilterColumn_data[] = 
SHADER_HEADER
R"(
layout (local_size_x = 16, local_size_y = 16) in;
layout (binding = 0) readonly buffer src_int8 { uint8_t src_int8_data[]; };
layout (binding = 1) writeonly buffer dst_int8 { uint8_t dst_int8_data[]; };
layout (binding = 2) readonly buffer kernel_float { float kernel_data[]; };
)"
SHADER_PARAM
SHADER_LOAD_SRC_RGBA
SHADER_STORE_DST_RGBA
SHADER_FILTER_COLUMN_MAIN
;

static const char FilterRow_data[] = 
SHADER_HEADER
R"(
layout (local_size_x = 16, local_size_y = 16) in;
layout (binding = 0) readonly buffer src_int8 { uint8_t src_int8_data[]; };
layout (binding = 1) writeonly buffer dst_int8 { uint8_t dst_int8_data[]; };
layout (binding = 2) readonly buffer kernel_float { float kernel_data[]; };
)"
SHADER_PARAM
SHADER_LOAD_SRC_RGBA
SHADER_STORE_DST_RGBA
SHADER_FILTER_ROW_MAIN
;

#include <immat.h>
#include <imgui.h>
#include <imgui_internal.h>

#if __ARM_NEON
#include <arm_neon.h>
#elif __SSE__ || __AVX__
#include <neon2sse.h>
#endif // __ARM_NEON

namespace ImGui
{
void ImMat::get_pixel(int x, int y, ImPixel& color) const
{
    assert(dims == 3 || dims == 2);
    x = std::max(0, std::min(x, w - 1));
    y = std::max(0, std::min(y, h - 1));
    if (dims == 3)
    {
        switch (type)
        {
            case IM_DT_INT8:
                if (c > 0) color.r = (float)at<uint8_t>(x, y, 0) / UINT8_MAX;
                if (c > 1) color.g = (float)at<uint8_t>(x, y, 1) / UINT8_MAX;
                if (c > 2) color.b = (float)at<uint8_t>(x, y, 2) / UINT8_MAX;
                if (c > 3) color.a = (float)at<uint8_t>(x, y, 3) / UINT8_MAX;
            break;
            case IM_DT_INT16:
                if (c > 0) color.r = (float)at<uint16_t>(x, y, 0) / UINT16_MAX;
                if (c > 1) color.g = (float)at<uint16_t>(x, y, 1) / UINT16_MAX;
                if (c > 2) color.b = (float)at<uint16_t>(x, y, 2) / UINT16_MAX;
                if (c > 3) color.a = (float)at<uint16_t>(x, y, 3) / UINT16_MAX;
            break;
            case IM_DT_INT32:
                if (c > 0) color.r = (float)at<uint32_t>(x, y, 0) / (float)UINT32_MAX;
                if (c > 1) color.g = (float)at<uint32_t>(x, y, 1) / (float)UINT32_MAX;
                if (c > 2) color.b = (float)at<uint32_t>(x, y, 2) / (float)UINT32_MAX;
                if (c > 3) color.a = (float)at<uint32_t>(x, y, 3) / (float)UINT32_MAX;
            break;
            case IM_DT_INT64:
                if (c > 0) color.r = (float)at<uint64_t>(x, y, 0) / (float)UINT64_MAX;
                if (c > 1) color.g = (float)at<uint64_t>(x, y, 1) / (float)UINT64_MAX;
                if (c > 2) color.b = (float)at<uint64_t>(x, y, 2) / (float)UINT64_MAX;
                if (c > 3) color.a = (float)at<uint64_t>(x, y, 3) / (float)UINT64_MAX;
            break;
            case IM_DT_FLOAT16:
                if (c > 0) color.r = im_float16_to_float32(at<uint16_t>(x, y, 0));
                if (c > 1) color.g = im_float16_to_float32(at<uint16_t>(x, y, 1));
                if (c > 2) color.b = im_float16_to_float32(at<uint16_t>(x, y, 2));
                if (c > 3) color.a = im_float16_to_float32(at<uint16_t>(x, y, 3));
            break;
            case IM_DT_FLOAT32:
                if (c > 0) color.r = at<float>(x, y, 0);
                if (c > 1) color.g = at<float>(x, y, 1);
                if (c > 2) color.b = at<float>(x, y, 2);
                if (c > 3) color.a = at<float>(x, y, 3);
            break;
            case IM_DT_FLOAT64:
                if (c > 0) color.r = (float)at<double>(x, y, 0);
                if (c > 1) color.g = (float)at<double>(x, y, 1);
                if (c > 2) color.b = (float)at<double>(x, y, 2);
                if (c > 3) color.a = (float)at<double>(x, y, 3);
            break;
            default: break;
        }
    }
    else
    {
        switch (type)
        {
            case IM_DT_INT8: color.r = (float)at<uint8_t>(x, y) / UINT8_MAX; break;
            case IM_DT_INT16: color.r = (float)at<uint16_t>(x, y) / UINT16_MAX; break;
            case IM_DT_INT32: color.r = (float)at<uint32_t>(x, y) / (float)UINT32_MAX; break;
            case IM_DT_INT64: color.r = (float)at<uint64_t>(x, y) / (float)UINT64_MAX; break;
            case IM_DT_FLOAT16: color.r = im_float16_to_float32(at<uint16_t>(x, y)); break;
            case IM_DT_FLOAT32: color.r = at<float>(x, y); break;
            case IM_DT_FLOAT64: color.r = (float)at<double>(x, y); break;
            default: break;
        }
    }
}

void ImMat::get_pixel(ImPoint p, ImPixel& color) const
{
    get_pixel((int)p.x, (int)p.y, color);
}

ImPixel ImMat::get_pixel(int x, int y) const
{
    ImPixel color;
    get_pixel(x, y, color);
    return color;
}

ImPixel ImMat::get_pixel(ImPoint p) const
{
    return get_pixel((int)p.x, (int)p.y);
}

void ImMat::set_pixel(int x, int y, ImPixel color)
{
    assert(dims == 3 || dims == 2);
    x = std::max(0, std::min(x, w - 1));
    y = std::max(0, std::min(y, h - 1));
    color.r = std::max(0.f, std::min(color.r, 1.f));
    color.g = std::max(0.f, std::min(color.g, 1.f));
    color.b = std::max(0.f, std::min(color.b, 1.f));
    color.a = std::max(0.f, std::min(color.a, 1.f));
    if (dims == 3)
    {
        switch (type)
        {
            case IM_DT_INT8:
                if (c > 0) at<uint8_t>(x, y, 0) = color.r * UINT8_MAX;
                if (c > 1) at<uint8_t>(x, y, 1) = color.g * UINT8_MAX;
                if (c > 2) at<uint8_t>(x, y, 2) = color.b * UINT8_MAX;
                if (c > 3) at<uint8_t>(x, y, 3) = color.a * UINT8_MAX;
            break;
            case IM_DT_INT16:
                if (c > 0) at<uint16_t>(x, y, 0) = color.r * UINT16_MAX;
                if (c > 1) at<uint16_t>(x, y, 1) = color.g * UINT16_MAX;
                if (c > 2) at<uint16_t>(x, y, 2) = color.b * UINT16_MAX;
                if (c > 3) at<uint16_t>(x, y, 3) = color.a * UINT16_MAX;
            break;
            case IM_DT_INT32:
                if (c > 0) at<uint32_t>(x, y, 0) = color.r * (float)UINT32_MAX;
                if (c > 1) at<uint32_t>(x, y, 1) = color.g * (float)UINT32_MAX;
                if (c > 2) at<uint32_t>(x, y, 2) = color.b * (float)UINT32_MAX;
                if (c > 3) at<uint32_t>(x, y, 3) = color.a * (float)UINT32_MAX;
            break;
            case IM_DT_INT64:
                if (c > 0) at<uint64_t>(x, y, 0) = color.r * (float)UINT64_MAX;
                if (c > 1) at<uint64_t>(x, y, 1) = color.g * (float)UINT64_MAX;
                if (c > 2) at<uint64_t>(x, y, 2) = color.b * (float)UINT64_MAX;
                if (c > 3) at<uint64_t>(x, y, 3) = color.a * (float)UINT64_MAX;
            break;
            case IM_DT_FLOAT16:
                if (c > 0) at<uint16_t>(x, y, 0) = im_float32_to_float16(color.r);
                if (c > 1) at<uint16_t>(x, y, 1) = im_float32_to_float16(color.g);
                if (c > 2) at<uint16_t>(x, y, 2) = im_float32_to_float16(color.b);
                if (c > 3) at<uint16_t>(x, y, 3) = im_float32_to_float16(color.a);
            break;
            case IM_DT_FLOAT32:
                if (c > 0) at<float>(x, y, 0) = color.r;
                if (c > 1) at<float>(x, y, 1) = color.g;
                if (c > 2) at<float>(x, y, 2) = color.b;
                if (c > 3) at<float>(x, y, 3) = color.a;
            break;
            case IM_DT_FLOAT64:
                if (c > 0) at<double>(x, y, 0) = (double)color.r;
                if (c > 1) at<double>(x, y, 1) = (double)color.g;
                if (c > 2) at<double>(x, y, 2) = (double)color.b;
                if (c > 3) at<double>(x, y, 3) = (double)color.a;
            break;
            default: break;
        }
    }
    else
    {
        switch (type)
        {
            case IM_DT_INT8: at<uint8_t>(x, y) = color.r * UINT8_MAX; break;
            case IM_DT_INT16: at<uint16_t>(x, y) = color.r * UINT16_MAX; break;
            case IM_DT_INT32: at<uint32_t>(x, y) = color.r * (float)UINT32_MAX; break;
            case IM_DT_INT64: at<uint64_t>(x, y) = color.r * (float)UINT64_MAX; break;
            case IM_DT_FLOAT16: at<uint16_t>(x, y) = im_float32_to_float16(color.r); break;
            case IM_DT_FLOAT32: at<float>(x, y) = color.r; break;
            case IM_DT_FLOAT64: at<double>(x, y) = (double)color.r; break;
            default: break;
        }
    }
}

void ImMat::set_pixel(ImPoint p, ImPixel color)
{
    set_pixel((int)p.x, (int)p.y, color);
}

void ImMat::alphablend(int x, int y, float alpha, ImPixel color)
{
    switch (type)
    {
        case IM_DT_INT8:
        {
            float alpha_org = c > 2 ? at<uint8_t>(x, y, 3) / (float)UINT8_MAX : 1;
            if (c > 0) at<uint8_t>(x, y, 0) = at<uint8_t>(x, y, 0) * (1 - alpha) + color.r * alpha * UINT8_MAX;
            if (c > 1) at<uint8_t>(x, y, 1) = at<uint8_t>(x, y, 1) * (1 - alpha) + color.g * alpha * UINT8_MAX;
            if (c > 2) at<uint8_t>(x, y, 2) = at<uint8_t>(x, y, 2) * (1 - alpha) + color.b * alpha * UINT8_MAX;
            if (c > 3) at<uint8_t>(x, y, 3) = (uint8_t)(CLAMP(color.a * alpha + alpha_org, 0.f, 1.f) * UINT8_MAX);
        }
        break;
        case IM_DT_INT16:
        {
            float alpha_org = c > 2 ? at<uint16_t>(x, y, 3) / (float)UINT16_MAX : 1;
            if (c > 0) at<uint16_t>(x, y, 0) = at<uint16_t>(x, y, 0) * (1 - alpha) + color.r * alpha * UINT16_MAX;
            if (c > 1) at<uint16_t>(x, y, 1) = at<uint16_t>(x, y, 1) * (1 - alpha) + color.g * alpha * UINT16_MAX;
            if (c > 2) at<uint16_t>(x, y, 2) = at<uint16_t>(x, y, 2) * (1 - alpha) + color.b * alpha * UINT16_MAX;
            if (c > 3) at<uint16_t>(x, y, 3) = (uint16_t)(CLAMP(color.a * alpha + alpha_org, 0.f, 1.f) * UINT16_MAX);
        }
        break;
        case IM_DT_INT32:
        {
            float alpha_org = c > 2 ? at<uint16_t>(x, y, 3) / (float)UINT32_MAX : 1;
            if (c > 0) at<uint32_t>(x, y, 0) = at<uint32_t>(x, y, 0) * (1 - alpha) + color.r * alpha * (float)UINT32_MAX;
            if (c > 1) at<uint32_t>(x, y, 1) = at<uint32_t>(x, y, 1) * (1 - alpha) + color.g * alpha * (float)UINT32_MAX;
            if (c > 2) at<uint32_t>(x, y, 2) = at<uint32_t>(x, y, 2) * (1 - alpha) + color.b * alpha * (float)UINT32_MAX;
            if (c > 3) at<uint32_t>(x, y, 3) = (uint32_t)(CLAMP(color.a * alpha + alpha_org, 0.f, 1.f) * (float)UINT32_MAX);
        }
        break;
        case IM_DT_INT64:
        {
            float alpha_org = c > 2 ? at<uint64_t>(x, y, 3) / (float)UINT64_MAX : 1;
            if (c > 0) at<uint64_t>(x, y, 0) = at<uint64_t>(x, y, 0) * (1 - alpha) + color.r * alpha * (float)UINT64_MAX;
            if (c > 1) at<uint64_t>(x, y, 1) = at<uint64_t>(x, y, 1) * (1 - alpha) + color.g * alpha * (float)UINT64_MAX;
            if (c > 2) at<uint64_t>(x, y, 2) = at<uint64_t>(x, y, 2) * (1 - alpha) + color.b * alpha * (float)UINT64_MAX;
            if (c > 3) at<uint64_t>(x, y, 3) = (uint64_t)(CLAMP(color.a * alpha + alpha_org, 0.f, 1.f) * (float)UINT64_MAX);
        }
        break;
        case IM_DT_FLOAT16:
            // TODO::Dicky add FLOAT16 alphablend
        break;
        case IM_DT_FLOAT32:
        {
            float alpha_org = c > 2 ? at<float>(x, y, 3) : 1;
            if (c > 0) at<float>(x, y, 0) = at<float>(x, y, 0) * (1 - alpha) + color.r * alpha;
            if (c > 1) at<float>(x, y, 1) = at<float>(x, y, 1) * (1 - alpha) + color.g * alpha;
            if (c > 2) at<float>(x, y, 2) = at<float>(x, y, 2) * (1 - alpha) + color.b * alpha;
            if (c > 3) at<float>(x, y, 3) = CLAMP(color.a * alpha + alpha_org, 0.f, 1.f);
        }
        break;
        case IM_DT_FLOAT64:
        {
            double alpha_org = c > 2 ? at<double>(x, y, 3) : 1;
            if (c > 0) at<double>(x, y, 0) = at<double>(x, y, 0) * (1 - alpha) + color.r * alpha;
            if (c > 1) at<double>(x, y, 1) = at<double>(x, y, 1) * (1 - alpha) + color.g * alpha;
            if (c > 2) at<double>(x, y, 2) = at<double>(x, y, 2) * (1 - alpha) + color.b * alpha;
            if (c > 3) at<double>(x, y, 3) = (double)(CLAMP(color.a * alpha + (float)alpha_org, 0.f, 1.f));
        }
        break;
        default: break;
    }
}

void ImMat::alphablend(int x, int y, ImPixel color)
{
    switch (type)
    {
        case IM_DT_INT8:
        {
            //float alpha_org = c > 2 ? at<uint8_t>(x, y, 3) / (float)UINT8_MAX : 1;
            float alpha = color.a * UINT8_MAX;
            if (c > 0) at<uint8_t>(x, y, 0) = at<uint8_t>(x, y, 0) * (1 - color.a) + color.r * alpha;
            if (c > 1) at<uint8_t>(x, y, 1) = at<uint8_t>(x, y, 1) * (1 - color.a) + color.g * alpha;
            if (c > 2) at<uint8_t>(x, y, 2) = at<uint8_t>(x, y, 2) * (1 - color.a) + color.b * alpha;
            if (c > 3) at<uint8_t>(x, y, 3) = UINT8_MAX; //(uint8_t)(CLAMP(color.a + alpha_org, 0.f, 1.f) * UINT8_MAX);
        }
        break;
        case IM_DT_INT16:
        {
            //float alpha_org = c > 2 ? at<uint16_t>(x, y, 3) / (float)UINT16_MAX : 1;
            float alpha = color.a * UINT16_MAX;
            if (c > 0) at<uint16_t>(x, y, 0) = at<uint16_t>(x, y, 0) * (1 - color.a) + color.r * alpha;
            if (c > 1) at<uint16_t>(x, y, 1) = at<uint16_t>(x, y, 1) * (1 - color.a) + color.g * alpha;
            if (c > 2) at<uint16_t>(x, y, 2) = at<uint16_t>(x, y, 2) * (1 - color.a) + color.b * alpha;
            if (c > 3) at<uint16_t>(x, y, 3) = UINT16_MAX;//(uint16_t)(CLAMP(color.a + alpha_org, 0.f, 1.f) * UINT16_MAX);
        }
        break;
        case IM_DT_INT32:
        {
            //float alpha_org = c > 2 ? at<uint16_t>(x, y, 3) / (float)UINT32_MAX : 1;
            float alpha = color.a * (float)UINT32_MAX;
            if (c > 0) at<uint32_t>(x, y, 0) = at<uint32_t>(x, y, 0) * (1 - color.a) + color.r * alpha;
            if (c > 1) at<uint32_t>(x, y, 1) = at<uint32_t>(x, y, 1) * (1 - color.a) + color.g * alpha;
            if (c > 2) at<uint32_t>(x, y, 2) = at<uint32_t>(x, y, 2) * (1 - color.a) + color.b * alpha;
            if (c > 3) at<uint32_t>(x, y, 3) = UINT32_MAX;//(uint32_t)(CLAMP(color.a + alpha_org, 0.f, 1.f) * (float)UINT32_MAX);
        }
        break;
        case IM_DT_INT64:
        {
            //float alpha_org = c > 2 ? at<uint64_t>(x, y, 3) / (float)UINT64_MAX : 1;
            float alpha = color.a * (float)UINT64_MAX;
            if (c > 0) at<uint64_t>(x, y, 0) = at<uint64_t>(x, y, 0) * (1 - color.a) + color.r * alpha;
            if (c > 1) at<uint64_t>(x, y, 1) = at<uint64_t>(x, y, 1) * (1 - color.a) + color.g * alpha;
            if (c > 2) at<uint64_t>(x, y, 2) = at<uint64_t>(x, y, 2) * (1 - color.a) + color.b * alpha;
            if (c > 3) at<uint64_t>(x, y, 3) = UINT64_MAX;//(uint64_t)(CLAMP(color.a + alpha_org, 0.f, 1.f) * (float)UINT64_MAX);
        }
        break;
        case IM_DT_FLOAT16:
            if (c > 0) at<uint16_t>(x, y, 0) = im_float32_to_float16(im_float16_to_float32(at<uint16_t>(x, y, 0)) * (1 - color.a) + color.r * color.a);
            if (c > 1) at<uint16_t>(x, y, 1) = im_float32_to_float16(im_float16_to_float32(at<uint16_t>(x, y, 1)) * (1 - color.a) + color.g * color.a);
            if (c > 2) at<uint16_t>(x, y, 2) = im_float32_to_float16(im_float16_to_float32(at<uint16_t>(x, y, 2)) * (1 - color.a) + color.b * color.a);
            if (c > 3) at<uint16_t>(x, y, 3) = im_float32_to_float16(1.f);//CLAMP(color.a + alpha_org, 0.f, 1.f);
        break;
        case IM_DT_FLOAT32:
        {
            //float alpha_org = c > 2 ? at<float>(x, y, 3) : 1;
            if (c > 0) at<float>(x, y, 0) = at<float>(x, y, 0) * (1 - color.a) + color.r * color.a;
            if (c > 1) at<float>(x, y, 1) = at<float>(x, y, 1) * (1 - color.a) + color.g * color.a;
            if (c > 2) at<float>(x, y, 2) = at<float>(x, y, 2) * (1 - color.a) + color.b * color.a;
            if (c > 3) at<float>(x, y, 3) = 1.f;//CLAMP(color.a + alpha_org, 0.f, 1.f);
        }
        break;
        case IM_DT_FLOAT64:
        {
            //double alpha_org = c > 2 ? at<double>(x, y, 3) : 1;
            if (c > 0) at<double>(x, y, 0) = at<double>(x, y, 0) * (1 - color.a) + color.r * color.a;
            if (c > 1) at<double>(x, y, 1) = at<double>(x, y, 1) * (1 - color.a) + color.g * color.a;
            if (c > 2) at<double>(x, y, 2) = at<double>(x, y, 2) * (1 - color.a) + color.b * color.a;
            if (c > 3) at<double>(x, y, 3) = 1.0; //(double)(CLAMP(color.a + alpha_org, 0.0, 1.0));
        }
        break;
        default: break;
    }
}

void ImMat::draw_line(float x1, float y1, float x2, float y2, float t, ImPixel color)
{
    assert(dims == 3);

    int _x0 = CLAMP((int)floorf(fminf(x1, x2) - t), 0, w - 1);
    int _x1 = CLAMP((int) ceilf(fmaxf(x1, x2) + t), 0, w - 1);
    int _y0 = CLAMP((int)floorf(fminf(y1, y2) - t), 0, h - 1);
    int _y1 = CLAMP((int) ceilf(fmaxf(y1, y2) + t), 0, h - 1);
    #pragma omp parallel for num_threads(OMP_THREADS)
    for (int y = _y0; y <= _y1; y++)
    {
        for (int x = _x0; x <= _x1; x++)
        {
            // capsuleSDF
            float pax = (float)x - x1, pay = (float)y - y1, bax = x2 - x1, bay = y2 - y1;
            float _h = CLAMP((pax * bax + pay * bay) / (bax * bax + bay * bay), 0.0f, 1.0f);
            float dx = pax - bax * _h, dy = pay - bay * _h;
            float sdf = sqrtf(dx * dx + dy * dy) - t;
            float alpha = CLAMP(0.5f - sdf, 0.f, 1.f);
            alphablend(x, y, alpha, color);
        }
    }
}

void ImMat::draw_line(ImPoint p1, ImPoint p2, float t, ImPixel color)
{
    draw_line(p1.x, p1.y, p2.x, p2.y, t, color);
}

void ImMat::draw_line(float x1, float y1, float x2, float y2, ImPixel color, int weight)
{
    // Bresenham
    x1 = CLAMP(x1, 0.f, w - 1.f);
    x2 = CLAMP(x2, 0.f, w - 1.f);
    y1 = CLAMP(y1, 0.f, h - 1.f);
    y2 = CLAMP(y2, 0.f, h - 1.f);
    int x = x1;
	int y = y1;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int s1 = x2 > x1 ? 1 : -1;
	int s2 = y2 > y1 ? 1 : -1;

	char interchange = 0;
	if (dy > dx)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;
	}

	int p = 2 * dy - dx;
	for(int i = 0; i < dx; i++)
	{
        for (int c = 0; c < weight; c++)
        {
            for (int r = 0; r < weight; r++)
            {
                alphablend(x+c, y+r, color);
            }
        }
		if (p >= 0)
		{
			if (!interchange)
				y += s2;
			else
				x += s1;
			p -= 2 * dx;
		}
		if (!interchange)
			x += s1;
		else
			y += s2;
		p += 2 * dy;
	}
}

void ImMat::draw_line(ImPoint p1, ImPoint p2, ImPixel color, int weight)
{
    draw_line(p1.x, p1.y, p2.x, p2.y, color, weight);
}

void ImMat::draw_rectangle(float x1, float y1, float x2, float y2, ImPixel color, int weight)
{
    draw_line(x1, y1, x1, y2, color, weight);
    draw_line(x1, y2, x2, y2, color, weight);
    draw_line(x2, y2, x2, y1, color, weight);
    draw_line(x2, y1, x1, y1, color, weight);
}

void ImMat::draw_rectangle(ImPoint p1, ImPoint p2, ImPixel color, int weight)
{
    draw_rectangle(p1.x, p1.y, p2.x, p2.y, color, weight);
}

void ImMat::draw_rectangle(float x1, float y1, float x2, float y2, float t, ImPixel color)
{
    draw_line(x1, y1, x1, y2, t, color);
    draw_line(x1, y2, x2, y2, t, color);
    draw_line(x2, y2, x2, y1, t, color);
    draw_line(x2, y1, x1, y1, t, color);
}

void ImMat::draw_rectangle(ImPoint p1, ImPoint p2, float t, ImPixel color)
{
    draw_rectangle(p1.x, p1.y, p2.x, p2.y, t, color);
}

void ImMat::draw_circle(float x1, float y1, float r, ImPixel color)
{
    // Bresenham circle
    float x = 0, y = r;
	float p = 3 - (2 * r);
	while (x <= y)
	{
        set_pixel(x1 + x, y1 + y, color);
        set_pixel(x1 - x, y1 + y, color);
        set_pixel(x1 + x, y1 - y, color);
        set_pixel(x1 - x, y1 - y, color);
        set_pixel(x1 + y, y1 + x, color);
		set_pixel(x1 + y, y1 - x, color);
		set_pixel(x1 - y, y1 + x, color);
		set_pixel(x1 - y, y1 - x, color);
		x = x + 1;
		if (p < 0)
			p = p + 4 * x + 6;
		else
		{
			p = p + 4 * (x - y) + 10;
			y = y - 1;
        }
    }
}

void ImMat::draw_circle(ImPoint p, float r, ImPixel color)
{
    draw_circle(p.x, p.y, r, color);
}

void ImMat::draw_circle_filled(float x, float y, float r, ImPixel color)
{
    // Bresenham circle
    int tx = 0, ty = r, d = 3 - 2 * r, i;
	while( tx < ty)
	{
		for (i = x - ty; i <= x + ty; i++)
		{
			set_pixel(i, y - tx, color);
			if (tx != 0) set_pixel(i, y + tx, color);
		}

		if (d < 0) d += 4 * tx + 6;
		else
		{
			for (i = x - tx; i <= x + tx; i++)
			{
				set_pixel(i, y - ty, color);
				set_pixel(i, y + ty, color);
			}
			d += 4 * (tx - ty) + 10, ty--;
		}
		tx++;
	}

	if (tx == ty)
    {
		for (i = x - ty; i <= x + ty; i++)
		{
			set_pixel(i, y - tx, color);
			set_pixel(i, y + tx, color);
		}
    }
}

void ImMat::draw_circle_filled(ImPoint p, float r, ImPixel color)
{
    draw_circle_filled(p.x, p.y, r, color);
}

void ImMat::draw_circle(float x1, float y1, float r, float t, ImPixel color)
{
    float perimeter = 2 * M_PI * r;
    int num_segments = perimeter / 8 / t;
    const float a_max = (M_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    const float a_min = 0;
    float x, y, x0, y0, _x, _y;
    num_segments--;
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        x = x1 + cos(a) * r;
        y = y1 + sin(a) * r;
        if (i == 0)
        {
            x0 = _x = x;
            y0 = _y = y;
        }
        else
        {
            draw_line(_x, _y, x, y, t, color);
            _x = x;
            _y = y;
        }
    }
    draw_line(x, y, x0, y0, t, color);
}

void ImMat::draw_circle(ImPoint p, float r, float t, ImPixel color)
{
    draw_circle(p.x, p.y, r, t, color);
}

void ImMat::draw_circle(float x1, float y1, float r, float t, std::function<ImPixel(float)> const &color)
{
    float perimeter = 2 * M_PI * r;
    int num_segments = perimeter / 8 / t;
    const float a_max = (M_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    const float a_min = 0;
    float x, y, x0, y0, _x, _y;
    num_segments--;
    for (int i = 0; i <= num_segments; i++)
    {
        float const t0 = fmodf(((float)(num_segments - i)) / ((float)num_segments), 1.0f);
        ImPixel uCol = color(t0);
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        x = x1 + cos(a) * r;
        y = y1 + sin(a) * r;
        if (i == 0)
        {
            x0 = _x = x;
            y0 = _y = y;
        }
        else
        {
            draw_line(_x, _y, x, y, t, uCol);
            _x = x;
            _y = y;
        }
    }
    draw_line(x, y, x0, y0, t, color(0));
}

void ImMat::draw_circle(ImPoint p, float r, float t, std::function<ImPixel(float)> const &color)
{
    draw_circle(p.x, p.y, r, t, color);
}

ImMat ImMat::blur(int kernel_size, float sigma)
{
    assert(device == IM_DD_CPU);
    assert(w > 0 && h > 0);
    std::vector<std::vector<double>> kernel(kernel_size, std::vector<double>(kernel_size));
    double sum = 0.0;
    int halfSize = kernel_size / 2;
    for (int i = -halfSize; i <= halfSize; i++) {
        for (int j = -halfSize; j <= halfSize; j++) {
            double exponent = -(i * i + j * j) / (2.0 * sigma * sigma);
            double value = (1.0 / (2.0 * 3.14159 * sigma * sigma)) * exp(exponent);
            kernel[i + halfSize][j + halfSize] = value;
            sum += value;
        }
    }
    // normaliza kernel
    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            kernel[i][j] /= sum;
        }
    }

    ImGui::ImMat dst;
    dst.create_type(w, h, c, type);
    dst.elempack = elempack;
    //Gaussian blur
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            //double sum = 0.0;
            ImPixel sum(0, 0, 0, 0);
            for (int k = -halfSize; k <= halfSize; k++) {
                for (int l = -halfSize; l <= halfSize; l++) {
                    int rowIndex = std::min(std::max(i + k, 0), (int)h - 1);
                    int colIndex = std::min(std::max(j + l, 0), (int)w - 1);

                    double weight = kernel[k + halfSize][l + halfSize];
                    auto p = get_pixel(colIndex, rowIndex);
                    sum = sum + p * weight;
                }
            }
            dst.set_pixel(j, i, sum);
        }
    }
    return dst;
}

ImMat ImMat::adaptive_threshold(float maxValue, int kernel_size, float delta)
{
    assert(device == IM_DD_CPU);
    assert(w > 0 && h > 0);
    ImGui::ImMat dst;
    dst.create_type(w, h, c, type);
    dst.elempack = elempack;
    maxValue = std::max(0.f, std::min(maxValue, 1.0f));
    delta = std::max(0.f, std::min(delta, 1.0f));
    ImMat mean = blur(kernel_size);
    int idelta = std::ceil(delta * 255);
    uint8_t tab[768];
    for(int i = 0; i < 768; i++ )
        tab[i] = (uint8_t)(i - 255 > -idelta ? maxValue * 255 : 0);

    for(int i = 0; i < h; i++ )
    {
        for(int j = 0; j < w; j++ )
        {
            auto ps = get_pixel(j, i);
            auto ms = mean.get_pixel(j, i);
            ImPixel ds;
            ds.r = tab[(int)(ps.r * 255 - ms.r * 255) + 255] / 255.f;
            ds.g = tab[(int)(ps.g * 255 - ms.g * 255) + 255] / 255.f;
            ds.b = tab[(int)(ps.b * 255 - ms.b * 255) + 255] / 255.f;
            ds.a = ps.a;
            dst.set_pixel(j, i, ds);
        }
    }
    return dst;
}

ImMat ImMat::lowpass(float lambda)
{
    assert(device == IM_DD_CPU);
    assert(dims == 2);
    assert(w > 0 && h > 0);
    ImMat m = clone();
    if (lambda < FLT_EPSILON)
        return m;
    float B = 1 + 2 / (lambda * lambda);
    float c = B - sqrt(B * B - 1);
    float d = 1 - c;
    #pragma omp parallel for num_threads(OMP_THREADS)
    for (int y = 0; y < h; y++)
    {
        /* apply low-pass filter to row y */
        /* left-to-right */
        float f = 0, g = 0;
        for (int x = 0; x < w; x++)
        {
            f = f * c + (float)m.at<uint8_t>(x, y) * d;
            g = g * c + f * d;
            m.at<uint8_t>(x, y) = (uint8_t)g;
        }
        /* right-to-left */
        for (int x = w - 1; x >= 0; x--)
        {
            f = f * c + (float)m.at<uint8_t>(x, y) * d;
            g = g * c + f * d;
            m.at<uint8_t>(x, y) = (uint8_t)g;
        }

        /* left-to-right mop-up */
        for (int x = 0; x < w; x++)
        {
            f = f * c;
            g = g * c + f * d;
            if (f + g < 1 / 255.0) break;
            m.at<uint8_t>(x, y) = (uint8_t)((float)m.at<uint8_t>(x, y) + g);
        }
    }
    #pragma omp parallel for num_threads(OMP_THREADS)
    for (int x = 0; x < w; x++)
    {
        /* apply low-pass filter to column x */
        /* bottom-to-top */
        float f = 0, g = 0;
        for (int y = 0; y < h; y++)
        {
            f = f * c + (float)m.at<uint8_t>(x, y) * d;
            g = g * c + f * d;
            m.at<uint8_t>(x, y) = (uint8_t)g;
        }

        /* top-to-bottom */
        for (int y = h - 1; y >= 0; y--)
        {
            f = f * c + (float)m.at<uint8_t>(x, y) * d;
            g = g * c + f * d;
            m.at<uint8_t>(x, y) = (uint8_t)g;
        }

        /* bottom-to-top mop-up */
        for (int y = 0; y < h; y++)
        {
            f = f * c;
            g = g * c + f * d;
            if (f + g < 1 / 255.0) break;
            m.at<uint8_t>(x, y) = (uint8_t)((float)m.at<uint8_t>(x, y) + g);
        }
    }
    return m;
}

ImMat ImMat::highpass(float lambda)
{
    assert(device == IM_DD_CPU);
    assert(dims == 2);
    /* apply lowpass filter to the copy */
    ImMat m = lowpass(lambda);
    if (lambda < FLT_EPSILON)
        return m;
    /* subtract copy from original */
    #pragma omp parallel for num_threads(OMP_THREADS)
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float f = (float)at<uint8_t>(x, y);
            f -= (float)m.at<uint8_t>(x, y);
            f += 128; /* normalize! */
            m.at<uint8_t>(x, y) = (uint8_t)f;
        }
    }
    return m;
}

ImMat ImMat::threshold(float thres)
{
    assert(device == IM_DD_CPU);
    assert(dims == 2);
    ImMat m = clone();
    float _thres = thres * 255;
    #pragma omp parallel for num_threads(OMP_THREADS)
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float p = (float)at<uint8_t>(x, y);
            m.at<uint8_t>(x, y) = p < _thres ? 0 : 255;
        }
    }
    return ImMat();
}

ImMat ImMat::resize(float _w, float _h, int interpolate) const
{
    assert(device == IM_DD_CPU);
    assert(dims == 2 || dims == 3);
    assert(w > 0 && h > 0 && _w > 0 && _h > 0);
    ImMat m((int)(_w), (int)(_h), (int)c, (size_t)elemsize, elempack);
    double scale_x = (double)w / _w;
	double scale_y = (double)h / _h;
    ImPixel p00, p01, p10, p11, av;

    for (int j = 0; j < _h; ++j)
	{
		float fy = (float)((j + 0.5) * scale_y - 0.5);
		int sy = std::floor(fy);
		fy -= sy;
		sy = std::min(sy, h - 2);
		sy = std::max(0, sy);
        const auto mw = m.w;
        #pragma omp parallel for num_threads(OMP_THREADS)
		for (int i = 0; i < mw; ++i)
		{
			float fx = (float)((i + 0.5) * scale_x - 0.5);
			int sx = std::floor(fx);
			fx -= sx;

			if (sx < 0) {
				fx = 0, sx = 0;
			}
			if (sx >= w - 1) {
				fx = 0, sx = w - 2;
			}
            if (interpolate == 0)
            {
                p00 = get_pixel(sx, sy);
                p01 = get_pixel(sx, sy + 1);
                p10 = get_pixel(sx + 1, sy);
                p11 = get_pixel(sx + 1, sy + 1);
                av = p00 * (1.f - fx) * (1.f - fy) + 
                    p01 * (1.f - fx) * fy +
                    p10 * fx * (1.f - fy) +
                    p11 * fx * fy;
                av.a = 1;
            }
            else
            {
                av = get_pixel(sx, sy);
            }
            m.set_pixel(i, j, av);
		}
	}
    return m;
}

void ImMat::copy_to(ImMat & mat, ImPoint offset, float alpha)
{
    // assert mat same as this
    assert(device == IM_DD_CPU);
    assert(!empty() && !mat.empty());
    assert(offset.x + w >= 0 && offset.y + h >= 0 &&
            offset.x < mat.w && offset.y < mat.h);
    ImSize size;
    ImPoint offset_src;
    ImPoint offset_dst;
    if (offset.x < 0)
    {
        offset_src.x = -offset.x;
        offset_dst.x = 0;
        size.w = std::min(w + (int)offset.x, mat.w);
    }
    else
    {
        offset_src.x = 0;
        offset_dst.x = offset.x;
        size.w = std::min(mat.w - (int)offset.x, w);
    }
    if (offset.y < 0)
    {
        offset_src.y = -offset.y;
        offset_dst.y = 0;
        size.h = std::min(h + (int)offset.y, mat.h);
    }
    else
    {
        offset_src.y = 0;
        offset_dst.y = offset.y;
        size.h = std::min(mat.h - (int)offset.y, h);
    }
    for (int i = 0; i < size.h; i++)
    {
        for (int j = 0; j < size.w; j++)
        {
            auto src_color = get_pixel(offset_src.x + j, offset_src.y + i);
            mat.alphablend(offset_dst.x + j, offset_dst.y + i, alpha, src_color);
        }
    }
}

ImMat ImMat::crop(ImPoint p1, ImPoint p2) const
{
    assert(device == IM_DD_CPU);
    assert(!empty());
    assert((p2.x - p1.x) > 0 && (p2.y - p1.y) > 0);
    int x1 = p1.x, x2 = p2.x;
    int y1 = p1.y, y2 = p2.y;
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x1 >= w) x1 = w - 1;
    if (y1 >= h) y1 = h - 1;
    if (x2 > w) x2 = w;
    if (y2 > h) y2 = h;
    ImMat dst(x2 - x1, y2 - y1, c, elemsize, elempack);

    if (dims == 1)
    {
        // TODO::Dicky
    }
    if (dims == 2)
    {
        for (int i = y1; i < y2; i++)
        {
            for (int j = x1; j < x2; j++)
            {
                switch (type)
                {
                    case IM_DT_INT8:    dst.at<uint8_t> (j - x1, i - y1) = at<uint8_t> (j, i); break;
                    case IM_DT_INT16:   dst.at<uint16_t>(j - x1, i - y1) = at<uint16_t>(j, i); break;
                    case IM_DT_INT32:   dst.at<uint32_t>(j - x1, i - y1) = at<uint32_t>(j, i); break;
                    case IM_DT_INT64:   dst.at<uint64_t>(j - x1, i - y1) = at<uint64_t>(j, i); break;
                    case IM_DT_FLOAT16: dst.at<uint16_t>(j - x1, i - y1) = at<uint16_t>(j, i); break;
                    case IM_DT_FLOAT32: dst.at<float>   (j - x1, i - y1) = at<float>   (j, i); break;
                    case IM_DT_FLOAT64: dst.at<double>  (j - x1, i - y1) = at<double>  (j, i); break;
                    default: break;
                }
            }
        }
    }
    else if (dims == 3)
    {
        for (int _c = 0; _c < c; _c++)
        {
            for (int i = y1; i < y2; i++)
            {
                for (int j = x1; j < x2; j++)
                {
                    switch (type)
                    {
                        case IM_DT_INT8:    dst.at<uint8_t> (j - x1, i - y1, _c) = at<uint8_t> (j, i, _c); break;
                        case IM_DT_INT16:   dst.at<uint16_t>(j - x1, i - y1, _c) = at<uint16_t>(j, i, _c); break;
                        case IM_DT_INT32:   dst.at<uint32_t>(j - x1, i - y1, _c) = at<uint32_t>(j, i, _c); break;
                        case IM_DT_INT64:   dst.at<uint64_t>(j - x1, i - y1, _c) = at<uint64_t>(j, i, _c); break;
                        case IM_DT_FLOAT16: dst.at<uint16_t>(j - x1, i - y1, _c) = at<uint16_t>(j, i, _c); break;
                        case IM_DT_FLOAT32: dst.at<float>   (j - x1, i - y1, _c) = at<float>   (j, i, _c); break;
                        case IM_DT_FLOAT64: dst.at<double>  (j - x1, i - y1, _c) = at<double>  (j, i, _c); break;
                        default: break;
                    }
                }
            }
        }
    }
    return dst;
}

ImMat ImMat::crop(ImBox box) const
{
    return crop(box.Min, box.Max);
}

ImMat ImMat::repeat(int nx, int ny)
{
    ImMat dst;
    assert(device == IM_DD_CPU);
    assert(!empty());
    assert(dims <= 2);
    assert(ny > 0 && nx > 0);
    if( nx == 1 && ny == 1 )
    {
        dst = this->clone();
    }
    else
    {
        ImSize ssize(w, h);
        ImSize dsize(nx * w, ny * h);
        dst.create_type(dsize.w, dsize.h, c, (ImDataType)type);
        int x, y;
        int esz = (int)elemsize;
        ssize.w *= esz; dsize.w *= esz;
        for( y = 0; y < ssize.h; y++ )
        {
            for( x = 0; x < dsize.w; x += ssize.w )
                memcpy((int8_t*)dst.data + (y * dst.w * dst.elemsize) + x, (int8_t *)data + y * w * elemsize, ssize.w);
        }

        for( ; y < dsize.h; y++ )
            memcpy((int8_t*)dst.data + (y * dst.w * dst.elemsize), (int8_t *)dst.data + (y - ssize.h) * dst.w * dst.elemsize, dsize.w );
    }
    return dst;
}

void ImMat::clean(ImPixel color)
{
    assert(dims == 3);
    assert(c > 0);
    assert(data);
    switch (type)
    {
        case IM_DT_INT8:
        {
            uint8_t s_buf[4] = {(uint8_t)(color.r * UINT8_MAX), 
                                (uint8_t)(color.g * UINT8_MAX),
                                (uint8_t)(color.b * UINT8_MAX),
                                (uint8_t)(color.a * UINT8_MAX)};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_INT16:
        {
            uint16_t s_buf[4] = {(uint16_t)(color.r * UINT16_MAX), 
                                 (uint16_t)(color.g * UINT16_MAX),
                                 (uint16_t)(color.b * UINT16_MAX),
                                 (uint16_t)(color.a * UINT16_MAX)};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_INT32:
        {
            uint32_t s_buf[4] = {(uint32_t)(color.r * UINT32_MAX), 
                                 (uint32_t)(color.g * UINT32_MAX),
                                 (uint32_t)(color.b * UINT32_MAX),
                                 (uint32_t)(color.a * UINT32_MAX)};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_INT64:
        {
            uint64_t s_buf[4] = {(uint64_t)(color.r * UINT64_MAX), 
                                 (uint64_t)(color.g * UINT64_MAX),
                                 (uint64_t)(color.b * UINT64_MAX),
                                 (uint64_t)(color.a * UINT64_MAX)};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_FLOAT32:
        {
            float s_buf[4] = {color.r, color.g, color.b, color.a};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_FLOAT64:
        {
            double s_buf[4] = {(double)color.r, (double)color.g, (double)color.b, (double)color.a};
            for (int i = 0; i < total() / c; i++)
            {
                memcpy((uint8_t*)data + i * elemsize * c, s_buf, c * elemsize);
            }
        }
        break;
        case IM_DT_FLOAT16:
        // TODO::Dicky add float16 clean with ImPixel
        break;
        default: break;
    }
}

static void RGB2XYZ(float R, float G, float B, float *X, float *Y, float *Z)
{
    
	*X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
	*Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
	*Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}


static void XYZ2Lab(float X, float Y, float Z, float *L, float *a, float *b)
{
    const float param_13 = 1.0f / 3.0f;
    const float param_16116 = 16.0f / 116.0f;
    const float Xn = 0.950456f;
    const float Yn = 1.0f;
    const float Zn = 1.088754f;

	float fX, fY, fZ;

	X /= Xn;
	Y /= Yn;
	Z /= Zn;

	if (Y > 0.008856f)
		fY = pow(Y, param_13);
	else
		fY = 7.787f * Y + param_16116;

	*L = 116.0f * fY - 16.0f;
	*L = *L > 0.0f ? *L : 0.0f; 

	if (X > 0.008856f)
		fX = pow(X, param_13);
	else
		fX = 7.787f * X + param_16116;

	if (Z > 0.008856)
		fZ = pow(Z, param_13);
	else
		fZ = 7.787f * Z + param_16116;

	*a = 500.0f * (fX - fY);
	*b = 200.0f * (fY - fZ);
}

static void RGB2Lab(float R, float G, float B, float *L, float *a, float *b)
{
	float X = 0.0f, Y = 0.0f, Z = 0.0f;
	RGB2XYZ(R, G, B, &X, &Y, &Z);
	XYZ2Lab(X, Y, Z, L, a, b);
}

static void Lab2XYZ(float L, float a, float b, float *X, float *Y, float *Z)
{
	float fX, fY, fZ;
    const float param_16116 = 16.0f / 116.0f;
    const float Xn = 0.950456f;
    const float Yn = 1.0f;
    const float Zn = 1.088754f;

	fY = (L + 16.0f) / 116.0f;
	if (fY > 0.206893f)
		*Y = fY * fY * fY;
	else
		*Y = (fY - param_16116) / 7.787f;

	fX = a / 500.0f + fY;
	if (fX > 0.206893f)
		*X = fX * fX * fX;
	else
		*X = (fX - param_16116) / 7.787f;

	fZ = fY - b / 200.0f;
	if (fZ > 0.206893f)
		*Z = fZ * fZ * fZ;
	else
		*Z = (fZ - param_16116) / 7.787f;

	(*X) *= Xn;
	(*Y) *=	Yn;
	(*Z) *= Zn;
}


static void XYZ2RGB(float X, float Y, float Z, float *R, float *G, float *B)
{
	float RR, GG, BB;
	RR =  3.240479f * X - 1.537150f * Y - 0.498535f * Z;
	GG = -0.969256f * X + 1.875992f * Y + 0.041556f * Z;
	BB =  0.055648f * X - 0.204043f * Y + 1.057311f * Z;

	*R = std::min(1.f, std::max(RR, 0.0f));
	*G = std::min(1.f, std::max(GG, 0.0f));
	*B = std::min(1.f, std::max(BB, 0.0f));
}

static void Lab2RGB(float L, float a, float b, float *R, float *G, float *B)
{
	float X = 0.0f, Y = 0.0f, Z = 0.0f;
	Lab2XYZ(L, a, b, &X, &Y, &Z);
	XYZ2RGB(X, Y, Z, R, G, B);
}

ImMat ImMat::cvtToLAB() const
{
    assert(device == IM_DD_CPU);
    assert(color_format == IM_CF_BGR || color_format == IM_CF_BGRA || color_format == IM_CF_ABGR);
    assert(total() > 0);
    ImMat m(w, h, 3, (size_t)4u);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel lab;
            RGB2Lab(pixel.r, pixel.g, pixel.b, &lab.r, &lab.g, &lab.b);
            m.set_pixel(_w, _h, lab);
        }
    }
    m.color_format = IM_CF_LAB;
    return m;
}

ImMat ImMat::cvtToGray() const
{
    assert(device == IM_DD_CPU);
    assert(color_format == IM_CF_BGR || color_format == IM_CF_BGRA || color_format == IM_CF_ABGR);
    assert(total() > 0);
    ImMat m(w, h, 1, (size_t)elemsize);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel gray;
            gray.r = 0.2989 * pixel.r + 0.5870 * pixel.g + 0.1140 * pixel.b; 
            m.set_pixel(_w, _h, gray);
        }
    }
    return m;
}

static void RGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        std::swap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        std::swap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = std::abs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

static void HSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

static void RGB2HLS(float r, float g, float b, float *h, float *l, float *s)
{
    float cmax = std::max(r, std::max(g, b));
    float cmin = std::min(r, std::min(g, b));
    float cdes = cmax - cmin;
    float hh, ll, ss;

    ll = (cmax+cmin)/2;
    if(cdes){
        if(ll <= 0.5)
            ss = (cmax-cmin)/(cmax+cmin);
        else
            ss = (cmax-cmin)/(2-cmax-cmin);

        if(cmax == r)
            hh = (0+(g-b)/cdes)*60;
        else if(cmax == g)
            hh = (2+(b-r)/cdes)*60;
        else// if(cmax == b)
            hh = (4+(r-g)/cdes)*60;
        if(hh<0)
            hh+=360;
    }else
        hh = ss = 0;

    *h = hh;
    *l = ll;
    *s = ss;
}

static float HLS2RGBvalue(float n1, float n2, float hue)
{
    if(hue > 360)
        hue -= 360;
    else if(hue < 0)
        hue += 360;
    if(hue < 60)
        return n1+(n2-n1)*hue/60;
    else if(hue < 180)
        return n2;
    else if(hue < 240)
        return n1+(n2-n1)*(240-hue)/60;
    else
        return n1;
}

static void HLS2RGB(float h, float l, float s, float *r, float *g, float *b)
{
    float cmax,cmin;

    if(l <= 0.5)
        cmax = l*(1+s);
    else
        cmax = l*(1-s)+s;
    cmin = 2*l-cmax;

    if(s == 0){
        *r = *g = *b = l;
    }else{
        *r = HLS2RGBvalue(cmin,cmax,h+120);
        *g = HLS2RGBvalue(cmin,cmax,h);
        *b = HLS2RGBvalue(cmin,cmax,h-120);
    }
}

ImMat ImMat::cvtToHSV() const
{
    assert(device == IM_DD_CPU);
    assert(color_format == IM_CF_BGR || color_format == IM_CF_BGRA);
    assert(total() > 0);
    ImMat m(w, h, 3, (size_t)4u);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel hsv;
            RGBtoHSV(pixel.r, pixel.g, pixel.b, hsv.r, hsv.g, hsv.b);
            m.set_pixel(_w, _h, hsv);
        }
    }
    m.color_format = IM_CF_HSV;
    return m;
}

ImMat ImMat::cvtToHLS() const
{
    assert(device == IM_DD_CPU);
    assert(color_format == IM_CF_BGR || color_format == IM_CF_BGRA || color_format == IM_CF_ABGR);
    assert(total() > 0);
    ImMat m(w, h, 3, (size_t)4u);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel hls;
            RGB2HLS(pixel.r, pixel.g, pixel.b, &hls.r, &hls.g, &hls.b);
            m.set_pixel(_w, _h, hls);
        }
    }
    m.color_format = IM_CF_HLS;
    return m;
}

ImMat ImMat::cvtToRGB() const
{
    assert(device == IM_DD_CPU);
    assert(color_format != IM_CF_BGR && color_format != IM_CF_BGRA && color_format != IM_CF_ABGR);
    assert(total() > 0);
    ImMat m(w, h, 3, (size_t)1u, 3);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel rgb;
            switch (color_format)
            {
                case IM_CF_LAB:
                    Lab2RGB(pixel.r, pixel.g, pixel.b, &rgb.r, &rgb.g, &rgb.b);
                    break;
                case IM_CF_GRAY:
                    rgb.r = 
                    rgb.g = 
                    rgb.b = pixel.r;
                    rgb.a = 1;
                break;
                case IM_CF_HSV:
                    HSVtoRGB(pixel.r, pixel.g, pixel.b, rgb.r, rgb.g, rgb.b);
                break;
                case IM_CF_HLS:
                    HLS2RGB(pixel.r, pixel.g, pixel.b, &rgb.r, &rgb.g, &rgb.b);
                break;
                default: break;
            }
            m.set_pixel(_w, _h, rgb);
        }
    }
    return m;
}

ImMat ImMat::cvtToARGB() const
{
    assert(device == IM_DD_CPU);
    assert(color_format != IM_CF_BGR && color_format != IM_CF_BGRA && color_format != IM_CF_ABGR);
    assert(total() > 0);
    ImMat m(w, h, 4, (size_t)1u, 4);
    for (int _h = 0; _h < h; _h++)
    {
        for (int _w = 0; _w < w; _w++)
        {
            auto pixel = get_pixel(_w, _h);
            ImPixel rgb;
            switch (color_format)
            {
                case IM_CF_LAB:
                    Lab2RGB(pixel.r, pixel.g, pixel.b, &rgb.r, &rgb.g, &rgb.b);
                    rgb.a = 1;
                    break;
                case IM_CF_GRAY:
                    rgb.r = 
                    rgb.g = 
                    rgb.b = pixel.r;
                    rgb.a = 1;
                break;
                case IM_CF_HSV:
                    HSVtoRGB(pixel.r, pixel.g, pixel.b, rgb.r, rgb.g, rgb.b);
                    rgb.a = 1;
                break;
                case IM_CF_HLS:
                    HLS2RGB(pixel.r, pixel.g, pixel.b, &rgb.r, &rgb.g, &rgb.b);
                    rgb.a = 1;
                break;
                default: break;
            }
            m.set_pixel(_w, _h, rgb);
        }
    }

    return m;
}

void ImMat::print(std::string name)
{
    std::cout << name << "(" << w << "x" << h << "x" << c << ")" << std::endl << "[" << std::endl;
    if (dims == 1)
    {
        for (int _w = 0; _w < w; _w++)
        {
            switch(type)
            {
                case IM_DT_INT8:    std::cout << (int)at<int8_t> (_w) << " "; break;
                case IM_DT_INT16:   std::cout << at<int16_t>(_w) << " "; break;
                case IM_DT_INT32:   std::cout << at<int32_t>(_w) << " "; break;
                case IM_DT_INT64:   std::cout << at<int64_t>(_w) << " "; break;
                case IM_DT_FLOAT32: std::cout << at<float>  (_w) << " "; break;
                case IM_DT_FLOAT64: std::cout << at<double> (_w) << " "; break;
                case IM_DT_FLOAT16: std::cout << im_float16_to_float32(at<uint16_t>  (_w)) << " "; break;
                default: break;
            }
        }
    }
    else if (dims == 2)
    {
        for (int _h = 0; _h < h; _h++)
        {
            std::cout << "    [ ";
            for (int _w = 0; _w < w; _w++)
            {
                switch(type)
                {
                    case IM_DT_INT8:    std::cout << (int)at<int8_t> (_w, _h) << " "; break;
                    case IM_DT_INT16:   std::cout << at<int16_t>(_w, _h) << " "; break;
                    case IM_DT_INT32:   std::cout << at<int32_t>(_w, _h) << " "; break;
                    case IM_DT_INT64:   std::cout << at<int64_t>(_w, _h) << " "; break;
                    case IM_DT_FLOAT32: std::cout << at<float>  (_w, _h) << " "; break;
                    case IM_DT_FLOAT64: std::cout << at<double> (_w, _h) << " "; break;
                    case IM_DT_FLOAT16: std::cout << im_float16_to_float32(at<uint16_t>  (_w, _h)) << " "; break;
                    default: break;
                }
            }
            std::cout << "]" << std::endl;
        }
    }
    else if (dims == 3)
    {
        for (int _c = 0; _c < c; _c++)
        {
            std::cout << "  [ " << std::endl;
            for (int _h = 0; _h < h; _h++)
            {
                std::cout << "    [ ";
                for (int _w = 0; _w < w; _w++)
                {
                    switch(type)
                    {
                        case IM_DT_INT8:    std::cout << at<int8_t> (_w, _h, _c) << " "; break;
                        case IM_DT_INT16:   std::cout << at<int16_t>(_w, _h, _c) << " "; break;
                        case IM_DT_INT32:   std::cout << at<int32_t>(_w, _h, _c) << " "; break;
                        case IM_DT_INT64:   std::cout << at<int64_t>(_w, _h, _c) << " "; break;
                        case IM_DT_FLOAT32: std::cout << at<float>  (_w, _h, _c) << " "; break;
                        case IM_DT_FLOAT64: std::cout << at<double> (_w, _h, _c) << " "; break;
                        case IM_DT_FLOAT16: std::cout << im_float16_to_float32(at<uint16_t>  (_w, _h, _c)) << " "; break;
                        default: break;
                    }
                }
                std::cout << "]" << std::endl;
            }
            std::cout << "  ]" << std::endl;
        }
    }
    
    std::cout << "]" << std::endl;
}

// Kalman class
ImKalman::ImKalman(int state_size,int mea_size)
{
    transitionMatrix.create_type(state_size, state_size, IM_DT_FLOAT32);
    measurementMatrix.create_type(state_size, mea_size, IM_DT_FLOAT32);
    processNoiseCov.create_type(state_size, state_size, IM_DT_FLOAT32);
    measurementNoiseCov.create_type(mea_size, mea_size, IM_DT_FLOAT32);
    errorCovPre.create_type(state_size, state_size, IM_DT_FLOAT32);
    errorCovPost.create_type(state_size, state_size, IM_DT_FLOAT32);
    statePost.create_type(1, state_size, IM_DT_FLOAT32);
    statePre.create_type(1, state_size, IM_DT_FLOAT32);
    K.create_type(mea_size, state_size, IM_DT_FLOAT32);

    measurementMatrix.eye(1.f);     // 观测矩阵的初始化
    processNoiseCov.eye(1e-5);      // 模型本身噪声协方差矩阵初始化
    measurementNoiseCov.eye(1e-1);  // 测量噪声的协方差矩阵初始化
    errorCovPost.eye(1.f);          // 转移噪声修正矩阵初始化
    statePost.randn(0.f, 5.0f);     // kalaman状态估计修正矩阵初始化
    transitionMatrix.eye(1.f);      // 状态转移矩阵/增益矩阵的初始化
    for (int x = 0; x < state_size; x++)
    {
        for (int y = 0; y < state_size; y++)
        {
            if (x > y && (x - state_size / 2 == y || y + state_size / 2 == x))
                transitionMatrix.at<float>(x, y) = 1.f;
        }
    }
}

void ImKalman::covariance(float noise_covariance, float measurement_noise_covariance)
{
    processNoiseCov.eye(noise_covariance);
    measurementNoiseCov.eye(measurement_noise_covariance);
}

ImMat& ImKalman::predicted()
{
    statePre    = transitionMatrix * statePost;
    errorCovPre = transitionMatrix * errorCovPost * transitionMatrix.t() + processNoiseCov;
    return statePost;
}

void ImKalman::update(ImMat& Y)
{
    K            = errorCovPre * measurementMatrix.t() * ((measurementMatrix * errorCovPre * measurementMatrix.t() + measurementNoiseCov).inv<float>());
    statePost    = statePre    + K * (Y - measurementMatrix * statePre);
    errorCovPost = errorCovPre - K * measurementMatrix * errorCovPre;
}

// warp Affine help
static inline int LU(float* A, size_t astep, int m, float* b, size_t bstep, int n, float eps)
{
    int i, j, k, p = 1;
    for( i = 0; i < m; i++ )
    {
        k = i;
        for( j = i+1; j < m; j++ ) if( std::abs(A[j*astep + i]) > std::abs(A[k*astep + i]) ) k = j;
        if( std::abs(A[k*astep + i]) < eps ) return 0;
        if( k != i )
        {
            for( j = i; j < m; j++ ) std::swap(A[i*astep + j], A[k*astep + j]);
            if( b ) for( j = 0; j < n; j++ ) std::swap(b[i*bstep + j], b[k*bstep + j]);
            p = -p;
        }
        float d = -1/A[i*astep + i];
        for( j = i+1; j < m; j++ )
        {
            float alpha = A[j*astep + i]*d;
            for( k = i+1; k < m; k++ ) A[j*astep + k] += alpha*A[i*astep + k];
            if( b ) for( k = 0; k < n; k++ ) b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }
    if( b )
    {
        for( i = m-1; i >= 0; i-- )
        {
            for( j = 0; j < n; j++ )
            {
                float s = b[i*bstep + j];
                for( k = i+1; k < m; k++ ) s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
        }
    }
    return p;
}

static bool solve(const ImMat& src, const ImMat& src2, ImMat& dst)
{
    // Gaussian elimination with the optimal pivot element chosen.
    bool result = true;
    assert(src.type == src2.type);
    int m = src.h, m_ = m, n = src.w, nb = src2.w;
    assert(m >= n); // The function can not solve under-determined linear systems
    dst.clone_from(src2);
    result = LU((float*)src.data, src.w, n, (float*)dst.data, dst.w, nb, FLT_EPSILON * 10) != 0;
    return result;
}

ImMat getPerspectiveTransform(const ImPoint src[], const ImPoint dst[])
{
    float a[8][8], b[8];
    for (int i = 0; i < 4; ++i)
    {
        a[i][0] = a[i + 4][3] = src[i].x;
        a[i][1] = a[i + 4][4] = src[i].y;
        a[i][2] = a[i + 4][5] = 1;
        a[i][3] = a[i][4] = a[i][5] =
        a[i + 4][0] = a[i + 4][1] = a[i + 4][2] = 0;
        a[i][6] = -src[i].x * dst[i].x;
        a[i][7] = -src[i].y * dst[i].x;
        a[i + 4][6] = -src[i].x * dst[i].y;
        a[i + 4][7] = -src[i].y * dst[i].y;
        b[i] = dst[i].x;
        b[i + 4] = dst[i].y;
    }
    ImGui::ImMat A, B;
    A.create_type(8, 8, a, IM_DT_FLOAT32);
    B.create_type(1, 8, b, IM_DT_FLOAT32);
    ImGui::ImMat M, X;
    M.create_type(3, 3, IM_DT_FLOAT32);
    solve(A, B, X);
    memcpy(M.data, X.data, sizeof(float) * X.total());
    M.at<float>(2, 2) = 1.f;
    return M;
}

ImMat getPerspectiveTransform(const ImMat src, const ImMat dst)
{
    float a[8][8], b[8];
    for (int i = 0; i < 4; ++i)
    {
        a[i][0] = a[i + 4][3] = src.at<float>(0, i);
        a[i][1] = a[i + 4][4] = src.at<float>(1, i);
        a[i][2] = a[i + 4][5] = 1;
        a[i][3] = a[i][4] = a[i][5] =
        a[i + 4][0] = a[i + 4][1] = a[i + 4][2] = 0;
        a[i][6] = -src.at<float>(0, i) * dst.at<float>(0, i);
        a[i][7] = -src.at<float>(1, i) * src.at<float>(0, i);
        a[i + 4][6] = -src.at<float>(0, i) * dst.at<float>(1, i);
        a[i + 4][7] = -src.at<float>(1, i) * dst.at<float>(1, i);
        b[i] = dst.at<float>(0, i);
        b[i + 4] = dst.at<float>(1, i);
    }
    ImGui::ImMat A, B;
    A.create_type(8, 8, a, IM_DT_FLOAT32);
    B.create_type(1, 8, b, IM_DT_FLOAT32);
    ImGui::ImMat M, X;
    M.create_type(3, 3, IM_DT_FLOAT32);
    solve(A, B, X);
    memcpy(M.data, X.data, sizeof(float) * X.total());
    M.at<float>(2, 2) = 1.f;
    return M;
}

ImMat getAffineTransform(const ImPoint src[], const ImPoint dst[])
{
    float a[6*6], b[6];
    for( int i = 0; i < 3; i++ )
    {
        int j = i * 12;
        int k = i * 12 + 6;
        a[j] = a[k + 3] = src[i].x;
        a[j + 1] = a[k + 4] = src[i].y;
        a[j + 2] = a[k + 5] = 1;
        a[j + 3] = a[j + 4] = a[j + 5] = 0;
        a[k] = a[k + 1] = a[k + 2] = 0;
        b[i * 2] = dst[i].x;
        b[i * 2 + 1] = dst[i].y;
    }
    ImMat A, B;
    A.create_type(6, 6, a, IM_DT_FLOAT32);
    B.create_type(1, 6, b, IM_DT_FLOAT32);
    ImMat M, X;
    M.create_type(3, 2, IM_DT_FLOAT32);
    solve(A, B, X);
    memcpy(M.data, X.data, sizeof(float) * X.total());
    return M;
}

ImMat getAffineTransform(int sw, int sh, int dw, int dh, float x_offset, float y_offset, float x_scale, float y_scale, float angle)
{
    ImMat mat;
    mat.create_type(3, 2, IM_DT_FLOAT32);
    float _angle = angle / 180.f * M_PI;
    float _x_scale = 1.f / (x_scale + FLT_EPSILON);
    float _y_scale = 1.f / (y_scale + FLT_EPSILON);
    float alpha_00 = std::cos(_angle) * _x_scale;
    float alpha_11 = std::cos(_angle) * _y_scale;
    float beta_01 = std::sin(_angle) * _x_scale;
    float beta_10 = std::sin(_angle) * _y_scale;
    float x_diff = dw - sw;
    float y_diff = dh - sh;
    float _x_offset = x_offset * (dw + sw * x_scale) / 2 + x_diff / 2;
    float _y_offset = y_offset * (dh + sh * y_scale) / 2 + y_diff / 2;
    int center_x = sw / 2 + _x_offset;
    int center_y = sh / 2 + _y_offset;
    mat.at<float>(0, 0) =  alpha_00;    mat.at<float>(1, 0) = beta_01;      mat.at<float>(2, 0) = (1 - alpha_00) * center_x - beta_01 * center_y - _x_offset;
    mat.at<float>(0, 1) = -beta_10;     mat.at<float>(1, 1) = alpha_11;     mat.at<float>(2, 1) = beta_10 * center_x + (1 - alpha_11) * center_y - _y_offset;
    return mat;
}

void getAffineParam(const ImMat& M, float& x_offset, float& y_offset, float& x_scale, float& y_scale, float& angle)
{
    x_offset = M.at<float>(2, 0);
    y_offset = M.at<float>(2, 1);
    angle = M.at<float>(1, 0) / M.at<float>(0, 0);
    angle = atan(angle);
    x_scale = M.at<float>(0, 0) / cos(angle);
    y_scale = M.at<float>(1, 1) / cos(angle);
}

static void invert_affine_transform(const float* tm, float* tm_inv)
{
    float D = tm[0] * tm[4] - tm[1] * tm[3];
    D = D != 0.f ? 1.f / D : 0.f;

    float A11 = tm[4] * D;
    float A22 = tm[0] * D;
    float A12 = -tm[1] * D;
    float A21 = -tm[3] * D;
    float b1 = -A11 * tm[2] - A12 * tm[5];
    float b2 = -A21 * tm[2] - A22 * tm[5];

    tm_inv[0] = A11;
    tm_inv[1] = A12;
    tm_inv[2] = b1;
    tm_inv[3] = A21;
    tm_inv[4] = A22;
    tm_inv[5] = b2;
}

static void SVD(const ImMat& A, ImMat& W, ImMat& U, ImMat& V)
{
    int m = A.h, n = A.w;
    bool at = false;
    if( m < n )
    {
        std::swap(m, n);
        at = true;
    }
    size_t esz = A.elemsize / sizeof(float);
    size_t astep = m * esz;
    size_t vstep = n * esz;
    ImMat temp_a;
    ImMat temp_w(1, n);
    ImMat temp_v(n, n);
    if( !at )
        temp_a = A.t().clone();
    else
        temp_a = A.clone();

    // SVD compute
    float * At = (float *)temp_a.data;
    float * Wt = (float *)temp_w.data;
    float * Vt = (float *)temp_v.data;
    double sd;
    float c, s;
    float eps = FLT_EPSILON * 2;
    int i, j, k, iter, max_iter = std::max(m, 30);
    for( i = 0; i < n; i++ )
    {
        for( k = 0, sd = 0; k < m; k++ )
        {
            float t = At[i * astep + k];
            sd += (double)t*t;
        }
        Wt[i] = sd;

        if( Vt )
        {
            for( k = 0; k < n; k++ )
                Vt[i * vstep + k] = 0;
            Vt[i * vstep + i] = 1;
        }
    }
    for( iter = 0; iter < max_iter; iter++ )
    {
        bool changed = false;

        for( i = 0; i < n - 1; i++ )
        {
            for( j = i + 1; j < n; j++ )
            {
                float *Ai = At + i * astep, *Aj = At + j * astep;
                double a = Wt[i], p = 0, b = Wt[j];

                for( k = 0; k < m; k++ )
                    p += (double)Ai[k] * Aj[k];

                if( std::abs(p) <= eps * std::sqrt((double)a * b) )
                    continue;

                p *= 2;
                double beta = a - b, gamma = hypot((double)p, beta);
                if( beta < 0 )
                {
                    double delta = (gamma - beta)*0.5;
                    s = (float)std::sqrt(delta/gamma);
                    c = (float)(p/(gamma*s*2));
                }
                else
                {
                    c = (float)std::sqrt((gamma + beta)/(gamma*2));
                    s = (float)(p/(gamma*c*2));
                }

                a = b = 0;
                for( k = 0; k < m; k++ )
                {
                    float t0 = c*Ai[k] + s*Aj[k];
                    float t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (double)t0*t0; b += (double)t1*t1;
                }
                Wt[i] = a; Wt[j] = b;

                changed = true;

                if( Vt )
                {
                    float *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = 0;//vblas.givens(Vi, Vj, n, c, s);

                    for( ; k < n; k++ )
                    {
                        float t0 = c*Vi[k] + s*Vj[k];
                        float t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        }
        if( !changed )
            break;
    }

    for( i = 0; i < n; i++ )
    {
        for( k = 0, sd = 0; k < m; k++ )
        {
            float t = At[i*astep + k];
            sd += (double)t*t;
        }
        Wt[i] = std::sqrt(sd);
    }

    for( i = 0; i < n-1; i++ )
    {
        j = i;
        for( k = i+1; k < n; k++ )
        {
            if( Wt[j] < Wt[k] )
                j = k;
        }
        if( i != j )
        {
            std::swap(Wt[i], Wt[j]);
            if( Vt )
            {
                for( k = 0; k < m; k++ )
                    std::swap(At[i*astep + k], At[j*astep + k]);

                for( k = 0; k < n; k++ )
                    std::swap(Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    for( i = 0; i < n; i++ )
    {
        sd = i < n ? Wt[i] : 0;

        for( int ii = 0; ii < 100 && sd <= FLT_MIN; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            const float val0 = (float)(1./m);
            for( k = 0; k < m; k++ )
            {
                float val = (std::rand() & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            for( iter = 0; iter < 2; iter++ )
            {
                for( j = 0; j < i; j++ )
                {
                    sd = 0;
                    for( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    float asum = 0;
                    for( k = 0; k < m; k++ )
                    {
                        float t = (float)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += std::abs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    for( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            for( k = 0; k < m; k++ )
            {
                float t = At[i*astep + k];
                sd += (double)t*t;
            }
            sd = std::sqrt(sd);
        }

        s = (float)(sd > FLT_MIN ? 1/sd : 0.);
        for( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }

    // finial
    W = temp_w.clone();
    if( !at )
    {
        U = temp_a.t().clone();
        V = temp_v.clone();
    }
    else
    {
        U = temp_v.t().clone();
        V = temp_a.clone();
    }

}

static inline ImMat meanAxis0(const ImMat &src)
{
    int num = src.h;
    int dim = src.w;
    // x1 y1
    // x2 y2
    ImMat output(dim, 1);
    for(int i = 0 ; i <  dim; i ++)
    {
        float sum = 0 ;
        for(int j = 0 ; j < num ; j++)
        {
            sum+=src.at<float>(i,j);
        }
        output.at<float>(i,0) = sum/num;
    }
    return output;
}

static inline ImMat elementwiseMinus(const ImMat &A,const ImMat &B)
{
    ImMat output(A.w, A.h);
    assert(B.w == A.w);
    if(B.w == A.w)
    {
        for(int i = 0 ; i <  A.h; i ++)
        {
            for(int j = 0 ; j < B.w; j++)
            {
                output.at<float>(j,i) = A.at<float>(j,i) - B.at<float>(j,0);
            }
        }
    }
    return output;
}

static inline ImMat varAxis0(const ImMat &src)
{
    ImMat temp_ = elementwiseMinus(src, meanAxis0(src));
    temp_ = temp_.mul(temp_);
    return meanAxis0(temp_);
}

static inline int MatrixRank(const ImMat& M)
{
    ImMat w, u, vt;
    SVD(M, w, u, vt);
    int rank = 0;
    for (int i = 0; i < w.h; i++)
    {
        for (int j = 0; j < w.w; j++)
        {
            if (w.at<float>(j,i) > 0.0001)
                rank++;
        }
    }
    return rank;
}

ImMat similarTransform(const ImMat& src, const ImMat& dst)
{
    int num = src.h;
    int dim = src.w;
    auto src_mean = meanAxis0(src);
    auto dst_mean = meanAxis0(dst);
    auto src_demean = elementwiseMinus(src, src_mean);
    auto dst_demean = elementwiseMinus(dst, dst_mean);
    auto A = (dst_demean.t() * src_demean) / static_cast<float>(num);
    ImMat d(1, dim);
    d.fill(1.0f);
    if (A.determinant() < 0) {
        d.at<float>(0, dim - 1) = -1.0f;
    }
    ImMat T = ImMat(dim + 1, dim).eye(1.f);

    ImMat U, S, V;
    SVD(A, S, U, V);

    auto range = [](ImMat& src, ImMat& dst, int row_min, int row_max, int col_min, int col_max)
    {
        for (int r = row_min; r < row_max; r++)
        {
            for (int c = col_min; c < col_max; c++)
            {
                dst.at<float>(c, r) = src.at<float>(c - col_min, r - row_min);
            }
        }
    };
    auto range_scale = [](ImMat& dst, float scale, int row_min, int row_max, int col_min, int col_max)
    {
        for (int r = row_min; r < row_max; r++)
        {
            for (int c = col_min; c < col_max; c++)
            {
                dst.at<float>(c, r) *= scale;
            }
        }
    };
    int rank = MatrixRank(A);
    if (rank == 0) {
        assert(rank == 0);

    } else if (rank == dim - 1) {
        if (U.determinant() * V.determinant() > 0) {
            auto UV = U * V;
            range(UV, T, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) = U * V;
        } else {
            int s = d.at<float>(dim - 1, 0) = -1;
            d.at<float>(dim - 1, 0) = -1;
            auto UV = U * V;
            range(UV, T, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) = U * V;
            ImMat diag_ = d.diag<float>();
            ImMat twp = diag_ * V;
            ImMat utwp = U * twp;
            range(utwp, T, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) = U * twp;
            d.at<float>(dim - 1, 0) = s;
        }
    }
    else{
        ImMat diag_ = d.diag<float>();
        ImMat twp = diag_ * V.t();
        ImMat res = U * twp;
        ImMat utwp = -U.t() * twp;
        range(utwp, T, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) = -U.t() * twp;
    }

    ImMat var_ = varAxis0(src_demean);
    float val = var_.sum().at<float>(0);
    ImMat res = d.mul(S);
    float sum = res.sum().at<float>(0);
    float scale =  1.0 / val * sum;
    
    ImMat temp0(dim, dim);
    range(T, temp0, 0, dim, 0, dim);
    temp0 = -temp0.t();
    range(temp0, T, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) = - T.rowRange(0, dim).colRange(0, dim).t();
    ImMat temp1(dim, dim);
    range(T, temp1, 0, dim, 0, dim); //temp1 = T.rowRange(0, dim).colRange(0, dim);
    ImMat temp2 = src_mean.t(); 
    ImMat temp3 = temp1 * temp2;
    ImMat temp4 = temp3 * scale;

    ImMat temp5 =  -(temp4 - dst_mean.t());
    range(temp5, T, 0, dim, dim, dim+1); //T.rowRange(0, dim).colRange(dim, dim + 1)=  -(temp4 - dst_mean.t()) ;
    range_scale(T, scale, 0, dim, 0, dim); //T.rowRange(0, dim).colRange(0, dim) *= scale;
    return T;
}

// mat resize
static void resize_bilinear_c1(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride)
{
    const int INTER_RESIZE_COEF_BITS = 11;
    const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;
    //     const int ONE=INTER_RESIZE_COEF_SCALE;

    double scale_x = (double)srcw / w;
    double scale_y = (double)srch / h;

    int* buf = new int[w + h + w + h];

    int* xofs = buf;     //new int[w];
    int* yofs = buf + w; //new int[h];

    short* ialpha = (short*)(buf + w + h);    //new short[w * 2];
    short* ibeta = (short*)(buf + w + h + w); //new short[h * 2];

    float fx;
    float fy;
    int sx;
    int sy;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X + (X >= 0.f ? 0.5f : -0.5f)), SHRT_MIN), SHRT_MAX);

    for (int dx = 0; dx < w; dx++)
    {
        fx = (float)((dx + 0.5) * scale_x - 0.5);
        sx = static_cast<int>(floor(fx));
        fx -= sx;

        if (sx < 0)
        {
            sx = 0;
            fx = 0.f;
        }
        if (sx >= srcw - 1)
        {
            sx = srcw - 2;
            fx = 1.f;
        }

        xofs[dx] = sx;

        float a0 = (1.f - fx) * INTER_RESIZE_COEF_SCALE;
        float a1 = fx * INTER_RESIZE_COEF_SCALE;

        ialpha[dx * 2] = SATURATE_CAST_SHORT(a0);
        ialpha[dx * 2 + 1] = SATURATE_CAST_SHORT(a1);
    }

    for (int dy = 0; dy < h; dy++)
    {
        fy = (float)((dy + 0.5) * scale_y - 0.5);
        sy = static_cast<int>(floor(fy));
        fy -= sy;

        if (sy < 0)
        {
            sy = 0;
            fy = 0.f;
        }
        if (sy >= srch - 1)
        {
            sy = srch - 2;
            fy = 1.f;
        }

        yofs[dy] = sy;

        float b0 = (1.f - fy) * INTER_RESIZE_COEF_SCALE;
        float b1 = fy * INTER_RESIZE_COEF_SCALE;

        ibeta[dy * 2] = SATURATE_CAST_SHORT(b0);
        ibeta[dy * 2 + 1] = SATURATE_CAST_SHORT(b1);
    }

#undef SATURATE_CAST_SHORT

    // loop body
    ImGui::ImMat rowsbuf0(w, (size_t)2u);
    ImGui::ImMat rowsbuf1(w, (size_t)2u);
    short* rows0 = (short*)rowsbuf0.data;
    short* rows1 = (short*)rowsbuf1.data;

    int prev_sy1 = -2;

    for (int dy = 0; dy < h; dy++)
    {
        sy = yofs[dy];

        if (sy == prev_sy1)
        {
            // reuse all rows
        }
        else if (sy == prev_sy1 + 1)
        {
            // hresize one row
            short* rows0_old = rows0;
            rows0 = rows1;
            rows1 = rows0_old;
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S1p = S1 + sx;
                rows1p[dx] = (S1p[0] * a0 + S1p[1] * a1) >> 4;

                ialphap += 2;
            }
        }
        else
        {
            // hresize two rows
            const unsigned char* S0 = src + srcstride * (sy);
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows0p = rows0;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S0p = S0 + sx;
                const unsigned char* S1p = S1 + sx;
                rows0p[dx] = (S0p[0] * a0 + S0p[1] * a1) >> 4;
                rows1p[dx] = (S1p[0] * a0 + S1p[1] * a1) >> 4;

                ialphap += 2;
            }
        }

        prev_sy1 = sy;

        // vresize
        short b0 = ibeta[0];
        short b1 = ibeta[1];

        short* rows0p = rows0;
        short* rows1p = rows1;
        unsigned char* Dp = dst + stride * (dy);

#if __ARM_NEON || __SSE__ || __AVX__
        int nn = w >> 3;
#else
        int nn = 0;
#endif
        int remain = w - (nn << 3);

#if __ARM_NEON || __SSE__ || __AVX__
#if __aarch64__ || __SSE__ || __AVX__
        int16x4_t _b0 = vdup_n_s16(b0);
        int16x4_t _b1 = vdup_n_s16(b1);
        int32x4_t _v2 = vdupq_n_s32(2);
        for (; nn > 0; nn--)
        {
            int16x4_t _rows0p_sr4 = vld1_s16(rows0p);
            int16x4_t _rows1p_sr4 = vld1_s16(rows1p);
            int16x4_t _rows0p_1_sr4 = vld1_s16(rows0p + 4);
            int16x4_t _rows1p_1_sr4 = vld1_s16(rows1p + 4);

            int32x4_t _rows0p_sr4_mb0 = vmull_s16(_rows0p_sr4, _b0);
            int32x4_t _rows1p_sr4_mb1 = vmull_s16(_rows1p_sr4, _b1);
            int32x4_t _rows0p_1_sr4_mb0 = vmull_s16(_rows0p_1_sr4, _b0);
            int32x4_t _rows1p_1_sr4_mb1 = vmull_s16(_rows1p_1_sr4, _b1);

            int32x4_t _acc = _v2;
            _acc = vsraq_n_s32(_acc, _rows0p_sr4_mb0, 16);
            _acc = vsraq_n_s32(_acc, _rows1p_sr4_mb1, 16);

            int32x4_t _acc_1 = _v2;
            _acc_1 = vsraq_n_s32(_acc_1, _rows0p_1_sr4_mb0, 16);
            _acc_1 = vsraq_n_s32(_acc_1, _rows1p_1_sr4_mb1, 16);

            int16x4_t _acc16 = vshrn_n_s32(_acc, 2);
            int16x4_t _acc16_1 = vshrn_n_s32(_acc_1, 2);

            uint8x8_t _D = vqmovun_s16(vcombine_s16(_acc16, _acc16_1));

            vst1_u8(Dp, _D);

            Dp += 8;
            rows0p += 8;
            rows1p += 8;
        }
#else
        if (nn > 0)
        {
            asm volatile(
                "vdup.s16   d16, %8         \n"
                "mov        r4, #2          \n"
                "vdup.s16   d17, %9         \n"
                "vdup.s32   q12, r4         \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "0:                         \n"
                "vmull.s16  q0, d2, d16     \n"
                "vmull.s16  q1, d3, d16     \n"
                "vorr.s32   q10, q12, q12   \n"
                "vorr.s32   q11, q12, q12   \n"
                "vmull.s16  q2, d6, d17     \n"
                "vmull.s16  q3, d7, d17     \n"
                "vsra.s32   q10, q0, #16    \n"
                "vsra.s32   q11, q1, #16    \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "vsra.s32   q10, q2, #16    \n"
                "vsra.s32   q11, q3, #16    \n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "vshrn.s32  d20, q10, #2    \n"
                "vshrn.s32  d21, q11, #2    \n"
                "vqmovun.s16 d20, q10        \n"
                "vst1.8     {d20}, [%2]!    \n"
                "subs       %3, #1          \n"
                "bne        0b              \n"
                "sub        %0, #16         \n"
                "sub        %1, #16         \n"
                : "=r"(rows0p), // %0
                "=r"(rows1p), // %1
                "=r"(Dp),     // %2
                "=r"(nn)      // %3
                : "0"(rows0p),
                "1"(rows1p),
                "2"(Dp),
                "3"(nn),
                "r"(b0), // %8
                "r"(b1)  // %9
                : "cc", "memory", "r4", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12");
        }
#endif // __aarch64__ || __SSE__ || __AVX__
#endif // __ARM_NEON || __SSE__ || __AVX__
        for (; remain; --remain)
        {
            //             D[x] = (rows0[x]*b0 + rows1[x]*b1) >> INTER_RESIZE_COEF_BITS;
            *Dp++ = (unsigned char)(((short)((b0 * (short)(*rows0p++)) >> 16) + (short)((b1 * (short)(*rows1p++)) >> 16) + 2) >> 2);
        }

        ibeta += 2;
    }

    delete[] buf;
}

static void resize_bilinear_c2(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride)
{
    const int INTER_RESIZE_COEF_BITS = 11;
    const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;
    //     const int ONE=INTER_RESIZE_COEF_SCALE;

    double scale_x = (double)srcw / w;
    double scale_y = (double)srch / h;

    int* buf = new int[w + h + w + h];

    int* xofs = buf;     //new int[w];
    int* yofs = buf + w; //new int[h];

    short* ialpha = (short*)(buf + w + h);    //new short[w * 2];
    short* ibeta = (short*)(buf + w + h + w); //new short[h * 2];

    float fx;
    float fy;
    int sx;
    int sy;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X + (X >= 0.f ? 0.5f : -0.5f)), SHRT_MIN), SHRT_MAX);

    for (int dx = 0; dx < w; dx++)
    {
        fx = (float)((dx + 0.5) * scale_x - 0.5);
        sx = static_cast<int>(floor(fx));
        fx -= sx;

        if (sx < 0)
        {
            sx = 0;
            fx = 0.f;
        }
        if (sx >= srcw - 1)
        {
            sx = srcw - 2;
            fx = 1.f;
        }

        xofs[dx] = sx * 2;

        float a0 = (1.f - fx) * INTER_RESIZE_COEF_SCALE;
        float a1 = fx * INTER_RESIZE_COEF_SCALE;

        ialpha[dx * 2] = SATURATE_CAST_SHORT(a0);
        ialpha[dx * 2 + 1] = SATURATE_CAST_SHORT(a1);
    }

    for (int dy = 0; dy < h; dy++)
    {
        fy = (float)((dy + 0.5) * scale_y - 0.5);
        sy = static_cast<int>(floor(fy));
        fy -= sy;

        if (sy < 0)
        {
            sy = 0;
            fy = 0.f;
        }
        if (sy >= srch - 1)
        {
            sy = srch - 2;
            fy = 1.f;
        }

        yofs[dy] = sy;

        float b0 = (1.f - fy) * INTER_RESIZE_COEF_SCALE;
        float b1 = fy * INTER_RESIZE_COEF_SCALE;

        ibeta[dy * 2] = SATURATE_CAST_SHORT(b0);
        ibeta[dy * 2 + 1] = SATURATE_CAST_SHORT(b1);
    }

#undef SATURATE_CAST_SHORT

    // loop body
    ImGui::ImMat rowsbuf0(w * 2 + 2, (size_t)2u);
    ImGui::ImMat rowsbuf1(w * 2 + 2, (size_t)2u);
    short* rows0 = (short*)rowsbuf0.data;
    short* rows1 = (short*)rowsbuf1.data;

    int prev_sy1 = -2;

    for (int dy = 0; dy < h; dy++)
    {
        sy = yofs[dy];

        if (sy == prev_sy1)
        {
            // reuse all rows
        }
        else if (sy == prev_sy1 + 1)
        {
            // hresize one row
            short* rows0_old = rows0;
            rows0 = rows1;
            rows1 = rows0_old;
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];

                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0a1XX = vld1_s16(ialphap);
                int16x4_t _a0a0a1a1 = vzip_s16(_a0a1XX, _a0a1XX).val[0];
                uint8x8_t _S1 = uint8x8_t();

                _S1 = vld1_lane_u8(S1p, _S1, 0);
                _S1 = vld1_lane_u8(S1p + 1, _S1, 1);
                _S1 = vld1_lane_u8(S1p + 2, _S1, 2);
                _S1 = vld1_lane_u8(S1p + 3, _S1, 3);

                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S1lowhigh = vget_low_s16(_S116);
                int32x4_t _S1ma0a1 = vmull_s16(_S1lowhigh, _a0a0a1a1);
                int32x2_t _rows1low = vadd_s32(vget_low_s32(_S1ma0a1), vget_high_s32(_S1ma0a1));
                int32x4_t _rows1 = vcombine_s32(_rows1low, vget_high_s32(_S1ma0a1));
                int16x4_t _rows1_sr4 = vshrn_n_s32(_rows1, 4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                rows1p[0] = (S1p[0] * a0 + S1p[2] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[3] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows1p += 2;
            }
        }
        else
        {
            // hresize two rows
            const unsigned char* S0 = src + srcstride * (sy);
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows0p = rows0;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S0p = S0 + sx;
                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0 = vdup_n_s16(a0);
                int16x4_t _a1 = vdup_n_s16(a1);
                uint8x8_t _S0 = uint8x8_t();
                uint8x8_t _S1 = uint8x8_t();

                _S0 = vld1_lane_u8(S0p, _S0, 0);
                _S0 = vld1_lane_u8(S0p + 1, _S0, 1);
                _S0 = vld1_lane_u8(S0p + 2, _S0, 2);
                _S0 = vld1_lane_u8(S0p + 3, _S0, 3);

                _S1 = vld1_lane_u8(S1p, _S1, 0);
                _S1 = vld1_lane_u8(S1p + 1, _S1, 1);
                _S1 = vld1_lane_u8(S1p + 2, _S1, 2);
                _S1 = vld1_lane_u8(S1p + 3, _S1, 3);

                int16x8_t _S016 = vreinterpretq_s16_u16(vmovl_u8(_S0));
                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S0lowhigh = vget_low_s16(_S016);
                int16x4_t _S1lowhigh = vget_low_s16(_S116);
                int32x2x2_t _S0S1low_S0S1high = vtrn_s32(vreinterpret_s32_s16(_S0lowhigh), vreinterpret_s32_s16(_S1lowhigh));
                int32x4_t _rows01 = vmull_s16(vreinterpret_s16_s32(_S0S1low_S0S1high.val[0]), _a0);
                _rows01 = vmlal_s16(_rows01, vreinterpret_s16_s32(_S0S1low_S0S1high.val[1]), _a1);
                int16x4_t _rows01_sr4 = vshrn_n_s32(_rows01, 4);
                int16x4_t _rows1_sr4 = vext_s16(_rows01_sr4, _rows01_sr4, 2);
                vst1_s16(rows0p, _rows01_sr4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                rows0p[0] = (S0p[0] * a0 + S0p[2] * a1) >> 4;
                rows0p[1] = (S0p[1] * a0 + S0p[3] * a1) >> 4;
                rows1p[0] = (S1p[0] * a0 + S1p[2] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[3] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows0p += 2;
                rows1p += 2;
            }
        }

        prev_sy1 = sy;

        // vresize
        short b0 = ibeta[0];
        short b1 = ibeta[1];

        short* rows0p = rows0;
        short* rows1p = rows1;
        unsigned char* Dp = dst + stride * (dy);

#if __ARM_NEON || __SSE__ || __AVX__
        int nn = (w * 2) >> 3;
#else
        int nn = 0;
#endif
        int remain = (w * 2) - (nn << 3);

#if __ARM_NEON || __SSE__ || __AVX__
#if __aarch64__ || __SSE__ || __AVX__
        int16x4_t _b0 = vdup_n_s16(b0);
        int16x4_t _b1 = vdup_n_s16(b1);
        int32x4_t _v2 = vdupq_n_s32(2);
        for (; nn > 0; nn--)
        {
            int16x4_t _rows0p_sr4 = vld1_s16(rows0p);
            int16x4_t _rows1p_sr4 = vld1_s16(rows1p);
            int16x4_t _rows0p_1_sr4 = vld1_s16(rows0p + 4);
            int16x4_t _rows1p_1_sr4 = vld1_s16(rows1p + 4);

            int32x4_t _rows0p_sr4_mb0 = vmull_s16(_rows0p_sr4, _b0);
            int32x4_t _rows1p_sr4_mb1 = vmull_s16(_rows1p_sr4, _b1);
            int32x4_t _rows0p_1_sr4_mb0 = vmull_s16(_rows0p_1_sr4, _b0);
            int32x4_t _rows1p_1_sr4_mb1 = vmull_s16(_rows1p_1_sr4, _b1);

            int32x4_t _acc = _v2;
            _acc = vsraq_n_s32(_acc, _rows0p_sr4_mb0, 16);
            _acc = vsraq_n_s32(_acc, _rows1p_sr4_mb1, 16);

            int32x4_t _acc_1 = _v2;
            _acc_1 = vsraq_n_s32(_acc_1, _rows0p_1_sr4_mb0, 16);
            _acc_1 = vsraq_n_s32(_acc_1, _rows1p_1_sr4_mb1, 16);

            int16x4_t _acc16 = vshrn_n_s32(_acc, 2);
            int16x4_t _acc16_1 = vshrn_n_s32(_acc_1, 2);

            uint8x8_t _D = vqmovun_s16(vcombine_s16(_acc16, _acc16_1));

            vst1_u8(Dp, _D);

            Dp += 8;
            rows0p += 8;
            rows1p += 8;
        }
#else
        if (nn > 0)
        {
            asm volatile(
                "vdup.s16   d16, %8         \n"
                "mov        r4, #2          \n"
                "vdup.s16   d17, %9         \n"
                "vdup.s32   q12, r4         \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "0:                         \n"
                "vmull.s16  q0, d2, d16     \n"
                "vmull.s16  q1, d3, d16     \n"
                "vorr.s32   q10, q12, q12   \n"
                "vorr.s32   q11, q12, q12   \n"
                "vmull.s16  q2, d6, d17     \n"
                "vmull.s16  q3, d7, d17     \n"
                "vsra.s32   q10, q0, #16    \n"
                "vsra.s32   q11, q1, #16    \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "vsra.s32   q10, q2, #16    \n"
                "vsra.s32   q11, q3, #16    \n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "vshrn.s32  d20, q10, #2    \n"
                "vshrn.s32  d21, q11, #2    \n"
                "vqmovun.s16 d20, q10        \n"
                "vst1.8     {d20}, [%2]!    \n"
                "subs       %3, #1          \n"
                "bne        0b              \n"
                "sub        %0, #16         \n"
                "sub        %1, #16         \n"
                : "=r"(rows0p), // %0
                "=r"(rows1p), // %1
                "=r"(Dp),     // %2
                "=r"(nn)      // %3
                : "0"(rows0p),
                "1"(rows1p),
                "2"(Dp),
                "3"(nn),
                "r"(b0), // %8
                "r"(b1)  // %9
                : "cc", "memory", "r4", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12");
        }
#endif // __aarch64__ || __SSE__ || __AVX__
#endif // __ARM_NEON || __SSE__ || __AVX__
        for (; remain; --remain)
        {
            //             D[x] = (rows0[x]*b0 + rows1[x]*b1) >> INTER_RESIZE_COEF_BITS;
            *Dp++ = (unsigned char)(((short)((b0 * (short)(*rows0p++)) >> 16) + (short)((b1 * (short)(*rows1p++)) >> 16) + 2) >> 2);
        }

        ibeta += 2;
    }

    delete[] buf;
}

static void resize_bilinear_c3(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride)
{
    const int INTER_RESIZE_COEF_BITS = 11;
    const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;
    //     const int ONE=INTER_RESIZE_COEF_SCALE;

    double scale_x = (double)srcw / w;
    double scale_y = (double)srch / h;

    int* buf = new int[w + h + w + h];

    int* xofs = buf;     //new int[w];
    int* yofs = buf + w; //new int[h];

    short* ialpha = (short*)(buf + w + h);    //new short[w * 2];
    short* ibeta = (short*)(buf + w + h + w); //new short[h * 2];

    float fx;
    float fy;
    int sx;
    int sy;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X + (X >= 0.f ? 0.5f : -0.5f)), SHRT_MIN), SHRT_MAX);

    for (int dx = 0; dx < w; dx++)
    {
        fx = (float)((dx + 0.5) * scale_x - 0.5);
        sx = static_cast<int>(floor(fx));
        fx -= sx;

        if (sx < 0)
        {
            sx = 0;
            fx = 0.f;
        }
        if (sx >= srcw - 1)
        {
            sx = srcw - 2;
            fx = 1.f;
        }

        xofs[dx] = sx * 3;

        float a0 = (1.f - fx) * INTER_RESIZE_COEF_SCALE;
        float a1 = fx * INTER_RESIZE_COEF_SCALE;

        ialpha[dx * 2] = SATURATE_CAST_SHORT(a0);
        ialpha[dx * 2 + 1] = SATURATE_CAST_SHORT(a1);
    }

    for (int dy = 0; dy < h; dy++)
    {
        fy = (float)((dy + 0.5) * scale_y - 0.5);
        sy = static_cast<int>(floor(fy));
        fy -= sy;

        if (sy < 0)
        {
            sy = 0;
            fy = 0.f;
        }
        if (sy >= srch - 1)
        {
            sy = srch - 2;
            fy = 1.f;
        }

        yofs[dy] = sy;

        float b0 = (1.f - fy) * INTER_RESIZE_COEF_SCALE;
        float b1 = fy * INTER_RESIZE_COEF_SCALE;

        ibeta[dy * 2] = SATURATE_CAST_SHORT(b0);
        ibeta[dy * 2 + 1] = SATURATE_CAST_SHORT(b1);
    }

#undef SATURATE_CAST_SHORT

    // loop body
    ImGui::ImMat rowsbuf0(w * 3 + 1, (size_t)2u);
    ImGui::ImMat rowsbuf1(w * 3 + 1, (size_t)2u);
    short* rows0 = (short*)rowsbuf0.data;
    short* rows1 = (short*)rowsbuf1.data;

    int prev_sy1 = -2;

    for (int dy = 0; dy < h; dy++)
    {
        sy = yofs[dy];

        if (sy == prev_sy1)
        {
            // reuse all rows
        }
        else if (sy == prev_sy1 + 1)
        {
            // hresize one row
            short* rows0_old = rows0;
            rows0 = rows1;
            rows1 = rows0_old;
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0 = vdup_n_s16(a0);
                int16x4_t _a1 = vdup_n_s16(a1);
                uint8x8_t _S1 = uint8x8_t();

                _S1 = vld1_lane_u8(S1p, _S1, 0);
                _S1 = vld1_lane_u8(S1p + 1, _S1, 1);
                _S1 = vld1_lane_u8(S1p + 2, _S1, 2);
                _S1 = vld1_lane_u8(S1p + 3, _S1, 3);
                _S1 = vld1_lane_u8(S1p + 4, _S1, 4);
                _S1 = vld1_lane_u8(S1p + 5, _S1, 5);

                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S1low = vget_low_s16(_S116);
                int16x4_t _S1high = vext_s16(_S1low, vget_high_s16(_S116), 3);
                int32x4_t _rows1 = vmull_s16(_S1low, _a0);
                _rows1 = vmlal_s16(_rows1, _S1high, _a1);
                int16x4_t _rows1_sr4 = vshrn_n_s32(_rows1, 4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                rows1p[0] = (S1p[0] * a0 + S1p[3] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[4] * a1) >> 4;
                rows1p[2] = (S1p[2] * a0 + S1p[5] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows1p += 3;
            }
        }
        else
        {
            // hresize two rows
            const unsigned char* S0 = src + srcstride * (sy);
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows0p = rows0;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S0p = S0 + sx;
                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0 = vdup_n_s16(a0);
                int16x4_t _a1 = vdup_n_s16(a1);
                uint8x8_t _S0 = uint8x8_t();
                uint8x8_t _S1 = uint8x8_t();

                _S0 = vld1_lane_u8(S0p, _S0, 0);
                _S0 = vld1_lane_u8(S0p + 1, _S0, 1);
                _S0 = vld1_lane_u8(S0p + 2, _S0, 2);
                _S0 = vld1_lane_u8(S0p + 3, _S0, 3);
                _S0 = vld1_lane_u8(S0p + 4, _S0, 4);
                _S0 = vld1_lane_u8(S0p + 5, _S0, 5);

                _S1 = vld1_lane_u8(S1p, _S1, 0);
                _S1 = vld1_lane_u8(S1p + 1, _S1, 1);
                _S1 = vld1_lane_u8(S1p + 2, _S1, 2);
                _S1 = vld1_lane_u8(S1p + 3, _S1, 3);
                _S1 = vld1_lane_u8(S1p + 4, _S1, 4);
                _S1 = vld1_lane_u8(S1p + 5, _S1, 5);

                int16x8_t _S016 = vreinterpretq_s16_u16(vmovl_u8(_S0));
                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S0low = vget_low_s16(_S016);
                int16x4_t _S1low = vget_low_s16(_S116);
                int16x4_t _S0high = vext_s16(_S0low, vget_high_s16(_S016), 3);
                int16x4_t _S1high = vext_s16(_S1low, vget_high_s16(_S116), 3);
                int32x4_t _rows0 = vmull_s16(_S0low, _a0);
                int32x4_t _rows1 = vmull_s16(_S1low, _a0);
                _rows0 = vmlal_s16(_rows0, _S0high, _a1);
                _rows1 = vmlal_s16(_rows1, _S1high, _a1);
                int16x4_t _rows0_sr4 = vshrn_n_s32(_rows0, 4);
                int16x4_t _rows1_sr4 = vshrn_n_s32(_rows1, 4);
                vst1_s16(rows0p, _rows0_sr4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                rows0p[0] = (S0p[0] * a0 + S0p[3] * a1) >> 4;
                rows0p[1] = (S0p[1] * a0 + S0p[4] * a1) >> 4;
                rows0p[2] = (S0p[2] * a0 + S0p[5] * a1) >> 4;
                rows1p[0] = (S1p[0] * a0 + S1p[3] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[4] * a1) >> 4;
                rows1p[2] = (S1p[2] * a0 + S1p[5] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows0p += 3;
                rows1p += 3;
            }
        }

        prev_sy1 = sy;

        // vresize
        short b0 = ibeta[0];
        short b1 = ibeta[1];

        short* rows0p = rows0;
        short* rows1p = rows1;
        unsigned char* Dp = dst + stride * (dy);

#if __ARM_NEON || __SSE__ || __AVX__
        int nn = (w * 3) >> 3;
#else
        int nn = 0;
#endif
        int remain = (w * 3) - (nn << 3);

#if __ARM_NEON || __SSE__ || __AVX__
#if __aarch64__ || __SSE__ || __AVX__
        int16x4_t _b0 = vdup_n_s16(b0);
        int16x4_t _b1 = vdup_n_s16(b1);
        int32x4_t _v2 = vdupq_n_s32(2);
        for (; nn > 0; nn--)
        {
            int16x4_t _rows0p_sr4 = vld1_s16(rows0p);
            int16x4_t _rows1p_sr4 = vld1_s16(rows1p);
            int16x4_t _rows0p_1_sr4 = vld1_s16(rows0p + 4);
            int16x4_t _rows1p_1_sr4 = vld1_s16(rows1p + 4);

            int32x4_t _rows0p_sr4_mb0 = vmull_s16(_rows0p_sr4, _b0);
            int32x4_t _rows1p_sr4_mb1 = vmull_s16(_rows1p_sr4, _b1);
            int32x4_t _rows0p_1_sr4_mb0 = vmull_s16(_rows0p_1_sr4, _b0);
            int32x4_t _rows1p_1_sr4_mb1 = vmull_s16(_rows1p_1_sr4, _b1);

            int32x4_t _acc = _v2;
            _acc = vsraq_n_s32(_acc, _rows0p_sr4_mb0, 16);
            _acc = vsraq_n_s32(_acc, _rows1p_sr4_mb1, 16);

            int32x4_t _acc_1 = _v2;
            _acc_1 = vsraq_n_s32(_acc_1, _rows0p_1_sr4_mb0, 16);
            _acc_1 = vsraq_n_s32(_acc_1, _rows1p_1_sr4_mb1, 16);

            int16x4_t _acc16 = vshrn_n_s32(_acc, 2);
            int16x4_t _acc16_1 = vshrn_n_s32(_acc_1, 2);

            uint8x8_t _D = vqmovun_s16(vcombine_s16(_acc16, _acc16_1));

            vst1_u8(Dp, _D);

            Dp += 8;
            rows0p += 8;
            rows1p += 8;
        }
#else
        if (nn > 0)
        {
            asm volatile(
                "vdup.s16   d16, %8         \n"
                "mov        r4, #2          \n"
                "vdup.s16   d17, %9         \n"
                "vdup.s32   q12, r4         \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "0:                         \n"
                "vmull.s16  q0, d2, d16     \n"
                "vmull.s16  q1, d3, d16     \n"
                "vorr.s32   q10, q12, q12   \n"
                "vorr.s32   q11, q12, q12   \n"
                "vmull.s16  q2, d6, d17     \n"
                "vmull.s16  q3, d7, d17     \n"
                "vsra.s32   q10, q0, #16    \n"
                "vsra.s32   q11, q1, #16    \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "vsra.s32   q10, q2, #16    \n"
                "vsra.s32   q11, q3, #16    \n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "vshrn.s32  d20, q10, #2    \n"
                "vshrn.s32  d21, q11, #2    \n"
                "vqmovun.s16 d20, q10        \n"
                "vst1.8     {d20}, [%2]!    \n"
                "subs       %3, #1          \n"
                "bne        0b              \n"
                "sub        %0, #16         \n"
                "sub        %1, #16         \n"
                : "=r"(rows0p), // %0
                "=r"(rows1p), // %1
                "=r"(Dp),     // %2
                "=r"(nn)      // %3
                : "0"(rows0p),
                "1"(rows1p),
                "2"(Dp),
                "3"(nn),
                "r"(b0), // %8
                "r"(b1)  // %9
                : "cc", "memory", "r4", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12");
        }
#endif // __aarch64__ || __SSE__ || __AVX__
#endif // __ARM_NEON || __SSE__ || __AVX__
        for (; remain; --remain)
        {
            //             D[x] = (rows0[x]*b0 + rows1[x]*b1) >> INTER_RESIZE_COEF_BITS;
            *Dp++ = (unsigned char)(((short)((b0 * (short)(*rows0p++)) >> 16) + (short)((b1 * (short)(*rows1p++)) >> 16) + 2) >> 2);
        }

        ibeta += 2;
    }

    delete[] buf;
}

static void resize_bilinear_c4(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride)
{
    const int INTER_RESIZE_COEF_BITS = 11;
    const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;
    //     const int ONE=INTER_RESIZE_COEF_SCALE;

    double scale_x = (double)srcw / w;
    double scale_y = (double)srch / h;

    int* buf = new int[w + h + w + h];

    int* xofs = buf;     //new int[w];
    int* yofs = buf + w; //new int[h];

    short* ialpha = (short*)(buf + w + h);    //new short[w * 2];
    short* ibeta = (short*)(buf + w + h + w); //new short[h * 2];

    float fx;
    float fy;
    int sx;
    int sy;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X + (X >= 0.f ? 0.5f : -0.5f)), SHRT_MIN), SHRT_MAX);

    for (int dx = 0; dx < w; dx++)
    {
        fx = (float)((dx + 0.5) * scale_x - 0.5);
        sx = static_cast<int>(floor(fx));
        fx -= sx;

        if (sx < 0)
        {
            sx = 0;
            fx = 0.f;
        }
        if (sx >= srcw - 1)
        {
            sx = srcw - 2;
            fx = 1.f;
        }

        xofs[dx] = sx * 4;

        float a0 = (1.f - fx) * INTER_RESIZE_COEF_SCALE;
        float a1 = fx * INTER_RESIZE_COEF_SCALE;

        ialpha[dx * 2] = SATURATE_CAST_SHORT(a0);
        ialpha[dx * 2 + 1] = SATURATE_CAST_SHORT(a1);
    }

    for (int dy = 0; dy < h; dy++)
    {
        fy = (float)((dy + 0.5) * scale_y - 0.5);
        sy = static_cast<int>(floor(fy));
        fy -= sy;

        if (sy < 0)
        {
            sy = 0;
            fy = 0.f;
        }
        if (sy >= srch - 1)
        {
            sy = srch - 2;
            fy = 1.f;
        }

        yofs[dy] = sy;

        float b0 = (1.f - fy) * INTER_RESIZE_COEF_SCALE;
        float b1 = fy * INTER_RESIZE_COEF_SCALE;

        ibeta[dy * 2] = SATURATE_CAST_SHORT(b0);
        ibeta[dy * 2 + 1] = SATURATE_CAST_SHORT(b1);
    }

#undef SATURATE_CAST_SHORT

    // loop body
    ImGui::ImMat rowsbuf0(w * 4, (size_t)2u);
    ImGui::ImMat rowsbuf1(w * 4, (size_t)2u);
    short* rows0 = (short*)rowsbuf0.data;
    short* rows1 = (short*)rowsbuf1.data;

    int prev_sy1 = -2;

    for (int dy = 0; dy < h; dy++)
    {
        sy = yofs[dy];

        if (sy == prev_sy1)
        {
            // reuse all rows
        }
        else if (sy == prev_sy1 + 1)
        {
            // hresize one row
            short* rows0_old = rows0;
            rows0 = rows1;
            rows1 = rows0_old;
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0 = vdup_n_s16(a0);
                int16x4_t _a1 = vdup_n_s16(a1);
                uint8x8_t _S1 = vld1_u8(S1p);
                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S1low = vget_low_s16(_S116);
                int16x4_t _S1high = vget_high_s16(_S116);
                int32x4_t _rows1 = vmull_s16(_S1low, _a0);
                _rows1 = vmlal_s16(_rows1, _S1high, _a1);
                int16x4_t _rows1_sr4 = vshrn_n_s32(_rows1, 4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                rows1p[0] = (S1p[0] * a0 + S1p[4] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[5] * a1) >> 4;
                rows1p[2] = (S1p[2] * a0 + S1p[6] * a1) >> 4;
                rows1p[3] = (S1p[3] * a0 + S1p[7] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows1p += 4;
            }
        }
        else
        {
            // hresize two rows
            const unsigned char* S0 = src + srcstride * (sy);
            const unsigned char* S1 = src + srcstride * (sy + 1);

            const short* ialphap = ialpha;
            short* rows0p = rows0;
            short* rows1p = rows1;
            for (int dx = 0; dx < w; dx++)
            {
                sx = xofs[dx];
                short a0 = ialphap[0];
                short a1 = ialphap[1];

                const unsigned char* S0p = S0 + sx;
                const unsigned char* S1p = S1 + sx;
#if __ARM_NEON || __SSE__ || __AVX__
                int16x4_t _a0 = vdup_n_s16(a0);
                int16x4_t _a1 = vdup_n_s16(a1);
                uint8x8_t _S0 = vld1_u8(S0p);
                uint8x8_t _S1 = vld1_u8(S1p);
                int16x8_t _S016 = vreinterpretq_s16_u16(vmovl_u8(_S0));
                int16x8_t _S116 = vreinterpretq_s16_u16(vmovl_u8(_S1));
                int16x4_t _S0low = vget_low_s16(_S016);
                int16x4_t _S1low = vget_low_s16(_S116);
                int16x4_t _S0high = vget_high_s16(_S016);
                int16x4_t _S1high = vget_high_s16(_S116);
                int32x4_t _rows0 = vmull_s16(_S0low, _a0);
                int32x4_t _rows1 = vmull_s16(_S1low, _a0);
                _rows0 = vmlal_s16(_rows0, _S0high, _a1);
                _rows1 = vmlal_s16(_rows1, _S1high, _a1);
                int16x4_t _rows0_sr4 = vshrn_n_s32(_rows0, 4);
                int16x4_t _rows1_sr4 = vshrn_n_s32(_rows1, 4);
                vst1_s16(rows0p, _rows0_sr4);
                vst1_s16(rows1p, _rows1_sr4);
#else
                rows0p[0] = (S0p[0] * a0 + S0p[4] * a1) >> 4;
                rows0p[1] = (S0p[1] * a0 + S0p[5] * a1) >> 4;
                rows0p[2] = (S0p[2] * a0 + S0p[6] * a1) >> 4;
                rows0p[3] = (S0p[3] * a0 + S0p[7] * a1) >> 4;
                rows1p[0] = (S1p[0] * a0 + S1p[4] * a1) >> 4;
                rows1p[1] = (S1p[1] * a0 + S1p[5] * a1) >> 4;
                rows1p[2] = (S1p[2] * a0 + S1p[6] * a1) >> 4;
                rows1p[3] = (S1p[3] * a0 + S1p[7] * a1) >> 4;
#endif // __ARM_NEON || __SSE__ || __AVX__

                ialphap += 2;
                rows0p += 4;
                rows1p += 4;
            }
        }

        prev_sy1 = sy;

        // vresize
        short b0 = ibeta[0];
        short b1 = ibeta[1];

        short* rows0p = rows0;
        short* rows1p = rows1;
        unsigned char* Dp = dst + stride * (dy);

#if __ARM_NEON || __SSE__ || __AVX__
        int nn = (w * 4) >> 3;
#else
        int nn = 0;
#endif
        int remain = (w * 4) - (nn << 3);

#if __ARM_NEON || __SSE__ || __AVX__
#if __aarch64__ || __SSE__ || __AVX__
        int16x4_t _b0 = vdup_n_s16(b0);
        int16x4_t _b1 = vdup_n_s16(b1);
        int32x4_t _v2 = vdupq_n_s32(2);
        for (; nn > 0; nn--)
        {
            int16x4_t _rows0p_sr4 = vld1_s16(rows0p);
            int16x4_t _rows1p_sr4 = vld1_s16(rows1p);
            int16x4_t _rows0p_1_sr4 = vld1_s16(rows0p + 4);
            int16x4_t _rows1p_1_sr4 = vld1_s16(rows1p + 4);

            int32x4_t _rows0p_sr4_mb0 = vmull_s16(_rows0p_sr4, _b0);
            int32x4_t _rows1p_sr4_mb1 = vmull_s16(_rows1p_sr4, _b1);
            int32x4_t _rows0p_1_sr4_mb0 = vmull_s16(_rows0p_1_sr4, _b0);
            int32x4_t _rows1p_1_sr4_mb1 = vmull_s16(_rows1p_1_sr4, _b1);

            int32x4_t _acc = _v2;
            _acc = vsraq_n_s32(_acc, _rows0p_sr4_mb0, 16);
            _acc = vsraq_n_s32(_acc, _rows1p_sr4_mb1, 16);

            int32x4_t _acc_1 = _v2;
            _acc_1 = vsraq_n_s32(_acc_1, _rows0p_1_sr4_mb0, 16);
            _acc_1 = vsraq_n_s32(_acc_1, _rows1p_1_sr4_mb1, 16);

            int16x4_t _acc16 = vshrn_n_s32(_acc, 2);
            int16x4_t _acc16_1 = vshrn_n_s32(_acc_1, 2);

            uint8x8_t _D = vqmovun_s16(vcombine_s16(_acc16, _acc16_1));

            vst1_u8(Dp, _D);

            Dp += 8;
            rows0p += 8;
            rows1p += 8;
        }
#else
        if (nn > 0)
        {
            asm volatile(
                "vdup.s16   d16, %8         \n"
                "mov        r4, #2          \n"
                "vdup.s16   d17, %9         \n"
                "vdup.s32   q12, r4         \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "0:                         \n"
                "vmull.s16  q0, d2, d16     \n"
                "vmull.s16  q1, d3, d16     \n"
                "vorr.s32   q10, q12, q12   \n"
                "vorr.s32   q11, q12, q12   \n"
                "vmull.s16  q2, d6, d17     \n"
                "vmull.s16  q3, d7, d17     \n"
                "vsra.s32   q10, q0, #16    \n"
                "vsra.s32   q11, q1, #16    \n"
                "pld        [%0, #128]      \n"
                "vld1.s16   {d2-d3}, [%0 :128]!\n"
                "vsra.s32   q10, q2, #16    \n"
                "vsra.s32   q11, q3, #16    \n"
                "pld        [%1, #128]      \n"
                "vld1.s16   {d6-d7}, [%1 :128]!\n"
                "vshrn.s32  d20, q10, #2    \n"
                "vshrn.s32  d21, q11, #2    \n"
                "vqmovun.s16 d20, q10        \n"
                "vst1.8     {d20}, [%2]!    \n"
                "subs       %3, #1          \n"
                "bne        0b              \n"
                "sub        %0, #16         \n"
                "sub        %1, #16         \n"
                : "=r"(rows0p), // %0
                "=r"(rows1p), // %1
                "=r"(Dp),     // %2
                "=r"(nn)      // %3
                : "0"(rows0p),
                "1"(rows1p),
                "2"(Dp),
                "3"(nn),
                "r"(b0), // %8
                "r"(b1)  // %9
                : "cc", "memory", "r4", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12");
        }
#endif // __aarch64__ || __SSE__ || __AVX__
#endif // __ARM_NEON || __SSE__ || __AVX__
        for (; remain; --remain)
        {
            //             D[x] = (rows0[x]*b0 + rows1[x]*b1) >> INTER_RESIZE_COEF_BITS;
            *Dp++ = (unsigned char)(((short)((b0 * (short)(*rows0p++)) >> 16) + (short)((b1 * (short)(*rows1p++)) >> 16) + 2) >> 2);
        }

        ibeta += 2;
    }

    delete[] buf;
}

static void resize_bilinear_c1(const unsigned char* src, int srcw, int srch, unsigned char* dst, int w, int h)
{
    return resize_bilinear_c1(src, srcw, srch, srcw, dst, w, h, w);
}

static void resize_bilinear_c2(const unsigned char* src, int srcw, int srch, unsigned char* dst, int w, int h)
{
    return resize_bilinear_c2(src, srcw, srch, srcw * 2, dst, w, h, w * 2);
}

static void resize_bilinear_c3(const unsigned char* src, int srcw, int srch, unsigned char* dst, int w, int h)
{
    return resize_bilinear_c3(src, srcw, srch, srcw * 3, dst, w, h, w * 3);
}

static void resize_bilinear_c4(const unsigned char* src, int srcw, int srch, unsigned char* dst, int w, int h)
{
    return resize_bilinear_c4(src, srcw, srch, srcw * 4, dst, w, h, w * 4);
}

ImMat MatResize(const ImMat& mat, const ImSize size, float sw, float sh)
{
    ImMat dst;
    int srcw = mat.w;
    int srch = mat.h;

    int w = size.w;
    int h = size.h;

    if (w == 0 || h == 0)
    {
        w = srcw * sw;
        h = srch * sh;
    }

    if (w == 0 || h == 0)
        return dst;

    if (w == srcw && h == srch)
    {
        dst = mat.clone();
        return dst;
    }

    dst.create(w, h, mat.c, 1u, mat.c);

    if (mat.c == 1)
        resize_bilinear_c1((const unsigned char*)mat.data, srcw, srch, (unsigned char*)dst.data, w, h);
    else if (mat.c == 2)
        resize_bilinear_c2((const unsigned char*)mat.data, srcw, srch, (unsigned char*)dst.data, w, h);
    else if (mat.c == 3)
        resize_bilinear_c3((const unsigned char*)mat.data, srcw, srch, (unsigned char*)dst.data, w, h);
    else if (mat.c == 4)
        resize_bilinear_c4((const unsigned char*)mat.data, srcw, srch, (unsigned char*)dst.data, w, h);

    return dst;
}

ImMat GrayToImage(const ImMat& mat)
{
    ImMat dst;
    if (mat.c != 1 || mat.device != IM_DD_CPU)
        return dst;
    dst.create_type(mat.w, mat.h, 4, mat.type);
    dst.elempack = 4;
    for (int row = 0; row < mat.h; row++)
    {
        for (int col = 0; col < mat.w; col++)
        {
            switch (mat.type)
            {
                case IM_DT_INT8:
                    dst.at<uint8_t>(col, row, 0) = mat.at<uint8_t>(col, row);
                    dst.at<uint8_t>(col, row, 1) = mat.at<uint8_t>(col, row);
                    dst.at<uint8_t>(col, row, 2) = mat.at<uint8_t>(col, row);
                    dst.at<uint8_t>(col, row, 3) = UINT8_MAX;
                break;
                case IM_DT_INT16:
                    dst.at<uint16_t>(col, row, 0) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 1) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 2) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 3) = UINT16_MAX;
                break;
                case IM_DT_INT32:
                    dst.at<uint32_t>(col, row, 0) = mat.at<uint32_t>(col, row);
                    dst.at<uint32_t>(col, row, 1) = mat.at<uint32_t>(col, row);
                    dst.at<uint32_t>(col, row, 2) = mat.at<uint32_t>(col, row);
                    dst.at<uint32_t>(col, row, 3) = UINT32_MAX;
                break;
                case IM_DT_INT64:
                    dst.at<uint64_t>(col, row, 0) = mat.at<uint64_t>(col, row);
                    dst.at<uint64_t>(col, row, 1) = mat.at<uint64_t>(col, row);
                    dst.at<uint64_t>(col, row, 2) = mat.at<uint64_t>(col, row);
                    dst.at<uint64_t>(col, row, 3) = UINT64_MAX;
                break;
                case IM_DT_FLOAT16:
                    dst.at<uint16_t>(col, row, 0) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 1) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 2) = mat.at<uint16_t>(col, row);
                    dst.at<uint16_t>(col, row, 3) = im_float32_to_float16(1.0);
                break;
                case IM_DT_FLOAT32:
                    dst.at<float>(col, row, 0) = mat.at<float>(col, row);
                    dst.at<float>(col, row, 1) = mat.at<float>(col, row);
                    dst.at<float>(col, row, 2) = mat.at<float>(col, row);
                    dst.at<float>(col, row, 3) = 1.0f;
                break;
                case IM_DT_FLOAT64:
                    dst.at<double>(col, row, 0) = mat.at<double>(col, row);
                    dst.at<double>(col, row, 1) = mat.at<double>(col, row);
                    dst.at<double>(col, row, 2) = mat.at<double>(col, row);
                    dst.at<double>(col, row, 3) = 1.0;
                break;
                default: break;
            }
        }
    }
    return dst;
}

static const ImVec2 GetTextSize(const ImWchar c)
{
    ImFontAtlas* atlas = GetIO().Fonts;
    const ImFontGlyph* glyph = GetCurrentContext()->Font->FindGlyph(c);
    if (glyph == NULL)
        return ImVec2(0, 0);
    const int V1 = (int)(glyph->V1 * atlas->TexHeight);
    const int V0 = (int)(glyph->V0 * atlas->TexHeight);
    float scale_x = c < 0x80 ? 2.0 : 1.0;
    float scale_y = c < 0x80 ? 2.0 : 1.0;
    float width = glyph->X0 * scale_x + glyph->AdvanceX * scale_x;
    float height = glyph->Y0 * scale_y + V1 - V0;
    return ImVec2(width, height);
}

static const unsigned char* GetTextData(const ImWchar c, ImVec2& size, ImVec4& rect, int& output_stride, int& char_width, int& char_height)
{
    ImFontAtlas* atlas = GetIO().Fonts;
    float scale_x = c < 0x80 ? 2.0 : 1.0;
    float scale_y = c < 0x80 ? 2.0 : 1.0;
    unsigned char* bitmap;
    int _out_width, _out_height;
    atlas->GetTexDataAsAlpha8(&bitmap, &_out_width, &_out_height);
    const ImFontGlyph* glyph = GetCurrentContext()->Font->FindGlyph(c);
    if (glyph == NULL)
        return nullptr;
    const int U1 = (int)(glyph->U1 * _out_width);
    const int U0 = (int)(glyph->U0 * _out_width);
    const int V1 = (int)(glyph->V1 * _out_height);
    const int V0 = (int)(glyph->V0 * _out_height);
    const unsigned char * ptr = &bitmap[_out_width * V0 + U0];
    output_stride = _out_width;
    size.x = U1 - U0;
    size.y = V1 - V0;
    rect.x = glyph->X0 * scale_x;
    rect.y = glyph->Y0 * scale_y;
    rect.z = glyph->X1 * scale_x;
    rect.w = glyph->Y1 * scale_y;
    char_width = rect.x + glyph->AdvanceX * scale_x;
    char_height = glyph->Y0 * scale_y + V1 - V0;
    return ptr;
}

void DrawTextToMat(ImMat& mat, const ImPoint pos, const char* str, const ImPixel& color, float scale)
{
    int start_x = pos.x;
    int start_y = pos.y;
    const char* str_ptr = str;
    const char* str_end = str_ptr + strlen(str);
    while (str_ptr < str_end)
    {
        unsigned int c = *str_ptr;
        if (c < 0x80)
            str_ptr += 1;
        else
            str_ptr += ImTextCharFromUtf8(&c, str_ptr, str_end);
        if (c < 32)
        {
            if (c == '\n')
            {
                start_x = pos.x;
                start_y += GetFontSize() * scale;
                continue;
            }
            if (c == '\r')
                continue;
        }
        float scale_internal = c < 0x80 ? 0.5 : 1.0;
        int output_stride = 0, char_width = 0, char_height = 0;
        ImVec2 size = {0, 0};
        ImVec4 rect = {0, 0, 0, 0};
        const unsigned char* out_data = GetTextData(c, size, rect, output_stride, char_width, char_height);
        if (out_data && output_stride)
        {
            ImMat char_mat(char_width, char_height, 4, 1u, 4);
            float x1 = rect.x;
            float x2 = rect.z;
            float y1 = rect.y;
            float y2 = rect.w;
            
            for (int x = 0; x < size.x; x++)
            {
                for (int y = 0; y < size.y; y++)
                {
                    const unsigned char alpha = out_data[y * output_stride + x] > 32 ? out_data[y * output_stride + x] : 0;
                    char_mat.set_pixel(ImPoint(x + x1, y + y1), ImPixel(color.r, color.g, color.b, alpha / 255.0));
                }
            }
            auto scale_mat = MatResize(char_mat, ImSize(char_width * scale * scale_internal, GetFontSize() * scale));
            ImageMatCopyTo(scale_mat, mat, ImPoint(start_x, start_y));
            start_x += ceil(char_width) * scale * scale_internal;
        }
    }
}

ImMat CreateTextMat(const char* str, const ImPixel& color, float scale, bool square)
{
    ImMat dst;
    if (!str || strlen(str) == 0)
        return dst;

    int lines = 1;
    float line_width = 0;
    float max_line_width = -1;
    const char* str_ptr = str;
    const char* str_end = str_ptr + strlen(str);
    while (str_ptr < str_end)
    {
        unsigned int c = *str_ptr;
        if (c < 0x80)
            str_ptr += 1;
        else
            str_ptr += ImTextCharFromUtf8(&c, str_ptr, str_end);
        if (c < 32)
        {
            if (c == '\n')
            {
                if (max_line_width < line_width) max_line_width = line_width;
                line_width = 0;
                lines++;
                continue;
            }
            if (c == '\r')
                continue;
        }
        float scale_internal = c < 0x80 ? 0.5 : 1.0;
        auto char_size = GetTextSize(c);
        line_width += ceil(char_size.x) * scale_internal;
        if (max_line_width < line_width) max_line_width = line_width;
    }

    float text_height = GetFontSize() * lines * scale;
    float text_width = max_line_width * scale;
    float x_offset = 0;
    float y_offset = 0;
    if (square)
    {
        float length = fmax(text_height, text_width);
        x_offset = (length - text_width) / 2;
        y_offset = (length - text_height) / 2;
        text_height = text_width = length;
    }
    dst.create(ceil(text_width), ceil(text_height), 4, 1u, 4);
    DrawTextToMat(dst, ImPoint(x_offset, y_offset), str, color, scale);
    return dst;
}

void ImageMatCopyTo(const ImMat& src, ImMat& dst, ImPoint pos)
{
    if (src.empty() || dst.empty())
        return;
    for (int x = 0; x < src.w; x++)
    {
        for (int y = 0; y < src.h; y++)
        {
            ImPixel pixel = src.get_pixel(x, y);
            if (pixel.a > 0)
                dst.set_pixel((int)(pos.x + x), (int)(pos.y + y), pixel);
        }
    }
}

// mat warpaffine
static void warpaffine_bilinear_c1(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride, const float* tm, int type = 0, unsigned int v = 0)
{
    const unsigned char* border_color = (const unsigned char*)&v;
    const int wgap = stride - w;

    const unsigned char* src0 = src;
    unsigned char* dst0 = dst;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X), SHRT_MIN), SHRT_MAX)
#define SATURATE_CAST_INT(X)   (int)::std::min(::std::max((int)((X) + ((X) >= 0.f ? 0.5f : -0.5f)), INT_MIN), INT_MAX)

    std::vector<int> adelta(w);
    std::vector<int> bdelta(w);
    for (int x = 0; x < w; x++)
    {
        adelta[x] = SATURATE_CAST_INT(tm[0] * x * (1 << 10));
        bdelta[x] = SATURATE_CAST_INT(tm[3] * x * (1 << 10));
    }

    int y = 0;
    for (; y < h; y++)
    {
        int X0 = SATURATE_CAST_INT((tm[1] * y + tm[2]) * (1 << 10));
        int Y0 = SATURATE_CAST_INT((tm[4] * y + tm[5]) * (1 << 10));

        int x = 0;
        for (; x + 7 < w; x += 8)
        {
            int sxy_inout = 0;
            {
                int X_0 = X0 + adelta[x];
                int Y_0 = Y0 + bdelta[x];
                int X_7 = X0 + adelta[x + 7];
                int Y_7 = Y0 + bdelta[x + 7];

                short sx_0 = SATURATE_CAST_SHORT((X_0 >> 10));
                short sy_0 = SATURATE_CAST_SHORT((Y_0 >> 10));
                short sx_7 = SATURATE_CAST_SHORT((X_7 >> 10));
                short sy_7 = SATURATE_CAST_SHORT((Y_7 >> 10));

                if (((unsigned short)sx_0 < srcw - 1 && (unsigned short)sy_0 < srch - 1) && ((unsigned short)sx_7 < srcw - 1 && (unsigned short)sy_7 < srch - 1))
                {
                    // all inside
                    sxy_inout = 1;
                }
                else if ((sx_0 < -1 && sx_7 < -1) || (sx_0 >= srcw && sx_7 >= srcw) || (sy_0 < -1 && sy_7 < -1) || (sy_0 >= srch && sy_7 >= srch))
                {
                    // all outside
                    sxy_inout = 2;
                }
            }

            if (sxy_inout == 1)
            {
                // all inside
#if __ARM_NEON || __SSE__ || __AVX__
                int32x4_t _Xl = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x));
                int32x4_t _Xh = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x + 4));
                int32x4_t _Yl = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x));
                int32x4_t _Yh = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x + 4));

                int16x4_t _sxl = vqshrn_n_s32(_Xl, 10);
                int16x4_t _sxh = vqshrn_n_s32(_Xh, 10);
                int16x4_t _syl = vqshrn_n_s32(_Yl, 10);
                int16x4_t _syh = vqshrn_n_s32(_Yh, 10);

                uint32x4_t _v1024m1 = vdupq_n_u32((1 << 10) - 1);
                uint16x8_t _fx = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xh), _v1024m1)));
                uint16x8_t _fy = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yh), _v1024m1)));

                uint16x8_t _alpha0 = vsubq_u16(vdupq_n_u16(1 << 10), _fx);
                uint16x8_t _alpha1 = _fx;
                uint16x8_t _beta0 = vsubq_u16(vdupq_n_u16(1 << 10), _fy);
                uint16x8_t _beta1 = _fy;

                int16x4_t _srcstride = vdup_n_s16(srcstride);

                int32x4_t _a0l = vaddw_s16(vmull_s16(_srcstride, _syl), _sxl);
                int32x4_t _a0h = vaddw_s16(vmull_s16(_srcstride, _syh), _sxh);
                int32x4_t _b0l = vaddw_s16(_a0l, _srcstride);
                int32x4_t _b0h = vaddw_s16(_a0h, _srcstride);

                uint8x8x2_t _a0a1 = uint8x8x2_t();
                uint8x8x2_t _b0b1 = uint8x8x2_t();
                {
                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0l, 0), _a0a1, 0);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0l, 0), _b0b1, 0);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0l, 1), _a0a1, 1);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0l, 1), _b0b1, 1);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0l, 2), _a0a1, 2);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0l, 2), _b0b1, 2);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0l, 3), _a0a1, 3);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0l, 3), _b0b1, 3);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0h, 0), _a0a1, 4);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0h, 0), _b0b1, 4);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0h, 1), _a0a1, 5);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0h, 1), _b0b1, 5);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0h, 2), _a0a1, 6);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0h, 2), _b0b1, 6);

                    _a0a1 = vld2_lane_u8(src0 + vgetq_lane_s32(_a0h, 3), _a0a1, 7);
                    _b0b1 = vld2_lane_u8(src0 + vgetq_lane_s32(_b0h, 3), _b0b1, 7);
                }

                uint16x8_t _a0_0 = vmovl_u8(_a0a1.val[0]);
                uint16x8_t _a1_0 = vmovl_u8(_a0a1.val[1]);
                uint16x8_t _b0_0 = vmovl_u8(_b0b1.val[0]);
                uint16x8_t _b1_0 = vmovl_u8(_b0b1.val[1]);

                uint16x4_t _a00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_0), vget_low_u16(_alpha0)), vget_low_u16(_a1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_0), vget_high_u16(_alpha0)), vget_high_u16(_a1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_0), vget_low_u16(_alpha0)), vget_low_u16(_b1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_0), vget_high_u16(_alpha0)), vget_high_u16(_b1_0), vget_high_u16(_alpha1)), 5);

                uint16x4_t _dst_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0l, vget_low_u16(_beta0)), _b00_0l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0h, vget_high_u16(_beta0)), _b00_0h, vget_high_u16(_beta1)), 15);

                uint8x8_t _dst = vqmovn_u16(vcombine_u16(_dst_0l, _dst_0h));

                vst1_u8(dst0, _dst);

                dst0 += 8;
#else
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    short fx = X & ((1 << 10) - 1);
                    short fy = Y & ((1 << 10) - 1);

                    short alpha0 = (1 << 10) - fx;
                    short alpha1 = fx;

                    short beta0 = (1 << 10) - fy;
                    short beta1 = fy;

                    const unsigned char* a0 = src0 + srcstride * sy + sx;
                    const unsigned char* a1 = src0 + srcstride * sy + sx + 1;
                    const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx;
                    const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx + 1;

                    dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);

                    dst0 += 1;
                }
#endif // __ARM_NEON || __SSE__ || __AVX__
            }
            else if (sxy_inout == 2)
            {
                // all outside
                if (type != -233)
                {
#if __ARM_NEON || __SSE__ || __AVX__
                    uint8x8_t _border_color = vdup_n_u8(border_color[0]);
                    vst1_u8(dst0, _border_color);
#else
                    for (int xi = 0; xi < 8; xi++)
                    {
                        dst0[xi] = border_color[0];
                    }
#endif // __ARM_NEON || __SSE__ || __AVX__
                }
                else
                {
                    // skip
                }

                dst0 += 8;
            }
            else // if (sxy_inout == 0)
            {
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
                    {
                        dst0[0] = border_color[0];
                    }
                    else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
                    {
                        // skip
                    }
                    else
                    {
                        short fx = X & ((1 << 10) - 1);
                        short fy = Y & ((1 << 10) - 1);

                        short alpha0 = (1 << 10) - fx;
                        short alpha1 = fx;

                        short beta0 = (1 << 10) - fy;
                        short beta1 = fy;

                        short sx1 = sx + 1;
                        short sy1 = sy + 1;

                        const unsigned char* a0 = src0 + srcstride * sy + sx;
                        const unsigned char* a1 = src0 + srcstride * sy + sx + 1;
                        const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx;
                        const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx + 1;

                        if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                        {
                            a0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                        {
                            a1 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b1 = type != -233 ? border_color : dst0;
                        }

                        dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                    }

                    dst0 += 1;
                }
            }
        }
        for (; x < w; x++)
        {
            int X = X0 + adelta[x];
            int Y = Y0 + bdelta[x];

            short sx = SATURATE_CAST_SHORT((X >> 10));
            short sy = SATURATE_CAST_SHORT((Y >> 10));

            if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
            {
                dst0[0] = border_color[0];
            }
            else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
            {
                // skip
            }
            else
            {
                short fx = X & ((1 << 10) - 1);
                short fy = Y & ((1 << 10) - 1);

                short alpha0 = (1 << 10) - fx;
                short alpha1 = fx;

                short beta0 = (1 << 10) - fy;
                short beta1 = fy;

                short sx1 = sx + 1;
                short sy1 = sy + 1;

                const unsigned char* a0 = src0 + srcstride * sy + sx;
                const unsigned char* a1 = src0 + srcstride * sy + sx + 1;
                const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx;
                const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx + 1;

                if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                {
                    a0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                {
                    a1 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                {
                    b0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                {
                    b1 = type != -233 ? border_color : dst0;
                }

                dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
            }

            dst0 += 1;
        }

        dst0 += wgap;
    }

#undef SATURATE_CAST_SHORT
#undef SATURATE_CAST_INT
}

static void warpaffine_bilinear_c2(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride, const float* tm, int type = 0, unsigned int v = 0)
{
    const unsigned char* border_color = (const unsigned char*)&v;
    const int wgap = stride - w * 2;

    const unsigned char* src0 = src;
    unsigned char* dst0 = dst;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X), SHRT_MIN), SHRT_MAX)
#define SATURATE_CAST_INT(X)   (int)::std::min(::std::max((int)((X) + ((X) >= 0.f ? 0.5f : -0.5f)), INT_MIN), INT_MAX)

    std::vector<int> adelta(w);
    std::vector<int> bdelta(w);
    for (int x = 0; x < w; x++)
    {
        adelta[x] = SATURATE_CAST_INT(tm[0] * x * (1 << 10));
        bdelta[x] = SATURATE_CAST_INT(tm[3] * x * (1 << 10));
    }

    int y = 0;
    for (; y < h; y++)
    {
        int X0 = SATURATE_CAST_INT((tm[1] * y + tm[2]) * (1 << 10));
        int Y0 = SATURATE_CAST_INT((tm[4] * y + tm[5]) * (1 << 10));

        int x = 0;
        for (; x + 7 < w; x += 8)
        {
            int sxy_inout = 0;
            {
                int X_0 = X0 + adelta[x];
                int Y_0 = Y0 + bdelta[x];
                int X_7 = X0 + adelta[x + 7];
                int Y_7 = Y0 + bdelta[x + 7];

                short sx_0 = SATURATE_CAST_SHORT((X_0 >> 10));
                short sy_0 = SATURATE_CAST_SHORT((Y_0 >> 10));
                short sx_7 = SATURATE_CAST_SHORT((X_7 >> 10));
                short sy_7 = SATURATE_CAST_SHORT((Y_7 >> 10));

                if (((unsigned short)sx_0 < srcw - 1 && (unsigned short)sy_0 < srch - 1) && ((unsigned short)sx_7 < srcw - 1 && (unsigned short)sy_7 < srch - 1))
                {
                    // all inside
                    sxy_inout = 1;
                }
                else if ((sx_0 < -1 && sx_7 < -1) || (sx_0 >= srcw && sx_7 >= srcw) || (sy_0 < -1 && sy_7 < -1) || (sy_0 >= srch && sy_7 >= srch))
                {
                    // all outside
                    sxy_inout = 2;
                }
            }

            if (sxy_inout == 1)
            {
                // all inside
#if __ARM_NEON || __SSE__ || __AVX__
                int32x4_t _Xl = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x));
                int32x4_t _Xh = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x + 4));
                int32x4_t _Yl = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x));
                int32x4_t _Yh = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x + 4));

                int16x4_t _sxl = vqshrn_n_s32(_Xl, 10);
                int16x4_t _sxh = vqshrn_n_s32(_Xh, 10);
                int16x4_t _syl = vqshrn_n_s32(_Yl, 10);
                int16x4_t _syh = vqshrn_n_s32(_Yh, 10);

                uint32x4_t _v1024m1 = vdupq_n_u32((1 << 10) - 1);
                uint16x8_t _fx = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xh), _v1024m1)));
                uint16x8_t _fy = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yh), _v1024m1)));

                uint16x8_t _alpha0 = vsubq_u16(vdupq_n_u16(1 << 10), _fx);
                uint16x8_t _alpha1 = _fx;
                uint16x8_t _beta0 = vsubq_u16(vdupq_n_u16(1 << 10), _fy);
                uint16x8_t _beta1 = _fy;

                int16x4_t _srcstride = vdup_n_s16(srcstride);
                int16x4_t _v2 = vdup_n_s16(2);

                int32x4_t _a0l = vmlal_s16(vmull_s16(_srcstride, _syl), _sxl, _v2);
                int32x4_t _a0h = vmlal_s16(vmull_s16(_srcstride, _syh), _sxh, _v2);
                int32x4_t _b0l = vaddw_s16(_a0l, _srcstride);
                int32x4_t _b0h = vaddw_s16(_a0h, _srcstride);

                uint8x8x4_t _a0a1 = uint8x8x4_t();
                uint8x8x4_t _b0b1 = uint8x8x4_t();
                {
                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0l, 0), _a0a1, 0);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0l, 0), _b0b1, 0);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0l, 1), _a0a1, 1);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0l, 1), _b0b1, 1);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0l, 2), _a0a1, 2);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0l, 2), _b0b1, 2);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0l, 3), _a0a1, 3);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0l, 3), _b0b1, 3);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0h, 0), _a0a1, 4);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0h, 0), _b0b1, 4);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0h, 1), _a0a1, 5);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0h, 1), _b0b1, 5);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0h, 2), _a0a1, 6);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0h, 2), _b0b1, 6);

                    _a0a1 = vld4_lane_u8(src0 + vgetq_lane_s32(_a0h, 3), _a0a1, 7);
                    _b0b1 = vld4_lane_u8(src0 + vgetq_lane_s32(_b0h, 3), _b0b1, 7);
                }

                uint16x8_t _a0_0 = vmovl_u8(_a0a1.val[0]);
                uint16x8_t _a0_1 = vmovl_u8(_a0a1.val[1]);
                uint16x8_t _a1_0 = vmovl_u8(_a0a1.val[2]);
                uint16x8_t _a1_1 = vmovl_u8(_a0a1.val[3]);
                uint16x8_t _b0_0 = vmovl_u8(_b0b1.val[0]);
                uint16x8_t _b0_1 = vmovl_u8(_b0b1.val[1]);
                uint16x8_t _b1_0 = vmovl_u8(_b0b1.val[2]);
                uint16x8_t _b1_1 = vmovl_u8(_b0b1.val[3]);

                uint16x4_t _a00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_0), vget_low_u16(_alpha0)), vget_low_u16(_a1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_1), vget_low_u16(_alpha0)), vget_low_u16(_a1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_0), vget_high_u16(_alpha0)), vget_high_u16(_a1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_1), vget_high_u16(_alpha0)), vget_high_u16(_a1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_0), vget_low_u16(_alpha0)), vget_low_u16(_b1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_1), vget_low_u16(_alpha0)), vget_low_u16(_b1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_0), vget_high_u16(_alpha0)), vget_high_u16(_b1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_1), vget_high_u16(_alpha0)), vget_high_u16(_b1_1), vget_high_u16(_alpha1)), 5);

                uint16x4_t _dst_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0l, vget_low_u16(_beta0)), _b00_0l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1l, vget_low_u16(_beta0)), _b00_1l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0h, vget_high_u16(_beta0)), _b00_0h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1h, vget_high_u16(_beta0)), _b00_1h, vget_high_u16(_beta1)), 15);

                uint8x8x2_t _dst;
                _dst.val[0] = vqmovn_u16(vcombine_u16(_dst_0l, _dst_0h));
                _dst.val[1] = vqmovn_u16(vcombine_u16(_dst_1l, _dst_1h));

                vst2_u8(dst0, _dst);

                dst0 += 2 * 8;
#else
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    short fx = X & ((1 << 10) - 1);
                    short fy = Y & ((1 << 10) - 1);

                    short alpha0 = (1 << 10) - fx;
                    short alpha1 = fx;

                    short beta0 = (1 << 10) - fy;
                    short beta1 = fy;

                    const unsigned char* a0 = src0 + srcstride * sy + sx * 2;
                    const unsigned char* a1 = src0 + srcstride * sy + sx * 2 + 2;
                    const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 2;
                    const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 2 + 2;

                    dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);

                    dst0 += 2;
                }
#endif // __ARM_NEON || __SSE__ || __AVX__
            }
            else if (sxy_inout == 2)
            {
                // all outside
                if (type != -233)
                {
#if __ARM_NEON || __SSE__ || __AVX__
                    uint8x8x2_t _border_color;
                    _border_color.val[0] = vdup_n_u8(border_color[0]);
                    _border_color.val[1] = vdup_n_u8(border_color[1]);

                    vst2_u8(dst0, _border_color);
#else
                    for (int xi = 0; xi < 8; xi++)
                    {
                        dst0[xi * 2] = border_color[0];
                        dst0[xi * 2 + 1] = border_color[1];
                    }
#endif
                }
                else
                {
                    // skip
                }

                dst0 += 16;
            }
            else // if (sxy_inout == 0)
            {
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
                    {
                        dst0[0] = border_color[0];
                        dst0[1] = border_color[1];
                    }
                    else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
                    {
                        // skip
                    }
                    else
                    {
                        short fx = X & ((1 << 10) - 1);
                        short fy = Y & ((1 << 10) - 1);

                        short alpha0 = (1 << 10) - fx;
                        short alpha1 = fx;

                        short beta0 = (1 << 10) - fy;
                        short beta1 = fy;

                        short sx1 = sx + 1;
                        short sy1 = sy + 1;

                        const unsigned char* a0 = src0 + srcstride * sy + sx * 2;
                        const unsigned char* a1 = src0 + srcstride * sy + sx * 2 + 2;
                        const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 2;
                        const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 2 + 2;

                        if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                        {
                            a0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                        {
                            a1 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b1 = type != -233 ? border_color : dst0;
                        }

                        dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                    }

                    dst0 += 2;
                }
            }
        }
        for (; x < w; x++)
        {
            int X = X0 + adelta[x];
            int Y = Y0 + bdelta[x];

            short sx = SATURATE_CAST_SHORT((X >> 10));
            short sy = SATURATE_CAST_SHORT((Y >> 10));

            if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
            {
                dst0[0] = border_color[0];
                dst0[1] = border_color[1];
            }
            else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
            {
                // skip
            }
            else
            {
                short fx = X & ((1 << 10) - 1);
                short fy = Y & ((1 << 10) - 1);

                short alpha0 = (1 << 10) - fx;
                short alpha1 = fx;

                short beta0 = (1 << 10) - fy;
                short beta1 = fy;

                short sx1 = sx + 1;
                short sy1 = sy + 1;

                const unsigned char* a0 = src0 + srcstride * sy + sx * 2;
                const unsigned char* a1 = src0 + srcstride * sy + sx * 2 + 2;
                const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 2;
                const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 2 + 2;

                if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                {
                    a0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                {
                    a1 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                {
                    b0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                {
                    b1 = type != -233 ? border_color : dst0;
                }

                dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
            }

            dst0 += 2;
        }

        dst0 += wgap;
    }

#undef SATURATE_CAST_SHORT
#undef SATURATE_CAST_INT
}

static void warpaffine_bilinear_c3(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride, const float* tm, int type = 0, unsigned int v = 0)
{
    const unsigned char* border_color = (const unsigned char*)&v;
    const int wgap = stride - w * 3;

    const unsigned char* src0 = src;
    unsigned char* dst0 = dst;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X), SHRT_MIN), SHRT_MAX)
#define SATURATE_CAST_INT(X)   (int)::std::min(::std::max((int)((X) + ((X) >= 0.f ? 0.5f : -0.5f)), INT_MIN), INT_MAX)

    std::vector<int> adelta(w);
    std::vector<int> bdelta(w);
    for (int x = 0; x < w; x++)
    {
        adelta[x] = SATURATE_CAST_INT(tm[0] * x * (1 << 10));
        bdelta[x] = SATURATE_CAST_INT(tm[3] * x * (1 << 10));
    }

    int y = 0;
    for (; y < h; y++)
    {
        int X0 = SATURATE_CAST_INT((tm[1] * y + tm[2]) * (1 << 10));
        int Y0 = SATURATE_CAST_INT((tm[4] * y + tm[5]) * (1 << 10));

        int x = 0;
        for (; x + 7 < w; x += 8)
        {
            int sxy_inout = 0;
            {
                int X_0 = X0 + adelta[x];
                int Y_0 = Y0 + bdelta[x];
                int X_7 = X0 + adelta[x + 7];
                int Y_7 = Y0 + bdelta[x + 7];

                short sx_0 = SATURATE_CAST_SHORT((X_0 >> 10));
                short sy_0 = SATURATE_CAST_SHORT((Y_0 >> 10));
                short sx_7 = SATURATE_CAST_SHORT((X_7 >> 10));
                short sy_7 = SATURATE_CAST_SHORT((Y_7 >> 10));

                if (((unsigned short)sx_0 < srcw - 1 && (unsigned short)sy_0 < srch - 1) && ((unsigned short)sx_7 < srcw - 1 && (unsigned short)sy_7 < srch - 1))
                {
                    // all inside
                    sxy_inout = 1;
                }
                else if ((sx_0 < -1 && sx_7 < -1) || (sx_0 >= srcw && sx_7 >= srcw) || (sy_0 < -1 && sy_7 < -1) || (sy_0 >= srch && sy_7 >= srch))
                {
                    // all outside
                    sxy_inout = 2;
                }
            }

            if (sxy_inout == 1)
            {
                // all inside
#if __ARM_NEON || __SSE__ || __AVX__
                int32x4_t _Xl = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x));
                int32x4_t _Xh = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x + 4));
                int32x4_t _Yl = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x));
                int32x4_t _Yh = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x + 4));

                int16x4_t _sxl = vqshrn_n_s32(_Xl, 10);
                int16x4_t _sxh = vqshrn_n_s32(_Xh, 10);
                int16x4_t _syl = vqshrn_n_s32(_Yl, 10);
                int16x4_t _syh = vqshrn_n_s32(_Yh, 10);

                uint32x4_t _v1024m1 = vdupq_n_u32((1 << 10) - 1);
                uint16x8_t _fx = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xh), _v1024m1)));
                uint16x8_t _fy = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yh), _v1024m1)));

                uint16x8_t _alpha0 = vsubq_u16(vdupq_n_u16(1 << 10), _fx);
                uint16x8_t _alpha1 = _fx;
                uint16x8_t _beta0 = vsubq_u16(vdupq_n_u16(1 << 10), _fy);
                uint16x8_t _beta1 = _fy;

                int16x4_t _srcstride = vdup_n_s16(srcstride);
                int16x4_t _v3 = vdup_n_s16(3);

                int32x4_t _a0l = vmlal_s16(vmull_s16(_srcstride, _syl), _sxl, _v3);
                int32x4_t _a0h = vmlal_s16(vmull_s16(_srcstride, _syh), _sxh, _v3);
                int32x4_t _b0l = vaddw_s16(_a0l, _srcstride);
                int32x4_t _b0h = vaddw_s16(_a0h, _srcstride);
                int32x4_t _a1l = vaddw_s16(_a0l, _v3);
                int32x4_t _a1h = vaddw_s16(_a0h, _v3);
                int32x4_t _b1l = vaddw_s16(_b0l, _v3);
                int32x4_t _b1h = vaddw_s16(_b0h, _v3);

                uint8x8x3_t _a0 = uint8x8x3_t();
                uint8x8x3_t _a1 = uint8x8x3_t();
                uint8x8x3_t _b0 = uint8x8x3_t();
                uint8x8x3_t _b1 = uint8x8x3_t();
                {
                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0l, 0), _a0, 0);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1l, 0), _a1, 0);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0l, 0), _b0, 0);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1l, 0), _b1, 0);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0l, 1), _a0, 1);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1l, 1), _a1, 1);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0l, 1), _b0, 1);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1l, 1), _b1, 1);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0l, 2), _a0, 2);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1l, 2), _a1, 2);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0l, 2), _b0, 2);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1l, 2), _b1, 2);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0l, 3), _a0, 3);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1l, 3), _a1, 3);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0l, 3), _b0, 3);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1l, 3), _b1, 3);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0h, 0), _a0, 4);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1h, 0), _a1, 4);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0h, 0), _b0, 4);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1h, 0), _b1, 4);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0h, 1), _a0, 5);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1h, 1), _a1, 5);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0h, 1), _b0, 5);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1h, 1), _b1, 5);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0h, 2), _a0, 6);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1h, 2), _a1, 6);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0h, 2), _b0, 6);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1h, 2), _b1, 6);

                    _a0 = vld3_lane_u8(src0 + vgetq_lane_s32(_a0h, 3), _a0, 7);
                    _a1 = vld3_lane_u8(src0 + vgetq_lane_s32(_a1h, 3), _a1, 7);
                    _b0 = vld3_lane_u8(src0 + vgetq_lane_s32(_b0h, 3), _b0, 7);
                    _b1 = vld3_lane_u8(src0 + vgetq_lane_s32(_b1h, 3), _b1, 7);
                }

                uint16x8_t _a0_0 = vmovl_u8(_a0.val[0]);
                uint16x8_t _a0_1 = vmovl_u8(_a0.val[1]);
                uint16x8_t _a0_2 = vmovl_u8(_a0.val[2]);
                uint16x8_t _a1_0 = vmovl_u8(_a1.val[0]);
                uint16x8_t _a1_1 = vmovl_u8(_a1.val[1]);
                uint16x8_t _a1_2 = vmovl_u8(_a1.val[2]);
                uint16x8_t _b0_0 = vmovl_u8(_b0.val[0]);
                uint16x8_t _b0_1 = vmovl_u8(_b0.val[1]);
                uint16x8_t _b0_2 = vmovl_u8(_b0.val[2]);
                uint16x8_t _b1_0 = vmovl_u8(_b1.val[0]);
                uint16x8_t _b1_1 = vmovl_u8(_b1.val[1]);
                uint16x8_t _b1_2 = vmovl_u8(_b1.val[2]);

                uint16x4_t _a00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_0), vget_low_u16(_alpha0)), vget_low_u16(_a1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_1), vget_low_u16(_alpha0)), vget_low_u16(_a1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_2), vget_low_u16(_alpha0)), vget_low_u16(_a1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_0), vget_high_u16(_alpha0)), vget_high_u16(_a1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_1), vget_high_u16(_alpha0)), vget_high_u16(_a1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_2), vget_high_u16(_alpha0)), vget_high_u16(_a1_2), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_0), vget_low_u16(_alpha0)), vget_low_u16(_b1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_1), vget_low_u16(_alpha0)), vget_low_u16(_b1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_2), vget_low_u16(_alpha0)), vget_low_u16(_b1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_0), vget_high_u16(_alpha0)), vget_high_u16(_b1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_1), vget_high_u16(_alpha0)), vget_high_u16(_b1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_2), vget_high_u16(_alpha0)), vget_high_u16(_b1_2), vget_high_u16(_alpha1)), 5);

                uint16x4_t _dst_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0l, vget_low_u16(_beta0)), _b00_0l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1l, vget_low_u16(_beta0)), _b00_1l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2l, vget_low_u16(_beta0)), _b00_2l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0h, vget_high_u16(_beta0)), _b00_0h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1h, vget_high_u16(_beta0)), _b00_1h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2h, vget_high_u16(_beta0)), _b00_2h, vget_high_u16(_beta1)), 15);

                uint8x8x3_t _dst;
                _dst.val[0] = vqmovn_u16(vcombine_u16(_dst_0l, _dst_0h));
                _dst.val[1] = vqmovn_u16(vcombine_u16(_dst_1l, _dst_1h));
                _dst.val[2] = vqmovn_u16(vcombine_u16(_dst_2l, _dst_2h));

                vst3_u8(dst0, _dst);

                dst0 += 3 * 8;
#else
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    short fx = X & ((1 << 10) - 1);
                    short fy = Y & ((1 << 10) - 1);

                    short alpha0 = (1 << 10) - fx;
                    short alpha1 = fx;

                    short beta0 = (1 << 10) - fy;
                    short beta1 = fy;

                    const unsigned char* a0 = src0 + srcstride * sy + sx * 3;
                    const unsigned char* a1 = src0 + srcstride * sy + sx * 3 + 3;
                    const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 3;
                    const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 3 + 3;

                    dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);

                    dst0 += 3;
                }
#endif // __ARM_NEON || __SSE__ || __AVX__
            }
            else if (sxy_inout == 2)
            {
                // all outside
                if (type != -233)
                {
#if __ARM_NEON || __SSE__ || __AVX__
                    uint8x8x3_t _border_color;
                    _border_color.val[0] = vdup_n_u8(border_color[0]);
                    _border_color.val[1] = vdup_n_u8(border_color[1]);
                    _border_color.val[2] = vdup_n_u8(border_color[2]);

                    vst3_u8(dst0, _border_color);
#else
                    for (int xi = 0; xi < 8; xi++)
                    {
                        dst0[xi * 3] = border_color[0];
                        dst0[xi * 3 + 1] = border_color[1];
                        dst0[xi * 3 + 2] = border_color[2];
                    }
#endif // __ARM_NEON || __SSE__ || __AVX__
                }
                else
                {
                    // skip
                }

                dst0 += 24;
            }
            else // if (sxy_inout == 0)
            {
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
                    {
                        dst0[0] = border_color[0];
                        dst0[1] = border_color[1];
                        dst0[2] = border_color[2];
                    }
                    else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
                    {
                        // skip
                    }
                    else
                    {
                        short fx = X & ((1 << 10) - 1);
                        short fy = Y & ((1 << 10) - 1);

                        short alpha0 = (1 << 10) - fx;
                        short alpha1 = fx;

                        short beta0 = (1 << 10) - fy;
                        short beta1 = fy;

                        short sx1 = sx + 1;
                        short sy1 = sy + 1;

                        const unsigned char* a0 = src0 + srcstride * sy + sx * 3;
                        const unsigned char* a1 = src0 + srcstride * sy + sx * 3 + 3;
                        const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 3;
                        const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 3 + 3;

                        if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                        {
                            a0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                        {
                            a1 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b1 = type != -233 ? border_color : dst0;
                        }

                        dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                    }

                    dst0 += 3;
                }
            }
        }
        for (; x < w; x++)
        {
            int X = X0 + adelta[x];
            int Y = Y0 + bdelta[x];

            short sx = SATURATE_CAST_SHORT((X >> 10));
            short sy = SATURATE_CAST_SHORT((Y >> 10));

            if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
            {
                dst0[0] = border_color[0];
                dst0[1] = border_color[1];
                dst0[2] = border_color[2];
            }
            else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
            {
                // skip
            }
            else
            {
                short fx = X & ((1 << 10) - 1);
                short fy = Y & ((1 << 10) - 1);

                short alpha0 = (1 << 10) - fx;
                short alpha1 = fx;

                short beta0 = (1 << 10) - fy;
                short beta1 = fy;

                short sx1 = sx + 1;
                short sy1 = sy + 1;

                const unsigned char* a0 = src0 + srcstride * sy + sx * 3;
                const unsigned char* a1 = src0 + srcstride * sy + sx * 3 + 3;
                const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 3;
                const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 3 + 3;

                if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                {
                    a0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                {
                    a1 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                {
                    b0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                {
                    b1 = type != -233 ? border_color : dst0;
                }

                dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
            }

            dst0 += 3;
        }

        dst0 += wgap;
    }

#undef SATURATE_CAST_SHORT
#undef SATURATE_CAST_INT
}

static void warpaffine_bilinear_c4(const unsigned char* src, int srcw, int srch, int srcstride, unsigned char* dst, int w, int h, int stride, const float* tm, int type = 0, unsigned int v = 0)
{
    const unsigned char* border_color = (const unsigned char*)&v;
    const int wgap = stride - w * 4;

    const unsigned char* src0 = src;
    unsigned char* dst0 = dst;

#define SATURATE_CAST_SHORT(X) (short)::std::min(::std::max((int)(X), SHRT_MIN), SHRT_MAX)
#define SATURATE_CAST_INT(X)   (int)::std::min(::std::max((int)((X) + ((X) >= 0.f ? 0.5f : -0.5f)), INT_MIN), INT_MAX)

    std::vector<int> adelta(w);
    std::vector<int> bdelta(w);
    for (int x = 0; x < w; x++)
    {
        adelta[x] = SATURATE_CAST_INT(tm[0] * x * (1 << 10));
        bdelta[x] = SATURATE_CAST_INT(tm[3] * x * (1 << 10));
    }

    int y = 0;
    for (; y < h; y++)
    {
        int X0 = SATURATE_CAST_INT((tm[1] * y + tm[2]) * (1 << 10));
        int Y0 = SATURATE_CAST_INT((tm[4] * y + tm[5]) * (1 << 10));

        int x = 0;
        for (; x + 7 < w; x += 8)
        {
            int sxy_inout = 0;
            {
                int X_0 = X0 + adelta[x];
                int Y_0 = Y0 + bdelta[x];
                int X_7 = X0 + adelta[x + 7];
                int Y_7 = Y0 + bdelta[x + 7];

                short sx_0 = SATURATE_CAST_SHORT((X_0 >> 10));
                short sy_0 = SATURATE_CAST_SHORT((Y_0 >> 10));
                short sx_7 = SATURATE_CAST_SHORT((X_7 >> 10));
                short sy_7 = SATURATE_CAST_SHORT((Y_7 >> 10));

                if (((unsigned short)sx_0 < srcw - 1 && (unsigned short)sy_0 < srch - 1) && ((unsigned short)sx_7 < srcw - 1 && (unsigned short)sy_7 < srch - 1))
                {
                    // all inside
                    sxy_inout = 1;
                }
                else if ((sx_0 < -1 && sx_7 < -1) || (sx_0 >= srcw && sx_7 >= srcw) || (sy_0 < -1 && sy_7 < -1) || (sy_0 >= srch && sy_7 >= srch))
                {
                    // all outside
                    sxy_inout = 2;
                }
            }

            if (sxy_inout == 1)
            {
                // all inside
#if __ARM_NEON || __SSE__ || __AVX__
                int32x4_t _Xl = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x));
                int32x4_t _Xh = vaddq_s32(vdupq_n_s32(X0), vld1q_s32(adelta.data() + x + 4));
                int32x4_t _Yl = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x));
                int32x4_t _Yh = vaddq_s32(vdupq_n_s32(Y0), vld1q_s32(bdelta.data() + x + 4));

                int16x4_t _sxl = vqshrn_n_s32(_Xl, 10);
                int16x4_t _sxh = vqshrn_n_s32(_Xh, 10);
                int16x4_t _syl = vqshrn_n_s32(_Yl, 10);
                int16x4_t _syh = vqshrn_n_s32(_Yh, 10);

                uint32x4_t _v1024m1 = vdupq_n_u32((1 << 10) - 1);
                uint16x8_t _fx = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Xh), _v1024m1)));
                uint16x8_t _fy = vcombine_u16(vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yl), _v1024m1)), vmovn_u32(vandq_u32(vreinterpretq_u32_s32(_Yh), _v1024m1)));

                uint16x8_t _alpha0 = vsubq_u16(vdupq_n_u16(1 << 10), _fx);
                uint16x8_t _alpha1 = _fx;
                uint16x8_t _beta0 = vsubq_u16(vdupq_n_u16(1 << 10), _fy);
                uint16x8_t _beta1 = _fy;

                int16x4_t _srcstride = vdup_n_s16(srcstride);
                int16x4_t _v4 = vdup_n_s16(4);

                int32x4_t _a0l = vmlal_s16(vmull_s16(_srcstride, _syl), _sxl, _v4);
                int32x4_t _a0h = vmlal_s16(vmull_s16(_srcstride, _syh), _sxh, _v4);
                int32x4_t _b0l = vaddw_s16(_a0l, _srcstride);
                int32x4_t _b0h = vaddw_s16(_a0h, _srcstride);

                uint8x8x4_t _a0 = uint8x8x4_t();
                uint8x8x4_t _a1 = uint8x8x4_t();
                uint8x8x4_t _b0 = uint8x8x4_t();
                uint8x8x4_t _b1 = uint8x8x4_t();
                {
                    uint8x8_t _a0a1_0 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 0));
                    uint8x8_t _a0a1_1 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 1));
                    uint8x8_t _a0a1_2 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 2));
                    uint8x8_t _a0a1_3 = vld1_u8(src0 + vgetq_lane_s32(_a0l, 3));
                    uint8x8_t _a0a1_4 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 0));
                    uint8x8_t _a0a1_5 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 1));
                    uint8x8_t _a0a1_6 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 2));
                    uint8x8_t _a0a1_7 = vld1_u8(src0 + vgetq_lane_s32(_a0h, 3));

                    // transpose 8x8
                    uint8x8x2_t _a0a101t_r = vtrn_u8(_a0a1_0, _a0a1_1);
                    uint8x8x2_t _a0a123t_r = vtrn_u8(_a0a1_2, _a0a1_3);
                    uint8x8x2_t _a0a145t_r = vtrn_u8(_a0a1_4, _a0a1_5);
                    uint8x8x2_t _a0a167t_r = vtrn_u8(_a0a1_6, _a0a1_7);

                    uint16x4x2_t _a0a102tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a101t_r.val[0]), vreinterpret_u16_u8(_a0a123t_r.val[0]));
                    uint16x4x2_t _a0a113tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a101t_r.val[1]), vreinterpret_u16_u8(_a0a123t_r.val[1]));
                    uint16x4x2_t _a0a146tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a145t_r.val[0]), vreinterpret_u16_u8(_a0a167t_r.val[0]));
                    uint16x4x2_t _a0a157tt_r = vtrn_u16(vreinterpret_u16_u8(_a0a145t_r.val[1]), vreinterpret_u16_u8(_a0a167t_r.val[1]));

                    uint32x2x2_t _a0a104ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a102tt_r.val[0]), vreinterpret_u32_u16(_a0a146tt_r.val[0]));
                    uint32x2x2_t _a0a115ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a113tt_r.val[0]), vreinterpret_u32_u16(_a0a157tt_r.val[0]));
                    uint32x2x2_t _a0a126ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a102tt_r.val[1]), vreinterpret_u32_u16(_a0a146tt_r.val[1]));
                    uint32x2x2_t _a0a137ttt_r = vtrn_u32(vreinterpret_u32_u16(_a0a113tt_r.val[1]), vreinterpret_u32_u16(_a0a157tt_r.val[1]));

                    _a0.val[0] = vreinterpret_u8_u32(_a0a104ttt_r.val[0]);
                    _a0.val[1] = vreinterpret_u8_u32(_a0a115ttt_r.val[0]);
                    _a0.val[2] = vreinterpret_u8_u32(_a0a126ttt_r.val[0]);
                    _a0.val[3] = vreinterpret_u8_u32(_a0a137ttt_r.val[0]);
                    _a1.val[0] = vreinterpret_u8_u32(_a0a104ttt_r.val[1]);
                    _a1.val[1] = vreinterpret_u8_u32(_a0a115ttt_r.val[1]);
                    _a1.val[2] = vreinterpret_u8_u32(_a0a126ttt_r.val[1]);
                    _a1.val[3] = vreinterpret_u8_u32(_a0a137ttt_r.val[1]);

                    uint8x8_t _b0b1_0 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 0));
                    uint8x8_t _b0b1_1 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 1));
                    uint8x8_t _b0b1_2 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 2));
                    uint8x8_t _b0b1_3 = vld1_u8(src0 + vgetq_lane_s32(_b0l, 3));
                    uint8x8_t _b0b1_4 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 0));
                    uint8x8_t _b0b1_5 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 1));
                    uint8x8_t _b0b1_6 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 2));
                    uint8x8_t _b0b1_7 = vld1_u8(src0 + vgetq_lane_s32(_b0h, 3));

                    // transpose 8x8
                    uint8x8x2_t _b0b101t_r = vtrn_u8(_b0b1_0, _b0b1_1);
                    uint8x8x2_t _b0b123t_r = vtrn_u8(_b0b1_2, _b0b1_3);
                    uint8x8x2_t _b0b145t_r = vtrn_u8(_b0b1_4, _b0b1_5);
                    uint8x8x2_t _b0b167t_r = vtrn_u8(_b0b1_6, _b0b1_7);

                    uint16x4x2_t _b0b102tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b101t_r.val[0]), vreinterpret_u16_u8(_b0b123t_r.val[0]));
                    uint16x4x2_t _b0b113tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b101t_r.val[1]), vreinterpret_u16_u8(_b0b123t_r.val[1]));
                    uint16x4x2_t _b0b146tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b145t_r.val[0]), vreinterpret_u16_u8(_b0b167t_r.val[0]));
                    uint16x4x2_t _b0b157tt_r = vtrn_u16(vreinterpret_u16_u8(_b0b145t_r.val[1]), vreinterpret_u16_u8(_b0b167t_r.val[1]));

                    uint32x2x2_t _b0b104ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b102tt_r.val[0]), vreinterpret_u32_u16(_b0b146tt_r.val[0]));
                    uint32x2x2_t _b0b115ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b113tt_r.val[0]), vreinterpret_u32_u16(_b0b157tt_r.val[0]));
                    uint32x2x2_t _b0b126ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b102tt_r.val[1]), vreinterpret_u32_u16(_b0b146tt_r.val[1]));
                    uint32x2x2_t _b0b137ttt_r = vtrn_u32(vreinterpret_u32_u16(_b0b113tt_r.val[1]), vreinterpret_u32_u16(_b0b157tt_r.val[1]));

                    _b0.val[0] = vreinterpret_u8_u32(_b0b104ttt_r.val[0]);
                    _b0.val[1] = vreinterpret_u8_u32(_b0b115ttt_r.val[0]);
                    _b0.val[2] = vreinterpret_u8_u32(_b0b126ttt_r.val[0]);
                    _b0.val[3] = vreinterpret_u8_u32(_b0b137ttt_r.val[0]);
                    _b1.val[0] = vreinterpret_u8_u32(_b0b104ttt_r.val[1]);
                    _b1.val[1] = vreinterpret_u8_u32(_b0b115ttt_r.val[1]);
                    _b1.val[2] = vreinterpret_u8_u32(_b0b126ttt_r.val[1]);
                    _b1.val[3] = vreinterpret_u8_u32(_b0b137ttt_r.val[1]);
                }

                uint16x8_t _a0_0 = vmovl_u8(_a0.val[0]);
                uint16x8_t _a0_1 = vmovl_u8(_a0.val[1]);
                uint16x8_t _a0_2 = vmovl_u8(_a0.val[2]);
                uint16x8_t _a0_3 = vmovl_u8(_a0.val[3]);
                uint16x8_t _a1_0 = vmovl_u8(_a1.val[0]);
                uint16x8_t _a1_1 = vmovl_u8(_a1.val[1]);
                uint16x8_t _a1_2 = vmovl_u8(_a1.val[2]);
                uint16x8_t _a1_3 = vmovl_u8(_a1.val[3]);
                uint16x8_t _b0_0 = vmovl_u8(_b0.val[0]);
                uint16x8_t _b0_1 = vmovl_u8(_b0.val[1]);
                uint16x8_t _b0_2 = vmovl_u8(_b0.val[2]);
                uint16x8_t _b0_3 = vmovl_u8(_b0.val[3]);
                uint16x8_t _b1_0 = vmovl_u8(_b1.val[0]);
                uint16x8_t _b1_1 = vmovl_u8(_b1.val[1]);
                uint16x8_t _b1_2 = vmovl_u8(_b1.val[2]);
                uint16x8_t _b1_3 = vmovl_u8(_b1.val[3]);

                uint16x4_t _a00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_0), vget_low_u16(_alpha0)), vget_low_u16(_a1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_1), vget_low_u16(_alpha0)), vget_low_u16(_a1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_2), vget_low_u16(_alpha0)), vget_low_u16(_a1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_a0_3), vget_low_u16(_alpha0)), vget_low_u16(_a1_3), vget_low_u16(_alpha1)), 5);
                uint16x4_t _a00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_0), vget_high_u16(_alpha0)), vget_high_u16(_a1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_1), vget_high_u16(_alpha0)), vget_high_u16(_a1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_2), vget_high_u16(_alpha0)), vget_high_u16(_a1_2), vget_high_u16(_alpha1)), 5);
                uint16x4_t _a00_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_a0_3), vget_high_u16(_alpha0)), vget_high_u16(_a1_3), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_0), vget_low_u16(_alpha0)), vget_low_u16(_b1_0), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_1), vget_low_u16(_alpha0)), vget_low_u16(_b1_1), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_2), vget_low_u16(_alpha0)), vget_low_u16(_b1_2), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_low_u16(_b0_3), vget_low_u16(_alpha0)), vget_low_u16(_b1_3), vget_low_u16(_alpha1)), 5);
                uint16x4_t _b00_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_0), vget_high_u16(_alpha0)), vget_high_u16(_b1_0), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_1), vget_high_u16(_alpha0)), vget_high_u16(_b1_1), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_2), vget_high_u16(_alpha0)), vget_high_u16(_b1_2), vget_high_u16(_alpha1)), 5);
                uint16x4_t _b00_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(vget_high_u16(_b0_3), vget_high_u16(_alpha0)), vget_high_u16(_b1_3), vget_high_u16(_alpha1)), 5);

                uint16x4_t _dst_0l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0l, vget_low_u16(_beta0)), _b00_0l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_1l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1l, vget_low_u16(_beta0)), _b00_1l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_2l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2l, vget_low_u16(_beta0)), _b00_2l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_3l = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_3l, vget_low_u16(_beta0)), _b00_3l, vget_low_u16(_beta1)), 15);
                uint16x4_t _dst_0h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_0h, vget_high_u16(_beta0)), _b00_0h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_1h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_1h, vget_high_u16(_beta0)), _b00_1h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_2h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_2h, vget_high_u16(_beta0)), _b00_2h, vget_high_u16(_beta1)), 15);
                uint16x4_t _dst_3h = vqshrn_n_u32(vmlal_u16(vmull_u16(_a00_3h, vget_high_u16(_beta0)), _b00_3h, vget_high_u16(_beta1)), 15);

                uint8x8x4_t _dst;
                _dst.val[0] = vqmovn_u16(vcombine_u16(_dst_0l, _dst_0h));
                _dst.val[1] = vqmovn_u16(vcombine_u16(_dst_1l, _dst_1h));
                _dst.val[2] = vqmovn_u16(vcombine_u16(_dst_2l, _dst_2h));
                _dst.val[3] = vqmovn_u16(vcombine_u16(_dst_3l, _dst_3h));

                vst4_u8(dst0, _dst);

                dst0 += 4 * 8;
#else
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    short fx = X & ((1 << 10) - 1);
                    short fy = Y & ((1 << 10) - 1);

                    short alpha0 = (1 << 10) - fx;
                    short alpha1 = fx;

                    short beta0 = (1 << 10) - fy;
                    short beta1 = fy;

                    const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                    const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                    const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                    const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                    dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                    dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);

                    dst0 += 4;
                }
#endif // __ARM_NEON || __SSE__ || __AVX__
            }
            else if (sxy_inout == 2)
            {
                // all outside
                if (type != -233)
                {
#if __ARM_NEON || __SSE__ || __AVX__
                    uint8x8x4_t _border_color;
                    _border_color.val[0] = vdup_n_u8(border_color[0]);
                    _border_color.val[1] = vdup_n_u8(border_color[1]);
                    _border_color.val[2] = vdup_n_u8(border_color[2]);
                    _border_color.val[3] = vdup_n_u8(border_color[3]);

                    vst4_u8(dst0, _border_color);
#else
                    for (int xi = 0; xi < 8; xi++)
                    {
                        dst0[xi * 4] = border_color[0];
                        dst0[xi * 4 + 1] = border_color[1];
                        dst0[xi * 4 + 2] = border_color[2];
                        dst0[xi * 4 + 3] = border_color[3];
                    }
#endif // __ARM_NEON || __SSE__ || __AVX__
                }
                else
                {
                    // skip
                }

                dst0 += 32;
            }
            else // if (sxy_inout == 0)
            {
                for (int xi = 0; xi < 8; xi++)
                {
                    int X = X0 + adelta[x + xi];
                    int Y = Y0 + bdelta[x + xi];

                    short sx = SATURATE_CAST_SHORT((X >> 10));
                    short sy = SATURATE_CAST_SHORT((Y >> 10));

                    if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
                    {
                        dst0[0] = border_color[0];
                        dst0[1] = border_color[1];
                        dst0[2] = border_color[2];
                        dst0[3] = border_color[3];
                    }
                    else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
                    {
                        // skip
                    }
                    else
                    {
                        short fx = X & ((1 << 10) - 1);
                        short fy = Y & ((1 << 10) - 1);

                        short alpha0 = (1 << 10) - fx;
                        short alpha1 = fx;

                        short beta0 = (1 << 10) - fy;
                        short beta1 = fy;

                        short sx1 = sx + 1;
                        short sy1 = sy + 1;

                        const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                        const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                        const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                        const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                        if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                        {
                            a0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                        {
                            a1 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b0 = type != -233 ? border_color : dst0;
                        }
                        if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                        {
                            b1 = type != -233 ? border_color : dst0;
                        }

                        dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                        dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);
                    }

                    dst0 += 4;
                }
            }
        }
        for (; x < w; x++)
        {
            int X = X0 + adelta[x];
            int Y = Y0 + bdelta[x];

            short sx = SATURATE_CAST_SHORT((X >> 10));
            short sy = SATURATE_CAST_SHORT((Y >> 10));

            if (type != -233 && (sx < -1 || sx >= srcw || sy < -1 || sy >= srch))
            {
                dst0[0] = border_color[0];
                dst0[1] = border_color[1];
                dst0[2] = border_color[2];
                dst0[3] = border_color[3];
            }
            else if (type == -233 && ((unsigned short)sx >= srcw - 1 || (unsigned short)sy >= srch - 1))
            {
                // skip
            }
            else
            {
                short fx = X & ((1 << 10) - 1);
                short fy = Y & ((1 << 10) - 1);

                short alpha0 = (1 << 10) - fx;
                short alpha1 = fx;

                short beta0 = (1 << 10) - fy;
                short beta1 = fy;

                short sx1 = sx + 1;
                short sy1 = sy + 1;

                const unsigned char* a0 = src0 + srcstride * sy + sx * 4;
                const unsigned char* a1 = src0 + srcstride * sy + sx * 4 + 4;
                const unsigned char* b0 = src0 + srcstride * (sy + 1) + sx * 4;
                const unsigned char* b1 = src0 + srcstride * (sy + 1) + sx * 4 + 4;

                if ((unsigned short)sx >= srcw || (unsigned short)sy >= srch)
                {
                    a0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy >= srch)
                {
                    a1 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx >= srcw || (unsigned short)sy1 >= srch)
                {
                    b0 = type != -233 ? border_color : dst0;
                }
                if ((unsigned short)sx1 >= srcw || (unsigned short)sy1 >= srch)
                {
                    b1 = type != -233 ? border_color : dst0;
                }

                dst0[0] = (unsigned char)(((((unsigned short)((a0[0] * alpha0 + a1[0] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[0] * alpha0 + b1[0] * alpha1) >> 5) * beta1))) >> 15);
                dst0[1] = (unsigned char)(((((unsigned short)((a0[1] * alpha0 + a1[1] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[1] * alpha0 + b1[1] * alpha1) >> 5) * beta1))) >> 15);
                dst0[2] = (unsigned char)(((((unsigned short)((a0[2] * alpha0 + a1[2] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[2] * alpha0 + b1[2] * alpha1) >> 5) * beta1))) >> 15);
                dst0[3] = (unsigned char)(((((unsigned short)((a0[3] * alpha0 + a1[3] * alpha1) >> 5) * beta0)) + (((unsigned short)((b0[3] * alpha0 + b1[3] * alpha1) >> 5) * beta1))) >> 15);
            }

            dst0 += 4;
        }

        dst0 += wgap;
    }

#undef SATURATE_CAST_SHORT
#undef SATURATE_CAST_INT
}


ImMat MatRotate(const ImMat& mat, float angle)
{
    ImMat dst;
    if (mat.empty())
        return dst;
    float scale = 1.0;
    float dx = mat.w / 2.0;
    float dy = mat.h / 2.0;
    // find rotation matrix
    float tm[6];
    angle *= (float)(M_PI / 180.f);
    float alpha = cosf(angle) * scale;
    float beta = sinf(angle) * scale;
    tm[0] = alpha;
    tm[1] = beta;
    tm[2] = (1.f - alpha) * dx - beta * dy;
    tm[3] = -beta;
    tm[4] = alpha;
    tm[5] = beta * dx + (1.f - alpha) * dy;

    // find corner coordinate
    float x0 = alpha * -dx - beta * -dy + dx;
    float y0 = beta * -dx + alpha * -dy + dy;
    float x1 = alpha * dx - beta * -dy + dx;
    float y1 = beta * dx + alpha * -dy + dy;
    float x2 = alpha * -dx - beta * dy + dx;
    float y2 = beta * -dx + alpha * dy + dy;
    float x3 = alpha * dx - beta * dy + dx;
    float y3 = beta * dx + alpha * dy + dy;
    
    // find minimum bounding rectangle area
    float width = fmax(x3, fmax(x2, fmax(x1, x0))) - fmin(x3, fmin(x2, fmin(x1, x0)));
    float height = fmax(y3, fmax(y2, fmax(y1, y0))) - fmin(y3, fmin(y2, fmin(y1, y0)));
    //dst.create(width, height, mat.c, 1u, mat.c);
    dst.create(mat.w, mat.h, mat.c, 1u, mat.elempack);

    switch (mat.c)
    {
        case 1 : warpaffine_bilinear_c1((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 1, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 1, (const float *)tm); break;
        case 2 : warpaffine_bilinear_c2((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 2, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 2, (const float *)tm); break;
        case 3 : warpaffine_bilinear_c3((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 3, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 3, (const float *)tm); break;
        case 4 : warpaffine_bilinear_c4((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 4, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 4, (const float *)tm); break;
        default: break;
    }

    return dst;
}

ImMat MatWarpAffine(const ImMat& mat, const ImMat& M, ImSize dsize)
{
    ImMat dst;
    if (mat.empty())
        return dst;
    ImMat inv_M = M.clone();
    invert_affine_transform((float*)M.data, (float*)inv_M.data);
    dst.create(dsize.w, dsize.h, mat.c, 1u, mat.elempack);
    switch (mat.c)
    {
        case 1 : warpaffine_bilinear_c1((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 1, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 1, (const float *)inv_M); break;
        case 2 : warpaffine_bilinear_c2((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 2, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 2, (const float *)inv_M); break;
        case 3 : warpaffine_bilinear_c3((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 3, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 3, (const float *)inv_M); break;
        case 4 : warpaffine_bilinear_c4((const unsigned char*)mat.data, mat.w, mat.h, mat.w * 4, (unsigned char *)dst.data, dst.w, dst.h, dst.w * 4, (const float *)inv_M); break;
        default: break;
    }
    return dst;
}

static inline void interpolateCubic(float x, float* coeffs)
{
    const float A = -0.75f;
    coeffs[0] = ((A * (x + 1.f) - 5.0f * A) * (x + 1.f) + 8.0f * A) * (x + 1.f) - 4.0f * A;
    coeffs[1] = ((A + 2.f) * x - (A + 3.f)) * x * x + 1.f;
    coeffs[2] = ((A + 2.f) * (1.f - x) - (A + 3.f)) * (1.f - x) * (1.f - x) + 1.f;
    coeffs[3] = 1.f - coeffs[0] - coeffs[1] - coeffs[2];
}

ImMat MatWarpPerspective(const ImMat& src, const ImMat& M, ImSize dsize, ImInterpolateMode mode)
{
#define INTER_BITS 5
#define INTER_TAB_SIZE (1 << INTER_BITS)
#define INTER_SCALE (1.f / INTER_TAB_SIZE)
    ImMat dst;
    if (src.empty() || M.empty())
        return dst;
    ImPixel pixel_fill(0, 0, 0, 0);
    dst.create(dsize.w, dsize.h, src.c, 1u, src.elempack);
    float * transform_matrix = (float *)M.data;
    for (int y = 0; y < dsize.h; y++)
    {
        for (int x = 0; x < dsize.w; x++)
        {
            
            float X0 = transform_matrix[0] * (float)x + transform_matrix[1] * (float)y + transform_matrix[2];
            float Y0 = transform_matrix[3] * (float)x + transform_matrix[4] * (float)y + transform_matrix[5];
            float W  = transform_matrix[6] * (float)x + transform_matrix[7] * (float)y + transform_matrix[8];
            if (mode == IM_INTERPOLATE_NEAREST)
            {
                W = W != 0.0f ? 1.f / W : 0.0f;
                int sx = (int)(X0 * W);
                int sy = (int)(Y0 * W);
                if (sx >= 0 && sx < src.w && sy >= 0 && sy < src.h)
                {
                    dst.set_pixel(x, y, src.get_pixel(sx, sy));
                }
                else
                {
                    dst.set_pixel(x, y, pixel_fill);
                }
            }
            else if (mode == IM_INTERPOLATE_BILINEAR)
            {
                W = W != 0.0f ? INTER_TAB_SIZE / W : 0.0f;
                int X = (int)(X0 * W), Y = (int)(Y0 * W);
                int sx = (int)(X >> INTER_BITS);
                int sy = (int)(Y >> INTER_BITS);
                int ay = (int)(Y & (INTER_TAB_SIZE - 1));
                int ax = (int)(X & (INTER_TAB_SIZE - 1));
                ImPixel p0 = (sx >= 0 && sx < src.w && sy >= 0 && sy < src.h) ? src.get_pixel(sx, sy) : pixel_fill;
                ImPixel p1 = (sx + 1 >= 0 && sx + 1 < src.w && sy >= 0 && sy < src.h) ? src.get_pixel(sx + 1, sy) : pixel_fill;
                ImPixel p2 = (sx >= 0 && sx < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx, sy + 1) : pixel_fill;
                ImPixel p3 = (sx + 1 >= 0 && sx + 1 < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx + 1, sy + 1) : pixel_fill;
                float taby = 1.f / INTER_TAB_SIZE * ay;
                float tabx = 1.f / INTER_TAB_SIZE * ax;
                float tabx2 = 1.0f - tabx, taby2 = 1.0f - taby;
                ImPixel v = p0 * tabx2 * taby2 +  p1 * tabx * taby2 + p2 * tabx2 * taby + p3 * tabx * taby;
                dst.set_pixel(x, y, v);
            }
            else if (mode == IM_INTERPOLATE_BICUBIC)
            {
                W = W != 0.0f ? INTER_TAB_SIZE / W : 0.0f;
                int X = (int)(X0 * W), Y = (int)(Y0 * W);
                int sx = (int)(X >> INTER_BITS) - 1;
                int sy = (int)(Y >> INTER_BITS) - 1;
                int ay = (int)(Y & (INTER_TAB_SIZE - 1));
                int ax = (int)(X & (INTER_TAB_SIZE - 1));
                ImPixel v[16];
                v[ 0] = (sx + 0 >= 0 && sx + 0 < src.w && sy + 0 >= 0 && sy + 0 < src.h) ? src.get_pixel(sx + 0, sy + 0) : pixel_fill;
                v[ 1] = (sx + 1 >= 0 && sx + 1 < src.w && sy + 0 >= 0 && sy + 0 < src.h) ? src.get_pixel(sx + 1, sy + 0) : pixel_fill;
                v[ 2] = (sx + 2 >= 0 && sx + 2 < src.w && sy + 0 >= 0 && sy + 0 < src.h) ? src.get_pixel(sx + 2, sy + 0) : pixel_fill;
                v[ 3] = (sx + 3 >= 0 && sx + 3 < src.w && sy + 0 >= 0 && sy + 0 < src.h) ? src.get_pixel(sx + 3, sy + 0) : pixel_fill;
                v[ 4] = (sx + 0 >= 0 && sx + 0 < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx + 0, sy + 1) : pixel_fill;
                v[ 5] = (sx + 1 >= 0 && sx + 1 < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx + 1, sy + 1) : pixel_fill;
                v[ 6] = (sx + 2 >= 0 && sx + 2 < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx + 2, sy + 1) : pixel_fill;
                v[ 7] = (sx + 3 >= 0 && sx + 3 < src.w && sy + 1 >= 0 && sy + 1 < src.h) ? src.get_pixel(sx + 3, sy + 1) : pixel_fill;
                v[ 8] = (sx + 0 >= 0 && sx + 0 < src.w && sy + 2 >= 0 && sy + 2 < src.h) ? src.get_pixel(sx + 0, sy + 2) : pixel_fill;
                v[ 9] = (sx + 1 >= 0 && sx + 1 < src.w && sy + 2 >= 0 && sy + 2 < src.h) ? src.get_pixel(sx + 1, sy + 2) : pixel_fill;
                v[10] = (sx + 2 >= 0 && sx + 2 < src.w && sy + 2 >= 0 && sy + 2 < src.h) ? src.get_pixel(sx + 2, sy + 2) : pixel_fill;
                v[11] = (sx + 3 >= 0 && sx + 3 < src.w && sy + 2 >= 0 && sy + 2 < src.h) ? src.get_pixel(sx + 3, sy + 2) : pixel_fill;
                v[12] = (sx + 0 >= 0 && sx + 0 < src.w && sy + 3 >= 0 && sy + 3 < src.h) ? src.get_pixel(sx + 0, sy + 3) : pixel_fill;
                v[13] = (sx + 1 >= 0 && sx + 1 < src.w && sy + 3 >= 0 && sy + 3 < src.h) ? src.get_pixel(sx + 1, sy + 3) : pixel_fill;
                v[14] = (sx + 2 >= 0 && sx + 2 < src.w && sy + 3 >= 0 && sy + 3 < src.h) ? src.get_pixel(sx + 2, sy + 3) : pixel_fill;
                v[15] = (sx + 3 >= 0 && sx + 3 < src.w && sy + 3 >= 0 && sy + 3 < src.h) ? src.get_pixel(sx + 3, sy + 3) : pixel_fill;
                float tab1y[4], tab1x[4];
                float ayy = (float)(INTER_SCALE * ay);
                float axx = (float)(INTER_SCALE * ax);
                interpolateCubic(ayy, tab1y);
                interpolateCubic(axx, tab1x);
                ImPixel p(0, 0, 0, 0);
                for (int i = 0; i < 16; i++)
                    p = p + v[i] * tab1y[(i >> 2)] * tab1x[(i & 3)];
                dst.set_pixel(x, y, p);
            }
        }
    }
    return dst;
}

} // namespace ImGui

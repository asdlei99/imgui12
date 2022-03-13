#include "imgui_extra_widget.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <stdio.h>

static const ImS32          IM_S32_MIN = INT_MIN;    // (-2147483647 - 1), (0x80000000);
static const ImS32          IM_S32_MAX = INT_MAX;    // (2147483647), (0x7FFFFFFF)
static const ImU32          IM_U32_MIN = 0;
static const ImU32          IM_U32_MAX = UINT_MAX;   // (0xFFFFFFFF)
#ifdef LLONG_MIN
static const ImS64          IM_S64_MIN = LLONG_MIN;  // (-9223372036854775807ll - 1ll);
static const ImS64          IM_S64_MAX = LLONG_MAX;  // (9223372036854775807ll);
#else
static const ImS64          IM_S64_MIN = -9223372036854775807LL - 1;
static const ImS64          IM_S64_MAX = 9223372036854775807LL;
#endif
static const ImU64          IM_U64_MIN = 0;
#ifdef ULLONG_MAX
static const ImU64          IM_U64_MAX = ULLONG_MAX; // (0xFFFFFFFFFFFFFFFFull);
#else
static const ImU64          IM_U64_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

template<typename TYPE>
static const char* ImAtoi(const char* src, TYPE* output)
{
    int negative = 0;
    if (*src == '-') { negative = 1; src++; }
    if (*src == '+') { src++; }
    TYPE v = 0;
    while (*src >= '0' && *src <= '9')
        v = (v * 10) + (*src++ - '0');
    *output = negative ? -v : v;
    return src;
}

template<typename TYPE, typename SIGNEDTYPE>
TYPE ImGui::RoundScalarWithFormatKnobT(const char* format, ImGuiDataType data_type, TYPE v)
{
    const char* fmt_start = ImParseFormatFindStart(format);
    if (fmt_start[0] != '%' || fmt_start[1] == '%') // Don't apply if the value is not visible in the format string
        return v;
    char v_str[64];
    ImFormatString(v_str, IM_ARRAYSIZE(v_str), fmt_start, v);
    const char* p = v_str;
    while (*p == ' ')
        p++;
    if (data_type == ImGuiDataType_Float || data_type == ImGuiDataType_Double)
        v = (TYPE)ImAtof(p);
    else
        ImAtoi(p, (SIGNEDTYPE*)&v);
    return v;
}

template<typename TYPE, typename FLOATTYPE>
float ImGui::SliderCalcRatioFromValueT(ImGuiDataType data_type, TYPE v, TYPE v_min, TYPE v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max)
        return 0.0f;

    const bool is_power = (power != 1.0f) && (data_type == ImGuiDataType_Float || data_type == ImGuiDataType_Double);
    const TYPE v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
    if (is_power)
    {
        if (v_clamped < 0.0f)
        {
            const float f = 1.0f - (float)((v_clamped - v_min) / (ImMin((TYPE)0, v_max) - v_min));
            return (1.0f - ImPow(f, 1.0f/power)) * linear_zero_pos;
        }
        else
        {
            const float f = (float)((v_clamped - ImMax((TYPE)0, v_min)) / (v_max - ImMax((TYPE)0, v_min)));
            return linear_zero_pos + ImPow(f, 1.0f/power) * (1.0f - linear_zero_pos);
        }
    }

    // Linear slider
    return (float)((FLOATTYPE)(v_clamped - v_min) / (FLOATTYPE)(v_max - v_min));
}

// FIXME: Move some of the code into SliderBehavior(). Current responsability is larger than what the equivalent DragBehaviorT<> does, we also do some rendering, etc.
template<typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
bool ImGui::SliderBehaviorKnobT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, TYPE* v, const TYPE v_min, const TYPE v_max, const char* format, float power, ImGuiSliderFlags flags, ImRect* out_grab_bb)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiAxis axis = (flags & ImGuiSliderFlags_Vertical) ? ImGuiAxis_Y : ImGuiAxis_X;
    const bool is_decimal = (data_type == ImGuiDataType_Float) || (data_type == ImGuiDataType_Double);
    const bool is_power = (power != 1.0f) && is_decimal;

    const float grab_padding = 2.0f;
    const float slider_sz = (bb.Max[axis] - bb.Min[axis]) - grab_padding * 2.0f;
    float grab_sz = style.GrabMinSize;
    SIGNEDTYPE v_range = (v_min < v_max ? v_max - v_min : v_min - v_max);
    if (!is_decimal && v_range >= 0)                                             // v_range < 0 may happen on integer overflows
        grab_sz = ImMax((float)(slider_sz / (v_range + 1)), style.GrabMinSize);  // For integer sliders: if possible have the grab size represent 1 unit
    grab_sz = ImMin(grab_sz, slider_sz);
    const float slider_usable_sz = slider_sz - grab_sz;
    const float slider_usable_pos_min = bb.Min[axis] + grab_padding + grab_sz * 0.5f;
    const float slider_usable_pos_max = bb.Max[axis] - grab_padding - grab_sz * 0.5f;

    // For power curve sliders that cross over sign boundary we want the curve to be symmetric around 0.0f
    float linear_zero_pos;   // 0.0->1.0f
    if (is_power && v_min * v_max < 0.0f)
    {
        // Different sign
        const FLOATTYPE linear_dist_min_to_0 = ImPow(v_min >= 0 ? (FLOATTYPE)v_min : -(FLOATTYPE)v_min, (FLOATTYPE)1.0f / power);
        const FLOATTYPE linear_dist_max_to_0 = ImPow(v_max >= 0 ? (FLOATTYPE)v_max : -(FLOATTYPE)v_max, (FLOATTYPE)1.0f / power);
        linear_zero_pos = (float)(linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0));
    }
    else
    {
        // Same sign
        linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
    }

    // Process interacting with the slider
    bool value_changed = false;
    if (g.ActiveId == id)
    {
        bool set_new_value = false;
        float clicked_t = 0.0f;
        if (g.ActiveIdSource == ImGuiInputSource_Mouse)
        {
            if (!g.IO.MouseDown[0])
            {
                ClearActiveID();
            }
            else
            {
                const float mouse_abs_pos = g.IO.MousePos[axis];
                clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
                if (axis == ImGuiAxis_Y)
                    clicked_t = 1.0f - clicked_t;
                set_new_value = true;
            }
        }
        else if (g.ActiveIdSource == ImGuiInputSource_Nav)
        {
            const ImVec2 delta2 = GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard | ImGuiNavDirSourceFlags_PadDPad, ImGuiInputReadMode_RepeatFast, 0.0f, 0.0f);
            float delta = (axis == ImGuiAxis_X) ? delta2.x : -delta2.y;
            if (g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
            {
                ClearActiveID();
            }
            else if (delta != 0.0f)
            {
                clicked_t = SliderCalcRatioFromValueT<TYPE,FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
                const int decimal_precision = is_decimal ? ImParseFormatPrecision(format, 3) : 0;
                if ((decimal_precision > 0) || is_power)
                {
                    delta /= 100.0f;    // Gamepad/keyboard tweak speeds in % of slider bounds
                    if (IsNavInputDown(ImGuiNavInput_TweakSlow))
                        delta /= 10.0f;
                }
                else
                {
                    if ((v_range >= -100.0f && v_range <= 100.0f) || IsNavInputDown(ImGuiNavInput_TweakSlow))
                        delta = ((delta < 0.0f) ? -1.0f : +1.0f) / (float)v_range; // Gamepad/keyboard tweak speeds in integer steps
                    else
                        delta /= 100.0f;
                }
                if (IsNavInputDown(ImGuiNavInput_TweakFast))
                    delta *= 10.0f;
                set_new_value = true;
                if ((clicked_t >= 1.0f && delta > 0.0f) || (clicked_t <= 0.0f && delta < 0.0f)) // This is to avoid applying the saturation when already past the limits
                    set_new_value = false;
                else
                    clicked_t = ImSaturate(clicked_t + delta);
            }
        }

        if (set_new_value)
        {
            TYPE v_new;
            if (is_power)
            {
                // Account for power curve scale on both sides of the zero
                if (clicked_t < linear_zero_pos)
                {
                    // Negative: rescale to the negative range before powering
                    float a = 1.0f - (clicked_t / linear_zero_pos);
                    a = ImPow(a, power);
                    v_new = ImLerp(ImMin(v_max, (TYPE)0), v_min, a);
                }
                else
                {
                    // Positive: rescale to the positive range before powering
                    float a;
                    if (ImFabs(linear_zero_pos - 1.0f) > 1.e-6f)
                        a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
                    else
                        a = clicked_t;
                    a = ImPow(a, power);
                    v_new = ImLerp(ImMax(v_min, (TYPE)0), v_max, a);
                }
            }
            else
            {
                // Linear slider
                if (is_decimal)
                {
                    v_new = ImLerp(v_min, v_max, clicked_t);
                }
                else
                {
                    // For integer values we want the clicking position to match the grab box so we round above
                    // This code is carefully tuned to work with large values (e.g. high ranges of U64) while preserving this property..
                    FLOATTYPE v_new_off_f = (v_max - v_min) * clicked_t;
                    TYPE v_new_off_floor = (TYPE)(v_new_off_f);
                    TYPE v_new_off_round = (TYPE)(v_new_off_f + (FLOATTYPE)0.5);
                    if (v_new_off_floor < v_new_off_round)
                        v_new = v_min + v_new_off_round;
                    else
                        v_new = v_min + v_new_off_floor;
                }
            }

            // Round to user desired precision based on format string
            v_new = RoundScalarWithFormatKnobT<TYPE,SIGNEDTYPE>(format, data_type, v_new);

            // Apply result
            if (*v != v_new)
            {
                *v = v_new;
                value_changed = true;
            }
        }
    }

    if (slider_sz < 1.0f)
    {
        *out_grab_bb = ImRect(bb.Min, bb.Min);
    }
    else
    {
        // Output grab position so it can be displayed by the caller
        float grab_t = SliderCalcRatioFromValueT<TYPE, FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
        if (axis == ImGuiAxis_Y)
            grab_t = 1.0f - grab_t;
        const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
        if (axis == ImGuiAxis_X)
            *out_grab_bb = ImRect(grab_pos - grab_sz * 0.5f, bb.Min.y + grab_padding, grab_pos + grab_sz * 0.5f, bb.Max.y - grab_padding);
        else
            *out_grab_bb = ImRect(bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f, bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f);
    }

    return value_changed;
}

// For 32-bits and larger types, slider bounds are limited to half the natural type range.
// So e.g. an integer Slider between INT_MAX-10 and INT_MAX will fail, but an integer Slider between INT_MAX/2-10 and INT_MAX/2 will be ok.
// It would be possible to lift that limitation with some work but it doesn't seem to be worth it for sliders.
bool ImGui::SliderBehavior(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format, float power, ImGuiSliderFlags flags, ImRect* out_grab_bb)
{
    switch (data_type)
    {
    case ImGuiDataType_S8:  { ImS32 v32 = (ImS32)*(ImS8*)v;  bool r = SliderBehaviorKnobT<ImS32, ImS32, float >(bb, id, ImGuiDataType_S32, &v32, *(const ImS8*)v_min,  *(const ImS8*)v_max,  format, power, flags, out_grab_bb); if (r) *(ImS8*)v  = (ImS8)v32;  return r; }
    case ImGuiDataType_U8:  { ImU32 v32 = (ImU32)*(ImU8*)v;  bool r = SliderBehaviorKnobT<ImU32, ImS32, float >(bb, id, ImGuiDataType_U32, &v32, *(const ImU8*)v_min,  *(const ImU8*)v_max,  format, power, flags, out_grab_bb); if (r) *(ImU8*)v  = (ImU8)v32;  return r; }
    case ImGuiDataType_S16: { ImS32 v32 = (ImS32)*(ImS16*)v; bool r = SliderBehaviorKnobT<ImS32, ImS32, float >(bb, id, ImGuiDataType_S32, &v32, *(const ImS16*)v_min, *(const ImS16*)v_max, format, power, flags, out_grab_bb); if (r) *(ImS16*)v = (ImS16)v32; return r; }
    case ImGuiDataType_U16: { ImU32 v32 = (ImU32)*(ImU16*)v; bool r = SliderBehaviorKnobT<ImU32, ImS32, float >(bb, id, ImGuiDataType_U32, &v32, *(const ImU16*)v_min, *(const ImU16*)v_max, format, power, flags, out_grab_bb); if (r) *(ImU16*)v = (ImU16)v32; return r; }
    case ImGuiDataType_S32:
        IM_ASSERT(*(const ImS32*)v_min >= IM_S32_MIN/2 && *(const ImS32*)v_max <= IM_S32_MAX/2);
        return SliderBehaviorKnobT<ImS32, ImS32, float >(bb, id, data_type, (ImS32*)v,  *(const ImS32*)v_min,  *(const ImS32*)v_max,  format, power, flags, out_grab_bb);
    case ImGuiDataType_U32:
        IM_ASSERT(*(const ImU32*)v_max <= IM_U32_MAX/2);
        return SliderBehaviorKnobT<ImU32, ImS32, float >(bb, id, data_type, (ImU32*)v,  *(const ImU32*)v_min,  *(const ImU32*)v_max,  format, power, flags, out_grab_bb);
    case ImGuiDataType_S64:
        IM_ASSERT(*(const ImS64*)v_min >= IM_S64_MIN/2 && *(const ImS64*)v_max <= IM_S64_MAX/2);
        return SliderBehaviorKnobT<ImS64, ImS64, double>(bb, id, data_type, (ImS64*)v,  *(const ImS64*)v_min,  *(const ImS64*)v_max,  format, power, flags, out_grab_bb);
    case ImGuiDataType_U64:
        IM_ASSERT(*(const ImU64*)v_max <= IM_U64_MAX/2);
        return SliderBehaviorKnobT<ImU64, ImS64, double>(bb, id, data_type, (ImU64*)v,  *(const ImU64*)v_min,  *(const ImU64*)v_max,  format, power, flags, out_grab_bb);
    case ImGuiDataType_Float:
        IM_ASSERT(*(const float*)v_min >= -FLT_MAX/2.0f && *(const float*)v_max <= FLT_MAX/2.0f);
        return SliderBehaviorKnobT<float, float, float >(bb, id, data_type, (float*)v,  *(const float*)v_min,  *(const float*)v_max,  format, power, flags, out_grab_bb);
    case ImGuiDataType_Double:
        IM_ASSERT(*(const double*)v_min >= -DBL_MAX/2.0f && *(const double*)v_max <= DBL_MAX/2.0f);
        return SliderBehaviorKnobT<double,double,double>(bb, id, data_type, (double*)v, *(const double*)v_min, *(const double*)v_max, format, power, flags, out_grab_bb);
    case ImGuiDataType_COUNT: break;
    }
    IM_ASSERT(0);
    return false;
}

void ImGui::UvMeter(char const *label, ImVec2 const &size, int *value, int v_min, int v_max, int steps, int* stack, int* count)
{
    float fvalue = (float)*value;
    float *fstack = nullptr;
    float _f = 0.f;
    if (stack) { fstack = &_f; *fstack = (float)*stack; }
    UvMeter(label, size, &fvalue, (float)v_min, (float)v_max, steps, fstack, count);
    *value = (int)fvalue;
    if (stack) *stack = (int)*fstack;
}

void ImGui::UvMeter(char const *label, ImVec2 const &size, float *value, float v_min, float v_max, int steps, float* stack, int* count)
{
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::InvisibleButton(label, size);
    float steps_size = (v_max - v_min) / (float)steps;
    if (stack && count)
    {
        if (*value > *stack) 
        {
            *stack = *value;
            *count = 0;
        }
        else
        {
            *(count) += 1;
            if (*count > 10)
            {
                *stack -= steps_size / 2;
                if (*stack < v_min) *stack = v_min;
            }
        }
    }

    if (size.y > size.x)
    {
        float stepHeight = size.y / (v_max - v_min + 1);
        auto y = pos.y + size.y;
        auto hue = 0.4f;
        auto sat = 0.6f;
        auto lum = 0.6f;
        for (float i = v_min; i <= v_max; i += steps_size)
        {
            hue = 0.4f - (i / (v_max - v_min)) / 2.0f;
            sat = (*value < i ? 0.0f : 0.6f);
            lum = (*value < i ? 0.0f : 0.6f);
            draw_list->AddRectFilled(ImVec2(pos.x, y), ImVec2(pos.x + size.x, y - (stepHeight * steps_size - 1)), static_cast<ImU32>(ImColor::HSV(hue, sat, lum)));
            y = pos.y + size.y - (i * stepHeight);
        }
        if (stack && count)
        {
            draw_list->AddLine(ImVec2(pos.x, pos.y + size.y - (*stack * stepHeight)), ImVec2(pos.x + size.x, pos.y + size.y - (*stack * stepHeight)), IM_COL32_WHITE, 2.f);
        }
    }
    else
    {
        float stepWidth = size.x / (v_max - v_min + 1);
        auto x = pos.x;
        auto hue = 0.4f;
        auto sat = 0.6f;
        auto lum = 0.6f;
        for (float i = v_min; i <= v_max; i += steps_size)
        {
            hue = 0.4f - (i / (v_max - v_min)) / 2.0f;
            sat = (*value < i ? 0.0f : 0.6f);
            lum = (*value < i ? 0.0f : 0.6f);
            draw_list->AddRectFilled(ImVec2(x, pos.y), ImVec2(x + (stepWidth * steps_size - 1), pos.y + size.y), static_cast<ImU32>(ImColor::HSV(hue, sat, lum)));
            x = pos.x + (i * stepWidth);
        }
        if (stack && count)
        {
            draw_list->AddLine(ImVec2(pos.x + (*stack * stepWidth), pos.y), ImVec2(pos.x + (*stack * stepWidth), pos.y + size.y), IM_COL32_WHITE, 2.f);
        }
    }
}

bool ImGui::Fader(const char *label, const ImVec2 &size, int *v, const int v_min, const int v_max, const char *format, float power)
{
    ImGuiDataType data_type = ImGuiDataType_S32;
    ImGuiWindow *window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, nullptr, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    IM_ASSERT(data_type >= 0 && data_type < ImGuiDataType_COUNT);
    if (format == nullptr)
        format = "%d";

    const bool hovered = ItemHoverable(frame_bb, id);
    if ((hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || g.NavActivateInputId == id)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        //        g.ActiveIdAllowNavDirFlags = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : g.HoveredId == id ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, v, &v_min, &v_max, format, power, ImGuiSliderFlags_Vertical, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    ImRect gutter;
    gutter.Min = grab_bb.Min;
    gutter.Max = ImVec2(frame_bb.Max.x - 2.0f, frame_bb.Max.y - 2.0f);
    auto w = ((gutter.Max.x - gutter.Min.x) - 4.0f) / 2.0f;
    gutter.Min.x += w;
    gutter.Max.x -= w;
    window->DrawList->AddRectFilled(gutter.Min, gutter.Max, GetColorU32(ImGuiCol_ButtonActive), style.GrabRounding);

    // Render grab
    window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(ImGuiCol_Text), style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char value_buf[64];
    sprintf(value_buf, format, int(*v * power));
    const char *value_buf_end = value_buf + strlen(value_buf);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, nullptr, ImVec2(0.5f, 0.0f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

///////////////////////////////////////////////////////////////////////////////
// New Knob controllor 
static void draw_circle(ImVec2 center, float _size, bool filled, int segments, float radius, ImGui::ColorSet& color)
{
    float circle_radius = _size * radius;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 _color = ImGui::GetColorU32(ImGui::IsItemActive() ? color.active : ImGui::IsItemHovered(0) ? color.hovered : color.base);
    if (filled)
        draw_list->AddCircleFilled(center, circle_radius, _color, segments);
    else
        draw_list->AddCircle(center, circle_radius, _color, segments);
}

static void bezier_arc(ImVec2 center, ImVec2 start, ImVec2 end, ImVec2& c1, ImVec2 & c2)
{
    float ax = start[0] - center[0];
    float ay = start[1] - center[1];
    float bx = end[0] - center[0];
    float by = end[1] - center[1];
    float q1 = ax * ax + ay * ay;
    float q2 = q1 + ax * bx + ay * by;
    float k2 = (4.0 / 3.0) * (sqrt(2.0 * q1 * q2) - q2) / (ax * by - ay * bx);
    c1 = ImVec2(center[0] + ax - k2 * ay, center[1] + ay + k2 * ax);
    c2 = ImVec2(center[0] + bx + k2 * by, center[1] + by - k2 * bx);
}

static void draw_arc1(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImU32 color, int num_segments)
{
    ImVec2 start = {center[0] + cos(start_angle) * radius, center[1] + sin(start_angle) * radius};
    ImVec2 end = {center[0] + cos(end_angle) * radius, center[1] + sin(end_angle) * radius};
    ImVec2 c1, c2;
    bezier_arc(center, start, end, c1, c2);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddBezierCubic(start, c1, c2, end, color, thickness, num_segments);
}

static void draw_arc(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImU32 color, int num_segments, int8_t bezier_count)
{
    float overlap = thickness * radius * 0.00001 * IM_PI;
    float delta = end_angle - start_angle;
    float bez_step = 1.0 / (float)bezier_count;
    float mid_angle = start_angle + overlap;
    for (int i = 0; i < bezier_count - 1; i++)
    {
        float mid_angle2 = delta * bez_step + mid_angle;
        draw_arc1(center, radius, mid_angle - overlap, mid_angle2 + overlap, thickness, color, num_segments);
        mid_angle = mid_angle2;
    }
    draw_arc1(center, radius, mid_angle - overlap, end_angle, thickness, color, num_segments);
}

static void draw_arc(ImVec2 center, float _radius, float _size, float radius, float start_angle, float end_angle, int segments, int8_t bezier_count, ImGui::ColorSet& color)
{
    float track_radius = _radius * radius;
    float track_size = _size * radius * 0.5 + 0.0001;
    ImU32 _color = ImGui::GetColorU32(ImGui::IsItemActive() ? color.active : ImGui::IsItemHovered(0) ? color.hovered : color.base);
    draw_arc(center, track_radius, start_angle, end_angle, track_size, _color, segments, bezier_count);
}

static void draw_dot(ImVec2 center, float _radius, float _size, float radius, float _angle, bool filled, int segments, ImGui::ColorSet& color)
{
    float dot_size = _size * radius;
    float dot_radius = _radius * radius;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 _color = ImGui::GetColorU32(ImGui::IsItemActive() ? color.active : ImGui::IsItemHovered(0) ? color.hovered : color.base);
    if (filled)
        draw_list->AddCircleFilled(
                ImVec2(center[0] + cos(_angle) * dot_radius, center[1] + sin(_angle) * dot_radius),
                dot_size, _color, segments);
    else
        draw_list->AddCircle(
                ImVec2(center[0] + cos(_angle) * dot_radius, center[1] + sin(_angle) * dot_radius),
                dot_size, _color, segments);
}

static void draw_tick(ImVec2 center, float radius, float start, float end, float width, float _angle, ImGui::ColorSet& color)
{
    float tick_start = start * radius;
    float tick_end = end * radius;
    float _angle_cos = cos(_angle);
    float _angle_sin = sin(_angle);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 _color = ImGui::GetColorU32(ImGui::IsItemActive() ? color.active : ImGui::IsItemHovered(0) ? color.hovered : color.base);
    draw_list->AddLine(
            ImVec2(center[0] + _angle_cos * tick_end, center[1] + _angle_sin * tick_end),
            ImVec2(center[0] + _angle_cos * tick_start, center[1] + _angle_sin * tick_start),
            _color,
            width * radius);
}

bool ImGui::Knob(char const *label, float *p_value, float v_min, float v_max, float v_step, float v_default, float size,
                ColorSet circle_color, ColorSet wiper_color, ColorSet track_color, ColorSet tick_color,
                ImGuiKnobType type, char const *format, int tick_steps)
{
    ImGuiStyle &style = ImGui::GetStyle();
    float line_height = ImGui::GetTextLineHeight();
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 ItemSize = ImVec2(size, size + line_height * 2 + style.ItemInnerSpacing.y * 2 + 4);
    std::string ViewID = "###" + std::string(label) + "_KNOB_VIEW_CONTORL_";
    ImGui::BeginChild(ViewID.c_str(), ItemSize, false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    bool showLabel = label[0] != '#' && label[1] != '#' && label[0] != '\0';
    float radius = std::fmin(ItemSize.x, ItemSize.y) / 2.0f;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
    auto textSize = CalcTextSize(label);
    bool is_no_limit = isnan(v_min) || isnan(v_max);
    if (showLabel)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddText(ImVec2(pos.x + ((ItemSize.x / 2) - (textSize.x / 2)), pos.y + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);
        center.y += line_height + 4;
    }

    ImGui::InvisibleButton(label, ImVec2(radius * 2, radius * 2 + line_height));

    float step = isnan(v_step) ? (v_max - v_min) / 200.f : v_step;
    bool value_changed = false;
    bool is_active = ImGui::IsItemActive();
    bool is_hovered = ImGui::IsItemActive();
    if (is_active && io.MouseDelta.y != 0.0f)
    {
        *p_value -= io.MouseDelta.y * step;
        if (!is_no_limit)
        {
            if (*p_value < v_min)
                *p_value = v_min;
            if (*p_value > v_max)
                *p_value = v_max;
        }
        value_changed = true;
    }

    if (is_active && ImGui::IsMouseDoubleClicked(0))
    {
        if (*p_value != v_default)
        {
            *p_value = v_default;
            value_changed = true;
        }
    }

    float angle_min = IM_PI * 0.75;
    float angle_max = IM_PI * 2.25;
    float t = (*p_value - v_min) / (v_max - v_min);
    if (is_no_limit)
    {
        angle_min = -IM_PI * 0.5;
        angle_max = IM_PI * 1.5;
        t = *p_value;
    }
    float angle = angle_min + (angle_max - angle_min) * t;

    switch (type)
    {
        case IMKNOB_TICK:
            draw_circle(center, 0.7, true, 32, radius, circle_color);
            draw_tick(center, radius, 0.4, 0.7, 0.1, angle, wiper_color);
        break;
        case IMKNOB_TICK_DOT:
            draw_circle(center, 0.85, true, 32, radius, circle_color);
            draw_dot(center, 0.6, 0.12, radius, angle, true, 12, wiper_color);
        break;
        case IMKNOB_TICK_WIPER:
            draw_circle(center, 0.6, true, 32, radius, circle_color);
            if (!is_no_limit)
            {
                draw_arc(center, 0.85, 0.25, radius, angle_min, angle_max, 16, 2, track_color);
                if (t > 0.01)
                    draw_arc(center, 0.85, 0.27, radius, angle_min, angle, 16, 2, wiper_color);
            }
            else
            {
                draw_circle(center, 0.75, false, 32, radius, circle_color);
            }
            draw_tick(center, radius, 0.4, 0.6, 0.1, angle, wiper_color);
        break;
        case IMKNOB_WIPER:
            draw_circle(center, 0.7, true, 32, radius, circle_color);
            if (!is_no_limit)
            {
                draw_arc(center, 0.8, 0.41, radius, angle_min, angle_max, 16, 2, track_color);
                if (t > 0.01)
                    draw_arc(center, 0.8, 0.43, radius, angle_min, angle, 16, 2, wiper_color);
            }
            else
            {
                draw_circle(center, 0.9, false, 32, radius, circle_color);
                draw_tick(center, radius, 0.75, 0.9, 0.1, angle, wiper_color);
            }
        break;
        case IMKNOB_WIPER_TICK:
            draw_circle(center, 0.6, true, 32, radius, circle_color);
            if (!is_no_limit)
            {
                draw_arc(center, 0.85, 0.41, radius, angle_min, angle_max, 16, 2, track_color);
            }
            else
            {
                draw_circle(center, 0.75, false, 32, radius, circle_color);
                draw_circle(center, 0.9, false, 32, radius, circle_color);
            }
            draw_tick(center, radius, 0.75, 0.9, 0.1, angle, wiper_color);
        break;
        case IMKNOB_WIPER_DOT:
            draw_circle(center, 0.6, true, 32, radius, circle_color);
            if (!is_no_limit)
            {
                draw_arc(center, 0.85, 0.41, radius, angle_min, angle_max, 16, 2, track_color);
            }
            else
            {
                draw_circle(center, 0.80, false, 32, radius, circle_color);
                draw_circle(center, 0.95, false, 32, radius, circle_color);
            }
            draw_dot(center, 0.85, 0.1, radius, angle, true, 12, wiper_color);
        break;
        case IMKNOB_WIPER_ONLY:
            if (!is_no_limit)
            {
                draw_arc(center, 0.8, 0.41, radius, angle_min, angle_max, 32, 2, track_color);
                if (t > 0.01)
                    draw_arc(center, 0.8, 0.43, radius, angle_min, angle, 16, 2, wiper_color);
            }
            else
            {
                draw_circle(center, 0.75, false, 32, radius, circle_color);
                draw_circle(center, 0.90, false, 32, radius, circle_color);
                draw_tick(center, radius, 0.75, 0.9, 0.1, angle, wiper_color);
            }
        break;
        case IMKNOB_STEPPED_TICK:
            for (int i = 0; i < tick_steps; i++)
            {
                float a = (float)i / (float)(tick_steps - 1);
                float angle = angle_min + (angle_max - angle_min) * a;
                draw_tick(center, radius, 0.7, 0.9, 0.04, angle, tick_color);
            }
            draw_circle(center, 0.6, true, 32, radius, circle_color);
            draw_tick(center, radius, 0.4, 0.7, 0.1, angle, wiper_color);
        break;
        case IMKNOB_STEPPED_DOT:
            for (int i = 0; i < tick_steps; i++)
            {
                float a = (float)i / (float)(tick_steps - 1);
                float angle = angle_min + (angle_max - angle_min) * a;
                draw_tick(center, radius, 0.7, 0.9, 0.04, angle, tick_color);
            }
            draw_circle(center, 0.6, true, 32, radius, circle_color);
            draw_dot(center, 0.4, 0.12, radius, angle, true, 12, wiper_color);
        break;
        case IMKNOB_SPACE:
            draw_circle(center, 0.3 - t * 0.1, true, 16, radius, circle_color);
            if (t > 0.01 && !is_no_limit)
            {
                draw_arc(center, 0.4, 0.15, radius, angle_min - 1.0, angle - 1.0, 16, 2, wiper_color);
                draw_arc(center, 0.6, 0.15, radius, angle_min + 1.0, angle + 1.0, 16, 2, wiper_color);
                draw_arc(center, 0.8, 0.15, radius, angle_min + 3.0, angle + 3.0, 16, 2, wiper_color);
            }
        break;
        default:
        break;
    }

    ImGui::PushItemWidth(size);
    std::string DragID = "###" + std::string(label) + "_KNOB_DRAG_CONTORL_";
    if (is_no_limit)
    {
        ImGui::DragFloat(DragID.c_str(), p_value, step, FLT_MIN, FLT_MAX, format);
    }
    else
    {
        ImGui::DragFloat(DragID.c_str(), p_value, step, v_min, v_max, format);
    }
    ImGui::PopItemWidth();
    ImGui::EndChild();
    return value_changed;
}

void ImGui::RoundProgressBar(float radius, float *p_value, float v_min, float v_max, ColorSet bar_color, ColorSet progress_color, ColorSet text_color)
{
    char label[20] = {0};
    ImVec4 base_color = ImVec4(0.f, 0.f, 0.f, 1.f);
    ColorSet back_color = {base_color, base_color, base_color};
    float percentage = (*p_value - v_min) / (v_max - v_min) * 100.f;
    sprintf(label, "%02.1f%%", percentage);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 center = ImVec2(pos.x + radius, pos.y + radius);
    ImGui::InvisibleButton("##round_progress_bar", ImVec2(radius * 2, radius * 2));
    float angle_min = IM_PI * 0.75;
    float angle_max = IM_PI * 2.25;
    float t = percentage / 100.0;
    angle_min = -IM_PI * 0.5;
    angle_max = IM_PI * 1.5;
    t = percentage / 100.f;
    float angle = angle_min + (angle_max - angle_min) * t;
    draw_circle(center, 0.95, true, 32, radius, bar_color);
    draw_circle(center, 0.80, true, 32, radius, back_color);
    for (float s = angle_min; s < angle; s += IM_PI / 72)
        draw_dot(center, 0.87, 0.1, radius, s, true, 12, progress_color);
    draw_dot(center, 0.87, 0.1, radius, angle, true, 12, progress_color);

    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const char* text_end = label + strlen(label);
    auto draw_size = ImGui::CalcTextSize(label);
    float font_scale = radius / draw_size.x;
    draw_size *= font_scale;
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphOutlineWidth, 1.f);
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphShadowOffset, ImVec2(3.0f, 3.0f));
    ImGui::PushStyleColor(ImGuiCol_TexGlyphShadow, bar_color.base);
    draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * font_scale, center - draw_size * ImVec2(0.5, 0.6), ImGui::GetColorU32(text_color.base), label, text_end);
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

// Splitter
bool ImGui::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 1.0, 0.01);
}

// ToggleButton
void ImGui::ToggleButton(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

bool ImGui::ToggleButton(const char *str_id, bool *v, const ImVec2 &size)
{
    bool valueChange = false;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    ImGui::InvisibleButton(str_id, size);
    if (ImGui::IsItemClicked())
    {
        *v = !*v;
        valueChange = true;
    }

    ImU32 col_tint = ImGui::GetColorU32((*v ? ImGui::GetColorU32(ImGuiCol_Text) : ImGui::GetColorU32(ImGuiCol_Border)));
    ImU32 col_bg = ImGui::GetColorU32(ImGui::GetColorU32(ImGuiCol_WindowBg));
    if (ImGui::IsItemHovered())
    {
        col_bg = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    }
    if (ImGui::IsItemActive() || *v)
    {
        col_bg = ImGui::GetColorU32(ImGuiCol_Button);
    }

    draw_list->AddRectFilled(pos, pos + size, ImGui::GetColorU32(col_bg));

    auto textSize = ImGui::CalcTextSize(str_id);
    draw_list->AddText(ImVec2(pos.x + (size.x - textSize.x) / 2, pos.y), col_tint, str_id);

    return valueChange;
}

bool ImGui::BulletToggleButton(const char* label, bool* v, ImVec2 &pos, ImVec2 &size)
{
    bool valueChange = false;

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    ImVec2 old_pos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(pos);
    ImGui::InvisibleButton(label, size);
    if (ImGui::IsItemClicked())
    {
        *v = !*v;
        valueChange = true;
    }
    pos += size / 2;
    if (*v)
    {
        draw_list->AddCircleFilled(pos, draw_list->_Data->FontSize * 0.20f, IM_COL32(255, 0, 0, 255), 8);
    }
    else
    {
        draw_list->AddCircleFilled(pos, draw_list->_Data->FontSize * 0.20f, IM_COL32(128, 128, 128, 255), 8);
    }
    ImGui::SetCursorScreenPos(old_pos);
    return valueChange;
}

// CheckButton
bool ImGui::CheckButton(const char* label, bool* pvalue, bool useSmallButton, float checkedStateAlphaMult) {
    bool rv = false;
    const bool tmp = pvalue ? *pvalue : false;
    if (tmp)
    {
        ImVec4 CheckButtonColor = ImVec4(1.0, 1.0, 1.0, checkedStateAlphaMult);
        ImVec4 CheckButtonHoveredColor = ImVec4(1.0, 1.0, 1.0, checkedStateAlphaMult);
        ImVec4 CheckButtonActiveColor = ImVec4(1.0, 1.0, 1.0, checkedStateAlphaMult);
        ImGui::PushStyleColor(ImGuiCol_Button,CheckButtonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,CheckButtonHoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,CheckButtonActiveColor);
    }
    if (useSmallButton) {if (ImGui::SmallButton(label)) {if (pvalue) *pvalue=!(*pvalue);rv=true;}}
    else if (ImGui::Button(label)) {if (pvalue) *pvalue=!(*pvalue);rv=true;}
    if (tmp) ImGui::PopStyleColor(3);
    return rv;
}

// ColoredButtonV1: code posted by @ocornut here: https://github.com/ocornut/imgui/issues/4722
// [Button rounding depends on the FrameRounding Style property (but can be overridden with the last argument)]
bool ImGui::ColoredButton(const char* label, const ImVec2& size_arg, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2,float frame_rounding_override)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    ImGuiButtonFlags flags = ImGuiButtonFlags_None;
    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const bool is_gradient = bg_color_1 != bg_color_2;
    if (held || hovered)
    {
        // Modify colors (ultimately this can be prebaked in the style)
        float h_increase = (held && hovered) ? 0.02f : 0.02f;
        float v_increase = (held && hovered) ? 0.20f : 0.07f;

        ImVec4 bg1f = ImGui::ColorConvertU32ToFloat4(bg_color_1);
        ImGui::ColorConvertRGBtoHSV(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
        bg1f.x = ImMin(bg1f.x + h_increase, 1.0f);
        bg1f.z = ImMin(bg1f.z + v_increase, 1.0f);
        ImGui::ColorConvertHSVtoRGB(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
        bg_color_1 = GetColorU32(bg1f);
        if (is_gradient)
        {
            ImVec4 bg2f = ImGui::ColorConvertU32ToFloat4(bg_color_2);
            ImGui::ColorConvertRGBtoHSV(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
            bg2f.z = ImMin(bg2f.z + h_increase, 1.0f);
            bg2f.z = ImMin(bg2f.z + v_increase, 1.0f);
            ImGui::ColorConvertHSVtoRGB(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
            bg_color_2 = ImGui::GetColorU32(bg2f);
        }
        else
        {
            bg_color_2 = bg_color_1;
        }
    }
    ImGui::RenderNavHighlight(bb, id);

#if 0
    // V1 : faster but prevents rounding
    window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, bg_color_1, bg_color_1, bg_color_2, bg_color_2);
    if (g.Style.FrameBorderSize > 0.0f)
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), 0.0f, 0, g.Style.FrameBorderSize);
#endif

    // V2
    const float frameRounding = frame_rounding_override>=0.f ? frame_rounding_override : g.Style.FrameRounding;
    int vert_start_idx = window->DrawList->VtxBuffer.Size;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, frameRounding);
    int vert_end_idx = window->DrawList->VtxBuffer.Size;
    if (is_gradient)
        ImGui::ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
    if (g.Style.FrameBorderSize > 0.0f)
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), frameRounding, 0, g.Style.FrameBorderSize);

    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("[", "]");
    ImGui::PushStyleColor(ImGuiCol_Text, text_color);
    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    ImGui::PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

// ProgressBar
float ImGui::ProgressBar(const char *optionalPrefixText, float value, const float minValue, const float maxValue, 
                        const char *format, const ImVec2 &sizeOfBarWithoutTextInPixels, 
                        const ImVec4 &colorLeft, const ImVec4 &colorRight, const ImVec4 &colorBorder)
{
    if (value<minValue) value=minValue;
    else if (value>maxValue) value = maxValue;
    const float valueFraction = (maxValue==minValue) ? 1.0f : ((value-minValue)/(maxValue-minValue));
    const bool needsPercConversion = strstr(format,"%%")!=NULL;

    ImVec2 size = sizeOfBarWithoutTextInPixels;
    if (size.x<=0) size.x = ImGui::GetWindowWidth()*0.25f;
    if (size.y<=0) size.y = ImGui::GetTextLineHeightWithSpacing(); // or without

    const ImFontAtlas* fontAtlas = ImGui::GetIO().Fonts;

    if (optionalPrefixText && strlen(optionalPrefixText)>0)
    {
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s",optionalPrefixText);
        ImGui::SameLine();
    }

    if (valueFraction>0)
    {
        ImGui::Image(fontAtlas->TexID,ImVec2(size.x*valueFraction,size.y), fontAtlas->TexUvWhitePixel,fontAtlas->TexUvWhitePixel,colorLeft,colorBorder);
    }
    if (valueFraction<1)
    {
        if (valueFraction>0) ImGui::SameLine(0,0);
        ImGui::Image(fontAtlas->TexID,ImVec2(size.x*(1.f-valueFraction),size.y), fontAtlas->TexUvWhitePixel,fontAtlas->TexUvWhitePixel,colorRight,colorBorder);
    }
    ImGui::SameLine();

    ImGui::Text(format,needsPercConversion ? (valueFraction*100.f+0.0001f) : value);
    return valueFraction;
}

// Slider 2D and Slider 3D
static float Dist2(ImVec2 start, ImVec2 end)
{
    return (start[0] - end[0]) * (start[0] - end[0]) + (start[1] - end[1]) * (start[1] - end[1]);
}

static float DistToSegmentSqr(ImVec2 pos, ImVec2 start, ImVec2 end)
{
    float l2 = Dist2(start, end);
    if (l2 == 0) 
        return Dist2(pos, start);
    float t = ((pos[0] - start[0]) * (end[0] - start[0]) + (pos[1] - start[1]) * (end[1] - start[1])) / l2;
    t = ImMax(0.f, ImMin(1.f, t));
    return Dist2(pos, ImVec2(start[0] + t * (end[0] - start[0]), start[1] + t * (end[1] - start[1])));
}

static float DistOnSegmentSqr(ImVec2 pos, ImVec2 start, ImVec2 end)
{
    float dist = Dist2(start, pos);
    float to_dist = DistToSegmentSqr(pos, start, end);
    return dist - to_dist;
}


bool ImGui::SliderScalar2D(char const* pLabel, float* fValueX, float* fValueY, const float fMinX, const float fMaxX, const float fMinY, const float fMaxY, float const fZoom /*= 1.0f*/)
{
    IM_ASSERT(fMinX < fMaxX);
    IM_ASSERT(fMinY < fMaxY);
    ImGuiID const iID = ImGui::GetID(pLabel);
    ImVec2 const vSizeSubstract =  ImGui::CalcTextSize(std::to_string(1.0f).c_str()) * 1.1f;
    float const w = ImGui::CalcItemWidth();
    float const vSizeFull = (w - vSizeSubstract.x)*fZoom;
    ImVec2 const vSize(vSizeFull, vSizeFull);

    float const fHeightOffset = ImGui::GetTextLineHeight();
    ImVec2 const vHeightOffset(0.0f, fHeightOffset);

    ImVec2 vPos = ImGui::GetCursorScreenPos();
    ImRect oRect(vPos + vHeightOffset, vPos + vSize + vHeightOffset);

    ImGui::Text("%s", pLabel);
    ImGui::PushID(iID);

    ImU32 const uFrameCol = ImGui::GetColorU32(ImGuiCol_FrameBg);

    ImVec2 const vOriginPos = ImGui::GetCursorScreenPos();
    ImGui::RenderFrame(oRect.Min, oRect.Max, uFrameCol, false, 0.0f);

    float const fDeltaX = fMaxX - fMinX;
    float const fDeltaY = fMaxY - fMinY;

    bool bModified = false;
    ImVec2 const vSecurity(15.0f, 15.0f);
    if (ImGui::IsMouseHoveringRect(oRect.Min - vSecurity, oRect.Max + vSecurity) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 const vCursorPos = ImGui::GetMousePos() - oRect.Min;

        *fValueX = vCursorPos.x/(oRect.Max.x - oRect.Min.x)*fDeltaX + fMinX;
        *fValueY = fDeltaY - vCursorPos.y/(oRect.Max.y - oRect.Min.y)*fDeltaY + fMinY;

        bModified = true;
    }

    *fValueX = ImMin(ImMax(*fValueX, fMinX), fMaxX);
    *fValueY = ImMin(ImMax(*fValueY, fMinY), fMaxY);

    float const fScaleX = (*fValueX - fMinX)/fDeltaX;
    float const fScaleY = 1.0f - (*fValueY - fMinY)/fDeltaY;

    constexpr float fCursorOff = 10.0f;
    float const fXLimit = fCursorOff/oRect.GetWidth();
    float const fYLimit = fCursorOff/oRect.GetHeight();

    ImVec2 const vCursorPos((oRect.Max.x - oRect.Min.x)*fScaleX + oRect.Min.x, (oRect.Max.y - oRect.Min.y)*fScaleY + oRect.Min.y);

    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    ImVec4 const vBlue	(  70.0f/255.0f, 102.0f/255.0f, 230.0f/255.0f, 1.0f ); // TODO: choose from style
    ImVec4 const vOrange( 255.0f/255.0f, 128.0f/255.0f,  64.0f/255.0f, 1.0f ); // TODO: choose from style

    ImS32 const uBlue	= ImGui::GetColorU32(vBlue);
    ImS32 const uOrange	= ImGui::GetColorU32(vOrange);

    constexpr float fBorderThickness	= 2.0f;
    constexpr float fLineThickness		= 3.0f;
    constexpr float fHandleRadius		= 7.0f;
    constexpr float fHandleOffsetCoef	= 2.0f;

    // Cursor
    pDrawList->AddCircleFilled(vCursorPos, 5.0f, uBlue, 16);

    // Vertical Line
    if (fScaleY > 2.0f*fYLimit)
        pDrawList->AddLine(ImVec2(vCursorPos.x, oRect.Min.y + fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y - fCursorOff), uOrange, fLineThickness);
    if (fScaleY < 1.0f - 2.0f*	fYLimit)
        pDrawList->AddLine(ImVec2(vCursorPos.x, oRect.Max.y - fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y + fCursorOff), uOrange, fLineThickness);

    // Horizontal Line
    if (fScaleX > 2.0f*fXLimit)
        pDrawList->AddLine(ImVec2(oRect.Min.x + fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x - fCursorOff, vCursorPos.y), uOrange, fLineThickness);
    if (fScaleX < 1.0f - 2.0f*fYLimit)
        pDrawList->AddLine(ImVec2(oRect.Max.x - fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x + fCursorOff, vCursorPos.y), uOrange, fLineThickness);

    char pBufferX[16];
    char pBufferY[16];
    ImFormatString(pBufferX, IM_ARRAYSIZE(pBufferX), "%.5f", *(float const*)fValueX);
    ImFormatString(pBufferY, IM_ARRAYSIZE(pBufferY), "%.5f", *(float const*)fValueY);

    ImU32 const uTextCol = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);

    ImGui::SetWindowFontScale(0.75f);

    ImVec2 const vXSize = ImGui::CalcTextSize(pBufferX);
    ImVec2 const vYSize = ImGui::CalcTextSize(pBufferY);

    ImVec2 const vHandlePosX = ImVec2(vCursorPos.x, oRect.Max.y + vYSize.x*0.5f);
    ImVec2 const vHandlePosY = ImVec2(oRect.Max.x + fHandleOffsetCoef * fCursorOff + vYSize.x, vCursorPos.y);

    if (ImGui::IsMouseHoveringRect(vHandlePosX - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosX + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
        ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 const vCursorPos = ImGui::GetMousePos() - oRect.Min;

        *fValueX = vCursorPos.x/(oRect.Max.x - oRect.Min.x)*fDeltaX + fMinX;

        bModified = true;
    }
    else if (ImGui::IsMouseHoveringRect(vHandlePosY - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosY + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
            ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 const vCursorPos = ImGui::GetMousePos() - oRect.Min;

        *fValueY = fDeltaY - vCursorPos.y/(oRect.Max.y - oRect.Min.y)*fDeltaY + fMinY;

        bModified = true;
    }

    pDrawList->AddText(
        ImVec2(ImMin(ImMax(vCursorPos.x - vXSize.x*0.5f, oRect.Min.x), oRect.Min.x + vSize.x - vXSize.x),
                        oRect.Max.y + fCursorOff),
        uTextCol,
        pBufferX);
    pDrawList->AddText(
        ImVec2(oRect.Max.x + fCursorOff, ImMin(ImMax(vCursorPos.y - vYSize.y*0.5f, oRect.Min.y),
                oRect.Min.y + vSize.y - vYSize.y)),
        uTextCol,
        pBufferY);
    ImGui::SetWindowFontScale(1.0f);

    // Borders::Right
    pDrawList->AddCircleFilled(ImVec2(oRect.Max.x, vCursorPos.y), 2.0f, uOrange, 3);
    // Handle Right::Y
    pDrawList->AddNgonFilled(ImVec2(oRect.Max.x + fHandleOffsetCoef*fCursorOff + vYSize.x, vCursorPos.y), fHandleRadius, uBlue, 4);
    if (fScaleY > fYLimit)
        pDrawList->AddLine(ImVec2(oRect.Max.x, oRect.Min.y), ImVec2(oRect.Max.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
    if (fScaleY < 1.0f - fYLimit)
        pDrawList->AddLine(ImVec2(oRect.Max.x, oRect.Max.y), ImVec2(oRect.Max.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
    // Borders::Top
    pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oRect.Min.y), 2.0f, uOrange, 3);
    if (fScaleX > fXLimit)
        pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Min.y), ImVec2(vCursorPos.x - fCursorOff, oRect.Min.y), uBlue, fBorderThickness);
    if (fScaleX < 1.0f - fXLimit)
        pDrawList->AddLine(ImVec2(oRect.Max.x, oRect.Min.y), ImVec2(vCursorPos.x + fCursorOff, oRect.Min.y), uBlue, fBorderThickness);
    // Borders::Left
    pDrawList->AddCircleFilled(ImVec2(oRect.Min.x, vCursorPos.y), 2.0f, uOrange, 3);
    if (fScaleY > fYLimit)
        pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Min.y), ImVec2(oRect.Min.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
    if (fScaleY < 1.0f - fYLimit)
        pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Max.y), ImVec2(oRect.Min.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
    // Borders::Bottom
    pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oRect.Max.y), 2.0f, uOrange, 3);
    // Handle Bottom::X
    pDrawList->AddNgonFilled(ImVec2(vCursorPos.x, oRect.Max.y + vXSize.y*2.0f), fHandleRadius, uBlue, 4);
    if (fScaleX > fXLimit)
        pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Max.y), ImVec2(vCursorPos.x - fCursorOff, oRect.Max.y), uBlue, fBorderThickness);
    if (fScaleX < 1.0f - fXLimit)
        pDrawList->AddLine(ImVec2(oRect.Max.x, oRect.Max.y), ImVec2(vCursorPos.x + fCursorOff, oRect.Max.y), uBlue, fBorderThickness);

    ImGui::PopID();

    ImGui::Dummy(vHeightOffset);
    ImGui::Dummy(vHeightOffset);
    ImGui::Dummy(vSize);

    char pBufferID[64];
    ImFormatString(pBufferID, IM_ARRAYSIZE(pBufferID), "Values##%d", *(ImS32 const*)&iID);
    float const fSpeedX = fDeltaX/128.0f;
    float const fSpeedY = fDeltaY/128.0f;

    char pBufferXID[64];
    ImFormatString(pBufferXID, IM_ARRAYSIZE(pBufferXID), "X##%d", *(ImS32 const*)&iID);
    char pBufferYID[64];
    ImFormatString(pBufferYID, IM_ARRAYSIZE(pBufferYID), "Y##%d", *(ImS32 const*)&iID);

    bModified |= ImGui::DragScalar(pBufferXID, ImGuiDataType_Float, fValueX, fSpeedX, &fMinX, &fMaxX);
    bModified |= ImGui::DragScalar(pBufferYID, ImGuiDataType_Float, fValueY, fSpeedY, &fMinY, &fMaxY);

    return bModified;
}

bool ImGui::SliderScalar3D(char const* pLabel, float* pValueX, float* pValueY, float* pValueZ, float const fMinX, float const fMaxX, float const fMinY, float const fMaxY, float const fMinZ, float const fMaxZ, float const fScale /*= 1.0f*/)
{
    IM_ASSERT(fMinX < fMaxX);
    IM_ASSERT(fMinY < fMaxY);
    IM_ASSERT(fMinZ < fMaxZ);

    ImGuiID const iID = ImGui::GetID(pLabel);

    ImVec2 const vSizeSubstract = ImGui::CalcTextSize(std::to_string(1.0f).c_str()) * 1.1f;
    float const w = ImGui::CalcItemWidth();
    float const vSizeFull = w;
    float const fMinSize = (vSizeFull - vSizeSubstract.x*0.5f)*fScale*0.75f;
    ImVec2 const vSize(fMinSize, fMinSize);

    float const fHeightOffset = ImGui::GetTextLineHeight();
    ImVec2 const vHeightOffset(0.0f, fHeightOffset);

    ImVec2 vPos = ImGui::GetCursorScreenPos();
    ImRect oRect(vPos + vHeightOffset, vPos + vSize + vHeightOffset);

    ImGui::Text("%s", pLabel);

    ImGui::PushID(iID);

    ImU32 const uFrameCol	= ImGui::GetColorU32(ImGuiCol_FrameBg) | 0xFF000000;
    ImU32 const uFrameCol2	= ImGui::GetColorU32(ImGuiCol_FrameBgActive);

    float& fX = *pValueX;
    float& fY = *pValueY;
    float& fZ = *pValueZ;

    float const fDeltaX = fMaxX - fMinX;
    float const fDeltaY = fMaxY - fMinY;
    float const fDeltaZ = fMaxZ - fMinZ;

    ImVec2 const vOriginPos = ImGui::GetCursorScreenPos();

    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    float const fX3 = vSize.x/3.0f;
    float const fY3 = vSize.y/3.0f;

    ImVec2 const vStart = oRect.Min;

    ImVec2 aPositions[] = {
        ImVec2(vStart.x,			vStart.y + fX3),
        ImVec2(vStart.x,			vStart.y + vSize.y),
        ImVec2(vStart.x + 2.0f*fX3,	vStart.y + vSize.y),
        ImVec2(vStart.x + vSize.x,	vStart.y + 2.0f*fY3),
        ImVec2(vStart.x + vSize.x,	vStart.y),
        ImVec2(vStart.x + fX3,		vStart.y)
    };

    pDrawList->AddPolyline(&aPositions[0], 6, uFrameCol2, true, 1.0f);

    // Cube Shape
    pDrawList->AddLine(
        oRect.Min + ImVec2(0.0f, vSize.y),
        oRect.Min + ImVec2(fX3, 2.0f*fY3), uFrameCol2, 1.0f);
    pDrawList->AddLine(
        oRect.Min + ImVec2(fX3, 2.0f*fY3),
        oRect.Min + ImVec2(vSize.x, 2.0f*fY3), uFrameCol2, 1.0f);
    pDrawList->AddLine(
        oRect.Min + ImVec2(fX3, 0.0f),
        oRect.Min + ImVec2(fX3, 2.0f*fY3), uFrameCol2, 1.0f);

    ImGui::PopID();

    constexpr float fDragZOffsetX = 40.0f;

    ImRect oZDragRect(ImVec2(vStart.x + 2.0f*fX3 + fDragZOffsetX, vStart.y + 2.0f*fY3), ImVec2(vStart.x + vSize.x + fDragZOffsetX, vStart.y + vSize.y));

    ImVec2 const vMousePos = ImGui::GetMousePos();
    ImVec2 const vSecurity(15.0f, 15.0f);
    ImVec2 const vDragStart	(oZDragRect.Min.x, oZDragRect.Max.y);
    ImVec2 const vDragEnd	(oZDragRect.Max.x, oZDragRect.Min.y);
    bool bModified = false;
    if (ImGui::IsMouseHoveringRect(oZDragRect.Min - vSecurity, oZDragRect.Max + vSecurity) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (DistToSegmentSqr(vMousePos, vDragStart, vDragEnd) < 100.0f) // 100 is arbitrary threshold
        {
            float const fMaxDist	= std::sqrt(Dist2(vDragStart, vDragEnd));
            float const fDist		= ImMax(ImMin(std::sqrt(DistOnSegmentSqr(vMousePos, vDragStart, vDragEnd))/fMaxDist, 1.0f), 0.0f);

            fZ = fDist*fDeltaZ*fDist + fMinZ;

            bModified = true;
        }
    }

    float const fScaleZ = (fZ - fMinZ)/fDeltaZ;

    ImVec2 const vRectStart	(vStart.x, vStart.y + fX3);
    ImVec2 const vRectEnd	(vStart.x + fX3, vStart.y);
    ImRect const oXYDrag((vRectEnd - vRectStart)*fScaleZ + vRectStart,
                        (vRectEnd - vRectStart)*fScaleZ + vRectStart + ImVec2(2.0f*fX3, 2.0f*fY3));
    if (ImGui::IsMouseHoveringRect(oXYDrag.Min - vSecurity, oXYDrag.Max + vSecurity) && ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        ImVec2 const vLocalPos = ImGui::GetMousePos() - oXYDrag.Min;

        fX = vLocalPos.x/(oXYDrag.Max.x - oXYDrag.Min.x)*fDeltaX + fMinX;
        fY = fDeltaY - vLocalPos.y/(oXYDrag.Max.y - oXYDrag.Min.y)*fDeltaY + fMinY;

        bModified = true;
    }

    fX = ImMin(ImMax(fX, fMinX), fMaxX);
    fY = ImMin(ImMax(fY, fMinY), fMaxY);
    fZ = ImMin(ImMax(fZ, fMinZ), fMaxZ);

    float const fScaleX = (fX - fMinX)/fDeltaX;
    float const fScaleY = 1.0f - (fY - fMinY)/fDeltaY;

    ImVec4 const vBlue	(  70.0f/255.0f, 102.0f/255.0f, 230.0f/255.0f, 1.0f );
    ImVec4 const vOrange( 255.0f/255.0f, 128.0f/255.0f,  64.0f/255.0f, 1.0f );

    ImS32 const uBlue	= ImGui::GetColorU32(vBlue);
    ImS32 const uOrange	= ImGui::GetColorU32(vOrange);

    constexpr float fBorderThickness	= 2.0f; // TODO: move as Style
    constexpr float fLineThickness		= 3.0f; // TODO: move as Style
    constexpr float fHandleRadius		= 7.0f; // TODO: move as Style
    constexpr float fHandleOffsetCoef	= 2.0f; // TODO: move as Style

    pDrawList->AddLine(
        vDragStart,
        vDragEnd, uFrameCol2, 1.0f);
    pDrawList->AddRectFilled(
        oXYDrag.Min, oXYDrag.Max, uFrameCol);

    constexpr float fCursorOff = 10.0f;
    float const fXLimit = fCursorOff/oXYDrag.GetWidth();
    float const fYLimit = fCursorOff/oXYDrag.GetHeight();
    float const fZLimit = fCursorOff/oXYDrag.GetWidth();

    char pBufferX[16];
    char pBufferY[16];
    char pBufferZ[16];
    ImFormatString(pBufferX, IM_ARRAYSIZE(pBufferX), "%.5f", *(float const*)&fX);
    ImFormatString(pBufferY, IM_ARRAYSIZE(pBufferY), "%.5f", *(float const*)&fY);
    ImFormatString(pBufferZ, IM_ARRAYSIZE(pBufferZ), "%.5f", *(float const*)&fZ);

    ImU32 const uTextCol = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);

    ImVec2 const vCursorPos((oXYDrag.Max.x - oXYDrag.Min.x)*fScaleX + oXYDrag.Min.x, (oXYDrag.Max.y - oXYDrag.Min.y)*fScaleY + oXYDrag.Min.y);

    ImGui::SetWindowFontScale(0.75f);

    ImVec2 const vXSize = ImGui::CalcTextSize(pBufferX);
    ImVec2 const vYSize = ImGui::CalcTextSize(pBufferY);
    ImVec2 const vZSize = ImGui::CalcTextSize(pBufferZ);

    ImVec2 const vTextSlideXMin	= oRect.Min + ImVec2(0.0f, vSize.y);
    ImVec2 const vTextSlideXMax	= oRect.Min + ImVec2(2.0f*fX3, vSize.y);
    ImVec2 const vXTextPos((vTextSlideXMax - vTextSlideXMin)*fScaleX + vTextSlideXMin);

    ImVec2 const vTextSlideYMin	= oRect.Min + ImVec2(vSize.x, 2.0f*fY3);
    ImVec2 const vTextSlideYMax	= oRect.Min + ImVec2(vSize.x, 0.0f);
    ImVec2 const vYTextPos((vTextSlideYMax - vTextSlideYMin)*(1.0f - fScaleY) + vTextSlideYMin);

    ImVec2 const vTextSlideZMin = vDragStart + ImVec2(fCursorOff, fCursorOff);
    ImVec2 const vTextSlideZMax = vDragEnd   + ImVec2(fCursorOff, fCursorOff);
    ImVec2 const vZTextPos((vTextSlideZMax - vTextSlideZMin)*fScaleZ + vTextSlideZMin);

    ImVec2 const vHandlePosX = vXTextPos + ImVec2(0.0f, vXSize.y + fHandleOffsetCoef*fCursorOff);
    ImVec2 const vHandlePosY = vYTextPos + ImVec2(vYSize.x + (fHandleOffsetCoef + 1.0f)*fCursorOff, 0.0f);

    if (ImGui::IsMouseHoveringRect(vHandlePosX - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosX + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
        ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        float const fCursorPosX = ImGui::GetMousePos().x - vStart.x;

        fX = fDeltaX*fCursorPosX/(2.0f*fX3) + fMinX;

        bModified = true;
    }
    else if (ImGui::IsMouseHoveringRect(vHandlePosY - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vHandlePosY + ImVec2(fHandleRadius, fHandleRadius) + vSecurity) &&
            ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        float const fCursorPosY = ImGui::GetMousePos().y - vStart.y;

        fY = fDeltaY*(1.0f - fCursorPosY/(2.0f*fY3)) + fMinY;

        bModified = true;
    }

    pDrawList->AddText(
        ImVec2(ImMin(ImMax(vXTextPos.x - vXSize.x*0.5f, vTextSlideXMin.x), vTextSlideXMax.x - vXSize.x),
                        vXTextPos.y + fCursorOff),
        uTextCol,
        pBufferX);
    pDrawList->AddText(
        ImVec2(vYTextPos.x + fCursorOff,
                ImMin(ImMax(vYTextPos.y - vYSize.y*0.5f, vTextSlideYMax.y), vTextSlideYMin.y - vYSize.y)),
        uTextCol,
        pBufferY);
    pDrawList->AddText(
        vZTextPos,
        uTextCol,
        pBufferZ);
    ImGui::SetWindowFontScale(1.0f);

    // Handles
    pDrawList->AddNgonFilled(vHandlePosX, fHandleRadius, uBlue, 4);
    pDrawList->AddNgonFilled(vHandlePosY, fHandleRadius, uBlue, 4);

    // Vertical Line
    if (fScaleY > 2.0f*fYLimit)
        pDrawList->AddLine(ImVec2(vCursorPos.x, oXYDrag.Min.y + fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y - fCursorOff), uOrange, fLineThickness);
    if (fScaleY < 1.0f - 2.0f*	fYLimit)
        pDrawList->AddLine(ImVec2(vCursorPos.x, oXYDrag.Max.y - fCursorOff), ImVec2(vCursorPos.x, vCursorPos.y + fCursorOff), uOrange, fLineThickness);

    // Horizontal Line
    if (fScaleX > 2.0f*fXLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Min.x + fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x - fCursorOff, vCursorPos.y), uOrange, fLineThickness);
    if (fScaleX < 1.0f - 2.0f*fYLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Max.x - fCursorOff, vCursorPos.y), ImVec2(vCursorPos.x + fCursorOff, vCursorPos.y), uOrange, fLineThickness);

    // Line To Text
    // X
    if (fScaleZ > 2.0f*fZLimit)
        pDrawList->AddLine(
            ImVec2(vCursorPos.x - 0.5f*fCursorOff, oXYDrag.Max.y + 0.5f*fCursorOff),
            ImVec2(vXTextPos.x + 0.5f*fCursorOff, vXTextPos.y - 0.5f*fCursorOff), uOrange, fLineThickness
        );
    else
        pDrawList->AddLine(
            ImVec2(vCursorPos.x, oXYDrag.Max.y),
            ImVec2(vXTextPos.x, vXTextPos.y), uOrange, 1.0f
        );
    pDrawList->AddCircleFilled(vXTextPos, 2.0f, uOrange, 3);
    // Y
    if (fScaleZ < 1.0f - 2.0f*fZLimit)
        pDrawList->AddLine(
            ImVec2(oXYDrag.Max.x + 0.5f*fCursorOff, vCursorPos.y - 0.5f*fCursorOff),
            ImVec2(vYTextPos.x - 0.5f*fCursorOff, vYTextPos.y + 0.5f*fCursorOff), uOrange, fLineThickness
        );
    else
        pDrawList->AddLine(
            ImVec2(oXYDrag.Max.x, vCursorPos.y),
            ImVec2(vYTextPos.x, vYTextPos.y), uOrange, 1.0f
        );
    pDrawList->AddCircleFilled(vYTextPos, 2.0f, uOrange, 3);

    // Borders::Right
    pDrawList->AddCircleFilled(ImVec2(oXYDrag.Max.x, vCursorPos.y), 2.0f, uOrange, 3);
    if (fScaleY > fYLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Min.y), ImVec2(oXYDrag.Max.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
    if (fScaleY < 1.0f - fYLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Max.y), ImVec2(oXYDrag.Max.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
    // Borders::Top
    pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oXYDrag.Min.y), 2.0f, uOrange, 3);
    if (fScaleX > fXLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Min.y), ImVec2(vCursorPos.x - fCursorOff, oXYDrag.Min.y), uBlue, fBorderThickness);
    if (fScaleX < 1.0f - fXLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Min.y), ImVec2(vCursorPos.x + fCursorOff, oXYDrag.Min.y), uBlue, fBorderThickness);
    // Borders::Left
    pDrawList->AddCircleFilled(ImVec2(oXYDrag.Min.x, vCursorPos.y), 2.0f, uOrange, 3);
    if (fScaleY > fYLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Min.y), ImVec2(oXYDrag.Min.x, vCursorPos.y - fCursorOff), uBlue, fBorderThickness);
    if (fScaleY < 1.0f - fYLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Max.y), ImVec2(oXYDrag.Min.x, vCursorPos.y + fCursorOff), uBlue, fBorderThickness);
    // Borders::Bottom
    pDrawList->AddCircleFilled(ImVec2(vCursorPos.x, oXYDrag.Max.y), 2.0f, uOrange, 3);
    if (fScaleX > fXLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Min.x, oXYDrag.Max.y), ImVec2(vCursorPos.x - fCursorOff, oXYDrag.Max.y), uBlue, fBorderThickness);
    if (fScaleX < 1.0f - fXLimit)
        pDrawList->AddLine(ImVec2(oXYDrag.Max.x, oXYDrag.Max.y), ImVec2(vCursorPos.x + fCursorOff, oXYDrag.Max.y), uBlue, fBorderThickness);

    pDrawList->AddLine(
        oRect.Min + ImVec2(0.0f, fY3),
        oRect.Min + ImVec2(2.0f*fX3, fY3), uFrameCol2, 1.0f);
    pDrawList->AddLine(
        oRect.Min + ImVec2(2.0f*fX3, fY3),
        oRect.Min + ImVec2(vSize.x, 0.0f), uFrameCol2, 1.0f);
    pDrawList->AddLine(
        oRect.Min + ImVec2(2.0f*fX3, fY3),
        oRect.Min + ImVec2(2.0f*fX3, vSize.y), uFrameCol2, 1.0f);

    // Cursor
    pDrawList->AddCircleFilled(vCursorPos, 5.0f, uBlue, 16);
    // CursorZ
    pDrawList->AddNgonFilled((vDragEnd - vDragStart)*fScaleZ + vDragStart, fHandleRadius, uBlue, 4);

    ImGui::Dummy(vHeightOffset);
    ImGui::Dummy(vHeightOffset*1.25f);
    ImGui::Dummy(vSize);

    char pBufferID[64];
    ImFormatString(pBufferID, IM_ARRAYSIZE(pBufferID), "Values##%d", *(ImS32 const*)&iID);

    float const fMoveDeltaX = fDeltaX/128.0f; // Arbitrary
    float const fMoveDeltaY = fDeltaY/128.0f; // Arbitrary
    float const fMoveDeltaZ = fDeltaZ/128.0f; // Arbitrary

    char pBufferXID[64];
    ImFormatString(pBufferXID, IM_ARRAYSIZE(pBufferXID), "X##%d", *(ImS32 const*)&iID);
    char pBufferYID[64];
    ImFormatString(pBufferYID, IM_ARRAYSIZE(pBufferYID), "Y##%d", *(ImS32 const*)&iID);
    char pBufferZID[64];
    ImFormatString(pBufferZID, IM_ARRAYSIZE(pBufferZID), "Z##%d", *(ImS32 const*)&iID);

    bModified |= ImGui::DragScalar(pBufferXID, ImGuiDataType_Float, &fX, fMoveDeltaX, &fMinX, &fMaxX);
    bModified |= ImGui::DragScalar(pBufferYID, ImGuiDataType_Float, &fY, fMoveDeltaY, &fMinY, &fMaxY);
    bModified |= ImGui::DragScalar(pBufferZID, ImGuiDataType_Float, &fZ, fMoveDeltaZ, &fMinZ, &fMaxZ);

    return bModified;
}

// RangeSelect
bool ImGui::InputVec2(char const* pLabel, ImVec2* pValue, ImVec2 const vMinValue, ImVec2 const vMaxValue, float const fScale /*= 1.0f*/)
{
    return ImGui::SliderScalar2D(pLabel, &pValue->x, &pValue->y, vMinValue.x, vMaxValue.x, vMinValue.y, vMaxValue.y, fScale);
}

bool ImGui::InputVec3(char const* pLabel, ImVec4* pValue, ImVec4 const vMinValue, ImVec4 const vMaxValue, float const fScale /*= 1.0f*/)
{
    return ImGui::SliderScalar3D(pLabel, &pValue->x, &pValue->y, &pValue->z, vMinValue.x, vMaxValue.x, vMinValue.y, vMaxValue.y, vMinValue.z, vMaxValue.z, fScale);
}

static float Rescale01(float const x, float const min, float const max)
{
	return (x - min) / (max - min);
}

static float Rescale(float const x, float const min, float const max, float const newMin, float const newMax)
{
	return Rescale01(x, min, max) * (newMax - newMin) + newMin;
}

static ImVec2 Rescale01v(ImVec2 const v, ImVec2 const min, ImVec2 const max)
{
	return ImVec2(Rescale01(v.x, min.x, max.x), Rescale01(v.y, min.y, max.y));
}

static ImVec2 Rescalev(ImVec2 const x, ImVec2 const min, ImVec2 const max, ImVec2 const newMin, ImVec2 const newMax)
{
	ImVec2 const vNorm = Rescale01v(x, min, max);
	return ImVec2(vNorm.x * (newMax.x - newMin.x) + newMin.x, vNorm.y * (newMax.y - newMin.y) + newMin.y);
}

bool ImGui::RangeSelect2D(char const* pLabel, float* pCurMinX, float* pCurMinY, float* pCurMaxX, float* pCurMaxY, float const fBoundMinX, float const fBoundMinY, float const fBoundMaxX, float const fBoundMaxY, float const fScale /*= 1.0f*/)
{
	float& fCurMinX = *pCurMinX;
	float& fCurMinY = *pCurMinY;
	float& fCurMaxX = *pCurMaxX;
	float& fCurMaxY = *pCurMaxY;
	float const fDeltaBoundX = fBoundMaxX - fBoundMinX;
	float const fDeltaBoundY = fBoundMaxY - fBoundMinY;
	float const fDeltaX = fCurMaxX - fCurMinX;
	float const fDeltaY = fCurMaxY - fCurMinY;
	float const fScaleX = fDeltaX / fDeltaBoundX;
	float const fScaleY = fDeltaY / fDeltaBoundY;
	float const fScaleMinX = Rescale01(fCurMinX, fBoundMinX, fBoundMaxX);
	float const fScaleMinY = Rescale01(fCurMinY, fBoundMinY, fBoundMaxY);
	float const fScaleMaxX = Rescale01(fCurMaxX, fBoundMinX, fBoundMaxX);
	float const fScaleMaxY = Rescale01(fCurMaxY, fBoundMinY, fBoundMaxY);
	ImGuiID const iID = ImGui::GetID(pLabel);
	ImVec2 const vSizeSubstract = ImGui::CalcTextSize(std::to_string(1.0f).c_str()) * 1.1f;
    float const w = ImGui::CalcItemWidth();
	float const vSizeFull = (w - vSizeSubstract.x) * fScale;
	ImVec2 const vSize(vSizeFull, vSizeFull);
	float const fHeightOffset = ImGui::GetTextLineHeight();
	ImVec2 const vHeightOffset(0.0f, fHeightOffset);
	ImVec2 vPos = ImGui::GetCursorScreenPos();
	ImRect oRect(vPos + vHeightOffset, vPos + vSize + vHeightOffset);
	constexpr float fCursorOff = 10.0f;
	float const fXLimit = fCursorOff / oRect.GetWidth();
	float const fYLimit = fCursorOff / oRect.GetHeight();
	ImVec2 const vCursorPos((oRect.Max.x - oRect.Min.x) * fScaleX + oRect.Min.x, (oRect.Max.y - oRect.Min.y) * fScaleY + oRect.Min.y);
	ImGui::Dummy(vHeightOffset);
	ImGui::Dummy(vHeightOffset);
	//ImGui::Text(pLabel);
	ImGui::PushID(iID);
	ImU32 const uFrameCol = ImGui::GetColorU32(ImGuiCol_FrameBg);
	ImU32 const uFrameZoneCol = ImGui::GetColorU32(ImGuiCol_FrameBgActive);
	ImVec2 const vOriginPos = ImGui::GetCursorScreenPos();
	ImGui::RenderFrame(oRect.Min, oRect.Max, uFrameCol, false, 0.0f);
	bool bModified = false;
	ImVec2 const vSecurity(15.0f, 15.0f);
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
	ImVec4 const vBlue(70.0f / 255.0f, 102.0f / 255.0f, 230.0f / 255.0f, 1.0f);
	ImVec4 const vOrange(255.0f / 255.0f, 128.0f / 255.0f, 64.0f / 255.0f, 1.0f);
	ImS32 const uBlue = ImGui::GetColorU32(vBlue);
	ImS32 const uOrange = ImGui::GetColorU32(vOrange);
	constexpr float fBorderThickness = 2.0f;
	constexpr float fLineThickness = 3.0f;
	constexpr float fHandleRadius = 7.0f;
	constexpr float fHandleOffsetCoef = 2.0f;
	float const fRegMinX = ImLerp(oRect.Min.x, oRect.Max.x, fScaleMinX);
	float const fRegMinY = ImLerp(oRect.Min.y, oRect.Max.y, fScaleMinY);
	float const fRegMaxX = ImLerp(oRect.Min.x, oRect.Max.x, fScaleMaxX);
	float const fRegMaxY = ImLerp(oRect.Min.y, oRect.Max.y, fScaleMaxY);
	ImRect oRegionRect(fRegMinX, fRegMinY, fRegMaxX, fRegMaxY);
	ImVec2 vMinCursorPos(fRegMinX, fRegMinY);
	ImVec2 vMaxCursorPos(fRegMaxX, fRegMaxY);
	float const fRegWidth = oRegionRect.GetWidth();
	ImRect oWidthHandle(ImVec2(vMinCursorPos.x + 0.25f * fRegWidth, oRect.Min.y - 0.5f * fCursorOff), ImVec2(vMaxCursorPos.x - 0.25f * fRegWidth, oRect.Min.y + 0.5f * fCursorOff));
	float const fRegHeight = oRegionRect.GetHeight();
	ImRect oHeightHandle(ImVec2(oRect.Min.x - 0.5f * fCursorOff, vMinCursorPos.y + 0.25f * fRegHeight), ImVec2(oRect.Min.x + 0.5f * fCursorOff, vMaxCursorPos.y - 0.25f * fRegHeight));
	ImGui::RenderFrame(oRegionRect.Min, oRegionRect.Max, uFrameZoneCol, false, 0.0f);
	pDrawList->AddNgonFilled(vMinCursorPos, 5.0f, uBlue, 4);
	pDrawList->AddNgonFilled(vMaxCursorPos, 5.0f, uBlue, 4);
	ImRect oDragZone(oRegionRect.Min + ImVec2(fCursorOff, fCursorOff) + vSecurity, oRegionRect.Max - ImVec2(fCursorOff, fCursorOff) - vSecurity);
	ImRect vDragBBMin(vMinCursorPos - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vMinCursorPos + ImVec2(fHandleRadius, fHandleRadius) + vSecurity);
	ImRect vDragBBMax(vMaxCursorPos - ImVec2(fHandleRadius, fHandleRadius) - vSecurity, vMaxCursorPos + ImVec2(fHandleRadius, fHandleRadius) + vSecurity);
	ImRect vDragHandleMin(oWidthHandle.Min - ImVec2(fCursorOff, fCursorOff) - vSecurity, oWidthHandle.Max + ImVec2(fCursorOff, fCursorOff) + vSecurity);
	ImRect vDragHandleHeight(oHeightHandle.Min - ImVec2(fCursorOff, fCursorOff) - vSecurity, oHeightHandle.Max + ImVec2(fCursorOff, fCursorOff) + vSecurity);
	ImRect vDragRect(oRegionRect.Min, oRegionRect.Max);
	float const fArbitrarySpeedScaleBar = 0.0125f;
	bool hovered;
	bool held;
	bool pressed = ImGui::ButtonBehavior(vDragBBMin, ImGui::GetID("##Zone"), &hovered, &held);
	if (hovered && held)
	{
		ImVec2 const vLocalCursorPos = ImGui::GetMousePos();
		ImVec2 newVal = Rescalev(vLocalCursorPos, ImVec2(oRect.Min.x, oRect.Min.y), ImVec2(oRect.Max.x, oRect.Max.y), ImVec2(fBoundMinX, fBoundMinY), ImVec2(fBoundMaxX, fBoundMaxY));
		newVal.x = ImClamp(newVal.x, fBoundMinX, *pCurMaxX);
		newVal.y = ImClamp(newVal.y, fBoundMinY, *pCurMaxY);
		*pCurMinX = newVal.x;
		*pCurMinY = newVal.y;
		bModified = true;
	}
	else
	{
		pressed = ImGui::ButtonBehavior(vDragBBMax, ImGui::GetID("##Zone"), &hovered, &held);
		if (hovered && held)
		{
			ImVec2 const vLocalCursorPos = ImGui::GetMousePos();
					
			ImVec2 newVal = Rescalev(vLocalCursorPos, ImVec2(oRect.Min.x, oRect.Min.y), ImVec2(oRect.Max.x, oRect.Max.y), ImVec2(fBoundMinX, fBoundMinY), ImVec2(fBoundMaxX, fBoundMaxY));
			newVal.x = ImClamp(newVal.x, *pCurMinX, fBoundMaxX);
			newVal.y = ImClamp(newVal.y, *pCurMinY, fBoundMaxY);
			*pCurMaxX = newVal.x;
			*pCurMaxY = newVal.y;
			bModified = true;
		}
		else
		{
			pressed = ImGui::ButtonBehavior(vDragHandleMin, ImGui::GetID("##Zone"), &hovered, &held);
			if (hovered && held)
			{
				constexpr float fSpeedHandleWidth = 0.125f;
				float fDeltaWidth = oWidthHandle.GetCenter().x - ImGui::GetMousePos().x;
				fDeltaWidth = fSpeedHandleWidth * ImClamp(fDeltaWidth, -0.5f * oRect.GetWidth(), 0.5f * oRect.GetWidth());
				float fDeltaWidthValue = Rescale(fDeltaWidth, -0.5f * oRect.GetWidth(), 0.5f * oRect.GetWidth(), -0.5f * (fBoundMaxX - fBoundMinX), 0.5f * (fBoundMaxX - fBoundMinX));
				if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
				{
					*pCurMinX = ImClamp(*pCurMinX + fArbitrarySpeedScaleBar * fDeltaWidthValue, fBoundMinX, *pCurMaxX);
					*pCurMaxX = ImClamp(*pCurMaxX - fArbitrarySpeedScaleBar * fDeltaWidthValue, *pCurMinX, fBoundMaxX);
				}
				else
				{
					if (*pCurMinX <= fBoundMinX && fDeltaWidthValue > 0.0f)
					{
						fDeltaWidthValue = 0.0f;
					}
					else if (*pCurMaxX >= fBoundMaxX && fDeltaWidthValue < 0.0f)
					{
						fDeltaWidthValue = 0.0f;
					}
					*pCurMinX = ImClamp(*pCurMinX - fDeltaWidthValue, fBoundMinX, *pCurMaxX);
					*pCurMaxX = ImClamp(*pCurMaxX - fDeltaWidthValue, *pCurMinX, fBoundMaxX);
				}
				bModified = true;
			}
			else
			{
				pressed = ImGui::ButtonBehavior(vDragHandleHeight, ImGui::GetID("##Zone"), &hovered, &held);
				if (hovered && held)
				{
					constexpr float fSpeedHandleHeight = 0.125f;
					float fDeltaHeight = oHeightHandle.GetCenter().y - ImGui::GetMousePos().y;
					// Apply Soft-Threshold
					//fDeltaHeight = fSpeedHandleHeight*Sign(fDeltaHeight)*std::max(std::abs(fDeltaHeight) - 0.5f*fCursorOff, 0.0f);
					fDeltaHeight = fSpeedHandleHeight * ImClamp(fDeltaHeight, -0.5f * oRect.GetHeight(), 0.5f * oRect.GetHeight());
					float fDeltaHeightValue = Rescale(fDeltaHeight, -0.5f * oRect.GetHeight(), 0.5f * oRect.GetHeight(), -0.5f * (fBoundMaxY - fBoundMinY), 0.5f * (fBoundMaxY - fBoundMinY));
					if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
					{
						*pCurMinY = ImClamp(*pCurMinY + fArbitrarySpeedScaleBar * fDeltaHeightValue, fBoundMinY, *pCurMaxY);
						*pCurMaxY = ImClamp(*pCurMaxY - fArbitrarySpeedScaleBar * fDeltaHeightValue, *pCurMinY, fBoundMaxY);
					}
					else
					{
						if (*pCurMinY <= fBoundMinY && fDeltaHeightValue > 0.0f)
						{
							fDeltaHeightValue = 0.0f;
						}
						else if (*pCurMaxY >= fBoundMaxY && fDeltaHeightValue < 0.0f)
						{
							fDeltaHeightValue = 0.0f;
						}
						*pCurMinY = ImClamp(*pCurMinY - fDeltaHeightValue, fBoundMinY, *pCurMaxY);
						*pCurMaxY = ImClamp(*pCurMaxY - fDeltaHeightValue, *pCurMinY, fBoundMaxY);
					}
					bModified = true;
				}
				else
				{
					pressed = ImGui::ButtonBehavior(vDragRect, ImGui::GetID("##Zone"), &hovered, &held);
					if (hovered && held)
					{
						// Top Left
						pDrawList->AddLine(oDragZone.Min, oDragZone.Min + ImVec2(oRegionRect.GetWidth() * 0.2f, 0.0f), uFrameCol, 1.0f);
						pDrawList->AddLine(oDragZone.Min, oDragZone.Min + ImVec2(0.0f, oRegionRect.GetHeight() * 0.2f), uFrameCol, 1.0f);
						// Bottom Right
						pDrawList->AddLine(oDragZone.Max, oDragZone.Max - ImVec2(oRegionRect.GetWidth() * 0.2f, 0.0f), uFrameCol, 1.0f);
						pDrawList->AddLine(oDragZone.Max, oDragZone.Max - ImVec2(0.0f, oRegionRect.GetHeight() * 0.2f), uFrameCol, 1.0f);
						if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(oDragZone.Min, oDragZone.Max))
						{
							ImVec2 vDragDelta = ImGui::GetMousePos() - oDragZone.GetCenter();
							if (*pCurMinX <= fBoundMinX && vDragDelta.x < 0.0f)
							{
								vDragDelta.x = 0.0f;
							}
							else if (*pCurMaxX >= fBoundMaxX && vDragDelta.x > 0.0f)
							{
								vDragDelta.x = 0.0f;
							}
							if (*pCurMinY <= fBoundMinY && vDragDelta.y < 0.0f)
							{
								vDragDelta.y = 0.0f;
							}
							else if (*pCurMaxY >= fBoundMaxY && vDragDelta.y > 0.0f)
							{
								vDragDelta.y = 0.0f;
							}
							float fLocalDeltaX = Rescale(vDragDelta.x, -0.5f * oRect.GetWidth(), 0.5f * oRect.GetWidth(), -0.5f * (fBoundMaxX - fBoundMinX), 0.5f * (fBoundMaxX - fBoundMinX));
							float fLocalDeltaY = Rescale(vDragDelta.y, -0.5f * oRect.GetHeight(), 0.5f * oRect.GetHeight(), -0.5f * (fBoundMaxY - fBoundMinY), 0.5f * (fBoundMaxY - fBoundMinY));
							*pCurMinX = ImClamp(*pCurMinX + fLocalDeltaX, fBoundMinX, *pCurMaxX);
							*pCurMaxX = ImClamp(*pCurMaxX + fLocalDeltaX, *pCurMinX, fBoundMaxX);
							*pCurMinY = ImClamp(*pCurMinY + fLocalDeltaY, fBoundMinY, *pCurMaxY);
							*pCurMaxY = ImClamp(*pCurMaxY + fLocalDeltaY, *pCurMinY, fBoundMaxY);
						}
					}
				}
			}
		}
	}
	char pBufferMinX[16];
	char pBufferMaxX[16];
	char pBufferMinY[16];
	char pBufferMaxY[16];
	ImFormatString(pBufferMinX, IM_ARRAYSIZE(pBufferMinX), "%.5f", *(float const*)pCurMinX);
	ImFormatString(pBufferMaxX, IM_ARRAYSIZE(pBufferMaxX), "%.5f", *(float const*)pCurMaxX);
	ImFormatString(pBufferMinY, IM_ARRAYSIZE(pBufferMinY), "%.5f", *(float const*)pCurMinY);
	ImFormatString(pBufferMaxY, IM_ARRAYSIZE(pBufferMaxY), "%.5f", *(float const*)pCurMaxY);
	ImU32 const uTextCol = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	ImGui::SetWindowFontScale(0.75f);
	ImVec2 const vMinXSize = ImGui::CalcTextSize(pBufferMinX);
	ImVec2 const vMaxXSize = ImGui::CalcTextSize(pBufferMaxX);
	ImVec2 const vMinYSize = ImGui::CalcTextSize(pBufferMinY);
	ImVec2 const vMaxYSize = ImGui::CalcTextSize(pBufferMaxY);
	pDrawList->AddText(ImVec2(vMinCursorPos.x - 0.5f * vMinXSize.x, oRect.Max.y + fCursorOff), uTextCol, pBufferMinX);
	pDrawList->AddText(ImVec2(vMaxCursorPos.x - 0.5f * vMaxXSize.x, oRect.Max.y + fCursorOff), uTextCol, pBufferMaxX);
	pDrawList->AddText(ImVec2(oRect.Max.x + fCursorOff, vMinCursorPos.y - 0.5f * vMinYSize.y), uTextCol, pBufferMinY);
	pDrawList->AddText(ImVec2(oRect.Max.x + fCursorOff, vMaxCursorPos.y - 0.5f * vMaxYSize.y), uTextCol, pBufferMaxY);
	ImGui::SetWindowFontScale(1.0f);
	if (!oRegionRect.IsInverted())
	{
		pDrawList->AddLine(vMinCursorPos + ImVec2(fCursorOff, 0.0f), vMinCursorPos + ImVec2(2.0f * fCursorOff, 0.0f), uOrange, fLineThickness);
		pDrawList->AddLine(vMinCursorPos + ImVec2(0.0f, fCursorOff), vMinCursorPos + ImVec2(0.0f, 2.0f * fCursorOff), uOrange, fLineThickness);
		pDrawList->AddLine(vMaxCursorPos - ImVec2(fCursorOff, 0.0f), vMaxCursorPos - ImVec2(2.0f * fCursorOff, 0.0f), uOrange, fLineThickness);
		pDrawList->AddLine(vMaxCursorPos - ImVec2(0.0f, fCursorOff), vMaxCursorPos - ImVec2(0.0f, 2.0f * fCursorOff), uOrange, fLineThickness);
	}
	// Cross Center
	pDrawList->AddLine(oDragZone.GetCenter() - ImVec2(fCursorOff, 0.0f), oDragZone.GetCenter() + ImVec2(fCursorOff, 0.0f), uFrameCol, 1.0f);
	pDrawList->AddLine(oDragZone.GetCenter() - ImVec2(0.0f, fCursorOff), oDragZone.GetCenter() + ImVec2(0.0f, fCursorOff), uFrameCol, 1.0f);
	//////////////////////////////////////////////////////////////////////////
	// Top Left
	pDrawList->AddLine(oRect.Min, ImVec2(vMinCursorPos.x, oRect.Min.y), uOrange, fLineThickness);
	pDrawList->AddLine(oRect.Min, ImVec2(oRect.Min.x, vMinCursorPos.y), uOrange, fLineThickness);
	// Bottom Left
	pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Max.y), ImVec2(vMinCursorPos.x - fCursorOff, oRect.Max.y), uOrange, fLineThickness);
	pDrawList->AddLine(ImVec2(oRect.Min.x, oRect.Max.y), ImVec2(oRect.Min.x, vMaxCursorPos.y), uOrange, fLineThickness);
	// 
	pDrawList->AddLine(ImVec2(vMinCursorPos.x + fCursorOff, oRect.Max.y), ImVec2(vMaxCursorPos.x - fCursorOff, oRect.Max.y), uOrange, fLineThickness);
	pDrawList->AddLine(ImVec2(vMaxCursorPos.x + fCursorOff, oRect.Max.y), ImVec2(oRect.Max.x, oRect.Max.y), uOrange, fLineThickness);
	// Right
	pDrawList->AddLine(oRect.Max, ImVec2(oRect.Max.x, vMaxCursorPos.y + fCursorOff), uOrange, fLineThickness);
	pDrawList->AddLine(ImVec2(oRect.Max.x, vMaxCursorPos.y - fCursorOff), ImVec2(oRect.Max.x, vMinCursorPos.y + fCursorOff), uOrange, fLineThickness);
	pDrawList->AddLine(ImVec2(oRect.Max.x, vMinCursorPos.y - fCursorOff), ImVec2(oRect.Max.x, oRect.Min.y), uOrange, fLineThickness);
	// Top Right
	pDrawList->AddLine(ImVec2(oRect.Max.x, oRect.Min.y), ImVec2(vMaxCursorPos.x, oRect.Min.y), uOrange, fLineThickness);
	// Top Handle
	pDrawList->AddLine(ImVec2(vMinCursorPos.x, oRect.Min.y - fCursorOff), ImVec2(vMinCursorPos.x, oRect.Min.y + fCursorOff), uBlue, fLineThickness);
	pDrawList->AddLine(ImVec2(vMaxCursorPos.x, oRect.Min.y - fCursorOff), ImVec2(vMaxCursorPos.x, oRect.Min.y + fCursorOff), uBlue, fLineThickness);
	pDrawList->AddRectFilled(oWidthHandle.Min, oWidthHandle.Max, uBlue);
	// Left Handle
	pDrawList->AddLine(ImVec2(oRect.Min.x - fCursorOff, vMinCursorPos.y), ImVec2(oRect.Min.x + fCursorOff, vMinCursorPos.y), uBlue, fLineThickness);
	pDrawList->AddLine(ImVec2(oRect.Min.x - fCursorOff, vMaxCursorPos.y), ImVec2(oRect.Min.x + fCursorOff, vMaxCursorPos.y), uBlue, fLineThickness);
	pDrawList->AddRectFilled(oHeightHandle.Min, oHeightHandle.Max, uBlue);
	// Dots
	pDrawList->AddCircleFilled(ImVec2(oRect.Max.x, vMinCursorPos.y), 2.0f, uBlue, 3);
	pDrawList->AddCircleFilled(ImVec2(oRect.Max.x, vMaxCursorPos.y), 2.0f, uBlue, 3);
	pDrawList->AddCircleFilled(ImVec2(vMinCursorPos.x, oRect.Max.y), 2.0f, uBlue, 3);
	pDrawList->AddCircleFilled(ImVec2(vMaxCursorPos.x, oRect.Max.y), 2.0f, uBlue, 3);
	//////////////////////////////////////////////////////////////////////////
	ImGui::PopID();
	//ImGui::Dummy(vHeightOffset);
	//ImGui::Dummy(vHeightOffset);
	ImGui::Dummy(vSize);
	//ImGui::Text("Min x: %f", fCurMinX);
	//ImGui::Text("Min y: %f", fCurMinY);
	//ImGui::Text("Max x: %f", fCurMaxX);
	//ImGui::Text("Max y: %f", fCurMaxY);
	//ImGui::Dummy(vHeightOffset);
	return bModified;
}

bool ImGui::RangeSelectVec2(const char* pLabel, ImVec2* pCurMin, ImVec2* pCurMax, ImVec2 const vBoundMin, ImVec2 const vBoundMax, float const fScale /*= 1.0f*/)
{
	return ImGui::RangeSelect2D(pLabel, &pCurMin->x, &pCurMin->y, &pCurMax->x, &pCurMax->y, vBoundMin.x, vBoundMin.y, vBoundMax.x, vBoundMax.y, fScale);
}

// Bezier Widget

template<int steps>
static void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) {
    static float C[(steps + 1) * 4], *K = 0;
    if (!K) {
        K = C;
        for (unsigned step = 0; step <= steps; ++step) {
            float t = (float) step / (float) steps;
            C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
            C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
            C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
            C[step * 4 + 3] = t * t*t;               // * P3
        }
    }
    for (unsigned step = 0; step <= steps; ++step) {
        ImVec2 point = {
            K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
            K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
        };
        results[step] = point;
    }
}

float ImGui::BezierValue(float dt01, float P[4]) 
{
    enum { STEPS = 256 };
    ImVec2 Q[4] = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
    ImVec2 results[STEPS + 1];
    bezier_table<STEPS>(Q, results);
    return results[(int) ((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
}

int ImGui::Bezier(const char *label, float P[5]) 
{
    // visuals
    enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
    enum { CURVE_WIDTH = 4 }; // main curved line width
    enum { LINE_WIDTH = 1 }; // handlers: small lines width
    enum { GRAB_RADIUS = 8 }; // handlers: circle radius
    enum { GRAB_BORDER = 2 }; // handlers: circle border width
    enum { AREA_CONSTRAINED = false }; // should grabbers be constrained to grid area?
    enum { AREA_WIDTH = 128 }; // area width in pixels. 0 for adaptive size (will use max avail width)
    // curve presets
    static struct { const char *name; float points[4]; } presets [] = 
    {
        { "Linear", 0.000f, 0.000f, 1.000f, 1.000f },
        { "In Sine", 0.470f, 0.000f, 0.745f, 0.715f },
        { "In Quad", 0.550f, 0.085f, 0.680f, 0.530f },
        { "In Cubic", 0.550f, 0.055f, 0.675f, 0.190f },
        { "In Quart", 0.895f, 0.030f, 0.685f, 0.220f },
        { "In Quint", 0.755f, 0.050f, 0.855f, 0.060f },
        { "In Expo", 0.950f, 0.050f, 0.795f, 0.035f },
        { "In Circ", 0.600f, 0.040f, 0.980f, 0.335f },
        { "In Back", 0.600f, -0.28f, 0.735f, 0.045f },
        { "Out Sine", 0.390f, 0.575f, 0.565f, 1.000f },
        { "Out Quad", 0.250f, 0.460f, 0.450f, 0.940f },
        { "Out Cubic", 0.215f, 0.610f, 0.355f, 1.000f },
        { "Out Quart", 0.165f, 0.840f, 0.440f, 1.000f },
        { "Out Quint", 0.230f, 1.000f, 0.320f, 1.000f },
        { "Out Expo", 0.190f, 1.000f, 0.220f, 1.000f },
        { "Out Circ", 0.075f, 0.820f, 0.165f, 1.000f },
        { "Out Back", 0.175f, 0.885f, 0.320f, 1.275f },
        { "InOut Sine", 0.445f, 0.050f, 0.550f, 0.950f },
        { "InOut Quad", 0.455f, 0.030f, 0.515f, 0.955f },
        { "InOut Cubic", 0.645f, 0.045f, 0.355f, 1.000f },
        { "InOut Quart", 0.770f, 0.000f, 0.175f, 1.000f },
        { "InOut Quint", 0.860f, 0.000f, 0.070f, 1.000f },
        { "InOut Expo", 1.000f, 0.000f, 0.000f, 1.000f },
        { "InOut Circ", 0.785f, 0.135f, 0.150f, 0.860f },
        { "InOut Back", 0.680f, -0.55f, 0.265f, 1.550f },
        // easeInElastic: not a bezier
        // easeOutElastic: not a bezier
        // easeInOutElastic: not a bezier
        // easeInBounce: not a bezier
        // easeOutBounce: not a bezier
        // easeInOutBounce: not a bezier
    };
    // preset selector
    bool reload = 0;
    ImGui::PushID(label);
    if (ImGui::ArrowButton("##lt", ImGuiDir_Left)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##lt"), ImGuiDir_Left, ImVec2(0, 0), 0)
        if (--P[4] >= 0) reload = 1; else ++P[4];
    }
    ImGui::SameLine();
    if (ImGui::Button("Presets")) {
        ImGui::OpenPopup("!Presets");
    }
    if (ImGui::BeginPopup("!Presets")) {
        for (int i = 0; i < IM_ARRAYSIZE(presets); ++i) {
            if( i == 1 || i == 9 || i == 17 ) ImGui::Separator();
            if (ImGui::MenuItem(presets[i].name, NULL, P[4] == i)) {
                P[4] = i;
                reload = 1;
            }
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##rt", ImGuiDir_Right)) { // ImGui::ArrowButton(ImGui::GetCurrentWindow()->GetID("##rt"), ImGuiDir_Right, ImVec2(0, 0), 0)
        if (++P[4] < IM_ARRAYSIZE(presets)) reload = 1; else --P[4];
    }
    ImGui::SameLine();
    ImGui::PopID();
    if (reload) {
        memcpy(P, presets[(int) P[4]].points, sizeof(float) * 4);
    }
    // bezier widget
    const ImGuiStyle& Style = GetStyle();
    const ImGuiIO& IO = GetIO();
    ImDrawList* DrawList = GetWindowDrawList();
    ImGuiWindow* Window = GetCurrentWindow();
    if (Window->SkipItems)
        return false;
    // header and spacing
    int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
    int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
    Dummy(ImVec2(0, 3));
    // prepare canvas
    const float avail = ImGui::GetContentRegionAvail().x;
    const float dim = AREA_WIDTH > 0 ? AREA_WIDTH : avail;
    ImVec2 Canvas(dim, dim);
    ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return changed;
    const ImGuiID id = Window->GetID(label);
    hovered |= 0 != ItemHoverable(ImRect(bb.Min, bb.Min + ImVec2(avail, dim)), id);
    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);
    // background grid
    for (int i = 0; i <= Canvas.x; i += (Canvas.x / 4)) {
        DrawList->AddLine(
            ImVec2(bb.Min.x + i, bb.Min.y),
            ImVec2(bb.Min.x + i, bb.Max.y),
            GetColorU32(ImGuiCol_TextDisabled));
    }
    for (int i = 0; i <= Canvas.y; i += (Canvas.y / 4)) {
        DrawList->AddLine(
            ImVec2(bb.Min.x, bb.Min.y + i),
            ImVec2(bb.Max.x, bb.Min.y + i),
            GetColorU32(ImGuiCol_TextDisabled));
    }
    // eval curve
    ImVec2 Q[4] = { { 0, 0 }, { P[0], P[1] }, { P[2], P[3] }, { 1, 1 } };
    ImVec2 results[SMOOTHNESS + 1];
    bezier_table<SMOOTHNESS>(Q, results);
    // control points: 2 lines and 2 circles
    {
        // handle grabbers
        ImVec2 mouse = GetIO().MousePos, pos[2];
        float distance[2];
        for (int i = 0; i < 2; ++i) {
            pos[i] = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
            distance[i] = (pos[i].x - mouse.x) * (pos[i].x - mouse.x) + (pos[i].y - mouse.y) * (pos[i].y - mouse.y);
        }
        int selected = distance[0] < distance[1] ? 0 : 1;
        if( distance[selected] < (4*GRAB_RADIUS * 4*GRAB_RADIUS) )
        {
            SetTooltip("(%4.3f, %4.3f)", P[selected * 2 + 0], P[selected * 2 + 1]);
            if (/*hovered &&*/ (IsMouseClicked(0) || IsMouseDragging(0))) {
                float &px = (P[selected * 2 + 0] += GetIO().MouseDelta.x / Canvas.x);
                float &py = (P[selected * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y);
                if (AREA_CONSTRAINED) {
                    px = (px < 0 ? 0 : (px > 1 ? 1 : px));
                    py = (py < 0 ? 0 : (py > 1 ? 1 : py));
                }
                changed = true;
            }
        }
    }
    // if (hovered || changed) DrawList->PushClipRectFullScreen();
    // draw curve
    {
        ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
        for (int i = 0; i < SMOOTHNESS; ++i) {
            ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
            ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
            ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
            ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
            DrawList->AddLine(r, s, color, CURVE_WIDTH);
        }
    }
    // draw preview (cycles every 1s)
    static clock_t epoch = clock();
    ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
    for (int i = 0; i < 3; ++i) {
        double now = ((clock() - epoch) / (double)CLOCKS_PER_SEC);
        float delta = ((int) (now * 1000) % 1000) / 1000.f; delta += i / 3.f; if (delta > 1) delta -= 1;
        int idx = (int) (delta * SMOOTHNESS);
        float evalx = results[idx].x; // 
        float evaly = results[idx].y; // ImGui::BezierValue( delta, P );
        ImVec2 p0 = ImVec2(evalx, 1 - 0) * (bb.Max - bb.Min) + bb.Min;
        ImVec2 p1 = ImVec2(0, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
        ImVec2 p2 = ImVec2(evalx, 1 - evaly) * (bb.Max - bb.Min) + bb.Min;
        DrawList->AddCircleFilled(p0, GRAB_RADIUS / 2, ImColor(white));
        DrawList->AddCircleFilled(p1, GRAB_RADIUS / 2, ImColor(white));
        DrawList->AddCircleFilled(p2, GRAB_RADIUS / 2, ImColor(white));
    }
    // draw lines and grabbers
    float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
    ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
    ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
    ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
    DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
    DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
    DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
    DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
    DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
    DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));
    // if (hovered || changed) DrawList->PopClipRect();
    return changed;
}

// Color bar and ring
void ImGui::DrawHueBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, float alpha, float gamma, float offset)
{
	auto HueFunc = [alpha, offset](float const tt) -> ImU32
	{
		float t;
		if (tt - offset < 0.0f)
			t = ImFmod(1.0f + (tt - offset), 1.0f);
		else
			t = ImFmod(tt - offset, 1.0f);
		float r, g, b;
		ImGui::ColorConvertHSVtoRGB(t, 1.0f, 1.0f, r, g, b);
		int const ur = static_cast<int>(255.0f * r);
		int const ug = static_cast<int>(255.0f * g);
		int const ub = static_cast<int>(255.0f * b);
		int const ua = static_cast<int>(255.0f * alpha);
		return IM_COL32(ur, ug, ub, ua);
	};
	ImGui::DrawColorBandEx< true >(pDrawList, vpos, size, HueFunc, division, gamma);
}

void ImGui::DrawHueBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, float colorStartRGB[3], float alpha, float gamma)
{
	float h, s, v;
	ImGui::ColorConvertRGBtoHSV(colorStartRGB[0], colorStartRGB[1], colorStartRGB[2], h, s, v);
	ImGui::DrawHueBand(pDrawList, vpos, size, division, alpha, gamma, h);
}

void ImGui::DrawLumianceBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, ImVec4 const& color, float gamma)
{
	float h, s, v;
	ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, h, s, v);
	auto LumianceFunc = [h, s, v](float const t) -> ImU32
	{
		float r, g, b;
		ImGui::ColorConvertHSVtoRGB(h, s, ImLerp(0.0f, v, t), r, g, b);
		int const ur = static_cast<int>(255.0f * r);
		int const ug = static_cast<int>(255.0f * g);
		int const ub = static_cast<int>(255.0f * b);
		return IM_COL32(ur, ug, ub, 255);
	};
	ImGui::DrawColorBandEx< true >(pDrawList, vpos, size, LumianceFunc, division, gamma);
}

void ImGui::DrawSaturationBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, int division, ImVec4 const& color, float gamma)
{
	float h, s, v;
	ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, h, s, v);
	auto SaturationFunc = [h, s, v](float const t) -> ImU32
	{
		float r, g, b;
		ImGui::ColorConvertHSVtoRGB(h, ImLerp(0.0f, 1.0f, t) * s, ImLerp(0.5f, 1.0f, t) * v, r, g, b);
		int const ur = static_cast<int>(255.0f * r);
		int const ug = static_cast<int>(255.0f * g);
		int const ub = static_cast<int>(255.0f * b);
		return IM_COL32(ur, ug, ub, 255);
	};
	ImGui::DrawColorBandEx< true >(pDrawList, vpos, size, SaturationFunc, division, gamma);
}

void ImGui::DrawContrastBand(ImDrawList* pDrawList, ImVec2 const vpos, ImVec2 const size, ImVec4 const& color)
{
    ImGui::DrawColorBandEx< true >(pDrawList, vpos, size,
	[size, color](float t)
	{
		int seg = t * size.x;
        float v = ((seg & 1) == 0) ? 1.0 : (1 - t);
		return IM_COL32(v * color.x * 255.f, v * color.y * 255.f, v * color.z * 255.f, color.w * 255.f);
	}, size.x, 1.f);
}

bool ImGui::ColorRing(const char* label, float thickness, int split)
{
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	float const width = ImGui::GetContentRegionAvail().x;
	float const height = width;
	ImGui::InvisibleButton("##Zone", ImVec2(width, height), 0);
	float radius = width * 0.5f;
	const float dAngle = 2.0f * IM_PI / ((float)split);
	float angle = 2.0f * IM_PI / 3.0f;
	ImVec2 offset = curPos + ImVec2(radius, radius);
	ImVec2 const uv = ImGui::GetFontTexUvWhitePixel();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
	pDrawList->PrimReserve(split * 6, split * 4);
	for (int i = 0; i < split; ++i)
	{
		float x0 = radius * ImCos(angle);
		float y0 = radius * ImSin(angle);
		float x1 = radius * ImCos(angle + dAngle);
		float y1 = radius * ImSin(angle + dAngle);
		float x2 = (radius - thickness) * ImCos(angle + dAngle);
		float y2 = (radius - thickness) * ImSin(angle + dAngle);
		float x3 = (radius - thickness) * ImCos(angle);
		float y3 = (radius - thickness) * ImSin(angle);
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 1));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 3));
		float r0, g0, b0;
		float r1, g1, b1;
		ImGui::ColorConvertHSVtoRGB(((float)i) / ((float)(split - 1)), 1.0f, 1.0f, r0, g0, b0);
		ImGui::ColorConvertHSVtoRGB(((float)((i + 1)%split)) / ((float)(split - 1)), 1.0f, 1.0f, r1, g1, b1);
		pDrawList->PrimWriteVtx(offset + ImVec2(x0, y0), uv, IM_COL32(r0 * 255, g0 * 255, b0 * 255, 255));
		pDrawList->PrimWriteVtx(offset + ImVec2(x1, y1), uv, IM_COL32(r1 * 255, g1 * 255, b1 * 255, 255));
		pDrawList->PrimWriteVtx(offset + ImVec2(x2, y2), uv, IM_COL32(r1 * 255, g1 * 255, b1 * 255, 255));
		pDrawList->PrimWriteVtx(offset + ImVec2(x3, y3), uv, IM_COL32(r0 * 255, g0 * 255, b0 * 255, 255));
		angle += dAngle;
	}
	ImGui::PopID();
	return false;
}

static void HueSelectorEx(char const* label, ImVec2 const size, float* hueCenter, float* hueWidth, float* featherLeft, float* featherRight, float defaultVal, float ui_zoom, ImU32 triangleColor, int division, float alpha, float hideHueAlpha, float offset)
{
    ImGuiIO &io = ImGui::GetIO();
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
	ImVec2 curPos = ImGui::GetCursorScreenPos();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##ZoneHueLineSlider", size);
	ImGui::DrawHueBand(pDrawList, curPos, size, division, alpha, 1.0f, offset);
	float center = ImClamp(ImFmod(*hueCenter + offset, 1.0f), 0.0f, 1.0f - 1e-4f);
	float width = ImClamp(*hueWidth, 0.0f, 0.5f - 1e-4f);
	float featherL = ImClamp(*featherLeft, 0.0f, 0.5f - 1e-4f);
	float featherR = ImClamp(*featherRight, 0.0f, 0.5f - 1e-4f);
	float xCenter = curPos.x + center * size.x;
	if (width == 0.0f)
	{
		pDrawList->AddLine(ImVec2(xCenter, curPos.y), ImVec2(xCenter, curPos.y + size.y), IM_COL32(0, 0, 0, 255));
	}
	else
	{
		ImGui::DrawColorDensityPlotEx< true >(pDrawList,
		    [hueAlpha = hideHueAlpha, center, width, left = featherL, right = featherR](float const xx, float const) -> ImU32
		    {
		    	float x = ImFmod(xx, 1.0f);
		    	float val;
		    	if (x < center - width && x > center - width - left)
		    	{
		    		val = ImClamp((center * (-1 + hueAlpha) + left + width + x - hueAlpha * (width + x)) / left, hueAlpha, 1.0f);
		    	}
		    	else if (x < center + width + right && x > center + width)
		    	{
		    		val = ImClamp((center - center * hueAlpha + right + width - hueAlpha * width + (-1 + hueAlpha) * x) / right, hueAlpha, 1.0f);
		    	}
		    	else if (x > center - width - left && x < center + width + right)
		    	{
		    		val = 1.0f;
		    	}
		    	else if (center + width + right > 1.0f)
		    	{
		    		val = ImClamp((center - center * hueAlpha + right + width - hueAlpha * width + (-1 + hueAlpha) * (x + 1.0f)) / right, hueAlpha, 1.0f);
		    	}
		    	else if (center - width - left < 0.0f)
		    	{
		    		val = ImClamp((center * (-1 + hueAlpha) + left + width + x - 1.0f - hueAlpha * (width + x - 1.0f)) / left, hueAlpha, 1.0f);
		    	}
		    	else
		    	{
		    		val = hueAlpha;
		    	}
		    	return IM_COL32(0, 0, 0, ImPow(1.0f - val, 1.0f / 2.2f) * 255);
		    }, 0.0f, 1.0f, 0.0f, 0.0f, curPos, size, division, 1);
	}
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            auto diff = io.MouseDelta.x * ui_zoom / size.x;
            *hueCenter += diff;
            *hueCenter = ImClamp(*hueCenter, 0.f, 1.f);
        }
        if (io.MouseWheel < -FLT_EPSILON)
        {
            *hueWidth *= 0.9;
        }
        if (io.MouseWheel > FLT_EPSILON)
        {
            *hueWidth *= 1.1;
            if (*hueWidth > 0.5)
                *hueWidth = 0.5;
        }
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            *hueCenter = defaultVal;
        }
    }

    const float arrowWidth = pDrawList->_Data->FontSize;
    float arrowOffset = curPos.x + *hueCenter * size.x;
    ImGui::Dummy(ImVec2(0, arrowWidth / 2));
    ImGui::RenderArrow(pDrawList, ImVec2(arrowOffset - arrowWidth / 2, curPos.y + size.y), IM_COL32(255,255,0,255), ImGuiDir_Up);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << *hueCenter;
    std::string value_str = oss.str();
    ImVec2 str_size = ImGui::CalcTextSize(value_str.c_str(), nullptr, true);
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphShadowOffset, ImVec2(1, 1));
    pDrawList->AddText(ImVec2(curPos.x + size.x / 2 - str_size.x * 0.5f, curPos.y + size.y / 2 - arrowWidth / 2), IM_COL32(255,255,0,255), value_str.c_str());
    ImGui::PopStyleVar();
	ImGui::PopID();
}

void ImGui::HueSelector(char const* label, ImVec2 const size, float* hueCenter, float* hueWidth, float* featherLeft, float* featherRight, float defaultVal, float ui_zoom, int division, float alpha, float hideHueAlpha, float offset)
{
	HueSelectorEx(label, size, hueCenter, hueWidth, featherLeft, featherRight, defaultVal, ui_zoom, IM_COL32(255, 128, 0, 255), division, alpha, hideHueAlpha, offset);
}

void ImGui::LumianceSelector(char const* label, ImVec2 const size, float* lumCenter, float defaultVal, float ui_zoom, int division, float gamma, bool rgb_color, ImVec4 const color)
{
    ImGuiIO &io = ImGui::GetIO();
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
    ImVec2 curPos = ImGui::GetCursorScreenPos();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##ZoneLumianceSlider", size);
    if (!rgb_color)
    {
        ImGui::DrawLumianceBand(pDrawList, curPos, size, division, color, gamma);
    }
    else
    {
        ImVec2 bar_size = ImVec2(size.x, size.y / 4);
        ImVec2 r_pos = curPos;
        ImVec2 g_pos = ImVec2(curPos.x, curPos.y + size.y / 4);
        ImVec2 b_pos = ImVec2(curPos.x, curPos.y + size.y * 2 / 4);
        ImVec2 w_pos = ImVec2(curPos.x, curPos.y + size.y * 3 / 4);
        ImGui::DrawLumianceBand(pDrawList, r_pos, bar_size, division, ImVec4(1, 0, 0, 1), gamma);
        ImGui::DrawLumianceBand(pDrawList, g_pos, bar_size, division, ImVec4(0, 1, 0, 1), gamma);
        ImGui::DrawLumianceBand(pDrawList, b_pos, bar_size, division, ImVec4(0, 0, 1, 1), gamma);
        ImGui::DrawLumianceBand(pDrawList, w_pos, bar_size, division, ImVec4(1, 1, 1, 1), gamma);
    }
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            auto diff = io.MouseDelta.x * 2 * ui_zoom / size.x;
            *lumCenter += diff;
            *lumCenter = ImClamp(*lumCenter, -1.f, 1.f);
        }
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            *lumCenter = defaultVal;
        }
    }
    const float arrowWidth = pDrawList->_Data->FontSize;
    float arrowOffset = curPos.x + (*lumCenter / 2 + 0.5) * size.x;
    ImGui::Dummy(ImVec2(0, arrowWidth / 2));
    ImGui::RenderArrow(pDrawList, ImVec2(arrowOffset - arrowWidth / 2, curPos.y + size.y), IM_COL32(255,255,0,255), ImGuiDir_Up);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << *lumCenter;
    std::string value_str = oss.str();
    ImVec2 str_size = ImGui::CalcTextSize(value_str.c_str(), nullptr, true);
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphShadowOffset, ImVec2(1, 1));
    pDrawList->AddText(ImVec2(curPos.x + size.x / 2 - str_size.x * 0.5f, curPos.y + size.y / 2 - arrowWidth / 2), IM_COL32(255,255,0,255), value_str.c_str());
	ImGui::PopStyleVar();
    ImGui::PopID();
}

void ImGui::SaturationSelector(char const* label, ImVec2 const size, float* satCenter, float defaultVal, float ui_zoom, int division, float gamma, bool rgb_color, ImVec4 const color)
{
    ImGuiIO &io = ImGui::GetIO();
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
    ImVec2 curPos = ImGui::GetCursorScreenPos();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##ZoneSaturationSlider", size);
    if (!rgb_color)
    {
        ImGui::DrawSaturationBand(pDrawList, curPos, size, division, color, gamma);
    }
    else
    {
        ImVec2 bar_size = ImVec2(size.x, size.y / 4);
        ImVec2 r_pos = curPos;
        ImVec2 g_pos = ImVec2(curPos.x, curPos.y + size.y / 4);
        ImVec2 b_pos = ImVec2(curPos.x, curPos.y + size.y * 2 / 4);
        ImVec2 w_pos = ImVec2(curPos.x, curPos.y + size.y * 3 / 4);
        ImGui::DrawSaturationBand(pDrawList, r_pos, bar_size, division, ImVec4(1, 0, 0, 1), gamma);
        ImGui::DrawSaturationBand(pDrawList, g_pos, bar_size, division, ImVec4(0, 1, 0, 1), gamma);
        ImGui::DrawSaturationBand(pDrawList, b_pos, bar_size, division, ImVec4(0, 0, 1, 1), gamma);
        ImGui::DrawSaturationBand(pDrawList, w_pos, bar_size, division, ImVec4(1, 1, 1, 1), gamma);
    }
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            auto diff = io.MouseDelta.x * 2 * ui_zoom / size.x;
            *satCenter += diff;
            *satCenter = ImClamp(*satCenter, -1.f, 1.f);
        }
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            *satCenter = defaultVal;
        }
    }
    const float arrowWidth = pDrawList->_Data->FontSize;
    float arrowOffset = curPos.x + (*satCenter / 2 + 0.5) * size.x;
    ImGui::Dummy(ImVec2(0, arrowWidth / 2));
    ImGui::RenderArrow(pDrawList, ImVec2(arrowOffset - arrowWidth / 2, curPos.y + size.y), IM_COL32(255,255,0,255), ImGuiDir_Up);
	std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << *satCenter;
    std::string value_str = oss.str();
    ImVec2 str_size = ImGui::CalcTextSize(value_str.c_str(), nullptr, true);
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphShadowOffset, ImVec2(1, 1));
    pDrawList->AddText(ImVec2(curPos.x + size.x / 2 - str_size.x * 0.5f, curPos.y + size.y / 2 - arrowWidth / 2), IM_COL32(255,255,0,255), value_str.c_str());
    ImGui::PopStyleVar();
    ImGui::PopID();
}

void ImGui::ContrastSelector(char const* label, ImVec2 const size, float* conCenter, float defaultVal, float ui_zoom, bool rgb_color, ImVec4 const color)
{
    ImGuiIO &io = ImGui::GetIO();
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
    ImVec2 curPos = ImGui::GetCursorScreenPos();
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##ZoneContrastSlider", size);
    if (!rgb_color)
    {
        ImGui::DrawContrastBand(pDrawList, curPos, size, color);
    }
    else
    {
        ImVec2 bar_size = ImVec2(size.x, size.y / 4);
        ImVec2 r_pos = curPos;
        ImVec2 g_pos = ImVec2(curPos.x, curPos.y + size.y / 4);
        ImVec2 b_pos = ImVec2(curPos.x, curPos.y + size.y * 2 / 4);
        ImVec2 w_pos = ImVec2(curPos.x, curPos.y + size.y * 3 / 4);
        ImGui::DrawContrastBand(pDrawList, r_pos, bar_size, ImVec4(1, 0, 0, 1));
        ImGui::DrawContrastBand(pDrawList, g_pos, bar_size, ImVec4(0, 1, 0, 1));
        ImGui::DrawContrastBand(pDrawList, b_pos, bar_size, ImVec4(0, 0, 1, 1));
        ImGui::DrawContrastBand(pDrawList, w_pos, bar_size, ImVec4(1, 1, 1, 1));
    }
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            auto diff = io.MouseDelta.x * 4 * ui_zoom / size.x;
            *conCenter += diff;
            *conCenter = ImClamp(*conCenter, 0.f, 4.f);
        }
        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            *conCenter = defaultVal;
        }
    }
    const float arrowWidth = pDrawList->_Data->FontSize;
    float arrowOffset = curPos.x + (*conCenter / 4) * size.x;
    ImGui::Dummy(ImVec2(0, arrowWidth / 2));
    ImGui::RenderArrow(pDrawList, ImVec2(arrowOffset - arrowWidth / 2, curPos.y + size.y), IM_COL32(255,255,0,255), ImGuiDir_Up);
	std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << *conCenter;
    std::string value_str = oss.str();
    ImVec2 str_size = ImGui::CalcTextSize(value_str.c_str(), nullptr, true);
    ImGui::PushStyleVar(ImGuiStyleVar_TexGlyphShadowOffset, ImVec2(1, 1));
    pDrawList->AddText(ImVec2(curPos.x + size.x / 2 - str_size.x * 0.5f, curPos.y + size.y / 2 - arrowWidth / 2), rgb_color ? IM_COL32(255,255,0,255) : IM_COL32(0,0,0,255), value_str.c_str());
    ImGui::PopStyleVar();
    ImGui::PopID();
}

static bool isCircleContainsPoint(ImVec2 point, float radius, ImVec2 center)
{
    float dist = (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y);
    return dist <= radius * radius;
}

static float PointToAngle(float dx, float dy)
{
    float hAngle = 0;
    if (dy == 0 && dx > 0)
        hAngle = 0.f;           // positive x axis
    else if (dy == 0 && dx < 0)
        hAngle = 180.f;         // negative x axis
    else if (dx == 0 && dy < 0)
        hAngle = 90.f;          // positive y axis
    else if (dx == 0 && dy > 0)
        hAngle = 270.f;         // negative y axis
    else if (dx > 0 && dy > 0)
        hAngle = 90.f - atan2(dx, dy) * 180.f / M_PI;   // first quadrant
    else if (dx < 0 && dy > 0)
        hAngle = 90.f + atan2(-dx, dy) * 180.f / M_PI;  // second quadrant
    else if (dx < 0 && dy < 0)
        hAngle = 270.f - atan2(-dx, -dy) * 180.f / M_PI;// third quadrant
    else if (dx > 0 && dy < 0)
        hAngle = 270.f + atan2(dx, -dy) * 180.f / M_PI; // fourth quadrant
    return hAngle;
}

void ImGui::BalanceSelector(char const* label, ImVec2 const size, ImVec4 * rgba, ImVec4 defaultVal, float ui_zoom, int division, float thickness, float colorOffset)
{
    ImGuiIO &io = ImGui::GetIO();
	ImGuiID const iID = ImGui::GetID(label);
	ImGui::PushID(iID);
    auto curPos = ImGui::GetCursorScreenPos();
    const float ringDiameter = size.x > size.y ? size.x / 2 : size.y / 2;
    const float ringRadius = ringDiameter / 2;
    auto ringPos = curPos + ImVec2((size.x - ringDiameter) / 4, 10);
    auto center_point = ringPos + ImVec2(ringRadius, ringRadius);
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton("##ZoneBalanceSlider", ImVec2(size.x, ringDiameter + 20));
    ImGui::DrawColorRingEx< true >(pDrawList, ringPos, ImVec2(ringDiameter, ringDiameter), thickness,
		[rgba](float t)
	{
		float r, g, b;
		ImGui::ColorConvertHSVtoRGB(t, 1.0f, 1.0f, r, g, b);

		return IM_COL32(r * 255, g * 255, b * 255, 255);
	}, division, colorOffset);
    
    if (ImGui::IsItemHovered())
    {
        if (isCircleContainsPoint(io.MousePos, ringRadius, center_point))
        {
            float x_offset = (io.MousePos.x - center_point.x) / ringRadius;
            float y_offset = -(io.MousePos.y - center_point.y) / ringRadius;
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                *rgba = defaultVal;
            }
            else if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {

            }
            float angle = PointToAngle(x_offset, y_offset);
            float length = sqrt(x_offset * x_offset + y_offset * y_offset);
            float r, g, b;
		    ImGui::ColorConvertHSVtoRGB(angle / 360.0, length, 1.0f, r, g, b);
            ImGui::BeginTooltip();
            ImGui::Text("a=%f", angle);
            ImGui::Text("l=%f", length);
            ImGui::Text("r=%f", r);
            ImGui::Text("g=%f", g);
            ImGui::Text("b=%f", b);
            ImGui::EndTooltip();
        }
    }
    ImGui::PushItemWidth(size.x / 3);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::DragFloat("##R", &rgba->x, 0.005f, -1.0f, 1.0f); ImGui::SameLine();
	ImGui::DragFloat("##G", &rgba->y, 0.005f, -1.0f, 1.0f); ImGui::SameLine();
    ImGui::DragFloat("##B", &rgba->z, 0.005f, -1.0f, 1.0f);
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
    ImGui::DragFloat("##W", &rgba->w, 0.005f, -1.0f, 1.0f);
    ImGui::PopID();
}

// imgInspect
inline void histogram(const int width, const int height, const unsigned char* const bits)
{
    unsigned int count[4][256] = {0};
    const unsigned char* ptrCols = bits;
    ImGui::InvisibleButton("histogram", ImVec2(256, 128));
    for (int l = 0; l < height * width; l++)
    {
        count[0][*ptrCols++]++;
        count[1][*ptrCols++]++;
        count[2][*ptrCols++]++;
        count[3][*ptrCols++]++;
    }
    unsigned int maxv = count[0][0];
    unsigned int* pCount = &count[0][0];
    for (int i = 0; i < 3 * 256; i++, pCount++)
    {
        maxv = (maxv > *pCount) ? maxv : *pCount;
    }
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const ImVec2 rmin = ImGui::GetItemRectMin();
    const ImVec2 rmax = ImGui::GetItemRectMax();
    const ImVec2 size = ImGui::GetItemRectSize();
    const float hFactor = size.y / float(maxv);
    for (int i = 0; i <= 10; i++)
    {
        float ax = rmin.x + (size.x / 10.f) * float(i);
        float ay = rmin.y + (size.y / 10.f) * float(i);
        drawList->AddLine(ImVec2(rmin.x, ay), ImVec2(rmax.x, ay), 0x80808080);
        drawList->AddLine(ImVec2(ax, rmin.y), ImVec2(ax, rmax.y), 0x80808080);
    }
    const float barWidth = (size.x / 256.f);
    for (int j = 0; j < 256; j++)
    {
        // pixel count << 2 + color index(on 2 bits)
        uint32_t cols[3] = {(count[0][j] << 2), (count[1][j] << 2) + 1, (count[2][j] << 2) + 2};
        if (cols[0] > cols[1])
            ImSwap(cols[0], cols[1]);
        if (cols[1] > cols[2])
            ImSwap(cols[1], cols[2]);
        if (cols[0] > cols[1])
            ImSwap(cols[0], cols[1]);
        float heights[3];
        uint32_t colors[3];
        uint32_t currentColor = 0xFFFFFFFF;
        for (int i = 0; i < 3; i++)
        {
            heights[i] = rmax.y - (cols[i] >> 2) * hFactor;
            colors[i] = currentColor;
            currentColor -= 0xFF << ((cols[i] & 3) * 8);
        }
        float currentHeight = rmax.y;
        const float left = rmin.x + barWidth * float(j);
        const float right = left + barWidth;
        for (int i = 0; i < 3; i++)
        {
            if (heights[i] >= currentHeight)
            {
                continue;
            }
            drawList->AddRectFilled(ImVec2(left, currentHeight), ImVec2(right, heights[i]), colors[i]);
            currentHeight = heights[i];
        }
    }
}

inline void drawNormal(ImDrawList* draw_list, const ImRect& rc, float x, float y)
{
    draw_list->AddCircle(rc.GetCenter(), rc.GetWidth() / 2.f, 0x20AAAAAA, 24, 1.f);
    draw_list->AddCircle(rc.GetCenter(), rc.GetWidth() / 4.f, 0x20AAAAAA, 24, 1.f);
    draw_list->AddLine(rc.GetCenter(), rc.GetCenter() + ImVec2(x, y) * rc.GetWidth() / 2.f, 0xFF0000FF, 2.f);
}
void ImGui::ImageInspect(const int width,
                        const int height,
                        const unsigned char* const bits,
                        ImVec2 mouseUVCoord,
                        ImVec2 displayedTextureSize,
                        bool histogram_full,
                        int zoom_size)
{
    ImGui::BeginTooltip();
    ImGui::BeginGroup();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    static const float zoomRectangleWidth = 80.f;
    // bitmap zoom
    ImGui::InvisibleButton("AnotherInvisibleMan", ImVec2(zoomRectangleWidth, zoomRectangleWidth));
    const ImRect pickRc(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    draw_list->AddRectFilled(pickRc.Min, pickRc.Max, 0xFF000000);
    static int zoomSize = zoom_size / 2;
    uint32_t zoomData[(zoomSize * 2 + 1) * (zoomSize * 2 + 1)];
    const float quadWidth = zoomRectangleWidth / float(zoomSize * 2 + 1);
    const ImVec2 quadSize(quadWidth, quadWidth);
    const int basex = ImClamp(int(mouseUVCoord.x * width), zoomSize, width - zoomSize);
    const int basey = ImClamp(int(mouseUVCoord.y * height), zoomSize, height - zoomSize);
    for (int y = -zoomSize; y <= zoomSize; y++)
    {
        for (int x = -zoomSize; x <= zoomSize; x++)
        {
            uint32_t texel = ((uint32_t*)bits)[(basey - y) * width + x + basex];
            ImVec2 pos = pickRc.Min + ImVec2(float(x + zoomSize), float(zoomSize - y)) * quadSize;
            draw_list->AddRectFilled(pos, pos + quadSize, texel);
        }
    }
    //ImGui::SameLine();
    // center quad
    const ImVec2 pos = pickRc.Min + ImVec2(float(zoomSize), float(zoomSize)) * quadSize;
    draw_list->AddRect(pos, pos + quadSize, 0xFF0000FF, 0.f, 15, 2.f);
    // normal direction
    ImGui::InvisibleButton("AndOneMore", ImVec2(zoomRectangleWidth, zoomRectangleWidth));
    ImRect normRc(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    for (int y = -zoomSize; y <= zoomSize; y++)
    {
        for (int x = -zoomSize; x <= zoomSize; x++)
        {
            uint32_t texel = ((uint32_t*)bits)[(basey - y) * width + x + basex];
            const ImVec2 posQuad = normRc.Min + ImVec2(float(x + zoomSize), float(zoomSize - y)) * quadSize;
            //draw_list->AddRectFilled(pos, pos + quadSize, texel);
            const float nx = float(texel & 0xFF) / 128.f - 1.f;
            const float ny = float((texel & 0xFF00)>>8) / 128.f - 1.f;
            const ImRect rc(posQuad, posQuad + quadSize);
            drawNormal(draw_list, rc, nx, ny);
        }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    uint32_t texel = ((uint32_t*)bits)[basey * width + basex];
    ImVec4 color = ImColor(texel);
    ImVec4 colHSV;
    ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z, colHSV.x, colHSV.y, colHSV.z);
    ImGui::Text("U %1.3f V %1.3f", mouseUVCoord.x, mouseUVCoord.y);
    ImGui::Text("Coord %d %d", int(mouseUVCoord.x * width), int(mouseUVCoord.y * height));
    ImGui::Separator();
    ImGui::Text("R 0x%02x  G 0x%02x  B 0x%02x", int(color.x * 255.f), int(color.y * 255.f), int(color.z * 255.f));
    ImGui::Text("R %1.3f G %1.3f B %1.3f", color.x, color.y, color.z);
    ImGui::Separator();
    ImGui::Text(
        "H 0x%02x  S 0x%02x  V 0x%02x", int(colHSV.x * 255.f), int(colHSV.y * 255.f), int(colHSV.z * 255.f));
    ImGui::Text("H %1.3f S %1.3f V %1.3f", colHSV.x, colHSV.y, colHSV.z);
    ImGui::Separator();
    ImGui::Text("Alpha 0x%02x", int(color.w * 255.f));
    ImGui::Text("Alpha %1.3f", color.w);
    ImGui::Separator();
    ImGui::Text("Size %d, %d", int(displayedTextureSize.x), int(displayedTextureSize.y));
    ImGui::EndGroup();
    if (histogram_full)
    {
        histogram(width, height, bits);
    }
    else
    {
        for (int y = -zoomSize; y <= zoomSize; y++)
        {
            for (int x = -zoomSize; x <= zoomSize; x++)
            {
                uint32_t texel = ((uint32_t*)bits)[(basey - y) * width + x + basex];
                zoomData[(y + zoomSize) * zoomSize * 2 + x + zoomSize] = texel;
            }
        }
        histogram(zoomSize * 2, zoomSize * 2, (const unsigned char*)zoomData);
    }
    ImGui::EndTooltip();
}
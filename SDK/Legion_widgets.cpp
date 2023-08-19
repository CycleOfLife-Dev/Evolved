// dear Legion, v1.76 WIP
// (widgets code)

/*

Index of this file:

// [SECTION] Forward Declarations
// [SECTION] Widgets: Text, etc.
// [SECTION] Widgets: Main (Button, Image, Checkbox, RadioButton, ProgressBar, Bullet, etc.)
// [SECTION] Widgets: Low-level Layout helpers (Spacing, Dummy, NewLine, Separator, etc.)
// [SECTION] Widgets: ComboBox
// [SECTION] Data Type and Data Formatting Helpers
// [SECTION] Widgets: DragScalar, DragFloat, DragInt, etc.
// [SECTION] Widgets: SliderScalar, SliderFloat, SliderInt, etc.
// [SECTION] Widgets: InputScalar, InputFloat, InputInt, etc.
// [SECTION] Widgets: InputText, InputTextMultiline
// [SECTION] Widgets: ColorEdit, ColorPicker, ColorButton, etc.
// [SECTION] Widgets: TreeNode, CollapsingHeader, etc.
// [SECTION] Widgets: Selectable
// [SECTION] Widgets: ListBox
// [SECTION] Widgets: PlotLines, PlotHistogram
// [SECTION] Widgets: Value helpers
// [SECTION] Widgets: MenuItem, BeginMenu, EndMenu, etc.
// [SECTION] Widgets: BeginTabBar, EndTabBar, etc.
// [SECTION] Widgets: BeginTabItem, EndTabItem, etc.
// [SECTION] Widgets: Columns, BeginColumns, EndColumns, etc.

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Legion.h"
#ifndef Legion_DISABLE

#ifndef Legion_DEFINE_MATH_OPERATORS
#define Legion_DEFINE_MATH_OPERATORS
#endif
#include "Legion_internal.h"

#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#if defined(_MSC_VER) && _MSC_VER >= 1922 // MSVC 2019 16.2 or later
#pragma warning (disable: 5054)     // operator '|': deprecated between enumerations of different types
#endif
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"         // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"            // warning : comparing floating point with == or != is unsafe   // storing and comparing against same constants (typically 0.0f) is ok.
#pragma clang diagnostic ignored "-Wformat-nonliteral"      // warning : format string is not a string literal              // passing non-literal to vsnformat(). yes, user passing incorrect format strings can crash the code.
#pragma clang diagnostic ignored "-Wsign-conversion"        // warning : implicit conversion changes signedness             //
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant              // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"       // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wdeprecated-enum-enum-conversion")
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion" // warning: bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlagsPrivate_') is deprecated
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wformat-nonliteral"        // warning: format not a string literal, format string not checked
#pragma GCC diagnostic ignored "-Wclass-memaccess"          // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

//-------------------------------------------------------------------------
// Data
//-------------------------------------------------------------------------

// Those MIN/MAX values are not define because we need to point to them
static const signed char    IM_S8_MIN  = -128;
static const signed char    IM_S8_MAX  = 127;
static const unsigned char  IM_U8_MIN  = 0;
static const unsigned char  IM_U8_MAX  = 0xFF;
static const signed short   IM_S16_MIN = -32768;
static const signed short   IM_S16_MAX = 32767;
static const unsigned short IM_U16_MIN = 0;
static const unsigned short IM_U16_MAX = 0xFFFF;
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

//-------------------------------------------------------------------------
// [SECTION] Forward Declarations
//-------------------------------------------------------------------------

// For InputTextEx()
static bool             InputTextFilterCharacter(unsigned int* p_char, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data);
static int              InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
static ImVec2           InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);

//-------------------------------------------------------------------------
// [SECTION] Widgets: Text, etc.
//-------------------------------------------------------------------------
// - TextEx() [Internal]
// - TextUnformatted()
// - Text()
// - TextV()
// - TextColored()
// - TextColoredV()
// - TextDisabled()
// - TextDisabledV()
// - TextWrapped()
// - TextWrappedV()
// - LabelText()
// - LabelTextV()
// - BulletText()
// - BulletTextV()
//-------------------------------------------------------------------------

/*bool Legion::TextExR(const char* No, ImTextureID Lock, ImTextureID Rules, const char* name, const char* mode, const char* map, const char* playercount, ImTextureID RoomStatus, ImTextureID SelectBG, ImTextureID HoverBG, ImFont* font, float font_size, , const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

    ImRect No(text_pos, text_pos + text_size + ImVec2(191, 20));
    ImRect Title(text_pos, text_pos + text_size + ImVec2(191, 20));
    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));
    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));
    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));
    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    const ImRect BGbb(window->DC.CursorPos - ImVec2(0, 32), window->DC.CursorPos + ImVec2(179, 7));

    if (BackGround != NULL)
        window->DrawList->AddImage(BackGround, BGbb.Min, BGbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RImbb(window->DC.CursorPos + ImVec2(170, -22), window->DC.CursorPos + ImVec2(154, -6));

    if (Rank != NULL)
        window->DrawList->AddImage(Rank, RImbb.Min, RImbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    RenderTextClippedExN(font, font_size, bb.Min + ImVec2(30, 16), bb.Max, text, text_end, &text_size, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonRight);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }
    if (pressed)
        MarkItemEdited(id);

    return pressed;
}
*/
bool Legion::PlayerList(const char* text, ImFont* font, float font_size, ImTextureID BackGround, ImTextureID Level, ImTextureID Rank, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    
    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, NULL, false, wrap_width);

    //const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(191, 32));
    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    const ImRect BGbb(window->DC.CursorPos - ImVec2(0, 32), window->DC.CursorPos + ImVec2(179, 7));

    if (BackGround != NULL)
        window->DrawList->AddImage(BackGround, BGbb.Min, BGbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RImbb(window->DC.CursorPos + ImVec2(170, -22), window->DC.CursorPos + ImVec2(154, -6));

    if (Rank != NULL)
        window->DrawList->AddImage(Rank, RImbb.Min, RImbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    TextColorButton(font, font_size, bb.Min + ImVec2(30, 16), bb.Max, text, NULL, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonRight);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }
    if (pressed)
        MarkItemEdited(id);

    return pressed;
}

bool Legion::ReportPlayerEx(const char* text, ImFont* font, float font_size, const char* text_end, bool IsSelectAble, ImTextureID SelectAble, ImTextureID Level, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;


    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RImbb(window->DC.CursorPos + ImVec2(170, -22), window->DC.CursorPos + ImVec2(154, -6));

    TextColorButton(font, font_size, bb.Min + ImVec2(30, 16), bb.Max, text, text_end, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);


    if (IsSelectAble && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(177, 36));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 3), SELbb.Max + ImVec2(0, 3), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    return pressed;
}

bool Legion::ReportPlayer(const char* text, ImFont* font, float font_size, int* v, int v_button, ImTextureID SelectAble, ImTextureID Level, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = ReportPlayerEx(text, font, font_size, NULL, *v == v_button, SelectAble, Level, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::FriendsPlayerEx(const char* text, ImFont* font, float font_size, const char* text_end, bool IsSelectAble, ImTextureID SelectAble, ImTextureID Level, ImTextureID Rank, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

    ImRect bb(text_pos, text_pos + text_size + ImVec2(270, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;


    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    ImRect RImbb(window->DC.CursorPos + ImVec2(250, -22), window->DC.CursorPos + ImVec2(234, -6));

    if (Rank != NULL)
        window->DrawList->AddImage(Rank, RImbb.Min, RImbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    TextColorButton(font, font_size, bb.Min + ImVec2(30, 16), bb.Max, text, text_end, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);


    if (IsSelectAble && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(277, 36));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 5), SELbb.Max + ImVec2(0, 5), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    return pressed;
}

bool Legion::FriendsPlayer(const char* text, ImFont* font, float font_size, int* v, int v_button, ImTextureID SelectAble, ImTextureID Level, ImTextureID Rank, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = FriendsPlayerEx(text, font, font_size, NULL, *v == v_button, SelectAble, Level, Rank, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ClanPlayerEx(const char* text, ImFont* font, float font_size, const char* text_end, bool IsSelectAble, ImTextureID SelectAble, ImTextureID Level, ImTextureID Clan, ImTextureID Rank, ImTextureID StaffIcon, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

    ImRect bb(text_pos, text_pos + text_size + ImVec2(270, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect Imbb2(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Clan != NULL)
        window->DrawList->AddImage(Clan, Imbb2.Min + ImVec2(34, 0), Imbb2.Max + ImVec2(34, 0), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RSImbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Rank != NULL)
        window->DrawList->AddImage(Rank, RSImbb.Min + ImVec2(64, 0), RSImbb.Max + ImVec2(64, 0), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RImbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(40, 1));
    if (StaffIcon != NULL)
        window->DrawList->AddImage(StaffIcon, RImbb.Min + ImVec2(210, 0), RImbb.Max + ImVec2(210, 0), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    TextColorButton(font, font_size, bb.Min + ImVec2(85, 16), bb.Max, text, text_end, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);


    if (IsSelectAble && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(277, 36));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 5), SELbb.Max + ImVec2(0, 5), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    return pressed;
}

bool Legion::ClanPlayer(const char* text, ImFont* font, float font_size, int* v, int v_button, ImTextureID SelectAble, ImTextureID Level, ImTextureID Clan, ImTextureID Rank, ImTextureID StaffIcon, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = ClanPlayerEx(text, font, font_size, NULL, *v == v_button, SelectAble, Level, Clan, Rank, StaffIcon, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::RoomFilterEx(const char* cid, const char* text, ImFont* font, float font_size, const ImVec2& size, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    LegionID id = window->GetID(cid);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);

    ImRect bb(text_pos, text_pos +  size);

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    if (text != NULL)
        TextColorButton(font, font_size, bb.Min, bb.Max, text, NULL, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    return pressed;
}

//bool Legion::RoomFilter(const char* cid, const char* text, ImFont* font, float font_size, const ImVec2& size, const ImVec2& align, LegionTextFlags flags)
//{
//    const bool pressed = RoomFilterEx(cid, text, font, font_size, size, align, flags);
//    if (pressed)
//        *v = v_button;
//    return pressed;
//}

bool Legion::VoteKickPlayerEx(const char* text, ImFont* font, float font_size, const char* text_end, bool IsSelectAble, ImTextureID SelectAble, ImTextureID Level, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);
    const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

    ImRect bb(text_pos, text_pos + text_size + ImVec2(191, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

  
    ImRect Imbb(window->DC.CursorPos - ImVec2(-4, 31), window->DC.CursorPos + ImVec2(36, 1));
    if (Level != NULL)
        window->DrawList->AddImage(Level, Imbb.Min, Imbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect RImbb(window->DC.CursorPos + ImVec2(170, -22), window->DC.CursorPos + ImVec2(230, -6));

    TextColorButton(font, font_size, bb.Min + ImVec2(30, 16), bb.Max, text, text_end, NULL, ImVec2(0.1f, 0.1f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);


    if (IsSelectAble && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(230, 35));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 5), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    return pressed;
}

bool Legion::VoteKickPlayer(const char* text, ImFont* font, float font_size, int* v, int v_button, ImTextureID SelectAble, ImTextureID Level, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = VoteKickPlayerEx(text, font, font_size, NULL, *v == v_button, SelectAble, Level, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::VoteKickReasonsPlayerEx(const char* text, ImFont* font, float font_size, const char* text_end, bool IsSelectAble, ImTextureID SelectAble, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    LegionID id = window->GetID(text);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;

    const float wrap_width = CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x);

    ImRect bb(text_pos, text_pos + ImVec2(230, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    TextColorButton(font, font_size, bb.Min, bb.Max, text, text_end, NULL, ImVec2(0.5f, 0.5f), &bb);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);


    if (IsSelectAble && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(230, 19));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 5), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    return pressed;
}

bool Legion::VoteKickReasonsPlayer(const char* text, ImFont* font, float font_size, int* v, int v_button, ImTextureID SelectAble, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = VoteKickReasonsPlayerEx(text, font, font_size, NULL, *v == v_button, SelectAble, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::NewClanPlayerListEx(const char* NickName, const char* Level, const char* Location, const char* Rank, const char* Point, const char* Battles, ImTextureID SelectAble, ImTextureID Online, ImTextureID Offline, const char* PlayerStatus, bool IsSelected, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(NickName);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    ImRect bb(text_pos, text_pos + ImVec2(600, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    const ImRect BGbb(text_pos, text_pos + ImVec2(15, 14));


    if (Online != NULL && strcmp(PlayerStatus, "Online") == 0)
        window->DrawList->AddImage(Online, BGbb.Min + ImVec2(2, 3), BGbb.Max + ImVec2(2, 3), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    else if (Offline != NULL && strcmp(PlayerStatus, "Offline") == 0)
        window->DrawList->AddImage(Offline, BGbb.Min + ImVec2(2, 3), BGbb.Max + ImVec2(2, 3), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    ImRect bbNick(text_pos, text_pos + ImVec2(120, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x + 5, bbNick.Min.y), ImVec2(bbNick.Max.x + 5, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(140, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 71, bbLev.Min.y), ImVec2(bbLev.Max.x + 71, bbLev.Max.y));
    ImRect bbLoc(text_pos, text_pos + ImVec2(275, 20));
    ImRect bbLocR(ImVec2(bbLoc.Min.x + 100, bbLoc.Min.y), ImVec2(bbLoc.Max.x + 100, bbLoc.Max.y));
    ImRect bbRank(text_pos, text_pos + ImVec2(400, 20));
    ImRect bbRankR(ImVec2(bbRank.Min.x + 160, bbRank.Min.y), ImVec2(bbRank.Max.x + 160, bbRank.Max.y));
    ImRect bbPoint(text_pos, text_pos + ImVec2(440, 20));
    ImRect bbPointR(ImVec2(bbRank.Min.x + 233, bbRank.Min.y), ImVec2(bbRank.Max.x + 233, bbRank.Max.y));
    ImRect bbBattels(text_pos, text_pos + ImVec2(470, 20));
    ImRect bbBattelsR(ImVec2(bbBattels.Min.x + 281, bbBattels.Min.y), ImVec2(bbBattels.Max.x + 281, bbBattels.Max.y));
    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, NickName, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Level, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);
    TextColorButtonV(font, font_size, bbLocR.Min, bbLocR.Max, Location, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLoc);
    TextColorButtonV(font, font_size, bbRankR.Min, bbRankR.Max, Rank, NULL, NULL, ImVec2(0.5f, 0.5f), &bbRank);
    TextColorButtonV(font, font_size, bbPointR.Min, bbPointR.Max, Point, NULL, NULL, ImVec2(0.5f, 0.5f), &bbPoint);
    TextColorButtonV(font, font_size, bbBattelsR.Min, bbBattelsR.Max, Battles, NULL, NULL, ImVec2(0.5f, 0.5f), &bbBattels);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);

    if (IsSelected && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(600, 19));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 1), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::NewClanPlayerList(const char* NickName, const char* Level, const char* Location, const char* Rank, const char* Point, const char* Battles, ImTextureID SelectAble, ImTextureID Online, ImTextureID Offline, const char* PlayerStatus, int* v, int v_button, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = NewClanPlayerListEx(NickName, Level, Location, Rank, Point, Battles, SelectAble, Online, Offline, PlayerStatus, *v == v_button, font, font_size, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::NewClanRankListEx(const char* Rank, const char* Name, const char* Leader, const char* Points, const char* Level, const char* Battles, ImTextureID SelectAble, ImTextureID RankImg, bool IsSelected, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(Rank);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    
    ImRect bb(text_pos, text_pos + ImVec2(600, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    const ImRect BGbb(text_pos, text_pos + ImVec2(15, 14));

    if (RankImg != NULL)
        window->DrawList->AddImage(RankImg, BGbb.Min + ImVec2(5, 3), BGbb.Max + ImVec2(5, 3), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
  
    ImRect bbNick(text_pos, text_pos + ImVec2(130, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x - 19, bbNick.Min.y), ImVec2(bbNick.Max.x - 19, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(140, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 60, bbLev.Min.y), ImVec2(bbLev.Max.x + 60, bbLev.Max.y));
    ImRect bbLoc(text_pos, text_pos + ImVec2(275, 20));
    ImRect bbLocR(ImVec2(bbLoc.Min.x + 92, bbLoc.Min.y), ImVec2(bbLoc.Max.x + 92, bbLoc.Max.y));
    ImRect bbRank(text_pos, text_pos + ImVec2(400, 20));
    ImRect bbRankR(ImVec2(bbRank.Min.x + 137, bbRank.Min.y), ImVec2(bbRank.Max.x + 137, bbRank.Max.y));
    ImRect bbPoint(text_pos, text_pos + ImVec2(440, 20));
    ImRect bbPointR(ImVec2(bbRank.Min.x + 225, bbRank.Min.y), ImVec2(bbRank.Max.x + 225, bbRank.Max.y));
    ImRect bbBattels(text_pos, text_pos + ImVec2(470, 20));
    ImRect bbBattelsR(ImVec2(bbBattels.Min.x + 281, bbBattels.Min.y), ImVec2(bbBattels.Max.x + 281, bbBattels.Max.y));

    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, Rank, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Name, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);
    TextColorButtonV(font, font_size, bbLocR.Min, bbLocR.Max, Leader, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLoc);
    TextColorButtonV(font, font_size, bbRankR.Min, bbRankR.Max, Points, NULL, NULL, ImVec2(0.5f, 0.5f), &bbRank);
    TextColorButtonV(font, font_size, bbPointR.Min, bbPointR.Max, Level, NULL, NULL, ImVec2(0.5f, 0.5f), &bbPoint);
    TextColorButtonV(font, font_size, bbBattelsR.Min, bbBattelsR.Max, Battles, NULL, NULL, ImVec2(0.5f, 0.5f), &bbBattels);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);

    if (IsSelected && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(600, 19));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 1), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::NewClanRankList(const char* Rank, const char* Name, const char* Leader, const char* Points, const char* Level, const char* Battles, ImTextureID SelectAble, ImTextureID RankImg, int* v, int v_button, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = NewClanRankListEx(Rank, Name, Leader, Points, Level, Battles, SelectAble, RankImg, *v == v_button, font, font_size, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::NewClanUnionListEx(const char* NickName, const char* Level, ImTextureID SelectAble, bool IsSelected, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(NickName);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    ImRect bb(text_pos, text_pos + ImVec2(380, 20));

    ItemSize(bb);

    if (!ItemAdd(bb, id))
        return false;

    ImRect bbNick(text_pos, text_pos + ImVec2(120, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x + 2, bbNick.Min.y), ImVec2(bbNick.Max.x + 2, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(140, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 130, bbLev.Min.y), ImVec2(bbLev.Max.x + 130, bbLev.Max.y));

    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, NickName, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Level, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);

    if (IsSelected && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(380, 19));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 1), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::NewClanUnionList(const char* NickName, const char* Level, ImTextureID SelectAble, int* v, int v_button, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = NewClanUnionListEx(NickName, Level, SelectAble, *v == v_button, font, font_size, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::NewClanHistoryListEx(const char* Name, const char* Date, const char* Status, ImTextureID SelectAble, bool IsSelected, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(Name);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    ImRect bb(text_pos, text_pos + ImVec2(600, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    ImRect bbNick(text_pos, text_pos + ImVec2(120, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x - 15, bbNick.Min.y), ImVec2(bbNick.Max.x - 15, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(275, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 128, bbLev.Min.y), ImVec2(bbLev.Max.x + 128, bbLev.Max.y));
    ImRect bbLoc(text_pos, text_pos + ImVec2(420, 20));
    ImRect bbLocR(ImVec2(bbLoc.Min.x + 285, bbLoc.Min.y), ImVec2(bbLoc.Max.x + 285, bbLoc.Max.y));
  
    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, Name, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Date, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);
    TextColorButtonV(font, font_size, bbLocR.Min, bbLocR.Max, Status, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLoc);
    
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_MouseButtonLeft);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }

    if (pressed)
        MarkItemEdited(id);

    if (IsSelected && SelectAble != NULL)
    {
        const ImRect SELbb(text_pos, text_pos + ImVec2(600, 19));
        window->DrawList->AddImage(SelectAble, SELbb.Min + ImVec2(0, 1), SELbb.Max + ImVec2(0, 1), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::NewClanHistoryList(const char* Name, const char* Date, const char* Status, ImTextureID SelectAble, int* v, int v_button, ImFont* font, float font_size, const ImVec2& align, LegionTextFlags flags)
{
    const bool pressed = NewClanHistoryListEx(Name, Date, Status, SelectAble, *v == v_button, font, font_size, align, flags);
    if (pressed)
        *v = v_button;
    return pressed;
}

int Legion::NewClanRequestJoinListEx(const char* NickName, const char* Level, ImTextureID AccseptButton, ImTextureID RejectButton, ImFont* font, float font_size, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(NickName);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    ImRect bb(text_pos, text_pos + ImVec2(600, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    ImRect bbNick(text_pos, text_pos + ImVec2(77, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x + 5, bbNick.Min.y), ImVec2(bbNick.Max.x + 5, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(404, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 71, bbLev.Min.y), ImVec2(bbLev.Max.x + 71, bbLev.Max.y));
 
    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, NickName, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Level, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);

    SameLineEx(440, -3);
    if (ImageButtonEx("Accsept", "ACCSEPT", true, AccseptButton, AccseptButton, AccseptButton, ImVec2(50, 20), font, 12))
        return 1;
    SameLine(495);
    if (ImageButtonEx("Accsept", "REJECT", true, RejectButton, RejectButton, RejectButton, ImVec2(50, 20), font, 12))
        return 2;

    return 0;
}

bool Legion::NewClanBannedPlayerListEx(const char* NickName, const char* Level, ImTextureID UnBanButton, ImFont* font, float font_size, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;

    LegionID id = window->GetID(NickName);
    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    ImRect bb(text_pos, text_pos + ImVec2(600, 20));

    ItemSize(bb, 0.0f);

    if (!ItemAdd(bb, id))
        return false;

    ImRect bbNick(text_pos, text_pos + ImVec2(77, 20));
    ImRect bbNickR(ImVec2(bbNick.Min.x + 5, bbNick.Min.y), ImVec2(bbNick.Max.x + 5, bbNick.Max.y));
    ImRect bbLev(text_pos, text_pos + ImVec2(404, 20));
    ImRect bbLevR(ImVec2(bbLev.Min.x + 71, bbLev.Min.y), ImVec2(bbLev.Max.x + 71, bbLev.Max.y));

    TextColorButtonV(font, font_size, bbNickR.Min, bbNickR.Max, NickName, NULL, NULL, ImVec2(0.5f, 0.5f), &bbNick);
    TextColorButtonV(font, font_size, bbLevR.Min, bbLevR.Max, Level, NULL, NULL, ImVec2(0.5f, 0.5f), &bbLev);

    SameLineEx(495, -3);
    bool pressed = ImageButtonEx("Unban", "UNBAN", true, UnBanButton, UnBanButton, UnBanButton, ImVec2(50, 20), font, 12);

    return pressed;
}

void Legion::TextExBtn(ImFont* font, float size, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    RenderTextClippedExN(font, size, pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect);
}

void Legion::TextExC(ImFont* font, float font_size, const char* text, const char* text_end, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    /* if (text_end - text > 2000 && !wrap_enabled)
    {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // - From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
        // - We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
        // - We use memchr(), pay attention that well optimized versions of those str/mem functions are much faster than a casually written loop.
        const char* line = text;
        const float line_height = GetTextLineHeight();
        ImVec2 text_size(0, 0);

        // Lines to skip (can't skip when logging text)
        ImVec2 pos = text_pos;
        if (!g.LogEnabled)
        {
            int lines_skippable = (int)((window->ClipRect.Min.y - text_pos.y) / line_height);
            if (lines_skippable > 0)
            {
                int lines_skipped = 0;
                while (line < text_end && lines_skipped < lines_skippable)
                {
                    const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    if ((flags & LegionTextFlags_NoWidthForLargeClippedText) == 0)
                        text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
        }

        // Lines to render
        if (line < text_end)
        {
            ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
            while (line < text_end)
            {
                if (IsClippedEx(line_rect, 0, false))
                    break;

                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                RenderText(pos, line, line_end, false);
                line = line_end + 1;
                line_rect.Min.y += line_height;
                line_rect.Max.y += line_height;
                pos.y += line_height;
            }

            // Count remaining lines
            int lines_skipped = 0;
            while (line < text_end)
            {
                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                if ((flags & LegionTextFlags_NoWidthForLargeClippedText) == 0)
                    text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                line = line_end + 1;
                lines_skipped++;
            }
            pos.y += lines_skipped * line_height;
        }
        text_size.y = (pos - text_pos).y;

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        ItemAdd(bb, 0);
    }
    else
    {*/
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        const ImVec2 text_size = CalcTextSizeV(text_begin, font, font_size, text_end, false, wrap_width);

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        if (!ItemAdd(bb, 0))
            return;

        if (text == "" || strlen(text) == 0 || text == NULL)
            return;
        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrappedEx(font, font_size, bb.Min, text_begin, text_end, wrap_width);
 //   }
}

void Legion::TextExCV(ImVec2 pos, ImFont* font, float font_size, const char* text, const char* text_end, LegionTextFlags flags)
{
    RenderTextEx(font, font_size, ImVec2(pos.x, pos.y), text);
}

void Legion::TextEx(const char* text, const char* text_end, LegionTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    IM_ASSERT(text != NULL);
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    if (text_end - text > 2000 && !wrap_enabled)
    {
        // Long text!
        // Perform manual coarse clipping to optimize for long multi-line text
        // - From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
        // - We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
        // - We use memchr(), pay attention that well optimized versions of those str/mem functions are much faster than a casually written loop.
        const char* line = text;
        const float line_height = GetTextLineHeight();
        ImVec2 text_size(0,0);

        // Lines to skip (can't skip when logging text)
        ImVec2 pos = text_pos;
        if (!g.LogEnabled)
        {
            int lines_skippable = (int)((window->ClipRect.Min.y - text_pos.y) / line_height);
            if (lines_skippable > 0)
            {
                int lines_skipped = 0;
                while (line < text_end && lines_skipped < lines_skippable)
                {
                    const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    if ((flags & LegionTextFlags_NoWidthForLargeClippedText) == 0)
                        text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
        }

        // Lines to render
        if (line < text_end)
        {
            ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
            while (line < text_end)
            {
                if (IsClippedEx(line_rect, 0, false))
                    break;

                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                RenderText(pos, line, line_end, false);
                line = line_end + 1;
                line_rect.Min.y += line_height;
                line_rect.Max.y += line_height;
                pos.y += line_height;
            }

            // Count remaining lines
            int lines_skipped = 0;
            while (line < text_end)
            {
                const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                if (!line_end)
                    line_end = text_end;
                if ((flags & LegionTextFlags_NoWidthForLargeClippedText) == 0)
                    text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                line = line_end + 1;
                lines_skipped++;
            }
            pos.y += lines_skipped * line_height;
        }
        text_size.y = (pos - text_pos).y;

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        ItemAdd(bb, 0);
    }
    else
    {
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        if (!ItemAdd(bb, 0))
            return;

        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
    }
}

void Legion::TextUnformatted(const char* text, const char* text_end)
{
    TextEx(text, text_end, LegionTextFlags_NoWidthForLargeClippedText);
}

void Legion::Text(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextV(fmt, args);
    va_end(args);
}

void Legion::TextV(const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    TextEx(g.TempBuffer, text_end, LegionTextFlags_NoWidthForLargeClippedText);
}


void Legion::TextExV(ImFont* font, float font_size, const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    TextExC(font, font_size, g.TempBuffer, text_end, LegionTextFlags_NoWidthForLargeClippedText);
}

void Legion::TextColored(const ImVec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextColoredV(col, fmt, args);
    va_end(args);
}

void Legion::TextColoredV(const ImVec4& col, const char* fmt, va_list args)
{
    PushStyleColor(LegionCol_Text, col);
    TextV(fmt, args);
    PopStyleColor();
}

void Legion::TextColored(ImFont* font, float font_size, const ImVec4& col, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextColoredV(font, font_size, col, fmt, args);
    va_end(args);
}

void Legion::TextColoredV(ImFont* font, float font_size, const ImVec4& col, const char* fmt, va_list args)
{
    PushStyleColor(LegionCol_Text, col);
    TextExV(font, font_size, fmt, args);
    PopStyleColor();
}

void Legion::TextDisabled(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextDisabledV(fmt, args);
    va_end(args);
}

void Legion::TextDisabledV(const char* fmt, va_list args)
{
    PushStyleColor(LegionCol_Text, GLegion->Style.Colors[LegionCol_TextDisabled]);
    TextV(fmt, args);
    PopStyleColor();
}

void Legion::TextWrapped(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextWrappedV(fmt, args);
    va_end(args);
}

void Legion::TextWrappedV(const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    bool need_backup = (window->DC.TextWrapPos < 0.0f);  // Keep existing wrap position if one is already set
    if (need_backup)
        PushTextWrapPos(0.0f);
    TextV(fmt, args);
    if (need_backup)
        PopTextWrapPos();
}

void Legion::TextWrappedEx(ImFont* font, float size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    TextWrappedExV(font, size, fmt, args);
    va_end(args);
}

void Legion::TextWrappedExV(ImFont* font, float size, const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    bool need_backup = (window->DC.TextWrapPos < 0.0f);  // Keep existing wrap position if one is already set
    if (need_backup)
        PushTextWrapPos(0.0f);
    TextExV(font, size, fmt, args);
    if (need_backup)
        PopTextWrapPos();
}

void Legion::LabelText(const char* label, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    LabelTextV(label, fmt, args);
    va_end(args);
}

// Add a label+text combo aligned to other label+value widgets
void Legion::LabelTextV(const char* label, const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect value_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2));
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + (label_size.x > 0.0f ? style.ItemInnerSpacing.x : 0.0f), style.FramePadding.y*2) + label_size);
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0))
        return;

    // Render
    const char* value_text_begin = &g.TempBuffer[0];
    const char* value_text_end = value_text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    RenderTextClipped(value_bb.Min, value_bb.Max, value_text_begin, value_text_end, NULL, ImVec2(0.0f,0.5f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(value_bb.Max.x + style.ItemInnerSpacing.x, value_bb.Min.y + style.FramePadding.y), label);
}

void Legion::BulletText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    BulletTextV(fmt, args);
    va_end(args);
}

// Text with a little bullet aligned to the typical tree node.
void Legion::BulletTextV(const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const char* text_begin = g.TempBuffer;
    const char* text_end = text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    const ImVec2 label_size = CalcTextSize(text_begin, text_end, false);
    const ImVec2 total_size = ImVec2(g.FontSize + (label_size.x > 0.0f ? (label_size.x + style.FramePadding.x * 2) : 0.0f), label_size.y);  // Empty text doesn't add padding
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(total_size, 0.0f);
    const ImRect bb(pos, pos + total_size);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    ImU32 text_col = GetColorU32(LegionCol_Text);
    RenderBullet(window->DrawList, bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, g.FontSize*0.5f), text_col);
    RenderText(bb.Min + ImVec2(g.FontSize + style.FramePadding.x * 2, 0.0f), text_begin, text_end, false);
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: Main
//-------------------------------------------------------------------------
// - ButtonBehavior() [Internal]
// - Button()
// - SmallButton()
// - InvisibleButton()
// - ArrowButton()
// - CloseButton() [Internal]
// - CollapseButton() [Internal]
// - ScrollbarEx() [Internal]
// - Scrollbar() [Internal]
// - Image()
// - ImageButton()
// - Checkbox()
// - CheckboxFlags()
// - RadioButton()
// - ProgressBar()
// - Bullet()
//-------------------------------------------------------------------------

// The ButtonBehavior() function is key to many interactions and used by many/most widgets.
// Because we handle so many cases (keyboard/gamepad navigation, drag and drop) and many specific behavior (via LegionButtonFlags_),
// this code is a little complex.
// By far the most common path is interacting with the Mouse using the default LegionButtonFlags_PressedOnClickRelease button behavior.
// See the series of events below and the corresponding state reported by dear Legion:
//------------------------------------------------------------------------------------------------------------------------------------------------
// with PressedOnClickRelease:             return-value  IsItemHovered()  IsItemActive()  IsItemActivated()  IsItemDeactivated()  IsItemClicked()
//   Frame N+0 (mouse is outside bb)        -             -                -               -                  -                    -
//   Frame N+1 (mouse moves inside bb)      -             true             -               -                  -                    -
//   Frame N+2 (mouse button is down)       -             true             true            true               -                    true
//   Frame N+3 (mouse button is down)       -             true             true            -                  -                    -
//   Frame N+4 (mouse moves outside bb)     -             -                true            -                  -                    -
//   Frame N+5 (mouse moves inside bb)      -             true             true            -                  -                    -
//   Frame N+6 (mouse button is released)   true          true             -               -                  true                 -
//   Frame N+7 (mouse button is released)   -             true             -               -                  -                    -
//   Frame N+8 (mouse moves outside bb)     -             -                -               -                  -                    -
//------------------------------------------------------------------------------------------------------------------------------------------------
// with PressedOnClick:                    return-value  IsItemHovered()  IsItemActive()  IsItemActivated()  IsItemDeactivated()  IsItemClicked()
//   Frame N+2 (mouse button is down)       true          true             true            true               -                    true
//   Frame N+3 (mouse button is down)       -             true             true            -                  -                    -
//   Frame N+6 (mouse button is released)   -             true             -               -                  true                 -
//   Frame N+7 (mouse button is released)   -             true             -               -                  -                    -
//------------------------------------------------------------------------------------------------------------------------------------------------
// with PressedOnRelease:                  return-value  IsItemHovered()  IsItemActive()  IsItemActivated()  IsItemDeactivated()  IsItemClicked()
//   Frame N+2 (mouse button is down)       -             true             -               -                  -                    true
//   Frame N+3 (mouse button is down)       -             true             -               -                  -                    -
//   Frame N+6 (mouse button is released)   true          true             -               -                  -                    -
//   Frame N+7 (mouse button is released)   -             true             -               -                  -                    -
//------------------------------------------------------------------------------------------------------------------------------------------------
// with PressedOnDoubleClick:              return-value  IsItemHovered()  IsItemActive()  IsItemActivated()  IsItemDeactivated()  IsItemClicked()
//   Frame N+0 (mouse button is down)       -             true             -               -                  -                    true
//   Frame N+1 (mouse button is down)       -             true             -               -                  -                    -
//   Frame N+2 (mouse button is released)   -             true             -               -                  -                    -
//   Frame N+3 (mouse button is released)   -             true             -               -                  -                    -
//   Frame N+4 (mouse button is down)       true          true             true            true               -                    true
//   Frame N+5 (mouse button is down)       -             true             true            -                  -                    -
//   Frame N+6 (mouse button is released)   -             true             -               -                  true                 -
//   Frame N+7 (mouse button is released)   -             true             -               -                  -                    -
//------------------------------------------------------------------------------------------------------------------------------------------------
// Note that some combinations are supported,
// - PressedOnDragDropHold can generally be associated with any flag.
// - PressedOnDoubleClick can be associated by PressedOnClickRelease/PressedOnRelease, in which case the second release event won't be reported.
//------------------------------------------------------------------------------------------------------------------------------------------------
// The behavior of the return-value changes when LegionButtonFlags_Repeat is set:
//                                         Repeat+                  Repeat+           Repeat+             Repeat+
//                                         PressedOnClickRelease    PressedOnClick    PressedOnRelease    PressedOnDoubleClick
//-------------------------------------------------------------------------------------------------------------------------------------------------
//   Frame N+0 (mouse button is down)       -                        true              -                   true
//   ...                                    -                        -                 -                   -
//   Frame N + RepeatDelay                  true                     true              -                   true
//   ...                                    -                        -                 -                   -
//   Frame N + RepeatDelay + RepeatRate*N   true                     true              -                   true
//-------------------------------------------------------------------------------------------------------------------------------------------------

bool Legion::ButtonBehavior(const ImRect& bb, LegionID id, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0])         { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1])   { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2])  { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0])        { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1])  { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonBehaviorEx(const ImRect& bb, const ImRect& image, LegionID id, const char* label, const char* tip, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, ImFont* font, float font_size, const ImVec2& align, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    ImTextureID textrue = normal;
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        textrue = hover;

        if (tip != "" && tip != NULL)
            SetTooltipEx(font, font_size, tiptexture, tip);

        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
                textrue = press;
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, image.Min, image.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (label != NULL && label != "")
    {
        const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

        if (islabel)
            TextColorButton(font, font_size, image.Min + g.Style.FramePadding, image.Max - g.Style.FramePadding, label, NULL, NULL, align, &image);
    }
        // RenderTextClippedExN(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, align, &bb);

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonBehaviorEx(const ImRect& bb, const ImRect& image, LegionID id, const char* label, const char* tip, bool islabel, ImColor normal, ImColor hover, ImColor press, ImTextureID tiptexture, ImFont* font, float font_size, const ImVec2& align, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    ImColor textrue = normal;
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        textrue = hover;

        if (tip != "" && tip != NULL)
            SetTooltipEx(font, font_size, tiptexture, tip);

        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
                textrue = press;
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

   // if (textrue != NULL)
      //  window->DrawList->AddImage(textrue, image.Min, image.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    RenderFrame(image.Min, image.Max, textrue, true, 3);
    if (label != NULL && label != "")
    {
        const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

        if (islabel)
            TextColorButton(font, font_size, image.Min + g.Style.FramePadding, image.Max - g.Style.FramePadding, label, NULL, NULL, align, &image);
    }
    // RenderTextClippedExN(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, align, &bb);

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonBehaviorExM(const ImRect& bb, const ImRect& image, LegionID id, const char* label, const char* description, const char* hoverlable, const char* counter, const char* tip, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, ImFont* font, float font_size, float font_size2, const ImVec2& align, const ImVec2& align2, const ImVec2& align3, const ImVec2& align4, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    ImTextureID textrue = normal;
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        textrue = hover;

        if (tip != "" && tip != NULL)
            SetTooltipEx(font, font_size, tiptexture, tip);

        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
                textrue = press;
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

     if (textrue != NULL)
         window->DrawList->AddImage(textrue, image.Min, image.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (label != NULL && label != "")
    {
   
        if (islabel)
            TextColorButton(font, font_size, image.Min + g.Style.FramePadding, image.Max - g.Style.FramePadding, label, NULL, NULL, align, &image);

    }

    if (description != NULL && description != "")
        TextColorButton(font, font_size2, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, description, NULL, NULL, align2, &bb);
    if (hoverlable != NULL && hoverlable != "")
    {
        if (hovered)
            TextColorButton(font, font_size2, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, hoverlable, NULL, NULL, align3, &bb);
    }
    if (counter != NULL && counter != "")
        TextColorButton(font, font_size2, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, counter, NULL, NULL, align4, &bb);
    // RenderTextClippedExN(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, align, &bb);

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonBehaviorExC(const ImRect& bb, const ImRect& image, LegionID id, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, const char* tip, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, ImFont* font, float font_size, const ImVec2& align, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    ImTextureID textrue = normal;
    char totaltext[200];

    strcpy(totaltext, normalcolor);
    strcat(totaltext, label);

    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        textrue = hover;
        if (hovercolor != NULL)
            strcpy(totaltext, hovercolor);
        else
            strcpy(totaltext, normalcolor);

        strcat(totaltext, label);

    //    if (tip != "" && tip != NULL)
        //    SetTooltipEx(font, font_size, NULL, tip);

        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
                textrue = press;
                if (pressedcolor != NULL)
                    strcpy(totaltext, pressedcolor);
                else
                    strcpy(totaltext, normalcolor);

                strcat(totaltext, label);
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, image.Min, image.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);
    if (label != NULL && label != "")
    {
        if (islabel)
            TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, totaltext, NULL, NULL, align, &bb);
    }
    // RenderTextClippedExN(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, align, &bb);

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonBehaviorExC(const ImRect& bb, const ImRect& image, LegionID id, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, const char* tip, bool islabel, ImColor normal, ImColor hover, ImColor press, ImFont* font, float font_size, const ImVec2& align, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    ImColor textrue = normal;
    char totaltext[200];

    strcpy(totaltext, normalcolor);
    strcat(totaltext, label);

    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        textrue = hover;
        if (hovercolor != NULL)
            strcpy(totaltext, hovercolor);
        else
            strcpy(totaltext, normalcolor);

        strcat(totaltext, label);

        if (tip != NULL)
            SetTooltipEx(font, font_size, NULL, tip);

        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
                textrue = press;
                if (pressedcolor != NULL)
                    strcpy(totaltext, pressedcolor);
                else
                    strcpy(totaltext, normalcolor);

                strcat(totaltext, label);
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    RenderFrame(image.Min, image.Max, textrue, true, 3);

    if (label != NULL && label != "")
    {
        if (islabel)
            TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, totaltext, NULL, NULL, align, &bb);
    }
    // RenderTextClippedExN(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, &label_size, align, &bb);

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ImageBehaviorExTip(ImFont* font, float fsize, const char* label, const char* tip, const ImVec2& size, bool istip, ImVec2 pos, ImTextureID normal, ImTextureID imtextrue_ID, const ImRect& bb, LegionID id, bool* out_hovered, bool* out_held, LegionButtonFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    if (flags & LegionButtonFlags_Disabled)
    {
        if (out_hovered) *out_hovered = false;
        if (out_held) *out_held = false;
        if (g.ActiveId == id) ClearActiveID();
        return false;
    }

    // Default only reacts to left mouse button
    if ((flags & LegionButtonFlags_MouseButtonMask_) == 0)
        flags |= LegionButtonFlags_MouseButtonDefault_;

    // Default behavior requires click + release inside bounding box
    if ((flags & LegionButtonFlags_PressedOnMask_) == 0)
        flags |= LegionButtonFlags_PressedOnDefault_;

    LegionWindow* backup_hovered_window = g.HoveredWindow;
    const bool flatten_hovered_children = (flags & LegionButtonFlags_FlattenChildren) && g.HoveredRootWindow == window;
    if (flatten_hovered_children)
        g.HoveredWindow = window;

#ifdef Legion_ENABLE_TEST_ENGINE
    if (id != 0 && window->DC.LastItemId != id)
        LegionTestEngineHook_ItemAdd(&g, bb, id);
#endif

    bool pressed = false;
    bool hovered = ItemHoverable(bb, id);

    // Drag source doesn't report as hovered
    if (hovered && g.DragDropActive && g.DragDropPayload.SourceId == id && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoDisableHover))
        hovered = false;

    // Special mode for Drag and Drop where holding button pressed for a long time while dragging another item triggers the button
    if (g.DragDropActive && (flags & LegionButtonFlags_PressedOnDragDropHold) && !(g.DragDropSourceFlags & LegionDragDropFlags_SourceNoHoldToOpenOthers))
        if (IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            hovered = true;
            SetHoveredID(id);
            if (CalcTypematicRepeatAmount(g.HoveredIdTimer + 0.0001f - g.IO.DeltaTime, g.HoveredIdTimer + 0.0001f, 0.70f, 0.00f))
            {
                pressed = true;
                FocusWindow(window);
            }
        }

    if (flatten_hovered_children)
        g.HoveredWindow = backup_hovered_window;

    // AllowOverlap mode (rarely used) requires previous frame HoveredId to be null or to match. This allows using patterns where a later submitted widget overlaps a previous one.
    if (hovered && (flags & LegionButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
        hovered = false;

    // Mouse handling
    if (hovered)
    {
        if (!(flags & LegionButtonFlags_NoKeyModifiers) || (!g.IO.KeyCtrl && !g.IO.KeyShift && !g.IO.KeyAlt))
        {
            // Poll buttons
            int mouse_button_clicked = -1;
            int mouse_button_released = -1;
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseClicked[0]) { mouse_button_clicked = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseClicked[1]) { mouse_button_clicked = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseClicked[2]) { mouse_button_clicked = 2; }
            if ((flags & LegionButtonFlags_MouseButtonLeft) && g.IO.MouseReleased[0]) { mouse_button_released = 0; }
            else if ((flags & LegionButtonFlags_MouseButtonRight) && g.IO.MouseReleased[1]) { mouse_button_released = 1; }
            else if ((flags & LegionButtonFlags_MouseButtonMiddle) && g.IO.MouseReleased[2]) { mouse_button_released = 2; }

            if (mouse_button_clicked != -1 && g.ActiveId != id)
            {
                if (flags & (LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnClickReleaseAnywhere))
                {
                    SetActiveID(id, window);
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    if (!(flags & LegionButtonFlags_NoNavFocus))
                        SetFocusID(id, window);
                    FocusWindow(window);
                }
                if ((flags & LegionButtonFlags_PressedOnClick) || ((flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[mouse_button_clicked]))
                {
                    pressed = true;
                    if (flags & LegionButtonFlags_NoHoldingActiveId)
                        ClearActiveID();
                    else
                        SetActiveID(id, window); // Hold on ID
                    g.ActiveIdMouseButton = mouse_button_clicked;
                    FocusWindow(window);
                }
            }
            if ((flags & LegionButtonFlags_PressedOnRelease) && mouse_button_released != -1)
            {
                // Repeat mode trumps on release behavior
                if (!((flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button_released] >= g.IO.KeyRepeatDelay))
                    pressed = true;
                ClearActiveID();
            }

            // 'Repeat' mode acts when held regardless of _PressedOn flags (see table above).
            // Relies on repeat logic of IsMouseClicked() but we may as well do it ourselves if we end up exposing finer RepeatDelay/RepeatRate settings.
            if (g.ActiveId == id && (flags & LegionButtonFlags_Repeat))
                if (g.IO.MouseDownDuration[g.ActiveIdMouseButton] > 0.0f && IsMouseClicked(g.ActiveIdMouseButton, true))
                    pressed = true;
        }

        if (pressed)
            g.NavDisableHighlight = true;
    }

    // Gamepad/Keyboard navigation
    // We report navigated item as hovered but we don't set g.HoveredId to not interfere with mouse.
    if (g.NavId == id && !g.NavDisableHighlight && g.NavDisableMouseHover && (g.ActiveId == 0 || g.ActiveId == id || g.ActiveId == window->MoveId))
        if (!(flags & LegionButtonFlags_NoHoveredOnFocus))
            hovered = true;
    if (g.NavActivateDownId == id)
    {
        bool nav_activated_by_code = (g.NavActivateId == id);
        bool nav_activated_by_inputs = IsNavInputTest(LegionNavInput_Activate, (flags & LegionButtonFlags_Repeat) ? LegionInputReadMode_Repeat : LegionInputReadMode_Pressed);
        if (nav_activated_by_code || nav_activated_by_inputs)
            pressed = true;
        if (nav_activated_by_code || nav_activated_by_inputs || g.ActiveId == id)
        {
            // Set active id so it can be queried by user via IsItemActive(), equivalent of holding the mouse button.
            g.NavActivateId = id; // This is so SetActiveId assign a Nav source
            SetActiveID(id, window);
            if ((nav_activated_by_code || nav_activated_by_inputs) && !(flags & LegionButtonFlags_NoNavFocus))
                SetFocusID(id, window);
        }
    }

    bool held = false;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (g.ActiveIdIsJustActivated)
                g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;

            const int mouse_button = g.ActiveIdMouseButton;
            IM_ASSERT(mouse_button >= 0 && mouse_button < LegionMouseButton_COUNT);
            if (g.IO.MouseDown[mouse_button])
            {
                held = true;
            }
            else
            {
                bool release_in = hovered && (flags & LegionButtonFlags_PressedOnClickRelease) != 0;
                bool release_anywhere = (flags & LegionButtonFlags_PressedOnClickReleaseAnywhere) != 0;
                if ((release_in || release_anywhere) && !g.DragDropActive)
                {
                    bool is_double_click_release = (flags & LegionButtonFlags_PressedOnDoubleClick) && g.IO.MouseDownWasDoubleClick[mouse_button];
                    bool is_repeating_already = (flags & LegionButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[mouse_button] >= g.IO.KeyRepeatDelay; // Repeat mode trumps <on release>
                    if (!is_double_click_release && !is_repeating_already)
                        pressed = true;
                }
                ClearActiveID();
            }
            if (!(flags & LegionButtonFlags_NoNavFocus))
                g.NavDisableHighlight = true;
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            if (g.NavActivateDownId != id)
                ClearActiveID();
        }
        if (pressed)
            g.ActiveIdHasBeenPressedBefore = true;
    }

    if (label != NULL || label != "")
    {
        const ImVec2 label_size = CalcTextSizeV(label, font, fsize, NULL, true);
        ImVec2 size2 = CalcItemSize(size, label_size.x, label_size.y);
        const ImRect bb2(pos, pos + size2);
        RenderTextClippedExN(font, fsize, bb.Min, bb.Max, label, NULL, NULL, g.Style.ButtonTextAlign, &bb2);
    }

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (istip && hovered && tip != "")
    {
        SetTooltipEx(font, fsize, imtextrue_ID, tip);
    }

    if (out_hovered) *out_hovered = hovered;
    if (out_held) *out_held = held;

    return pressed;
}

bool Legion::ButtonEx(const char* label, const ImVec2& size_arg, LegionButtonFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & LegionButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (window->DC.ItemFlags & LegionItemFlags_ButtonRepeat)
        flags |= LegionButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? LegionCol_ButtonActive : hovered ? LegionCol_ButtonHovered : LegionCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & LegionButtonFlags_DontClosePopups) && (window->Flags & LegionWindowFlags_Popup))
    //    CloseCurrentPopup();

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
    return pressed;
}

bool Legion::Button(const char* label, const ImVec2& size_arg)
{
    return ButtonEx(label, size_arg, 0);
}

// Small buttons fits within text without additional vertical spacing.
bool Legion::SmallButton(const char* label)
{
    LegionContext& g = *GLegion;
    float backup_padding_y = g.Style.FramePadding.y;
    g.Style.FramePadding.y = 0.0f;
    bool pressed = ButtonEx(label, ImVec2(0, 0), LegionButtonFlags_AlignTextBaseLine);
    g.Style.FramePadding.y = backup_padding_y;
    return pressed;
}

// Tip: use Legion::PushID()/PopID() to push indices or pointers in the ID stack.
// Then you can keep 'str_id' empty or the same for all your buttons (instead of creating a string based on a non-string id)
bool Legion::InvisibleButton(const char* str_id, const ImVec2& size_arg)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    // Cannot use zero-size for InvisibleButton(). Unlike Button() there is not way to fallback using the label size.
    IM_ASSERT(size_arg.x != 0.0f && size_arg.y != 0.0f);

    const LegionID id = window->GetID(str_id);
    ImVec2 size = CalcItemSize(size_arg, 0.0f, 0.0f);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(size);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    return pressed;
}

bool Legion::ArrowButtonEx(const char* str_id, LegionDir dir, ImVec2 size, LegionButtonFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionID id = window->GetID(str_id);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const float default_size = GetFrameHeight();
    ItemSize(size, (size.y >= default_size) ? g.Style.FramePadding.y : -1.0f);
    if (!ItemAdd(bb, id))
        return false;

    if (window->DC.ItemFlags & LegionItemFlags_ButtonRepeat)
        flags |= LegionButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    const ImU32 bg_col = GetColorU32((held && hovered) ? LegionCol_ButtonActive : hovered ? LegionCol_ButtonHovered : LegionCol_Button);
    const ImU32 text_col = GetColorU32(LegionCol_Text);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, bg_col, true, g.Style.FrameRounding);
    RenderArrow(window->DrawList, bb.Min + ImVec2(ImMax(0.0f, (size.x - g.FontSize) * 0.5f), ImMax(0.0f, (size.y - g.FontSize) * 0.5f)), text_col, dir);

    return pressed;
}

bool Legion::ArrowButton(const char* str_id, LegionDir dir)
{
    float sz = GetFrameHeight();
    return ArrowButtonEx(str_id, dir, ImVec2(sz, sz), LegionButtonFlags_None);
}

// Button to close a window
bool Legion::CloseButton(LegionID id, const ImVec2& pos)//, float size)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    // We intentionally allow interaction when clipped so that a mechanical Alt,Right,Validate sequence close a window.
    // (this isn't the regular behavior of buttons, but it doesn't affect the user much because navigation tends to keep items visible).
    const ImRect bb(pos, pos + ImVec2(g.FontSize, g.FontSize) + g.Style.FramePadding * 2.0f);
    bool is_clipped = !ItemAdd(bb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (is_clipped)
        return pressed;

    // Render
    ImU32 col = GetColorU32(held ? LegionCol_ButtonActive : LegionCol_ButtonHovered);
    ImVec2 center = bb.GetCenter();
    if (hovered)
        window->DrawList->AddCircleFilled(center, ImMax(2.0f, g.FontSize * 0.5f + 1.0f), col, 12);

    float cross_extent = g.FontSize * 0.5f * 0.7071f - 1.0f;
    ImU32 cross_col = GetColorU32(LegionCol_Text);
    center -= ImVec2(0.5f, 0.5f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent,+cross_extent), center + ImVec2(-cross_extent,-cross_extent), cross_col, 1.0f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent,-cross_extent), center + ImVec2(-cross_extent,+cross_extent), cross_col, 1.0f);

    return pressed;
}

bool Legion::CollapseButton(LegionID id, const ImVec2& pos)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    ImRect bb(pos, pos + ImVec2(g.FontSize, g.FontSize) + g.Style.FramePadding * 2.0f);
    ItemAdd(bb, id);
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_None);

    // Render
    ImU32 bg_col = GetColorU32((held && hovered) ? LegionCol_ButtonActive : hovered ? LegionCol_ButtonHovered : LegionCol_Button);
    ImU32 text_col = GetColorU32(LegionCol_Text);
    ImVec2 center = bb.GetCenter();
    if (hovered || held)
        window->DrawList->AddCircleFilled(center/*+ ImVec2(0.0f, -0.5f)*/, g.FontSize * 0.5f + 1.0f, bg_col, 12);
    RenderArrow(window->DrawList, bb.Min + g.Style.FramePadding, text_col, window->Collapsed ? LegionDir_Right : LegionDir_Down, 1.0f);

    // Switch to moving the window after mouse is moved beyond the initial drag threshold
    if (IsItemActive() && IsMouseDragging(0))
        StartMouseMovingWindow(window);

    return pressed;
}

LegionID Legion::GetWindowScrollbarID(LegionWindow* window, LegionAxis axis)
{
    return window->GetIDNoKeepAlive(axis == LegionAxis_X ? "#SCROLLX" : "#SCROLLY");
}

// Vertical/Horizontal scrollbar
// The entire piece of code below is rather confusing because:
// - We handle absolute seeking (when first clicking outside the grab) and relative manipulation (afterward or when clicking inside the grab)
// - We store values as normalized ratio and in a form that allows the window content to change while we are holding on a scrollbar
// - We handle both horizontal and vertical scrollbars, which makes the terminology not ideal.
// Still, the code should probably be made simpler..
bool Legion::ScrollbarEx(const ImRect& bb_frame, LegionID id, LegionAxis axis, float* p_scroll_v, float size_avail_v, float size_contents_v, ImDrawCornerFlags rounding_corners)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const float bb_frame_width = bb_frame.GetWidth();
    const float bb_frame_height = bb_frame.GetHeight();
    if (bb_frame_width <= 0.0f || bb_frame_height <= 0.0f)
        return false;

    // When we are too small, start hiding and disabling the grab (this reduce visual noise on very small window and facilitate using the resize grab)
    float alpha = 1.0f;
    if ((axis == LegionAxis_Y) && bb_frame_height < g.FontSize + g.Style.FramePadding.y * 2.0f)
        alpha = ImSaturate((bb_frame_height - g.FontSize) / (g.Style.FramePadding.y * 2.0f));
    if (alpha <= 0.0f)
        return false;

    const LegionStyle& style = g.Style;
    const bool allow_interaction = (alpha >= 1.0f);
    const bool horizontal = (axis == LegionAxis_X);

    ImRect bb = bb_frame;
    bb.Expand(ImVec2(-ImClamp(IM_FLOOR((bb_frame_width - 2.0f) * 0.5f), 0.0f, 3.0f), -ImClamp(IM_FLOOR((bb_frame_height - 2.0f) * 0.5f), 0.0f, 3.0f)));

    // V denote the main, longer axis of the scrollbar (= height for a vertical scrollbar)
    const float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();

    // Calculate the height of our grabbable box. It generally represent the amount visible (vs the total scrollable amount)
    // But we maintain a minimum size in pixel to allow for the user to still aim inside.
    IM_ASSERT(ImMax(size_contents_v, size_avail_v) > 0.0f); // Adding this assert to check if the ImMax(XXX,1.0f) is still needed. PLEASE CONTACT ME if this triggers.
    const float win_size_v = ImMax(ImMax(size_contents_v, size_avail_v), 1.0f);
    const float grab_h_pixels = ImClamp(scrollbar_size_v * (size_avail_v / win_size_v), style.GrabMinSize, scrollbar_size_v);
    const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

    // Handle input right away. None of the code of Begin() is relying on scrolling position before calling Scrollbar().
    bool held = false;
    bool hovered = false;
    ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_NoNavFocus);

    float scroll_max = ImMax(1.0f, size_contents_v - size_avail_v);
    float scroll_ratio = ImSaturate(*p_scroll_v / scroll_max);
    float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
    if (held && allow_interaction && grab_h_norm < 1.0f)
    {
        float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
        float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;

        // Click position in scrollbar normalized space (0.0f->1.0f)
        const float clicked_v_norm = ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
        SetHoveredID(id);

        bool seek_absolute = false;
        if (g.ActiveIdIsJustActivated)
        {
            // On initial click calculate the distance between mouse and the center of the grab
            seek_absolute = (clicked_v_norm < grab_v_norm || clicked_v_norm > grab_v_norm + grab_h_norm);
            if (seek_absolute)
                g.ScrollbarClickDeltaToGrabCenter = 0.0f;
            else
                g.ScrollbarClickDeltaToGrabCenter = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
        }

        // Apply scroll
        // It is ok to modify Scroll here because we are being called in Begin() after the calculation of ContentSize and before setting up our starting position
        const float scroll_v_norm = ImSaturate((clicked_v_norm - g.ScrollbarClickDeltaToGrabCenter - grab_h_norm * 0.5f) / (1.0f - grab_h_norm));
        *p_scroll_v = IM_ROUND(scroll_v_norm * scroll_max);//(win_size_contents_v - win_size_v));

        // Update values for rendering
        scroll_ratio = ImSaturate(*p_scroll_v / scroll_max);
        grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

        // Update distance to grab now that we have seeked and saturated
        if (seek_absolute)
            g.ScrollbarClickDeltaToGrabCenter = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
    }

    // Render
    window->DrawList->AddRectFilled(bb_frame.Min, bb_frame.Max, GetColorU32(LegionCol_ScrollbarBg), window->WindowRounding, rounding_corners);
    const ImU32 grab_col = GetColorU32(held ? LegionCol_ScrollbarGrabActive : hovered ? LegionCol_ScrollbarGrabHovered : LegionCol_ScrollbarGrab, alpha);
    ImRect grab_rect;
    if (horizontal)
        grab_rect = ImRect(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y, ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, bb.Max.y);
    else
        grab_rect = ImRect(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm), bb.Max.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels);
    window->DrawList->AddRectFilled(grab_rect.Min, grab_rect.Max, grab_col, style.ScrollbarRounding);

    return held;
}

void Legion::Scrollbar(LegionAxis axis)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    const LegionID id = GetWindowScrollbarID(window, axis);
    KeepAliveID(id);

    // Calculate scrollbar bounding box
    const ImRect outer_rect = window->Rect();
    const ImRect inner_rect = window->InnerRect;
    const float border_size = window->WindowBorderSize;
    const float scrollbar_size = window->ScrollbarSizes[axis ^ 1];
    IM_ASSERT(scrollbar_size > 0.0f);
    const float other_scrollbar_size = window->ScrollbarSizes[axis];
    ImDrawCornerFlags rounding_corners = (other_scrollbar_size <= 0.0f) ? ImDrawCornerFlags_BotRight : 0;
    ImRect bb;
    if (axis == LegionAxis_X)
    {
        bb.Min = ImVec2(inner_rect.Min.x, ImMax(outer_rect.Min.y, outer_rect.Max.y - border_size - scrollbar_size));
        bb.Max = ImVec2(inner_rect.Max.x, outer_rect.Max.y);
        rounding_corners |= ImDrawCornerFlags_BotLeft;
    }
    else
    {
        bb.Min = ImVec2(ImMax(outer_rect.Min.x, outer_rect.Max.x - border_size - scrollbar_size), inner_rect.Min.y);
        bb.Max = ImVec2(outer_rect.Max.x, window->InnerRect.Max.y);
        rounding_corners |= ((window->Flags & LegionWindowFlags_NoTitleBar) && !(window->Flags & LegionWindowFlags_MenuBar)) ? ImDrawCornerFlags_TopRight : 0;
    }
    ScrollbarEx(bb, id, axis, &window->Scroll[axis], inner_rect.Max[axis] - inner_rect.Min[axis], window->ContentSize[axis] + window->WindowPadding[axis] * 2.0f, rounding_corners);
}

bool Legion::ScrollbarExN(ImTextureID scrollbg, ImTextureID scrollbar, const ImRect& bb_frame, LegionID id, LegionAxis axis, float* p_scroll_v, float size_avail_v, float size_contents_v, ImDrawCornerFlags rounding_corners)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const float bb_frame_width = bb_frame.GetWidth();
    const float bb_frame_height = bb_frame.GetHeight();
    if (bb_frame_width <= 0.0f || bb_frame_height <= 0.0f)
        return false;

    // When we are too small, start hiding and disabling the grab (this reduce visual noise on very small window and facilitate using the resize grab)
    float alpha = 1.0f;
    if ((axis == LegionAxis_Y) && bb_frame_height < g.FontSize + g.Style.FramePadding.y * 2.0f)
        alpha = ImSaturate((bb_frame_height - g.FontSize) / (g.Style.FramePadding.y * 2.0f));
    if (alpha <= 0.0f)
        return false;

    const LegionStyle& style = g.Style;
    const bool allow_interaction = (alpha >= 1.0f);
    const bool horizontal = (axis == LegionAxis_X);

    ImRect bb = bb_frame;
    bb.Expand(ImVec2(-ImClamp(IM_FLOOR((bb_frame_width - 2.0f) * 0.5f), 0.0f, 3.0f), -ImClamp(IM_FLOOR((bb_frame_height - 2.0f) * 0.5f), 0.0f, 3.0f)));

    // V denote the main, longer axis of the scrollbar (= height for a vertical scrollbar)
    const float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();

    // Calculate the height of our grabbable box. It generally represent the amount visible (vs the total scrollable amount)
    // But we maintain a minimum size in pixel to allow for the user to still aim inside.
    IM_ASSERT(ImMax(size_contents_v, size_avail_v) > 0.0f); // Adding this assert to check if the ImMax(XXX,1.0f) is still needed. PLEASE CONTACT ME if this triggers.
    const float win_size_v = ImMax(ImMax(size_contents_v, size_avail_v), 1.0f);
    const float grab_h_pixels = ImClamp(scrollbar_size_v * (size_avail_v / win_size_v), style.GrabMinSize, scrollbar_size_v);
    const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

    // Handle input right away. None of the code of Begin() is relying on scrolling position before calling Scrollbar().
    bool held = false;
    bool hovered = false;
    ButtonBehavior(bb, id, &hovered, &held, LegionButtonFlags_NoNavFocus);

    float scroll_max = ImMax(1.0f, size_contents_v - size_avail_v);
    float scroll_ratio = ImSaturate(*p_scroll_v / scroll_max);
    float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
    if (held && allow_interaction && grab_h_norm < 1.0f)
    {
        float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
        float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;

        // Click position in scrollbar normalized space (0.0f->1.0f)
        const float clicked_v_norm = ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
        SetHoveredID(id);

        bool seek_absolute = false;
        if (g.ActiveIdIsJustActivated)
        {
            // On initial click calculate the distance between mouse and the center of the grab
            seek_absolute = (clicked_v_norm < grab_v_norm || clicked_v_norm > grab_v_norm + grab_h_norm);
            if (seek_absolute)
                g.ScrollbarClickDeltaToGrabCenter = 0.0f;
            else
                g.ScrollbarClickDeltaToGrabCenter = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
        }

        // Apply scroll
        // It is ok to modify Scroll here because we are being called in Begin() after the calculation of ContentSize and before setting up our starting position
        const float scroll_v_norm = ImSaturate((clicked_v_norm - g.ScrollbarClickDeltaToGrabCenter - grab_h_norm * 0.5f) / (1.0f - grab_h_norm));
        *p_scroll_v = IM_ROUND(scroll_v_norm * scroll_max);//(win_size_contents_v - win_size_v));

        // Update values for rendering
        scroll_ratio = ImSaturate(*p_scroll_v / scroll_max);
        grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

        // Update distance to grab now that we have seeked and saturated
        if (seek_absolute)
            g.ScrollbarClickDeltaToGrabCenter = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
    }

    // Render
    if (scrollbg != NULL)
    window->DrawList->AddImage(scrollbg, bb_frame.Min, bb_frame.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    const ImU32 grab_col = GetColorU32(held ? LegionCol_ScrollbarGrabActive : hovered ? LegionCol_ScrollbarGrabHovered : LegionCol_ScrollbarGrab, alpha);
    ImRect grab_rect;
    if (horizontal)
        grab_rect = ImRect(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y, ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, bb.Max.y);
    else
        grab_rect = ImRect(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm), bb.Max.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels);
    if (scrollbar != NULL)
    window->DrawList->AddImage(scrollbar, grab_rect.Min, grab_rect.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    return held;
}

void Legion::ScrollbarN(ImTextureID scrollbg, ImTextureID scrollbar, LegionAxis axis)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    const LegionID id = GetWindowScrollbarID(window, axis);
    KeepAliveID(id);

    // Calculate scrollbar bounding box
    const ImRect outer_rect = window->Rect();
    const ImRect inner_rect = window->InnerRect;
    const float border_size = window->WindowBorderSize;
    const float scrollbar_size = window->ScrollbarSizes[axis ^ 1];
    IM_ASSERT(scrollbar_size > 0.0f);
    const float other_scrollbar_size = window->ScrollbarSizes[axis];
    ImDrawCornerFlags rounding_corners = (other_scrollbar_size <= 0.0f) ? ImDrawCornerFlags_BotRight : 0;
    ImRect bb;
    if (axis == LegionAxis_X)
    {
        bb.Min = ImVec2(inner_rect.Min.x, ImMax(outer_rect.Min.y, outer_rect.Max.y - border_size - scrollbar_size));
        bb.Max = ImVec2(inner_rect.Max.x, outer_rect.Max.y);
        rounding_corners |= ImDrawCornerFlags_BotLeft;
    }
    else
    {
        bb.Min = ImVec2(ImMax(outer_rect.Min.x, outer_rect.Max.x - border_size - scrollbar_size), inner_rect.Min.y);
        bb.Max = ImVec2(outer_rect.Max.x, window->InnerRect.Max.y);
        rounding_corners |= ((window->Flags & LegionWindowFlags_NoTitleBar) && !(window->Flags & LegionWindowFlags_MenuBar)) ? ImDrawCornerFlags_TopRight : 0;
    }
    ScrollbarExN(scrollbg, scrollbar, bb, id, axis, &window->Scroll[axis], inner_rect.Max[axis] - inner_rect.Min[axis], window->ContentSize[axis] + window->WindowPadding[axis] * 2.0f, rounding_corners);
}

void Legion::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    if (border_col.w > 0.0f)
        bb.Max += ImVec2(2, 2);
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    if (border_col.w > 0.0f)
    {
        window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
        window->DrawList->AddImage(user_texture_id, bb.Min + ImVec2(1, 1), bb.Max - ImVec2(1, 1), uv0, uv1, GetColorU32(tint_col));
    }
    else
    {
        window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, uv0, uv1, GetColorU32(tint_col));
    }
}

bool Legion::ImageExTip(const char* cid, ImFont* font, float font_size, const char* tip, ImTextureID normal, ImTextureID user_textrue_id, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

    ImVec2 pos = window->DC.CursorPos;

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0);

    if (hovered && (tip != "" && tip != NULL && tip != "" && tip != " " && strlen(tip) > 2))
    {
        SetTooltipEx(font, font_size, user_textrue_id, tip);
    }
    return pressed;
}


bool Legion::ImageExTipV(const char* cid, ImFont* font, float font_size, const char* tip, ImTextureID normal, ImTextureID image, ImTextureID user_textrue_id, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x * 0.85f, size.y * 0.85f));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (image != NULL)
        window->DrawList->AddImage(image, bb2.Min + ImVec2(11, 11), bb2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0);

    if (hovered && tip != "" && tip != NULL)
    {
        SetTooltipEx(font, font_size, user_textrue_id, tip);
    }
    return pressed;
}

bool Legion::ImageEx(const char* cid, ImFont* font, float font_size, const char* label, const char* tip, bool istip, ImTextureID normal, ImTextureID user_textrue_id, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

    ImVec2 pos = window->DC.CursorPos;

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImageBehaviorExTip(font, font_size, label, tip, size, istip, pos, normal, user_textrue_id, bb, id, &hovered, &held, 0);
    return pressed;
}

void Legion::ImageV(const char* cid, ImTextureID textrue, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

    ImVec2 pos = window->DC.CursorPos;

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
}

void Legion::ImageExV(const char* cid, ImTextureID textrue, ImTextureID img, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect Imgbb(window->DC.CursorPos, window->DC.CursorPos + size);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (img != NULL)
        window->DrawList->AddImage(img, Imgbb.Min + ImVec2(size.x / 6.5f, size.y / 6.5f), Imgbb.Max, ImVec2(0, 0), ImVec2(1.2f, 1.2f), GetColorU32(ImVec4(1, 1, 1, 1)));
}


void Legion::ImageExShop(const char* cid, ImTextureID textrue, ImTextureID img, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect Imgbb(window->DC.CursorPos + ImVec2(15,20), window->DC.CursorPos + ImVec2(95, 95));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (img != NULL)
        window->DrawList->AddImage(img, Imgbb.Min, Imgbb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
}

bool Legion::ImageExS(const char* cid, ImFont* font, float font_size, const char* label, ImTextureID normal, ImTextureID image, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x - 8, size.y - 3));
    const ImRect bb3(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x - 8, size.y * 0.3f));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (image != NULL)
        window->DrawList->AddImage(image, bb2.Min + ImVec2(8, 3), bb2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    bool hovered, held;
    bool pressed = ButtonBehavior(bb2, id, &hovered, &held);

    if (label != "" && label != NULL)
    {
        RenderFrame(bb3.Min + ImVec2(8, bb3.Min.y * 0.3f), bb3.Max, GetColorU32(ImVec4(40 / 255, 40 / 255, 40 / 255, 0.3f)), true, style.FrameRounding);
        TextColorButton(font, font_size, bb.Min, bb.Max, label, NULL, NULL, ImVec2(0.5f, 0.5f));
    }

    return pressed;
}

bool Legion::BattleFieldEx(const char* cid, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID Claimed, bool IsClaimed, bool Selected, bool IsButtonActive, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImVec2 pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    const ImRect bb(pos, pos + size);
    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x * 0.90f, size.y * 0.70f));

    ImRect bbBtn(pos, pos + ImVec2(81, 20));
    ImRect bbBtndR(ImVec2(bbBtn.Min.x + 9, bbBtn.Min.y+76), ImVec2(bbBtn.Max.x + 9, bbBtn.Max.y+76));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (image != NULL)
        window->DrawList->AddImage(image, bb2.Min + ImVec2(9, 8), bb2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    
    bool pressed = false;

    if (Claimed != NULL && IsClaimed)
    {
        bool hovered, held;
        ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIMED Image", "", false, Claimed, Claimed, Claimed, NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);
        return false;
    }


    if (!IsClaimed && IsButtonActive)
    {
       // SameLineEx(5, 72);
      //  const LegionID id = window->GetID("CLAIM");
        //RenderNavHighlight(bbBtn, id2);
        bool hovered, held;
         pressed = ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIM", "", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);

       // pressed = ImageButtonExS("CLAIM", "CLAIM", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), ImVec2(81, 20), font, font_size, bbBtndR);
    }

  //  if (pressed)
    //    MarkItemEdited(id);

    return pressed;
}

bool Legion::BattleField(const char* cid, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID Claimed, bool IsClaimed, int* v, int v_button, bool IsButtonActive, const ImVec2& size)
{
    const bool pressed = BattleFieldEx(cid, font, font_size, normal, image, Claimed, IsClaimed, *v == v_button, IsButtonActive, size);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::BattleFieldTaskEx(const char* cid, const char* label, const char* count, const char* mission, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID pointimage, ImTextureID Claimed, bool IsClaimed, bool Selected, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImVec2 pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    const ImRect bb(pos, pos + size);
    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + ImVec2(63, 63));
    const ImRect bb3(window->DC.CursorPos, window->DC.CursorPos + ImVec2(50, 50));

    ImRect bbCount(pos, pos + ImVec2(81, 20));
    ImRect bbCountR(ImVec2(bbCount.Min.x - 15, bbCount.Min.y + 7), ImVec2(bbCount.Max.x - 15, bbCount.Max.y + 7));

    ImRect bblabel(pos, pos + ImVec2(380, 20));
    ImRect bblabelR(ImVec2(bblabel.Min.x + 70, bblabel.Min.y + 25), ImVec2(bblabel.Max.x + 75, bblabel.Max.y + 20));

    ImRect bbmission(pos, pos + ImVec2(60, 20));
    ImRect bbmissionR(ImVec2(bbmission.Min.x + 440, bbmission.Min.y + 25), ImVec2(bbmission.Max.x + 440, bbmission.Max.y + 25));

    ImRect bbBtn(pos, pos + ImVec2(104, 55));
    ImRect bbBtndR(ImVec2(bbBtn.Min.x + 505, bbBtn.Min.y + 7), ImVec2(bbBtn.Max.x + 505, bbBtn.Max.y + 7));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (image != NULL)
        window->DrawList->AddImage(image, bb2.Min + ImVec2(12, 7), bb2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    window->DrawList->AddImage(pointimage, bb3.Min + ImVec2(25, 18), bb3.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    RenderTextClippedExN(font, font_size, bblabelR.Min, bblabelR.Max, label, NULL, NULL, g.Style.ButtonTextAlign, &bblabelR);
    RenderTextClippedExN(font, font_size, bbCountR.Min, bbCountR.Max, count, NULL, NULL, g.Style.ButtonTextAlign, &bbCountR);
    RenderTextClippedExN(font, font_size, bbmissionR.Min, bbmissionR.Max, mission, NULL, NULL, g.Style.ButtonTextAlign, &bbmissionR);

    bool pressed = false;

    if (Claimed != NULL && IsClaimed)
    {
        bool hovered, held;
        ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIMED Image", "", false, Claimed, Claimed, Claimed, NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);
        pressed = false;
    }

    if (!IsClaimed)
    {
        bool hovered, held;
        pressed = ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIM", "", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);
    }

    /*if (pressed)
        MarkItemEdited(id);*/

    return pressed;
}

bool Legion::BattleFieldTask(const char* cid, const char* label, const char* count, const char* mission, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID pointimage, ImTextureID Claimed, bool IsClaimed, int* v, int v_button, const ImVec2& size)
{
    const bool pressed = BattleFieldTaskEx(cid, label, count, mission, font, font_size, normal, image, pointimage, Claimed, IsClaimed, *v == v_button, size);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::AchivementsEx(const char* cid, ImFont* font, float font_size, ImTextureID background, ImTextureID achivimage, ImTextureID Claimed, float v, bool IsClaimed, bool Selected)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const ImVec2 pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);

    const ImRect bb(pos, pos + ImVec2(126, 152));
    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + ImVec2(115.5, 112));

    ImRect bblabel(pos, pos + ImVec2(126, 20));
    ImRect bblabelR(ImVec2(bblabel.Min.x, bblabel.Min.y + 116), ImVec2(bblabel.Max.x, bblabel.Max.y + 116));

    ImRect bbBtn(pos, pos + ImVec2(120, 4));
    ImRect bbBtndR(ImVec2(bbBtn.Min.x + 3, bbBtn.Min.y + 145), ImVec2(bbBtn.Max.x + 3, bbBtn.Max.y + 145));

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    if (background != NULL)
        window->DrawList->AddImage(background, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (achivimage != NULL)
        window->DrawList->AddImage(achivimage, bb2.Min + ImVec2(10, 10), bb2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    TextColorButton(font, font_size, bblabelR.Min, bblabelR.Max, cid, NULL, NULL, ImVec2(0.5f, 0.5f));
    bool pressed = false;

    if (v < 100)
    {
        ProgressBehaviorEx(font, 12, bbBtndR, id, v, 0, 100, 1, 0, ImColor(249, 249, 249), ImColor(74, 170, 76), false);
    }

    if (Claimed != NULL && IsClaimed)
    {
        bool hovered, held;
        ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIMED Image", "", false, Claimed, Claimed, Claimed, NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);
        pressed = false;
    }

    if (!IsClaimed && v >= 100)
    {

        bool hovered, held;
        pressed = ButtonBehaviorEx(bbBtndR, bbBtndR, id, "CLAIM", "", true, ImColor(8, 137, 196), ImColor(3, 198, 252), ImColor(3, 148, 252), NULL, font, font_size, ImVec2(0.5f, 0.5f), &hovered, &held);
    }

    if (pressed)
        MarkItemEdited(id);

    return pressed;
}

bool Legion::Achivements(const char* cid, ImFont* font, float font_size, ImTextureID background, ImTextureID achivimage, ImTextureID Claimed, float val, bool IsClaimed, int* v, int v_button)
{
    const bool pressed = AchivementsEx(cid, font, font_size, background, achivimage, Claimed, val, IsClaimed, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}

void Legion::ImageText(ImFont* font, float font_size, const char* label, const char* cid, ImTextureID normal, const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cid);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;

    if (normal != NULL)
        window->DrawList->AddImage(normal, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

    ImVec2 size2 = CalcItemSize(size, label_size.x, label_size.y);

    const ImRect bb2(window->DC.CursorPos, window->DC.CursorPos + size2);

    RenderTextClippedExN(font, font_size, bb2.Min, bb2.Max, label, NULL, &label_size, g.Style.ButtonTextAlign, &bb2);
 
}

bool Legion::ShopSearchControl(const char* CheckBoxId, const char* ComboboxId, const char* preview_value, ImTextureID ItemBG, ImTextureID Item, ImTextureID CheckBoxBG, ImTextureID CheckBoxActiveBG, ImTextureID ComboBoxBG, ImTextureID ComboBoxArrow, ImTextureID ComboBoxListBG, ImTextureID ColorLeftArrow, ImTextureID ColorRightArrow, const ImVec2& FrameSize, const ImVec2& ItemBGSize, const ImVec2& CheckBoxSize, const ImVec2& ComboBoxSize, ImFont* font, float font_size, const char* label, bool* CheckBoxPressed, bool* LeftArrowPressed, bool* RightArrowPressed, LegionComboFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID CheckBoxID = window->GetID(CheckBoxId);
    const LegionID ComboboxID = window->GetID(ComboboxId);

    char LeftArrow[200];
    char RightArrow[200];
    strcpy(LeftArrow, CheckBoxId);
    strcat(LeftArrow, ComboboxId);
    strcpy(RightArrow, ComboboxId);
    strcat(RightArrow, CheckBoxId);

    const LegionID LeftArrowID = window->GetID(LeftArrow);
    const LegionID RightArrowID = window->GetID(RightArrow);

    char Combobox2xd[200];
    strcpy(Combobox2xd, ComboboxId);
    strcat(Combobox2xd, ComboboxId);
    const LegionID Combobox2ID = window->GetID(Combobox2xd);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + FrameSize);
    const ImRect Item_bb(window->DC.CursorPos, window->DC.CursorPos + ItemBGSize);
    const ImRect ColorArrows_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(12, 19));
    const ImRect ColorArrows_bb_Left(ImVec2(ColorArrows_bb.Min.x + style.ItemInnerSpacing.x, ColorArrows_bb.Min.y + style.ItemInnerSpacing.y + 40), ImVec2(ColorArrows_bb.Max.x + style.ItemInnerSpacing.x, ColorArrows_bb.Max.y + style.ItemInnerSpacing.y + 40));
    const ImRect ColorArrows_bb_Right(ImVec2(ColorArrows_bb.Min.x + style.ItemInnerSpacing.x + 80, ColorArrows_bb.Min.y + style.ItemInnerSpacing.y + 40), ImVec2(ColorArrows_bb.Max.x + style.ItemInnerSpacing.x + 80, ColorArrows_bb.Max.y + style.ItemInnerSpacing.y + 40));
    const ImRect CheckBox_bb(window->DC.CursorPos, window->DC.CursorPos + CheckBoxSize);
    const ImRect CheckBox_bb_Loc(ImVec2(CheckBox_bb.Min.x + style.ItemInnerSpacing.x, CheckBox_bb.Min.y + style.ItemInnerSpacing.y), ImVec2(CheckBox_bb.Max.x + style.ItemInnerSpacing.x, CheckBox_bb.Max.y + style.ItemInnerSpacing.y));
    const ImRect Combo_bb(window->DC.CursorPos, window->DC.CursorPos + ComboBoxSize);
    const ImRect Combo_bb_frame(ImVec2(Combo_bb.Min.x + style.ItemInnerSpacing.x + 90, Combo_bb.Min.y + style.ItemInnerSpacing.y + 15), ImVec2(Combo_bb.Max.x + style.ItemInnerSpacing.x + 90, Combo_bb.Max.y + style.ItemInnerSpacing.y + 15));
    const ImRect Combo2_bb_frame(ImVec2(Combo_bb.Min.x + style.ItemInnerSpacing.x + 90, Combo_bb.Min.y + style.ItemInnerSpacing.y + 45), ImVec2(Combo_bb.Max.x + style.ItemInnerSpacing.x + 90, Combo_bb.Max.y + style.ItemInnerSpacing.y + 45));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return false;

    if (ItemBG != NULL)
        window->DrawList->AddImage(ItemBG, Item_bb.Min, Item_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (Item != NULL)
        window->DrawList->AddImage(Item, Item_bb.Min + ImVec2(15, 15), Item_bb.Max, ImVec2(0, 0), ImVec2(1.2f, 1.2f), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (g.NavActivateId == CheckBoxID)
    {
  
    bool CheckBox_hovered, CheckBox_held;
    bool CheckBox_pressed;   
  
    CheckBox_pressed = ButtonBehavior(CheckBox_bb, CheckBoxID, &CheckBox_hovered, &CheckBox_held);
    
    if (CheckBox_pressed)
    {
        *CheckBoxPressed = !(*CheckBoxPressed);
        MarkItemEdited(CheckBoxID);
    }

    if (CheckBoxBG != NULL)
        window->DrawList->AddImage(CheckBoxBG, CheckBox_bb_Loc.Min, CheckBox_bb_Loc.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));


        if (CheckBoxActiveBG != NULL)
            window->DrawList->AddImage(CheckBoxActiveBG, CheckBox_bb.Min + ImVec2(4, 4), CheckBox_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    bool LeftArrow_hovered, LeftArrow_held;
    bool LeftArrow_pressed;

    LeftArrow_pressed = ImageBehaviorExTip(font, font_size, "", "", ImVec2(12, 19), false, ImVec2(0, 0), ColorLeftArrow, NULL, ColorArrows_bb_Left, LeftArrowID, &LeftArrow_hovered, &LeftArrow_held, 0);

    *LeftArrowPressed = LeftArrow_pressed;
    
    bool RightArrow_hovered, RightArrow_held;
    bool RightArrow_pressed;

    RightArrow_pressed = ImageBehaviorExTip(font, font_size, "", "", ImVec2(12, 19), false, ImVec2(0, 0), ColorRightArrow, NULL, ColorArrows_bb_Right, RightArrowID, &RightArrow_hovered, &RightArrow_held, 0);
    *RightArrowPressed = RightArrow_pressed;
   
    bool hovered, held;
    bool pressed = ButtonBehavior(Combo_bb_frame, ComboboxID, &hovered, &held);
    bool popup_open = IsPopupOpen(ComboboxID);

    if (ComboBoxBG != NULL)
        window->DrawList->AddImage(ComboBoxBG, Combo_bb_frame.Min, Combo_bb_frame.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (ComboBoxArrow != NULL)
        window->DrawList->AddImage(ComboBoxArrow, Combo_bb_frame.Min + ImVec2(ComboBoxSize.x - 35, 8), Combo_bb_frame.Max - ImVec2(15, 8));

    if (preview_value != NULL)
        TextLabelColorV(ImVec2(Combo_bb_frame.Min.x + style.FramePadding.x + 10, Combo_bb_frame.Max.y - 23), font, font_size, preview_value);

    if ((pressed || g.NavActivateId == ComboboxID) && !popup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = ComboboxID;
        OpenPopupEx(ComboboxID);
        popup_open = true;
    }

    if (popup_open)
    {
        char name[16];
        ImFormatString(name, IM_ARRAYSIZE(name), label, g.BeginPopupStack.Size); // Recycle windows based on depth

        // Peak into expected window size so we can position it
        if (LegionWindow* popup_window = FindWindowByName(name))
            if (popup_window->WasActive)
            {
                ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
                if (flags & LegionComboFlags_PopupAlignLeft)
                    popup_window->AutoPosLastDirection = LegionDir_Left;
                ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(Combo_bb_frame.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, Combo_bb_frame, LegionPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        LegionWindowFlags window_flags = LegionWindowFlags_NoBackground | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

        // Horizontally align ourselves with the framed text
        bool ret = BeginEx(ComboBoxListBG, NULL, NULL, name, ImVec2(Combo_bb_frame.Max.x * 0.36f, 150), ImVec2(0, 0), NULL, window_flags);
        if (!ret)
        {
            EndPopup();
        //    return false;
        }
        EndPopup();
    }
    ////

    bool Comobohovered, Comoboheld;
    bool Comobopressed = ButtonBehavior(Combo2_bb_frame, Combobox2ID, &Comobohovered, &Comoboheld);
    bool Comobopopup_open = IsPopupOpen(Combobox2ID);
    
    if (ComboBoxBG != NULL)
        window->DrawList->AddImage(ComboBoxBG, Combo2_bb_frame.Min, Combo2_bb_frame.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (ComboBoxArrow != NULL)
        window->DrawList->AddImage(ComboBoxArrow, Combo2_bb_frame.Min + ImVec2(ComboBoxSize.x - 35, 8), Combo2_bb_frame.Max - ImVec2(15, 8));

    if (preview_value != NULL)
        TextLabelColorV(ImVec2(Combo2_bb_frame.Min.x + style.FramePadding.x + 10, Combo2_bb_frame.Max.y - 23), font, font_size, preview_value);

    if ((Comobopressed || g.NavActivateId == Combobox2ID) && !Comobopopup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = Combobox2ID;
        OpenPopupEx(Combobox2ID);
        Comobopopup_open = true;
    }

    if (Comobopopup_open)
    {
        char coname[16];
        ImFormatString(coname, IM_ARRAYSIZE(coname), ComboboxId, g.BeginPopupStack.Size); // Recycle windows based on depth

        // Peak into expected window size so we can position it
        if (LegionWindow* popup_window = FindWindowByName(coname))
            if (popup_window->WasActive)
            {
                ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
                if (flags & LegionComboFlags_PopupAlignLeft)
                    popup_window->AutoPosLastDirection = LegionDir_Left;
                ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(Combo2_bb_frame.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, Combo2_bb_frame, LegionPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        LegionWindowFlags window_flags = LegionWindowFlags_NoBackground | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

        // Horizontally align ourselves with the framed text
        bool ret2 = BeginEx(ComboBoxListBG, NULL, NULL, coname, ImVec2(Combo2_bb_frame.Max.x * 0.36f, 150), ImVec2(0, 0), NULL, window_flags);
        if (!ret2)
        {
            EndPopup();
         //   return false;
        }
        EndPopup();
    }
    TextLabelColorV(ImVec2(Item_bb.Max.x + style.ItemInnerSpacing.x, Item_bb.Min.y + style.ItemInnerSpacing.y + 4), font, font_size, label);

    return true;
}


bool Legion::ImageCheckBoxV(const char* cid, const char* coid, const char* coid2, const char* preview_value, ImTextureID textrue, ImTextureID img, ImTextureID checkbackground, ImTextureID checkedbox, ImTextureID combobox, ImTextureID Arrow, ImTextureID ListBackGround, ImTextureID LeftArrow, ImTextureID RightArrow, const ImVec2& size, const ImVec2& Imsize, const ImVec2& cosize, const ImVec2& checksize, ImFont* font, float font_size, const char* label, bool* v, bool isreadonly, LegionComboFlags flags)
{
    LegionContext& g = *GLegion;

    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const LegionStyle& style = g.Style;

    char leftarrow[200];
    char rightarrow[200];
    strcpy(leftarrow, cid);
    strcat(leftarrow, coid);
    strcpy(rightarrow, coid);
    strcat(rightarrow, cid);

    const LegionID checkid = window->GetID(cid);
    const LegionID combid = window->GetID(coid);
    const LegionID combid2 = window->GetID(coid2);
    const LegionID leftarrowid = window->GetID(leftarrow);
    const LegionID rightarrowid = window->GetID(rightarrow);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect Image_bb(window->DC.CursorPos, window->DC.CursorPos + Imsize);
    const ImRect Combo_bb(window->DC.CursorPos, window->DC.CursorPos + cosize);
    const ImRect Combo_bb_fix(ImVec2(Combo_bb.Min.x + style.ItemInnerSpacing.x + 90, Combo_bb.Min.y + style.ItemInnerSpacing.y + 15), ImVec2(Combo_bb.Max.x + style.ItemInnerSpacing.x + 90, Combo_bb.Max.y + style.ItemInnerSpacing.y + 15));
    const ImRect Combo_bb_fix2(ImVec2(Combo_bb.Min.x + style.ItemInnerSpacing.x + 90, Combo_bb.Min.y + style.ItemInnerSpacing.y + 35), ImVec2(Combo_bb.Max.x + style.ItemInnerSpacing.x + 90, Combo_bb.Max.y + style.ItemInnerSpacing.y + 35));
    const ImRect CheckBox_bb(window->DC.CursorPos, window->DC.CursorPos + checksize);
    const ImRect ColorArrows_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(12, 19));
    const ImRect ColorArrows_bb_Left(ImVec2(ColorArrows_bb.Min.x + style.ItemInnerSpacing.x, ColorArrows_bb.Min.y + style.ItemInnerSpacing.y + 40), ImVec2(ColorArrows_bb.Max.x + style.ItemInnerSpacing.x, ColorArrows_bb.Max.y + style.ItemInnerSpacing.y + 40));
    const ImRect ColorArrows_bb_Right(ImVec2(ColorArrows_bb.Min.x + style.ItemInnerSpacing.x + 80, ColorArrows_bb.Min.y + style.ItemInnerSpacing.y + 40), ImVec2(ColorArrows_bb.Max.x + style.ItemInnerSpacing.x + 80, ColorArrows_bb.Max.y + style.ItemInnerSpacing.y + 40));

    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return false;

    if (textrue != NULL)
        window->DrawList->AddImage(textrue, Image_bb.Min, Image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (img != NULL)
        window->DrawList->AddImage(img, Image_bb.Min + ImVec2(15, 15), Image_bb.Max, ImVec2(0, 0), ImVec2(1.2f, 1.2f), GetColorU32(ImVec4(1, 1, 1, 1)));

    bool checkboxhovered, checkboxheld;
    bool checkboxpressed;

    if (isreadonly)
        checkboxpressed = *v;
    else
        checkboxpressed = ButtonBehavior(CheckBox_bb, checkid, &checkboxhovered, &checkboxheld);
    // 

    if (checkboxpressed)
    {
        *v = !(*v);
        MarkItemEdited(checkid);
    }
 
    if (checkbackground != NULL)
        window->DrawList->AddImage(checkbackground, CheckBox_bb.Min + ImVec2(4, 4), CheckBox_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (window->DC.ItemFlags & LegionItemFlags_MixedValue)
    {
        // Undocumented tristate/mixed/indeterminate checkbox (#2644)
    //    ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz - 26 / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz- 26 / 3.6f)));
     //   window->DrawList->AddImage(background, check_bb.Min + pad, check_bb.Max - pad, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }
    else if (*v)
    {
        window->DrawList->AddImage(checkedbox, CheckBox_bb.Min + ImVec2(4, 4), CheckBox_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }
    TextLabelColorV(ImVec2(Image_bb.Max.x + style.ItemInnerSpacing.x, Image_bb.Min.y + style.ItemInnerSpacing.y + 4), font, font_size, label);

   /* bool leftarrowhovered, leftarrowheld;
    bool leftarrowpressed;

    leftarrowpressed = ImageBehaviorExTip(font, font_size, "", "", ImVec2(12, 19), false, ImVec2(0, 0), LeftArrow, NULL, ColorArrows_bb_Left, leftarrowid, &leftarrowhovered, &leftarrowheld, 0);

    if (leftarrowpressed)
    {
        printf("LeftArrow\n");
    }

    bool rightarrowhovered, rightarrowheld;
    bool rightarrowpressed;

    rightarrowpressed = ImageBehaviorExTip(font, font_size, "", "", ImVec2(12, 19), false, ImVec2(0, 0), RightArrow, NULL, ColorArrows_bb_Right, rightarrowid, &rightarrowhovered, &rightarrowheld, 0);

    if (rightarrowpressed)
    {
        printf("RightArrow\n");
    }

    // 
    bool hovered, held;
    bool pressed = ButtonBehavior(Combo_bb_fix, combid, &hovered, &held);
    bool popup_open = IsPopupOpen(combid);

    if (combobox != NULL)
        window->DrawList->AddImage(combobox, Combo_bb_fix.Min, Combo_bb_fix.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (Arrow != NULL)
        window->DrawList->AddImage(Arrow, Combo_bb_fix.Min + ImVec2(cosize.x - 35, 8), Combo_bb_fix.Max - ImVec2(15, 8));

    if (preview_value != NULL)
        TextLabelColorV(ImVec2(Combo_bb_fix.Min.x + style.FramePadding.x + 10, Combo_bb_fix.Max.y - 23), font, font_size, preview_value);

    if ((pressed || g.NavActivateId == combid) && !popup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = combid;
        OpenPopupEx(combid);
        popup_open = true;
    }

    if (!popup_open)
        return false;

    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), label, g.BeginPopupStack.Size); // Recycle windows based on depth

    // Peak into expected window size so we can position it
    if (LegionWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
            if (flags & LegionComboFlags_PopupAlignLeft)
                popup_window->AutoPosLastDirection = LegionDir_Left;
            ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(Combo_bb_fix.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, Combo_bb_fix, LegionPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    LegionWindowFlags window_flags = LegionWindowFlags_NoBackground | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

    // Horizontally align ourselves with the framed text
    bool ret = BeginEx(ListBackGround, NULL, NULL, name, ImVec2(Combo_bb.Max.x - 20, 150), ImVec2(0, 0), NULL, window_flags);
    if (!ret)
    {
        EndPopup();
        return false;
    }
    /*
        bool hovered2, held2;
        bool pressed2 = ButtonBehavior(Combo_bb_fix2, combid2, &hovered2, &held2);
        bool popup_open2 = IsPopupOpen(combid2);

        if (combobox != NULL)
            window->DrawList->AddImage(combobox, Combo_bb_fix2.Min, Combo_bb_fix2.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

        if (Arrow != NULL)
            window->DrawList->AddImage(Arrow, Combo_bb_fix2.Min + ImVec2(cosize.x - 35, 8), Combo_bb_fix2.Max - ImVec2(15, 8));

        if (preview_value != NULL)
            TextLabelColorV(ImVec2(Combo_bb_fix2.Min.x + style.FramePadding.x + 10, Combo_bb_fix2.Max.y - 23), font, font_size, preview_value);

        if ((pressed2 || g.NavActivateId == combid2) && !popup_open2)
        {
            if (window->DC.NavLayerCurrent == 0)
                window->NavLastIds[0] = combid2;
            OpenPopupEx(combid2);
            popup_open2 = true;
        }

        if (!popup_open2)
            return false;

        char name2[16];
        ImFormatString(name2, IM_ARRAYSIZE(name2), label, g.BeginPopupStack.Size); // Recycle windows based on depth

        // Peak into expected window size so we can position it
        if (LegionWindow* popup_window = FindWindowByName(name2))
            if (popup_window->WasActive)
            {
                ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
                if (flags & LegionComboFlags_PopupAlignLeft)
                    popup_window->AutoPosLastDirection = LegionDir_Left;
                ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(Combo_bb_fix2.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, Combo_bb_fix2, LegionPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()

        // Horizontally align ourselves with the framed text
        bool ret2 = BeginEx(ListBackGround, NULL, NULL, name2, ImVec2(Combo_bb.Max.x - 20, 150), ImVec2(0, 0), NULL, window_flags);
        if (!ret2)
        {
            EndPopup();
            return false;
        }*/

    return true;
}

// frame_padding < 0: uses FramePadding from style (default)
// frame_padding = 0: no framing
// frame_padding > 0: set framing size
// The color used are the button colors.
bool Legion::CircleButton(const char* cId, const char* label, bool islabel, const ImVec2& size, ImFont* font, float font_size, const ImVec4& bg_col, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    
    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;
    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, bb, id, label, "", islabel, NULL, NULL, NULL, NULL, font, font_size, align, &hovered, &held);
  
    // Render
    ImVec2 center = bb.GetSize();
    window->DrawList->AddCircleFilled(ImVec2(bb.Min.x, bb.Min.y + ((bb.Max.y - bb.Min.y))), 16, ImColor(34, 123, 51));
   //     window->DrawList->AddCircleFilled(center, 1.0f, GetColorU32(bg_col), 120);

    return pressed;
}

bool Legion::ImageButtonEx(const char* cId, const char* label, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, image_bb, id, label, "", islabel, normal, hover, press, NULL, font, font_size, align, &hovered, &held);

    return pressed;
}

bool Legion::ImageButtonEx(const char* cId, const char* label, bool islabel, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;
    RenderNavHighlight(bb, id);
    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, image_bb, id, label, "", islabel, normal, hover, press, NULL, font, font_size, align, &hovered, &held);

    return pressed;
}

bool Legion::ImageButtonExM(const char* cId, const char* label, const char* description, const char* hoverlable, const char* counter, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, float font_size2, const ImVec2& align, const ImVec2& align2, const ImVec2& align3, const ImVec2& align4, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;
    RenderNavHighlight(bb, id);
    bool hovered, held;
    bool pressed = ButtonBehaviorExM(bb, image_bb, id, label, description, hoverlable, counter, "", islabel, normal, hover, press, NULL, font, font_size, font_size2, align, align2, align3, align4, &hovered, &held);

    return pressed;
}

bool Legion::ImageButtonExV(const char* cId, const char* label, bool islabel, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);
    char totaltext[200] = {};
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, image_bb, id, label, NULL, islabel, normal, hover, press, NULL, font, font_size, align, &hovered, &held);

    if (pressed)
        MarkItemEdited(id);

    return pressed;
}

bool Legion::ImageButtonV(const char* cId, const char* label, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align)
{
    const bool pressed = ImageButtonExV(cId, label, islabel, *v == v_button, normal, hover, press, size, font, font_size, align);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ImageButtonExV(const char* cId, const char* label, bool islabel, bool Selected, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);
    char totaltext[200] = {};
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(image_bb, id, &hovered, &held);

    if (pressed)
        MarkItemEdited(id);

    RenderNavHighlight(bb, id);
    if (hovered)
        RenderFrame(image_bb.Min, image_bb.Max, hover, true, 3);
    else if (pressed)
        RenderFrame(image_bb.Min, image_bb.Max, press, true, 3);
    else
        RenderFrame(image_bb.Min, image_bb.Max, normal, true, 3);
    
        TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, NULL, align, &bb);

        if (Selected)
        {
            RenderFrame(image_bb.Min, image_bb.Max, press, true, 3);
            TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, NULL, align, &bb);
            return false;
        }

    return pressed;
}

bool Legion::ImageButtonV(const char* cId, const char* label, bool islabel, int* v, int v_button, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align)
{
    const bool pressed = ImageButtonExV(cId, label, islabel, *v == v_button, normal, hover, press, size, font, font_size, align);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ImageButtonExVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);
    char totaltext[200];
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorExC(bb, image_bb, id, label, normalcolor, hovercolor, pressedcolor, NULL, islabel, normal, hover, press, font, font_size, align, &hovered, &held);

    if (pressed)
        MarkItemEdited(id);

    if (Selected)
    {
        if (press != NULL)
            window->DrawList->AddImage(press, image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

        if (islabel)
        {
            if (pressedcolor != NULL)
                strcpy(totaltext, pressedcolor);
            else
                strcpy(totaltext, normalcolor);

            strcat(totaltext, label);

            TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, totaltext, NULL, NULL, align, &bb);
        }

        return true;
    }

    return pressed;
}

bool Legion::ImageButtonVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align)
{
    const bool pressed = ImageButtonExVC(cId, label, normalcolor, hovercolor, pressedcolor, islabel, *v == v_button, normal, hover, press, size, font, font_size, align);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ImageButtonExVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, bool Selected, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);
    char totaltext[200];
    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorExC(bb, image_bb, id, label, normalcolor, hovercolor, pressedcolor, NULL, islabel, normal, hover, press, font, font_size, align, &hovered, &held);

    if (pressed)
        MarkItemEdited(id);
    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);
    if (Selected)
    {
        RenderFrame(image_bb.Min, image_bb.Max, press, true, 3);

        if (islabel)
        {
            if (pressedcolor != NULL)
                strcpy(totaltext, pressedcolor);
            else
                strcpy(totaltext, normalcolor);

            strcat(totaltext, label);

            TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, totaltext, NULL, NULL, align, &bb);
        }

        return true;
    }

    return pressed;
}

bool Legion::ImageButtonVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, int* v, int v_button, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align)
{
    const bool pressed = ImageButtonExVC(cId, label, normalcolor, hovercolor, pressedcolor, islabel, *v == v_button, normal, hover, press, size, font, font_size, align);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ImageButtonExT(const char* cId, const char* label, bool islabel, const char* tooltip, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, image_bb, id, label, tooltip, islabel, normal, hover, press, tiptexture, font, font_size, align, &hovered, &held);

    if (hovered)
    {
        window->DrawList->AddImage(normal, image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    if (pressed)
        MarkItemEdited(id);

    if (Selected)
    {
        window->DrawList->AddImage(press, image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::ImageButtonT(const char* cId, const char* label, const char* tooltip, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align)
{
    const bool pressed = ImageButtonExT(cId, label, islabel, tooltip, *v == v_button, normal, hover, press, tiptexture, size, font, font_size, align);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    // Default to using texture ID as ID. User can still push string/integer prefixes.
    // We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
    PushID((void*)(intptr_t)user_texture_id);
    const LegionID id = window->GetID("#image");
    PopID();

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? LegionCol_ButtonActive : hovered ? LegionCol_ButtonHovered : LegionCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
    if (bg_col.w > 0.0f)
        window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));
    window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    return pressed;
}

bool Legion::Checkbox(const char* label, bool* v)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    RenderNavHighlight(total_bb, id);
    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? LegionCol_FrameBgActive : hovered ? LegionCol_FrameBgHovered : LegionCol_FrameBg), true, style.FrameRounding);
    ImU32 check_col = GetColorU32(LegionCol_CheckMark);
    if (window->DC.ItemFlags & LegionItemFlags_MixedValue)
    {
        // Undocumented tristate/mixed/indeterminate checkbox (#2644)
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    }
    else if (*v)
    {
        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }

    if (g.LogEnabled)
        LogRenderedText(&total_bb.Min, *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | LegionItemStatusFlags_Checkable | (*v ? LegionItemStatusFlags_Checked : 0));
    return pressed;
}

bool Legion::CheckboxEx(const char* cid, ImTextureID background, ImTextureID checkedbox, ImFont* font, float font_size, const char* label, bool* v, bool isreadonly)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);
    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz - 46 + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed;

    if (isreadonly)
        pressed = *v;
    else
        pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    if (pressed)
    {
        if (!isreadonly)
            *v = !(*v);
        MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(26, 26));
    window->DrawList->AddImage(background, check_bb.Min, check_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

     if (*v)
    {
   //     const ImRect checkmark_bb(pos, pos + ImVec2(24, 24));
        window->DrawList->AddImage(checkedbox, check_bb.Min, check_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    if (label_size.x > 0.0f)
        TextLabelColorV(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.ItemInnerSpacing.y + 3.8f), font, font_size, label);

    return pressed;
}

bool Legion::CheckboxExS(const char* cid, ImTextureID background, ImTextureID checkedbox, bool* v)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(20, 20));
    if (!ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed;

    pressed = ButtonBehavior(total_bb, id, &hovered, &held);

    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(26, 26));
    window->DrawList->AddImage(background, check_bb.Min, check_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    if (*v)
    {

        window->DrawList->AddImage(checkedbox, check_bb.Min, check_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));
    }

    return pressed;
}

bool Legion::CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
    bool v = ((*flags & flags_value) == flags_value);
    bool pressed = Checkbox(label, &v);
    if (pressed)
    {
        if (v)
            *flags |= flags_value;
        else
            *flags &= ~flags_value;
    }

    return pressed;
}

bool Legion::RadioButton(const char* label, bool active)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    ImVec2 center = check_bb.GetCenter();
    center.x = IM_ROUND(center.x);
    center.y = IM_ROUND(center.y);
    const float radius = (square_sz - 1.0f) * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        MarkItemEdited(id);

    RenderNavHighlight(total_bb, id);
    window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? LegionCol_FrameBgActive : hovered ? LegionCol_FrameBgHovered : LegionCol_FrameBg), 16);
    if (active)
    {
        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        window->DrawList->AddCircleFilled(center, radius - pad, GetColorU32(LegionCol_CheckMark), 16);
    }

    if (style.FrameBorderSize > 0.0f)
    {
        window->DrawList->AddCircle(center + ImVec2(1,1), radius, GetColorU32(LegionCol_BorderShadow), 16, style.FrameBorderSize);
        window->DrawList->AddCircle(center, radius, GetColorU32(LegionCol_Border), 16, style.FrameBorderSize);
    }

    if (g.LogEnabled)
        LogRenderedText(&total_bb.Min, active ? "(x)" : "( )");
    if (label_size.x > 0.0f)
        RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y), label);

    return pressed;
}

// FIXME: This would work nicely if it was a public template, e.g. 'template<T> RadioButton(const char* label, T* v, T v_button)', but I'm not sure how we would expose it..
bool Legion::RadioButton(const char* label, int* v, int v_button)
{
    const bool pressed = RadioButton(label, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool Legion::RadioButtonEx(const char* cid, ImTextureID user_texture_id, ImVec2 size, bool active)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(cid);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    ImVec2 center = check_bb.GetCenter();
    center.x = (float)(int)center.x - 7.5f;
    center.y = (float)(int)center.y - 7.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);
    if (pressed)
        MarkItemEdited(id);

    if (user_texture_id != NULL)
        window->DrawList->AddImage(user_texture_id, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (active)
    {
        window->DrawList->AddRect(center, pos + size, GetColorU32(ImVec4(0.00f, 0.00f, 1.00f, 1.00f)));
    }

    return pressed;
}


bool Legion::RadioButtonEx(const char* cid, ImTextureID texture, ImVec2 size, int* v, int v_button)
{
    const bool pressed = RadioButtonEx(cid, texture, size, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}

// size_arg (for each axis) < 0.0f: align to end, 0.0f: auto, > 0.0f: specified size
void Legion::ProgressBar(float fraction, const ImVec2& size_arg, const char* overlay)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    fraction = ImSaturate(fraction);
    RenderFrame(bb.Min, bb.Max, GetColorU32(LegionCol_FrameBg), false, style.FrameRounding);
    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
    RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(LegionCol_PlotHistogram), 0.0f, fraction, style.FrameRounding);

    // Default displaying the fraction as   string, but user can override it
    char overlay_buf[32];
    if (!overlay)
    {
        ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
        overlay = overlay_buf;
    }

    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
}

void Legion::ProgressBarV(float fraction, ImVec4 color, ImVec4 acolor, const ImVec2& size_arg, const char* overlay)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImRect bb(pos, pos + size_arg);
    ItemSize(size_arg, style.FramePadding.y);
    if (!ItemAdd(bb, 0))
        return;

    // Render
    fraction = ImSaturate(fraction);
    RenderFrame(bb.Min, bb.Max, GetColorU32(LegionCol_FrameBg), false, 2);
    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    const ImVec2 fill_br = ImVec2(ImLerp(bb.Min.x, bb.Max.x, fraction), bb.Max.y);
    RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(LegionCol_PlotHistogram), 0.0f, fraction, 2);

    // Default displaying the fraction as   string, but user can override it
    char overlay_buf[32];
    if (!overlay)
    {
        ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
        overlay = overlay_buf;
    }

    ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f)
        RenderTextClipped(ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y), bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
}

void Legion::LabelEx(const char* cId, const char* label, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align, int frame_padding)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionID id = window->GetID(cId);

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
    const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + style.FramePadding + size);
    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return;
    TextColorButton(font, font_size, bb.Min + g.Style.FramePadding, bb.Max - g.Style.FramePadding, label, NULL, NULL, align, &bb);
}


void Legion::Bullet()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const float line_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + g.Style.FramePadding.y*2), g.FontSize);
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(g.FontSize, line_height));
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
    {
        SameLine(0, style.FramePadding.x*2);
        return;
    }

    // Render and stay on same line
    ImU32 text_col = GetColorU32(LegionCol_Text);
    RenderBullet(window->DrawList, bb.Min + ImVec2(style.FramePadding.x + g.FontSize*0.5f, line_height*0.5f), text_col);
    SameLine(0, style.FramePadding.x * 2.0f);
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: Low-level Layout helpers
//-------------------------------------------------------------------------
// - Spacing()
// - Dummy()
// - NewLine()
// - AlignTextToFramePadding()
// - SeparatorEx() [Internal]
// - Separator()
// - SplitterBehavior() [Internal]
// - ShrinkWidths() [Internal]
//-------------------------------------------------------------------------

void Legion::Spacing()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ItemSize(ImVec2(0,0));
}

void Legion::Dummy(const ImVec2& size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(size);
    ItemAdd(bb, 0);
}

void Legion::NewLine()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionLayoutType backup_layout_type = window->DC.LayoutType;
    window->DC.LayoutType = LegionLayoutType_Vertical;
    if (window->DC.CurrLineSize.y > 0.0f)     // In the event that we are on a line with items that is smaller that FontSize high, we will preserve its height.
        ItemSize(ImVec2(0,0));
    else
        ItemSize(ImVec2(0.0f, g.FontSize));
    window->DC.LayoutType = backup_layout_type;
}

void Legion::AlignTextToFramePadding()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    window->DC.CurrLineSize.y = ImMax(window->DC.CurrLineSize.y, g.FontSize + g.Style.FramePadding.y * 2);
    window->DC.CurrLineTextBaseOffset = ImMax(window->DC.CurrLineTextBaseOffset, g.Style.FramePadding.y);
}

// Horizontal/vertical separating line
void Legion::SeparatorEx(LegionSeparatorFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    IM_ASSERT(ImIsPowerOfTwo(flags & (LegionSeparatorFlags_Horizontal | LegionSeparatorFlags_Vertical)));   // Check that only 1 option is selected

    float thickness_draw = 1.0f;
    float thickness_layout = 0.0f;
    if (flags & LegionSeparatorFlags_Vertical)
    {
        // Vertical separator, for menu bars (use current line height). Not exposed because it is misleading and it doesn't have an effect on regular layout.
        float y1 = window->DC.CursorPos.y;
        float y2 = window->DC.CursorPos.y + window->DC.CurrLineSize.y;
        const ImRect bb(ImVec2(window->DC.CursorPos.x, y1), ImVec2(window->DC.CursorPos.x + thickness_draw, y2));
        ItemSize(ImVec2(thickness_layout, 0.0f));
        if (!ItemAdd(bb, 0))
            return;

        // Draw
        window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Min.x, bb.Max.y), GetColorU32(LegionCol_Separator));
        if (g.LogEnabled)
            LogText(" |");
    }
    else if (flags & LegionSeparatorFlags_Horizontal)
    {
        // Horizontal Separator
        float x1 = window->Pos.x;
        float x2 = window->Pos.x + window->Size.x;
        if (!window->DC.GroupStack.empty())
            x1 += window->DC.Indent.x;

        LegionColumns* columns = (flags & LegionSeparatorFlags_SpanAllColumns) ? window->DC.CurrentColumns : NULL;
        if (columns)
            PushColumnsBackground();

        // We don't provide our width to the layout so that it doesn't get feed back into AutoFit
        const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + thickness_draw));
        ItemSize(ImVec2(0.0f, thickness_layout));
        const bool item_visible = ItemAdd(bb, 0);
        if (item_visible)
        {
            // Draw
            window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(LegionCol_Separator));
            if (g.LogEnabled)
                LogRenderedText(&bb.Min, "--------------------------------");
        }
        if (columns)
        {
            PopColumnsBackground();
            columns->LineMinY = window->DC.CursorPos.y;
        }
    }
}

void Legion::Separator()
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    // Those flags should eventually be overridable by the user
    LegionSeparatorFlags flags = (window->DC.LayoutType == LegionLayoutType_Horizontal) ? LegionSeparatorFlags_Vertical : LegionSeparatorFlags_Horizontal;
    flags |= LegionSeparatorFlags_SpanAllColumns;
    SeparatorEx(flags);
}

// Using 'hover_visibility_delay' allows us to hide the highlight and mouse cursor for a short time, which can be convenient to reduce visual noise.
bool Legion::SplitterBehavior(const ImRect& bb, LegionID id, LegionAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend, float hover_visibility_delay)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    const LegionItemFlags item_flags_backup = window->DC.ItemFlags;
    window->DC.ItemFlags |= LegionItemFlags_NoNav | LegionItemFlags_NoNavDefaultFocus;
    bool item_add = ItemAdd(bb, id);
    window->DC.ItemFlags = item_flags_backup;
    if (!item_add)
        return false;

    bool hovered, held;
    ImRect bb_interact = bb;
    bb_interact.Expand(axis == LegionAxis_Y ? ImVec2(0.0f, hover_extend) : ImVec2(hover_extend, 0.0f));
    ButtonBehavior(bb_interact, id, &hovered, &held, LegionButtonFlags_FlattenChildren | LegionButtonFlags_AllowItemOverlap);
    if (g.ActiveId != id)
        SetItemAllowOverlap();

    if (held || (g.HoveredId == id && g.HoveredIdPreviousFrame == id && g.HoveredIdTimer >= hover_visibility_delay))
        SetMouseCursor(axis == LegionAxis_Y ? LegionMouseCursor_ResizeNS : LegionMouseCursor_ResizeEW);

    ImRect bb_render = bb;
    if (held)
    {
        ImVec2 mouse_delta_2d = g.IO.MousePos - g.ActiveIdClickOffset - bb_interact.Min;
        float mouse_delta = (axis == LegionAxis_Y) ? mouse_delta_2d.y : mouse_delta_2d.x;

        // Minimum pane size
        float size_1_maximum_delta = ImMax(0.0f, *size1 - min_size1);
        float size_2_maximum_delta = ImMax(0.0f, *size2 - min_size2);
        if (mouse_delta < -size_1_maximum_delta)
            mouse_delta = -size_1_maximum_delta;
        if (mouse_delta > size_2_maximum_delta)
            mouse_delta = size_2_maximum_delta;

        // Apply resize
        if (mouse_delta != 0.0f)
        {
            if (mouse_delta < 0.0f)
                IM_ASSERT(*size1 + mouse_delta >= min_size1);
            if (mouse_delta > 0.0f)
                IM_ASSERT(*size2 - mouse_delta >= min_size2);
            *size1 += mouse_delta;
            *size2 -= mouse_delta;
            bb_render.Translate((axis == LegionAxis_X) ? ImVec2(mouse_delta, 0.0f) : ImVec2(0.0f, mouse_delta));
            MarkItemEdited(id);
        }
    }

    // Render
    const ImU32 col = GetColorU32(held ? LegionCol_SeparatorActive : (hovered && g.HoveredIdTimer >= hover_visibility_delay) ? LegionCol_SeparatorHovered : LegionCol_Separator);
    window->DrawList->AddRectFilled(bb_render.Min, bb_render.Max, col, 0.0f);

    return held;
}

static int Legion_CDECL ShrinkWidthItemComparer(const void* lhs, const void* rhs)
{
    const LegionShrinkWidthItem* a = (const LegionShrinkWidthItem*)lhs;
    const LegionShrinkWidthItem* b = (const LegionShrinkWidthItem*)rhs;
    if (int d = (int)(b->Width - a->Width))
        return d;
    return (b->Index - a->Index);
}

// Shrink excess width from a set of item, by removing width from the larger items first.
void Legion::ShrinkWidths(LegionShrinkWidthItem* items, int count, float width_excess)
{
    if (count == 1)
    {
        items[0].Width = ImMax(items[0].Width - width_excess, 1.0f);
        return;
    }
    ImQsort(items, (size_t)count, sizeof(LegionShrinkWidthItem), ShrinkWidthItemComparer);
    int count_same_width = 1;
    while (width_excess > 0.0f && count_same_width < count)
    {
        while (count_same_width < count && items[0].Width <= items[count_same_width].Width)
            count_same_width++;
        float max_width_to_remove_per_item = (count_same_width < count) ? (items[0].Width - items[count_same_width].Width) : (items[0].Width - 1.0f);
        float width_to_remove_per_item = ImMin(width_excess / count_same_width, max_width_to_remove_per_item);
        for (int item_n = 0; item_n < count_same_width; item_n++)
            items[item_n].Width -= width_to_remove_per_item;
        width_excess -= width_to_remove_per_item * count_same_width;
    }

    // Round width and redistribute remainder left-to-right (could make it an option of the function?)
    // Ensure that e.g. the right-most tab of a shrunk tab-bar always reaches exactly at the same distance from the right-most edge of the tab bar separator.
    width_excess = 0.0f;
    for (int n = 0; n < count; n++)
    {
        float width_rounded = ImFloor(items[n].Width);
        width_excess += items[n].Width - width_rounded;
        items[n].Width = width_rounded;
    }
    if (width_excess > 0.0f)
        for (int n = 0; n < count; n++)
            if (items[n].Index < (int)(width_excess + 0.01f))
                items[n].Width += 1.0f;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: ComboBox
//-------------------------------------------------------------------------
// - BeginCombo()
// - EndCombo()
// - Combo()
//-------------------------------------------------------------------------

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    LegionContext& g = *GLegion;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool Legion::BeginCombo(const char* label, const char* preview_value, LegionComboFlags flags)
{
    // Always consume the SetNextWindowSizeConstraint() call in our early return paths
    LegionContext& g = *GLegion;
    bool has_window_size_constraint = (g.NextWindowData.Flags & LegionNextWindowDataFlags_HasSizeConstraint) != 0;
    g.NextWindowData.Flags &= ~LegionNextWindowDataFlags_HasSizeConstraint;

    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT((flags & (LegionComboFlags_NoArrowButton | LegionComboFlags_NoPreview)) != (LegionComboFlags_NoArrowButton | LegionComboFlags_NoPreview)); // Can't use both flags together

    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);

    const float arrow_size = (flags & LegionComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float expected_w = CalcItemWidth();
    const float w = (flags & LegionComboFlags_NoPreview) ? arrow_size : expected_w;
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);
    bool popup_open = IsPopupOpen(id);

    const ImU32 frame_col = GetColorU32(hovered ? LegionCol_FrameBgHovered : LegionCol_FrameBg);
    const float value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);
    RenderNavHighlight(frame_bb, id);
    if (!(flags & LegionComboFlags_NoPreview))
        window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(value_x2, frame_bb.Max.y), frame_col, style.FrameRounding, (flags & LegionComboFlags_NoArrowButton) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Left);
    if (!(flags & LegionComboFlags_NoArrowButton))
    {
        ImU32 bg_col = GetColorU32((popup_open || hovered) ? LegionCol_ButtonHovered : LegionCol_Button);
        ImU32 text_col = GetColorU32(LegionCol_Text);
        window->DrawList->AddRectFilled(ImVec2(value_x2, frame_bb.Min.y), frame_bb.Max, bg_col, style.FrameRounding, (w <= arrow_size) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Right);
        if (value_x2 + arrow_size - style.FramePadding.x <= frame_bb.Max.x)
            RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), text_col, LegionDir_Down, 1.0f);
    }
    RenderFrameBorder(frame_bb.Min, frame_bb.Max, style.FrameRounding);
    if (preview_value != NULL && !(flags & LegionComboFlags_NoPreview))
        RenderTextClipped(frame_bb.Min + style.FramePadding, ImVec2(value_x2, frame_bb.Max.y), preview_value, NULL, NULL, ImVec2(0.0f,0.0f));
    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if ((pressed || g.NavActivateId == id) && !popup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = id;
        OpenPopupEx(id);
        popup_open = true;
    }

    if (!popup_open)
        return false;

    if (has_window_size_constraint)
    {
        g.NextWindowData.Flags |= LegionNextWindowDataFlags_HasSizeConstraint;
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & LegionComboFlags_HeightMask_) == 0)
            flags |= LegionComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & LegionComboFlags_HeightMask_));    // Only one
        int popup_max_height_in_items = -1;
        if (flags & LegionComboFlags_HeightRegular)     popup_max_height_in_items = 8;
        else if (flags & LegionComboFlags_HeightSmall)  popup_max_height_in_items = 4;
        else if (flags & LegionComboFlags_HeightLarge)  popup_max_height_in_items = 20;
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
    }

    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Peak into expected window size so we can position it
    if (LegionWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
            if (flags & LegionComboFlags_PopupAlignLeft)
                popup_window->AutoPosLastDirection = LegionDir_Left;
            ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, LegionPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    LegionWindowFlags window_flags = LegionWindowFlags_AlwaysAutoResize | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

    // Horizontally align ourselves with the framed text
    PushStyleVar(LegionStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
    bool ret = Begin(name, NULL, window_flags);
    PopStyleVar();
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

bool Legion::BeginComboV(ImFont* font, float font_size, const char* label, const char* preview_value, const ImVec2& size_arge, ImTextureID BackGround, ImTextureID ComboBackGround, ImTextureID Arrows, ImTextureID Scroll, ImTextureID ScrollBar, LegionComboFlags flags)
{
    LegionContext& g = *GLegion;

    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size_arge);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, bb, id, "", "", false, BackGround, NULL, NULL, NULL, font, font_size, ImVec2(0.1f, 0.1f), &hovered, &held);
    bool popup_open = IsPopupOpen(id);

    if (BackGround != NULL)
        window->DrawList->AddImage(BackGround, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (BackGround != Arrows)
        window->DrawList->AddImage(Arrows, bb.Min + ImVec2(size_arge.x - 35, 8), bb.Max - ImVec2(15, 8));

    if (preview_value != NULL)
        TextLabelColorV(ImVec2(bb.Min.x + style.FramePadding.x + 10, bb.Max.y - 23), font, font_size, preview_value);

    if ((pressed || g.NavActivateId == id) && !popup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = id;
        OpenPopupEx(id);
        popup_open = true;
    }

    if (!popup_open)
        return false;

    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), label, g.BeginPopupStack.Size); // Recycle windows based on depth

    // Peak into expected window size so we can position it
    if (LegionWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
            if (flags & LegionComboFlags_PopupAlignLeft)
                popup_window->AutoPosLastDirection = LegionDir_Left;
            ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, LegionPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }
       
    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    LegionWindowFlags window_flags =  LegionWindowFlags_NoBackground | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

    // Horizontally align ourselves with the framed text
   // PushStyleVar(LegionStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
    bool ret = BeginEx(ComboBackGround, Scroll, Scroll, name, ImVec2((bb.Min.x / 1.4f) - 10, 150), ImVec2(0, 0), NULL, window_flags);
   // PopStyleVar();
    if (!ret)
    {
        EndPopup();
        return false;
    }
    return true;
}

bool Legion::BeginComboEx(ImFont* font, float font_size, const char* label, const char* preview_value, const ImVec2& size_arge, float listsize, ImTextureID BackGround, ImTextureID ComboBackGround, ImTextureID Arrows, ImTextureID Scroll, ImTextureID ScrollBar, LegionComboFlags flags)
{
    LegionContext& g = *GLegion;

    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size_arge);

    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehaviorEx(bb, bb, id, "", "", false, BackGround, NULL, NULL, NULL, font, font_size, ImVec2(0.1f, 0.1f), &hovered, &held);
    bool popup_open = IsPopupOpen(id);

    if (BackGround != NULL)
        window->DrawList->AddImage(BackGround, bb.Min, bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    if (BackGround != Arrows)
        window->DrawList->AddImage(Arrows, bb.Min + ImVec2(size_arge.x - 35, 8), bb.Max - ImVec2(15, 8));

    if (preview_value != NULL)
        TextLabelColorV(ImVec2(bb.Min.x + style.FramePadding.x + 10, bb.Max.y - 23), font, font_size, preview_value);

    if ((pressed || g.NavActivateId == id) && !popup_open)
    {
        if (window->DC.NavLayerCurrent == 0)
            window->NavLastIds[0] = id;
        OpenPopupEx(id);
        popup_open = true;
    }

    if (!popup_open)
        return false;

    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), label, g.BeginPopupStack.Size); // Recycle windows based on depth

    // Peak into expected window size so we can position it
    if (LegionWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            ImVec2 size_expected = CalcWindowExpectedSize(popup_window);
            if (flags & LegionComboFlags_PopupAlignLeft)
                popup_window->AutoPosLastDirection = LegionDir_Left;
            ImRect r_outer = GetWindowAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, LegionPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    LegionWindowFlags window_flags = LegionWindowFlags_NoBackground | LegionWindowFlags_Popup | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoMove;

    // Horizontally align ourselves with the framed text
   // PushStyleVar(LegionStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
    bool ret = BeginEx(ComboBackGround, Scroll, Scroll, name, ImVec2((bb.Min.x / listsize) - 10, 150), ImVec2(0, 0), NULL, window_flags);
    // PopStyleVar();
    if (!ret)
    {
        EndPopup();
        return false;
    }
    return true;
}

void Legion::EndCombo()
{
    EndPopup();
}

// Getter for the old Combo() API: const char*[]
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}

// Old API, prefer using BeginCombo() nowadays if you can.
bool Legion::Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    LegionContext& g = *GLegion;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & LegionNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0,0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!BeginCombo(label, preview_value, LegionComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID((void*)(intptr_t)i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        if (Selectable(item_text, item_selected))
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();
    return value_changed;
}

// Combo box helper allowing to pass an array of strings.
bool Legion::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool Legion::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

//-------------------------------------------------------------------------
// [SECTION] Data Type and Data Formatting Helpers [Internal]
//-------------------------------------------------------------------------
// - PatchFormatStringFloatToInt()
// - DataTypeGetInfo()
// - DataTypeFormatString()
// - DataTypeApplyOp()
// - DataTypeApplyOpFromText()
// - GetMinimumStepAtDecimalPrecision
// - RoundScalarWithFormat<>()
//-------------------------------------------------------------------------

static const LegionDataTypeInfo GDataTypeInfo[] =
{
    { sizeof(char),             "%d",   "%d"    },  // LegionDataType_S8
    { sizeof(unsigned char),    "%u",   "%u"    },
    { sizeof(short),            "%d",   "%d"    },  // LegionDataType_S16
    { sizeof(unsigned short),   "%u",   "%u"    },
    { sizeof(int),              "%d",   "%d"    },  // LegionDataType_S32
    { sizeof(unsigned int),     "%u",   "%u"    },
#ifdef _MSC_VER
    { sizeof(ImS64),            "%I64d","%I64d" },  // LegionDataType_S64
    { sizeof(ImU64),            "%I64u","%I64u" },
#else
    { sizeof(ImS64),            "%lld", "%lld"  },  // LegionDataType_S64
    { sizeof(ImU64),            "%llu", "%llu"  },
#endif
    { sizeof(float),            "%f",   "%f"    },  // LegionDataType_Float (float are promoted to double in va_arg)
    { sizeof(double),           "%f",   "%lf"   },  // LegionDataType_Double
};
IM_STATIC_ASSERT(IM_ARRAYSIZE(GDataTypeInfo) == LegionDataType_COUNT);

// FIXME-LEGACY: Prior to 1.61 our DragInt() function internally used floats and because of this the compile-time default value for format was "%.0f".
// Even though we changed the compile-time default, we expect users to have carried %f around, which would break the display of DragInt() calls.
// To honor backward compatibility we are rewriting the format string, unless Legion_DISABLE_OBSOLETE_FUNCTIONS is enabled. What could possibly go wrong?!
static const char* PatchFormatStringFloatToInt(const char* fmt)
{
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f')
    {
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
        if (fmt_start == fmt && fmt_end[0] == 0)
            return "%d";
        LegionContext& g = *GLegion;
        ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return g.TempBuffer;
#else
        IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
    }
    return fmt;
}

const LegionDataTypeInfo* Legion::DataTypeGetInfo(LegionDataType data_type)
{
    IM_ASSERT(data_type >= 0 && data_type < LegionDataType_COUNT);
    return &GDataTypeInfo[data_type];
}

int Legion::DataTypeFormatString(char* buf, int buf_size, LegionDataType data_type, const void* p_data, const char* format)
{
    // Signedness doesn't matter when pushing integer arguments
    if (data_type == LegionDataType_S32 || data_type == LegionDataType_U32)
        return ImFormatString(buf, buf_size, format, *(const ImU32*)p_data);
    if (data_type == LegionDataType_S64 || data_type == LegionDataType_U64)
        return ImFormatString(buf, buf_size, format, *(const ImU64*)p_data);
    if (data_type == LegionDataType_Float)
        return ImFormatString(buf, buf_size, format, *(const float*)p_data);
    if (data_type == LegionDataType_Double)
        return ImFormatString(buf, buf_size, format, *(const double*)p_data);
    if (data_type == LegionDataType_S8)
        return ImFormatString(buf, buf_size, format, *(const ImS8*)p_data);
    if (data_type == LegionDataType_U8)
        return ImFormatString(buf, buf_size, format, *(const ImU8*)p_data);
    if (data_type == LegionDataType_S16)
        return ImFormatString(buf, buf_size, format, *(const ImS16*)p_data);
    if (data_type == LegionDataType_U16)
        return ImFormatString(buf, buf_size, format, *(const ImU16*)p_data);
    IM_ASSERT(0);
    return 0;
}

void Legion::DataTypeApplyOp(LegionDataType data_type, int op, void* output, void* arg1, const void* arg2)
{
    IM_ASSERT(op == '+' || op == '-');
    switch (data_type)
    {
        case LegionDataType_S8:
            if (op == '+') { *(ImS8*)output  = ImAddClampOverflow(*(const ImS8*)arg1,  *(const ImS8*)arg2,  IM_S8_MIN,  IM_S8_MAX); }
            if (op == '-') { *(ImS8*)output  = ImSubClampOverflow(*(const ImS8*)arg1,  *(const ImS8*)arg2,  IM_S8_MIN,  IM_S8_MAX); }
            return;
        case LegionDataType_U8:
            if (op == '+') { *(ImU8*)output  = ImAddClampOverflow(*(const ImU8*)arg1,  *(const ImU8*)arg2,  IM_U8_MIN,  IM_U8_MAX); }
            if (op == '-') { *(ImU8*)output  = ImSubClampOverflow(*(const ImU8*)arg1,  *(const ImU8*)arg2,  IM_U8_MIN,  IM_U8_MAX); }
            return;
        case LegionDataType_S16:
            if (op == '+') { *(ImS16*)output = ImAddClampOverflow(*(const ImS16*)arg1, *(const ImS16*)arg2, IM_S16_MIN, IM_S16_MAX); }
            if (op == '-') { *(ImS16*)output = ImSubClampOverflow(*(const ImS16*)arg1, *(const ImS16*)arg2, IM_S16_MIN, IM_S16_MAX); }
            return;
        case LegionDataType_U16:
            if (op == '+') { *(ImU16*)output = ImAddClampOverflow(*(const ImU16*)arg1, *(const ImU16*)arg2, IM_U16_MIN, IM_U16_MAX); }
            if (op == '-') { *(ImU16*)output = ImSubClampOverflow(*(const ImU16*)arg1, *(const ImU16*)arg2, IM_U16_MIN, IM_U16_MAX); }
            return;
        case LegionDataType_S32:
            if (op == '+') { *(ImS32*)output = ImAddClampOverflow(*(const ImS32*)arg1, *(const ImS32*)arg2, IM_S32_MIN, IM_S32_MAX); }
            if (op == '-') { *(ImS32*)output = ImSubClampOverflow(*(const ImS32*)arg1, *(const ImS32*)arg2, IM_S32_MIN, IM_S32_MAX); }
            return;
        case LegionDataType_U32:
            if (op == '+') { *(ImU32*)output = ImAddClampOverflow(*(const ImU32*)arg1, *(const ImU32*)arg2, IM_U32_MIN, IM_U32_MAX); }
            if (op == '-') { *(ImU32*)output = ImSubClampOverflow(*(const ImU32*)arg1, *(const ImU32*)arg2, IM_U32_MIN, IM_U32_MAX); }
            return;
        case LegionDataType_S64:
            if (op == '+') { *(ImS64*)output = ImAddClampOverflow(*(const ImS64*)arg1, *(const ImS64*)arg2, IM_S64_MIN, IM_S64_MAX); }
            if (op == '-') { *(ImS64*)output = ImSubClampOverflow(*(const ImS64*)arg1, *(const ImS64*)arg2, IM_S64_MIN, IM_S64_MAX); }
            return;
        case LegionDataType_U64:
            if (op == '+') { *(ImU64*)output = ImAddClampOverflow(*(const ImU64*)arg1, *(const ImU64*)arg2, IM_U64_MIN, IM_U64_MAX); }
            if (op == '-') { *(ImU64*)output = ImSubClampOverflow(*(const ImU64*)arg1, *(const ImU64*)arg2, IM_U64_MIN, IM_U64_MAX); }
            return;
        case LegionDataType_Float:
            if (op == '+') { *(float*)output = *(const float*)arg1 + *(const float*)arg2; }
            if (op == '-') { *(float*)output = *(const float*)arg1 - *(const float*)arg2; }
            return;
        case LegionDataType_Double:
            if (op == '+') { *(double*)output = *(const double*)arg1 + *(const double*)arg2; }
            if (op == '-') { *(double*)output = *(const double*)arg1 - *(const double*)arg2; }
            return;
        case LegionDataType_COUNT: break;
    }
    IM_ASSERT(0);
}

// User can input math operators (e.g. +100) to edit a numerical values.
// NB: This is _not_ a full expression evaluator. We should probably add one and replace this dumb mess..
bool Legion::DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, LegionDataType data_type, void* p_data, const char* format)
{
    while (ImCharIsBlankA(*buf))
        buf++;

    // We don't support '-' op because it would conflict with inputing negative value.
    // Instead you can use +-100 to subtract from an existing value
    char op = buf[0];
    if (op == '+' || op == '*' || op == '/')
    {
        buf++;
        while (ImCharIsBlankA(*buf))
            buf++;
    }
    else
    {
        op = 0;
    }
    if (!buf[0])
        return false;

    // Copy the value in an opaque buffer so we can compare at the end of the function if it changed at all.
    IM_ASSERT(data_type < LegionDataType_COUNT);
    int data_backup[2];
    const LegionDataTypeInfo* type_info = Legion::DataTypeGetInfo(data_type);
    IM_ASSERT(type_info->Size <= sizeof(data_backup));
    memcpy(data_backup, p_data, type_info->Size);

    if (format == NULL)
        format = type_info->ScanFmt;

    // FIXME-LEGACY: The aim is to remove those operators and write a proper expression evaluator at some point..
    int arg1i = 0;
    if (data_type == LegionDataType_S32)
    {
        int* v = (int*)p_data;
        int arg0i = *v;
        float arg1f = 0.0f;
        if (op && sscanf(initial_value_buf, format, &arg0i) < 1)
            return false;
        // Store operand in a float so we can use fractional value for multipliers (*1.1), but constant always parsed as integer so we can fit big integers (e.g. 2000000003) past float precision
        if (op == '+')      { if (sscanf(buf, "%d", &arg1i)) *v = (int)(arg0i + arg1i); }                   // Add (use "+-" to subtract)
        else if (op == '*') { if (sscanf(buf, "%f", &arg1f)) *v = (int)(arg0i * arg1f); }                   // Multiply
        else if (op == '/') { if (sscanf(buf, "%f", &arg1f) && arg1f != 0.0f) *v = (int)(arg0i / arg1f); }  // Divide
        else                { if (sscanf(buf, format, &arg1i) == 1) *v = arg1i; }                           // Assign constant
    }
    else if (data_type == LegionDataType_Float)
    {
        // For floats we have to ignore format with precision (e.g. "%.2f") because sscanf doesn't take them in
        format = "%f";
        float* v = (float*)p_data;
        float arg0f = *v, arg1f = 0.0f;
        if (op && sscanf(initial_value_buf, format, &arg0f) < 1)
            return false;
        if (sscanf(buf, format, &arg1f) < 1)
            return false;
        if (op == '+')      { *v = arg0f + arg1f; }                    // Add (use "+-" to subtract)
        else if (op == '*') { *v = arg0f * arg1f; }                    // Multiply
        else if (op == '/') { if (arg1f != 0.0f) *v = arg0f / arg1f; } // Divide
        else                { *v = arg1f; }                            // Assign constant
    }
    else if (data_type == LegionDataType_Double)
    {
        format = "%lf"; // scanf differentiate float/double unlike printf which forces everything to double because of ellipsis
        double* v = (double*)p_data;
        double arg0f = *v, arg1f = 0.0;
        if (op && sscanf(initial_value_buf, format, &arg0f) < 1)
            return false;
        if (sscanf(buf, format, &arg1f) < 1)
            return false;
        if (op == '+')      { *v = arg0f + arg1f; }                    // Add (use "+-" to subtract)
        else if (op == '*') { *v = arg0f * arg1f; }                    // Multiply
        else if (op == '/') { if (arg1f != 0.0f) *v = arg0f / arg1f; } // Divide
        else                { *v = arg1f; }                            // Assign constant
    }
    else if (data_type == LegionDataType_U32 || data_type == LegionDataType_S64 || data_type == LegionDataType_U64)
    {
        // All other types assign constant
        // We don't bother handling support for legacy operators since they are a little too crappy. Instead we will later implement a proper expression evaluator in the future.
        sscanf(buf, format, p_data);
    }
    else
    {
        // Small types need a 32-bit buffer to receive the result from scanf()
        int v32;
        sscanf(buf, format, &v32);
        if (data_type == LegionDataType_S8)
            *(ImS8*)p_data = (ImS8)ImClamp(v32, (int)IM_S8_MIN, (int)IM_S8_MAX);
        else if (data_type == LegionDataType_U8)
            *(ImU8*)p_data = (ImU8)ImClamp(v32, (int)IM_U8_MIN, (int)IM_U8_MAX);
        else if (data_type == LegionDataType_S16)
            *(ImS16*)p_data = (ImS16)ImClamp(v32, (int)IM_S16_MIN, (int)IM_S16_MAX);
        else if (data_type == LegionDataType_U16)
            *(ImU16*)p_data = (ImU16)ImClamp(v32, (int)IM_U16_MIN, (int)IM_U16_MAX);
        else
            IM_ASSERT(0);
    }

    return memcmp(data_backup, p_data, type_info->Size) != 0;
}

static float GetMinimumStepAtDecimalPrecision(int decimal_precision)
{
    static const float min_steps[10] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f };
    if (decimal_precision < 0)
        return FLT_MIN;
    return (decimal_precision < IM_ARRAYSIZE(min_steps)) ? min_steps[decimal_precision] : ImPow(10.0f, (float)-decimal_precision);
}

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
TYPE Legion::RoundScalarWithFormatT(const char* format, LegionDataType data_type, TYPE v)
{
    const char* fmt_start = ImParseFormatFindStart(format);
    if (fmt_start[0] != '%' || fmt_start[1] == '%') // Don't apply if the value is not visible in the format string
        return v;
    char v_str[64];
    ImFormatString(v_str, IM_ARRAYSIZE(v_str), fmt_start, v);
    const char* p = v_str;
    while (*p == ' ')
        p++;
    if (data_type == LegionDataType_Float || data_type == LegionDataType_Double)
        v = (TYPE)ImAtof(p);
    else
        ImAtoi(p, (SIGNEDTYPE*)&v);
    return v;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: DragScalar, DragFloat, DragInt, etc.
//-------------------------------------------------------------------------
// - DragBehaviorT<>() [Internal]
// - DragBehavior() [Internal]
// - DragScalar()
// - DragScalarN()
// - DragFloat()
// - DragFloat2()
// - DragFloat3()
// - DragFloat4()
// - DragFloatRange2()
// - DragInt()
// - DragInt2()
// - DragInt3()
// - DragInt4()
// - DragIntRange2()
//-------------------------------------------------------------------------

// This is called by DragBehavior() when the widget is active (held by mouse or being manipulated with Nav controls)
template<typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
bool Legion::DragBehaviorT(LegionDataType data_type, TYPE* v, float v_speed, const TYPE v_min, const TYPE v_max, const char* format, float power, LegionDragFlags flags)
{
    LegionContext& g = *GLegion;
    const LegionAxis axis = (flags & LegionDragFlags_Vertical) ? LegionAxis_Y : LegionAxis_X;
    const bool is_decimal = (data_type == LegionDataType_Float) || (data_type == LegionDataType_Double);
    const bool is_clamped = (v_min < v_max);
    const bool is_power = (power != 1.0f && is_decimal && is_clamped && (v_max - v_min < FLT_MAX));
    const bool is_locked = (v_min > v_max);
    if (is_locked)
        return false;

    // Default tweak speed
    if (v_speed == 0.0f && is_clamped && (v_max - v_min < FLT_MAX))
        v_speed = (float)((v_max - v_min) * g.DragSpeedDefaultRatio);

    // Inputs accumulates into g.DragCurrentAccum, which is flushed into the current value as soon as it makes a difference with our precision settings
    float adjust_delta = 0.0f;
    if (g.ActiveIdSource == LegionInputSource_Mouse && IsMousePosValid() && g.IO.MouseDragMaxDistanceSqr[0] > 1.0f*1.0f)
    {
        adjust_delta = g.IO.MouseDelta[axis];
        if (g.IO.KeyAlt)
            adjust_delta *= 1.0f / 100.0f;
        if (g.IO.KeyShift)
            adjust_delta *= 10.0f;
    }
    else if (g.ActiveIdSource == LegionInputSource_Nav)
    {
        int decimal_precision = is_decimal ? ImParseFormatPrecision(format, 3) : 0;
        adjust_delta = GetNavInputAmount2d(LegionNavDirSourceFlags_Keyboard | LegionNavDirSourceFlags_PadDPad, LegionInputReadMode_RepeatFast, 1.0f / 10.0f, 10.0f)[axis];
        v_speed = ImMax(v_speed, GetMinimumStepAtDecimalPrecision(decimal_precision));
    }
    adjust_delta *= v_speed;

    // For vertical drag we currently assume that Up=higher value (like we do with vertical sliders). This may become a parameter.
    if (axis == LegionAxis_Y)
        adjust_delta = -adjust_delta;

    // Clear current value on activation
    // Avoid altering values and clamping when we are _already_ past the limits and heading in the same direction, so e.g. if range is 0..255, current value is 300 and we are pushing to the right side, keep the 300.
    bool is_just_activated = g.ActiveIdIsJustActivated;
    bool is_already_past_limits_and_pushing_outward = is_clamped && ((*v >= v_max && adjust_delta > 0.0f) || (*v <= v_min && adjust_delta < 0.0f));
    bool is_drag_direction_change_with_power = is_power && ((adjust_delta < 0 && g.DragCurrentAccum > 0) || (adjust_delta > 0 && g.DragCurrentAccum < 0));
    if (is_just_activated || is_already_past_limits_and_pushing_outward || is_drag_direction_change_with_power)
    {
        g.DragCurrentAccum = 0.0f;
        g.DragCurrentAccumDirty = false;
    }
    else if (adjust_delta != 0.0f)
    {
        g.DragCurrentAccum += adjust_delta;
        g.DragCurrentAccumDirty = true;
    }

    if (!g.DragCurrentAccumDirty)
        return false;

    TYPE v_cur = *v;
    FLOATTYPE v_old_ref_for_accum_remainder = (FLOATTYPE)0.0f;

    if (is_power)
    {
        // Offset + round to user desired precision, with a curve on the v_min..v_max range to get more precision on one side of the range
        FLOATTYPE v_old_norm_curved = ImPow((FLOATTYPE)(v_cur - v_min) / (FLOATTYPE)(v_max - v_min), (FLOATTYPE)1.0f / power);
        FLOATTYPE v_new_norm_curved = v_old_norm_curved + (g.DragCurrentAccum / (v_max - v_min));
        v_cur = v_min + (SIGNEDTYPE)ImPow(ImSaturate((float)v_new_norm_curved), power) * (v_max - v_min);
        v_old_ref_for_accum_remainder = v_old_norm_curved;
    }
    else
    {
        v_cur += (SIGNEDTYPE)g.DragCurrentAccum;
    }

    // Round to user desired precision based on format string
    v_cur = RoundScalarWithFormatT<TYPE, SIGNEDTYPE>(format, data_type, v_cur);

    // Preserve remainder after rounding has been applied. This also allow slow tweaking of values.
    g.DragCurrentAccumDirty = false;
    if (is_power)
    {
        FLOATTYPE v_cur_norm_curved = ImPow((FLOATTYPE)(v_cur - v_min) / (FLOATTYPE)(v_max - v_min), (FLOATTYPE)1.0f / power);
        g.DragCurrentAccum -= (float)(v_cur_norm_curved - v_old_ref_for_accum_remainder);
    }
    else
    {
        g.DragCurrentAccum -= (float)((SIGNEDTYPE)v_cur - (SIGNEDTYPE)*v);
    }

    // Lose zero sign for float/double
    if (v_cur == (TYPE)-0)
        v_cur = (TYPE)0;

    // Clamp values (+ handle overflow/wrap-around for integer types)
    if (*v != v_cur && is_clamped)
    {
        if (v_cur < v_min || (v_cur > *v && adjust_delta < 0.0f && !is_decimal))
            v_cur = v_min;
        if (v_cur > v_max || (v_cur < *v && adjust_delta > 0.0f && !is_decimal))
            v_cur = v_max;
    }

    // Apply result
    if (*v == v_cur)
        return false;
    *v = v_cur;
    return true;
}

bool Legion::DragBehavior(LegionID id, LegionDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, float power, LegionDragFlags flags)
{
    LegionContext& g = *GLegion;
    if (g.ActiveId == id)
    {
        if (g.ActiveIdSource == LegionInputSource_Mouse && !g.IO.MouseDown[0])
            ClearActiveID();
        else if (g.ActiveIdSource == LegionInputSource_Nav && g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
            ClearActiveID();
    }
    if (g.ActiveId != id)
        return false;

    switch (data_type)
    {
    case LegionDataType_S8:     { ImS32 v32 = (ImS32)*(ImS8*)p_v;  bool r = DragBehaviorT<ImS32, ImS32, float>(LegionDataType_S32, &v32, v_speed, p_min ? *(const ImS8*) p_min : IM_S8_MIN,  p_max ? *(const ImS8*)p_max  : IM_S8_MAX,  format, power, flags); if (r) *(ImS8*)p_v = (ImS8)v32; return r; }
    case LegionDataType_U8:     { ImU32 v32 = (ImU32)*(ImU8*)p_v;  bool r = DragBehaviorT<ImU32, ImS32, float>(LegionDataType_U32, &v32, v_speed, p_min ? *(const ImU8*) p_min : IM_U8_MIN,  p_max ? *(const ImU8*)p_max  : IM_U8_MAX,  format, power, flags); if (r) *(ImU8*)p_v = (ImU8)v32; return r; }
    case LegionDataType_S16:    { ImS32 v32 = (ImS32)*(ImS16*)p_v; bool r = DragBehaviorT<ImS32, ImS32, float>(LegionDataType_S32, &v32, v_speed, p_min ? *(const ImS16*)p_min : IM_S16_MIN, p_max ? *(const ImS16*)p_max : IM_S16_MAX, format, power, flags); if (r) *(ImS16*)p_v = (ImS16)v32; return r; }
    case LegionDataType_U16:    { ImU32 v32 = (ImU32)*(ImU16*)p_v; bool r = DragBehaviorT<ImU32, ImS32, float>(LegionDataType_U32, &v32, v_speed, p_min ? *(const ImU16*)p_min : IM_U16_MIN, p_max ? *(const ImU16*)p_max : IM_U16_MAX, format, power, flags); if (r) *(ImU16*)p_v = (ImU16)v32; return r; }
    case LegionDataType_S32:    return DragBehaviorT<ImS32, ImS32, float >(data_type, (ImS32*)p_v,  v_speed, p_min ? *(const ImS32* )p_min : IM_S32_MIN, p_max ? *(const ImS32* )p_max : IM_S32_MAX, format, power, flags);
    case LegionDataType_U32:    return DragBehaviorT<ImU32, ImS32, float >(data_type, (ImU32*)p_v,  v_speed, p_min ? *(const ImU32* )p_min : IM_U32_MIN, p_max ? *(const ImU32* )p_max : IM_U32_MAX, format, power, flags);
    case LegionDataType_S64:    return DragBehaviorT<ImS64, ImS64, double>(data_type, (ImS64*)p_v,  v_speed, p_min ? *(const ImS64* )p_min : IM_S64_MIN, p_max ? *(const ImS64* )p_max : IM_S64_MAX, format, power, flags);
    case LegionDataType_U64:    return DragBehaviorT<ImU64, ImS64, double>(data_type, (ImU64*)p_v,  v_speed, p_min ? *(const ImU64* )p_min : IM_U64_MIN, p_max ? *(const ImU64* )p_max : IM_U64_MAX, format, power, flags);
    case LegionDataType_Float:  return DragBehaviorT<float, float, float >(data_type, (float*)p_v,  v_speed, p_min ? *(const float* )p_min : -FLT_MAX,   p_max ? *(const float* )p_max : FLT_MAX,    format, power, flags);
    case LegionDataType_Double: return DragBehaviorT<double,double,double>(data_type, (double*)p_v, v_speed, p_min ? *(const double*)p_min : -DBL_MAX,   p_max ? *(const double*)p_max : DBL_MAX,    format, power, flags);
    case LegionDataType_COUNT:  break;
    }
    IM_ASSERT(0);
    return false;
}

// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a Drag widget, p_min and p_max are optional.
// Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool Legion::DragScalar(const char* label, LegionDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (power != 1.0f)
        IM_ASSERT(p_min != NULL && p_max != NULL); // When using a power curve the drag needs to have known bounds

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const float w = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == LegionDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    // Tabbing or CTRL-clicking on Drag turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = TempInputIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = FocusableItemRegister(window, id);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        const bool double_clicked = (hovered && g.IO.MouseDoubleClicked[0]);
        if (focus_requested || clicked || double_clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask = (1 << LegionDir_Left) | (1 << LegionDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || double_clicked || g.NavInputId == id)
            {
                temp_input_start = true;
                FocusableItemUnregister(window);
            }
        }
    }
    if (temp_input_is_active || temp_input_start)
        return TempInputScalar(frame_bb, id, label, data_type, p_data, format);

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? LegionCol_FrameBgActive : g.HoveredId == id ? LegionCol_FrameBgHovered : LegionCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

    // Drag behavior
    const bool value_changed = DragBehavior(id, data_type, p_data, v_speed, p_min, p_max, format, power, LegionDragFlags_None);
    if (value_changed)
        MarkItemEdited(id);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return value_changed;
}

bool Legion::DragScalarN(const char* label, LegionDataType data_type, void* p_data, int components, float v_speed, const void* p_min, const void* p_max, const char* format, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= DragScalar("", data_type, p_data, v_speed, p_min, p_max, format, power);
        PopID();
        PopItemWidth();
        p_data = (void*)((char*)p_data + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Legion::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
    return DragScalar(label, LegionDataType_Float, v, v_speed, &v_min, &v_max, format, power);
}

bool Legion::DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
{
    return DragScalarN(label, LegionDataType_Float, v, 2, v_speed, &v_min, &v_max, format, power);
}

bool Legion::DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, float power)
{
    return DragScalarN(label, LegionDataType_Float, v, 3, v_speed, &v_min, &v_max, format, power);
}

bool Legion::DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
{
    return DragScalarN(label, LegionDataType_Float, v, 4, v_speed, &v_min, &v_max, format, power);
}

bool Legion::DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min, float v_max, const char* format, const char* format_max, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2, CalcItemWidth());

    bool value_changed = DragFloat("##min", v_current_min, v_speed, (v_min >= v_max) ? -FLT_MAX : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), format, power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragFloat("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? FLT_MAX : v_max, format_max ? format_max : format, power);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextEx(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();
    return value_changed;
}

// NB: v_speed is float to allow adjusting the drag speed with more precision
bool Legion::DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format)
{
    return DragScalar(label, LegionDataType_S32, v, v_speed, &v_min, &v_max, format);
}

bool Legion::DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format)
{
    return DragScalarN(label, LegionDataType_S32, v, 2, v_speed, &v_min, &v_max, format);
}

bool Legion::DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format)
{
    return DragScalarN(label, LegionDataType_S32, v, 3, v_speed, &v_min, &v_max, format);
}

bool Legion::DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format)
{
    return DragScalarN(label, LegionDataType_S32, v, 4, v_speed, &v_min, &v_max, format);
}

bool Legion::DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min, int v_max, const char* format, const char* format_max)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    PushID(label);
    BeginGroup();
    PushMultiItemsWidths(2, CalcItemWidth());

    bool value_changed = DragInt("##min", v_current_min, v_speed, (v_min >= v_max) ? INT_MIN : v_min, (v_min >= v_max) ? *v_current_max : ImMin(v_max, *v_current_max), format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);
    value_changed |= DragInt("##max", v_current_max, v_speed, (v_min >= v_max) ? *v_current_min : ImMax(v_min, *v_current_min), (v_min >= v_max) ? INT_MAX : v_max, format_max ? format_max : format);
    PopItemWidth();
    SameLine(0, g.Style.ItemInnerSpacing.x);

    TextEx(label, FindRenderedTextEnd(label));
    EndGroup();
    PopID();

    return value_changed;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: SliderScalar, SliderFloat, SliderInt, etc.
//-------------------------------------------------------------------------
// - SliderBehaviorT<>() [Internal]
// - SliderBehavior() [Internal]
// - SliderScalar()
// - SliderScalarN()
// - SliderFloat()
// - SliderFloat2()
// - SliderFloat3()
// - SliderFloat4()
// - SliderAngle()
// - SliderInt()
// - SliderInt2()
// - SliderInt3()
// - SliderInt4()
// - VSliderScalar()
// - VSliderFloat()
// - VSliderInt()
//-------------------------------------------------------------------------

template<typename TYPE, typename FLOATTYPE>
float Legion::SliderCalcRatioFromValueT(LegionDataType data_type, TYPE v, TYPE v_min, TYPE v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max)
        return 0.0f;

    const bool is_power = (power != 1.0f) && (data_type == LegionDataType_Float || data_type == LegionDataType_Double);
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
bool Legion::SliderBehaviorT(const ImRect& bb, LegionID id, LegionDataType data_type, TYPE* v, const TYPE v_min, const TYPE v_max, const char* format, float power, LegionSliderFlags flags, ImRect* out_grab_bb)
{
    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    const LegionAxis axis = (flags & LegionSliderFlags_Vertical) ? LegionAxis_Y : LegionAxis_X;
    const bool is_decimal = (data_type == LegionDataType_Float) || (data_type == LegionDataType_Double);
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
        if (g.ActiveIdSource == LegionInputSource_Mouse)
        {
            if (!g.IO.MouseDown[0])
            {
                ClearActiveID();
            }
            else
            {
                const float mouse_abs_pos = g.IO.MousePos[axis];
                clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
                if (axis == LegionAxis_Y)
                    clicked_t = 1.0f - clicked_t;
                set_new_value = true;
            }
        }
        else if (g.ActiveIdSource == LegionInputSource_Nav)
        {
            const ImVec2 delta2 = GetNavInputAmount2d(LegionNavDirSourceFlags_Keyboard | LegionNavDirSourceFlags_PadDPad, LegionInputReadMode_RepeatFast, 0.0f, 0.0f);
            float delta = (axis == LegionAxis_X) ? delta2.x : -delta2.y;
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
                    if (IsNavInputDown(LegionNavInput_TweakSlow))
                        delta /= 10.0f;
                }
                else
                {
                    if ((v_range >= -100.0f && v_range <= 100.0f) || IsNavInputDown(LegionNavInput_TweakSlow))
                        delta = ((delta < 0.0f) ? -1.0f : +1.0f) / (float)v_range; // Gamepad/keyboard tweak speeds in integer steps
                    else
                        delta /= 100.0f;
                }
                if (IsNavInputDown(LegionNavInput_TweakFast))
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
            v_new = RoundScalarWithFormatT<TYPE,SIGNEDTYPE>(format, data_type, v_new);

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
        if (axis == LegionAxis_Y)
            grab_t = 1.0f - grab_t;
        const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
        if (axis == LegionAxis_X)
            *out_grab_bb = ImRect(grab_pos - grab_sz * 0.5f, bb.Min.y + grab_padding, grab_pos + grab_sz * 0.5f, bb.Max.y - grab_padding);
        else
            *out_grab_bb = ImRect(bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f, bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f);
    }

    return value_changed;
}

// For 32-bit and larger types, slider bounds are limited to half the natural type range.
// So e.g. an integer Slider between INT_MAX-10 and INT_MAX will fail, but an integer Slider between INT_MAX/2-10 and INT_MAX/2 will be ok.
// It would be possible to lift that limitation with some work but it doesn't seem to be worth it for sliders.
bool Legion::SliderBehavior(const ImRect& bb, LegionID id, LegionDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, float power, LegionSliderFlags flags, ImRect* out_grab_bb)
{
    switch (data_type)
    {
    case LegionDataType_S8:  { ImS32 v32 = (ImS32)*(ImS8*)p_v;  bool r = SliderBehaviorT<ImS32, ImS32, float>(bb, id, LegionDataType_S32, &v32, *(const ImS8*)p_min,  *(const ImS8*)p_max,  format, power, flags, out_grab_bb); if (r) *(ImS8*)p_v  = (ImS8)v32;  return r; }
    case LegionDataType_U8:  { ImU32 v32 = (ImU32)*(ImU8*)p_v;  bool r = SliderBehaviorT<ImU32, ImS32, float>(bb, id, LegionDataType_U32, &v32, *(const ImU8*)p_min,  *(const ImU8*)p_max,  format, power, flags, out_grab_bb); if (r) *(ImU8*)p_v  = (ImU8)v32;  return r; }
    case LegionDataType_S16: { ImS32 v32 = (ImS32)*(ImS16*)p_v; bool r = SliderBehaviorT<ImS32, ImS32, float>(bb, id, LegionDataType_S32, &v32, *(const ImS16*)p_min, *(const ImS16*)p_max, format, power, flags, out_grab_bb); if (r) *(ImS16*)p_v = (ImS16)v32; return r; }
    case LegionDataType_U16: { ImU32 v32 = (ImU32)*(ImU16*)p_v; bool r = SliderBehaviorT<ImU32, ImS32, float>(bb, id, LegionDataType_U32, &v32, *(const ImU16*)p_min, *(const ImU16*)p_max, format, power, flags, out_grab_bb); if (r) *(ImU16*)p_v = (ImU16)v32; return r; }
    case LegionDataType_S32:
        IM_ASSERT(*(const ImS32*)p_min >= IM_S32_MIN/2 && *(const ImS32*)p_max <= IM_S32_MAX/2);
        return SliderBehaviorT<ImS32, ImS32, float >(bb, id, data_type, (ImS32*)p_v,  *(const ImS32*)p_min,  *(const ImS32*)p_max,  format, power, flags, out_grab_bb);
    case LegionDataType_U32:
        IM_ASSERT(*(const ImU32*)p_max <= IM_U32_MAX/2);
        return SliderBehaviorT<ImU32, ImS32, float >(bb, id, data_type, (ImU32*)p_v,  *(const ImU32*)p_min,  *(const ImU32*)p_max,  format, power, flags, out_grab_bb);
    case LegionDataType_S64:
        IM_ASSERT(*(const ImS64*)p_min >= IM_S64_MIN/2 && *(const ImS64*)p_max <= IM_S64_MAX/2);
        return SliderBehaviorT<ImS64, ImS64, double>(bb, id, data_type, (ImS64*)p_v,  *(const ImS64*)p_min,  *(const ImS64*)p_max,  format, power, flags, out_grab_bb);
    case LegionDataType_U64:
        IM_ASSERT(*(const ImU64*)p_max <= IM_U64_MAX/2);
        return SliderBehaviorT<ImU64, ImS64, double>(bb, id, data_type, (ImU64*)p_v,  *(const ImU64*)p_min,  *(const ImU64*)p_max,  format, power, flags, out_grab_bb);
    case LegionDataType_Float:
        IM_ASSERT(*(const float*)p_min >= -FLT_MAX/2.0f && *(const float*)p_max <= FLT_MAX/2.0f);
        return SliderBehaviorT<float, float, float >(bb, id, data_type, (float*)p_v,  *(const float*)p_min,  *(const float*)p_max,  format, power, flags, out_grab_bb);
    case LegionDataType_Double:
        IM_ASSERT(*(const double*)p_min >= -DBL_MAX/2.0f && *(const double*)p_max <= DBL_MAX/2.0f);
        return SliderBehaviorT<double,double,double>(bb, id, data_type, (double*)p_v, *(const double*)p_min, *(const double*)p_max, format, power, flags, out_grab_bb);
    case LegionDataType_COUNT: break;
    }
    IM_ASSERT(0);
    return false;
}

float Legion::RoundScalar(float value, int decimal_precision)
{
    // Round past decimal precision
    // So when our value is 1.99999 with a precision of 0.001 we'll end up rounding to 2.0
    // FIXME: Investigate better rounding methods
    const float min_step = GetMinimumStepAtDecimalPrecision(decimal_precision);
    bool negative = value < 0.0f;
    value = fabsf(value);
    float remainder = fmodf(value, min_step);
    if (remainder <= min_step * 0.5f)
        value -= remainder;
    else
        value += (min_step - remainder);
    return negative ? -value : value;
}

static inline float SliderBehaviorCalcRatioFromValue(float v, float v_min, float v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max)
        return 0.0f;

    const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
    const float v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
    if (is_non_linear)
    {
        if (v_clamped < 0.0f)
        {
            const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
            return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
        }
        else
        {
            const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
            return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
        }
    }

    // Linear slider
    return (v_clamped - v_min) / (v_max - v_min);
}


bool Legion::SliderBehaviorEx(ImFont* font, float font_size, const ImRect& frame_bb, LegionID id, float* v, float v_min, float v_max, float power, int decimal_precision, LegionSliderFlags flags, ImColor color, ImVec2 valuesize, const char* label, char* value, bool isread)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();
    const LegionStyle& style = g.Style;

    window->DrawList->AddLine(ImVec2(frame_bb.Min.x, frame_bb.Min.y + ((frame_bb.Max.y - frame_bb.Min.y) / 2)), ImVec2(frame_bb.Max.x, frame_bb.Min.y + ((frame_bb.Max.y - frame_bb.Min.y) / 2)), ImColor(22, 37, 50), 4.0f);

    const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
    const bool is_horizontal = (flags & LegionSliderFlags_Vertical) == 0;

    const float grab_padding = 2.0f;
    const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
    float grab_sz;
    if (decimal_precision > 0)
        grab_sz = ImMin(style.GrabMinSize, slider_sz);
    else
        grab_sz = ImMin(ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize), slider_sz);
    const float slider_usable_sz = slider_sz - grab_sz;
    const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz * 0.5f;
    const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz * 0.5f;

    float linear_zero_pos = 0.0f;
    if (v_min * v_max < 0.0f)
    {
        const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
        const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
        linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
    }
    else
    {
        linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
    }

    bool isDown = false;
    bool value_changed = false;
    if (g.ActiveId == id && !isread)
    {
        if (g.IO.MouseDown[0])
        {
            isDown = true;
            const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
            float clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
            if (!is_horizontal)
                clicked_t = 1.0f - clicked_t;

            float new_value;
            if (is_non_linear)
            {
                if (clicked_t < linear_zero_pos)
                {
                    float a = 1.0f - (clicked_t / linear_zero_pos);
                    a = powf(a, power);
                    new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
                }
                else
                {
                    float a;
                    if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
                        a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
                    else
                        a = clicked_t;
                    a = powf(a, power);
                    new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
                }
            }
            else
            {
                new_value = ImLerp(v_min, v_max, clicked_t);
            }

            new_value = RoundScalar(new_value, decimal_precision);
            if (*v != new_value)
            {
                *v = new_value;
                value_changed = true;
            }
        }
        else
        {
            ClearActiveID();
        }
    }

    float grab_t = SliderBehaviorCalcRatioFromValue(*v, v_min, v_max, power, linear_zero_pos);

    if (!is_horizontal)
        grab_t = 1.0f - grab_t;
    const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
    ImRect grab_bb;
    if (is_horizontal)
        grab_bb = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
    else
        grab_bb = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
    window->DrawList->AddLine(ImVec2(frame_bb.Min.x, frame_bb.Min.y + ((frame_bb.Max.y - frame_bb.Min.y) / 2)), ImVec2(grab_bb.Min.x, frame_bb.Min.y + ((frame_bb.Max.y - frame_bb.Min.y) / 2)), ImColor(34, 123, 51), 4.0f);
    // if (isDown)
      //   window->DrawList->AddCircleFilled(ImVec2(grab_bb.Min.x + 6, grab_bb.Min.y + ((grab_bb.Max.y - grab_bb.Min.y) / 2)), 9, ImColor(34, 143, 51));
    window->DrawList->AddCircleFilled(ImVec2(grab_bb.Min.x + 6, grab_bb.Min.y + ((grab_bb.Max.y - grab_bb.Min.y) / 2)), 7, ImColor(34, 123, 51));
    //window->DrawList->AddRectFilled(ImVec2(grab_bb.Min.x, frame_bb.Min.y + 2), ImVec2(grab_bb.Max.x + valuesize.x, frame_bb.Min.y + 14), GetColorU32(ImVec4(0.21f, 0.20f, 0.21f, 1.00f)), 3);

    // window->DrawList->AddText(ImVec2(grab_bb.Min.x - 2, frame_bb.Min.y - 2), ImColor(255, 255, 255), value, label);

    char totaltext[200];

    strcpy(totaltext, "{c1ccdd}");
    strcat(totaltext, value);
    TextLabelColorV(ImVec2(grab_bb.Min.x - 2, frame_bb.Min.y - 2), font, font_size, totaltext);

    return value_changed;
}

// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a slider, they are all required.
// Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool Legion::SliderScalar(const char* label, LegionDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == LegionDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    bool temp_input_is_active = TempInputIsActive(id);
    bool temp_input_start = false;
    if (!temp_input_is_active)
    {
        const bool focus_requested = FocusableItemRegister(window, id);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Left) | (1 << LegionDir_Right);
            if (focus_requested || (clicked && g.IO.KeyCtrl) || g.NavInputId == id)
            {
                temp_input_start = true;
                FocusableItemUnregister(window);
            }
        }
    }
    if (temp_input_is_active || temp_input_start)
        return TempInputScalar(frame_bb, id, label, data_type, p_data, format);

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? LegionCol_FrameBgActive : g.HoveredId == id ? LegionCol_FrameBgHovered : LegionCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, power, LegionSliderFlags_None, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? LegionCol_SliderGrabActive : LegionCol_SliderGrab), style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f,0.5f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return value_changed;
}

// Add multiple sliders on 1 line for compact edition of multiple components
bool Legion::SliderScalarN(const char* label, LegionDataType data_type, void* v, int components, const void* v_min, const void* v_max, const char* format, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= SliderScalar("", data_type, v, v_min, v_max, format, power);
        PopID();
        PopItemWidth();
        v = (void*)((char*)v + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Legion::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
    return SliderScalar(label, LegionDataType_Float, v, &v_min, &v_max, format, power);
}

bool Legion::SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, float power)
{
    return SliderScalarN(label, LegionDataType_Float, v, 2, &v_min, &v_max, format, power);
}

bool Legion::SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power)
{
    return SliderScalarN(label, LegionDataType_Float, v, 3, &v_min, &v_max, format, power);
}

bool Legion::SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, float power)
{
    return SliderScalarN(label, LegionDataType_Float, v, 4, &v_min, &v_max, format, power);
}

bool Legion::SliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max, const char* format)
{
    if (format == NULL)
        format = "%.0f deg";
    float v_deg = (*v_rad) * 360.0f / (2*IM_PI);
    bool value_changed = SliderFloat(label, &v_deg, v_degrees_min, v_degrees_max, format, 1.0f);
    *v_rad = v_deg * (2*IM_PI) / 360.0f;
    return value_changed;
}

bool Legion::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format)
{
    return SliderScalar(label, LegionDataType_S32, v, &v_min, &v_max, format);
}

bool Legion::SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format)
{
    return SliderScalarN(label, LegionDataType_S32, v, 2, &v_min, &v_max, format);
}

bool Legion::SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format)
{
    return SliderScalarN(label, LegionDataType_S32, v, 3, &v_min, &v_max, format);
}

bool Legion::SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format)
{
    return SliderScalarN(label, LegionDataType_S32, v, 4, &v_min, &v_max, format);
}

bool Legion::VSliderScalar(const char* label, const ImVec2& size, LegionDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == LegionDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    const bool hovered = ItemHoverable(frame_bb, id);
    if ((hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || g.NavInputId == id)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Up) | (1 << LegionDir_Down);
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? LegionCol_FrameBgActive : g.HoveredId == id ? LegionCol_FrameBgHovered : LegionCol_FrameBg);
    RenderNavHighlight(frame_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, power, LegionSliderFlags_Vertical, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    // Render grab
    if (grab_bb.Max.y > grab_bb.Min.y)
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? LegionCol_SliderGrabActive : LegionCol_SliderGrab), style.GrabRounding);

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    // For the vertical slider we allow centered text to overlap the frame padding
    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f,0.0f));
    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    return value_changed;
}

bool Legion::VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format, float power)
{
    return VSliderScalar(label, size, LegionDataType_Float, v, &v_min, &v_max, format, power);
}

bool Legion::VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format)
{
    return VSliderScalar(label, size, LegionDataType_S32, v, &v_min, &v_max, format);
}

bool Legion::SliderFloatEx(ImFont* font, float font_size, const char* label, float* v, float v_min, float v_max, const char* display_format, float power, ImVec2 size, ImColor color, bool islabel, float sizes, bool isread)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const float w = CalcItemWidth();
    const ImVec2 label_size = CalcTextSize(label, NULL, true) * 2.7f;
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w - sizes, label_size.y + style.FramePadding.y * 0.5f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    if (!ItemAdd(total_bb, id))
    {
        ItemSize(total_bb, style.FramePadding.y);
        return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        SetHoveredID(id);
    if (!display_format)
        display_format = "%.3f";
    int decimal_precision = ImParseFormatPrecision(display_format, 3);
    bool start_text_input = false;
    const bool tab_focus_requested = FocusableItemRegister(window, g.ActiveId == id);
    if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
    {
        SetActiveID(id, window);
        FocusWindow(window);
        if (tab_focus_requested || g.IO.KeyCtrl)
        {
            start_text_input = true;
            g.TempInputId = 0;
        }
    }
    //if (start_text_input || (g.ActiveId == id && g.TempInputTextId == id))
    //	return InputScalarAsWidgetReplacement(frame_bb, label, LegionDataType_Float, v, id, decimal_precision);
    ItemSize(total_bb, style.FramePadding.y);
    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);

    const bool value_changed = SliderBehaviorEx(font, font_size, frame_bb, id, v, v_min, v_max, power, decimal_precision, 0, color, CalcTextSizeV(value_buf, font, font_size, NULL, false), value_buf_end, value_buf, isread);
    if (label_size.x > 0.0f && islabel)
        TextLabelColorV(ImVec2(frame_bb.Min.x, frame_bb.Min.y), font, font_size, label);
        //RenderText(ImVec2(frame_bb.Min.x, frame_bb.Min.y), label);
    return value_changed;
}
/// ProgressBar

bool Legion::ProgressBehaviorEx(ImFont* font, float font_size, const ImRect& frame_bb, LegionID id, float v, float v_min, float v_max, float power, LegionSliderFlags flags, ImColor color, ImColor valuecolor, bool IsValue)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();
    const LegionStyle& style = g.Style;

    RenderFrame(frame_bb.Min, frame_bb.Max, color, false, 2);
  
    float grab_sz = ImMin(style.GrabMinSize, frame_bb.GetWidth());

    const float slider_usable_pos_min = frame_bb.Min.x +  grab_sz * 0.5f;
    const float slider_usable_pos_max = frame_bb.Max.x -  grab_sz * 0.5f;

    float linear_zero_pos = 0.0f;
    if (v_min * v_max < 0.0f)
    {
        const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
        const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
        linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
    }
    else
    {
        linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
    }


    float grab_t = SliderBehaviorCalcRatioFromValue(v, v_min, v_max, power, linear_zero_pos);

    const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
    ImRect grab_bb = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y));

    RenderFrame(frame_bb.Min, grab_bb.Max, valuecolor, false, 2);

    if (IsValue)
    {
        float value = (v / v_max) * 100;

        char value_buf[64];
        ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), "{00ff05}%.0f%%", value);

        char totaltext[200];

        strcpy(totaltext, value_buf);
        strcat(totaltext, "%%");
        TextLabelColorV(ImVec2(frame_bb.Max.x + 8, frame_bb.Max.y - 11), font, font_size, totaltext);
    }
    return true;
}


bool Legion::ProgressBarEx(ImFont* font, float font_size, const char* label, float v, float v_min, float v_max, float power, ImVec2 size, ImColor color, ImColor valuecolor, bool islabel, bool IsValue)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(frame_bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        SetHoveredID(id);

 
    return ProgressBehaviorEx(font, font_size, frame_bb, id, v, v_min, v_max, power, 0, color, valuecolor, IsValue);
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: InputScalar, InputFloat, InputInt, etc.
//-------------------------------------------------------------------------
// - ImParseFormatFindStart() [Internal]
// - ImParseFormatFindEnd() [Internal]
// - ImParseFormatTrimDecorations() [Internal]
// - ImParseFormatPrecision() [Internal]
// - TempInputTextScalar() [Internal]
// - InputScalar()
// - InputScalarN()
// - InputFloat()
// - InputFloat2()
// - InputFloat3()
// - InputFloat4()
// - InputInt()
// - InputInt2()
// - InputInt3()
// - InputInt4()
// - InputDouble()
//-------------------------------------------------------------------------

// We don't use strchr() because our strings are usually very short and often start with '%'
const char* ImParseFormatFindStart(const char* fmt)
{
    while (char c = fmt[0])
    {
        if (c == '%' && fmt[1] != '%')
            return fmt;
        else if (c == '%')
            fmt++;
        fmt++;
    }
    return fmt;
}

const char* ImParseFormatFindEnd(const char* fmt)
{
    // Printf/scanf types modifiers: I/L/h/j/l/t/w/z. Other uppercase letters qualify as types aka end of the format.
    if (fmt[0] != '%')
        return fmt;
    const unsigned int ignored_uppercase_mask = (1 << ('I'-'A')) | (1 << ('L'-'A'));
    const unsigned int ignored_lowercase_mask = (1 << ('h'-'a')) | (1 << ('j'-'a')) | (1 << ('l'-'a')) | (1 << ('t'-'a')) | (1 << ('w'-'a')) | (1 << ('z'-'a'));
    for (char c; (c = *fmt) != 0; fmt++)
    {
        if (c >= 'A' && c <= 'Z' && ((1 << (c - 'A')) & ignored_uppercase_mask) == 0)
            return fmt + 1;
        if (c >= 'a' && c <= 'z' && ((1 << (c - 'a')) & ignored_lowercase_mask) == 0)
            return fmt + 1;
    }
    return fmt;
}

// Extract the format out of a format string with leading or trailing decorations
//  fmt = "blah blah"  -> return fmt
//  fmt = "%.3f"       -> return fmt
//  fmt = "hello %.3f" -> return fmt + 6
//  fmt = "%.3f hello" -> return buf written with "%.3f"
const char* ImParseFormatTrimDecorations(const char* fmt, char* buf, size_t buf_size)
{
    const char* fmt_start = ImParseFormatFindStart(fmt);
    if (fmt_start[0] != '%')
        return fmt;
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);
    if (fmt_end[0] == 0) // If we only have leading decoration, we don't need to copy the data.
        return fmt_start;
    ImStrncpy(buf, fmt_start, ImMin((size_t)(fmt_end - fmt_start) + 1, buf_size));
    return buf;
}

// Parse display precision back from the display format string
// FIXME: This is still used by some navigation code path to infer a minimum tweak step, but we should aim to rework widgets so it isn't needed.
int ImParseFormatPrecision(const char* fmt, int default_precision)
{
    fmt = ImParseFormatFindStart(fmt);
    if (fmt[0] != '%')
        return default_precision;
    fmt++;
    while (*fmt >= '0' && *fmt <= '9')
        fmt++;
    int precision = INT_MAX;
    if (*fmt == '.')
    {
        fmt = ImAtoi<int>(fmt + 1, &precision);
        if (precision < 0 || precision > 99)
            precision = default_precision;
    }
    if (*fmt == 'e' || *fmt == 'E') // Maximum precision with scientific notation
        precision = -1;
    if ((*fmt == 'g' || *fmt == 'G') && precision == INT_MAX)
        precision = -1;
    return (precision == INT_MAX) ? default_precision : precision;
}

// Create text input in place of another active widget (e.g. used when doing a CTRL+Click on drag/slider widgets)
// FIXME: Facilitate using this in variety of other situations.
bool Legion::TempInputText(const ImRect& bb, LegionID id, const char* label, char* buf, int buf_size, LegionInputTextFlags flags)
{
    // On the first frame, g.TempInputTextId == 0, then on subsequent frames it becomes == id.
    // We clear ActiveID on the first frame to allow the InputText() taking it back.
    LegionContext& g = *GLegion;
    const bool init = (g.TempInputId != id);
    if (init)
        ClearActiveID();

    g.CurrentWindow->DC.CursorPos = bb.Min;
    bool value_changed = InputTextEx(label, NULL, buf, buf_size, bb.GetSize(), flags);
    if (init)
    {
        // First frame we started displaying the InputText widget, we expect it to take the active id.
        IM_ASSERT(g.ActiveId == id);
        g.TempInputId = g.ActiveId;
    }
    return value_changed;
}

bool Legion::TempInputScalar(const ImRect& bb, LegionID id, const char* label, LegionDataType data_type, void* p_data, const char* format)
{
    LegionContext& g = *GLegion;

    char fmt_buf[32];
    char data_buf[32];
    format = ImParseFormatTrimDecorations(format, fmt_buf, IM_ARRAYSIZE(fmt_buf));
    DataTypeFormatString(data_buf, IM_ARRAYSIZE(data_buf), data_type, p_data, format);
    ImStrTrimBlanks(data_buf);

    LegionInputTextFlags flags = LegionInputTextFlags_AutoSelectAll | LegionInputTextFlags_NoMarkEdited;
    flags |= ((data_type == LegionDataType_Float || data_type == LegionDataType_Double) ? LegionInputTextFlags_CharsScientific : LegionInputTextFlags_CharsDecimal);
    bool value_changed = TempInputText(bb, id, label, data_buf, IM_ARRAYSIZE(data_buf), flags);
    if (value_changed)
    {
        value_changed = DataTypeApplyOpFromText(data_buf, g.InputTextState.InitialTextA.Data, data_type, p_data, NULL);
        if (value_changed)
            MarkItemEdited(id);
    }
    return value_changed;
}

// Note: p_data, p_step, p_step_fast are _pointers_ to a memory address holding the data. For an Input widget, p_step and p_step_fast are optional.
// Read code of e.g. InputFloat(), InputInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool Legion::InputScalar(const char* label, LegionDataType data_type, void* p_data, const void* p_step, const void* p_step_fast, const char* format, LegionInputTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    LegionStyle& style = g.Style;

    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    char buf[64];
    DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_data, format);

    bool value_changed = false;
    if ((flags & (LegionInputTextFlags_CharsHexadecimal | LegionInputTextFlags_CharsScientific)) == 0)
        flags |= LegionInputTextFlags_CharsDecimal;
    flags |= LegionInputTextFlags_AutoSelectAll;
    flags |= LegionInputTextFlags_NoMarkEdited;  // We call MarkItemEdited() ourselve by comparing the actual data rather than the string.

    if (p_step != NULL)
    {
        const float button_size = GetFrameHeight();

        BeginGroup(); // The only purpose of the group here is to allow the caller to query item data e.g. IsItemActive()
        PushID(label);
        SetNextItemWidth(ImMax(1.0f, CalcItemWidth() - (button_size + style.ItemInnerSpacing.x) * 2));
        if (InputText("", buf, IM_ARRAYSIZE(buf), flags)) // PushId(label) + "" gives us the expected ID from outside point of view
            value_changed = DataTypeApplyOpFromText(buf, g.InputTextState.InitialTextA.Data, data_type, p_data, format);

        // Step buttons
        const ImVec2 backup_frame_padding = style.FramePadding;
        style.FramePadding.x = style.FramePadding.y;
        LegionButtonFlags button_flags = LegionButtonFlags_Repeat | LegionButtonFlags_DontClosePopups;
        if (flags & LegionInputTextFlags_ReadOnly)
            button_flags |= LegionButtonFlags_Disabled;
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("-", ImVec2(button_size, button_size), button_flags))
        {
            DataTypeApplyOp(data_type, '-', p_data, p_data, g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
            value_changed = true;
        }
        SameLine(0, style.ItemInnerSpacing.x);
        if (ButtonEx("+", ImVec2(button_size, button_size), button_flags))
        {
            DataTypeApplyOp(data_type, '+', p_data, p_data, g.IO.KeyCtrl && p_step_fast ? p_step_fast : p_step);
            value_changed = true;
        }

        const char* label_end = FindRenderedTextEnd(label);
        if (label != label_end)
        {
            SameLine(0, style.ItemInnerSpacing.x);
            TextEx(label, label_end);
        }
        style.FramePadding = backup_frame_padding;

        PopID();
        EndGroup();
    }
    else
    {
        if (InputText(label, buf, IM_ARRAYSIZE(buf), flags))
            value_changed = DataTypeApplyOpFromText(buf, g.InputTextState.InitialTextA.Data, data_type, p_data, format);
    }
    if (value_changed)
        MarkItemEdited(window->DC.LastItemId);

    return value_changed;
}

bool Legion::InputScalarN(const char* label, LegionDataType data_type, void* p_data, int components, const void* p_step, const void* p_step_fast, const char* format, LegionInputTextFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    bool value_changed = false;
    BeginGroup();
    PushID(label);
    PushMultiItemsWidths(components, CalcItemWidth());
    size_t type_size = GDataTypeInfo[data_type].Size;
    for (int i = 0; i < components; i++)
    {
        PushID(i);
        if (i > 0)
            SameLine(0, g.Style.ItemInnerSpacing.x);
        value_changed |= InputScalar("", data_type, p_data, p_step, p_step_fast, format, flags);
        PopID();
        PopItemWidth();
        p_data = (void*)((char*)p_data + type_size);
    }
    PopID();

    const char* label_end = FindRenderedTextEnd(label);
    if (label != label_end)
    {
        SameLine(0.0f, g.Style.ItemInnerSpacing.x);
        TextEx(label, label_end);
    }

    EndGroup();
    return value_changed;
}

bool Legion::InputFloat(const char* label, float* v, float step, float step_fast, const char* format, LegionInputTextFlags flags)
{
    flags |= LegionInputTextFlags_CharsScientific;
    return InputScalar(label, LegionDataType_Float, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), format, flags);
}

bool Legion::InputFloat2(const char* label, float v[2], const char* format, LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_Float, v, 2, NULL, NULL, format, flags);
}

bool Legion::InputFloat3(const char* label, float v[3], const char* format, LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_Float, v, 3, NULL, NULL, format, flags);
}

bool Legion::InputFloat4(const char* label, float v[4], const char* format, LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_Float, v, 4, NULL, NULL, format, flags);
}

// Prefer using "const char* format" directly, which is more flexible and consistent with other API.
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
bool Legion::InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, LegionInputTextFlags flags)
{
    char format[16] = "%f";
    if (decimal_precision >= 0)
        ImFormatString(format, IM_ARRAYSIZE(format), "%%.%df", decimal_precision);
    return InputFloat(label, v, step, step_fast, format, flags);
}

bool Legion::InputFloat2(const char* label, float v[2], int decimal_precision, LegionInputTextFlags flags)
{
    char format[16] = "%f";
    if (decimal_precision >= 0)
        ImFormatString(format, IM_ARRAYSIZE(format), "%%.%df", decimal_precision);
    return InputScalarN(label, LegionDataType_Float, v, 2, NULL, NULL, format, flags);
}

bool Legion::InputFloat3(const char* label, float v[3], int decimal_precision, LegionInputTextFlags flags)
{
    char format[16] = "%f";
    if (decimal_precision >= 0)
        ImFormatString(format, IM_ARRAYSIZE(format), "%%.%df", decimal_precision);
    return InputScalarN(label, LegionDataType_Float, v, 3, NULL, NULL, format, flags);
}

bool Legion::InputFloat4(const char* label, float v[4], int decimal_precision, LegionInputTextFlags flags)
{
    char format[16] = "%f";
    if (decimal_precision >= 0)
        ImFormatString(format, IM_ARRAYSIZE(format), "%%.%df", decimal_precision);
    return InputScalarN(label, LegionDataType_Float, v, 4, NULL, NULL, format, flags);
}
#endif // Legion_DISABLE_OBSOLETE_FUNCTIONS

bool Legion::InputInt(const char* label, int* v, int step, int step_fast, LegionInputTextFlags flags)
{
    // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
    const char* format = (flags & LegionInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
    return InputScalar(label, LegionDataType_S32, (void*)v, (void*)(step>0 ? &step : NULL), (void*)(step_fast>0 ? &step_fast : NULL), format, flags);
}

bool Legion::InputInt2(const char* label, int v[2], LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_S32, v, 2, NULL, NULL, "%d", flags);
}

bool Legion::InputInt3(const char* label, int v[3], LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_S32, v, 3, NULL, NULL, "%d", flags);
}

bool Legion::InputInt4(const char* label, int v[4], LegionInputTextFlags flags)
{
    return InputScalarN(label, LegionDataType_S32, v, 4, NULL, NULL, "%d", flags);
}

bool Legion::InputDouble(const char* label, double* v, double step, double step_fast, const char* format, LegionInputTextFlags flags)
{
    flags |= LegionInputTextFlags_CharsScientific;
    return InputScalar(label, LegionDataType_Double, (void*)v, (void*)(step>0.0 ? &step : NULL), (void*)(step_fast>0.0 ? &step_fast : NULL), format, flags);
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: InputText, InputTextMultiline, InputTextWithHint
//-------------------------------------------------------------------------
// - InputText()
// - InputTextWithHint()
// - InputTextMultiline()
// - InputTextEx() [Internal]
//-------------------------------------------------------------------------

bool Legion::InputTextV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, char* buf, size_t buf_size, const ImVec2& size_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & LegionInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextExV(cid, font, font_size, texture, label, NULL, buf, (int)buf_size, size_arg, flags, callback, user_data );
}

bool Legion::InputText(const char* label, char* buf, size_t buf_size, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & LegionInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextEx(label, NULL, buf, (int)buf_size, ImVec2(0,0), flags, callback, user_data);
}

bool Legion::InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    return InputTextEx(label, NULL, buf, (int)buf_size, size, flags | LegionInputTextFlags_Multiline, callback, user_data);
}

bool Legion::InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & LegionInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextEx(label, hint, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
}

bool Legion::InputTextWithHintV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, size_t buf_size, const ImVec2& size_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & LegionInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextExV(cid, font, font_size, texture, label, hint, buf, (int)buf_size, size_arg, flags, callback, user_data);
}

int Legion::InputTextWithHintExV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, size_t buf_size, const ImVec2& size_arg, ImTextureID btntexture, const char* btnlabel, const ImVec2& btnsize_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    IM_ASSERT(!(flags & LegionInputTextFlags_Multiline)); // call InputTextMultiline()
    return InputTextExB(cid, font, font_size, texture, label, hint, buf, (int)buf_size, size_arg, btntexture, btnlabel, btnsize_arg, flags, callback, user_data);
}

static int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
    int line_count = 0;
    const char* s = text_begin;
    while (char c = *s++) // We are only matching for \n so we can ignore UTF-8 decoding
        if (c == '\n')
            line_count++;
    s--;
    if (s[0] != '\n' && s[0] != '\r')
        line_count++;
    *out_text_end = s;
    return line_count;
}

static ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
    LegionContext& g = *GLegion;
    ImFont* font = g.Font;
    const float line_height = g.FontSize;
    const float scale = line_height / font->FontSize;

    ImVec2 text_size = ImVec2(0,0);
    float line_width = 0.0f;

    const ImWchar* s = text_begin;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)(*s++);
        if (c == '\n')
        {
            text_size.x = ImMax(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0.0f;
            if (stop_on_new_line)
                break;
            continue;
        }
        if (c == '\r')
            continue;

        const float char_width = font->GetCharAdvance((ImWchar)c) * scale;
        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (out_offset)
        *out_offset = ImVec2(line_width, text_size.y + line_height);  // offset allow for the possibility of sitting after a trailing \n

    if (line_width > 0 || text_size.y == 0.0f)                        // whereas size.y will ignore the trailing \n
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

// Wrapper for stb_textedit.h to edit text (our wrapper is for: statically sized buffer, single-line, wchar characters. InputText converts between UTF-8 and wchar)
namespace ImStb
{

static int     STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING* obj)                             { return obj->CurLenW; }
static ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING* obj, int idx)                      { return obj->TextW[idx]; }
static float   STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING* obj, int line_start_idx, int char_idx)  { ImWchar c = obj->TextW[line_start_idx + char_idx]; if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE; LegionContext& g = *GLegion; return g.Font->GetCharAdvance(c) * (g.FontSize / g.Font->FontSize); }
static int     STB_TEXTEDIT_KEYTOTEXT(int key)                                                    { return key >= 0x200000 ? 0 : key; }
static ImWchar STB_TEXTEDIT_NEWLINE = '\n';
static void    STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, STB_TEXTEDIT_STRING* obj, int line_start_idx)
{
    const ImWchar* text = obj->TextW.Data;
    const ImWchar* text_remaining = NULL;
    const ImVec2 size = InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
    r->x0 = 0.0f;
    r->x1 = size.x;
    r->baseline_y_delta = size.y;
    r->ymin = 0.0f;
    r->ymax = size.y;
    r->num_chars = (int)(text_remaining - (text + line_start_idx));
}

static bool is_separator(unsigned int c)                                        { return ImCharIsBlankW(c) || c==',' || c==';' || c=='(' || c==')' || c=='{' || c=='}' || c=='[' || c==']' || c=='|'; }
static int  is_word_boundary_from_right(STB_TEXTEDIT_STRING* obj, int idx)      { return idx > 0 ? (is_separator( obj->TextW[idx-1] ) && !is_separator( obj->TextW[idx] ) ) : 1; }
static int  STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)   { idx--; while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; }
#ifdef __APPLE__    // FIXME: Move setting to IO structure
static int  is_word_boundary_from_left(STB_TEXTEDIT_STRING* obj, int idx)       { return idx > 0 ? (!is_separator( obj->TextW[idx-1] ) && is_separator( obj->TextW[idx] ) ) : 1; }
static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)  { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_left(obj, idx)) idx++; return idx > len ? len : idx; }
#else
static int  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING* obj, int idx)  { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; }
#endif
#define STB_TEXTEDIT_MOVEWORDLEFT   STB_TEXTEDIT_MOVEWORDLEFT_IMPL    // They need to be #define for stb_textedit.h
#define STB_TEXTEDIT_MOVEWORDRIGHT  STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

static void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
{
    ImWchar* dst = obj->TextW.Data + pos;

    // We maintain our buffer length in both UTF-8 and wchar formats
    obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
    obj->CurLenW -= n;

    // Offset remaining text (FIXME-OPT: Use memmove)
    const ImWchar* src = obj->TextW.Data + pos + n;
    while (ImWchar c = *src++)
        *dst++ = c;
    *dst = '\0';
}

static bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
{
    const bool is_resizable = (obj->UserFlags & LegionInputTextFlags_CallbackResize) != 0;
    const int text_len = obj->CurLenW;
    IM_ASSERT(pos <= text_len);

    const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
    if (!is_resizable && (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufCapacityA))
        return false;

    // Grow internal buffer if needed
    if (new_text_len + text_len + 1 > obj->TextW.Size)
    {
        if (!is_resizable)
            return false;
        IM_ASSERT(text_len < obj->TextW.Size);
        obj->TextW.resize(text_len + ImClamp(new_text_len * 4, 32, ImMax(256, new_text_len)) + 1);
    }

    ImWchar* text = obj->TextW.Data;
    if (pos != text_len)
        memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
    memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));

    obj->CurLenW += new_text_len;
    obj->CurLenA += new_text_len_utf8;
    obj->TextW[obj->CurLenW] = '\0';

    return true;
}

// We don't use an enum so we can build even with conflicting symbols (if another user of stb_textedit.h leak their STB_TEXTEDIT_K_* symbols)
#define STB_TEXTEDIT_K_LEFT         0x200000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x200001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x200002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x200003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x200004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x200005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x200006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x200007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x200008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x200009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x20000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x20000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x20000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x20000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_SHIFT        0x400000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "imstb_textedit.h"

// stb_textedit internally allows for a single undo record to do addition and deletion, but somehow, calling
// the stb_textedit_paste() function creates two separate records, so we perform it manually. (FIXME: Report to nothings/stb?)
static void stb_textedit_replace(STB_TEXTEDIT_STRING* str, STB_TexteditState* state, const STB_TEXTEDIT_CHARTYPE* text, int text_len)
{
    stb_text_makeundo_replace(str, state, 0, str->CurLenW, text_len);
    ImStb::STB_TEXTEDIT_DELETECHARS(str, 0, str->CurLenW);
    if (text_len <= 0)
        return;
    if (ImStb::STB_TEXTEDIT_INSERTCHARS(str, 0, text, text_len))
    {
        state->cursor = text_len;
        state->has_preferred_x = 0;
        return;
    }
    IM_ASSERT(0); // Failed to insert character, normally shouldn't happen because of how we currently use stb_textedit_replace()
}

} // namespace ImStb

void LegionInputTextState::OnKeyPressed(int key)
{
    stb_textedit_key(this, &Stb, key);
    CursorFollow = true;
    CursorAnimReset();
}

LegionInputTextCallbackData::LegionInputTextCallbackData()
{
    memset(this, 0, sizeof(*this));
}

// Public API to manipulate UTF-8 text
// We expose UTF-8 to the user (unlike the STB_TEXTEDIT_* functions which are manipulating wchar)
// FIXME: The existence of this rarely exercised code path is a bit of a nuisance.
void LegionInputTextCallbackData::DeleteChars(int pos, int bytes_count)
{
    IM_ASSERT(pos + bytes_count <= BufTextLen);
    char* dst = Buf + pos;
    const char* src = Buf + pos + bytes_count;
    while (char c = *src++)
        *dst++ = c;
    *dst = '\0';

    if (CursorPos + bytes_count >= pos)
        CursorPos -= bytes_count;
    else if (CursorPos >= pos)
        CursorPos = pos;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen -= bytes_count;
}

void LegionInputTextCallbackData::InsertChars(int pos, const char* new_text, const char* new_text_end)
{
    const bool is_resizable = (Flags & LegionInputTextFlags_CallbackResize) != 0;
    const int new_text_len = new_text_end ? (int)(new_text_end - new_text) : (int)strlen(new_text);
    if (new_text_len + BufTextLen >= BufSize)
    {
        if (!is_resizable)
            return;

        // Contrary to STB_TEXTEDIT_INSERTCHARS() this is working in the UTF8 buffer, hence the midly similar code (until we remove the U16 buffer alltogether!)
        LegionContext& g = *GLegion;
        LegionInputTextState* edit_state = &g.InputTextState;
        IM_ASSERT(edit_state->ID != 0 && g.ActiveId == edit_state->ID);
        IM_ASSERT(Buf == edit_state->TextA.Data);
        int new_buf_size = BufTextLen + ImClamp(new_text_len * 4, 32, ImMax(256, new_text_len)) + 1;
        edit_state->TextA.reserve(new_buf_size + 1);
        Buf = edit_state->TextA.Data;
        BufSize = edit_state->BufCapacityA = new_buf_size;
    }

    if (BufTextLen != pos)
        memmove(Buf + pos + new_text_len, Buf + pos, (size_t)(BufTextLen - pos));
    memcpy(Buf + pos, new_text, (size_t)new_text_len * sizeof(char));
    Buf[BufTextLen + new_text_len] = '\0';

    if (CursorPos >= pos)
        CursorPos += new_text_len;
    SelectionStart = SelectionEnd = CursorPos;
    BufDirty = true;
    BufTextLen += new_text_len;
}

// Return false to discard a character.
static bool InputTextFilterCharacter(unsigned int* p_char, LegionInputTextFlags flags, LegionInputTextCallback callback, void* user_data)
{
    unsigned int c = *p_char;

    // Filter non-printable (NB: isprint is unreliable! see #2467)
    if (c < 0x20)
    {
        bool pass = false;
        pass |= (c == '\n' && (flags & LegionInputTextFlags_Multiline));
        pass |= (c == '\t' && (flags & LegionInputTextFlags_AllowTabInput));
        if (!pass)
            return false;
    }

    // We ignore Ascii representation of delete (emitted from Backspace on OSX, see #2578, #2817)
    if (c == 127)
        return false;

    // Filter private Unicode range. GLFW on OSX seems to send private characters for special keys like arrow keys (FIXME)
    if (c >= 0xE000 && c <= 0xF8FF)
        return false;

    // Filter Unicode ranges we are not handling in this build.
    if (c > IM_UNICODE_CODEPOINT_MAX)
        return false;

    // Generic named filters
    if (flags & (LegionInputTextFlags_CharsDecimal | LegionInputTextFlags_CharsHexadecimal | LegionInputTextFlags_CharsUppercase | LegionInputTextFlags_CharsNoBlank | LegionInputTextFlags_CharsScientific | LegionInputTextFlags_CharsHexadecimalNoBlank))
    {
        if (flags & LegionInputTextFlags_CharsDecimal)
            if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/'))
                return false;

        if (flags & LegionInputTextFlags_CharsScientific)
            if (!(c >= '0' && c <= '9') && (c != '.') && (c != '-') && (c != '+') && (c != '*') && (c != '/') && (c != 'e') && (c != 'E'))
                return false;

        if (flags & LegionInputTextFlags_CharsHexadecimal)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F') && !(c >= ' '))
                return false;

        if (flags & LegionInputTextFlags_CharsUppercase)
            if (c >= 'a' && c <= 'z')
                *p_char = (c += (unsigned int)('A'-'a'));

        if (flags & LegionInputTextFlags_CharsNoBlank)
            if (ImCharIsBlankW(c))
                return false;

        if (flags & LegionInputTextFlags_CharsHexadecimalNoBlank)
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F'))
                return false;
    }

    // Custom callback filter
    if (flags & LegionInputTextFlags_CallbackCharFilter)
    {
        LegionInputTextCallbackData callback_data;
        memset(&callback_data, 0, sizeof(LegionInputTextCallbackData));
        callback_data.EventFlag = LegionInputTextFlags_CallbackCharFilter;
        callback_data.EventChar = (ImWchar)c;
        callback_data.Flags = flags;
        callback_data.UserData = user_data;
        if (callback(&callback_data) != 0)
            return false;
        *p_char = callback_data.EventChar;
        if (!callback_data.EventChar)
            return false;
    }

    return true;
}

// Edit a string of text
// - buf_size account for the zero-terminator, so a buf_size of 6 can hold "Hello" but not "Hello!".
//   This is so we can easily call InputText() on static arrays using ARRAYSIZE() and to match
//   Note that in std::string world, capacity() would omit 1 byte used by the zero-terminator.
// - When active, hold on a privately held copy of the text (and apply back to 'buf'). So changing 'buf' while the InputText is active has no effect.
// - If you want to use Legion::InputText() with std::string, see misc/cpp/Legion_stdlib.h
// (FIXME: Rather confusing and messy function, among the worse part of our codebase, expecting to rewrite a V2 at some point.. Partly because we are
//  doing UTF8 > U16 > UTF8 conversions on the go to easily interface with stb_textedit. Ideally should stay in UTF-8 all the time. See https://github.com/nothings/stb/issues/188)
int Legion::InputTextExB(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImTextureID btntexture, const char* btnlabel, const ImVec2& btnsize_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* callback_user_data)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackHistory) && (flags & LegionInputTextFlags_Multiline)));        // Can't use both together (they both use up/down keys)
    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackCompletion) && (flags & LegionInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

    LegionContext& g = *GLegion;
    LegionIO& io = g.IO;
    const LegionStyle& style = g.Style;

    const bool RENDER_SELECTION_WHEN_INACTIVE = false;
    const bool is_multiline = (flags & LegionInputTextFlags_Multiline) != 0;
    const bool is_readonly = (flags & LegionInputTextFlags_ReadOnly) != 0;
    const bool is_password = (flags & LegionInputTextFlags_Password) != 0;
    const bool is_undoable = (flags & LegionInputTextFlags_NoUndoRedo) == 0;
    const bool is_resizable = (flags & LegionInputTextFlags_CallbackResize) != 0;
    if (is_resizable)
        IM_ASSERT(callback != NULL); // Must provide a callback if you set the LegionInputTextFlags_CallbackResize flag!

    if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope,
        BeginGroup();
    const LegionID id = window->GetID(cid);
    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);
    const ImVec2 frame_size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? g.FontSize * 8.0f : label_size.y) + style.FramePadding.y * 2.0f); // Arbitrary default of 8 lines high for multi-line
    const ImVec2 total_size = ImVec2(frame_size.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_size.y);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size_arg);
    const ImRect total_bb(frame_bb.Min, frame_bb.Min + total_size);
  
    LegionWindow* draw_window = window;
    ImVec2 inner_size = frame_size;

    if (is_multiline)
    {
        if (!ItemAdd(total_bb, id, &frame_bb))
        {
            ItemSize(total_bb, style.FramePadding.y);
            EndGroup();
            return false;
        }

        // We reproduce the contents of BeginChildFrame() in order to provide 'label' so our window internal data are easier to read/debug.
        PushStyleColor(LegionCol_ChildBg, style.Colors[LegionCol_FrameBg]);
        PushStyleVar(LegionStyleVar_ChildRounding, style.FrameRounding);
        PushStyleVar(LegionStyleVar_ChildBorderSize, style.FrameBorderSize);
        PushStyleVar(LegionStyleVar_WindowPadding, style.FramePadding);
        bool child_visible = BeginChildEx(label, id, frame_bb.GetSize(), true, LegionWindowFlags_NoMove | LegionWindowFlags_AlwaysUseWindowPadding);
        PopStyleVar(3);
        PopStyleColor();
        if (!child_visible)
        {
            EndChild();
            EndGroup();
            return false;
        }
        draw_window = g.CurrentWindow; // Child window
        draw_window->DC.NavLayerActiveMaskNext |= draw_window->DC.NavLayerCurrentMask; // This is to ensure that EndChild() will display a navigation highlight so we can "enter" into it.
        inner_size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb))
            return false;
    }
    const ImRect total_bbV(frame_bb.Min + ImVec2(btnsize_arg.x, 0) , frame_bb.Max);
    const bool hovered = ItemHoverable(total_bbV, id);
    if (hovered)
        g.MouseCursor = LegionMouseCursor_TextInput;

    // We are only allowed to access the state if we are already the active widget.
    LegionInputTextState* state = GetInputTextState(id);

    const bool focus_requested = FocusableItemRegister(window, id);
    const bool focus_requested_by_code = focus_requested && (g.FocusRequestCurrWindow == window && g.FocusRequestCurrCounterRegular == window->DC.FocusCounterRegular);
    const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == LegionInputSource_NavKeyboard));
    const bool user_scroll_finish = is_multiline && state != NULL && g.ActiveId == 0 && g.ActiveIdPreviousFrame == GetWindowScrollbarID(draw_window, LegionAxis_Y);
    const bool user_scroll_active = is_multiline && state != NULL && g.ActiveId == GetWindowScrollbarID(draw_window, LegionAxis_Y);

    bool clear_active_id = false;
    bool select_all = (g.ActiveId != id) && ((flags & LegionInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);

    const bool init_make_active = (focus_requested || user_clicked || user_scroll_finish || user_nav_input_start);
    const bool init_state = (init_make_active || user_scroll_active);
    if (init_state && g.ActiveId != id)
    {
        // Access state even if we don't own it yet.
        state = &g.InputTextState;
        state->CursorAnimReset();

        // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
        // From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
        const int buf_len = (int)strlen(buf);
        state->InitialTextA.resize(buf_len + 1);    // UTF-8. we use +1 to make sure that .Data is always pointing to at least an empty string.
        memcpy(state->InitialTextA.Data, buf, buf_len + 1);

        // Start edition
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);          // wchar count <= UTF-8 count. we use +1 to make sure that .Data is always pointing to at least an empty string.
        state->TextA.resize(0);
        state->TextAIsValid = false;                // TextA is not valid yet (we will display buf until then)
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, buf_size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);      // We can't get the result from ImStrncpy() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.

        // Preserve cursor position and undo/redo stack if we come back to same widget
        // FIXME: For non-readonly widgets we might be able to require that TextAIsValid && TextA == buf ? (untested) and discard undo stack if user buffer has changed.
        const bool recycle_state = (state->ID == id);
        if (recycle_state)
        {
            // Recycle existing cursor/selection/undo stack but clamp position
            // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
            state->CursorClamp();
        }
        else
        {
            state->ID = id;
            state->ScrollX = 0.0f;
            stb_textedit_initialize_state(&state->Stb, !is_multiline);
            if (!is_multiline && focus_requested_by_code)
                select_all = true;
        }
        if (flags & LegionInputTextFlags_AlwaysInsertMode)
            state->Stb.insert_mode = 1;
        if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
            select_all = true;
    }

    if (g.ActiveId != id && init_make_active)
    {
        IM_ASSERT(state && state->ID == id);
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);

        // Declare our inputs
        IM_ASSERT(LegionNavInput_COUNT < 32);
        g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Left) | (1 << LegionDir_Right);
        if (is_multiline || (flags & LegionInputTextFlags_CallbackHistory))
            g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Up) | (1 << LegionDir_Down);
        g.ActiveIdUsingNavInputMask |= (1 << LegionNavInput_Cancel);
        g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Home) | ((ImU64)1 << LegionKey_End);
        if (is_multiline)
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_PageUp) | ((ImU64)1 << LegionKey_PageDown); // FIXME-NAV: Page up/down actually not supported yet by widget, but claim them ahead.
        if (flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_AllowTabInput))  // Disable keyboard tabbing out as we will use the \t character.
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Tab);
    }

    // We have an edge case if ActiveId was set through another widget (e.g. widget being swapped), clear id immediately (don't wait until the end of the function)
    if (g.ActiveId == id && state == NULL)
        ClearActiveID();

    // Release focus when we click outside
    if (g.ActiveId == id && io.MouseClicked[0] && !init_state && !init_make_active) //-V560
        clear_active_id = true;

    // Lock the decision of whether we are going to take the path displaying the cursor or selection
    const bool render_cursor = (g.ActiveId == id) || (state && user_scroll_active);
    bool render_selection = state && state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    bool value_changed = false;
    bool enter_pressed = false;

    // When read-only we always use the live data passed to the function
    // FIXME-OPT: Because our selection/cursor code currently needs the wide text we need to convert it when active, which is not ideal :(
    if (is_readonly && state != NULL && (render_cursor || render_selection))
    {
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);
        state->CursorClamp();
        render_selection &= state->HasSelection();
    }

    // Select the buffer to render.
    const bool buf_display_from_state = (render_cursor || render_selection || g.ActiveId == id) && !is_readonly && state && state->TextAIsValid;
    const bool is_displaying_hint = (hint != NULL && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);

    // Password pushes a temporary font with only a fallback glyph
    if (is_password && !is_displaying_hint)
    {
        const ImFontGlyph* glyph = g.Font->FindGlyph('*');
        ImFont* password_font = &g.InputTextPasswordFont;
        password_font->FontSize = g.Font->FontSize;
        password_font->Scale = g.Font->Scale;
        password_font->DisplayOffset = g.Font->DisplayOffset;
        password_font->Ascent = g.Font->Ascent;
        password_font->Descent = g.Font->Descent;
        password_font->ContainerAtlas = g.Font->ContainerAtlas;
        password_font->FallbackGlyph = glyph;
        password_font->FallbackAdvanceX = glyph->AdvanceX;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // Process mouse inputs and character inputs
    int backup_current_text_length = 0;
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        backup_current_text_length = state->CurLenA;
        state->BufCapacityA = buf_size;
        state->UserFlags = flags;
        state->UserCallback = callback;
        state->UserCallbackData = callback_user_data;

        // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];
        g.WantTextInputNextFrame = 1;

        // Edit in progress
        const float mouse_x = (io.MousePos.x - btnsize_arg.x - frame_bb.Min.x - style.FramePadding.x) + state->ScrollX ;
        const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize * 0.5f));

        const bool is_osx = io.ConfigMacOSXBehaviors;
        if (select_all || (hovered && !is_osx && io.MouseDoubleClicked[0]))
        {
            state->SelectAll();
            state->SelectedAllMouseLock = true;
        }
        else if (hovered && is_osx && io.MouseDoubleClicked[0])
        {
            // Double-click select a word only, OS X style (by simulating keystrokes)
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
        }
        else if (io.MouseClicked[0] && !state->SelectedAllMouseLock)
        {
            if (hovered)
            {
                stb_textedit_click(state, &state->Stb, mouse_x, mouse_y);
                state->CursorAnimReset();
            }
        }
        else if (io.MouseDown[0] && !state->SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(state, &state->Stb, mouse_x, mouse_y);
            state->CursorAnimReset();
            state->CursorFollow = true;
        }
        if (state->SelectedAllMouseLock && !io.MouseDown[0])
            state->SelectedAllMouseLock = false;

        // It is ill-defined whether the back-end needs to send a \t character when pressing the TAB keys.
        // Win32 and GLFW naturally do it but not SDL.
        const bool ignore_char_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
        if ((flags & LegionInputTextFlags_AllowTabInput) && IsKeyPressedMap(LegionKey_Tab) && !ignore_char_inputs && !io.KeyShift && !is_readonly)
            if (!io.InputQueueCharacters.contains('\t'))
            {
                unsigned int c = '\t'; // Insert TAB
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }

        // Process regular text input (before we check for Return because using some IME will effectively send a Return?)
        // We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
        if (io.InputQueueCharacters.Size > 0)
        {
            if (!ignore_char_inputs && !is_readonly && !user_nav_input_start)
                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    // Insert character if they pass filtering
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c == '\t' && io.KeyShift)
                        continue;
                    if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        state->OnKeyPressed((int)c);
                }

            // Consume characters
            io.InputQueueCharacters.resize(0);
        }
    }

    // Process other shortcuts/key-presses
    bool cancel_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        IM_ASSERT(state != NULL);
        IM_ASSERT(io.KeyMods == GetMergedKeyModFlags() && "Mismatching io.KeyCtrl/io.KeyShift/io.KeyAlt/io.KeySuper vs io.KeyMods"); // We rarely do this check, but if anything let's do it here.

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_osx = io.ConfigMacOSXBehaviors;
        const bool is_osx_shift_shortcut = is_osx && (io.KeyMods == (LegionKeyModFlags_Super | LegionKeyModFlags_Shift));
        const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
        const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
        const bool is_ctrl_key_only = (io.KeyMods == LegionKeyModFlags_Ctrl);
        const bool is_shift_key_only = (io.KeyMods == LegionKeyModFlags_Shift);
        const bool is_shortcut_key = g.IO.ConfigMacOSXBehaviors ? (io.KeyMods == LegionKeyModFlags_Super) : (io.KeyMods == LegionKeyModFlags_Ctrl);

        const bool is_cut = ((is_shortcut_key && IsKeyPressedMap(LegionKey_X)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Delete))) && !is_readonly && !is_password && (!is_multiline || state->HasSelection());
        const bool is_copy = ((is_shortcut_key && IsKeyPressedMap(LegionKey_C)) || (is_ctrl_key_only && IsKeyPressedMap(LegionKey_Insert))) && !is_password && (!is_multiline || state->HasSelection());
        const bool is_paste = ((is_shortcut_key && IsKeyPressedMap(LegionKey_V)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Insert))) && !is_readonly;
        const bool is_undo = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Z)) && !is_readonly && is_undoable);
        const bool is_redo = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Y)) || (is_osx_shift_shortcut && IsKeyPressedMap(LegionKey_Z))) && !is_readonly && is_undoable;

        if (IsKeyPressedMap(LegionKey_LeftArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_RightArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_UpArrow) && is_multiline) { if (io.KeyCtrl) SetScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_DownArrow) && is_multiline) { if (io.KeyCtrl) SetScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Home)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
        else if (IsKeyPressedMap(LegionKey_End)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Delete) && !is_readonly) { state->OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Backspace) && !is_readonly)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
                else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
                    state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (IsKeyPressedMap(LegionKey_Enter) || IsKeyPressedMap(LegionKey_KeyPadEnter))
        {
            bool ctrl_enter_for_new_line = (flags & LegionInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                enter_pressed = clear_active_id = true;
            }
            else if (!is_readonly)
            {
                unsigned int c = '\n'; // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }
        }
        else if (IsKeyPressedMap(LegionKey_Escape))
        {
            clear_active_id = cancel_edit = true;
        }
        else if (is_undo || is_redo)
        {
            state->OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
            state->ClearSelection();
        }
        else if (is_shortcut_key && IsKeyPressedMap(LegionKey_A))
        {
            state->SelectAll();
            state->CursorFollow = true;
        }
        else if (is_cut || is_copy)
        {
            // Cut, Copy
            if (io.SetClipboardTextFn)
            {
                const int ib = state->HasSelection() ? ImMin(state->Stb.select_start, state->Stb.select_end) : 0;
                const int ie = state->HasSelection() ? ImMax(state->Stb.select_start, state->Stb.select_end) : state->CurLenW;
                const int clipboard_data_len = ImTextCountUtf8BytesFromStr(state->TextW.Data + ib, state->TextW.Data + ie) + 1;
                char* clipboard_data = (char*)IM_ALLOC(clipboard_data_len * sizeof(char));
                ImTextStrToUtf8(clipboard_data, clipboard_data_len, state->TextW.Data + ib, state->TextW.Data + ie);
                SetClipboardText(clipboard_data);
                MemFree(clipboard_data);
            }
            if (is_cut)
            {
                if (!state->HasSelection())
                    state->SelectAll();
                state->CursorFollow = true;
                stb_textedit_cut(state, &state->Stb);
            }
        }
        else if (is_paste)
        {
            if (const char* clipboard = GetClipboardText())
            {
                // Filter pasted buffer
                const int clipboard_len = (int)strlen(clipboard);
                ImWchar* clipboard_filtered = (ImWchar*)IM_ALLOC((clipboard_len + 1) * sizeof(ImWchar));
                int clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s; )
                {
                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0)
                        break;
                    if (!InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(state, &state->Stb, clipboard_filtered, clipboard_filtered_len);
                    state->CursorFollow = true;
                }
                MemFree(clipboard_filtered);
            }
        }

        // Update render selection flag after events have been handled, so selection highlight can be displayed during the same frame.
        render_selection |= state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    }

    // Process callbacks and apply result back to user's buffer.
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        const char* apply_new_text = NULL;
        int apply_new_text_length = 0;
        if (cancel_edit)
        {
            // Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
            if (!is_readonly && strcmp(buf, state->InitialTextA.Data) != 0)
            {
                // Push records into the undo stack so we can CTRL+Z the revert operation itself
                apply_new_text = state->InitialTextA.Data;
                apply_new_text_length = state->InitialTextA.Size - 1;
                ImVector<ImWchar> w_text;
                if (apply_new_text_length > 0)
                {
                    w_text.resize(ImTextCountCharsFromUtf8(apply_new_text, apply_new_text + apply_new_text_length) + 1);
                    ImTextStrFromUtf8(w_text.Data, w_text.Size, apply_new_text, apply_new_text + apply_new_text_length);
                }
                stb_textedit_replace(state, &state->Stb, w_text.Data, (apply_new_text_length > 0) ? (w_text.Size - 1) : 0);
            }
        }

        // When using 'LegionInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
        // If we didn't do that, code like InputInt() with LegionInputTextFlags_EnterReturnsTrue would fail.
        // This also allows the user to use InputText() with LegionInputTextFlags_EnterReturnsTrue without maintaining any user-side storage (please note that if you use this property along LegionInputTextFlags_CallbackResize you can end up with your temporary string object unnecessarily allocating once a frame, either store your string data, either if you don't then don't use LegionInputTextFlags_CallbackResize).
        bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & LegionInputTextFlags_EnterReturnsTrue) != 0);
        if (apply_edit_back_to_user_buffer)
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
            if (!is_readonly)
            {
                state->TextAIsValid = true;
                state->TextA.resize(state->TextW.Size * 4 + 1);
                ImTextStrToUtf8(state->TextA.Data, state->TextA.Size, state->TextW.Data, NULL);
            }

            // User callback
            if ((flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_CallbackHistory | LegionInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                LegionInputTextFlags event_flag = 0;
                LegionKey event_key = LegionKey_COUNT;
                if ((flags & LegionInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(LegionKey_Tab))
                {
                    event_flag = LegionInputTextFlags_CallbackCompletion;
                    event_key = LegionKey_Tab;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_UpArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_UpArrow;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_DownArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_DownArrow;
                }
                else if (flags & LegionInputTextFlags_CallbackAlways)
                    event_flag = LegionInputTextFlags_CallbackAlways;

                if (event_flag)
                {
                    LegionInputTextCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(LegionInputTextCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = callback_user_data;

                    callback_data.EventKey = event_key;
                    callback_data.Buf = state->TextA.Data;
                    callback_data.BufTextLen = state->CurLenA;
                    callback_data.BufSize = state->BufCapacityA;
                    callback_data.BufDirty = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
                    ImWchar* text = state->TextW.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + state->Stb.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    IM_ASSERT(callback_data.Buf == state->TextA.Data);  // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == state->BufCapacityA);
                    IM_ASSERT(callback_data.Flags == flags);
                    if (callback_data.CursorPos != utf8_cursor_pos) { state->Stb.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos); state->CursorFollow = true; }
                    if (callback_data.SelectionStart != utf8_selection_start) { state->Stb.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart); }
                    if (callback_data.SelectionEnd != utf8_selection_end) { state->Stb.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd); }
                    if (callback_data.BufDirty)
                    {
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
                        if (callback_data.BufTextLen > backup_current_text_length&& is_resizable)
                            state->TextW.resize(state->TextW.Size + (callback_data.BufTextLen - backup_current_text_length));
                        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, callback_data.Buf, NULL);
                        state->CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
                        state->CursorAnimReset();
                    }
                }
            }

            // Will copy result string if modified
            if (!is_readonly && strcmp(state->TextA.Data, buf) != 0)
            {
                apply_new_text = state->TextA.Data;
                apply_new_text_length = state->CurLenA;
            }
        }

        // Copy result to user buffer
        if (apply_new_text)
        {
            // We cannot test for 'backup_current_text_length != apply_new_text_length' here because we have no guarantee that the size
            // of our owned buffer matches the size of the string object held by the user, and by design we allow InputText() to be used
            // without any storage on user's side.
            IM_ASSERT(apply_new_text_length >= 0);
            if (is_resizable)
            {
                LegionInputTextCallbackData callback_data;
                callback_data.EventFlag = LegionInputTextFlags_CallbackResize;
                callback_data.Flags = flags;
                callback_data.Buf = buf;
                callback_data.BufTextLen = apply_new_text_length;
                callback_data.BufSize = ImMax(buf_size, apply_new_text_length + 1);
                callback_data.UserData = callback_user_data;
                callback(&callback_data);
                buf = callback_data.Buf;
                buf_size = callback_data.BufSize;
                apply_new_text_length = ImMin(callback_data.BufTextLen, buf_size - 1);
                IM_ASSERT(apply_new_text_length <= buf_size);
            }
            //Legion_DEBUG_LOG("InputText(\"%s\"): apply_new_text length %d\n", label, apply_new_text_length);

            // If the underlying buffer resize was denied or not carried to the next frame, apply_new_text_length+1 may be >= buf_size.
            ImStrncpy(buf, apply_new_text, ImMin(apply_new_text_length + 1, buf_size));
            value_changed = true;
        }

        // Clear temporary user storage
        state->UserFlags = 0;
        state->UserCallback = NULL;
        state->UserCallbackData = NULL;
    }

    // Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
    if (clear_active_id && g.ActiveId == id)
        ClearActiveID();

    // Render frame
    if (!is_multiline)
    {
        RenderNavHighlight(frame_bb, id);

        if (texture != NULL)
            window->DrawList->AddImage(texture, frame_bb.Min + ImVec2(-6, -6), frame_bb.Max + ImVec2(0, 9), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + inner_size.x, frame_bb.Min.y + inner_size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 draw_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.0f, 0.0f);

    // Set upper limit of single-line InputTextEx() at 2 million characters strings. The current pathological worst case is a long line
    // without any carriage return, which would makes ImFont::RenderText() reserve too many vertices and probably crash. Avoid it altogether.
    // Note that we only use this limit on single-line InputText(), so a pathologically large line on a InputTextMultiline() would still crash.
    const int buf_display_max_length = 2 * 1024 * 1024;
    const char* buf_display = buf_display_from_state ? state->TextA.Data : buf; //-V595
    const char* buf_display_end = NULL; // We have specialized paths below for setting the length
    if (is_displaying_hint)
    {
        buf_display = hint;
        buf_display_end = hint + strlen(hint);
    }

    // Render text. We currently only render selection when the widget is active or while scrolling.
    // FIXME: We could remove the '&& render_cursor' to keep rendering selection when inactive.
    if (render_cursor || render_selection)
    {
        IM_ASSERT(state != NULL);
        if (!is_displaying_hint)
            buf_display_end = buf_display + state->CurLenA;

        // Render text (with cursor and selection)
        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = state->TextW.Data;
        ImVec2 cursor_offset, select_start_offset;

        {
            // Find lines numbers straddling 'cursor' (slot 0) and 'select_start' (slot 1) positions.
            const ImWchar* searches_input_ptr[2] = { NULL, NULL };
            int searches_result_line_no[2] = { -1000, -1000 };
            int searches_remaining = 0;
            if (render_cursor)
            {
                searches_input_ptr[0] = text_begin + state->Stb.cursor;
                searches_result_line_no[0] = -1;
                searches_remaining++;
            }
            if (render_selection)
            {
                searches_input_ptr[1] = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
                searches_result_line_no[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            //for (const ImWchar* s = text_begin; (s = (const ImWchar*)wcschr((const wchar_t*)s, (wchar_t)'\n')) != NULL; s++)  // FIXME-OPT: Could use this when wchar_t are 16-bit
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n')
                {
                    line_count++;
                    if (searches_result_line_no[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_no[0] = line_count; if (--searches_remaining <= 0) break; }
                    if (searches_result_line_no[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_no[1] = line_count; if (--searches_remaining <= 0) break; }
                }
            line_count++;
            if (searches_result_line_no[0] == -1)
                searches_result_line_no[0] = line_count;
            if (searches_result_line_no[1] == -1)
                searches_result_line_no[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and measuring distance
            cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x + btnsize_arg.x;
            cursor_offset.y = searches_result_line_no[0] * g.FontSize;
            if (searches_result_line_no[1] >= 0)
            {
                select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x + btnsize_arg.x;
                select_start_offset.y = searches_result_line_no[1] * g.FontSize;
            }

            // Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
            if (is_multiline)
                text_size = ImVec2(inner_size.x, line_count * g.FontSize);
        }

        // Scroll
        if (render_cursor && state->CursorFollow)
        {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & LegionInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = inner_size.x * 0.25f;
                if (cursor_offset.x < state->ScrollX)
                    state->ScrollX = IM_FLOOR(ImMax(0.0f, cursor_offset.x - scroll_increment_x));
                else if (cursor_offset.x - inner_size.x >= state->ScrollX)
                    state->ScrollX = IM_FLOOR(cursor_offset.x - inner_size.x + scroll_increment_x);
            }
            else
            {
                state->ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline)
            {
                float scroll_y = draw_window->Scroll.y;
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - inner_size.y >= scroll_y)
                    scroll_y = cursor_offset.y - inner_size.y;
                draw_pos.y += (draw_window->Scroll.y - scroll_y);   // Manipulate cursor pos immediately avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
            }

            state->CursorFollow = false;
        }

        // Draw selection
        const ImVec2 draw_scroll = ImVec2(state->ScrollX, 0.0f);
        if (render_selection)
        {
            const ImWchar* text_selected_begin = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
            const ImWchar* text_selected_end = text_begin + ImMax(state->Stb.select_start, state->Stb.select_end);

            ImU32 bg_color = GetColorU32(LegionCol_TextSelectedBg, render_cursor ? 1.0f : 0.6f); // FIXME: current code flow mandate that render_cursor is always true here, we are leaving the transparent one for tests.
            float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImVec2 rect_pos = draw_pos + select_start_offset - draw_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    //p = (const ImWchar*)wmemchr((const wchar_t*)p, '\n', text_selected_end - p);  // FIXME-OPT: Could use this when wchar_t are 16-bit
                    //p = p ? p + 1 : text_selected_end;
                    while (p < text_selected_end)
                        if (*p++ == '\n')
                            break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f) rect_size.x = IM_FLOOR(g.Font->GetCharAdvance((ImWchar)' ') * 0.50f); // So we can see selected empty lines
                    ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = draw_pos.x - draw_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        // We test for 'buf_display_max_length' as a way to avoid some pathological cases (e.g. single-line 1 MB string) which would make ImDrawList crash.
        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, ImVec2(draw_pos.x + btnsize_arg.x, draw_pos.y) - draw_scroll, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }

        // Draw blinking cursor
        if (render_cursor)
        {
            state->CursorAnim += io.DeltaTime;
            bool cursor_is_visible = (!g.IO.ConfigInputTextCursorBlink) || (state->CursorAnim <= 0.0f) || ImFmod(state->CursorAnim, 1.20f) <= 0.80f;
            ImVec2 cursor_screen_pos = draw_pos + cursor_offset - draw_scroll;
            ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
            if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
                draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(LegionCol_Text));

            // Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
            if (!is_readonly)
                g.PlatformImePos = ImVec2(cursor_screen_pos.x - 1.0f, cursor_screen_pos.y - g.FontSize);
        }
    }
    else
    {
        // Render text only (no selection, no cursor)
        if (is_multiline)
            text_size = ImVec2(inner_size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_display_end) * g.FontSize); // We don't need width
        else if (!is_displaying_hint && g.ActiveId == id)
            buf_display_end = buf_display + state->CurLenA;
        else if (!is_displaying_hint)
            buf_display_end = buf_display + strlen(buf_display);

        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, ImVec2(draw_pos.x + btnsize_arg.x, draw_pos.y), col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }
    }

    if (is_multiline)
    {
        Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
        EndChild();
        EndGroup();
    }

    if (is_password && !is_displaying_hint)
        PopFont();

    // Log as text
    if (g.LogEnabled && !(is_password && !is_displaying_hint))
        LogRenderedText(&draw_pos, buf_display, buf_display_end);

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if (value_changed && !(flags & LegionInputTextFlags_NoMarkEdited))
        MarkItemEdited(id);

    IndentEx(-2, - 27.45f);
    if (ImageButtonEx("TextButton", btnlabel, true, btntexture, btntexture, btntexture, btnsize_arg, font, font_size))
        return 3;

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    if ((flags & LegionInputTextFlags_EnterReturnsTrue) != 0 && enter_pressed)
        return 2;
    else
        return value_changed;
}

bool Legion::InputTextExV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* callback_user_data)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackHistory) && (flags & LegionInputTextFlags_Multiline)));        // Can't use both together (they both use up/down keys)
    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackCompletion) && (flags & LegionInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

    LegionContext& g = *GLegion;
    LegionIO& io = g.IO;
    const LegionStyle& style = g.Style;

    const bool RENDER_SELECTION_WHEN_INACTIVE = false;
    const bool is_multiline = (flags & LegionInputTextFlags_Multiline) != 0;
    const bool is_readonly = (flags & LegionInputTextFlags_ReadOnly) != 0;
    const bool is_password = (flags & LegionInputTextFlags_Password) != 0;
    const bool is_undoable = (flags & LegionInputTextFlags_NoUndoRedo) == 0;
    const bool is_resizable = (flags & LegionInputTextFlags_CallbackResize) != 0;
    if (is_resizable)
        IM_ASSERT(callback != NULL); // Must provide a callback if you set the LegionInputTextFlags_CallbackResize flag!

    if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope,
        BeginGroup();
    const LegionID id = window->GetID(cid);
    const ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);
    const ImVec2 frame_size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? g.FontSize * 8.0f : label_size.y) + style.FramePadding.y*2.0f); // Arbitrary default of 8 lines high for multi-line
    const ImVec2 total_size = ImVec2(frame_size.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_size.y);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size_arg);
    const ImRect total_bb(frame_bb.Min, frame_bb.Min + total_size);

    LegionWindow* draw_window = window;
    ImVec2 inner_size = frame_size;
    if (is_multiline)
    {
        if (!ItemAdd(total_bb, id, &frame_bb))
        {
            ItemSize(total_bb, style.FramePadding.y);
            EndGroup();
            return false;
        }

        // We reproduce the contents of BeginChildFrame() in order to provide 'label' so our window internal data are easier to read/debug.
        PushStyleColor(LegionCol_ChildBg, style.Colors[LegionCol_FrameBg]);
        PushStyleVar(LegionStyleVar_ChildRounding, style.FrameRounding);
        PushStyleVar(LegionStyleVar_ChildBorderSize, style.FrameBorderSize);
        PushStyleVar(LegionStyleVar_WindowPadding, style.FramePadding);
        bool child_visible = BeginChildEx(label, id, frame_bb.GetSize(), true, LegionWindowFlags_NoMove | LegionWindowFlags_AlwaysUseWindowPadding);
        PopStyleVar(3);
        PopStyleColor();
        if (!child_visible)
        {
            EndChild();
            EndGroup();
            return false;
        }
        draw_window = g.CurrentWindow; // Child window
        draw_window->DC.NavLayerActiveMaskNext |= draw_window->DC.NavLayerCurrentMask; // This is to ensure that EndChild() will display a navigation highlight so we can "enter" into it.
        inner_size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb))
            return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        g.MouseCursor = LegionMouseCursor_TextInput;

    // We are only allowed to access the state if we are already the active widget.
    LegionInputTextState* state = GetInputTextState(id);

    const bool focus_requested = FocusableItemRegister(window, id);
    const bool focus_requested_by_code = focus_requested && (g.FocusRequestCurrWindow == window && g.FocusRequestCurrCounterRegular == window->DC.FocusCounterRegular);
    const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == LegionInputSource_NavKeyboard));
    const bool user_scroll_finish = is_multiline && state != NULL && g.ActiveId == 0 && g.ActiveIdPreviousFrame == GetWindowScrollbarID(draw_window, LegionAxis_Y);
    const bool user_scroll_active = is_multiline && state != NULL && g.ActiveId == GetWindowScrollbarID(draw_window, LegionAxis_Y);

    bool clear_active_id = false;
    bool select_all = (g.ActiveId != id) && ((flags & LegionInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);

    const bool init_make_active = (focus_requested || user_clicked || user_scroll_finish || user_nav_input_start);
    const bool init_state = (init_make_active || user_scroll_active);
    if (init_state && g.ActiveId != id)
    {
        // Access state even if we don't own it yet.
        state = &g.InputTextState;
        state->CursorAnimReset();

        // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
        // From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
        const int buf_len = (int)strlen(buf);
        state->InitialTextA.resize(buf_len + 1);    // UTF-8. we use +1 to make sure that .Data is always pointing to at least an empty string.
        memcpy(state->InitialTextA.Data, buf, buf_len + 1);

        // Start edition
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);          // wchar count <= UTF-8 count. we use +1 to make sure that .Data is always pointing to at least an empty string.
        state->TextA.resize(0);
        state->TextAIsValid = false;                // TextA is not valid yet (we will display buf until then)
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, buf_size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);      // We can't get the result from ImStrncpy() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.

        // Preserve cursor position and undo/redo stack if we come back to same widget
        // FIXME: For non-readonly widgets we might be able to require that TextAIsValid && TextA == buf ? (untested) and discard undo stack if user buffer has changed.
        const bool recycle_state = (state->ID == id);
        if (recycle_state)
        {
            // Recycle existing cursor/selection/undo stack but clamp position
            // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
            state->CursorClamp();
        }
        else
        {
            state->ID = id;
            state->ScrollX = 0.0f;
            stb_textedit_initialize_state(&state->Stb, !is_multiline);
            if (!is_multiline && focus_requested_by_code)
                select_all = true;
        }
        if (flags & LegionInputTextFlags_AlwaysInsertMode)
            state->Stb.insert_mode = 1;
        if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
            select_all = true;
    }

    if (g.ActiveId != id && init_make_active)
    {
        IM_ASSERT(state && state->ID == id);
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);

        // Declare our inputs
        IM_ASSERT(LegionNavInput_COUNT < 32);
        g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Left) | (1 << LegionDir_Right);
        if (is_multiline || (flags & LegionInputTextFlags_CallbackHistory))
            g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Up) | (1 << LegionDir_Down);
        g.ActiveIdUsingNavInputMask |= (1 << LegionNavInput_Cancel);
        g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Home) | ((ImU64)1 << LegionKey_End);
        if (is_multiline)
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_PageUp) | ((ImU64)1 << LegionKey_PageDown); // FIXME-NAV: Page up/down actually not supported yet by widget, but claim them ahead.
        if (flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_AllowTabInput))  // Disable keyboard tabbing out as we will use the \t character.
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Tab);
    }

    // We have an edge case if ActiveId was set through another widget (e.g. widget being swapped), clear id immediately (don't wait until the end of the function)
    if (g.ActiveId == id && state == NULL)
        ClearActiveID();

    // Release focus when we click outside
    if (g.ActiveId == id && io.MouseClicked[0] && !init_state && !init_make_active) //-V560
        clear_active_id = true;

    // Lock the decision of whether we are going to take the path displaying the cursor or selection
    const bool render_cursor = (g.ActiveId == id) || (state && user_scroll_active);
    bool render_selection = state && state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    bool value_changed = false;
    bool enter_pressed = false;

    // When read-only we always use the live data passed to the function
    // FIXME-OPT: Because our selection/cursor code currently needs the wide text we need to convert it when active, which is not ideal :(
    if (is_readonly && state != NULL && (render_cursor || render_selection))
    {
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);
        state->CursorClamp();
        render_selection &= state->HasSelection();
    }

    // Select the buffer to render.
    const bool buf_display_from_state = (render_cursor || render_selection || g.ActiveId == id) && !is_readonly && state && state->TextAIsValid;
    const bool is_displaying_hint = (hint != NULL && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);

    // Password pushes a temporary font with only a fallback glyph
    if (is_password && !is_displaying_hint)
    {
        const ImFontGlyph* glyph = g.Font->FindGlyph('*');
        ImFont* password_font = &g.InputTextPasswordFont;
        password_font->FontSize = g.Font->FontSize;
        password_font->Scale = g.Font->Scale;
        password_font->DisplayOffset = g.Font->DisplayOffset;
        password_font->Ascent = g.Font->Ascent;
        password_font->Descent = g.Font->Descent;
        password_font->ContainerAtlas = g.Font->ContainerAtlas;
        password_font->FallbackGlyph = glyph;
        password_font->FallbackAdvanceX = glyph->AdvanceX;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // Process mouse inputs and character inputs
    int backup_current_text_length = 0;
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        backup_current_text_length = state->CurLenA;
        state->BufCapacityA = buf_size;
        state->UserFlags = flags;
        state->UserCallback = callback;
        state->UserCallbackData = callback_user_data;

        // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];
        g.WantTextInputNextFrame = 1;

        // Edit in progress
        const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + state->ScrollX;
        const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize*0.5f));

        const bool is_osx = io.ConfigMacOSXBehaviors;
        if (select_all || (hovered && !is_osx && io.MouseDoubleClicked[0]))
        {
            state->SelectAll();
            state->SelectedAllMouseLock = true;
        }
        else if (hovered && is_osx && io.MouseDoubleClicked[0])
        {
            // Double-click select a word only, OS X style (by simulating keystrokes)
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
        }
        else if (io.MouseClicked[0] && !state->SelectedAllMouseLock)
        {
            if (hovered)
            {
                stb_textedit_click(state, &state->Stb, mouse_x, mouse_y);
                state->CursorAnimReset();
            }
        }
        else if (io.MouseDown[0] && !state->SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(state, &state->Stb, mouse_x, mouse_y);
            state->CursorAnimReset();
            state->CursorFollow = true;
        }
        if (state->SelectedAllMouseLock && !io.MouseDown[0])
            state->SelectedAllMouseLock = false;

        // It is ill-defined whether the back-end needs to send a \t character when pressing the TAB keys.
        // Win32 and GLFW naturally do it but not SDL.
        const bool ignore_char_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
        if ((flags & LegionInputTextFlags_AllowTabInput) && IsKeyPressedMap(LegionKey_Tab) && !ignore_char_inputs && !io.KeyShift && !is_readonly)
            if (!io.InputQueueCharacters.contains('\t'))
            {
                unsigned int c = '\t'; // Insert TAB
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }

        // Process regular text input (before we check for Return because using some IME will effectively send a Return?)
        // We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
        if (io.InputQueueCharacters.Size > 0)
        {
            if (!ignore_char_inputs && !is_readonly && !user_nav_input_start)
                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    // Insert character if they pass filtering
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c == '\t' && io.KeyShift)
                        continue;
                    if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        state->OnKeyPressed((int)c);
                }

            // Consume characters
            io.InputQueueCharacters.resize(0);
        }
    }

    // Process other shortcuts/key-presses
    bool cancel_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        IM_ASSERT(state != NULL);
        IM_ASSERT(io.KeyMods == GetMergedKeyModFlags() && "Mismatching io.KeyCtrl/io.KeyShift/io.KeyAlt/io.KeySuper vs io.KeyMods"); // We rarely do this check, but if anything let's do it here.

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_osx = io.ConfigMacOSXBehaviors;
        const bool is_osx_shift_shortcut = is_osx && (io.KeyMods == (LegionKeyModFlags_Super | LegionKeyModFlags_Shift));
        const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
        const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
        const bool is_ctrl_key_only = (io.KeyMods == LegionKeyModFlags_Ctrl);
        const bool is_shift_key_only = (io.KeyMods == LegionKeyModFlags_Shift);
        const bool is_shortcut_key = g.IO.ConfigMacOSXBehaviors ? (io.KeyMods == LegionKeyModFlags_Super) : (io.KeyMods == LegionKeyModFlags_Ctrl);

        const bool is_cut   = ((is_shortcut_key && IsKeyPressedMap(LegionKey_X)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Delete))) && !is_readonly && !is_password && (!is_multiline || state->HasSelection());
        const bool is_copy  = ((is_shortcut_key && IsKeyPressedMap(LegionKey_C)) || (is_ctrl_key_only  && IsKeyPressedMap(LegionKey_Insert))) && !is_password && (!is_multiline || state->HasSelection());
        const bool is_paste = ((is_shortcut_key && IsKeyPressedMap(LegionKey_V)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Insert))) && !is_readonly;
        const bool is_undo  = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Z)) && !is_readonly && is_undoable);
        const bool is_redo  = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Y)) || (is_osx_shift_shortcut && IsKeyPressedMap(LegionKey_Z))) && !is_readonly && is_undoable;

        if (IsKeyPressedMap(LegionKey_LeftArrow))                        { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_RightArrow))                  { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_UpArrow) && is_multiline)     { if (io.KeyCtrl) SetScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_DownArrow) && is_multiline)   { if (io.KeyCtrl) SetScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Home))                        { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
        else if (IsKeyPressedMap(LegionKey_End))                         { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Delete) && !is_readonly)      { state->OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Backspace) && !is_readonly)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT|STB_TEXTEDIT_K_SHIFT);
                else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
                    state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART|STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (IsKeyPressedMap(LegionKey_Enter) || IsKeyPressedMap(LegionKey_KeyPadEnter))
        {
            bool ctrl_enter_for_new_line = (flags & LegionInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                enter_pressed = clear_active_id = true;
            }
            else if (!is_readonly)
            {
                unsigned int c = '\n'; // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }
        }
        else if (IsKeyPressedMap(LegionKey_Escape))
        {
            clear_active_id = cancel_edit = true;
        }
        else if (is_undo || is_redo)
        {
            state->OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
            state->ClearSelection();
        }
        else if (is_shortcut_key && IsKeyPressedMap(LegionKey_A))
        {
            state->SelectAll();
            state->CursorFollow = true;
        }
        else if (is_cut || is_copy)
        {
            // Cut, Copy
            if (io.SetClipboardTextFn)
            {
                const int ib = state->HasSelection() ? ImMin(state->Stb.select_start, state->Stb.select_end) : 0;
                const int ie = state->HasSelection() ? ImMax(state->Stb.select_start, state->Stb.select_end) : state->CurLenW;
                const int clipboard_data_len = ImTextCountUtf8BytesFromStr(state->TextW.Data + ib, state->TextW.Data + ie) + 1;
                char* clipboard_data = (char*)IM_ALLOC(clipboard_data_len * sizeof(char));
                ImTextStrToUtf8(clipboard_data, clipboard_data_len, state->TextW.Data + ib, state->TextW.Data + ie);
                SetClipboardText(clipboard_data);
                MemFree(clipboard_data);
            }
            if (is_cut)
            {
                if (!state->HasSelection())
                    state->SelectAll();
                state->CursorFollow = true;
                stb_textedit_cut(state, &state->Stb);
            }
        }
        else if (is_paste)
        {
            if (const char* clipboard = GetClipboardText())
            {
                // Filter pasted buffer
                const int clipboard_len = (int)strlen(clipboard);
                ImWchar* clipboard_filtered = (ImWchar*)IM_ALLOC((clipboard_len+1) * sizeof(ImWchar));
                int clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s; )
                {
                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0)
                        break;
                    if (!InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(state, &state->Stb, clipboard_filtered, clipboard_filtered_len);
                    state->CursorFollow = true;
                }
                MemFree(clipboard_filtered);
            }
        }

        // Update render selection flag after events have been handled, so selection highlight can be displayed during the same frame.
        render_selection |= state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    }

    // Process callbacks and apply result back to user's buffer.
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        const char* apply_new_text = NULL;
        int apply_new_text_length = 0;
        if (cancel_edit)
        {
            // Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
            if (!is_readonly && strcmp(buf, state->InitialTextA.Data) != 0)
            {
                // Push records into the undo stack so we can CTRL+Z the revert operation itself
                apply_new_text = state->InitialTextA.Data;
                apply_new_text_length = state->InitialTextA.Size - 1;
                ImVector<ImWchar> w_text;
                if (apply_new_text_length > 0)
                {
                    w_text.resize(ImTextCountCharsFromUtf8(apply_new_text, apply_new_text + apply_new_text_length) + 1);
                    ImTextStrFromUtf8(w_text.Data, w_text.Size, apply_new_text, apply_new_text + apply_new_text_length);
                }
                stb_textedit_replace(state, &state->Stb, w_text.Data, (apply_new_text_length > 0) ? (w_text.Size - 1) : 0);
            }
        }

        // When using 'LegionInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
        // If we didn't do that, code like InputInt() with LegionInputTextFlags_EnterReturnsTrue would fail.
        // This also allows the user to use InputText() with LegionInputTextFlags_EnterReturnsTrue without maintaining any user-side storage (please note that if you use this property along LegionInputTextFlags_CallbackResize you can end up with your temporary string object unnecessarily allocating once a frame, either store your string data, either if you don't then don't use LegionInputTextFlags_CallbackResize).
        bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & LegionInputTextFlags_EnterReturnsTrue) != 0);
        if (apply_edit_back_to_user_buffer)
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
            if (!is_readonly)
            {
                state->TextAIsValid = true;
                state->TextA.resize(state->TextW.Size * 4 + 1);
                ImTextStrToUtf8(state->TextA.Data, state->TextA.Size, state->TextW.Data, NULL);
            }

            // User callback
            if ((flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_CallbackHistory | LegionInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                LegionInputTextFlags event_flag = 0;
                LegionKey event_key = LegionKey_COUNT;
                if ((flags & LegionInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(LegionKey_Tab))
                {
                    event_flag = LegionInputTextFlags_CallbackCompletion;
                    event_key = LegionKey_Tab;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_UpArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_UpArrow;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_DownArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_DownArrow;
                }
                else if (flags & LegionInputTextFlags_CallbackAlways)
                    event_flag = LegionInputTextFlags_CallbackAlways;

                if (event_flag)
                {
                    LegionInputTextCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(LegionInputTextCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = callback_user_data;

                    callback_data.EventKey = event_key;
                    callback_data.Buf = state->TextA.Data;
                    callback_data.BufTextLen = state->CurLenA;
                    callback_data.BufSize = state->BufCapacityA;
                    callback_data.BufDirty = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
                    ImWchar* text = state->TextW.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + state->Stb.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    IM_ASSERT(callback_data.Buf == state->TextA.Data);  // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == state->BufCapacityA);
                    IM_ASSERT(callback_data.Flags == flags);
                    if (callback_data.CursorPos != utf8_cursor_pos)            { state->Stb.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos); state->CursorFollow = true; }
                    if (callback_data.SelectionStart != utf8_selection_start)  { state->Stb.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart); }
                    if (callback_data.SelectionEnd != utf8_selection_end)      { state->Stb.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd); }
                    if (callback_data.BufDirty)
                    {
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
                        if (callback_data.BufTextLen > backup_current_text_length && is_resizable)
                            state->TextW.resize(state->TextW.Size + (callback_data.BufTextLen - backup_current_text_length));
                        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, callback_data.Buf, NULL);
                        state->CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
                        state->CursorAnimReset();
                    }
                }
            }

            // Will copy result string if modified
            if (!is_readonly && strcmp(state->TextA.Data, buf) != 0)
            {
                apply_new_text = state->TextA.Data;
                apply_new_text_length = state->CurLenA;
            }
        }

        // Copy result to user buffer
        if (apply_new_text)
        {
            // We cannot test for 'backup_current_text_length != apply_new_text_length' here because we have no guarantee that the size
            // of our owned buffer matches the size of the string object held by the user, and by design we allow InputText() to be used
            // without any storage on user's side.
            IM_ASSERT(apply_new_text_length >= 0);
            if (is_resizable)
            {
                LegionInputTextCallbackData callback_data;
                callback_data.EventFlag = LegionInputTextFlags_CallbackResize;
                callback_data.Flags = flags;
                callback_data.Buf = buf;
                callback_data.BufTextLen = apply_new_text_length;
                callback_data.BufSize = ImMax(buf_size, apply_new_text_length + 1);
                callback_data.UserData = callback_user_data;
                callback(&callback_data);
                buf = callback_data.Buf;
                buf_size = callback_data.BufSize;
                apply_new_text_length = ImMin(callback_data.BufTextLen, buf_size - 1);
                IM_ASSERT(apply_new_text_length <= buf_size);
            }
            //Legion_DEBUG_LOG("InputText(\"%s\"): apply_new_text length %d\n", label, apply_new_text_length);

            // If the underlying buffer resize was denied or not carried to the next frame, apply_new_text_length+1 may be >= buf_size.
            ImStrncpy(buf, apply_new_text, ImMin(apply_new_text_length + 1, buf_size));
            value_changed = true;
        }

        // Clear temporary user storage
        state->UserFlags = 0;
        state->UserCallback = NULL;
        state->UserCallbackData = NULL;
    }

    // Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
    if (clear_active_id && g.ActiveId == id)
        ClearActiveID();

    // Render frame
    if (!is_multiline)
    {
        RenderNavHighlight(frame_bb, id);

        if (texture != NULL)
            window->DrawList->AddImage(texture, frame_bb.Min + ImVec2(-6, -6), frame_bb.Max + ImVec2(0, 9), ImVec2(0, 0), ImVec2(1, 1), GetColorU32(ImVec4(1, 1, 1, 1)));

    }

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + inner_size.x, frame_bb.Min.y + inner_size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 draw_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.0f, 0.0f);

    // Set upper limit of single-line InputTextEx() at 2 million characters strings. The current pathological worst case is a long line
    // without any carriage return, which would makes ImFont::RenderText() reserve too many vertices and probably crash. Avoid it altogether.
    // Note that we only use this limit on single-line InputText(), so a pathologically large line on a InputTextMultiline() would still crash.
    const int buf_display_max_length = 2 * 1024 * 1024;
    const char* buf_display = buf_display_from_state ? state->TextA.Data : buf; //-V595
    const char* buf_display_end = NULL; // We have specialized paths below for setting the length
    if (is_displaying_hint)
    {
        buf_display = hint;
        buf_display_end = hint + strlen(hint);
    }

    // Render text. We currently only render selection when the widget is active or while scrolling.
    // FIXME: We could remove the '&& render_cursor' to keep rendering selection when inactive.
    if (render_cursor || render_selection)
    {
        IM_ASSERT(state != NULL);
        if (!is_displaying_hint)
            buf_display_end = buf_display + state->CurLenA;

        // Render text (with cursor and selection)
        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = state->TextW.Data;
        ImVec2 cursor_offset, select_start_offset;

        {
            // Find lines numbers straddling 'cursor' (slot 0) and 'select_start' (slot 1) positions.
            const ImWchar* searches_input_ptr[2] = { NULL, NULL };
            int searches_result_line_no[2] = { -1000, -1000 };
            int searches_remaining = 0;
            if (render_cursor)
            {
                searches_input_ptr[0] = text_begin + state->Stb.cursor;
                searches_result_line_no[0] = -1;
                searches_remaining++;
            }
            if (render_selection)
            {
                searches_input_ptr[1] = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
                searches_result_line_no[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            //for (const ImWchar* s = text_begin; (s = (const ImWchar*)wcschr((const wchar_t*)s, (wchar_t)'\n')) != NULL; s++)  // FIXME-OPT: Could use this when wchar_t are 16-bit
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n')
                {
                    line_count++;
                    if (searches_result_line_no[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_no[0] = line_count; if (--searches_remaining <= 0) break; }
                    if (searches_result_line_no[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_no[1] = line_count; if (--searches_remaining <= 0) break; }
                }
            line_count++;
            if (searches_result_line_no[0] == -1)
                searches_result_line_no[0] = line_count;
            if (searches_result_line_no[1] == -1)
                searches_result_line_no[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and measuring distance
            cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
            cursor_offset.y = searches_result_line_no[0] * g.FontSize;
            if (searches_result_line_no[1] >= 0)
            {
                select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
                select_start_offset.y = searches_result_line_no[1] * g.FontSize;
            }

            // Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
            if (is_multiline)
                text_size = ImVec2(inner_size.x, line_count * g.FontSize);
        }

        // Scroll
        if (render_cursor && state->CursorFollow)
        {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & LegionInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = inner_size.x * 0.25f;
                if (cursor_offset.x < state->ScrollX)
                    state->ScrollX = IM_FLOOR(ImMax(0.0f, cursor_offset.x - scroll_increment_x));
                else if (cursor_offset.x - inner_size.x >= state->ScrollX)
                    state->ScrollX = IM_FLOOR(cursor_offset.x - inner_size.x + scroll_increment_x);
            }
            else
            {
                state->ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline)
            {
                float scroll_y = draw_window->Scroll.y;
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - inner_size.y >= scroll_y)
                    scroll_y = cursor_offset.y - inner_size.y;
                draw_pos.y += (draw_window->Scroll.y - scroll_y);   // Manipulate cursor pos immediately avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
            }

            state->CursorFollow = false;
        }

        // Draw selection
        const ImVec2 draw_scroll = ImVec2(state->ScrollX, 0.0f);
        if (render_selection)
        {
            const ImWchar* text_selected_begin = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
            const ImWchar* text_selected_end = text_begin + ImMax(state->Stb.select_start, state->Stb.select_end);

            ImU32 bg_color = GetColorU32(LegionCol_TextSelectedBg, render_cursor ? 1.0f : 0.6f); // FIXME: current code flow mandate that render_cursor is always true here, we are leaving the transparent one for tests.
            float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImVec2 rect_pos = draw_pos + select_start_offset - draw_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    //p = (const ImWchar*)wmemchr((const wchar_t*)p, '\n', text_selected_end - p);  // FIXME-OPT: Could use this when wchar_t are 16-bit
                    //p = p ? p + 1 : text_selected_end;
                    while (p < text_selected_end)
                        if (*p++ == '\n')
                            break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f) rect_size.x = IM_FLOOR(g.Font->GetCharAdvance((ImWchar)' ') * 0.50f); // So we can see selected empty lines
                    ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos +ImVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = draw_pos.x - draw_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        // We test for 'buf_display_max_length' as a way to avoid some pathological cases (e.g. single-line 1 MB string) which would make ImDrawList crash.
        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos - draw_scroll, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }

        // Draw blinking cursor
        if (render_cursor)
        {
            state->CursorAnim += io.DeltaTime;
            bool cursor_is_visible = (!g.IO.ConfigInputTextCursorBlink) || (state->CursorAnim <= 0.0f) || ImFmod(state->CursorAnim, 1.20f) <= 0.80f;
            ImVec2 cursor_screen_pos = draw_pos + cursor_offset - draw_scroll;
            ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
            if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
                draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(LegionCol_Text));

            // Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
            if (!is_readonly)
                g.PlatformImePos = ImVec2(cursor_screen_pos.x - 1.0f, cursor_screen_pos.y - g.FontSize);
        }
    }
    else
    {
        // Render text only (no selection, no cursor)
        if (is_multiline)
            text_size = ImVec2(inner_size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_display_end) * g.FontSize); // We don't need width
        else if (!is_displaying_hint && g.ActiveId == id)
            buf_display_end = buf_display + state->CurLenA;
        else if (!is_displaying_hint)
            buf_display_end = buf_display + strlen(buf_display);

        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }
    }

    if (is_multiline)
    {
        Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
        EndChild();
        EndGroup();
    }

    if (is_password && !is_displaying_hint)
        PopFont();

    // Log as text
    if (g.LogEnabled && !(is_password && !is_displaying_hint))
        LogRenderedText(&draw_pos, buf_display, buf_display_end);

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if (value_changed && !(flags & LegionInputTextFlags_NoMarkEdited))
        MarkItemEdited(id);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    if ((flags & LegionInputTextFlags_EnterReturnsTrue) != 0)
        return enter_pressed;
    else
        return false;
}

bool Legion::InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, LegionInputTextFlags flags, LegionInputTextCallback callback, void* callback_user_data)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackHistory) && (flags & LegionInputTextFlags_Multiline)));        // Can't use both together (they both use up/down keys)
    IM_ASSERT(!((flags & LegionInputTextFlags_CallbackCompletion) && (flags & LegionInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

    LegionContext& g = *GLegion;
    LegionIO& io = g.IO;
    const LegionStyle& style = g.Style;

    const bool RENDER_SELECTION_WHEN_INACTIVE = false;
    const bool is_multiline = (flags & LegionInputTextFlags_Multiline) != 0;
    const bool is_readonly = (flags & LegionInputTextFlags_ReadOnly) != 0;
    const bool is_password = (flags & LegionInputTextFlags_Password) != 0;
    const bool is_undoable = (flags & LegionInputTextFlags_NoUndoRedo) == 0;
    const bool is_resizable = (flags & LegionInputTextFlags_CallbackResize) != 0;
    if (is_resizable)
        IM_ASSERT(callback != NULL); // Must provide a callback if you set the LegionInputTextFlags_CallbackResize flag!

    if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope,
        BeginGroup();
    const LegionID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 frame_size = CalcItemSize(size_arg, CalcItemWidth(), (is_multiline ? g.FontSize * 8.0f : label_size.y) + style.FramePadding.y * 2.0f); // Arbitrary default of 8 lines high for multi-line
    const ImVec2 total_size = ImVec2(frame_size.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_size.y);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    const ImRect total_bb(frame_bb.Min, frame_bb.Min + total_size);

    LegionWindow* draw_window = window;
    ImVec2 inner_size = frame_size;
    if (is_multiline)
    {
        if (!ItemAdd(total_bb, id, &frame_bb))
        {
            ItemSize(total_bb, style.FramePadding.y);
            EndGroup();
            return false;
        }

        // We reproduce the contents of BeginChildFrame() in order to provide 'label' so our window internal data are easier to read/debug.
        PushStyleColor(LegionCol_ChildBg, style.Colors[LegionCol_FrameBg]);
        PushStyleVar(LegionStyleVar_ChildRounding, style.FrameRounding);
        PushStyleVar(LegionStyleVar_ChildBorderSize, style.FrameBorderSize);
        PushStyleVar(LegionStyleVar_WindowPadding, style.FramePadding);
        bool child_visible = BeginChildEx(label, id, frame_bb.GetSize(), true, LegionWindowFlags_NoMove | LegionWindowFlags_AlwaysUseWindowPadding);
        PopStyleVar(3);
        PopStyleColor();
        if (!child_visible)
        {
            EndChild();
            EndGroup();
            return false;
        }
        draw_window = g.CurrentWindow; // Child window
        draw_window->DC.NavLayerActiveMaskNext |= draw_window->DC.NavLayerCurrentMask; // This is to ensure that EndChild() will display a navigation highlight so we can "enter" into it.
        inner_size.x -= draw_window->ScrollbarSizes.x;
    }
    else
    {
        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb))
            return false;
    }
    const bool hovered = ItemHoverable(frame_bb, id);
    if (hovered)
        g.MouseCursor = LegionMouseCursor_TextInput;

    // We are only allowed to access the state if we are already the active widget.
    LegionInputTextState* state = GetInputTextState(id);

    const bool focus_requested = FocusableItemRegister(window, id);
    const bool focus_requested_by_code = focus_requested && (g.FocusRequestCurrWindow == window && g.FocusRequestCurrCounterRegular == window->DC.FocusCounterRegular);
    const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

    const bool user_clicked = hovered && io.MouseClicked[0];
    const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == LegionInputSource_NavKeyboard));
    const bool user_scroll_finish = is_multiline && state != NULL && g.ActiveId == 0 && g.ActiveIdPreviousFrame == GetWindowScrollbarID(draw_window, LegionAxis_Y);
    const bool user_scroll_active = is_multiline && state != NULL && g.ActiveId == GetWindowScrollbarID(draw_window, LegionAxis_Y);

    bool clear_active_id = false;
    bool select_all = (g.ActiveId != id) && ((flags & LegionInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);

    const bool init_make_active = (focus_requested || user_clicked || user_scroll_finish || user_nav_input_start);
    const bool init_state = (init_make_active || user_scroll_active);
    if (init_state && g.ActiveId != id)
    {
        // Access state even if we don't own it yet.
        state = &g.InputTextState;
        state->CursorAnimReset();

        // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
        // From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
        const int buf_len = (int)strlen(buf);
        state->InitialTextA.resize(buf_len + 1);    // UTF-8. we use +1 to make sure that .Data is always pointing to at least an empty string.
        memcpy(state->InitialTextA.Data, buf, buf_len + 1);

        // Start edition
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);          // wchar count <= UTF-8 count. we use +1 to make sure that .Data is always pointing to at least an empty string.
        state->TextA.resize(0);
        state->TextAIsValid = false;                // TextA is not valid yet (we will display buf until then)
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, buf_size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);      // We can't get the result from ImStrncpy() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.

        // Preserve cursor position and undo/redo stack if we come back to same widget
        // FIXME: For non-readonly widgets we might be able to require that TextAIsValid && TextA == buf ? (untested) and discard undo stack if user buffer has changed.
        const bool recycle_state = (state->ID == id);
        if (recycle_state)
        {
            // Recycle existing cursor/selection/undo stack but clamp position
            // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
            state->CursorClamp();
        }
        else
        {
            state->ID = id;
            state->ScrollX = 0.0f;
            stb_textedit_initialize_state(&state->Stb, !is_multiline);
            if (!is_multiline && focus_requested_by_code)
                select_all = true;
        }
        if (flags & LegionInputTextFlags_AlwaysInsertMode)
            state->Stb.insert_mode = 1;
        if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
            select_all = true;
    }

    if (g.ActiveId != id && init_make_active)
    {
        IM_ASSERT(state && state->ID == id);
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);

        // Declare our inputs
        IM_ASSERT(LegionNavInput_COUNT < 32);
        g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Left) | (1 << LegionDir_Right);
        if (is_multiline || (flags & LegionInputTextFlags_CallbackHistory))
            g.ActiveIdUsingNavDirMask |= (1 << LegionDir_Up) | (1 << LegionDir_Down);
        g.ActiveIdUsingNavInputMask |= (1 << LegionNavInput_Cancel);
        g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Home) | ((ImU64)1 << LegionKey_End);
        if (is_multiline)
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_PageUp) | ((ImU64)1 << LegionKey_PageDown); // FIXME-NAV: Page up/down actually not supported yet by widget, but claim them ahead.
        if (flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_AllowTabInput))  // Disable keyboard tabbing out as we will use the \t character.
            g.ActiveIdUsingKeyInputMask |= ((ImU64)1 << LegionKey_Tab);
    }

    // We have an edge case if ActiveId was set through another widget (e.g. widget being swapped), clear id immediately (don't wait until the end of the function)
    if (g.ActiveId == id && state == NULL)
        ClearActiveID();

    // Release focus when we click outside
    if (g.ActiveId == id && io.MouseClicked[0] && !init_state && !init_make_active) //-V560
        clear_active_id = true;

    // Lock the decision of whether we are going to take the path displaying the cursor or selection
    const bool render_cursor = (g.ActiveId == id) || (state && user_scroll_active);
    bool render_selection = state && state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    bool value_changed = false;
    bool enter_pressed = false;

    // When read-only we always use the live data passed to the function
    // FIXME-OPT: Because our selection/cursor code currently needs the wide text we need to convert it when active, which is not ideal :(
    if (is_readonly && state != NULL && (render_cursor || render_selection))
    {
        const char* buf_end = NULL;
        state->TextW.resize(buf_size + 1);
        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, buf, NULL, &buf_end);
        state->CurLenA = (int)(buf_end - buf);
        state->CursorClamp();
        render_selection &= state->HasSelection();
    }

    // Select the buffer to render.
    const bool buf_display_from_state = (render_cursor || render_selection || g.ActiveId == id) && !is_readonly && state && state->TextAIsValid;
    const bool is_displaying_hint = (hint != NULL && (buf_display_from_state ? state->TextA.Data : buf)[0] == 0);

    // Password pushes a temporary font with only a fallback glyph
    if (is_password && !is_displaying_hint)
    {
        const ImFontGlyph* glyph = g.Font->FindGlyph('*');
        ImFont* password_font = &g.InputTextPasswordFont;
        password_font->FontSize = g.Font->FontSize;
        password_font->Scale = g.Font->Scale;
        password_font->DisplayOffset = g.Font->DisplayOffset;
        password_font->Ascent = g.Font->Ascent;
        password_font->Descent = g.Font->Descent;
        password_font->ContainerAtlas = g.Font->ContainerAtlas;
        password_font->FallbackGlyph = glyph;
        password_font->FallbackAdvanceX = glyph->AdvanceX;
        IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
        PushFont(password_font);
    }

    // Process mouse inputs and character inputs
    int backup_current_text_length = 0;
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        backup_current_text_length = state->CurLenA;
        state->BufCapacityA = buf_size;
        state->UserFlags = flags;
        state->UserCallback = callback;
        state->UserCallbackData = callback_user_data;

        // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
        // Down the line we should have a cleaner library-wide concept of Selected vs Active.
        g.ActiveIdAllowOverlap = !io.MouseDown[0];
        g.WantTextInputNextFrame = 1;

        // Edit in progress
        const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + state->ScrollX;
        const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize * 0.5f));

        const bool is_osx = io.ConfigMacOSXBehaviors;
        if (select_all || (hovered && !is_osx && io.MouseDoubleClicked[0]))
        {
            state->SelectAll();
            state->SelectedAllMouseLock = true;
        }
        else if (hovered && is_osx && io.MouseDoubleClicked[0])
        {
            // Double-click select a word only, OS X style (by simulating keystrokes)
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
            state->OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
        }
        else if (io.MouseClicked[0] && !state->SelectedAllMouseLock)
        {
            if (hovered)
            {
                stb_textedit_click(state, &state->Stb, mouse_x, mouse_y);
                state->CursorAnimReset();
            }
        }
        else if (io.MouseDown[0] && !state->SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
        {
            stb_textedit_drag(state, &state->Stb, mouse_x, mouse_y);
            state->CursorAnimReset();
            state->CursorFollow = true;
        }
        if (state->SelectedAllMouseLock && !io.MouseDown[0])
            state->SelectedAllMouseLock = false;

        // It is ill-defined whether the back-end needs to send a \t character when pressing the TAB keys.
        // Win32 and GLFW naturally do it but not SDL.
        const bool ignore_char_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
        if ((flags & LegionInputTextFlags_AllowTabInput) && IsKeyPressedMap(LegionKey_Tab) && !ignore_char_inputs && !io.KeyShift && !is_readonly)
            if (!io.InputQueueCharacters.contains('\t'))
            {
                unsigned int c = '\t'; // Insert TAB
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }

        // Process regular text input (before we check for Return because using some IME will effectively send a Return?)
        // We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
        if (io.InputQueueCharacters.Size > 0)
        {
            if (!ignore_char_inputs && !is_readonly && !user_nav_input_start)
                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    // Insert character if they pass filtering
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c == '\t' && io.KeyShift)
                        continue;
                    if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        state->OnKeyPressed((int)c);
                }

            // Consume characters
            io.InputQueueCharacters.resize(0);
        }
    }

    // Process other shortcuts/key-presses
    bool cancel_edit = false;
    if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
    {
        IM_ASSERT(state != NULL);
        IM_ASSERT(io.KeyMods == GetMergedKeyModFlags() && "Mismatching io.KeyCtrl/io.KeyShift/io.KeyAlt/io.KeySuper vs io.KeyMods"); // We rarely do this check, but if anything let's do it here.

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        const bool is_osx = io.ConfigMacOSXBehaviors;
        const bool is_osx_shift_shortcut = is_osx && (io.KeyMods == (LegionKeyModFlags_Super | LegionKeyModFlags_Shift));
        const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl;                     // OS X style: Text editing cursor movement using Alt instead of Ctrl
        const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt;  // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
        const bool is_ctrl_key_only = (io.KeyMods == LegionKeyModFlags_Ctrl);
        const bool is_shift_key_only = (io.KeyMods == LegionKeyModFlags_Shift);
        const bool is_shortcut_key = g.IO.ConfigMacOSXBehaviors ? (io.KeyMods == LegionKeyModFlags_Super) : (io.KeyMods == LegionKeyModFlags_Ctrl);

        const bool is_cut = ((is_shortcut_key && IsKeyPressedMap(LegionKey_X)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Delete))) && !is_readonly && !is_password && (!is_multiline || state->HasSelection());
        const bool is_copy = ((is_shortcut_key && IsKeyPressedMap(LegionKey_C)) || (is_ctrl_key_only && IsKeyPressedMap(LegionKey_Insert))) && !is_password && (!is_multiline || state->HasSelection());
        const bool is_paste = ((is_shortcut_key && IsKeyPressedMap(LegionKey_V)) || (is_shift_key_only && IsKeyPressedMap(LegionKey_Insert))) && !is_readonly;
        const bool is_undo = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Z)) && !is_readonly && is_undoable);
        const bool is_redo = ((is_shortcut_key && IsKeyPressedMap(LegionKey_Y)) || (is_osx_shift_shortcut && IsKeyPressedMap(LegionKey_Z))) && !is_readonly && is_undoable;

        if (IsKeyPressedMap(LegionKey_LeftArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_RightArrow)) { state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_UpArrow) && is_multiline) { if (io.KeyCtrl) SetScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f)); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_DownArrow) && is_multiline) { if (io.KeyCtrl) SetScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, GetScrollMaxY())); else state->OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Home)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
        else if (IsKeyPressedMap(LegionKey_End)) { state->OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Delete) && !is_readonly) { state->OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
        else if (IsKeyPressedMap(LegionKey_Backspace) && !is_readonly)
        {
            if (!state->HasSelection())
            {
                if (is_wordmove_key_down)
                    state->OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
                else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
                    state->OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
            }
            state->OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
        }
        else if (IsKeyPressedMap(LegionKey_Enter) || IsKeyPressedMap(LegionKey_KeyPadEnter))
        {
            bool ctrl_enter_for_new_line = (flags & LegionInputTextFlags_CtrlEnterForNewLine) != 0;
            if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
            {
                enter_pressed = clear_active_id = true;
            }
            else if (!is_readonly)
            {
                unsigned int c = '\n'; // Insert new line
                if (InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                    state->OnKeyPressed((int)c);
            }
        }
        else if (IsKeyPressedMap(LegionKey_Escape))
        {
            clear_active_id = cancel_edit = true;
        }
        else if (is_undo || is_redo)
        {
            state->OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
            state->ClearSelection();
        }
        else if (is_shortcut_key && IsKeyPressedMap(LegionKey_A))
        {
            state->SelectAll();
            state->CursorFollow = true;
        }
        else if (is_cut || is_copy)
        {
            // Cut, Copy
            if (io.SetClipboardTextFn)
            {
                const int ib = state->HasSelection() ? ImMin(state->Stb.select_start, state->Stb.select_end) : 0;
                const int ie = state->HasSelection() ? ImMax(state->Stb.select_start, state->Stb.select_end) : state->CurLenW;
                const int clipboard_data_len = ImTextCountUtf8BytesFromStr(state->TextW.Data + ib, state->TextW.Data + ie) + 1;
                char* clipboard_data = (char*)IM_ALLOC(clipboard_data_len * sizeof(char));
                ImTextStrToUtf8(clipboard_data, clipboard_data_len, state->TextW.Data + ib, state->TextW.Data + ie);
                SetClipboardText(clipboard_data);
                MemFree(clipboard_data);
            }
            if (is_cut)
            {
                if (!state->HasSelection())
                    state->SelectAll();
                state->CursorFollow = true;
                stb_textedit_cut(state, &state->Stb);
            }
        }
        else if (is_paste)
        {
            if (const char* clipboard = GetClipboardText())
            {
                // Filter pasted buffer
                const int clipboard_len = (int)strlen(clipboard);
                ImWchar* clipboard_filtered = (ImWchar*)IM_ALLOC((clipboard_len + 1) * sizeof(ImWchar));
                int clipboard_filtered_len = 0;
                for (const char* s = clipboard; *s; )
                {
                    unsigned int c;
                    s += ImTextCharFromUtf8(&c, s, NULL);
                    if (c == 0)
                        break;
                    if (!InputTextFilterCharacter(&c, flags, callback, callback_user_data))
                        continue;
                    clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                }
                clipboard_filtered[clipboard_filtered_len] = 0;
                if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                {
                    stb_textedit_paste(state, &state->Stb, clipboard_filtered, clipboard_filtered_len);
                    state->CursorFollow = true;
                }
                MemFree(clipboard_filtered);
            }
        }

        // Update render selection flag after events have been handled, so selection highlight can be displayed during the same frame.
        render_selection |= state->HasSelection() && (RENDER_SELECTION_WHEN_INACTIVE || render_cursor);
    }

    // Process callbacks and apply result back to user's buffer.
    if (g.ActiveId == id)
    {
        IM_ASSERT(state != NULL);
        const char* apply_new_text = NULL;
        int apply_new_text_length = 0;
        if (cancel_edit)
        {
            // Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
            if (!is_readonly && strcmp(buf, state->InitialTextA.Data) != 0)
            {
                // Push records into the undo stack so we can CTRL+Z the revert operation itself
                apply_new_text = state->InitialTextA.Data;
                apply_new_text_length = state->InitialTextA.Size - 1;
                ImVector<ImWchar> w_text;
                if (apply_new_text_length > 0)
                {
                    w_text.resize(ImTextCountCharsFromUtf8(apply_new_text, apply_new_text + apply_new_text_length) + 1);
                    ImTextStrFromUtf8(w_text.Data, w_text.Size, apply_new_text, apply_new_text + apply_new_text_length);
                }
                stb_textedit_replace(state, &state->Stb, w_text.Data, (apply_new_text_length > 0) ? (w_text.Size - 1) : 0);
            }
        }

        // When using 'LegionInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
        // If we didn't do that, code like InputInt() with LegionInputTextFlags_EnterReturnsTrue would fail.
        // This also allows the user to use InputText() with LegionInputTextFlags_EnterReturnsTrue without maintaining any user-side storage (please note that if you use this property along LegionInputTextFlags_CallbackResize you can end up with your temporary string object unnecessarily allocating once a frame, either store your string data, either if you don't then don't use LegionInputTextFlags_CallbackResize).
        bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & LegionInputTextFlags_EnterReturnsTrue) != 0);
        if (apply_edit_back_to_user_buffer)
        {
            // Apply new value immediately - copy modified buffer back
            // Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
            // FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
            // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
            if (!is_readonly)
            {
                state->TextAIsValid = true;
                state->TextA.resize(state->TextW.Size * 4 + 1);
                ImTextStrToUtf8(state->TextA.Data, state->TextA.Size, state->TextW.Data, NULL);
            }

            // User callback
            if ((flags & (LegionInputTextFlags_CallbackCompletion | LegionInputTextFlags_CallbackHistory | LegionInputTextFlags_CallbackAlways)) != 0)
            {
                IM_ASSERT(callback != NULL);

                // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                LegionInputTextFlags event_flag = 0;
                LegionKey event_key = LegionKey_COUNT;
                if ((flags & LegionInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(LegionKey_Tab))
                {
                    event_flag = LegionInputTextFlags_CallbackCompletion;
                    event_key = LegionKey_Tab;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_UpArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_UpArrow;
                }
                else if ((flags & LegionInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(LegionKey_DownArrow))
                {
                    event_flag = LegionInputTextFlags_CallbackHistory;
                    event_key = LegionKey_DownArrow;
                }
                else if (flags & LegionInputTextFlags_CallbackAlways)
                    event_flag = LegionInputTextFlags_CallbackAlways;

                if (event_flag)
                {
                    LegionInputTextCallbackData callback_data;
                    memset(&callback_data, 0, sizeof(LegionInputTextCallbackData));
                    callback_data.EventFlag = event_flag;
                    callback_data.Flags = flags;
                    callback_data.UserData = callback_user_data;

                    callback_data.EventKey = event_key;
                    callback_data.Buf = state->TextA.Data;
                    callback_data.BufTextLen = state->CurLenA;
                    callback_data.BufSize = state->BufCapacityA;
                    callback_data.BufDirty = false;

                    // We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
                    ImWchar* text = state->TextW.Data;
                    const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + state->Stb.cursor);
                    const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_start);
                    const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + state->Stb.select_end);

                    // Call user code
                    callback(&callback_data);

                    // Read back what user may have modified
                    IM_ASSERT(callback_data.Buf == state->TextA.Data);  // Invalid to modify those fields
                    IM_ASSERT(callback_data.BufSize == state->BufCapacityA);
                    IM_ASSERT(callback_data.Flags == flags);
                    if (callback_data.CursorPos != utf8_cursor_pos) { state->Stb.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos); state->CursorFollow = true; }
                    if (callback_data.SelectionStart != utf8_selection_start) { state->Stb.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart); }
                    if (callback_data.SelectionEnd != utf8_selection_end) { state->Stb.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd); }
                    if (callback_data.BufDirty)
                    {
                        IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
                        if (callback_data.BufTextLen > backup_current_text_length&& is_resizable)
                            state->TextW.resize(state->TextW.Size + (callback_data.BufTextLen - backup_current_text_length));
                        state->CurLenW = ImTextStrFromUtf8(state->TextW.Data, state->TextW.Size, callback_data.Buf, NULL);
                        state->CurLenA = callback_data.BufTextLen;  // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
                        state->CursorAnimReset();
                    }
                }
            }

            // Will copy result string if modified
            if (!is_readonly && strcmp(state->TextA.Data, buf) != 0)
            {
                apply_new_text = state->TextA.Data;
                apply_new_text_length = state->CurLenA;
            }
        }

        // Copy result to user buffer
        if (apply_new_text)
        {
            // We cannot test for 'backup_current_text_length != apply_new_text_length' here because we have no guarantee that the size
            // of our owned buffer matches the size of the string object held by the user, and by design we allow InputText() to be used
            // without any storage on user's side.
            IM_ASSERT(apply_new_text_length >= 0);
            if (is_resizable)
            {
                LegionInputTextCallbackData callback_data;
                callback_data.EventFlag = LegionInputTextFlags_CallbackResize;
                callback_data.Flags = flags;
                callback_data.Buf = buf;
                callback_data.BufTextLen = apply_new_text_length;
                callback_data.BufSize = ImMax(buf_size, apply_new_text_length + 1);
                callback_data.UserData = callback_user_data;
                callback(&callback_data);
                buf = callback_data.Buf;
                buf_size = callback_data.BufSize;
                apply_new_text_length = ImMin(callback_data.BufTextLen, buf_size - 1);
                IM_ASSERT(apply_new_text_length <= buf_size);
            }
            //Legion_DEBUG_LOG("InputText(\"%s\"): apply_new_text length %d\n", label, apply_new_text_length);

            // If the underlying buffer resize was denied or not carried to the next frame, apply_new_text_length+1 may be >= buf_size.
            ImStrncpy(buf, apply_new_text, ImMin(apply_new_text_length + 1, buf_size));
            value_changed = true;
        }

        // Clear temporary user storage
        state->UserFlags = 0;
        state->UserCallback = NULL;
        state->UserCallbackData = NULL;
    }

    // Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
    if (clear_active_id && g.ActiveId == id)
        ClearActiveID();

    // Render frame
    if (!is_multiline)
    {
        RenderNavHighlight(frame_bb, id);
        RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(LegionCol_FrameBg), true, style.FrameRounding);
    }

    const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + inner_size.x, frame_bb.Min.y + inner_size.y); // Not using frame_bb.Max because we have adjusted size
    ImVec2 draw_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
    ImVec2 text_size(0.0f, 0.0f);

    // Set upper limit of single-line InputTextEx() at 2 million characters strings. The current pathological worst case is a long line
    // without any carriage return, which would makes ImFont::RenderText() reserve too many vertices and probably crash. Avoid it altogether.
    // Note that we only use this limit on single-line InputText(), so a pathologically large line on a InputTextMultiline() would still crash.
    const int buf_display_max_length = 2 * 1024 * 1024;
    const char* buf_display = buf_display_from_state ? state->TextA.Data : buf; //-V595
    const char* buf_display_end = NULL; // We have specialized paths below for setting the length
    if (is_displaying_hint)
    {
        buf_display = hint;
        buf_display_end = hint + strlen(hint);
    }

    // Render text. We currently only render selection when the widget is active or while scrolling.
    // FIXME: We could remove the '&& render_cursor' to keep rendering selection when inactive.
    if (render_cursor || render_selection)
    {
        IM_ASSERT(state != NULL);
        if (!is_displaying_hint)
            buf_display_end = buf_display + state->CurLenA;

        // Render text (with cursor and selection)
        // This is going to be messy. We need to:
        // - Display the text (this alone can be more easily clipped)
        // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
        // - Measure text height (for scrollbar)
        // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
        // FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
        const ImWchar* text_begin = state->TextW.Data;
        ImVec2 cursor_offset, select_start_offset;

        {
            // Find lines numbers straddling 'cursor' (slot 0) and 'select_start' (slot 1) positions.
            const ImWchar* searches_input_ptr[2] = { NULL, NULL };
            int searches_result_line_no[2] = { -1000, -1000 };
            int searches_remaining = 0;
            if (render_cursor)
            {
                searches_input_ptr[0] = text_begin + state->Stb.cursor;
                searches_result_line_no[0] = -1;
                searches_remaining++;
            }
            if (render_selection)
            {
                searches_input_ptr[1] = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
                searches_result_line_no[1] = -1;
                searches_remaining++;
            }

            // Iterate all lines to find our line numbers
            // In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
            searches_remaining += is_multiline ? 1 : 0;
            int line_count = 0;
            //for (const ImWchar* s = text_begin; (s = (const ImWchar*)wcschr((const wchar_t*)s, (wchar_t)'\n')) != NULL; s++)  // FIXME-OPT: Could use this when wchar_t are 16-bit
            for (const ImWchar* s = text_begin; *s != 0; s++)
                if (*s == '\n')
                {
                    line_count++;
                    if (searches_result_line_no[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_no[0] = line_count; if (--searches_remaining <= 0) break; }
                    if (searches_result_line_no[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_no[1] = line_count; if (--searches_remaining <= 0) break; }
                }
            line_count++;
            if (searches_result_line_no[0] == -1)
                searches_result_line_no[0] = line_count;
            if (searches_result_line_no[1] == -1)
                searches_result_line_no[1] = line_count;

            // Calculate 2d position by finding the beginning of the line and measuring distance
            cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
            cursor_offset.y = searches_result_line_no[0] * g.FontSize;
            if (searches_result_line_no[1] >= 0)
            {
                select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
                select_start_offset.y = searches_result_line_no[1] * g.FontSize;
            }

            // Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
            if (is_multiline)
                text_size = ImVec2(inner_size.x, line_count * g.FontSize);
        }

        // Scroll
        if (render_cursor && state->CursorFollow)
        {
            // Horizontal scroll in chunks of quarter width
            if (!(flags & LegionInputTextFlags_NoHorizontalScroll))
            {
                const float scroll_increment_x = inner_size.x * 0.25f;
                if (cursor_offset.x < state->ScrollX)
                    state->ScrollX = IM_FLOOR(ImMax(0.0f, cursor_offset.x - scroll_increment_x));
                else if (cursor_offset.x - inner_size.x >= state->ScrollX)
                    state->ScrollX = IM_FLOOR(cursor_offset.x - inner_size.x + scroll_increment_x);
            }
            else
            {
                state->ScrollX = 0.0f;
            }

            // Vertical scroll
            if (is_multiline)
            {
                float scroll_y = draw_window->Scroll.y;
                if (cursor_offset.y - g.FontSize < scroll_y)
                    scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                else if (cursor_offset.y - inner_size.y >= scroll_y)
                    scroll_y = cursor_offset.y - inner_size.y;
                draw_pos.y += (draw_window->Scroll.y - scroll_y);   // Manipulate cursor pos immediately avoid a frame of lag
                draw_window->Scroll.y = scroll_y;
            }

            state->CursorFollow = false;
        }

        // Draw selection
        const ImVec2 draw_scroll = ImVec2(state->ScrollX, 0.0f);
        if (render_selection)
        {
            const ImWchar* text_selected_begin = text_begin + ImMin(state->Stb.select_start, state->Stb.select_end);
            const ImWchar* text_selected_end = text_begin + ImMax(state->Stb.select_start, state->Stb.select_end);

            ImU32 bg_color = GetColorU32(LegionCol_TextSelectedBg, render_cursor ? 1.0f : 0.6f); // FIXME: current code flow mandate that render_cursor is always true here, we are leaving the transparent one for tests.
            float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
            float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
            ImVec2 rect_pos = draw_pos + select_start_offset - draw_scroll;
            for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
            {
                if (rect_pos.y > clip_rect.w + g.FontSize)
                    break;
                if (rect_pos.y < clip_rect.y)
                {
                    //p = (const ImWchar*)wmemchr((const wchar_t*)p, '\n', text_selected_end - p);  // FIXME-OPT: Could use this when wchar_t are 16-bit
                    //p = p ? p + 1 : text_selected_end;
                    while (p < text_selected_end)
                        if (*p++ == '\n')
                            break;
                }
                else
                {
                    ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                    if (rect_size.x <= 0.0f) rect_size.x = IM_FLOOR(g.Font->GetCharAdvance((ImWchar)' ') * 0.50f); // So we can see selected empty lines
                    ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
                    rect.ClipWith(clip_rect);
                    if (rect.Overlaps(clip_rect))
                        draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                }
                rect_pos.x = draw_pos.x - draw_scroll.x;
                rect_pos.y += g.FontSize;
            }
        }

        // We test for 'buf_display_max_length' as a way to avoid some pathological cases (e.g. single-line 1 MB string) which would make ImDrawList crash.
        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos - draw_scroll, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }

        // Draw blinking cursor
        if (render_cursor)
        {
            state->CursorAnim += io.DeltaTime;
            bool cursor_is_visible = (!g.IO.ConfigInputTextCursorBlink) || (state->CursorAnim <= 0.0f) || ImFmod(state->CursorAnim, 1.20f) <= 0.80f;
            ImVec2 cursor_screen_pos = draw_pos + cursor_offset - draw_scroll;
            ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
            if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
                draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(LegionCol_Text));

            // Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
            if (!is_readonly)
                g.PlatformImePos = ImVec2(cursor_screen_pos.x - 1.0f, cursor_screen_pos.y - g.FontSize);
        }
    }
    else
    {
        // Render text only (no selection, no cursor)
        if (is_multiline)
            text_size = ImVec2(inner_size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_display_end) * g.FontSize); // We don't need width
        else if (!is_displaying_hint && g.ActiveId == id)
            buf_display_end = buf_display + state->CurLenA;
        else if (!is_displaying_hint)
            buf_display_end = buf_display + strlen(buf_display);

        if (is_multiline || (buf_display_end - buf_display) < buf_display_max_length)
        {
            ImU32 col = GetColorU32(is_displaying_hint ? LegionCol_TextDisabled : LegionCol_Text);
            draw_window->DrawList->AddText(g.Font, g.FontSize, draw_pos, col, buf_display, buf_display_end, 0.0f, is_multiline ? NULL : &clip_rect);
        }
    }

    if (is_multiline)
    {
        Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
        EndChild();
        EndGroup();
    }

    if (is_password && !is_displaying_hint)
        PopFont();

    // Log as text
    if (g.LogEnabled && !(is_password && !is_displaying_hint))
        LogRenderedText(&draw_pos, buf_display, buf_display_end);

    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    if (value_changed && !(flags & LegionInputTextFlags_NoMarkEdited))
        MarkItemEdited(id);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    if ((flags & LegionInputTextFlags_EnterReturnsTrue) != 0)
        return enter_pressed;
    else
        return value_changed;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: ColorEdit, ColorPicker, ColorButton, etc.
//-------------------------------------------------------------------------
// - ColorEdit3()
// - ColorEdit4()
// - ColorPicker3()
// - RenderColorRectWithAlphaCheckerboard() [Internal]
// - ColorPicker4()
// - ColorButton()
// - SetColorEditOptions()
// - ColorTooltip() [Internal]
// - ColorEditOptionsPopup() [Internal]
// - ColorPickerOptionsPopup() [Internal]
//-------------------------------------------------------------------------

bool Legion::ColorEdit3(const char* label, float col[3], LegionColorEditFlags flags)
{
    return ColorEdit4(label, col, flags | LegionColorEditFlags_NoAlpha);
}

// Edit colors components (each component in 0.0f..1.0f range).
// See enum LegionColorEditFlags_ for available options. e.g. Only access 3 floats if LegionColorEditFlags_NoAlpha flag is set.
// With typical options: Left-click on colored square to open color picker. Right-click to open option menu. CTRL-Click over input fields to edit them and TAB to go to next item.
bool Legion::ColorEdit4(const char* label, float col[4], LegionColorEditFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const float square_sz = GetFrameHeight();
    const float w_full = CalcItemWidth();
    const float w_button = (flags & LegionColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
    const float w_inputs = w_full - w_button;
    const char* label_display_end = FindRenderedTextEnd(label);
    g.NextItemData.ClearFlags();

    BeginGroup();
    PushID(label);

    // If we're not showing any slider there's no point in doing any HSV conversions
    const LegionColorEditFlags flags_untouched = flags;
    if (flags & LegionColorEditFlags_NoInputs)
        flags = (flags & (~LegionColorEditFlags__DisplayMask)) | LegionColorEditFlags_DisplayRGB | LegionColorEditFlags_NoOptions;

    // Context menu: display and modify options (before defaults are applied)
    if (!(flags & LegionColorEditFlags_NoOptions))
        ColorEditOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & LegionColorEditFlags__DisplayMask))
        flags |= (g.ColorEditOptions & LegionColorEditFlags__DisplayMask);
    if (!(flags & LegionColorEditFlags__DataTypeMask))
        flags |= (g.ColorEditOptions & LegionColorEditFlags__DataTypeMask);
    if (!(flags & LegionColorEditFlags__PickerMask))
        flags |= (g.ColorEditOptions & LegionColorEditFlags__PickerMask);
    if (!(flags & LegionColorEditFlags__InputMask))
        flags |= (g.ColorEditOptions & LegionColorEditFlags__InputMask);
    flags |= (g.ColorEditOptions & ~(LegionColorEditFlags__DisplayMask | LegionColorEditFlags__DataTypeMask | LegionColorEditFlags__PickerMask | LegionColorEditFlags__InputMask));
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__DisplayMask)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__InputMask));   // Check that only 1 is selected

    const bool alpha = (flags & LegionColorEditFlags_NoAlpha) == 0;
    const bool hdr = (flags & LegionColorEditFlags_HDR) != 0;
    const int components = alpha ? 4 : 3;

    // Convert to the formats we need
    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    if ((flags & LegionColorEditFlags_InputHSV) && (flags & LegionColorEditFlags_DisplayRGB))
        ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
    else if ((flags & LegionColorEditFlags_InputRGB) && (flags & LegionColorEditFlags_DisplayHSV))
    {
        // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
        if (memcmp(g.ColorEditLastColor, col, sizeof(float) * 3) == 0)
        {
            if (f[1] == 0)
                f[0] = g.ColorEditLastHue;
            if (f[2] == 0)
                f[1] = g.ColorEditLastSat;
        }
    }
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    bool value_changed = false;
    bool value_changed_as_float = false;

    const ImVec2 pos = window->DC.CursorPos;
    const float inputs_offset_x = (style.ColorButtonPosition == LegionDir_Left) ? w_button : 0.0f;
    window->DC.CursorPos.x = pos.x + inputs_offset_x;

    if ((flags & (LegionColorEditFlags_DisplayRGB | LegionColorEditFlags_DisplayHSV)) != 0 && (flags & LegionColorEditFlags_NoInputs) == 0)
    {
        // RGB/HSV 0..255 Sliders
        const float w_item_one  = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components-1)) / (float)components));
        const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components-1)));

        const bool hide_prefix = (w_item_one <= CalcTextSize((flags & LegionColorEditFlags_Float) ? "M:0.000" : "M:000").x);
        static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
        static const char* fmt_table_int[3][4] =
        {
            {   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
            { "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
            { "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
        };
        static const char* fmt_table_float[3][4] =
        {
            {   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
            { "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
            { "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
        };
        const int fmt_idx = hide_prefix ? 0 : (flags & LegionColorEditFlags_DisplayHSV) ? 2 : 1;

        for (int n = 0; n < components; n++)
        {
            if (n > 0)
                SameLine(0, style.ItemInnerSpacing.x);
            SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

            // FIXME: When LegionColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
            if (flags & LegionColorEditFlags_Float)
            {
                value_changed |= DragFloat(ids[n], &f[n], 1.0f/255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                value_changed_as_float |= value_changed;
            }
            else
            {
                value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
            }
            if (!(flags & LegionColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context");
        }
    }
    else if ((flags & LegionColorEditFlags_DisplayHex) != 0 && (flags & LegionColorEditFlags_NoInputs) == 0)
    {
        // RGB Hexadecimal Input
        char buf[64];
        if (alpha)
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0],0,255), ImClamp(i[1],0,255), ImClamp(i[2],0,255), ImClamp(i[3],0,255));
        else
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0],0,255), ImClamp(i[1],0,255), ImClamp(i[2],0,255));
        SetNextItemWidth(w_inputs);
        if (InputText("##Text", buf, IM_ARRAYSIZE(buf), LegionInputTextFlags_CharsHexadecimal | LegionInputTextFlags_CharsUppercase))
        {
            value_changed = true;
            char* p = buf;
            while (*p == '#' || ImCharIsBlankA(*p))
                p++;
            i[0] = i[1] = i[2] = i[3] = 0;
            if (alpha)
                sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
            else
                sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
        }
        if (!(flags & LegionColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context");
    }

    LegionWindow* picker_active_window = NULL;
    if (!(flags & LegionColorEditFlags_NoSmallPreview))
    {
        const float button_offset_x = ((flags & LegionColorEditFlags_NoInputs) || (style.ColorButtonPosition == LegionDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

        const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
        if (ColorButton("##ColorButton", col_v4, flags))
        {
            if (!(flags & LegionColorEditFlags_NoPicker))
            {
                // Store current color and open a picker
                g.ColorPickerRef = col_v4;
                OpenPopup("picker");
                SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1,style.ItemSpacing.y));
            }
        }
        if (!(flags & LegionColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context");

        if (BeginPopup("picker"))
        {
            picker_active_window = g.CurrentWindow;
            if (label != label_display_end)
            {
                TextEx(label, label_display_end);
                Spacing();
            }
            LegionColorEditFlags picker_flags_to_forward = LegionColorEditFlags__DataTypeMask | LegionColorEditFlags__PickerMask | LegionColorEditFlags__InputMask | LegionColorEditFlags_HDR | LegionColorEditFlags_NoAlpha | LegionColorEditFlags_AlphaBar;
            LegionColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | LegionColorEditFlags__DisplayMask | LegionColorEditFlags_NoLabel | LegionColorEditFlags_AlphaPreviewHalf;
            SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
            value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
            EndPopup();
        }
    }

    if (label != label_display_end && !(flags & LegionColorEditFlags_NoLabel))
    {
        const float text_offset_x = (flags & LegionColorEditFlags_NoInputs) ? w_button : w_full + style.ItemInnerSpacing.x;
        window->DC.CursorPos = ImVec2(pos.x + text_offset_x, pos.y + style.FramePadding.y);
        TextEx(label, label_display_end);
    }

    // Convert back
    if (value_changed && picker_active_window == NULL)
    {
        if (!value_changed_as_float)
            for (int n = 0; n < 4; n++)
                f[n] = i[n] / 255.0f;
        if ((flags & LegionColorEditFlags_DisplayHSV) && (flags & LegionColorEditFlags_InputRGB))
        {
            g.ColorEditLastHue = f[0];
            g.ColorEditLastSat = f[1];
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
            memcpy(g.ColorEditLastColor, f, sizeof(float) * 3);
        }
        if ((flags & LegionColorEditFlags_DisplayRGB) && (flags & LegionColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

        col[0] = f[0];
        col[1] = f[1];
        col[2] = f[2];
        if (alpha)
            col[3] = f[3];
    }

    PopID();
    EndGroup();

    // Drag and Drop Target
    // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
    if ((window->DC.LastItemStatusFlags & LegionItemStatusFlags_HoveredRect) && !(flags & LegionColorEditFlags_NoDragDrop) && BeginDragDropTarget())
    {
        bool accepted_drag_drop = false;
        if (const LegionPayload* payload = AcceptDragDropPayload(Legion_PAYLOAD_TYPE_COLOR_3F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512
            value_changed = accepted_drag_drop = true;
        }
        if (const LegionPayload* payload = AcceptDragDropPayload(Legion_PAYLOAD_TYPE_COLOR_4F))
        {
            memcpy((float*)col, payload->Data, sizeof(float) * components);
            value_changed = accepted_drag_drop = true;
        }

        // Drag-drop payloads are always RGB
        if (accepted_drag_drop && (flags & LegionColorEditFlags_InputHSV))
            ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
        EndDragDropTarget();
    }

    // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
    if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
        window->DC.LastItemId = g.ActiveId;

    if (value_changed)
        MarkItemEdited(window->DC.LastItemId);

    return value_changed;
}

bool Legion::ColorPicker3(const char* label, float col[3], LegionColorEditFlags flags)
{
    float col4[4] = { col[0], col[1], col[2], 1.0f };
    if (!ColorPicker4(label, col4, flags | LegionColorEditFlags_NoAlpha))
        return false;
    col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
    return true;
}

// Helper for ColorPicker4()
static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
{
    ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
    Legion::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1,         pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), LegionDir_Right, IM_COL32(0,0,0,alpha8));
    Legion::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x,             pos.y), half_sz,                              LegionDir_Right, IM_COL32(255,255,255,alpha8));
    Legion::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), LegionDir_Left,  IM_COL32(0,0,0,alpha8));
    Legion::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x,     pos.y), half_sz,                              LegionDir_Left,  IM_COL32(255,255,255,alpha8));
}

// Note: ColorPicker4() only accesses 3 floats if LegionColorEditFlags_NoAlpha flag is set.
// (In C++ the 'float col[4]' notation for a function argument is equivalent to 'float* col', we only specify a size to facilitate understanding of the code.)
// FIXME: we adjust the big color square height based on item width, which may cause a flickering feedback loop (if automatic height makes a vertical scrollbar appears, affecting automatic width..)
// FIXME: this is trying to be aware of style.Alpha but not fully correct. Also, the color wheel will have overlapping glitches with (style.Alpha < 1.0)
bool Legion::ColorPicker4(const char* label, float col[4], LegionColorEditFlags flags, const float* ref_col)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImDrawList* draw_list = window->DrawList;
    LegionStyle& style = g.Style;
    LegionIO& io = g.IO;

    const float width = CalcItemWidth();
    g.NextItemData.ClearFlags();

    PushID(label);
    BeginGroup();

    if (!(flags & LegionColorEditFlags_NoSidePreview))
        flags |= LegionColorEditFlags_NoSmallPreview;

    // Context menu: display and store options.
    if (!(flags & LegionColorEditFlags_NoOptions))
        ColorPickerOptionsPopup(col, flags);

    // Read stored options
    if (!(flags & LegionColorEditFlags__PickerMask))
        flags |= ((g.ColorEditOptions & LegionColorEditFlags__PickerMask) ? g.ColorEditOptions : LegionColorEditFlags__OptionsDefault) & LegionColorEditFlags__PickerMask;
    if (!(flags & LegionColorEditFlags__InputMask))
        flags |= ((g.ColorEditOptions & LegionColorEditFlags__InputMask) ? g.ColorEditOptions : LegionColorEditFlags__OptionsDefault) & LegionColorEditFlags__InputMask;
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__PickerMask)); // Check that only 1 is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__InputMask));  // Check that only 1 is selected
    if (!(flags & LegionColorEditFlags_NoOptions))
        flags |= (g.ColorEditOptions & LegionColorEditFlags_AlphaBar);

    // Setup
    int components = (flags & LegionColorEditFlags_NoAlpha) ? 3 : 4;
    bool alpha_bar = (flags & LegionColorEditFlags_AlphaBar) && !(flags & LegionColorEditFlags_NoAlpha);
    ImVec2 picker_pos = window->DC.CursorPos;
    float square_sz = GetFrameHeight();
    float bars_width = square_sz; // Arbitrary smallish width of Hue/Alpha picking bars
    float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
    float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
    float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
    float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

    float backup_initial_col[4];
    memcpy(backup_initial_col, col, components * sizeof(float));

    float wheel_thickness = sv_picker_size * 0.08f;
    float wheel_r_outer = sv_picker_size * 0.50f;
    float wheel_r_inner = wheel_r_outer - wheel_thickness;
    ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width)*0.5f, picker_pos.y + sv_picker_size*0.5f);

    // Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
    float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
    ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
    ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
    ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

    float H = col[0], S = col[1], V = col[2];
    float R = col[0], G = col[1], B = col[2];
    if (flags & LegionColorEditFlags_InputRGB)
    {
        // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
        ColorConvertRGBtoHSV(R, G, B, H, S, V);
        if (memcmp(g.ColorEditLastColor, col, sizeof(float) * 3) == 0)
        {
            if (S == 0)
                H = g.ColorEditLastHue;
            if (V == 0)
                S = g.ColorEditLastSat;
        }
    }
    else if (flags & LegionColorEditFlags_InputHSV)
    {
        ColorConvertHSVtoRGB(H, S, V, R, G, B);
    }

    bool value_changed = false, value_changed_h = false, value_changed_sv = false;

    PushItemFlag(LegionItemFlags_NoNav, true);
    if (flags & LegionColorEditFlags_PickerHueWheel)
    {
        // Hue wheel + SV triangle logic
        InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
        if (IsItemActive())
        {
            ImVec2 initial_off = g.IO.MouseClickedPos[0] - wheel_center;
            ImVec2 current_off = g.IO.MousePos - wheel_center;
            float initial_dist2 = ImLengthSqr(initial_off);
            if (initial_dist2 >= (wheel_r_inner-1)*(wheel_r_inner-1) && initial_dist2 <= (wheel_r_outer+1)*(wheel_r_outer+1))
            {
                // Interactive with Hue wheel
                H = ImAtan2(current_off.y, current_off.x) / IM_PI*0.5f;
                if (H < 0.0f)
                    H += 1.0f;
                value_changed = value_changed_h = true;
            }
            float cos_hue_angle = ImCos(-H * 2.0f * IM_PI);
            float sin_hue_angle = ImSin(-H * 2.0f * IM_PI);
            if (ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, ImRotate(initial_off, cos_hue_angle, sin_hue_angle)))
            {
                // Interacting with SV triangle
                ImVec2 current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
                if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated))
                    current_off_unrotated = ImTriangleClosestPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated);
                float uu, vv, ww;
                ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
                V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
                S = ImClamp(uu / V, 0.0001f, 1.0f);
                value_changed = value_changed_sv = true;
            }
        }
        if (!(flags & LegionColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context");
    }
    else if (flags & LegionColorEditFlags_PickerHueBar)
    {
        // SV rectangle logic
        InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
        if (IsItemActive())
        {
            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size-1));
            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = value_changed_sv = true;
        }
        if (!(flags & LegionColorEditFlags_NoOptions))
            OpenPopupOnItemClick("context");

        // Hue bar logic
        SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
        InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = value_changed_h = true;
        }
    }

    // Alpha bar logic
    if (alpha_bar)
    {
        SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
        InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
        if (IsItemActive())
        {
            col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size-1));
            value_changed = true;
        }
    }
    PopItemFlag(); // LegionItemFlags_NoNav

    if (!(flags & LegionColorEditFlags_NoSidePreview))
    {
        SameLine(0, style.ItemInnerSpacing.x);
        BeginGroup();
    }

    if (!(flags & LegionColorEditFlags_NoLabel))
    {
        const char* label_display_end = FindRenderedTextEnd(label);
        if (label != label_display_end)
        {
            if ((flags & LegionColorEditFlags_NoSidePreview))
                SameLine(0, style.ItemInnerSpacing.x);
            TextEx(label, label_display_end);
        }
    }

    if (!(flags & LegionColorEditFlags_NoSidePreview))
    {
        PushItemFlag(LegionItemFlags_NoNavDefaultFocus, true);
        ImVec4 col_v4(col[0], col[1], col[2], (flags & LegionColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if ((flags & LegionColorEditFlags_NoLabel))
            Text("Current");

        LegionColorEditFlags sub_flags_to_forward = LegionColorEditFlags__InputMask | LegionColorEditFlags_HDR | LegionColorEditFlags_AlphaPreview | LegionColorEditFlags_AlphaPreviewHalf | LegionColorEditFlags_NoTooltip;
        ColorButton("##current", col_v4, (flags & sub_flags_to_forward), ImVec2(square_sz * 3, square_sz * 2));
        if (ref_col != NULL)
        {
            Text("Original");
            ImVec4 ref_col_v4(ref_col[0], ref_col[1], ref_col[2], (flags & LegionColorEditFlags_NoAlpha) ? 1.0f : ref_col[3]);
            if (ColorButton("##original", ref_col_v4, (flags & sub_flags_to_forward), ImVec2(square_sz * 3, square_sz * 2)))
            {
                memcpy(col, ref_col, components * sizeof(float));
                value_changed = true;
            }
        }
        PopItemFlag();
        EndGroup();
    }

    // Convert back color to RGB
    if (value_changed_h || value_changed_sv)
    {
        if (flags & LegionColorEditFlags_InputRGB)
        {
            ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10*1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);
            g.ColorEditLastHue = H;
            g.ColorEditLastSat = S;
            memcpy(g.ColorEditLastColor, col, sizeof(float) * 3);
        }
        else if (flags & LegionColorEditFlags_InputHSV)
        {
            col[0] = H;
            col[1] = S;
            col[2] = V;
        }
    }

    // R,G,B and H,S,V slider color editor
    bool value_changed_fix_hue_wrap = false;
    if ((flags & LegionColorEditFlags_NoInputs) == 0)
    {
        PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
        LegionColorEditFlags sub_flags_to_forward = LegionColorEditFlags__DataTypeMask | LegionColorEditFlags__InputMask | LegionColorEditFlags_HDR | LegionColorEditFlags_NoAlpha | LegionColorEditFlags_NoOptions | LegionColorEditFlags_NoSmallPreview | LegionColorEditFlags_AlphaPreview | LegionColorEditFlags_AlphaPreviewHalf;
        LegionColorEditFlags sub_flags = (flags & sub_flags_to_forward) | LegionColorEditFlags_NoPicker;
        if (flags & LegionColorEditFlags_DisplayRGB || (flags & LegionColorEditFlags__DisplayMask) == 0)
            if (ColorEdit4("##rgb", col, sub_flags | LegionColorEditFlags_DisplayRGB))
            {
                // FIXME: Hackily differenciating using the DragInt (ActiveId != 0 && !ActiveIdAllowOverlap) vs. using the InputText or DropTarget.
                // For the later we don't want to run the hue-wrap canceling code. If you are well versed in HSV picker please provide your input! (See #2050)
                value_changed_fix_hue_wrap = (g.ActiveId != 0 && !g.ActiveIdAllowOverlap);
                value_changed = true;
            }
        if (flags & LegionColorEditFlags_DisplayHSV || (flags & LegionColorEditFlags__DisplayMask) == 0)
            value_changed |= ColorEdit4("##hsv", col, sub_flags | LegionColorEditFlags_DisplayHSV);
        if (flags & LegionColorEditFlags_DisplayHex || (flags & LegionColorEditFlags__DisplayMask) == 0)
            value_changed |= ColorEdit4("##hex", col, sub_flags | LegionColorEditFlags_DisplayHex);
        PopItemWidth();
    }

    // Try to cancel hue wrap (after ColorEdit4 call), if any
    if (value_changed_fix_hue_wrap && (flags & LegionColorEditFlags_InputRGB))
    {
        float new_H, new_S, new_V;
        ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
        if (new_H <= 0 && H > 0)
        {
            if (new_V <= 0 && V != new_V)
                ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
            else if (new_S <= 0)
                ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
        }
    }

    if (value_changed)
    {
        if (flags & LegionColorEditFlags_InputRGB)
        {
            R = col[0];
            G = col[1];
            B = col[2];
            ColorConvertRGBtoHSV(R, G, B, H, S, V);
            if (memcmp(g.ColorEditLastColor, col, sizeof(float) * 3) == 0) // Fix local Hue as display below will use it immediately.
            {
                if (S == 0)
                    H = g.ColorEditLastHue;
                if (V == 0)
                    S = g.ColorEditLastSat;
            }
        }
        else if (flags & LegionColorEditFlags_InputHSV)
        {
            H = col[0];
            S = col[1];
            V = col[2];
            ColorConvertHSVtoRGB(H, S, V, R, G, B);
        }
    }

    const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
    const ImU32 col_black = IM_COL32(0,0,0,style_alpha8);
    const ImU32 col_white = IM_COL32(255,255,255,style_alpha8);
    const ImU32 col_midgrey = IM_COL32(128,128,128,style_alpha8);
    const ImU32 col_hues[6 + 1] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(0,255,255,style_alpha8), IM_COL32(0,0,255,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

    ImVec4 hue_color_f(1, 1, 1, style.Alpha); ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
    ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);
    ImU32 user_col32_striped_of_alpha = ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!

    ImVec2 sv_cursor_pos;

    if (flags & LegionColorEditFlags_PickerHueWheel)
    {
        // Render Hue Wheel
        const float aeps = 0.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
        const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
        for (int n = 0; n < 6; n++)
        {
            const float a0 = (n)     /6.0f * 2.0f * IM_PI - aeps;
            const float a1 = (n+1.0f)/6.0f * 2.0f * IM_PI + aeps;
            const int vert_start_idx = draw_list->VtxBuffer.Size;
            draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer)*0.5f, a0, a1, segment_per_arc);
            draw_list->PathStroke(col_white, false, wheel_thickness);
            const int vert_end_idx = draw_list->VtxBuffer.Size;

            // Paint colors over existing vertices
            ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
            ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
            ShadeVertsLinearColorGradientKeepAlpha(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n+1]);
        }

        // Render Cursor + preview on Hue Wheel
        float cos_hue_angle = ImCos(H * 2.0f * IM_PI);
        float sin_hue_angle = ImSin(H * 2.0f * IM_PI);
        ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner+wheel_r_outer)*0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner+wheel_r_outer)*0.5f);
        float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
        int hue_cursor_segments = ImClamp((int)(hue_cursor_rad / 1.4f), 9, 32);
        draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad+1, col_midgrey, hue_cursor_segments);
        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments);

        // Render SV triangle (rotated according to hue)
        ImVec2 tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
        ImVec2 trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
        ImVec2 trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
        ImVec2 uv_white = GetFontTexUvWhitePixel();
        draw_list->PrimReserve(6, 6);
        draw_list->PrimVtx(tra, uv_white, hue_color32);
        draw_list->PrimVtx(trb, uv_white, hue_color32);
        draw_list->PrimVtx(trc, uv_white, col_white);
        draw_list->PrimVtx(tra, uv_white, 0);
        draw_list->PrimVtx(trb, uv_white, col_black);
        draw_list->PrimVtx(trc, uv_white, 0);
        draw_list->AddTriangle(tra, trb, trc, col_midgrey, 1.5f);
        sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
    }
    else if (flags & LegionColorEditFlags_PickerHueBar)
    {
        // Render SV Square
        draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), col_white, hue_color32, hue_color32, col_white);
        draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0, 0, col_black, col_black);
        RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0.0f);
        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S)     * sv_picker_size), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

        // Render Hue Bar
        for (int i = 0; i < 6; ++i)
            draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)), col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1]);
        float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
        RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y), ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size), 0.0f);
        RenderArrowsForVerticalBar(draw_list, ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f, style.Alpha);
    }

    // Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
    float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
    draw_list->AddCircleFilled(sv_cursor_pos, sv_cursor_rad, user_col32_striped_of_alpha, 12);
    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad+1, col_midgrey, 12);
    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad, col_white, 12);

    // Render alpha bar
    if (alpha_bar)
    {
        float alpha = ImSaturate(col[3]);
        ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
        RenderColorRectWithAlphaCheckerboard(draw_list, bar1_bb.Min, bar1_bb.Max, 0, bar1_bb.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
        draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, user_col32_striped_of_alpha, user_col32_striped_of_alpha, user_col32_striped_of_alpha & ~IM_COL32_A_MASK, user_col32_striped_of_alpha & ~IM_COL32_A_MASK);
        float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);
        RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
        RenderArrowsForVerticalBar(draw_list, ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f, style.Alpha);
    }

    EndGroup();

    if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0)
        value_changed = false;
    if (value_changed)
        MarkItemEdited(window->DC.LastItemId);

    PopID();

    return value_changed;
}

// A little colored square. Return true when clicked.
// FIXME: May want to display/ignore the alpha component in the color display? Yet show it in the tooltip.
// 'desc_id' is not called 'label' because we don't display it next to the button, but only in the tooltip.
// Note that 'col' may be encoded in HSV if LegionColorEditFlags_InputHSV is set.
bool Legion::ColorButton(const char* desc_id, const ImVec4& col, LegionColorEditFlags flags, ImVec2 size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionID id = window->GetID(desc_id);
    float default_size = GetFrameHeight();
    if (size.x == 0.0f)
        size.x = default_size;
    if (size.y == 0.0f)
        size.y = default_size;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (flags & LegionColorEditFlags_NoAlpha)
        flags &= ~(LegionColorEditFlags_AlphaPreview | LegionColorEditFlags_AlphaPreviewHalf);

    ImVec4 col_rgb = col;
    if (flags & LegionColorEditFlags_InputHSV)
        ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);

    ImVec4 col_rgb_without_alpha(col_rgb.x, col_rgb.y, col_rgb.z, 1.0f);
    float grid_step = ImMin(size.x, size.y) / 2.99f;
    float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
    ImRect bb_inner = bb;
    float off = 0.0f;
    if ((flags & LegionColorEditFlags_NoBorder) == 0)
    {
        off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
        bb_inner.Expand(off);
    }
    if ((flags & LegionColorEditFlags_AlphaPreviewHalf) && col_rgb.w < 1.0f)
    {
        float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
        RenderColorRectWithAlphaCheckerboard(window->DrawList, ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, GetColorU32(col_rgb), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawCornerFlags_TopRight| ImDrawCornerFlags_BotRight);
        window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), GetColorU32(col_rgb_without_alpha), rounding, ImDrawCornerFlags_TopLeft|ImDrawCornerFlags_BotLeft);
    }
    else
    {
        // Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
        ImVec4 col_source = (flags & LegionColorEditFlags_AlphaPreview) ? col_rgb : col_rgb_without_alpha;
        if (col_source.w < 1.0f)
            RenderColorRectWithAlphaCheckerboard(window->DrawList, bb_inner.Min, bb_inner.Max, GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
        else
            window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, GetColorU32(col_source), rounding, ImDrawCornerFlags_All);
    }
    RenderNavHighlight(bb, id);
    if ((flags & LegionColorEditFlags_NoBorder) == 0)
    {
        if (g.Style.FrameBorderSize > 0.0f)
            RenderFrameBorder(bb.Min, bb.Max, rounding);
        else
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(LegionCol_FrameBg), rounding); // Color button are often in need of some sort of border
    }

    // Drag and Drop Source
    // NB: The ActiveId test is merely an optional micro-optimization, BeginDragDropSource() does the same test.
    if (g.ActiveId == id && !(flags & LegionColorEditFlags_NoDragDrop) && BeginDragDropSource())
    {
        if (flags & LegionColorEditFlags_NoAlpha)
            SetDragDropPayload(Legion_PAYLOAD_TYPE_COLOR_3F, &col_rgb, sizeof(float) * 3, LegionCond_Once);
        else
            SetDragDropPayload(Legion_PAYLOAD_TYPE_COLOR_4F, &col_rgb, sizeof(float) * 4, LegionCond_Once);
        ColorButton(desc_id, col, flags);
        SameLine();
        TextEx("Color");
        EndDragDropSource();
    }

    // Tooltip
    if (!(flags & LegionColorEditFlags_NoTooltip) && hovered)
        ColorTooltip(desc_id, &col.x, flags & (LegionColorEditFlags__InputMask | LegionColorEditFlags_NoAlpha | LegionColorEditFlags_AlphaPreview | LegionColorEditFlags_AlphaPreviewHalf));

    return pressed;
}

// Initialize/override default color options
void Legion::SetColorEditOptions(LegionColorEditFlags flags)
{
    LegionContext& g = *GLegion;
    if ((flags & LegionColorEditFlags__DisplayMask) == 0)
        flags |= LegionColorEditFlags__OptionsDefault & LegionColorEditFlags__DisplayMask;
    if ((flags & LegionColorEditFlags__DataTypeMask) == 0)
        flags |= LegionColorEditFlags__OptionsDefault & LegionColorEditFlags__DataTypeMask;
    if ((flags & LegionColorEditFlags__PickerMask) == 0)
        flags |= LegionColorEditFlags__OptionsDefault & LegionColorEditFlags__PickerMask;
    if ((flags & LegionColorEditFlags__InputMask) == 0)
        flags |= LegionColorEditFlags__OptionsDefault & LegionColorEditFlags__InputMask;
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__DisplayMask));    // Check only 1 option is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__DataTypeMask));   // Check only 1 option is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__PickerMask));     // Check only 1 option is selected
    IM_ASSERT(ImIsPowerOfTwo(flags & LegionColorEditFlags__InputMask));      // Check only 1 option is selected
    g.ColorEditOptions = flags;
}

// Note: only access 3 floats if LegionColorEditFlags_NoAlpha flag is set.
void Legion::ColorTooltip(const char* text, const float* col, LegionColorEditFlags flags)
{
    LegionContext& g = *GLegion;

    BeginTooltipEx(0, LegionTooltipFlags_OverridePreviousTooltip);
    const char* text_end = text ? FindRenderedTextEnd(text, NULL) : text;
    if (text_end > text)
    {
        TextEx(text, text_end);
        Separator();
    }

    ImVec2 sz(g.FontSize * 3 + g.Style.FramePadding.y * 2, g.FontSize * 3 + g.Style.FramePadding.y * 2);
    ImVec4 cf(col[0], col[1], col[2], (flags & LegionColorEditFlags_NoAlpha) ? 1.0f : col[3]);
    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & LegionColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
    ColorButton("##preview", cf, (flags & (LegionColorEditFlags__InputMask | LegionColorEditFlags_NoAlpha | LegionColorEditFlags_AlphaPreview | LegionColorEditFlags_AlphaPreviewHalf)) | LegionColorEditFlags_NoTooltip, sz);
    SameLine();
    if ((flags & LegionColorEditFlags_InputRGB) || !(flags & LegionColorEditFlags__InputMask))
    {
        if (flags & LegionColorEditFlags_NoAlpha)
            Text("#%02X%02X%02X\nR: %d, G: %d, B: %d\n(%.3f, %.3f, %.3f)", cr, cg, cb, cr, cg, cb, col[0], col[1], col[2]);
        else
            Text("#%02X%02X%02X%02X\nR:%d, G:%d, B:%d, A:%d\n(%.3f, %.3f, %.3f, %.3f)", cr, cg, cb, ca, cr, cg, cb, ca, col[0], col[1], col[2], col[3]);
    }
    else if (flags & LegionColorEditFlags_InputHSV)
    {
        if (flags & LegionColorEditFlags_NoAlpha)
            Text("H: %.3f, S: %.3f, V: %.3f", col[0], col[1], col[2]);
        else
            Text("H: %.3f, S: %.3f, V: %.3f, A: %.3f", col[0], col[1], col[2], col[3]);
    }
    EndTooltip();
}

void Legion::ColorEditOptionsPopup(const float* col, LegionColorEditFlags flags)
{
    bool allow_opt_inputs = !(flags & LegionColorEditFlags__DisplayMask);
    bool allow_opt_datatype = !(flags & LegionColorEditFlags__DataTypeMask);
    if ((!allow_opt_inputs && !allow_opt_datatype) || !BeginPopup("context"))
        return;
    LegionContext& g = *GLegion;
    LegionColorEditFlags opts = g.ColorEditOptions;
    if (allow_opt_inputs)
    {
        if (RadioButton("RGB", (opts & LegionColorEditFlags_DisplayRGB) != 0)) opts = (opts & ~LegionColorEditFlags__DisplayMask) | LegionColorEditFlags_DisplayRGB;
        if (RadioButton("HSV", (opts & LegionColorEditFlags_DisplayHSV) != 0)) opts = (opts & ~LegionColorEditFlags__DisplayMask) | LegionColorEditFlags_DisplayHSV;
        if (RadioButton("Hex", (opts & LegionColorEditFlags_DisplayHex) != 0)) opts = (opts & ~LegionColorEditFlags__DisplayMask) | LegionColorEditFlags_DisplayHex;
    }
    if (allow_opt_datatype)
    {
        if (allow_opt_inputs) Separator();
        if (RadioButton("0..255",     (opts & LegionColorEditFlags_Uint8) != 0)) opts = (opts & ~LegionColorEditFlags__DataTypeMask) | LegionColorEditFlags_Uint8;
        if (RadioButton("0.00..1.00", (opts & LegionColorEditFlags_Float) != 0)) opts = (opts & ~LegionColorEditFlags__DataTypeMask) | LegionColorEditFlags_Float;
    }

    if (allow_opt_inputs || allow_opt_datatype)
        Separator();
    if (Button("Copy as..", ImVec2(-1,0)))
        OpenPopup("Copy");
    if (BeginPopup("Copy"))
    {
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & LegionColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
        char buf[64];
        ImFormatString(buf, IM_ARRAYSIZE(buf), "(%.3ff, %.3ff, %.3ff, %.3ff)", col[0], col[1], col[2], (flags & LegionColorEditFlags_NoAlpha) ? 1.0f : col[3]);
        if (Selectable(buf))
            SetClipboardText(buf);
        ImFormatString(buf, IM_ARRAYSIZE(buf), "(%d,%d,%d,%d)", cr, cg, cb, ca);
        if (Selectable(buf))
            SetClipboardText(buf);
        ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", cr, cg, cb);
        if (Selectable(buf))
            SetClipboardText(buf);
        if (!(flags & LegionColorEditFlags_NoAlpha))
        {
            ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", cr, cg, cb, ca);
            if (Selectable(buf))
                SetClipboardText(buf);
        }
        EndPopup();
    }

    g.ColorEditOptions = opts;
    EndPopup();
}

void Legion::ColorPickerOptionsPopup(const float* ref_col, LegionColorEditFlags flags)
{
    bool allow_opt_picker = !(flags & LegionColorEditFlags__PickerMask);
    bool allow_opt_alpha_bar = !(flags & LegionColorEditFlags_NoAlpha) && !(flags & LegionColorEditFlags_AlphaBar);
    if ((!allow_opt_picker && !allow_opt_alpha_bar) || !BeginPopup("context"))
        return;
    LegionContext& g = *GLegion;
    if (allow_opt_picker)
    {
        ImVec2 picker_size(g.FontSize * 8, ImMax(g.FontSize * 8 - (GetFrameHeight() + g.Style.ItemInnerSpacing.x), 1.0f)); // FIXME: Picker size copied from main picker function
        PushItemWidth(picker_size.x);
        for (int picker_type = 0; picker_type < 2; picker_type++)
        {
            // Draw small/thumbnail version of each picker type (over an invisible button for selection)
            if (picker_type > 0) Separator();
            PushID(picker_type);
            LegionColorEditFlags picker_flags = LegionColorEditFlags_NoInputs|LegionColorEditFlags_NoOptions|LegionColorEditFlags_NoLabel|LegionColorEditFlags_NoSidePreview|(flags & LegionColorEditFlags_NoAlpha);
            if (picker_type == 0) picker_flags |= LegionColorEditFlags_PickerHueBar;
            if (picker_type == 1) picker_flags |= LegionColorEditFlags_PickerHueWheel;
            ImVec2 backup_pos = GetCursorScreenPos();
            if (Selectable("##selectable", false, 0, picker_size)) // By default, Selectable() is closing popup
                g.ColorEditOptions = (g.ColorEditOptions & ~LegionColorEditFlags__PickerMask) | (picker_flags & LegionColorEditFlags__PickerMask);
            SetCursorScreenPos(backup_pos);
            ImVec4 dummy_ref_col;
            memcpy(&dummy_ref_col, ref_col, sizeof(float) * ((picker_flags & LegionColorEditFlags_NoAlpha) ? 3 : 4));
            ColorPicker4("##dummypicker", &dummy_ref_col.x, picker_flags);
            PopID();
        }
        PopItemWidth();
    }
    if (allow_opt_alpha_bar)
    {
        if (allow_opt_picker) Separator();
        CheckboxFlags("Alpha Bar", (unsigned int*)&g.ColorEditOptions, LegionColorEditFlags_AlphaBar);
    }
    EndPopup();
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: TreeNode, CollapsingHeader, etc.
//-------------------------------------------------------------------------
// - TreeNode()
// - TreeNodeV()
// - TreeNodeEx()
// - TreeNodeExV()
// - TreeNodeBehavior() [Internal]
// - TreePush()
// - TreePop()
// - GetTreeNodeToLabelSpacing()
// - SetNextItemOpen()
// - CollapsingHeader()
//-------------------------------------------------------------------------

bool Legion::TreeNode(const char* str_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool Legion::TreeNode(const void* ptr_id, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
    va_end(args);
    return is_open;
}

bool Legion::TreeNode(const char* label)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    return TreeNodeBehavior(window->GetID(label), 0, label, NULL);
}

bool Legion::TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
    return TreeNodeExV(str_id, 0, fmt, args);
}

bool Legion::TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
    return TreeNodeExV(ptr_id, 0, fmt, args);
}

bool Legion::TreeNodeEx(const char* label, LegionTreeNodeFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return TreeNodeBehavior(window->GetID(label), flags, label, NULL);
}

bool Legion::TreeNodeEx(const char* str_id, LegionTreeNodeFlags flags, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(str_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool Legion::TreeNodeEx(const void* ptr_id, LegionTreeNodeFlags flags, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
    va_end(args);
    return is_open;
}

bool Legion::TreeNodeExV(const char* str_id, LegionTreeNodeFlags flags, const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(str_id), flags, g.TempBuffer, label_end);
}

bool Legion::TreeNodeExV(const void* ptr_id, LegionTreeNodeFlags flags, const char* fmt, va_list args)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const char* label_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    return TreeNodeBehavior(window->GetID(ptr_id), flags, g.TempBuffer, label_end);
}

bool Legion::TreeNodeBehaviorIsOpen(LegionID id, LegionTreeNodeFlags flags)
{
    if (flags & LegionTreeNodeFlags_Leaf)
        return true;

    // We only write to the tree storage if the user clicks (or explicitly use the SetNextItemOpen function)
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    LegionStorage* storage = window->DC.StateStorage;

    bool is_open;
    if (g.NextItemData.Flags & LegionNextItemDataFlags_HasOpen)
    {
        if (g.NextItemData.OpenCond & LegionCond_Always)
        {
            is_open = g.NextItemData.OpenVal;
            storage->SetInt(id, is_open);
        }
        else
        {
            // We treat LegionCond_Once and LegionCond_FirstUseEver the same because tree node state are not saved persistently.
            const int stored_value = storage->GetInt(id, -1);
            if (stored_value == -1)
            {
                is_open = g.NextItemData.OpenVal;
                storage->SetInt(id, is_open);
            }
            else
            {
                is_open = stored_value != 0;
            }
        }
    }
    else
    {
        is_open = storage->GetInt(id, (flags & LegionTreeNodeFlags_DefaultOpen) ? 1 : 0) != 0;
    }

    // When logging is enabled, we automatically expand tree nodes (but *NOT* collapsing headers.. seems like sensible behavior).
    // NB- If we are above max depth we still allow manually opened nodes to be logged.
    if (g.LogEnabled && !(flags & LegionTreeNodeFlags_NoAutoOpenOnLog) && (window->DC.TreeDepth - g.LogDepthRef) < g.LogDepthToExpand)
        is_open = true;

    return is_open;
}

bool Legion::TreeNodeBehavior(LegionID id, LegionTreeNodeFlags flags, const char* label, const char* label_end)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const bool display_frame = (flags & LegionTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & LegionTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));

    if (!label_end)
        label_end = FindRenderedTextEnd(label);
    const ImVec2 label_size = CalcTextSize(label, label_end, false);

    // We vertically grow up to current line height up the typical widget height.
    const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y*2), label_size.y + padding.y*2);
    ImRect frame_bb;
    frame_bb.Min.x = (flags & LegionTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame)
    {
        // Framed header expand a little outside the default padding, to the edge of InnerClipRect
        // (FIXME: May remove this at some point and make InnerClipRect align with WindowPadding.x instead of WindowPadding.x*0.5f)
        frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
        frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
    }

    const float text_offset_x = g.FontSize + (display_frame ? padding.x*3 : padding.x*2);               // Collapser arrow width + Spacing
    const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x*2 : 0.0f);    // Include collapser
    ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
    ItemSize(ImVec2(text_width, frame_height), padding.y);

    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if (!display_frame && (flags & (LegionTreeNodeFlags_SpanAvailWidth | LegionTreeNodeFlags_SpanFullWidth)) == 0)
        interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;

    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    const bool is_leaf = (flags & LegionTreeNodeFlags_Leaf) != 0;
    bool is_open = TreeNodeBehaviorIsOpen(id, flags);
    if (is_open && !g.NavIdIsAlive && (flags & LegionTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & LegionTreeNodeFlags_NoTreePushOnOpen))
        window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);

    bool item_add = ItemAdd(interact_bb, id);
    window->DC.LastItemStatusFlags |= LegionItemStatusFlags_HasDisplayRect;
    window->DC.LastItemDisplayRect = frame_bb;

    if (!item_add)
    {
        if (is_open && !(flags & LegionTreeNodeFlags_NoTreePushOnOpen))
            TreePushOverrideID(id);
        Legion_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | (is_leaf ? 0 : LegionItemStatusFlags_Openable) | (is_open ? LegionItemStatusFlags_Opened : 0));
        return is_open;
    }

    LegionButtonFlags button_flags = LegionTreeNodeFlags_None;
    if (flags & LegionTreeNodeFlags_AllowItemOverlap)
        button_flags |= LegionButtonFlags_AllowItemOverlap;
    if (!is_leaf)
        button_flags |= LegionButtonFlags_PressedOnDragDropHold;

    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
    // When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
    if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
        button_flags |= LegionButtonFlags_NoKeyModifiers;

    // Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
    // Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
    // - Single-click on label = Toggle on MouseUp (default)
    // - Single-click on arrow = Toggle on MouseUp (when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
    // - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
    // - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
    // This makes _OpenOnArrow have a subtle effect on _OpenOnDoubleClick: arrow click reacts on Down rather than Up.
    // It is rather standard that arrow click react on Down rather than Up and we'd be tempted to make it the default 
    // (by removing the _OpenOnArrow test below), however this would have a perhaps surprising effect on CollapsingHeader()? 
    // So right now we are making this optional. May evolve later.
    if (is_mouse_x_over_arrow && (flags & LegionTreeNodeFlags_OpenOnArrow))
        button_flags |= LegionButtonFlags_PressedOnClick;
    else if (flags & LegionTreeNodeFlags_OpenOnDoubleClick)
        button_flags |= LegionButtonFlags_PressedOnDoubleClick;
    else
        button_flags |= LegionButtonFlags_PressedOnClickRelease;

    bool selected = (flags & LegionTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;

    bool hovered, held;
    bool pressed = ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
        if (pressed)
        {
            if ((flags & (LegionTreeNodeFlags_OpenOnArrow | LegionTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
                toggled = true;
            if (flags & LegionTreeNodeFlags_OpenOnArrow)
                toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
            if ((flags & LegionTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
                toggled = true;
            if (g.DragDropActive && is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
                toggled = false;
        }

        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Left && is_open)
        {
            toggled = true;
            NavMoveRequestCancel();
        }
        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
        {
            toggled = true;
            NavMoveRequestCancel();
        }

        if (toggled)
        {
            is_open = !is_open;
            window->DC.StateStorage->SetInt(id, is_open);
            window->DC.LastItemStatusFlags |= LegionItemStatusFlags_ToggledOpen;
        }
    }
    if (flags & LegionTreeNodeFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, TreeNodeBehavior() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        window->DC.LastItemStatusFlags |= LegionItemStatusFlags_ToggledSelection;

    // Render
    const ImU32 text_col = GetColorU32(LegionCol_Text);
    LegionNavHighlightFlags nav_highlight_flags = LegionNavHighlightFlags_TypeThin;
    if (display_frame)
    {
        // Framed type
        const ImU32 bg_col = GetColorU32((held && hovered) ? LegionCol_HeaderActive : hovered ? LegionCol_HeaderHovered : LegionCol_Header);
        RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
        RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        if (flags & LegionTreeNodeFlags_Bullet)
            RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), text_col);
        else if (!is_leaf)
            RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), text_col, is_open ? LegionDir_Down : LegionDir_Right, 1.0f);
        else // Leaf without bullet, left-adjusted text
            text_pos.x -= text_offset_x;
        if (flags & LegionTreeNodeFlags_ClipLabelForTrailingButton)
            frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
        if (g.LogEnabled)
        {
            // NB: '##' is normally used to hide text (as a library-wide feature), so we need to specify the text range to make sure the ## aren't stripped out here.
            const char log_prefix[] = "\n##";
            const char log_suffix[] = "##";
            LogRenderedText(&text_pos, log_prefix, log_prefix+3);
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
            LogRenderedText(&text_pos, log_suffix, log_suffix+2);
        }
        else
        {
            RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        }
    }
    else
    {
        // Unframed typed for tree nodes
        if (hovered || selected)
        {
            const ImU32 bg_col = GetColorU32((held && hovered) ? LegionCol_HeaderActive : hovered ? LegionCol_HeaderHovered : LegionCol_Header);
            RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
            RenderNavHighlight(frame_bb, id, nav_highlight_flags);
        }
        if (flags & LegionTreeNodeFlags_Bullet)
            RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), text_col);
        else if (!is_leaf)
            RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), text_col, is_open ? LegionDir_Down : LegionDir_Right, 0.70f);
        if (g.LogEnabled)
            LogRenderedText(&text_pos, ">");
        RenderText(text_pos, label, label_end, false);
    }

    if (is_open && !(flags & LegionTreeNodeFlags_NoTreePushOnOpen))
        TreePushOverrideID(id);
    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (is_leaf ? 0 : LegionItemStatusFlags_Openable) | (is_open ? LegionItemStatusFlags_Opened : 0));
    return is_open;
}

void Legion::TreePush(const char* str_id)
{
    LegionWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(str_id ? str_id : "#TreePush");
}

void Legion::TreePush(const void* ptr_id)
{
    LegionWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    PushID(ptr_id ? ptr_id : (const void*)"#TreePush");
}

void Legion::TreePushOverrideID(LegionID id)
{
    LegionWindow* window = GetCurrentWindow();
    Indent();
    window->DC.TreeDepth++;
    window->IDStack.push_back(id);
}

void Legion::TreePop()
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    Unindent();

    window->DC.TreeDepth--;
    ImU32 tree_depth_mask = (1 << window->DC.TreeDepth);

    // Handle Left arrow to move to parent tree node (when LegionTreeNodeFlags_NavLeftJumpsBackHere is enabled)
    if (g.NavMoveDir == LegionDir_Left && g.NavWindow == window && NavMoveRequestButNoResultYet())
        if (g.NavIdIsAlive && (window->DC.TreeJumpToParentOnPopMask & tree_depth_mask))
        {
            SetNavID(window->IDStack.back(), g.NavLayer, 0);
            NavMoveRequestCancel();
        }
    window->DC.TreeJumpToParentOnPopMask &= tree_depth_mask - 1;

    IM_ASSERT(window->IDStack.Size > 1); // There should always be 1 element in the IDStack (pushed during window creation). If this triggers you called TreePop/PopID too much.
    PopID();
}

// Horizontal distance preceding label when using TreeNode() or Bullet()
float Legion::GetTreeNodeToLabelSpacing()
{
    LegionContext& g = *GLegion;
    return g.FontSize + (g.Style.FramePadding.x * 2.0f);
}

// Set next TreeNode/CollapsingHeader open state.
void Legion::SetNextItemOpen(bool is_open, LegionCond cond)
{
    LegionContext& g = *GLegion;
    if (g.CurrentWindow->SkipItems)
        return;
    g.NextItemData.Flags |= LegionNextItemDataFlags_HasOpen;
    g.NextItemData.OpenVal = is_open;
    g.NextItemData.OpenCond = cond ? cond : LegionCond_Always;
}

// CollapsingHeader returns true when opened but do not indent nor push into the ID stack (because of the LegionTreeNodeFlags_NoTreePushOnOpen flag).
// This is basically the same as calling TreeNodeEx(label, LegionTreeNodeFlags_CollapsingHeader). You can remove the _NoTreePushOnOpen flag if you want behavior closer to normal TreeNode().
bool Legion::CollapsingHeader(const char* label, LegionTreeNodeFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    return TreeNodeBehavior(window->GetID(label), flags | LegionTreeNodeFlags_CollapsingHeader, label);
}

bool Legion::CollapsingHeader(const char* label, bool* p_open, LegionTreeNodeFlags flags)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    if (p_open && !*p_open)
        return false;

    LegionID id = window->GetID(label);
    flags |= LegionTreeNodeFlags_CollapsingHeader;
    if (p_open)
        flags |= LegionTreeNodeFlags_AllowItemOverlap | LegionTreeNodeFlags_ClipLabelForTrailingButton;
    bool is_open = TreeNodeBehavior(id, flags, label);
    if (p_open)
    {
        // Create a small overlapping close button
        // FIXME: We can evolve this into user accessible helpers to add extra buttons on title bars, headers, etc.
        // FIXME: CloseButton can overlap into text, need find a way to clip the text somehow.
        LegionContext& g = *GLegion;
        LegionItemHoveredDataBackup last_item_backup;
        float button_size = g.FontSize;
        float button_x = ImMax(window->DC.LastItemRect.Min.x, window->DC.LastItemRect.Max.x - g.Style.FramePadding.x * 2.0f - button_size);
        float button_y = window->DC.LastItemRect.Min.y;
        if (CloseButton(window->GetID((void*)((intptr_t)id + 1)), ImVec2(button_x, button_y)))
            *p_open = false;
        last_item_backup.Restore();
    }

    return is_open;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: Selectable
//-------------------------------------------------------------------------
// - Selectable()
//-------------------------------------------------------------------------

// Tip: pass a non-visible label (e.g. "##dummy") then you can use the space to draw other text or image.
// But you need to make sure the ID is unique, e.g. enclose calls in PushID/PopID or use ##unique_id.
// With this scheme, LegionSelectableFlags_SpanAllColumns and LegionSelectableFlags_AllowItemOverlap are also frequently used flags.
// FIXME: Selectable() with (size.x == 0.0f) and (SelectableTextAlign.x > 0.0f) followed by SameLine() is currently not supported.
bool Legion::Selectable(const char* label, bool selected, LegionSelectableFlags flags, const ImVec2& size_arg)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns) // FIXME-OPT: Avoid if vertically clipped.
        PushColumnsBackground();

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    LegionID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    const float min_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Min.x : pos.x;
    const float max_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Max.x : GetContentRegionMaxAbs().x;
    if (size_arg.x == 0.0f || (flags & LegionSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb_enlarged(min_x, pos.y, text_max.x, text_max.y);
    const float spacing_x = style.ItemSpacing.x;
    const float spacing_y = style.ItemSpacing.y;
    const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
    const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
    bb_enlarged.Min.x -= spacing_L;
    bb_enlarged.Min.y -= spacing_U;
    bb_enlarged.Max.x += (spacing_x - spacing_L);
    bb_enlarged.Max.y += (spacing_y - spacing_U);
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_align.Min, bb_align.Max, IM_COL32(255, 0, 0, 255)); }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_enlarged.Min, bb_enlarged.Max, IM_COL32(0, 255, 0, 255)); }

    bool item_add;
    if (flags & LegionSelectableFlags_Disabled)
    {
        LegionItemFlags backup_item_flags = window->DC.ItemFlags;
        window->DC.ItemFlags |= LegionItemFlags_Disabled | LegionItemFlags_NoNavDefaultFocus;
        item_add = ItemAdd(bb_enlarged, id);
        window->DC.ItemFlags = backup_item_flags;
    }
    else
    {
        item_add = ItemAdd(bb_enlarged, id);
    }
    if (!item_add)
    {
        if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
            PopColumnsBackground();
        return false;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    LegionButtonFlags button_flags = 0;
    if (flags & LegionSelectableFlags_NoHoldingActiveID) { button_flags |= LegionButtonFlags_NoHoldingActiveId; }
    if (flags & LegionSelectableFlags_SelectOnClick)     { button_flags |= LegionButtonFlags_PressedOnClick; }
    if (flags & LegionSelectableFlags_SelectOnRelease)   { button_flags |= LegionButtonFlags_PressedOnRelease; }
    if (flags & LegionSelectableFlags_Disabled)          { button_flags |= LegionButtonFlags_Disabled; }
    if (flags & LegionSelectableFlags_AllowDoubleClick)  { button_flags |= LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnDoubleClick; }
    if (flags & LegionSelectableFlags_AllowItemOverlap)  { button_flags |= LegionButtonFlags_AllowItemOverlap; }

    if (flags & LegionSelectableFlags_Disabled)
        selected = false;

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_enlarged, id, &hovered, &held, button_flags);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & LegionSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        window->DC.LastItemStatusFlags |= LegionItemStatusFlags_ToggledSelection;

    // Render
    if (held && (flags & LegionSelectableFlags_DrawHoveredWhenHeld))
        hovered = true;
    if (hovered || selected)
    {
        const ImU32 col = GetColorU32((held && hovered) ? LegionCol_HeaderActive : hovered ? LegionCol_HeaderHovered : LegionCol_Header);
        RenderFrame(bb_enlarged.Min, bb_enlarged.Max, col, false, 0.0f);
        RenderNavHighlight(bb_enlarged, id, LegionNavHighlightFlags_TypeThin | LegionNavHighlightFlags_NoRounding);
    }

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
        PopColumnsBackground();

    if (flags & LegionSelectableFlags_Disabled) PushStyleColor(LegionCol_Text, style.Colors[LegionCol_TextDisabled]);
    RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb_enlarged);
    if (flags & LegionSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & LegionWindowFlags_Popup) && !(flags & LegionSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & LegionItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return pressed;
}

bool Legion::Selectable(const char* label, bool* p_selected, LegionSelectableFlags flags, const ImVec2& size_arg)
{
    if (Selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool Legion::SelectableV(ImFont* font, float fsize, const char* time, const char* nickname, const char* message/*, ImTextureID scroll, ImTextureID scrollbar*/, const ImVec2& size_arg)
{
   // BeginChildV(NULL, NULL, NULL, "ChatBox", size_arg);
    TextColorChatBox(font, fsize, time);
    SameLine();
    TextColorChatBox(font, fsize, nickname);
    SameLine();
    TextColorChatBox(font, fsize, message);
 // Legion::SetScrollHereVY(1.0f, "ChatBox");
   // Legion::SetScrollHereY(1.0f);
  //  EndChild();

    return true;
}

bool Legion::SelectableExV(const char* cid, ImFont* font, float fsize, const char* label, bool selected, LegionSelectableFlags flags, const ImVec2& size_arg)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns) // FIXME-OPT: Avoid if vertically clipped.
        PushColumnsBackground();

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    LegionID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    const float min_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Min.x : pos.x;
    const float max_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Max.x : GetContentRegionMaxAbs().x;
    if (size_arg.x == 0.0f || (flags & LegionSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb_enlarged(min_x, pos.y, text_max.x, text_max.y);
    const float spacing_x = style.ItemSpacing.x;
    const float spacing_y = style.ItemSpacing.y;
    const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
    const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
    bb_enlarged.Min.x -= spacing_L;
    bb_enlarged.Min.y -= spacing_U;
    bb_enlarged.Max.x += (spacing_x - spacing_L);
    bb_enlarged.Max.y += (spacing_y - spacing_U);
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_align.Min, bb_align.Max, IM_COL32(255, 0, 0, 255)); }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_enlarged.Min, bb_enlarged.Max, IM_COL32(0, 255, 0, 255)); }

    bool item_add;
    if (flags & LegionSelectableFlags_Disabled)
    {
        LegionItemFlags backup_item_flags = window->DC.ItemFlags;
        window->DC.ItemFlags |= LegionItemFlags_Disabled | LegionItemFlags_NoNavDefaultFocus;
        item_add = ItemAdd(bb_enlarged, id);
        window->DC.ItemFlags = backup_item_flags;
    }
    else
    {
        item_add = ItemAdd(bb_enlarged, id);
    }
    if (!item_add)
    {
        if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
            PopColumnsBackground();
        return false;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    LegionButtonFlags button_flags = 0;
    if (flags & LegionSelectableFlags_NoHoldingActiveID) { button_flags |= LegionButtonFlags_NoHoldingActiveId; }
    if (flags & LegionSelectableFlags_SelectOnClick) { button_flags |= LegionButtonFlags_PressedOnClick; }
    if (flags & LegionSelectableFlags_SelectOnRelease) { button_flags |= LegionButtonFlags_PressedOnRelease; }
    if (flags & LegionSelectableFlags_Disabled) { button_flags |= LegionButtonFlags_Disabled; }
    if (flags & LegionSelectableFlags_AllowDoubleClick) { button_flags |= LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnDoubleClick; }
    if (flags & LegionSelectableFlags_AllowItemOverlap) { button_flags |= LegionButtonFlags_AllowItemOverlap; }

    if (flags & LegionSelectableFlags_Disabled)
        selected = false;

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_enlarged, id, &hovered, &held, button_flags);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & LegionSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        window->DC.LastItemStatusFlags |= LegionItemStatusFlags_ToggledSelection;

    // Render
    if (held && (flags & LegionSelectableFlags_DrawHoveredWhenHeld))
        hovered = true;
    if (hovered || selected)
    {
        const ImU32 col = GetColorU32((held && hovered) ? LegionCol_HeaderActive : hovered ? LegionCol_HeaderHovered : LegionCol_Header);
      //  RenderFrame(bb_enlarged.Min, bb_enlarged.Max, col, false, 0.0f);
      //  RenderNavHighlight(bb_enlarged, id, LegionNavHighlightFlags_TypeThin | LegionNavHighlightFlags_NoRounding);
    }

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
        PopColumnsBackground();

    if (flags & LegionSelectableFlags_Disabled) PushStyleColor(LegionCol_Text, style.Colors[LegionCol_TextDisabled]);
        TextLabelColorV(ImVec2(text_min.x + 5, text_max.y - 10), font, fsize, label/*, NULL, &label_size, style.SelectableTextAlign, &bb_enlarged*/);
    if (flags & LegionSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & LegionWindowFlags_Popup) && !(flags & LegionSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & LegionItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return pressed;
}

bool Legion::SelectableEx(const char* cid, ImFont* font, float fsize, const char* label, bool selected, LegionSelectableFlags flags, const ImVec2& size_arg)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns) // FIXME-OPT: Avoid if vertically clipped.
        PushColumnsBackground();

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    LegionID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    const float min_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Min.x : pos.x;
    const float max_x = (flags & LegionSelectableFlags_SpanAllColumns) ? window->ContentRegionRect.Max.x : GetContentRegionMaxAbs().x;
    if (size_arg.x == 0.0f || (flags & LegionSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb_enlarged(min_x, pos.y, text_max.x, text_max.y);
    const float spacing_x = style.ItemSpacing.x;
    const float spacing_y = style.ItemSpacing.y;
    const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
    const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
    bb_enlarged.Min.x -= spacing_L;
    bb_enlarged.Min.y -= spacing_U;
    bb_enlarged.Max.x += (spacing_x - spacing_L);
    bb_enlarged.Max.y += (spacing_y - spacing_U);
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_align.Min, bb_align.Max, IM_COL32(255, 0, 0, 255)); }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb_enlarged.Min, bb_enlarged.Max, IM_COL32(0, 255, 0, 255)); }

    bool item_add;
    if (flags & LegionSelectableFlags_Disabled)
    {
        LegionItemFlags backup_item_flags = window->DC.ItemFlags;
        window->DC.ItemFlags |= LegionItemFlags_Disabled | LegionItemFlags_NoNavDefaultFocus;
        item_add = ItemAdd(bb_enlarged, id);
        window->DC.ItemFlags = backup_item_flags;
    }
    else
    {
        item_add = ItemAdd(bb_enlarged, id);
    }
    if (!item_add)
    {
        if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
            PopColumnsBackground();
        return false;
    }

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    LegionButtonFlags button_flags = 0;
    if (flags & LegionSelectableFlags_NoHoldingActiveID) { button_flags |= LegionButtonFlags_NoHoldingActiveId; }
    if (flags & LegionSelectableFlags_SelectOnClick) { button_flags |= LegionButtonFlags_PressedOnClick; }
    if (flags & LegionSelectableFlags_SelectOnRelease) { button_flags |= LegionButtonFlags_PressedOnRelease; }
    if (flags & LegionSelectableFlags_Disabled) { button_flags |= LegionButtonFlags_Disabled; }
    if (flags & LegionSelectableFlags_AllowDoubleClick) { button_flags |= LegionButtonFlags_PressedOnClickRelease | LegionButtonFlags_PressedOnDoubleClick; }
    if (flags & LegionSelectableFlags_AllowItemOverlap) { button_flags |= LegionButtonFlags_AllowItemOverlap; }

    if (flags & LegionSelectableFlags_Disabled)
        selected = false;

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb_enlarged, id, &hovered, &held, button_flags);

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & LegionSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            g.NavDisableHighlight = true;
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent);
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & LegionSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        window->DC.LastItemStatusFlags |= LegionItemStatusFlags_ToggledSelection;

    // Render
    if (held && (flags & LegionSelectableFlags_DrawHoveredWhenHeld))
        hovered = true;
    if (hovered || selected)
    {
      //  const ImU32 col = GetColorU32((held && hovered) ? LegionCol_HeaderActive : hovered ? LegionCol_HeaderHovered : LegionCol_Header);
        //  RenderFrame(bb_enlarged.Min, bb_enlarged.Max, col, false, 0.0f);
        //  RenderNavHighlight(bb_enlarged, id, LegionNavHighlightFlags_TypeThin | LegionNavHighlightFlags_NoRounding);
    }

    if ((flags & LegionSelectableFlags_SpanAllColumns) && window->DC.CurrentColumns)
        PopColumnsBackground();

    if (flags & LegionSelectableFlags_Disabled) PushStyleColor(LegionCol_Text, style.Colors[LegionCol_TextDisabled]);
      RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.ButtonTextAlign, &bb_enlarged);
    if (flags & LegionSelectableFlags_Disabled) PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & LegionWindowFlags_Popup) && !(flags & LegionSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & LegionItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return pressed;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: ListBox
//-------------------------------------------------------------------------
// - ListBox()
// - ListBoxHeader()
// - ListBoxFooter()
//-------------------------------------------------------------------------
// FIXME: This is an old API. We should redesign some of it, rename ListBoxHeader->BeginListBox, ListBoxFooter->EndListBox
// and promote using them over existing ListBox() functions, similarly to change with combo boxes.
//-------------------------------------------------------------------------

// FIXME: In principle this function should be called BeginListBox(). We should rename it after re-evaluating if we want to keep the same signature.
// Helper to calculate the size of a listbox and display a label on the right.
// Tip: To have a list filling the entire window width, PushItemWidth(-1) and pass an non-visible label e.g. "##empty"
bool Legion::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const LegionStyle& style = g.Style;
    const LegionID id = GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    // Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
    ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
    ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
    window->DC.LastItemRect = bb; // Forward storage for ListBoxFooter.. dodgy.
    g.NextItemData.ClearFlags();

    if (!IsRectVisible(bb.Min, bb.Max))
    {
        ItemSize(bb.GetSize(), style.FramePadding.y);
        ItemAdd(bb, 0, &frame_bb);
        return false;
    }

    BeginGroup();
    if (label_size.x > 0)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

    BeginChildFrame(id, frame_bb.GetSize());
    return true;
}

// FIXME: In principle this function should be called EndListBox(). We should rename it after re-evaluating if we want to keep the same signature.
bool Legion::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
    // Size default to hold ~7.25 items.
    // We add +25% worth of item height to allow the user to see at a glance if there are more items up/down, without looking at the scrollbar.
    // We don't add this extra bit if items_count <= height_in_items. It is slightly dodgy, because it means a dynamic list of items will make the widget resize occasionally when it crosses that size.
    // I am expecting that someone will come and complain about this behavior in a remote future, then we can advise on a better solution.
    if (height_in_items < 0)
        height_in_items = ImMin(items_count, 7);
    const LegionStyle& style = GetStyle();
    float height_in_items_f = (height_in_items < items_count) ? (height_in_items + 0.25f) : (height_in_items + 0.00f);

    // We include ItemSpacing.y so that a list sized for the exact number of items doesn't make a scrollbar appears. We could also enforce that by passing a flag to BeginChild().
    ImVec2 size;
    size.x = 0.0f;
    size.y = ImFloor(GetTextLineHeightWithSpacing() * height_in_items_f + style.FramePadding.y * 2.0f);
    return ListBoxHeader(label, size);
}

// FIXME: In principle this function should be called EndListBox(). We should rename it after re-evaluating if we want to keep the same signature.
void Legion::ListBoxFooter()
{
    LegionWindow* parent_window = GetCurrentWindow()->ParentWindow;
    const ImRect bb = parent_window->DC.LastItemRect;
    const LegionStyle& style = GetStyle();

    EndChildFrame();

    // Redeclare item size so that it includes the label (we have stored the full size in LastItemRect)
    // We call SameLine() to restore DC.CurrentLine* data
    SameLine();
    parent_window->DC.CursorPos = bb.Min;
    ItemSize(bb, style.FramePadding.y);
    EndGroup();
}

bool Legion::ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_items)
{
    const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
    return value_changed;
}

bool Legion::ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
    if (!ListBoxHeader(label, items_count, height_in_items))
        return false;

    // Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
    LegionContext& g = *GLegion;
    bool value_changed = false;
    LegionListClipper clipper(items_count, GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
    while (clipper.Step())
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        {
            const bool item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";

            PushID(i);
            if (Selectable(item_text, item_selected))
            {
                *current_item = i;
                value_changed = true;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }
    ListBoxFooter();
    if (value_changed)
        MarkItemEdited(g.CurrentWindow->DC.LastItemId);

    return value_changed;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: PlotLines, PlotHistogram
//-------------------------------------------------------------------------
// - PlotEx() [Internal]
// - PlotLines()
// - PlotHistogram()
//-------------------------------------------------------------------------

void Legion::PlotEx(LegionPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 frame_size)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (frame_size.x == 0.0f)
        frame_size.x = CalcItemWidth();
    if (frame_size.y == 0.0f)
        frame_size.y = label_size.y + (style.FramePadding.y * 2);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, 0, &frame_bb))
        return;
    const bool hovered = ItemHoverable(frame_bb, id);

    // Determine scale from values if not specified
    if (scale_min == FLT_MAX || scale_max == FLT_MAX)
    {
        float v_min = FLT_MAX;
        float v_max = -FLT_MAX;
        for (int i = 0; i < values_count; i++)
        {
            const float v = values_getter(data, i);
            if (v != v) // Ignore NaN values
                continue;
            v_min = ImMin(v_min, v);
            v_max = ImMax(v_max, v);
        }
        if (scale_min == FLT_MAX)
            scale_min = v_min;
        if (scale_max == FLT_MAX)
            scale_max = v_max;
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(LegionCol_FrameBg), true, style.FrameRounding);

    const int values_count_min = (plot_type == LegionPlotType_Lines) ? 2 : 1;
    if (values_count >= values_count_min)
    {
        int res_w = ImMin((int)frame_size.x, values_count) + ((plot_type == LegionPlotType_Lines) ? -1 : 0);
        int item_count = values_count + ((plot_type == LegionPlotType_Lines) ? -1 : 0);

        // Tooltip on hover
        int v_hovered = -1;
        if (hovered && inner_bb.Contains(g.IO.MousePos))
        {
            const float t = ImClamp((g.IO.MousePos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x), 0.0f, 0.9999f);
            const int v_idx = (int)(t * item_count);
            IM_ASSERT(v_idx >= 0 && v_idx < values_count);

            const float v0 = values_getter(data, (v_idx + values_offset) % values_count);
            const float v1 = values_getter(data, (v_idx + 1 + values_offset) % values_count);
            if (plot_type == LegionPlotType_Lines)
                SetTooltip("%d: %8.4g\n%d: %8.4g", v_idx, v0, v_idx+1, v1);
            else if (plot_type == LegionPlotType_Histogram)
                SetTooltip("%d: %8.4g", v_idx, v0);
            v_hovered = v_idx;
        }

        const float t_step = 1.0f / (float)res_w;
        const float inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        float v0 = values_getter(data, (0 + values_offset) % values_count);
        float t0 = 0.0f;
        ImVec2 tp0 = ImVec2( t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale) );                       // Point in the normalized space of our target rectangle
        float histogram_zero_line_t = (scale_min * scale_max < 0.0f) ? (-scale_min * inv_scale) : (scale_min < 0.0f ? 0.0f : 1.0f);   // Where does the zero line stands

        const ImU32 col_base = GetColorU32((plot_type == LegionPlotType_Lines) ? LegionCol_PlotLines : LegionCol_PlotHistogram);
        const ImU32 col_hovered = GetColorU32((plot_type == LegionPlotType_Lines) ? LegionCol_PlotLinesHovered : LegionCol_PlotHistogramHovered);

        for (int n = 0; n < res_w; n++)
        {
            const float t1 = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const float v1 = values_getter(data, (v1_idx + values_offset + 1) % values_count);
            const ImVec2 tp1 = ImVec2( t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale) );

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, (plot_type == LegionPlotType_Lines) ? tp1 : ImVec2(tp1.x, histogram_zero_line_t));
            if (plot_type == LegionPlotType_Lines)
            {
                window->DrawList->AddLine(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }
            else if (plot_type == LegionPlotType_Histogram)
            {
                if (pos1.x >= pos0.x + 2.0f)
                    pos1.x -= 1.0f;
                window->DrawList->AddRectFilled(pos0, pos1, v_hovered == v1_idx ? col_hovered : col_base);
            }

            t0 = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    if (overlay_text)
        RenderTextClipped(ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y), frame_bb.Max, overlay_text, NULL, NULL, ImVec2(0.5f,0.0f));

    if (label_size.x > 0.0f)
        RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);
}

struct LegionPlotArrayGetterData
{
    const float* Values;
    int Stride;

    LegionPlotArrayGetterData(const float* values, int stride) { Values = values; Stride = stride; }
};

static float Plot_ArrayGetter(void* data, int idx)
{
    LegionPlotArrayGetterData* plot_data = (LegionPlotArrayGetterData*)data;
    const float v = *(const float*)(const void*)((const unsigned char*)plot_data->Values + (size_t)idx * plot_data->Stride);
    return v;
}

void Legion::PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    LegionPlotArrayGetterData data(values, stride);
    PlotEx(LegionPlotType_Lines, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void Legion::PlotLines(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(LegionPlotType_Lines, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void Legion::PlotHistogram(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
{
    LegionPlotArrayGetterData data(values, stride);
    PlotEx(LegionPlotType_Histogram, label, &Plot_ArrayGetter, (void*)&data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

void Legion::PlotHistogram(const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size)
{
    PlotEx(LegionPlotType_Histogram, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size);
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: Value helpers
// Those is not very useful, legacy API.
//-------------------------------------------------------------------------
// - Value()
//-------------------------------------------------------------------------

void Legion::Value(const char* prefix, bool b)
{
    Text("%s: %s", prefix, (b ? "true" : "false"));
}

void Legion::Value(const char* prefix, int v)
{
    Text("%s: %d", prefix, v);
}

void Legion::Value(const char* prefix, unsigned int v)
{
    Text("%s: %d", prefix, v);
}

void Legion::Value(const char* prefix, float v, const char* float_format)
{
    if (float_format)
    {
        char fmt[64];
        ImFormatString(fmt, IM_ARRAYSIZE(fmt), "%%s: %s", float_format);
        Text(fmt, prefix, v);
    }
    else
    {
        Text("%s: %.3f", prefix, v);
    }
}

//-------------------------------------------------------------------------
// [SECTION] MenuItem, BeginMenu, EndMenu, etc.
//-------------------------------------------------------------------------
// - LegionMenuColumns [Internal]
// - BeginMenuBar()
// - EndMenuBar()
// - BeginMainMenuBar()
// - EndMainMenuBar()
// - BeginMenu()
// - EndMenu()
// - MenuItem()
//-------------------------------------------------------------------------

// Helpers for internal use
LegionMenuColumns::LegionMenuColumns()
{
    Spacing = Width = NextWidth = 0.0f;
    memset(Pos, 0, sizeof(Pos));
    memset(NextWidths, 0, sizeof(NextWidths));
}

void LegionMenuColumns::Update(int count, float spacing, bool clear)
{
    IM_ASSERT(count == IM_ARRAYSIZE(Pos));
    IM_UNUSED(count);
    Width = NextWidth = 0.0f;
    Spacing = spacing;
    if (clear)
        memset(NextWidths, 0, sizeof(NextWidths));
    for (int i = 0; i < IM_ARRAYSIZE(Pos); i++)
    {
        if (i > 0 && NextWidths[i] > 0.0f)
            Width += Spacing;
        Pos[i] = IM_FLOOR(Width);
        Width += NextWidths[i];
        NextWidths[i] = 0.0f;
    }
}

float LegionMenuColumns::DeclColumns(float w0, float w1, float w2) // not using va_arg because they promote float to double
{
    NextWidth = 0.0f;
    NextWidths[0] = ImMax(NextWidths[0], w0);
    NextWidths[1] = ImMax(NextWidths[1], w1);
    NextWidths[2] = ImMax(NextWidths[2], w2);
    for (int i = 0; i < IM_ARRAYSIZE(Pos); i++)
        NextWidth += NextWidths[i] + ((i > 0 && NextWidths[i] > 0.0f) ? Spacing : 0.0f);
    return ImMax(Width, NextWidth);
}

float LegionMenuColumns::CalcExtraSpace(float avail_w) const
{
    return ImMax(0.0f, avail_w - Width);
}

// FIXME: Provided a rectangle perhaps e.g. a BeginMenuBarEx() could be used anywhere..
// Currently the main responsibility of this function being to setup clip-rect + horizontal layout + menu navigation layer.
// Ideally we also want this to be responsible for claiming space out of the main window scrolling rectangle, in which case LegionWindowFlags_MenuBar will become unnecessary.
// Then later the same system could be used for multiple menu-bars, scrollbars, side-bars.
bool Legion::BeginMenuBar()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    if (!(window->Flags & LegionWindowFlags_MenuBar))
        return false;

    IM_ASSERT(!window->DC.MenuBarAppending);
    BeginGroup(); // Backup position on layer 0 // FIXME: Misleading to use a group for that backup/restore
    PushID("##menubar");

    // We don't clip with current window clipping rectangle as it is already set to the area below. However we clip with window full rect.
    // We remove 1 worth of rounding to Max.x to that text in long menus and small windows don't tend to display over the lower-right rounded area, which looks particularly glitchy.
    ImRect bar_rect = window->MenuBarRect();
    ImRect clip_rect(IM_ROUND(bar_rect.Min.x + window->WindowBorderSize), IM_ROUND(bar_rect.Min.y + window->WindowBorderSize), IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))), IM_ROUND(bar_rect.Max.y));
    clip_rect.ClipWith(window->OuterRectClipped);
    PushClipRect(clip_rect.Min, clip_rect.Max, false);

    window->DC.CursorPos = ImVec2(bar_rect.Min.x + window->DC.MenuBarOffset.x, bar_rect.Min.y + window->DC.MenuBarOffset.y);
    window->DC.LayoutType = LegionLayoutType_Horizontal;
    window->DC.NavLayerCurrent = LegionNavLayer_Menu;
    window->DC.NavLayerCurrentMask = (1 << LegionNavLayer_Menu);
    window->DC.MenuBarAppending = true;
    AlignTextToFramePadding();
    return true;
}

void Legion::EndMenuBar()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    LegionContext& g = *GLegion;

    // Nav: When a move request within one of our child menu failed, capture the request to navigate among our siblings.
    if (NavMoveRequestButNoResultYet() && (g.NavMoveDir == LegionDir_Left || g.NavMoveDir == LegionDir_Right) && (g.NavWindow->Flags & LegionWindowFlags_ChildMenu))
    {
        LegionWindow* nav_earliest_child = g.NavWindow;
        while (nav_earliest_child->ParentWindow && (nav_earliest_child->ParentWindow->Flags & LegionWindowFlags_ChildMenu))
            nav_earliest_child = nav_earliest_child->ParentWindow;
        if (nav_earliest_child->ParentWindow == window && nav_earliest_child->DC.ParentLayoutType == LegionLayoutType_Horizontal && g.NavMoveRequestForward == LegionNavForward_None)
        {
            // To do so we claim focus back, restore NavId and then process the movement request for yet another frame.
            // This involve a one-frame delay which isn't very problematic in this situation. We could remove it by scoring in advance for multiple window (probably not worth the hassle/cost)
            const LegionNavLayer layer = LegionNavLayer_Menu;
            IM_ASSERT(window->DC.NavLayerActiveMaskNext & (1 << layer)); // Sanity check
            FocusWindow(window);
            SetNavIDWithRectRel(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);
            g.NavLayer = layer;
            g.NavDisableHighlight = true; // Hide highlight for the current frame so we don't see the intermediary selection.
            g.NavMoveRequestForward = LegionNavForward_ForwardQueued;
            NavMoveRequestCancel();
        }
    }

    IM_ASSERT(window->Flags & LegionWindowFlags_MenuBar);
    IM_ASSERT(window->DC.MenuBarAppending);
    PopClipRect();
    PopID();
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->MenuBarRect().Min.x; // Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
    window->DC.GroupStack.back().EmitItem = false;
    EndGroup(); // Restore position on layer 0
    window->DC.LayoutType = LegionLayoutType_Vertical;
    window->DC.NavLayerCurrent = LegionNavLayer_Main;
    window->DC.NavLayerCurrentMask = (1 << LegionNavLayer_Main);
    window->DC.MenuBarAppending = false;
}

// For the main menu bar, which cannot be moved, we honor g.Style.DisplaySafeAreaPadding to ensure text can be visible on a TV set.
bool Legion::BeginMainMenuBar()
{
    LegionContext& g = *GLegion;
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
    SetNextWindowPos(ImVec2(0.0f, 0.0f));
    SetNextWindowSize(ImVec2(g.IO.DisplaySize.x, g.NextWindowData.MenuBarOffsetMinVal.y + g.FontBaseSize + g.Style.FramePadding.y));
    PushStyleVar(LegionStyleVar_WindowRounding, 0.0f);
    PushStyleVar(LegionStyleVar_WindowMinSize, ImVec2(0, 0));
    LegionWindowFlags window_flags = LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoMove | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_MenuBar;
    bool is_open = Begin("##MainMenuBar", NULL, window_flags) && BeginMenuBar();
    PopStyleVar(2);
    g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
    if (!is_open)
    {
        End();
        return false;
    }
    return true; //-V1020
}

void Legion::EndMainMenuBar()
{
    EndMenuBar();

    // When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
    // FIXME: With this strategy we won't be able to restore a NULL focus.
    LegionContext& g = *GLegion;
    if (g.CurrentWindow == g.NavWindow && g.NavLayer == LegionNavLayer_Main && !g.NavAnyRequest)
        FocusTopMostWindowUnderOne(g.NavWindow, NULL);

    End();
}


bool Legion::BeginMenuEx(ImFont* font, float font_size, ImTextureID texture, ImTextureID scrollbg, ImTextureID scrollbar, const char* label, bool enabled)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    bool menu_is_open = IsPopupOpen(id);

    // Sub-menus are ChildWindow so that mouse can be hovering across them (otherwise top-most popup menu would steal focus and not allow hovering on parent menu)
    LegionWindowFlags flags = LegionWindowFlags_ChildMenu | LegionWindowFlags_AlwaysAutoResize | LegionWindowFlags_NoMove | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoNavFocus;
    if (window->Flags & (LegionWindowFlags_Popup | LegionWindowFlags_ChildMenu))
        flags |= LegionWindowFlags_ChildWindow;

    // If a menu with same the ID was already submitted, we will append to it, matching the behavior of Begin().
    // We are relying on a O(N) search - so O(N log N) over the frame - which seems like the most efficient for the expected small amount of BeginMenu() calls per frame.
    // If somehow this is ever becoming a problem we can switch to use e.g. a LegionStorager mapping key to last frame used.
    if (g.MenusIdSubmittedThisFrame.contains(id))
    {
        if (menu_is_open)
            menu_is_open = BeginPopupExV(texture, scrollbg, scrollbar, id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
        else
            g.NextWindowData.ClearFlags();          // we behave like Begin() and need to consume those values
        return menu_is_open;
    }

    // Tag menu as used. Next time BeginMenu() with same ID is called it will append to existing menu
    g.MenusIdSubmittedThisFrame.push_back(id);

    ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);
    bool pressed;
    bool menuset_is_open = !(window->Flags & LegionWindowFlags_Popup) && (g.OpenPopupStack.Size > g.BeginPopupStack.Size&& g.OpenPopupStack[g.BeginPopupStack.Size].OpenParentId == window->IDStack.back());
    LegionWindow* backed_nav_window = g.NavWindow;
    if (menuset_is_open)
        g.NavWindow = window;  // Odd hack to allow hovering across menus of a same menu-set (otherwise we wouldn't be able to hover parent)

    // The reference position stored in popup_pos will be used by Begin() to find a suitable position for the child menu,
    // However the final position is going to be different! It is choosen by FindBestWindowPosForPopup().
    // e.g. Menus tend to overlap each other horizontally to amplify relative Z-ordering.
    ImVec2 popup_pos, pos = window->DC.CursorPos;
    if (window->DC.LayoutType == LegionLayoutType_Horizontal)
    {
        // Menu inside an horizontal menu bar
        // Selectable extend their highlight by half ItemSpacing in each direction.
        // For ChildMenu, the popup position will be overwritten by the call to FindBestWindowPosForPopup() in Begin()
        popup_pos = ImVec2(pos.x - 1.0f - IM_FLOOR(style.ItemSpacing.x * 0.5f), pos.y - style.FramePadding.y + window->MenuBarHeight());
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * 0.5f);
        PushStyleVar(LegionStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y));
        float w = label_size.x;
        pressed = Selectable(label, menu_is_open, LegionSelectableFlags_NoHoldingActiveID | LegionSelectableFlags_SelectOnClick | LegionSelectableFlags_DontClosePopups | (!enabled ? LegionSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
        PopStyleVar();
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    else
    {
        // Menu inside a menu
        // (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
        //  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
        popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
        float min_w = window->DC.MenuColumns.DeclColumns(label_size.x, 0.0f, IM_FLOOR(g.FontSize * 1.20f)); // Feedback to next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - min_w);
        pressed = Selectable(label, menu_is_open, LegionSelectableFlags_NoHoldingActiveID | LegionSelectableFlags_SelectOnClick | LegionSelectableFlags_DontClosePopups | LegionSelectableFlags_SpanAvailWidth | (!enabled ? LegionSelectableFlags_Disabled : 0), ImVec2(min_w, 0.0f));
        ImU32 text_col = GetColorU32(enabled ? LegionCol_Text : LegionCol_TextDisabled);
        RenderArrow(window->DrawList, pos + ImVec2(window->DC.MenuColumns.Pos[2] + extra_w + g.FontSize * 0.30f, 0.0f), text_col, LegionDir_Right);
    }

    const bool hovered = enabled && ItemHoverable(window->DC.LastItemRect, id);
    if (menuset_is_open)
        g.NavWindow = backed_nav_window;

    bool want_open = false;
    bool want_close = false;
    if (window->DC.LayoutType == LegionLayoutType_Vertical) // (window->Flags & (LegionWindowFlags_Popup|LegionWindowFlags_ChildMenu))
    {
        // Close menu when not hovering it anymore unless we are moving roughly in the direction of the menu
        // Implement http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to avoid using timers, so menus feels more reactive.
        bool moving_toward_other_child_menu = false;

        LegionWindow* child_menu_window = (g.BeginPopupStack.Size < g.OpenPopupStack.Size && g.OpenPopupStack[g.BeginPopupStack.Size].SourceWindow == window) ? g.OpenPopupStack[g.BeginPopupStack.Size].Window : NULL;
        if (g.HoveredWindow == window && child_menu_window != NULL && !(window->Flags & LegionWindowFlags_MenuBar))
        {
            // FIXME-DPI: Values should be derived from a master "scale" factor.
            ImRect next_window_rect = child_menu_window->Rect();
            ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
            ImVec2 tb = (window->Pos.x < child_menu_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
            ImVec2 tc = (window->Pos.x < child_menu_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
            float extra = ImClamp(ImFabs(ta.x - tb.x) * 0.30f, 5.0f, 30.0f);    // add a bit of extra slack.
            ta.x += (window->Pos.x < child_menu_window->Pos.x) ? -0.5f : +0.5f; // to avoid numerical issues
            tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);                // triangle is maximum 200 high to limit the slope and the bias toward large sub-menus // FIXME: Multiply by fb_scale?
            tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
            moving_toward_other_child_menu = ImTriangleContainsPoint(ta, tb, tc, g.IO.MousePos);
            //GetForegroundDrawList()->AddTriangleFilled(ta, tb, tc, moving_within_opened_triangle ? IM_COL32(0,128,0,128) : IM_COL32(128,0,0,128)); // [DEBUG]
        }
        if (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id && !moving_toward_other_child_menu)
            want_close = true;

        if (!menu_is_open && hovered && pressed) // Click to open
            want_open = true;
        else if (!menu_is_open && hovered && !moving_toward_other_child_menu) // Hover to open
            want_open = true;

        if (g.NavActivateId == id)
        {
            want_close = menu_is_open;
            want_open = !menu_is_open;
        }
        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Right) // Nav-Right to open
        {
            want_open = true;
            NavMoveRequestCancel();
        }
    }
    else
    {
        // Menu bar
        if (menu_is_open && pressed && menuset_is_open) // Click an open menu again to close it
        {
            want_close = true;
            want_open = menu_is_open = false;
        }
        else if (pressed || (hovered && menuset_is_open && !menu_is_open)) // First click to open, then hover to open others
        {
            want_open = true;
        }
        else if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Down) // Nav-Down to open
        {
            want_open = true;
            NavMoveRequestCancel();
        }
    }

    if (!enabled) // explicitly close if an open menu becomes disabled, facilitate users code a lot in pattern such as 'if (BeginMenu("options", has_object)) { ..use object.. }'
        want_close = true;
    if (want_close && IsPopupOpen(id))
        ClosePopupToLevel(g.BeginPopupStack.Size, true);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | LegionItemStatusFlags_Openable | (menu_is_open ? LegionItemStatusFlags_Opened : 0));

    if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.BeginPopupStack.Size)
    {
        // Don't recycle same menu level in the same frame, first close the other menu and yield for a frame.
        OpenPopup(label);
        return false;
    }

    menu_is_open |= want_open;
    if (want_open)
        OpenPopup(label);

    if (menu_is_open)
    {
        SetNextWindowPos(popup_pos, LegionCond_Always);
        menu_is_open = BeginPopupExV(texture, scrollbg, scrollbar, id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
    }
    else
    {
        g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    }

    return menu_is_open;
}

bool Legion::BeginMenu(const char* label, bool enabled)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    const LegionStyle& style = g.Style;
    const LegionID id = window->GetID(label);
    bool menu_is_open = IsPopupOpen(id);

    // Sub-menus are ChildWindow so that mouse can be hovering across them (otherwise top-most popup menu would steal focus and not allow hovering on parent menu)
    LegionWindowFlags flags = LegionWindowFlags_ChildMenu | LegionWindowFlags_AlwaysAutoResize | LegionWindowFlags_NoMove | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoNavFocus;
    if (window->Flags & (LegionWindowFlags_Popup | LegionWindowFlags_ChildMenu))
        flags |= LegionWindowFlags_ChildWindow;

    // If a menu with same the ID was already submitted, we will append to it, matching the behavior of Begin().
    // We are relying on a O(N) search - so O(N log N) over the frame - which seems like the most efficient for the expected small amount of BeginMenu() calls per frame.
    // If somehow this is ever becoming a problem we can switch to use e.g. a LegionStorager mapping key to last frame used.
    if (g.MenusIdSubmittedThisFrame.contains(id))
    {
        if (menu_is_open)
            menu_is_open = BeginPopupEx(id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
        else
            g.NextWindowData.ClearFlags();          // we behave like Begin() and need to consume those values
        return menu_is_open;
    }

    // Tag menu as used. Next time BeginMenu() with same ID is called it will append to existing menu
    g.MenusIdSubmittedThisFrame.push_back(id);

    ImVec2 label_size = CalcTextSize(label, NULL, true);
    bool pressed;
    bool menuset_is_open = !(window->Flags & LegionWindowFlags_Popup) && (g.OpenPopupStack.Size > g.BeginPopupStack.Size && g.OpenPopupStack[g.BeginPopupStack.Size].OpenParentId == window->IDStack.back());
    LegionWindow* backed_nav_window = g.NavWindow;
    if (menuset_is_open)
        g.NavWindow = window;  // Odd hack to allow hovering across menus of a same menu-set (otherwise we wouldn't be able to hover parent)

    // The reference position stored in popup_pos will be used by Begin() to find a suitable position for the child menu,
    // However the final position is going to be different! It is choosen by FindBestWindowPosForPopup().
    // e.g. Menus tend to overlap each other horizontally to amplify relative Z-ordering.
    ImVec2 popup_pos, pos = window->DC.CursorPos;
    if (window->DC.LayoutType == LegionLayoutType_Horizontal)
    {
        // Menu inside an horizontal menu bar
        // Selectable extend their highlight by half ItemSpacing in each direction.
        // For ChildMenu, the popup position will be overwritten by the call to FindBestWindowPosForPopup() in Begin()
        popup_pos = ImVec2(pos.x - 1.0f - IM_FLOOR(style.ItemSpacing.x * 0.5f), pos.y - style.FramePadding.y + window->MenuBarHeight());
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * 0.5f);
        PushStyleVar(LegionStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y));
        float w = label_size.x;
        pressed = Selectable(label, menu_is_open, LegionSelectableFlags_NoHoldingActiveID | LegionSelectableFlags_SelectOnClick | LegionSelectableFlags_DontClosePopups | (!enabled ? LegionSelectableFlags_Disabled : 0), ImVec2(w, 0.0f));
        PopStyleVar();
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    else
    {
        // Menu inside a menu
        // (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
        //  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
        popup_pos = ImVec2(pos.x, pos.y - style.WindowPadding.y);
        float min_w = window->DC.MenuColumns.DeclColumns(label_size.x, 0.0f, IM_FLOOR(g.FontSize * 1.20f)); // Feedback to next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - min_w);
        pressed = Selectable(label, menu_is_open, LegionSelectableFlags_NoHoldingActiveID | LegionSelectableFlags_SelectOnClick | LegionSelectableFlags_DontClosePopups | LegionSelectableFlags_SpanAvailWidth | (!enabled ? LegionSelectableFlags_Disabled : 0), ImVec2(min_w, 0.0f));
        ImU32 text_col = GetColorU32(enabled ? LegionCol_Text : LegionCol_TextDisabled);
        RenderArrow(window->DrawList, pos + ImVec2(window->DC.MenuColumns.Pos[2] + extra_w + g.FontSize * 0.30f, 0.0f), text_col, LegionDir_Right);
    }

    const bool hovered = enabled && ItemHoverable(window->DC.LastItemRect, id);
    if (menuset_is_open)
        g.NavWindow = backed_nav_window;

    bool want_open = false;
    bool want_close = false;
    if (window->DC.LayoutType == LegionLayoutType_Vertical) // (window->Flags & (LegionWindowFlags_Popup|LegionWindowFlags_ChildMenu))
    {
        // Close menu when not hovering it anymore unless we are moving roughly in the direction of the menu
        // Implement http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown to avoid using timers, so menus feels more reactive.
        bool moving_toward_other_child_menu = false;

        LegionWindow* child_menu_window = (g.BeginPopupStack.Size < g.OpenPopupStack.Size && g.OpenPopupStack[g.BeginPopupStack.Size].SourceWindow == window) ? g.OpenPopupStack[g.BeginPopupStack.Size].Window : NULL;
        if (g.HoveredWindow == window && child_menu_window != NULL && !(window->Flags & LegionWindowFlags_MenuBar))
        {
            // FIXME-DPI: Values should be derived from a master "scale" factor.
            ImRect next_window_rect = child_menu_window->Rect();
            ImVec2 ta = g.IO.MousePos - g.IO.MouseDelta;
            ImVec2 tb = (window->Pos.x < child_menu_window->Pos.x) ? next_window_rect.GetTL() : next_window_rect.GetTR();
            ImVec2 tc = (window->Pos.x < child_menu_window->Pos.x) ? next_window_rect.GetBL() : next_window_rect.GetBR();
            float extra = ImClamp(ImFabs(ta.x - tb.x) * 0.30f, 5.0f, 30.0f);    // add a bit of extra slack.
            ta.x += (window->Pos.x < child_menu_window->Pos.x) ? -0.5f : +0.5f; // to avoid numerical issues
            tb.y = ta.y + ImMax((tb.y - extra) - ta.y, -100.0f);                // triangle is maximum 200 high to limit the slope and the bias toward large sub-menus // FIXME: Multiply by fb_scale?
            tc.y = ta.y + ImMin((tc.y + extra) - ta.y, +100.0f);
            moving_toward_other_child_menu = ImTriangleContainsPoint(ta, tb, tc, g.IO.MousePos);
            //GetForegroundDrawList()->AddTriangleFilled(ta, tb, tc, moving_within_opened_triangle ? IM_COL32(0,128,0,128) : IM_COL32(128,0,0,128)); // [DEBUG]
        }
        if (menu_is_open && !hovered && g.HoveredWindow == window && g.HoveredIdPreviousFrame != 0 && g.HoveredIdPreviousFrame != id && !moving_toward_other_child_menu)
            want_close = true;

        if (!menu_is_open && hovered && pressed) // Click to open
            want_open = true;
        else if (!menu_is_open && hovered && !moving_toward_other_child_menu) // Hover to open
            want_open = true;

        if (g.NavActivateId == id)
        {
            want_close = menu_is_open;
            want_open = !menu_is_open;
        }
        if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Right) // Nav-Right to open
        {
            want_open = true;
            NavMoveRequestCancel();
        }
    }
    else
    {
        // Menu bar
        if (menu_is_open && pressed && menuset_is_open) // Click an open menu again to close it
        {
            want_close = true;
            want_open = menu_is_open = false;
        }
        else if (pressed || (hovered && menuset_is_open && !menu_is_open)) // First click to open, then hover to open others
        {
            want_open = true;
        }
        else if (g.NavId == id && g.NavMoveRequest && g.NavMoveDir == LegionDir_Down) // Nav-Down to open
        {
            want_open = true;
            NavMoveRequestCancel();
        }
    }

    if (!enabled) // explicitly close if an open menu becomes disabled, facilitate users code a lot in pattern such as 'if (BeginMenu("options", has_object)) { ..use object.. }'
        want_close = true;
    if (want_close && IsPopupOpen(id))
        ClosePopupToLevel(g.BeginPopupStack.Size, true);

    Legion_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | LegionItemStatusFlags_Openable | (menu_is_open ? LegionItemStatusFlags_Opened : 0));

    if (!menu_is_open && want_open && g.OpenPopupStack.Size > g.BeginPopupStack.Size)
    {
        // Don't recycle same menu level in the same frame, first close the other menu and yield for a frame.
        OpenPopup(label);
        return false;
    }

    menu_is_open |= want_open;
    if (want_open)
        OpenPopup(label);

    if (menu_is_open)
    {
        SetNextWindowPos(popup_pos, LegionCond_Always);
        menu_is_open = BeginPopupEx(id, flags); // menu_is_open can be 'false' when the popup is completely clipped (e.g. zero size display)
    }
    else
    {
        g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    }

    return menu_is_open;
}

void Legion::EndMenu()
{
    // Nav: When a left move request _within our child menu_ failed, close ourselves (the _parent_ menu).
    // A menu doesn't close itself because EndMenuBar() wants the catch the last Left<>Right inputs.
    // However, it means that with the current code, a BeginMenu() from outside another menu or a menu-bar won't be closable with the Left direction.
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (g.NavWindow && g.NavWindow->ParentWindow == window && g.NavMoveDir == LegionDir_Left && NavMoveRequestButNoResultYet() && window->DC.LayoutType == LegionLayoutType_Vertical)
    {
        ClosePopupToLevel(g.BeginPopupStack.Size, true);
        NavMoveRequestCancel();
    }

    EndPopup();
}

bool Legion::MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    LegionStyle& style = g.Style;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 label_size = CalcTextSize(label, NULL, true);

    // We've been using the equivalent of LegionSelectableFlags_SetNavIdOnHover on all Selectable() since early Nav system days (commit 43ee5d73),
    // but I am unsure whether this should be kept at all. For now moved it to be an opt-in feature used by menus only.
    LegionSelectableFlags flags = LegionSelectableFlags_SelectOnRelease | LegionSelectableFlags_SetNavIdOnHover | (enabled ? 0 : LegionSelectableFlags_Disabled);
    bool pressed;
    if (window->DC.LayoutType == LegionLayoutType_Horizontal)
    {
        // Mimic the exact layout spacing of BeginMenu() to allow MenuItem() inside a menu bar, which is a little misleading but may be useful
        // Note that in this situation we render neither the shortcut neither the selected tick mark
        float w = label_size.x;
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * 0.5f);
        PushStyleVar(LegionStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y));
        pressed = Selectable(label, false, flags, ImVec2(w, 0.0f));
        PopStyleVar();
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    else
    {
        // Menu item inside a vertical menu
        // (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
        //  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
        float shortcut_w = shortcut ? CalcTextSize(shortcut, NULL).x : 0.0f;
        float min_w = window->DC.MenuColumns.DeclColumns(label_size.x, shortcut_w, IM_FLOOR(g.FontSize * 1.20f)); // Feedback for next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - min_w);
        pressed = Selectable(label, false, flags | LegionSelectableFlags_SpanAvailWidth, ImVec2(min_w, 0.0f));
        if (shortcut_w > 0.0f)
        {
            PushStyleColor(LegionCol_Text, g.Style.Colors[LegionCol_TextDisabled]);
            RenderText(pos + ImVec2(window->DC.MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
            PopStyleColor();
        }
        if (selected)
            RenderCheckMark(window->DrawList, pos + ImVec2(window->DC.MenuColumns.Pos[2] + extra_w + g.FontSize * 0.40f, g.FontSize * 0.134f * 0.5f), GetColorU32(enabled ? LegionCol_Text : LegionCol_TextDisabled), g.FontSize  * 0.866f);
    }

    Legion_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | LegionItemStatusFlags_Checkable | (selected ? LegionItemStatusFlags_Checked : 0));
    return pressed;
}

bool Legion::MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
    if (MenuItem(label, shortcut, p_selected ? *p_selected : false, enabled))
    {
        if (p_selected)
            *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool Legion::MenuItemEx(ImFont* font, float font_size, ImTextureID texture, const char* label, const char* shortcut, bool selected, bool enabled)
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    LegionContext& g = *GLegion;
    LegionStyle& style = g.Style;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 label_size = CalcTextSizeV(label, font, font_size, NULL, true);

    // We've been using the equivalent of LegionSelectableFlags_SetNavIdOnHover on all Selectable() since early Nav system days (commit 43ee5d73),
    // but I am unsure whether this should be kept at all. For now moved it to be an opt-in feature used by menus only.
    LegionSelectableFlags flags = LegionSelectableFlags_SelectOnRelease | LegionSelectableFlags_SetNavIdOnHover | (enabled ? 0 : LegionSelectableFlags_Disabled);
    bool pressed;
    if (window->DC.LayoutType == LegionLayoutType_Horizontal)
    {
        // Mimic the exact layout spacing of BeginMenu() to allow MenuItem() inside a menu bar, which is a little misleading but may be useful
        // Note that in this situation we render neither the shortcut neither the selected tick mark
        float w = label_size.x;
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * 0.5f);
        PushStyleVar(LegionStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x * 2.0f, style.ItemSpacing.y));

        pressed = SelectableEx(label, font, font_size, label, false, flags, ImVec2(w, 0.0f));
        PopStyleVar();
        window->DC.CursorPos.x += IM_FLOOR(style.ItemSpacing.x * (-1.0f + 0.5f)); // -1 spacing to compensate the spacing added when Selectable() did a SameLine(). It would also work to call SameLine() ourselves after the PopStyleVar().
    }
    else
    {
        // Menu item inside a vertical menu
        // (In a typical menu window where all items are BeginMenu() or MenuItem() calls, extra_w will always be 0.0f.
        //  Only when they are other items sticking out we're going to add spacing, yet only register minimum width into the layout system.
        float shortcut_w = shortcut ? CalcTextSizeV(shortcut, font, font_size, NULL).x : 0.0f;
        float min_w = window->DC.MenuColumns.DeclColumns(label_size.x, shortcut_w, IM_FLOOR(g.FontSize * 1.20f)); // Feedback for next frame
        float extra_w = ImMax(0.0f, GetContentRegionAvail().x - min_w);
        pressed = SelectableEx(label, font, font_size, label, false, flags | LegionSelectableFlags_SpanAvailWidth, ImVec2(min_w, 0.0f));
        if (shortcut_w > 0.0f)
        {
            RenderTextEx(font, font_size, pos + ImVec2(window->DC.MenuColumns.Pos[1] + extra_w, 0.0f), shortcut, NULL, false);
        }
        if (selected)
            RenderCheckMark(window->DrawList, pos + ImVec2(window->DC.MenuColumns.Pos[2] + extra_w + g.FontSize * 0.40f, g.FontSize * 0.134f * 0.5f), GetColorU32(enabled ? LegionCol_Text : LegionCol_TextDisabled), g.FontSize * 0.866f);
    }

    Legion_TEST_ENGINE_ITEM_INFO(window->DC.LastItemId, label, window->DC.ItemFlags | LegionItemStatusFlags_Checkable | (selected ? LegionItemStatusFlags_Checked : 0));
    return pressed;
}

bool Legion::MenuItemEx(ImFont* font, float font_size, ImTextureID texture, const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
    if (MenuItemEx(font, font_size, texture, label, shortcut, p_selected ? *p_selected : false, enabled))
    {
        if (p_selected)
            *p_selected = !*p_selected;
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------
// [SECTION] Widgets: BeginTabBar, EndTabBar, etc.
//-------------------------------------------------------------------------
// - BeginTabBar()
// - BeginTabBarEx() [Internal]
// - EndTabBar()
// - TabBarLayout() [Internal]
// - TabBarCalcTabID() [Internal]
// - TabBarCalcMaxTabWidth() [Internal]
// - TabBarFindTabById() [Internal]
// - TabBarRemoveTab() [Internal]
// - TabBarCloseTab() [Internal]
// - TabBarScrollClamp()v
// - TabBarScrollToTab() [Internal]
// - TabBarQueueChangeTabOrder() [Internal]
// - TabBarScrollingButtons() [Internal]
// - TabBarTabListPopupButton() [Internal]
//-------------------------------------------------------------------------

namespace Legion
{
    static void             TabBarLayout(LegionTabBar* tab_bar);
    static ImU32            TabBarCalcTabID(LegionTabBar* tab_bar, const char* label);
    static float            TabBarCalcMaxTabWidth();
    static float            TabBarScrollClamp(LegionTabBar* tab_bar, float scrolling);
    static void             TabBarScrollToTab(LegionTabBar* tab_bar, LegionTabItem* tab);
    static LegionTabItem*    TabBarScrollingButtons(LegionTabBar* tab_bar);
    static LegionTabItem*    TabBarTabListPopupButton(LegionTabBar* tab_bar);
}

LegionTabBar::LegionTabBar()
{
    ID = 0;
    SelectedTabId = NextSelectedTabId = VisibleTabId = 0;
    CurrFrameVisible = PrevFrameVisible = -1;
    LastTabContentHeight = 0.0f;
    OffsetMax = OffsetMaxIdeal = OffsetNextTab = 0.0f;
    ScrollingAnim = ScrollingTarget = ScrollingTargetDistToVisibility = ScrollingSpeed = 0.0f;
    Flags = LegionTabBarFlags_None;
    ReorderRequestTabId = 0;
    ReorderRequestDir = 0;
    WantLayout = VisibleTabWasSubmitted = false;
    LastTabItemIdx = -1;
}

static int Legion_CDECL TabItemComparerByVisibleOffset(const void* lhs, const void* rhs)
{
    const LegionTabItem* a = (const LegionTabItem*)lhs;
    const LegionTabItem* b = (const LegionTabItem*)rhs;
    return (int)(a->Offset - b->Offset);
}

static LegionTabBar* GetTabBarFromTabBarRef(const LegionPtrOrIndex& ref)
{
    LegionContext& g = *GLegion;
    return ref.Ptr ? (LegionTabBar*)ref.Ptr : g.TabBars.GetByIndex(ref.Index);
}

static LegionPtrOrIndex GetTabBarRefFromTabBar(LegionTabBar* tab_bar)
{
    LegionContext& g = *GLegion;
    if (g.TabBars.Contains(tab_bar))
        return LegionPtrOrIndex(g.TabBars.GetIndex(tab_bar));
    return LegionPtrOrIndex(tab_bar);
}

bool    Legion::BeginTabBar(const char* str_id, LegionTabBarFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    LegionID id = window->GetID(str_id);
    LegionTabBar* tab_bar = g.TabBars.GetOrAddByKey(id);
    ImRect tab_bar_bb = ImRect(window->DC.CursorPos.x, window->DC.CursorPos.y, window->WorkRect.Max.x, window->DC.CursorPos.y + g.FontSize + g.Style.FramePadding.y * 2);
    tab_bar->ID = id;
    return BeginTabBarEx(tab_bar, tab_bar_bb, flags | LegionTabBarFlags_IsFocused);
}

bool    Legion::BeginTabBarEx(LegionTabBar* tab_bar, const ImRect& tab_bar_bb, LegionTabBarFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    if ((flags & LegionTabBarFlags_DockNode) == 0)
        PushOverrideID(tab_bar->ID);

    // Add to stack
    g.CurrentTabBarStack.push_back(GetTabBarRefFromTabBar(tab_bar));
    g.CurrentTabBar = tab_bar;

    if (tab_bar->CurrFrameVisible == g.FrameCount)
    {
        //Legion_DEBUG_LOG("BeginTabBarEx already called this frame\n", g.FrameCount);
        IM_ASSERT(0);
        return true;
    }

    // When toggling back from ordered to manually-reorderable, shuffle tabs to enforce the last visible order.
    // Otherwise, the most recently inserted tabs would move at the end of visible list which can be a little too confusing or magic for the user.
    if ((flags & LegionTabBarFlags_Reorderable) && !(tab_bar->Flags & LegionTabBarFlags_Reorderable) && tab_bar->Tabs.Size > 1 && tab_bar->PrevFrameVisible != -1)
        ImQsort(tab_bar->Tabs.Data, tab_bar->Tabs.Size, sizeof(LegionTabItem), TabItemComparerByVisibleOffset);

    // Flags
    if ((flags & LegionTabBarFlags_FittingPolicyMask_) == 0)
        flags |= LegionTabBarFlags_FittingPolicyDefault_;

    tab_bar->Flags = flags;
    tab_bar->BarRect = tab_bar_bb;
    tab_bar->WantLayout = true; // Layout will be done on the first call to ItemTab()
    tab_bar->PrevFrameVisible = tab_bar->CurrFrameVisible;
    tab_bar->CurrFrameVisible = g.FrameCount;
    tab_bar->FramePadding = g.Style.FramePadding;

    // Layout
    ItemSize(ImVec2(tab_bar->OffsetMaxIdeal, tab_bar->BarRect.GetHeight()), tab_bar->FramePadding.y);
    window->DC.CursorPos.x = tab_bar->BarRect.Min.x;

    // Draw separator
    const ImU32 col = GetColorU32((flags & LegionTabBarFlags_IsFocused) ? LegionCol_TabActive : LegionCol_TabUnfocusedActive);
    const float y = tab_bar->BarRect.Max.y - 1.0f;
    {
        const float separator_min_x = tab_bar->BarRect.Min.x - IM_FLOOR(window->WindowPadding.x * 0.5f);
        const float separator_max_x = tab_bar->BarRect.Max.x + IM_FLOOR(window->WindowPadding.x * 0.5f);
        window->DrawList->AddLine(ImVec2(separator_min_x, y), ImVec2(separator_max_x, y), col, 1.0f);
    }
    return true;
}

void    Legion::EndTabBar()
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    LegionTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        IM_ASSERT_USER_ERROR(tab_bar != NULL, "Mismatched BeginTabBar()/EndTabBar()!");
        return;
    }
    if (tab_bar->WantLayout)
        TabBarLayout(tab_bar);

    // Restore the last visible height if no tab is visible, this reduce vertical flicker/movement when a tabs gets removed without calling SetTabItemClosed().
    const bool tab_bar_appearing = (tab_bar->PrevFrameVisible + 1 < g.FrameCount);
    if (tab_bar->VisibleTabWasSubmitted || tab_bar->VisibleTabId == 0 || tab_bar_appearing)
        tab_bar->LastTabContentHeight = ImMax(window->DC.CursorPos.y - tab_bar->BarRect.Max.y, 0.0f);
    else
        window->DC.CursorPos.y = tab_bar->BarRect.Max.y + tab_bar->LastTabContentHeight;

    if ((tab_bar->Flags & LegionTabBarFlags_DockNode) == 0)
        PopID();

    g.CurrentTabBarStack.pop_back();
    g.CurrentTabBar = g.CurrentTabBarStack.empty() ? NULL : GetTabBarFromTabBarRef(g.CurrentTabBarStack.back());
}

// This is called only once a frame before by the first call to ItemTab()
// The reason we're not calling it in BeginTabBar() is to leave a chance to the user to call the SetTabItemClosed() functions.
static void Legion::TabBarLayout(LegionTabBar* tab_bar)
{
    LegionContext& g = *GLegion;
    tab_bar->WantLayout = false;

    // Garbage collect
    int tab_dst_n = 0;
    for (int tab_src_n = 0; tab_src_n < tab_bar->Tabs.Size; tab_src_n++)
    {
        LegionTabItem* tab = &tab_bar->Tabs[tab_src_n];
        if (tab->LastFrameVisible < tab_bar->PrevFrameVisible)
        {
            if (tab->ID == tab_bar->SelectedTabId)
                tab_bar->SelectedTabId = 0;
            continue;
        }
        if (tab_dst_n != tab_src_n)
            tab_bar->Tabs[tab_dst_n] = tab_bar->Tabs[tab_src_n];
        tab_dst_n++;
    }
    if (tab_bar->Tabs.Size != tab_dst_n)
        tab_bar->Tabs.resize(tab_dst_n);

    // Setup next selected tab
    LegionID scroll_track_selected_tab_id = 0;
    if (tab_bar->NextSelectedTabId)
    {
        tab_bar->SelectedTabId = tab_bar->NextSelectedTabId;
        tab_bar->NextSelectedTabId = 0;
        scroll_track_selected_tab_id = tab_bar->SelectedTabId;
    }

    // Process order change request (we could probably process it when requested but it's just saner to do it in a single spot).
    if (tab_bar->ReorderRequestTabId != 0)
    {
        if (LegionTabItem* tab1 = TabBarFindTabByID(tab_bar, tab_bar->ReorderRequestTabId))
        {
            //IM_ASSERT(tab_bar->Flags & LegionTabBarFlags_Reorderable); // <- this may happen when using debug tools
            int tab2_order = tab_bar->GetTabOrder(tab1) + tab_bar->ReorderRequestDir;
            if (tab2_order >= 0 && tab2_order < tab_bar->Tabs.Size)
            {
                LegionTabItem* tab2 = &tab_bar->Tabs[tab2_order];
                LegionTabItem item_tmp = *tab1;
                *tab1 = *tab2;
                *tab2 = item_tmp;
                if (tab2->ID == tab_bar->SelectedTabId)
                    scroll_track_selected_tab_id = tab2->ID;
                tab1 = tab2 = NULL;
            }
            if (tab_bar->Flags & LegionTabBarFlags_SaveSettings)
                MarkIniSettingsDirty();
        }
        tab_bar->ReorderRequestTabId = 0;
    }

    // Tab List Popup (will alter tab_bar->BarRect and therefore the available width!)
    const bool tab_list_popup_button = (tab_bar->Flags & LegionTabBarFlags_TabListPopupButton) != 0;
    if (tab_list_popup_button)
        if (LegionTabItem* tab_to_select = TabBarTabListPopupButton(tab_bar)) // NB: Will alter BarRect.Max.x!
            scroll_track_selected_tab_id = tab_bar->SelectedTabId = tab_to_select->ID;

    // Compute ideal widths
    g.ShrinkWidthBuffer.resize(tab_bar->Tabs.Size);
    float width_total_contents = 0.0f;
    LegionTabItem* most_recently_selected_tab = NULL;
    bool found_selected_tab_id = false;
    for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
    {
        LegionTabItem* tab = &tab_bar->Tabs[tab_n];
        IM_ASSERT(tab->LastFrameVisible >= tab_bar->PrevFrameVisible);

        if (most_recently_selected_tab == NULL || most_recently_selected_tab->LastFrameSelected < tab->LastFrameSelected)
            most_recently_selected_tab = tab;
        if (tab->ID == tab_bar->SelectedTabId)
            found_selected_tab_id = true;

        // Refresh tab width immediately, otherwise changes of style e.g. style.FramePadding.x would noticeably lag in the tab bar.
        // Additionally, when using TabBarAddTab() to manipulate tab bar order we occasionally insert new tabs that don't have a width yet,
        // and we cannot wait for the next BeginTabItem() call. We cannot compute this width within TabBarAddTab() because font size depends on the active window.
        const char* tab_name = tab_bar->GetTabName(tab);
        const bool has_close_button = (tab->Flags & LegionTabItemFlags_NoCloseButton) ? false : true;
        tab->ContentWidth = TabItemCalcSize(tab_name, has_close_button).x;

        width_total_contents += (tab_n > 0 ? g.Style.ItemInnerSpacing.x : 0.0f) + tab->ContentWidth;

        // Store data so we can build an array sorted by width if we need to shrink tabs down
        g.ShrinkWidthBuffer[tab_n].Index = tab_n;
        g.ShrinkWidthBuffer[tab_n].Width = tab->ContentWidth;
    }

    // Compute width
    const float initial_offset_x = 0.0f; // g.Style.ItemInnerSpacing.x;
    const float width_avail = ImMax(tab_bar->BarRect.GetWidth() - initial_offset_x, 0.0f);
    float width_excess = (width_avail < width_total_contents) ? (width_total_contents - width_avail) : 0.0f;
    if (width_excess > 0.0f && (tab_bar->Flags & LegionTabBarFlags_FittingPolicyResizeDown))
    {
        // If we don't have enough room, resize down the largest tabs first
        ShrinkWidths(g.ShrinkWidthBuffer.Data, g.ShrinkWidthBuffer.Size, width_excess);
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
            tab_bar->Tabs[g.ShrinkWidthBuffer[tab_n].Index].Width = IM_FLOOR(g.ShrinkWidthBuffer[tab_n].Width);
    }
    else
    {
        const float tab_max_width = TabBarCalcMaxTabWidth();
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
        {
            LegionTabItem* tab = &tab_bar->Tabs[tab_n];
            tab->Width = ImMin(tab->ContentWidth, tab_max_width);
            IM_ASSERT(tab->Width > 0.0f);
        }
    }

    // Layout all active tabs
    float offset_x = initial_offset_x;
    float offset_x_ideal = offset_x;
    tab_bar->OffsetNextTab = offset_x; // This is used by non-reorderable tab bar where the submission order is always honored.
    for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
    {
        LegionTabItem* tab = &tab_bar->Tabs[tab_n];
        tab->Offset = offset_x;
        if (scroll_track_selected_tab_id == 0 && g.NavJustMovedToId == tab->ID)
            scroll_track_selected_tab_id = tab->ID;
        offset_x += tab->Width + g.Style.ItemInnerSpacing.x;
        offset_x_ideal += tab->ContentWidth + g.Style.ItemInnerSpacing.x;
    }
    tab_bar->OffsetMax = ImMax(offset_x - g.Style.ItemInnerSpacing.x, 0.0f);
    tab_bar->OffsetMaxIdeal = ImMax(offset_x_ideal - g.Style.ItemInnerSpacing.x, 0.0f);

    // Horizontal scrolling buttons
    const bool scrolling_buttons = (tab_bar->OffsetMax > tab_bar->BarRect.GetWidth() && tab_bar->Tabs.Size > 1) && !(tab_bar->Flags & LegionTabBarFlags_NoTabListScrollingButtons) && (tab_bar->Flags & LegionTabBarFlags_FittingPolicyScroll);
    if (scrolling_buttons)
        if (LegionTabItem* tab_to_select = TabBarScrollingButtons(tab_bar)) // NB: Will alter BarRect.Max.x!
            scroll_track_selected_tab_id = tab_bar->SelectedTabId = tab_to_select->ID;

    // If we have lost the selected tab, select the next most recently active one
    if (found_selected_tab_id == false)
        tab_bar->SelectedTabId = 0;
    if (tab_bar->SelectedTabId == 0 && tab_bar->NextSelectedTabId == 0 && most_recently_selected_tab != NULL)
        scroll_track_selected_tab_id = tab_bar->SelectedTabId = most_recently_selected_tab->ID;

    // Lock in visible tab
    tab_bar->VisibleTabId = tab_bar->SelectedTabId;
    tab_bar->VisibleTabWasSubmitted = false;

    // Update scrolling
    if (scroll_track_selected_tab_id)
        if (LegionTabItem* scroll_track_selected_tab = TabBarFindTabByID(tab_bar, scroll_track_selected_tab_id))
            TabBarScrollToTab(tab_bar, scroll_track_selected_tab);
    tab_bar->ScrollingAnim = TabBarScrollClamp(tab_bar, tab_bar->ScrollingAnim);
    tab_bar->ScrollingTarget = TabBarScrollClamp(tab_bar, tab_bar->ScrollingTarget);
    if (tab_bar->ScrollingAnim != tab_bar->ScrollingTarget)
    {
        // Scrolling speed adjust itself so we can always reach our target in 1/3 seconds.
        // Teleport if we are aiming far off the visible line
        tab_bar->ScrollingSpeed = ImMax(tab_bar->ScrollingSpeed, 70.0f * g.FontSize);
        tab_bar->ScrollingSpeed = ImMax(tab_bar->ScrollingSpeed, ImFabs(tab_bar->ScrollingTarget - tab_bar->ScrollingAnim) / 0.3f);
        const bool teleport = (tab_bar->PrevFrameVisible + 1 < g.FrameCount) || (tab_bar->ScrollingTargetDistToVisibility > 10.0f * g.FontSize);
        tab_bar->ScrollingAnim = teleport ? tab_bar->ScrollingTarget : ImLinearSweep(tab_bar->ScrollingAnim, tab_bar->ScrollingTarget, g.IO.DeltaTime * tab_bar->ScrollingSpeed);
    }
    else
    {
        tab_bar->ScrollingSpeed = 0.0f;
    }

    // Clear name buffers
    if ((tab_bar->Flags & LegionTabBarFlags_DockNode) == 0)
        tab_bar->TabsNames.Buf.resize(0);
}

// Dockables uses Name/ID in the global namespace. Non-dockable items use the ID stack.
static ImU32   Legion::TabBarCalcTabID(LegionTabBar* tab_bar, const char* label)
{
    if (tab_bar->Flags & LegionTabBarFlags_DockNode)
    {
        LegionID id = ImHashStr(label);
        KeepAliveID(id);
        return id;
    }
    else
    {
        LegionWindow* window = GLegion->CurrentWindow;
        return window->GetID(label);
    }
}

static float Legion::TabBarCalcMaxTabWidth()
{
    LegionContext& g = *GLegion;
    return g.FontSize * 20.0f;
}

LegionTabItem* Legion::TabBarFindTabByID(LegionTabBar* tab_bar, LegionID tab_id)
{
    if (tab_id != 0)
        for (int n = 0; n < tab_bar->Tabs.Size; n++)
            if (tab_bar->Tabs[n].ID == tab_id)
                return &tab_bar->Tabs[n];
    return NULL;
}

// The *TabId fields be already set by the docking system _before_ the actual TabItem was created, so we clear them regardless.
void Legion::TabBarRemoveTab(LegionTabBar* tab_bar, LegionID tab_id)
{
    if (LegionTabItem* tab = TabBarFindTabByID(tab_bar, tab_id))
        tab_bar->Tabs.erase(tab);
    if (tab_bar->VisibleTabId == tab_id)      { tab_bar->VisibleTabId = 0; }
    if (tab_bar->SelectedTabId == tab_id)     { tab_bar->SelectedTabId = 0; }
    if (tab_bar->NextSelectedTabId == tab_id) { tab_bar->NextSelectedTabId = 0; }
}

// Called on manual closure attempt
void Legion::TabBarCloseTab(LegionTabBar* tab_bar, LegionTabItem* tab)
{
    if ((tab_bar->VisibleTabId == tab->ID) && !(tab->Flags & LegionTabItemFlags_UnsavedDocument))
    {
        // This will remove a frame of lag for selecting another tab on closure.
        // However we don't run it in the case where the 'Unsaved' flag is set, so user gets a chance to fully undo the closure
        tab->LastFrameVisible = -1;
        tab_bar->SelectedTabId = tab_bar->NextSelectedTabId = 0;
    }
    else if ((tab_bar->VisibleTabId != tab->ID) && (tab->Flags & LegionTabItemFlags_UnsavedDocument))
    {
        // Actually select before expecting closure
        tab_bar->NextSelectedTabId = tab->ID;
    }
}

static float Legion::TabBarScrollClamp(LegionTabBar* tab_bar, float scrolling)
{
    scrolling = ImMin(scrolling, tab_bar->OffsetMax - tab_bar->BarRect.GetWidth());
    return ImMax(scrolling, 0.0f);
}

static void Legion::TabBarScrollToTab(LegionTabBar* tab_bar, LegionTabItem* tab)
{
    LegionContext& g = *GLegion;
    float margin = g.FontSize * 1.0f; // When to scroll to make Tab N+1 visible always make a bit of N visible to suggest more scrolling area (since we don't have a scrollbar)
    int order = tab_bar->GetTabOrder(tab);
    float tab_x1 = tab->Offset + (order > 0 ? -margin : 0.0f);
    float tab_x2 = tab->Offset + tab->Width + (order + 1 < tab_bar->Tabs.Size ? margin : 1.0f);
    tab_bar->ScrollingTargetDistToVisibility = 0.0f;
    if (tab_bar->ScrollingTarget > tab_x1 || (tab_x2 - tab_x1 >= tab_bar->BarRect.GetWidth()))
    {
        tab_bar->ScrollingTargetDistToVisibility = ImMax(tab_bar->ScrollingAnim - tab_x2, 0.0f);
        tab_bar->ScrollingTarget = tab_x1;
    }
    else if (tab_bar->ScrollingTarget < tab_x2 - tab_bar->BarRect.GetWidth())
    {
        tab_bar->ScrollingTargetDistToVisibility = ImMax((tab_x1 - tab_bar->BarRect.GetWidth()) - tab_bar->ScrollingAnim, 0.0f);
        tab_bar->ScrollingTarget = tab_x2 - tab_bar->BarRect.GetWidth();
    }
}

void Legion::TabBarQueueChangeTabOrder(LegionTabBar* tab_bar, const LegionTabItem* tab, int dir)
{
    IM_ASSERT(dir == -1 || dir == +1);
    IM_ASSERT(tab_bar->ReorderRequestTabId == 0);
    tab_bar->ReorderRequestTabId = tab->ID;
    tab_bar->ReorderRequestDir = (ImS8)dir;
}

static LegionTabItem* Legion::TabBarScrollingButtons(LegionTabBar* tab_bar)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    const ImVec2 arrow_button_size(g.FontSize - 2.0f, g.FontSize + g.Style.FramePadding.y * 2.0f);
    const float scrolling_buttons_width = arrow_button_size.x * 2.0f;

    const ImVec2 backup_cursor_pos = window->DC.CursorPos;
    //window->DrawList->AddRect(ImVec2(tab_bar->BarRect.Max.x - scrolling_buttons_width, tab_bar->BarRect.Min.y), ImVec2(tab_bar->BarRect.Max.x, tab_bar->BarRect.Max.y), IM_COL32(255,0,0,255));

    const ImRect avail_bar_rect = tab_bar->BarRect;
    bool want_clip_rect = !avail_bar_rect.Contains(ImRect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(scrolling_buttons_width, 0.0f)));
    if (want_clip_rect)
        PushClipRect(tab_bar->BarRect.Min, tab_bar->BarRect.Max + ImVec2(g.Style.ItemInnerSpacing.x, 0.0f), true);

    LegionTabItem* tab_to_select = NULL;

    int select_dir = 0;
    ImVec4 arrow_col = g.Style.Colors[LegionCol_Text];
    arrow_col.w *= 0.5f;

    PushStyleColor(LegionCol_Text, arrow_col);
    PushStyleColor(LegionCol_Button, ImVec4(0, 0, 0, 0));
    const float backup_repeat_delay = g.IO.KeyRepeatDelay;
    const float backup_repeat_rate = g.IO.KeyRepeatRate;
    g.IO.KeyRepeatDelay = 0.250f;
    g.IO.KeyRepeatRate = 0.200f;
    window->DC.CursorPos = ImVec2(tab_bar->BarRect.Max.x - scrolling_buttons_width, tab_bar->BarRect.Min.y);
    if (ArrowButtonEx("##<", LegionDir_Left, arrow_button_size, LegionButtonFlags_PressedOnClick | LegionButtonFlags_Repeat))
        select_dir = -1;
    window->DC.CursorPos = ImVec2(tab_bar->BarRect.Max.x - scrolling_buttons_width + arrow_button_size.x, tab_bar->BarRect.Min.y);
    if (ArrowButtonEx("##>", LegionDir_Right, arrow_button_size, LegionButtonFlags_PressedOnClick | LegionButtonFlags_Repeat))
        select_dir = +1;
    PopStyleColor(2);
    g.IO.KeyRepeatRate = backup_repeat_rate;
    g.IO.KeyRepeatDelay = backup_repeat_delay;

    if (want_clip_rect)
        PopClipRect();

    if (select_dir != 0)
        if (LegionTabItem* tab_item = TabBarFindTabByID(tab_bar, tab_bar->SelectedTabId))
        {
            int selected_order = tab_bar->GetTabOrder(tab_item);
            int target_order = selected_order + select_dir;
            tab_to_select = &tab_bar->Tabs[(target_order >= 0 && target_order < tab_bar->Tabs.Size) ? target_order : selected_order]; // If we are at the end of the list, still scroll to make our tab visible
        }
    window->DC.CursorPos = backup_cursor_pos;
    tab_bar->BarRect.Max.x -= scrolling_buttons_width + 1.0f;

    return tab_to_select;
}

static LegionTabItem* Legion::TabBarTabListPopupButton(LegionTabBar* tab_bar)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;

    // We use g.Style.FramePadding.y to match the square ArrowButton size
    const float tab_list_popup_button_width = g.FontSize + g.Style.FramePadding.y;
    const ImVec2 backup_cursor_pos = window->DC.CursorPos;
    window->DC.CursorPos = ImVec2(tab_bar->BarRect.Min.x - g.Style.FramePadding.y, tab_bar->BarRect.Min.y);
    tab_bar->BarRect.Min.x += tab_list_popup_button_width;

    ImVec4 arrow_col = g.Style.Colors[LegionCol_Text];
    arrow_col.w *= 0.5f;
    PushStyleColor(LegionCol_Text, arrow_col);
    PushStyleColor(LegionCol_Button, ImVec4(0, 0, 0, 0));
    bool open = BeginCombo("##v", NULL, LegionComboFlags_NoPreview);
    PopStyleColor(2);

    LegionTabItem* tab_to_select = NULL;
    if (open)
    {
        for (int tab_n = 0; tab_n < tab_bar->Tabs.Size; tab_n++)
        {
            LegionTabItem* tab = &tab_bar->Tabs[tab_n];
            const char* tab_name = tab_bar->GetTabName(tab);
            if (Selectable(tab_name, tab_bar->SelectedTabId == tab->ID))
                tab_to_select = tab;
        }
        EndCombo();
    }

    window->DC.CursorPos = backup_cursor_pos;
    return tab_to_select;
}

//-------------------------------------------------------------------------
// [SECTION] Widgets: BeginTabItem, EndTabItem, etc.
//-------------------------------------------------------------------------
// - BeginTabItem()
// - EndTabItem()
// - TabItemEx() [Internal]
// - SetTabItemClosed()
// - TabItemCalcSize() [Internal]
// - TabItemBackground() [Internal]
// - TabItemLabelAndCloseButton() [Internal]
//-------------------------------------------------------------------------

bool    Legion::BeginTabItem(const char* label, bool* p_open, LegionTabItemFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    LegionTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        IM_ASSERT_USER_ERROR(tab_bar, "BeginTabItem() Needs to be called between BeginTabBar() and EndTabBar()!");
        return false;
    }
    bool ret = TabItemEx(tab_bar, label, p_open, flags);
    if (ret && !(flags & LegionTabItemFlags_NoPushId))
    {
        LegionTabItem* tab = &tab_bar->Tabs[tab_bar->LastTabItemIdx];
        PushOverrideID(tab->ID); // We already hashed 'label' so push into the ID stack directly instead of doing another hash through PushID(label)
    }
    return ret;
}

void    Legion::EndTabItem()
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return;

    LegionTabBar* tab_bar = g.CurrentTabBar;
    if (tab_bar == NULL)
    {
        IM_ASSERT(tab_bar != NULL && "Needs to be called between BeginTabBar() and EndTabBar()!");
        return;
    }
    IM_ASSERT(tab_bar->LastTabItemIdx >= 0);
    LegionTabItem* tab = &tab_bar->Tabs[tab_bar->LastTabItemIdx];
    if (!(tab->Flags & LegionTabItemFlags_NoPushId))
        window->IDStack.pop_back();
}

bool    Legion::TabItemEx(LegionTabBar* tab_bar, const char* label, bool* p_open, LegionTabItemFlags flags)
{
    // Layout whole tab bar if not already done
    if (tab_bar->WantLayout)
        TabBarLayout(tab_bar);

    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const LegionStyle& style = g.Style;
    const LegionID id = TabBarCalcTabID(tab_bar, label);

    // If the user called us with *p_open == false, we early out and don't render. We make a dummy call to ItemAdd() so that attempts to use a contextual popup menu with an implicit ID won't use an older ID.
    if (p_open && !*p_open)
    {
        PushItemFlag(LegionItemFlags_NoNav | LegionItemFlags_NoNavDefaultFocus, true);
        ItemAdd(ImRect(), id);
        PopItemFlag();
        return false;
    }

    // Store into LegionTabItemFlags_NoCloseButton, also honor LegionTabItemFlags_NoCloseButton passed by user (although not documented)
    if (flags & LegionTabItemFlags_NoCloseButton)
        p_open = NULL;
    else if (p_open == NULL)
        flags |= LegionTabItemFlags_NoCloseButton;

    // Calculate tab contents size
    ImVec2 size = TabItemCalcSize(label, p_open != NULL);

    // Acquire tab data
    LegionTabItem* tab = TabBarFindTabByID(tab_bar, id);
    bool tab_is_new = false;
    if (tab == NULL)
    {
        tab_bar->Tabs.push_back(LegionTabItem());
        tab = &tab_bar->Tabs.back();
        tab->ID = id;
        tab->Width = size.x;
        tab_is_new = true;
    }
    tab_bar->LastTabItemIdx = (short)tab_bar->Tabs.index_from_ptr(tab);
    tab->ContentWidth = size.x;

    const bool tab_bar_appearing = (tab_bar->PrevFrameVisible + 1 < g.FrameCount);
    const bool tab_bar_focused = (tab_bar->Flags & LegionTabBarFlags_IsFocused) != 0;
    const bool tab_appearing = (tab->LastFrameVisible + 1 < g.FrameCount);
    tab->LastFrameVisible = g.FrameCount;
    tab->Flags = flags;

    // Append name with zero-terminator
    tab->NameOffset = tab_bar->TabsNames.size();
    tab_bar->TabsNames.append(label, label + strlen(label) + 1);

    // If we are not reorderable, always reset offset based on submission order.
    // (We already handled layout and sizing using the previous known order, but sizing is not affected by order!)
    if (!tab_appearing && !(tab_bar->Flags & LegionTabBarFlags_Reorderable))
    {
        tab->Offset = tab_bar->OffsetNextTab;
        tab_bar->OffsetNextTab += tab->Width + g.Style.ItemInnerSpacing.x;
    }

    // Update selected tab
    if (tab_appearing && (tab_bar->Flags & LegionTabBarFlags_AutoSelectNewTabs) && tab_bar->NextSelectedTabId == 0)
        if (!tab_bar_appearing || tab_bar->SelectedTabId == 0)
            tab_bar->NextSelectedTabId = id;  // New tabs gets activated
    if ((flags & LegionTabItemFlags_SetSelected) && (tab_bar->SelectedTabId != id)) // SetSelected can only be passed on explicit tab bar
        tab_bar->NextSelectedTabId = id;

    // Lock visibility
    bool tab_contents_visible = (tab_bar->VisibleTabId == id);
    if (tab_contents_visible)
        tab_bar->VisibleTabWasSubmitted = true;

    // On the very first frame of a tab bar we let first tab contents be visible to minimize appearing glitches
    if (!tab_contents_visible && tab_bar->SelectedTabId == 0 && tab_bar_appearing)
        if (tab_bar->Tabs.Size == 1 && !(tab_bar->Flags & LegionTabBarFlags_AutoSelectNewTabs))
            tab_contents_visible = true;

    if (tab_appearing && !(tab_bar_appearing && !tab_is_new))
    {
        PushItemFlag(LegionItemFlags_NoNav | LegionItemFlags_NoNavDefaultFocus, true);
        ItemAdd(ImRect(), id);
        PopItemFlag();
        return tab_contents_visible;
    }

    if (tab_bar->SelectedTabId == id)
        tab->LastFrameSelected = g.FrameCount;

    // Backup current layout position
    const ImVec2 backup_main_cursor_pos = window->DC.CursorPos;

    // Layout
    size.x = tab->Width;
    window->DC.CursorPos = tab_bar->BarRect.Min + ImVec2(IM_FLOOR(tab->Offset - tab_bar->ScrollingAnim), 0.0f);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, pos + size);

    // We don't have CPU clipping primitives to clip the CloseButton (until it becomes a texture), so need to add an extra draw call (temporary in the case of vertical animation)
    bool want_clip_rect = (bb.Min.x < tab_bar->BarRect.Min.x) || (bb.Max.x > tab_bar->BarRect.Max.x);
    if (want_clip_rect)
        PushClipRect(ImVec2(ImMax(bb.Min.x, tab_bar->BarRect.Min.x), bb.Min.y - 1), ImVec2(tab_bar->BarRect.Max.x, bb.Max.y), true);

    ImVec2 backup_cursor_max_pos = window->DC.CursorMaxPos;
    ItemSize(bb.GetSize(), style.FramePadding.y);
    window->DC.CursorMaxPos = backup_cursor_max_pos;

    if (!ItemAdd(bb, id))
    {
        if (want_clip_rect)
            PopClipRect();
        window->DC.CursorPos = backup_main_cursor_pos;
        return tab_contents_visible;
    }

    // Click to Select a tab
    LegionButtonFlags button_flags = (LegionButtonFlags_PressedOnClick | LegionButtonFlags_AllowItemOverlap);
    if (g.DragDropActive)
        button_flags |= LegionButtonFlags_PressedOnDragDropHold;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);
    if (pressed)
        tab_bar->NextSelectedTabId = id;
    hovered |= (g.HoveredId == id);

    // Allow the close button to overlap unless we are dragging (in which case we don't want any overlapping tabs to be hovered)
    if (!held)
        SetItemAllowOverlap();

    // Drag and drop: re-order tabs
    if (held && !tab_appearing && IsMouseDragging(0))
    {
        if (!g.DragDropActive && (tab_bar->Flags & LegionTabBarFlags_Reorderable))
        {
            // While moving a tab it will jump on the other side of the mouse, so we also test for MouseDelta.x
            if (g.IO.MouseDelta.x < 0.0f && g.IO.MousePos.x < bb.Min.x)
            {
                if (tab_bar->Flags & LegionTabBarFlags_Reorderable)
                    TabBarQueueChangeTabOrder(tab_bar, tab, -1);
            }
            else if (g.IO.MouseDelta.x > 0.0f && g.IO.MousePos.x > bb.Max.x)
            {
                if (tab_bar->Flags & LegionTabBarFlags_Reorderable)
                    TabBarQueueChangeTabOrder(tab_bar, tab, +1);
            }
        }
    }

#if 0
    if (hovered && g.HoveredIdNotActiveTimer > 0.50f && bb.GetWidth() < tab->ContentWidth)
    {
        // Enlarge tab display when hovering
        bb.Max.x = bb.Min.x + IM_FLOOR(ImLerp(bb.GetWidth(), tab->ContentWidth, ImSaturate((g.HoveredIdNotActiveTimer - 0.40f) * 6.0f)));
        display_draw_list = GetForegroundDrawList(window);
        TabItemBackground(display_draw_list, bb, flags, GetColorU32(LegionCol_TitleBgActive));
    }
#endif

    // Render tab shape
    ImDrawList* display_draw_list = window->DrawList;
    const ImU32 tab_col = GetColorU32((held || hovered) ? LegionCol_TabHovered : tab_contents_visible ? (tab_bar_focused ? LegionCol_TabActive : LegionCol_TabUnfocusedActive) : (tab_bar_focused ? LegionCol_Tab : LegionCol_TabUnfocused));
    TabItemBackground(display_draw_list, bb, flags, tab_col);
    RenderNavHighlight(bb, id);

    // Select with right mouse button. This is so the common idiom for context menu automatically highlight the current widget.
    const bool hovered_unblocked = IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByPopup);
    if (hovered_unblocked && (IsMouseClicked(1) || IsMouseReleased(1)))
        tab_bar->NextSelectedTabId = id;

    if (tab_bar->Flags & LegionTabBarFlags_NoCloseWithMiddleMouseButton)
        flags |= LegionTabItemFlags_NoCloseWithMiddleMouseButton;

    // Render tab label, process close button
    const LegionID close_button_id = p_open ? window->GetID((void*)((intptr_t)id + 1)) : 0;
    bool just_closed = TabItemLabelAndCloseButton(display_draw_list, bb, flags, tab_bar->FramePadding, label, id, close_button_id);
    if (just_closed && p_open != NULL)
    {
        *p_open = false;
        TabBarCloseTab(tab_bar, tab);
    }

    // Restore main window position so user can draw there
    if (want_clip_rect)
        PopClipRect();
    window->DC.CursorPos = backup_main_cursor_pos;

    // Tooltip (FIXME: Won't work over the close button because ItemOverlap systems messes up with HoveredIdTimer)
    // We test IsItemHovered() to discard e.g. when another item is active or drag and drop over the tab bar (which g.HoveredId ignores)
    if (g.HoveredId == id && !held && g.HoveredIdNotActiveTimer > 0.50f && IsItemHovered())
        if (!(tab_bar->Flags & LegionTabBarFlags_NoTooltip))
            SetTooltip("%.*s", (int)(FindRenderedTextEnd(label) - label), label);

    return tab_contents_visible;
}

// [Public] This is call is 100% optional but it allows to remove some one-frame glitches when a tab has been unexpectedly removed.
// To use it to need to call the function SetTabItemClosed() after BeginTabBar() and before any call to BeginTabItem()
void    Legion::SetTabItemClosed(const char* label)
{
    LegionContext& g = *GLegion;
    bool is_within_manual_tab_bar = g.CurrentTabBar && !(g.CurrentTabBar->Flags & LegionTabBarFlags_DockNode);
    if (is_within_manual_tab_bar)
    {
        LegionTabBar* tab_bar = g.CurrentTabBar;
        IM_ASSERT(tab_bar->WantLayout);         // Needs to be called AFTER BeginTabBar() and BEFORE the first call to BeginTabItem()
        LegionID tab_id = TabBarCalcTabID(tab_bar, label);
        TabBarRemoveTab(tab_bar, tab_id);
    }
}

ImVec2 Legion::TabItemCalcSize(const char* label, bool has_close_button)
{
    LegionContext& g = *GLegion;
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size = ImVec2(label_size.x + g.Style.FramePadding.x, label_size.y + g.Style.FramePadding.y * 2.0f);
    if (has_close_button)
        size.x += g.Style.FramePadding.x + (g.Style.ItemInnerSpacing.x + g.FontSize); // We use Y intentionally to fit the close button circle.
    else
        size.x += g.Style.FramePadding.x + 1.0f;
    return ImVec2(ImMin(size.x, TabBarCalcMaxTabWidth()), size.y);
}

void Legion::TabItemBackground(ImDrawList* draw_list, const ImRect& bb, LegionTabItemFlags flags, ImU32 col)
{
    // While rendering tabs, we trim 1 pixel off the top of our bounding box so they can fit within a regular frame height while looking "detached" from it.
    LegionContext& g = *GLegion;
    const float width = bb.GetWidth();
    IM_UNUSED(flags);
    IM_ASSERT(width > 0.0f);
    const float rounding = ImMax(0.0f, ImMin(g.Style.TabRounding, width * 0.5f - 1.0f));
    const float y1 = bb.Min.y + 1.0f;
    const float y2 = bb.Max.y - 1.0f;
    draw_list->PathLineTo(ImVec2(bb.Min.x, y2));
    draw_list->PathArcToFast(ImVec2(bb.Min.x + rounding, y1 + rounding), rounding, 6, 9);
    draw_list->PathArcToFast(ImVec2(bb.Max.x - rounding, y1 + rounding), rounding, 9, 12);
    draw_list->PathLineTo(ImVec2(bb.Max.x, y2));
    draw_list->PathFillConvex(col);
    if (g.Style.TabBorderSize > 0.0f)
    {
        draw_list->PathLineTo(ImVec2(bb.Min.x + 0.5f, y2));
        draw_list->PathArcToFast(ImVec2(bb.Min.x + rounding + 0.5f, y1 + rounding + 0.5f), rounding, 6, 9);
        draw_list->PathArcToFast(ImVec2(bb.Max.x - rounding - 0.5f, y1 + rounding + 0.5f), rounding, 9, 12);
        draw_list->PathLineTo(ImVec2(bb.Max.x - 0.5f, y2));
        draw_list->PathStroke(GetColorU32(LegionCol_Border), false, g.Style.TabBorderSize);
    }
}

// Render text label (with custom clipping) + Unsaved Document marker + Close Button logic
// We tend to lock style.FramePadding for a given tab-bar, hence the 'frame_padding' parameter.
bool Legion::TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect& bb, LegionTabItemFlags flags, ImVec2 frame_padding, const char* label, LegionID tab_id, LegionID close_button_id)
{
    LegionContext& g = *GLegion;
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    if (bb.GetWidth() <= 1.0f)
        return false;

    // Render text label (with clipping + alpha gradient) + unsaved marker
    const char* TAB_UNSAVED_MARKER = "*";
    ImRect text_pixel_clip_bb(bb.Min.x + frame_padding.x, bb.Min.y + frame_padding.y, bb.Max.x - frame_padding.x, bb.Max.y);
    if (flags & LegionTabItemFlags_UnsavedDocument)
    {
        text_pixel_clip_bb.Max.x -= CalcTextSize(TAB_UNSAVED_MARKER, NULL, false).x;
        ImVec2 unsaved_marker_pos(ImMin(bb.Min.x + frame_padding.x + label_size.x + 2, text_pixel_clip_bb.Max.x), bb.Min.y + frame_padding.y + IM_FLOOR(-g.FontSize * 0.25f));
        RenderTextClippedEx(draw_list, unsaved_marker_pos, bb.Max - frame_padding, TAB_UNSAVED_MARKER, NULL, NULL);
    }
    ImRect text_ellipsis_clip_bb = text_pixel_clip_bb;

    // Close Button
    // We are relying on a subtle and confusing distinction between 'hovered' and 'g.HoveredId' which happens because we are using LegionButtonFlags_AllowOverlapMode + SetItemAllowOverlap()
    //  'hovered' will be true when hovering the Tab but NOT when hovering the close button
    //  'g.HoveredId==id' will be true when hovering the Tab including when hovering the close button
    //  'g.ActiveId==close_button_id' will be true when we are holding on the close button, in which case both hovered booleans are false
    bool close_button_pressed = false;
    bool close_button_visible = false;
    if (close_button_id != 0)
        if (g.HoveredId == tab_id || g.HoveredId == close_button_id || g.ActiveId == close_button_id)
            close_button_visible = true;
    if (close_button_visible)
    {
        LegionItemHoveredDataBackup last_item_backup;
        const float close_button_sz = g.FontSize;
        PushStyleVar(LegionStyleVar_FramePadding, frame_padding);
        if (CloseButton(close_button_id, ImVec2(bb.Max.x - frame_padding.x * 2.0f - close_button_sz, bb.Min.y)))
            close_button_pressed = true;
        PopStyleVar();
        last_item_backup.Restore();

        // Close with middle mouse button
        if (!(flags & LegionTabItemFlags_NoCloseWithMiddleMouseButton) && IsMouseClicked(2))
            close_button_pressed = true;

        text_pixel_clip_bb.Max.x -= close_button_sz;
    }

    float ellipsis_max_x = close_button_visible ? text_pixel_clip_bb.Max.x : bb.Max.x - 1.0f;
    RenderTextEllipsis(draw_list, text_ellipsis_clip_bb.Min, text_ellipsis_clip_bb.Max, text_pixel_clip_bb.Max.x, ellipsis_max_x, label, NULL, &label_size);

    return close_button_pressed;
}


//-------------------------------------------------------------------------
// [SECTION] Widgets: Columns, BeginColumns, EndColumns, etc.
// In the current version, Columns are very weak. Needs to be replaced with a more full-featured system.
//-------------------------------------------------------------------------
// - GetColumnIndex()
// - GetColumnCount()
// - GetColumnOffset()
// - GetColumnWidth()
// - SetColumnOffset()
// - SetColumnWidth()
// - PushColumnClipRect() [Internal]
// - PushColumnsBackground() [Internal]
// - PopColumnsBackground() [Internal]
// - FindOrCreateColumns() [Internal]
// - GetColumnsID() [Internal]
// - BeginColumns()
// - NextColumn()
// - EndColumns()
// - Columns()
//-------------------------------------------------------------------------

int Legion::GetColumnIndex()
{
    LegionWindow* window = GetCurrentWindowRead();
    return window->DC.CurrentColumns ? window->DC.CurrentColumns->Current : 0;
}

int Legion::GetColumnsCount()
{
    LegionWindow* window = GetCurrentWindowRead();
    return window->DC.CurrentColumns ? window->DC.CurrentColumns->Count : 1;
}

float Legion::GetColumnOffsetFromNorm(const LegionColumns* columns, float offset_norm)
{
    return offset_norm * (columns->OffMaxX - columns->OffMinX);
}

float Legion::GetColumnNormFromOffset(const LegionColumns* columns, float offset)
{
    return offset / (columns->OffMaxX - columns->OffMinX);
}

static const float COLUMNS_HIT_RECT_HALF_WIDTH = 4.0f;

static float GetDraggedColumnOffset(LegionColumns* columns, int column_index)
{
    // Active (dragged) column always follow mouse. The reason we need this is that dragging a column to the right edge of an auto-resizing
    // window creates a feedback loop because we store normalized positions. So while dragging we enforce absolute positioning.
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    IM_ASSERT(column_index > 0); // We are not supposed to drag column 0.
    IM_ASSERT(g.ActiveId == columns->ID + LegionID(column_index));

    float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + COLUMNS_HIT_RECT_HALF_WIDTH - window->Pos.x;
    x = ImMax(x, Legion::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing);
    if ((columns->Flags & LegionColumnsFlags_NoPreserveWidths))
        x = ImMin(x, Legion::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

    return x;
}

float Legion::GetColumnOffset(int column_index)
{
    LegionWindow* window = GetCurrentWindowRead();
    LegionColumns* columns = window->DC.CurrentColumns;
    if (columns == NULL)
        return 0.0f;

    if (column_index < 0)
        column_index = columns->Current;
    IM_ASSERT(column_index < columns->Columns.Size);

    const float t = columns->Columns[column_index].OffsetNorm;
    const float x_offset = ImLerp(columns->OffMinX, columns->OffMaxX, t);
    return x_offset;
}

static float GetColumnWidthEx(LegionColumns* columns, int column_index, bool before_resize = false)
{
    if (column_index < 0)
        column_index = columns->Current;

    float offset_norm;
    if (before_resize)
        offset_norm = columns->Columns[column_index + 1].OffsetNormBeforeResize - columns->Columns[column_index].OffsetNormBeforeResize;
    else
        offset_norm = columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm;
    return Legion::GetColumnOffsetFromNorm(columns, offset_norm);
}

float Legion::GetColumnWidth(int column_index)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    LegionColumns* columns = window->DC.CurrentColumns;
    if (columns == NULL)
        return GetContentRegionAvail().x;

    if (column_index < 0)
        column_index = columns->Current;
    return GetColumnOffsetFromNorm(columns, columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm);
}

void Legion::SetColumnOffset(int column_index, float offset)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = g.CurrentWindow;
    LegionColumns* columns = window->DC.CurrentColumns;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    IM_ASSERT(column_index < columns->Columns.Size);

    const bool preserve_width = !(columns->Flags & LegionColumnsFlags_NoPreserveWidths) && (column_index < columns->Count-1);
    const float width = preserve_width ? GetColumnWidthEx(columns, column_index, columns->IsBeingResized) : 0.0f;

    if (!(columns->Flags & LegionColumnsFlags_NoForceWithinWindow))
        offset = ImMin(offset, columns->OffMaxX - g.Style.ColumnsMinSpacing * (columns->Count - column_index));
    columns->Columns[column_index].OffsetNorm = GetColumnNormFromOffset(columns, offset - columns->OffMinX);

    if (preserve_width)
        SetColumnOffset(column_index + 1, offset + ImMax(g.Style.ColumnsMinSpacing, width));
}

void Legion::SetColumnWidth(int column_index, float width)
{
    LegionWindow* window = GetCurrentWindowRead();
    LegionColumns* columns = window->DC.CurrentColumns;
    IM_ASSERT(columns != NULL);

    if (column_index < 0)
        column_index = columns->Current;
    SetColumnOffset(column_index + 1, GetColumnOffset(column_index) + width);
}

void Legion::PushColumnClipRect(int column_index)
{
    LegionWindow* window = GetCurrentWindowRead();
    LegionColumns* columns = window->DC.CurrentColumns;
    if (column_index < 0)
        column_index = columns->Current;

    LegionColumnData* column = &columns->Columns[column_index];
    PushClipRect(column->ClipRect.Min, column->ClipRect.Max, false);
}

// Get into the columns background draw command (which is generally the same draw command as before we called BeginColumns)
void Legion::PushColumnsBackground()
{
    LegionWindow* window = GetCurrentWindowRead();
    LegionColumns* columns = window->DC.CurrentColumns;
    if (columns->Count == 1)
        return;
    columns->Splitter.SetCurrentChannel(window->DrawList, 0);
    int cmd_size = window->DrawList->CmdBuffer.Size;
    PushClipRect(columns->HostClipRect.Min, columns->HostClipRect.Max, false);
    IM_UNUSED(cmd_size);
    IM_ASSERT(cmd_size == window->DrawList->CmdBuffer.Size); // Being in channel 0 this should not have created an ImDrawCmd
}

void Legion::PopColumnsBackground()
{
    LegionWindow* window = GetCurrentWindowRead();
    LegionColumns* columns = window->DC.CurrentColumns;
    if (columns->Count == 1)
        return;
    columns->Splitter.SetCurrentChannel(window->DrawList, columns->Current + 1);
    PopClipRect();
}

LegionColumns* Legion::FindOrCreateColumns(LegionWindow* window, LegionID id)
{
    // We have few columns per window so for now we don't need bother much with turning this into a faster lookup.
    for (int n = 0; n < window->ColumnsStorage.Size; n++)
        if (window->ColumnsStorage[n].ID == id)
            return &window->ColumnsStorage[n];

    window->ColumnsStorage.push_back(LegionColumns());
    LegionColumns* columns = &window->ColumnsStorage.back();
    columns->ID = id;
    return columns;
}

LegionID Legion::GetColumnsID(const char* str_id, int columns_count)
{
    LegionWindow* window = GetCurrentWindow();

    // Differentiate column ID with an arbitrary prefix for cases where users name their columns set the same as another widget.
    // In addition, when an identifier isn't explicitly provided we include the number of columns in the hash to make it uniquer.
    PushID(0x11223347 + (str_id ? 0 : columns_count));
    LegionID id = window->GetID(str_id ? str_id : "columns");
    PopID();

    return id;
}

void Legion::BeginColumns(const char* str_id, int columns_count, LegionColumnsFlags flags)
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();

    IM_ASSERT(columns_count >= 1);
    IM_ASSERT(window->DC.CurrentColumns == NULL);   // Nested columns are currently not supported

    // Acquire storage for the columns set
    LegionID id = GetColumnsID(str_id, columns_count);
    LegionColumns* columns = FindOrCreateColumns(window, id);
    IM_ASSERT(columns->ID == id);
    columns->Current = 0;
    columns->Count = columns_count;
    columns->Flags = flags;
    window->DC.CurrentColumns = columns;

    columns->HostCursorPosY = window->DC.CursorPos.y;
    columns->HostCursorMaxPosX = window->DC.CursorMaxPos.x;
    columns->HostClipRect = window->ClipRect;
    columns->HostWorkRect = window->WorkRect;

    // Set state for first column
    // We aim so that the right-most column will have the same clipping width as other after being clipped by parent ClipRect
    const float column_padding = g.Style.ItemSpacing.x;
    const float half_clip_extend_x = ImFloor(ImMax(window->WindowPadding.x * 0.5f, window->WindowBorderSize));
    const float max_1 = window->WorkRect.Max.x + column_padding - ImMax(column_padding - window->WindowPadding.x, 0.0f);
    const float max_2 = window->WorkRect.Max.x + half_clip_extend_x;
    columns->OffMinX = window->DC.Indent.x - column_padding + ImMax(column_padding - window->WindowPadding.x, 0.0f);
    columns->OffMaxX = ImMax(ImMin(max_1, max_2) - window->Pos.x, columns->OffMinX + 1.0f);
    columns->LineMinY = columns->LineMaxY = window->DC.CursorPos.y;

    // Clear data if columns count changed
    if (columns->Columns.Size != 0 && columns->Columns.Size != columns_count + 1)
        columns->Columns.resize(0);

    // Initialize default widths
    columns->IsFirstFrame = (columns->Columns.Size == 0);
    if (columns->Columns.Size == 0)
    {
        columns->Columns.reserve(columns_count + 1);
        for (int n = 0; n < columns_count + 1; n++)
        {
            LegionColumnData column;
            column.OffsetNorm = n / (float)columns_count;
            columns->Columns.push_back(column);
        }
    }

    for (int n = 0; n < columns_count; n++)
    {
        // Compute clipping rectangle
        LegionColumnData* column = &columns->Columns[n];
        float clip_x1 = IM_ROUND(window->Pos.x + GetColumnOffset(n));
        float clip_x2 = IM_ROUND(window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
        column->ClipRect = ImRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
        column->ClipRect.ClipWith(window->ClipRect);
    }

    if (columns->Count > 1)
    {
        columns->Splitter.Split(window->DrawList, 1 + columns->Count);
        columns->Splitter.SetCurrentChannel(window->DrawList, 1);
        PushColumnClipRect(0);
    }

    // We don't generally store Indent.x inside ColumnsOffset because it may be manipulated by the user.
    float offset_0 = GetColumnOffset(columns->Current);
    float offset_1 = GetColumnOffset(columns->Current + 1);
    float width = offset_1 - offset_0;
    PushItemWidth(width * 0.65f);
    window->DC.ColumnsOffset.x = ImMax(column_padding - window->WindowPadding.x, 0.0f);
    window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
    window->WorkRect.Max.x = window->Pos.x + offset_1 - column_padding;
}

void Legion::NextColumn()
{
    LegionWindow* window = GetCurrentWindow();
    if (window->SkipItems || window->DC.CurrentColumns == NULL)
        return;

    LegionContext& g = *GLegion;
    LegionColumns* columns = window->DC.CurrentColumns;

    if (columns->Count == 1)
    {
        window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
        IM_ASSERT(columns->Current == 0);
        return;
    }
    PopItemWidth();
    PopClipRect();

    const float column_padding = g.Style.ItemSpacing.x;
    columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
    if (++columns->Current < columns->Count)
    {
        // Columns 1+ ignore IndentX (by canceling it out)
        // FIXME-COLUMNS: Unnecessary, could be locked?
        window->DC.ColumnsOffset.x = GetColumnOffset(columns->Current) - window->DC.Indent.x + column_padding;
        columns->Splitter.SetCurrentChannel(window->DrawList, columns->Current + 1);
    }
    else
    {
        // New row/line
        // Column 0 honor IndentX
        window->DC.ColumnsOffset.x = ImMax(column_padding - window->WindowPadding.x, 0.0f);
        columns->Splitter.SetCurrentChannel(window->DrawList, 1);
        columns->Current = 0;
        columns->LineMinY = columns->LineMaxY;
    }
    window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
    window->DC.CursorPos.y = columns->LineMinY;
    window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
    window->DC.CurrLineTextBaseOffset = 0.0f;

    PushColumnClipRect(columns->Current);     // FIXME-COLUMNS: Could it be an overwrite?

    // FIXME-COLUMNS: Share code with BeginColumns() - move code on columns setup.
    float offset_0 = GetColumnOffset(columns->Current);
    float offset_1 = GetColumnOffset(columns->Current + 1);
    float width = offset_1 - offset_0;
    PushItemWidth(width * 0.65f);
    window->WorkRect.Max.x = window->Pos.x + offset_1 - column_padding;
}

void Legion::EndColumns()
{
    LegionContext& g = *GLegion;
    LegionWindow* window = GetCurrentWindow();
    LegionColumns* columns = window->DC.CurrentColumns;
    IM_ASSERT(columns != NULL);

    PopItemWidth();
    if (columns->Count > 1)
    {
        PopClipRect();
        columns->Splitter.Merge(window->DrawList);
    }

    const LegionColumnsFlags flags = columns->Flags;
    columns->LineMaxY = ImMax(columns->LineMaxY, window->DC.CursorPos.y);
    window->DC.CursorPos.y = columns->LineMaxY;
    if (!(flags & LegionColumnsFlags_GrowParentContentsSize))
        window->DC.CursorMaxPos.x = columns->HostCursorMaxPosX;  // Restore cursor max pos, as columns don't grow parent

    // Draw columns borders and handle resize
    // The IsBeingResized flag ensure we preserve pre-resize columns width so back-and-forth are not lossy
    bool is_being_resized = false;
    if (!(flags & LegionColumnsFlags_NoBorder) && !window->SkipItems)
    {
        // We clip Y boundaries CPU side because very long triangles are mishandled by some GPU drivers.
        const float y1 = ImMax(columns->HostCursorPosY, window->ClipRect.Min.y);
        const float y2 = ImMin(window->DC.CursorPos.y, window->ClipRect.Max.y);
        int dragging_column = -1;
        for (int n = 1; n < columns->Count; n++)
        {
            LegionColumnData* column = &columns->Columns[n];
            float x = window->Pos.x + GetColumnOffset(n);
            const LegionID column_id = columns->ID + LegionID(n);
            const float column_hit_hw = COLUMNS_HIT_RECT_HALF_WIDTH;
            const ImRect column_hit_rect(ImVec2(x - column_hit_hw, y1), ImVec2(x + column_hit_hw, y2));
            KeepAliveID(column_id);
            if (IsClippedEx(column_hit_rect, column_id, false))
                continue;

            bool hovered = false, held = false;
            if (!(flags & LegionColumnsFlags_NoResize))
            {
                ButtonBehavior(column_hit_rect, column_id, &hovered, &held);
                if (hovered || held)
                    g.MouseCursor = LegionMouseCursor_ResizeEW;
                if (held && !(column->Flags & LegionColumnsFlags_NoResize))
                    dragging_column = n;
            }

            // Draw column
            const ImU32 col = GetColorU32(held ? LegionCol_SeparatorActive : hovered ? LegionCol_SeparatorHovered : LegionCol_Separator);
            const float xi = IM_FLOOR(x);
            window->DrawList->AddLine(ImVec2(xi, y1 + 1.0f), ImVec2(xi, y2), col);
        }

        // Apply dragging after drawing the column lines, so our rendered lines are in sync with how items were displayed during the frame.
        if (dragging_column != -1)
        {
            if (!columns->IsBeingResized)
                for (int n = 0; n < columns->Count + 1; n++)
                    columns->Columns[n].OffsetNormBeforeResize = columns->Columns[n].OffsetNorm;
            columns->IsBeingResized = is_being_resized = true;
            float x = GetDraggedColumnOffset(columns, dragging_column);
            SetColumnOffset(dragging_column, x);
        }
    }
    columns->IsBeingResized = is_being_resized;

    window->WorkRect = columns->HostWorkRect;
    window->DC.CurrentColumns = NULL;
    window->DC.ColumnsOffset.x = 0.0f;
    window->DC.CursorPos.x = IM_FLOOR(window->Pos.x + window->DC.Indent.x + window->DC.ColumnsOffset.x);
}

// [2018-03: This is currently the only public API, while we are working on making BeginColumns/EndColumns user-facing]
void Legion::Columns(int columns_count, const char* id, bool border)
{
    LegionWindow* window = GetCurrentWindow();
    IM_ASSERT(columns_count >= 1);

    LegionColumnsFlags flags = (border ? 0 : LegionColumnsFlags_NoBorder);
    //flags |= LegionColumnsFlags_NoPreserveWidths; // NB: Legacy behavior
    LegionColumns* columns = window->DC.CurrentColumns;
    if (columns != NULL && columns->Count == columns_count && columns->Flags == flags)
        return;

    if (columns != NULL)
        EndColumns();

    if (columns_count != 1)
        BeginColumns(id, columns_count, flags);
}

//-------------------------------------------------------------------------

#endif // #ifndef Legion_DISABLE

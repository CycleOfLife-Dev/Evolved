// dear Legion, v1.76 WIP
// (headers)

// Help:
// - Read FAQ at http://dearLegion.org/faq
// - Newcomers, read 'Programmer guide' in Legion.cpp for notes on how to setup Dear Legion in your codebase.
// - Call and read Legion::ShowDemoWindow() in Legion_demo.cpp for demo code. All applications in examples/ are doing that.
// Read Legion.cpp for details, links and comments.

// Resources:
// - FAQ                   http://dearLegion.org/faq
// - Homepage & latest     https://github.com/ocornut/Legion
// - Releases & changelog  https://github.com/ocornut/Legion/releases
// - Gallery               https://github.com/ocornut/Legion/issues/3075 (please post your screenshots/video there!)
// - Glossary              https://github.com/ocornut/Legion/wiki/Glossary
// - Wiki                  https://github.com/ocornut/Legion/wiki
// - Issues & support      https://github.com/ocornut/Legion/issues

/*

Index of this file:
// Header mess
// Forward declarations and basic types
// Legion API (Dear Legion end-user API)
// Flags & Enumerations
// Memory allocations macros
// ImVector<>
// LegionStyle
// LegionIO
// Misc data structures (LegionInputTextCallbackData, LegionSizeCallbackData, LegionPayload)
// Obsolete functions
// Helpers (LegionOnceUponAFrame, LegionTextFilter, LegionTextBuffer, LegionStorage, LegionListClipper, ImColor)
// Draw List API (ImDrawCallback, ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawListFlags, ImDrawList, ImDrawData)
// Font API (ImFontConfig, ImFontGlyph, ImFontGlyphRangesBuilder, ImFontAtlasFlags, ImFontAtlas, ImFont)

*/

#pragma once

// Configuration file with compile-time options (edit imconfig.h or #define Legion_USER_CONFIG to your own filename)
#ifdef Legion_USER_CONFIG
#include Legion_USER_CONFIG
#endif


#ifndef Legion_DISABLE

//-----------------------------------------------------------------------------
// Header mess
//-----------------------------------------------------------------------------

// Includes
#include <float.h>                  // FLT_MIN, FLT_MAX
#include <stdarg.h>                 // va_list, va_start, va_end
#include <stddef.h>                 // ptrdiff_t, NULL
#include <string.h>                 // memset, memmove, memcpy, strlen, strchr, strcpy, strcmp

// Version
// (Integer encoded as XYYZZ for use in #if preprocessor conditionals. Work in progress versions typically starts at XYY99 then bounce up to XYY00, XYY01 etc. when release tagging happens)
#define Legion_VERSION               "1.76 WIP"
#define Legion_VERSION_NUM           17502
#define Legion_CHECKVERSION()        Legion::DebugCheckVersionAndDataLayout(Legion_VERSION, sizeof(LegionIO), sizeof(LegionStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))

// Define attributes of all API symbols declarations (e.g. for DLL under Windows)
// Legion_API is used for core Legion functions, Legion_IMPL_API is used for the default bindings files (Legion_impl_xxx.h)
// Using dear Legion via a shared library is not recommended, because we don't guarantee backward nor forward ABI compatibility (also function call overhead, as dear Legion is a call-heavy API)
#ifndef Legion_API
#define Legion_API
#endif
#ifndef Legion_IMPL_API
#define Legion_IMPL_API              Legion_API
#endif

// Helper Macros
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h
#endif
#if !defined(Legion_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1))) // To apply printf-style warnings to our functions.
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*_ARR)))       // Size of a static C-style array. Don't use on pointers!
#define IM_UNUSED(_VAR)             ((void)_VAR)                                // Used to silence "unused variable warnings". Often useful as asserts may be stripped out from final builds.
#if (__cplusplus >= 201100)
#define IM_OFFSETOF(_TYPE,_MEMBER)  offsetof(_TYPE, _MEMBER)                    // Offset of _MEMBER within _TYPE. Standardized as offsetof() in C++11
#else
#define IM_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))           // Offset of _MEMBER within _TYPE. Old style macro.
#endif

// Warnings
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wclass-memaccess"          // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

//-----------------------------------------------------------------------------
// Forward declarations and basic types
//-----------------------------------------------------------------------------

// Forward declarations
struct ImDrawChannel;               // Temporary storage to output draw commands out of order, used by ImDrawListSplitter and ImDrawList::ChannelsSplit()
struct ImDrawCmd;                   // A single draw command within a parent ImDrawList (generally maps to 1 GPU draw call, unless it is a callback)
struct ImDrawData;                  // All draw command lists required to render the frame + pos/size coordinates to use for the projection matrix.
struct ImDrawList;                  // A single draw command list (generally one per window, conceptually you may see this as a dynamic "mesh" builder)
struct ImDrawListSharedData;        // Data shared among multiple draw lists (typically owned by parent Legion context, but you may create one yourself)
struct ImDrawListSplitter;          // Helper to split a draw list into different layers which can be drawn into out of order, then flattened back.
struct ImDrawVert;                  // A single vertex (pos + uv + col = 20 bytes by default. Override layout with Legion_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct ImFont;                      // Runtime data for a single font within a parent ImFontAtlas
struct ImFontAtlas;                 // Runtime data for multiple fonts, bake multiple fonts into a single texture, TTF/OTF font loader
struct ImFontConfig;                // Configuration data when adding a font or merging fonts
struct ImFontGlyph;                 // A single font glyph (code point + coordinates within in ImFontAtlas + offset)
struct ImFontGlyphRangesBuilder;    // Helper to build glyph ranges from text/string data
struct ImColor;                     // Helper functions to create a color that can be converted to either u32 or float4 (*OBSOLETE* please avoid using)
struct LegionContext;                // Dear Legion context (opaque structure, unless including Legion_internal.h)
struct LegionIO;                     // Main configuration and I/O between your application and Legion
struct LegionInputTextCallbackData;  // Shared state of InputText() when using custom LegionInputTextCallback (rare/advanced use)
struct LegionListClipper;            // Helper to manually clip large list of items
struct LegionOnceUponAFrame;         // Helper for running a block of code not more than once a frame, used by Legion_ONCE_UPON_A_FRAME macro
struct LegionPayload;                // User data payload for drag and drop operations
struct LegionSizeCallbackData;       // Callback data when using SetNextWindowSizeConstraints() (rare/advanced use)
struct LegionStorage;                // Helper for key->value storage
struct LegionStyle;                  // Runtime data for styling/colors
struct LegionTextBuffer;             // Helper to hold and append into a text buffer (~string builder)
struct LegionTextFilter;             // Helper to parse and apply text filters (e.g. "aaaaa[,bbbbb][,ccccc]")

// Enums/Flags (declared as int for compatibility with old C++, to allow using as flags and to not pollute the top of this file)
// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
//   In Visual Studio IDE: CTRL+comma ("Edit.NavigateTo") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
//   With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.
typedef int LegionCol;               // -> enum LegionCol_             // Enum: A color identifier for styling
typedef int LegionCond;              // -> enum LegionCond_            // Enum: A condition for many Set*() functions
typedef int LegionDataType;          // -> enum LegionDataType_        // Enum: A primary data type
typedef int LegionDir;               // -> enum LegionDir_             // Enum: A cardinal direction
typedef int LegionKey;               // -> enum LegionKey_             // Enum: A key identifier (Legion-side enum)
typedef int LegionNavInput;          // -> enum LegionNavInput_        // Enum: An input identifier for navigation
typedef int LegionMouseButton;       // -> enum LegionMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
typedef int LegionMouseCursor;       // -> enum LegionMouseCursor_     // Enum: A mouse cursor identifier
typedef int LegionStyleVar;          // -> enum LegionStyleVar_        // Enum: A variable identifier for styling
typedef int ImDrawCornerFlags;      // -> enum ImDrawCornerFlags_    // Flags: for ImDrawList::AddRect(), AddRectFilled() etc.
typedef int ImDrawListFlags;        // -> enum ImDrawListFlags_      // Flags: for ImDrawList
typedef int ImFontAtlasFlags;       // -> enum ImFontAtlasFlags_     // Flags: for ImFontAtlas
typedef int LegionBackendFlags;      // -> enum LegionBackendFlags_    // Flags: for io.BackendFlags
typedef int LegionColorEditFlags;    // -> enum LegionColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
typedef int LegionConfigFlags;       // -> enum LegionConfigFlags_     // Flags: for io.ConfigFlags
typedef int LegionComboFlags;        // -> enum LegionComboFlags_      // Flags: for BeginCombo()
typedef int LegionDragDropFlags;     // -> enum LegionDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
typedef int LegionFocusedFlags;      // -> enum LegionFocusedFlags_    // Flags: for IsWindowFocused()
typedef int LegionHoveredFlags;      // -> enum LegionHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
typedef int LegionInputTextFlags;    // -> enum LegionInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
typedef int LegionKeyModFlags;       // -> enum LegionKeyModFlags_     // Flags: for io.KeyMods (Ctrl/Shift/Alt/Super)
typedef int LegionSelectableFlags;   // -> enum LegionSelectableFlags_ // Flags: for Selectable()
typedef int LegionTabBarFlags;       // -> enum LegionTabBarFlags_     // Flags: for BeginTabBar()
typedef int LegionTabItemFlags;      // -> enum LegionTabItemFlags_    // Flags: for BeginTabItem()
typedef int LegionTreeNodeFlags;     // -> enum LegionTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int LegionWindowFlags;       // -> enum LegionWindowFlags_     // Flags: for Begin(), BeginChild()

// Other types
#ifndef ImTextureID                 // ImTextureID [configurable type: override in imconfig.h with '#define ImTextureID xxx']
typedef void* ImTextureID;          // User data for rendering back-end to identify a texture. This is whatever to you want it to be! read the FAQ about ImTextureID for details.
#endif
typedef unsigned int LegionID;       // A unique ID used by widgets, typically hashed from a stack of string.
typedef int (*LegionInputTextCallback)(LegionInputTextCallbackData *data);
typedef void (*LegionSizeCallback)(LegionSizeCallbackData* data);

// Decoded character types
// (we generally use UTF-8 encoded string in the API. This is storage specifically for a decoded character used for keyboard input and display)
typedef unsigned short ImWchar16;   // A single decoded U16 character/code point. We encode them as multi bytes UTF-8 when used in strings.
typedef unsigned int ImWchar32;     // A single decoded U32 character/code point. We encode them as multi bytes UTF-8 when used in strings.
#ifdef Legion_USE_WCHAR32            // ImWchar [configurable type: override in imconfig.h with '#define Legion_USE_WCHAR32' to support Unicode planes 1-16]
typedef ImWchar32 ImWchar;
#else
typedef ImWchar16 ImWchar;
#endif

// Basic scalar data types
typedef signed char         ImS8;   // 8-bit signed integer
typedef unsigned char       ImU8;   // 8-bit unsigned integer
typedef signed short        ImS16;  // 16-bit signed integer
typedef unsigned short      ImU16;  // 16-bit unsigned integer
typedef signed int          ImS32;  // 32-bit signed integer == int
typedef unsigned int        ImU32;  // 32-bit unsigned integer (often used to store packed colors)
#if defined(_MSC_VER) && !defined(__clang__)
typedef signed   __int64    ImS64;  // 64-bit signed integer (pre and post C++11 with Visual Studio)
typedef unsigned __int64    ImU64;  // 64-bit unsigned integer (pre and post C++11 with Visual Studio)
#elif (defined(__clang__) || defined(__GNUC__)) && (__cplusplus < 201100)
#include <stdint.h>
typedef int64_t             ImS64;  // 64-bit signed integer (pre C++11)
typedef uint64_t            ImU64;  // 64-bit unsigned integer (pre C++11)
#else
typedef signed   long long  ImS64;  // 64-bit signed integer (post C++11)
typedef unsigned long long  ImU64;  // 64-bit unsigned integer (post C++11)
#endif

// 2D vector (often used to store positions or sizes)
struct ImVec2
{
    float                                   x, y;
    ImVec2()                                { x = y = 0.0f; }
    ImVec2(float _x, float _y)              { x = _x; y = _y; }
    float  operator[] (size_t idx) const    { IM_ASSERT(idx <= 1); return (&x)[idx]; }    // We very rarely use this [] operator, the assert overhead is fine.
    float& operator[] (size_t idx)          { IM_ASSERT(idx <= 1); return (&x)[idx]; }    // We very rarely use this [] operator, the assert overhead is fine.
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec2.
#endif
};

// 4D vector (often used to store floating-point colors)
struct ImVec4
{
    float                                   x, y, z, w;
    ImVec4()                                { x = y = z = w = 0.0f; }
    ImVec4(float _x, float _y, float _z, float _w)  { x = _x; y = _y; z = _z; w = _w; }
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA     // Define additional constructors and implicit cast operators in imconfig.h to convert back and forth between your math types and ImVec4.
#endif
};

//-----------------------------------------------------------------------------
// Legion: Dear Legion end-user API
// (This is a namespace. You can add extra Legion:: functions in your own separate file. Please don't modify Legion source files!)
//-----------------------------------------------------------------------------

namespace Legion
{
    // Context creation and access
    // Each context create its own ImFontAtlas by default. You may instance one yourself and pass it to CreateContext() to share a font atlas between Legion contexts.
    // None of those functions is reliant on the current context.
    Legion_API LegionContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL);
    Legion_API void          DestroyContext(LegionContext* ctx = NULL);   // NULL = destroy current context
    Legion_API LegionContext* GetCurrentContext();
    Legion_API void          SetCurrentContext(LegionContext* ctx);

    // Main
    Legion_API LegionIO&      GetIO();                                    // access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)
    Legion_API LegionStyle&   GetStyle();                                 // access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame!
    Legion_API void          NewFrame();                                 // start a new Dear Legion frame, you can submit any command from this point until Render()/EndFrame().
    Legion_API void          EndFrame();                                 // ends the Dear Legion frame. automatically called by Render(). If you don't need to render data (skipping rendering) you may call EndFrame() without Render()... but you'll have wasted CPU already! If you don't need to render, better to not create any windows and not call NewFrame() at all!
    Legion_API void          Render();                                   // ends the Dear Legion frame, finalize the draw data. You can get call GetDrawData() to obtain it and run your rendering function (up to v1.60, this used to call io.RenderDrawListsFn(). Nowadays, we allow and prefer calling your render function yourself.)
    Legion_API ImDrawData*   GetDrawData();                              // valid after Render() and until the next call to NewFrame(). this is what you have to render.

    // Demo, Debug, Information
    Legion_API void          ShowDemoWindow(bool* p_open = NULL);        // create Demo window (previously called ShowTestWindow). demonstrate most Legion features. call this to learn about the library! try to make it always available in your application!
    Legion_API void          ShowAboutWindow(bool* p_open = NULL);       // create About window. display Dear Legion version, credits and build/system information.
    Legion_API void          ShowMetricsWindow(bool* p_open = NULL);     // create Debug/Metrics window. display Dear Legion internals: draw commands (with individual draw calls and vertices), window list, basic internal state, etc.
    Legion_API void          ShowStyleEditor(LegionStyle* ref = NULL);    // add style editor block (not a window). you can pass in a reference LegionStyle structure to compare to, revert to and save to (else it uses the default style)
    Legion_API bool          ShowStyleSelector(const char* label);       // add style selector block (not a window), essentially a combo listing the default styles.
    Legion_API void          ShowFontSelector(const char* label);        // add font selector block (not a window), essentially a combo listing the loaded fonts.
    Legion_API void          ShowUserGuide();                            // add basic help/info block (not a window): how to manipulate Legion as a end-user (mouse/keyboard controls).
    Legion_API const char*   GetVersion();                               // get the compiled version string e.g. "1.23" (essentially the compiled value for Legion_VERSION)
   
    // Styles
    Legion_API void          StyleColorsDark(LegionStyle* dst = NULL);    // new, recommended style (default)
    Legion_API void          StyleColorsClassic(LegionStyle* dst = NULL); // classic Legion style
    Legion_API void          StyleColorsLight(LegionStyle* dst = NULL);   // best used with borders and a custom, thicker font

    // Windows
    // - Begin() = push window to the stack and start appending to it. End() = pop window from the stack.
    // - You may append multiple times to the same window during the same frame.
    // - Passing 'bool* p_open != NULL' shows a window-closing widget in the upper-right corner of the window,
    //   which clicking will set the boolean to false when clicked.
    // - Begin() return false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
    //   anything to the window. Always call a matching End() for each Begin() call, regardless of its return value!
    //   [Important: due to legacy reason, this is inconsistent with most other functions such as BeginMenu/EndMenu,
    //    BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function
    //    returned true. Begin and BeginChild are the only odd ones out. Will be fixed in a future update.]
    // - Note that the bottom of window stack always contains a window called "Debug".
    Legion_API bool          Begin(const char* name, bool* p_open = NULL, LegionWindowFlags flags = 0);
    Legion_API bool          BeginEx(ImTextureID user_texture_id, ImTextureID scrollbg, ImTextureID scrollbar, const char* name, bool* p_open = NULL, LegionWindowFlags flags = 0, bool LegionAxis_Y = true);
    Legion_API bool          BeginEx(ImTextureID user_texture_id, ImTextureID scrollbg, ImTextureID scrollbar, const char* name, const ImVec2& size_first_use, const ImVec2& pos = ImVec2(0, 0), bool* p_open = NULL, LegionWindowFlags flags = 0, bool LegionAxis_Y = true);
    Legion_API void          End();

    // Child Windows
    // - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
    // - For each independent axis of 'size': ==0.0f: use remaining host window size / >0.0f: fixed size / <0.0f: use remaining window size minus abs(size) / Each axis can use a different mode, e.g. ImVec2(0,400).
    // - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting anything to the window.
    //   Always call a matching EndChild() for each BeginChild() call, regardless of its return value [as with Begin: this is due to legacy reason and inconsistent with most BeginXXX functions apart from the regular Begin() which behaves like BeginChild().]
    Legion_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0,0), bool border = false, LegionWindowFlags flags = 0);
    Legion_API bool          BeginChild(LegionID id, const ImVec2& size = ImVec2(0, 0), bool border = false, LegionWindowFlags flags = 0);
    Legion_API bool          BeginChildV(ImTextureID texture, ImTextureID scrollbg, ImTextureID scrollbar, const char* str_id, const ImVec2& size_arg, bool border = false, LegionWindowFlags extra_flags = 0, bool LegionAxis_Y = true);
    Legion_API void          EndChild();

    // Windows Utilities
    // - 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window we will Begin() into.
    Legion_API bool          IsWindowAppearing();
    Legion_API bool          IsWindowCollapsed();
    Legion_API bool          IsWindowFocused(LegionFocusedFlags flags=0); // is current window focused? or its root/child, depending on flags. see flags for options.
    Legion_API bool          IsWindowHovered(LegionHoveredFlags flags=0); // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to Legion or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
    Legion_API ImDrawList*   GetWindowDrawList();                        // get draw list associated to the current window, to append your own drawing primitives
    Legion_API ImVec2        GetWindowPos();                             // get current window position in screen space (useful if you want to do your own drawing via the DrawList API)
    Legion_API ImVec2        GetWindowSize();                            // get current window size
    Legion_API float         GetWindowWidth();                           // get current window width (shortcut for GetWindowSize().x)
    Legion_API float         GetWindowHeight();                          // get current window height (shortcut for GetWindowSize().y)

    // Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
    Legion_API void          SetNextWindowPos(const ImVec2& pos, LegionCond cond = 0, const ImVec2& pivot = ImVec2(0,0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
    Legion_API void          SetNextWindowSize(const ImVec2& size, LegionCond cond = 0);                  // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
    Legion_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, LegionSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); // set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. Use callback to apply non-trivial programmatic constraints.
    Legion_API void          SetNextWindowContentSize(const ImVec2& size);                               // set next window content size (~ scrollable client area, which enforce the range of scrollbars). Not including window decorations (title bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0f to leave it automatic. call before Begin()
    Legion_API void          SetNextWindowCollapsed(bool collapsed, LegionCond cond = 0);                 // set next window collapsed state. call before Begin()
    Legion_API void          SetNextWindowFocus();                                                       // set next window to be focused / top-most. call before Begin()
    Legion_API void          SetNextWindowBgAlpha(float alpha);                                          // set next window background color alpha. helper to easily override the Alpha component of LegionCol_WindowBg/ChildBg/PopupBg. you may also use LegionWindowFlags_NoBackground.
    Legion_API void          SetWindowPos(const ImVec2& pos, LegionCond cond = 0);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
    Legion_API void          SetWindowSize(const ImVec2& size, LegionCond cond = 0);                      // (not recommended) set current window size - call within Begin()/End(). set to ImVec2(0,0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
    Legion_API void          SetWindowCollapsed(bool collapsed, LegionCond cond = 0);                     // (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
    Legion_API void          SetWindowFocus();                                                           // (not recommended) set current window to be focused / top-most. prefer using SetNextWindowFocus().
    Legion_API void          SetWindowFontScale(float scale);                                            // set font scale. Adjust IO.FontGlobalScale if you want to scale all windows. This is an old API! For correct scaling, prefer to reload font + rebuild ImFontAtlas + call style.ScaleAllSizes().
    Legion_API void          SetWindowPos(const char* name, const ImVec2& pos, LegionCond cond = 0);      // set named window position.
    Legion_API void          SetWindowSize(const char* name, const ImVec2& size, LegionCond cond = 0);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
    Legion_API void          SetWindowCollapsed(const char* name, bool collapsed, LegionCond cond = 0);   // set named window collapsed state
    Legion_API void          SetWindowFocus(const char* name);                                           // set named window to be focused / top-most. use NULL to remove focus.

    // Content region
    // - Those functions are bound to be redesigned soon (they are confusing, incomplete and return values in local window coordinates which increases confusion)
    Legion_API ImVec2        GetContentRegionMax();                                          // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
    Legion_API ImVec2        GetContentRegionAvail();                                        // == GetContentRegionMax() - GetCursorPos()
    Legion_API ImVec2        GetWindowContentRegionMin();                                    // content boundaries min (roughly (0,0)-Scroll), in window coordinates
    Legion_API ImVec2        GetWindowContentRegionMax();                                    // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
    Legion_API float         GetWindowContentRegionWidth();                                  //

    // Windows Scrolling
    Legion_API float         GetScrollX();                                                   // get scrolling amount [0..GetScrollMaxX()]
    Legion_API float         GetScrollY();                                                   // get scrolling amount [0..GetScrollMaxY()]
    Legion_API float         GetScrollY(const char* name);                                                   // get scrolling amount [0..GetScrollMaxY()]
    Legion_API float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
    Legion_API float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
    Legion_API float         GetScrollMaxY(const char* name);                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
    Legion_API void          SetScrollX(float scroll_x);                                     // set scrolling amount [0..GetScrollMaxX()]
    Legion_API void          SetScrollY(float scroll_y);                                     // set scrolling amount [0..GetScrollMaxY()]
    Legion_API void          SetScrollHereX(float center_x_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    Legion_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    Legion_API void          SetScrollHereVY(float center_y_ratio, const char* winname);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
    Legion_API void          SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
    Legion_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.
    Legion_API void          SetScrollFromPosExY(float local_y, float center_y_ratio, const char* name);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.

    // Parameters stacks (shared)
    Legion_API void          PushFont(ImFont* font);                                         // use NULL as a shortcut to push default font
    Legion_API void          PopFont();
    Legion_API void          PushStyleColor(LegionCol idx, ImU32 col);
    Legion_API void          PushStyleColor(LegionCol idx, const ImVec4& col);
    Legion_API void          PopStyleColor(int count = 1);
    Legion_API void          PushStyleVar(LegionStyleVar idx, float val);
    Legion_API void          PushStyleVar(LegionStyleVar idx, const ImVec2& val);
    Legion_API void          PopStyleVar(int count = 1);
    Legion_API const ImVec4& GetStyleColorVec4(LegionCol idx);                                // retrieve style color as stored in LegionStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.
    Legion_API ImFont*       GetFont();                                                      // get current font
    Legion_API float         GetFontSize();                                                  // get current font size (= height in pixels) of current font with current scale applied
    Legion_API ImVec2        GetFontTexUvWhitePixel();                                       // get UV coordinate for a while pixel, useful to draw custom shapes via the ImDrawList API
    Legion_API ImU32         GetColorU32(LegionCol idx, float alpha_mul = 1.0f);              // retrieve given style color with style alpha applied and optional extra alpha multiplier
    Legion_API ImU32         GetColorU32(const ImVec4& col);                                 // retrieve given color with style alpha applied
    Legion_API ImU32         GetColorU32(ImU32 col);                                         // retrieve given color with style alpha applied

    // Parameters stacks (current window)
    Legion_API void          PushItemWidth(float item_width);                                // push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side). 0.0f = default to ~2/3 of windows width,
    Legion_API void          PopItemWidth();
    Legion_API void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
    Legion_API float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
    Legion_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // push word-wrapping position for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
    Legion_API void          PopTextWrapPos();
    Legion_API void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
    Legion_API void          PopAllowKeyboardFocus();
    Legion_API void          PushButtonRepeat(bool repeat);                                  // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
    Legion_API void          PopButtonRepeat();

    // Cursor / Layout
    // - By "cursor" we mean the current output position.
    // - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
    // - You can call SameLine() between widgets to undo the last carriage return and output at the right of the preceeding widget.
    // - Attention! We currently have inconsistencies between window-local and absolute positions we will aim to fix with future API:
    //    Window-local coordinates:   SameLine(), GetCursorPos(), SetCursorPos(), GetCursorStartPos(), GetContentRegionMax(), GetWindowContentRegion*(), PushTextWrapPos()
    //    Absolute coordinate:        GetCursorScreenPos(), SetCursorScreenPos(), all ImDrawList:: functions.
    Legion_API void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
    Legion_API void          SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
    Legion_API void          SameLineEx(float offset_from_start_x = 0.0f, float offset_from_start_y = 0.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
    Legion_API void          NewLine();                                                      // undo a SameLine() or force a new line when in an horizontal-layout context.
    Legion_API void          Spacing();                                                      // add vertical spacing.
    Legion_API void          Dummy(const ImVec2& size);                                      // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
    Legion_API void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
    Legion_API void          IndentV(float x = 0.0f, float y = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
    Legion_API void          IndentEx(float x = 0.0f, float y = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
    Legion_API void          IndentExV(float x = 0.0f, float y = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
    Legion_API void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by style.IndentSpacing or indent_w if != 0
    Legion_API void          BeginGroup();                                                   // lock horizontal starting position
    Legion_API void          EndGroup();                                                     // unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
    Legion_API ImVec2        GetCursorPos();                                                 // cursor position in window coordinates (relative to window position)
    Legion_API float         GetCursorPosX();                                                //   (some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
    Legion_API float         GetCursorPosY();                                                //    other functions such as GetCursorScreenPos or everything in ImDrawList::
    Legion_API void          SetCursorPos(const ImVec2& local_pos);                          //    are using the main, absolute coordinate system.
    Legion_API void          SetCursorPosX(float local_x);                                   //    GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.)
    Legion_API void          SetCursorPosY(float local_y);                                   //
    Legion_API ImVec2        GetCursorStartPos();                                            // initial cursor position in window coordinates
    Legion_API ImVec2        GetCursorScreenPos();                                           // cursor position in absolute screen coordinates [0..io.DisplaySize] (useful to work with ImDrawList API)
    Legion_API void          SetCursorScreenPos(const ImVec2& pos);                          // cursor position in absolute screen coordinates [0..io.DisplaySize]
    Legion_API void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
    Legion_API float         GetTextLineHeight();                                            // ~ FontSize
    Legion_API float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
    Legion_API float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
    Legion_API float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

    // ID stack/scopes
    // - Read the FAQ for more details about how ID are handled in dear Legion. If you are creating widgets in a loop you most
    //   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
    // - The resulting ID are hashes of the entire stack.
    // - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
    // - In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
    //   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
    Legion_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
    Legion_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
    Legion_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
    Legion_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
    Legion_API void          PopID();                                                        // pop from the ID stack.
    Legion_API LegionID       GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into LegionStorage yourself
    Legion_API LegionID       GetID(const char* str_id_begin, const char* str_id_end);
    Legion_API LegionID       GetID(const void* ptr_id);

    // Widgets: Text
    Legion_API void          TextUnformatted(const char* text, const char* text_end = NULL); // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
    Legion_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); // formatted text
    Legion_API void          TextV(const char* fmt, va_list args)                            IM_FMTLIST(1);
    Legion_API void          TextExV(ImFont* font, float font_size, const char* fmt, va_list args)                            IM_FMTLIST(1);
    Legion_API void          TextColored(const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut for PushStyleColor(LegionCol_Text, col); Text(fmt, ...); PopStyleColor();
    Legion_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
    Legion_API void          TextColored(ImFont* font, float font_size, const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut for PushStyleColor(LegionCol_Text, col); Text(fmt, ...); PopStyleColor();
    Legion_API void          TextColoredV(ImFont* font, float font_size, const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
    Legion_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); // shortcut for PushStyleColor(LegionCol_Text, style.Colors[LegionCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
    Legion_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
    Legion_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
    Legion_API void          TextWrappedEx(ImFont* font, float size, const char* fmt, ...)   IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
    Legion_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
    Legion_API void          TextWrappedExV(ImFont* font, float size, const char* fmt, va_list args)                    IM_FMTLIST(1);
    Legion_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); // display text+label aligned the same way as value+label widgets
    Legion_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
    Legion_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); // shortcut for Bullet()+Text()
    Legion_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);

    // Widgets: Main
    // - Most widgets return true when the value has been changed or when pressed/selected
    // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive, IsItemHovered, etc.) to query widget state.
    Legion_API bool          Button(const char* label, const ImVec2& size = ImVec2(0,0));    // button
    Legion_API bool          SmallButton(const char* label);                                 // button with FramePadding=(0,0) to easily embed within text
    Legion_API bool          InvisibleButton(const char* str_id, const ImVec2& size);        // button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
    Legion_API bool          ArrowButton(const char* str_id, LegionDir dir);                  // square button with an arrow shape
    Legion_API bool          ImageExTip(const char* cid, ImFont* font, float font_size, const char* tip, ImTextureID normal, ImTextureID user_textrue_id, const ImVec2& size);
    Legion_API bool          ImageExTipV(const char* cid, ImFont* font, float font_size, const char* tip, ImTextureID normal, ImTextureID image, ImTextureID user_textrue_id, const ImVec2& size);
    Legion_API bool          ImageEx(const char* cid, ImFont* font, float font_size, const char* label, const char* tip, bool istip, ImTextureID normal, ImTextureID user_textrue_id, const ImVec2& size);
    Legion_API void          ImageV(const char* cid, ImTextureID textrue, const ImVec2& size);
    Legion_API void          ImageExV(const char* cid, ImTextureID textrue, ImTextureID img, const ImVec2& size);
    Legion_API void          ImageExShop(const char* cid, ImTextureID textrue, ImTextureID img, const ImVec2& size);
    Legion_API bool          ImageExS(const char* cid, ImFont* font, float font_size, const char* label, ImTextureID normal, ImTextureID image, const ImVec2& size);
    Legion_API bool          ImageCheckBoxV(const char* cid, const char* combid, const char* combid2, const char* preview_value, ImTextureID textrue, ImTextureID img, ImTextureID checkbackground, ImTextureID checkedbox, ImTextureID combobox, ImTextureID Arrow, ImTextureID ListBackGround, ImTextureID LeftArrow, ImTextureID RightArrow, const ImVec2& size, const ImVec2& Imsize, const ImVec2& cosize, const ImVec2& checksize, ImFont* font, float font_size, const char* label, bool* v, bool isreadonly, LegionComboFlags flags = 0);
    Legion_API void          ImageText(ImFont* font, float font_size, const char* label, const char* cid, ImTextureID normal, const ImVec2& size);
    Legion_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0,0), const ImVec2& uv1 = ImVec2(1,1), const ImVec4& tint_col = ImVec4(1,1,1,1), const ImVec4& border_col = ImVec4(0,0,0,0));
    Legion_API bool          CircleButton(const char* cId, const char* label, bool islabel, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);   // <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          BattleFieldEx(const char* cid, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID Claimed, bool IsClaimed, bool Selected, bool IsButtonActive, const ImVec2& size);
    Legion_API bool          BattleField(const char* cid, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID Claimed, bool IsClaimed, int* v, int v_button, bool IsButtonActive, const ImVec2& size);
    Legion_API bool          BattleFieldTaskEx(const char* cid, const char* label, const char* count, const char* mission, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID pointimage, ImTextureID Claimed, bool IsClaimed, bool Selected, const ImVec2& size);
    Legion_API bool          BattleFieldTask(const char* cid, const char* label, const char* count, const char* mission, ImFont* font, float font_size, ImTextureID normal, ImTextureID image, ImTextureID pointimage, ImTextureID Claimed, bool IsClaimed, int* v, int v_button, const ImVec2& size);
    Legion_API bool          AchivementsEx(const char* cid, ImFont* font, float font_size, ImTextureID background, ImTextureID achivimage, ImTextureID Claimed, float v, bool IsClaimed, bool Selected);
    Legion_API bool          Achivements(const char* cid, ImFont* font, float font_size, ImTextureID background, ImTextureID achivimage, ImTextureID Claimed, float val, bool IsClaimed, int* v, int v_button);
    Legion_API bool          ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));    // <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonEx(const char* cId, const char* label, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);    // <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonEx(const char* cId, const char* label, bool islabel, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);    // <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExM(const char* cId, const char* label, const char* description, const char* hoverlable, const char* counter, bool islabel, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font, float font_size, float font_size2, const ImVec2& align, const ImVec2& align2, const ImVec2& align3, const ImVec2& align4, int frame_padding = -1);
    Legion_API bool          ImageButtonV(const char* cId, const char* label, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f));// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExV(const char* cId, const char* label, bool islabel, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f));// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, int* v, int v_button, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f));// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExV(const char* cId, const char* label, bool islabel, bool Selected, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonV(const char* cId, const char* label, bool islabel, int* v, int v_button, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f));// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExVC(const char* cId, const char* label, const char* normalcolor, const char* hovercolor, const char* pressedcolor, bool islabel, bool Selected, ImColor normal, ImColor hover, ImColor press, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonT(const char* cId, const char* label, const char* tooltip, bool islabel, int* v, int v_button, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f));// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          ImageButtonExT(const char* cId, const char* label, bool islabel, const char* tooltip, bool Selected, ImTextureID normal, ImTextureID hover, ImTextureID press, ImTextureID tiptexture, const ImVec2& size, ImFont* font = NULL, float font_size = 0.0f, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API bool          Checkbox(const char* label, bool* v);
    Legion_API bool          CheckboxEx(const char* cid, ImTextureID background, ImTextureID checkedbox, ImFont* font, float font_size, const char* label, bool* v, bool isreadonly = false);
    Legion_API bool          CheckboxExS(const char* cid, ImTextureID background, ImTextureID checkedbox, bool* v);
    Legion_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    Legion_API bool          RadioButtonEx(const char* cid, ImTextureID user_texture_id, ImVec2 size, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    Legion_API bool          RadioButton(const char* label, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
    Legion_API bool          RadioButtonEx(const char* cid, ImTextureID user_texture_id, ImVec2 size, int* v, int v_button);           // shortcut to handle the above pattern when value is an integer
    Legion_API bool          RadioButton(const char* label, int* v, int v_button);           // shortcut to handle the above pattern when value is an integer
    Legion_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-1, 0), const char* overlay = NULL);
    Legion_API void          ProgressBarV(float fraction, ImVec4 color, ImVec4 acolor, const ImVec2& size_arg = ImVec2(-1, 0), const char* overlay = NULL);
    Legion_API void          LabelEx(const char* cId, const char* label, const ImVec2& size, ImFont* font, float font_size, const ImVec2& align = ImVec2(0.5f, 0.5f), int frame_padding = -1);// <0 frame_padding uses default frame padding settings. 0 for no padding
    Legion_API void          Bullet();                                                       // draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses
    Legion_API bool          ShopSearchControl(const char* CheckBoxId, const char* ComboboxId, const char* preview_value, ImTextureID ItemBG, ImTextureID Item, ImTextureID CheckBoxBG, ImTextureID CheckBoxActiveBG, ImTextureID ComboBoxBG, ImTextureID ComboBoxArrow, ImTextureID ComboBoxListBG, ImTextureID ColorLeftArrow, ImTextureID ColorRightArrow, const ImVec2& FrameSize, const ImVec2& ItemBGSize, const ImVec2& CheckBoxSize, const ImVec2& ComboBoxSize, ImFont* font, float font_size, const char* label, bool* CheckBoxPressed, bool* LeftArrowPressed, bool* RightArrowPressed, LegionComboFlags flags = 0);

    // Widgets: Combo Box
    // - The BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
    // - The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose.
    Legion_API bool          BeginCombo(const char* label, const char* preview_value, LegionComboFlags flags = 0);
    Legion_API bool          BeginComboV(ImFont* font, float font_size, const char* label, const char* preview_value, const ImVec2& size_arge = ImVec2(0, 0), ImTextureID BackGround = NULL, ImTextureID ComboBackGround = NULL, ImTextureID Arrows = NULL, ImTextureID Scroll = NULL, ImTextureID ScrollBar = NULL, LegionComboFlags flags = 0);
    Legion_API bool          BeginComboEx(ImFont* font, float font_size, const char* label, const char* preview_value, const ImVec2& size_arge = ImVec2(0, 0), float listsize = 1.4f, ImTextureID BackGround = NULL, ImTextureID ComboBackGround = NULL, ImTextureID Arrows = NULL, ImTextureID Scroll = NULL, ImTextureID ScrollBar = NULL, LegionComboFlags flags = 0);
    Legion_API void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
    Legion_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    Legion_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    Legion_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    // Widgets: Drags
    // - CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
    // - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
    // - Use v_min < v_max to clamp edits to given limits. Note that CTRL+Click manual input can override those limits.
    // - Use v_max = FLT_MAX / INT_MAX etc to avoid clamping to a maximum, same with v_min = -FLT_MAX / INT_MIN to avoid clamping to a minimum.
    // - Use v_min > v_max to lock edits.
    Legion_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);     // If v_min >= v_max we have no bound
    Legion_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, float power = 1.0f);
    Legion_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");                                       // If v_min >= v_max we have no bound
    Legion_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    Legion_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    Legion_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
    Legion_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL);
    Legion_API bool          DragScalar(const char* label, LegionDataType data_type, void* p_data, float v_speed, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, float power = 1.0f);
    Legion_API bool          DragScalarN(const char* label, LegionDataType data_type, void* p_data, int components, float v_speed, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, float power = 1.0f);

    // Widgets: Sliders
    // - CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
    // - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    Legion_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
    Legion_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg");
    Legion_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
    Legion_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d");
    Legion_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d");
    Legion_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d");
    Legion_API bool          SliderScalar(const char* label, LegionDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);
    Legion_API bool          SliderScalarN(const char* label, LegionDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);
    Legion_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
    Legion_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d");
    Legion_API bool          VSliderScalar(const char* label, const ImVec2& size, LegionDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, float power = 1.0f);
    Legion_API bool          SliderFloatEx(ImFont* font, float font_size, const char* label, float* v, float v_min, float v_max, const char* display_format, float power, ImVec2 size, ImColor color, bool islabel = false, float sizes = 0, bool isread = false);
    Legion_API bool          ProgressBarEx(ImFont* font, float font_size, const char* label, float v, float v_min, float v_max, float power, ImVec2 size, ImColor color, ImColor valuecolor, bool islabel, bool IsValue);

    // Widgets: Input with Keyboard
    // - If you want to use InputText() with std::string or any custom dynamic string type, see misc/cpp/Legion_stdlib.h and comments in Legion_demo.cpp.
    // - Most of the LegionInputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
    Legion_API bool          InputTextV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, char* buf, size_t buf_size, const ImVec2& size_arg, LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API bool          InputText(const char* label, char* buf, size_t buf_size, LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0,0), LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API bool          InputTextWithHintV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, size_t buf_size, const ImVec2& size_arg, LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API int           InputTextWithHintExV(const char* cid, ImFont* font, float font_size, ImTextureID texture, const char* label, const char* hint, char* buf, size_t buf_size, const ImVec2& size_arg, ImTextureID btntexture, const char* btnlabel, const ImVec2& btnsize_arg, LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, LegionInputTextFlags flags = 0, LegionInputTextCallback callback = NULL, void* user_data = NULL);
    Legion_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", LegionInputTextFlags flags = 0);
    Legion_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", LegionInputTextFlags flags = 0);
    Legion_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", LegionInputTextFlags flags = 0);
    Legion_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", LegionInputTextFlags flags = 0);
    Legion_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, LegionInputTextFlags flags = 0);
    Legion_API bool          InputInt2(const char* label, int v[2], LegionInputTextFlags flags = 0);
    Legion_API bool          InputInt3(const char* label, int v[3], LegionInputTextFlags flags = 0);
    Legion_API bool          InputInt4(const char* label, int v[4], LegionInputTextFlags flags = 0);
    Legion_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", LegionInputTextFlags flags = 0);
    Legion_API bool          InputScalar(const char* label, LegionDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, LegionInputTextFlags flags = 0);
    Legion_API bool          InputScalarN(const char* label, LegionDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, LegionInputTextFlags flags = 0);

    // Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little colored preview square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
    // - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
    // - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
    Legion_API bool          ColorEdit3(const char* label, float col[3], LegionColorEditFlags flags = 0);
    Legion_API bool          ColorEdit4(const char* label, float col[4], LegionColorEditFlags flags = 0);
    Legion_API bool          ColorPicker3(const char* label, float col[3], LegionColorEditFlags flags = 0);
    Legion_API bool          ColorPicker4(const char* label, float col[4], LegionColorEditFlags flags = 0, const float* ref_col = NULL);
    Legion_API bool          ColorButton(const char* desc_id, const ImVec4& col, LegionColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0));  // display a colored square/button, hover for details, return true when pressed.
    Legion_API void          SetColorEditOptions(LegionColorEditFlags flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

    // Widgets: Trees
    // - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
    Legion_API bool          TreeNode(const char* label);
    Legion_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
    Legion_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   // "
    Legion_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
    Legion_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
    Legion_API bool          TreeNodeEx(const char* label, LegionTreeNodeFlags flags = 0);
    Legion_API bool          TreeNodeEx(const char* str_id, LegionTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    Legion_API bool          TreeNodeEx(const void* ptr_id, LegionTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    Legion_API bool          TreeNodeExV(const char* str_id, LegionTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    Legion_API bool          TreeNodeExV(const void* ptr_id, LegionTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    Legion_API void          TreePush(const char* str_id);                                       // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
    Legion_API void          TreePush(const void* ptr_id = NULL);                                // "
    Legion_API void          TreePop();                                                          // ~ Unindent()+PopId()
    Legion_API float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
    Legion_API bool          CollapsingHeader(const char* label, LegionTreeNodeFlags flags = 0);  // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
    Legion_API bool          CollapsingHeader(const char* label, bool* p_open, LegionTreeNodeFlags flags = 0); // when 'p_open' isn't NULL, display an additional small close button on upper right of the header
    Legion_API void          SetNextItemOpen(bool is_open, LegionCond cond = 0);                  // set next TreeNode/CollapsingHeader open state.

    // Widgets: Selectables
    // - A selectable highlights when hovered, and can display another color when selected.
    // - Neighbors selectable extend their highlight bounds in order to leave no gap between them. This is so a series of selected Selectable appear contiguous.
    Legion_API bool          Selectable(const char* label, bool selected = false, LegionSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    Legion_API bool          SelectableExV(const char* cid, ImFont* font, float fsize, const char* label, bool selected = false, LegionSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    Legion_API bool          SelectableEx(const char* cid, ImFont* font, float fsize, const char* label, bool selected = false, LegionSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    Legion_API bool          Selectable(const char* label, bool* p_selected, LegionSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.
    Legion_API bool          SelectableV(ImFont* font, float fsize, const char* time, const char* nickname, const char* message/*, ImTextureID scroll, ImTextureID scrollbar*/, const ImVec2& size_arg);       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

    // Widgets: List Boxes
    // - FIXME: To be consistent with all the newer API, ListBoxHeader/ListBoxFooter should in reality be called BeginListBox/EndListBox. Will rename them.
    Legion_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    Legion_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
    Legion_API bool          ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0,0)); // use if you want to reimplement ListBox() will custom data or interactions. if the function return true, you can output elements then call ListBoxFooter() afterwards.
    Legion_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1); // "
    Legion_API void          ListBoxFooter();                                                    // terminate the scrolling region. only call ListBoxFooter() if ListBoxHeader() returned true!

    // Widgets: Data Plotting
    Legion_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    Legion_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
    Legion_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    Legion_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

    // Widgets: Value() Helpers.
    // - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the Legion namespace)
    Legion_API void          Value(const char* prefix, bool b);
    Legion_API void          Value(const char* prefix, int v);
    Legion_API void          Value(const char* prefix, unsigned int v);
    Legion_API void          Value(const char* prefix, float v, const char* float_format = NULL);

    // Widgets: Menus
    // - Use BeginMenuBar() on a window LegionWindowFlags_MenuBar to append to its menu bar.
    // - Use BeginMainMenuBar() to create a menu bar at the top of the screen and append to it.
    // - Use BeginMenu() to create a menu. You can call BeginMenu() multiple time with the same identifier to append more items to it.
    Legion_API bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires LegionWindowFlags_MenuBar flag set on parent window).
    Legion_API void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
    Legion_API bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
    Legion_API void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
    Legion_API bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
    Legion_API bool          BeginMenuEx(ImFont* font, float font_size, ImTextureID texture, ImTextureID scrollbg, ImTextureID scrollbar, const char* label, bool enabled);                  // create a sub-menu entry. only call EndMenu() if this returns true!
    Legion_API void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
    Legion_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by Legion at the moment
    Legion_API bool          MenuItemEx(ImFont* font, float font_size, ImTextureID texture, const char* label, const char* shortcut, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by Legion at the moment
    Legion_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL
    Legion_API bool          MenuItemEx(ImFont* font, float font_size, ImTextureID texture, const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

    // Tooltips
    // - Tooltip are windows following the mouse which do not take focus away.
    Legion_API void          BeginTooltip();                                                     // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
    Legion_API void          BeginTooltipV(ImTextureID texture);                                                     // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
    Legion_API void          EndTooltip();
    Legion_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     // set a text-only tooltip, typically use with Legion::IsItemHovered(). override any previous call to SetTooltip().
    Legion_API void          SetTooltipEx(ImFont* font, float font_size, ImTextureID texture, const char* fmt, ...) IM_FMTARGS(1);                     // set a text-only tooltip, typically use with Legion::IsItemHovered(). override any previous call to SetTooltip().
    Legion_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);
    Legion_API void          SetTooltipExV(ImFont* font, float font_size, ImTextureID texture, const char* fmt, va_list args) IM_FMTLIST(1);

    // Popups, Modals
    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by Legion instead of being held by the programmer as we are used to with regular Begin() calls.
    //   User can manipulate the visibility state by calling OpenPopup().
    // - We default to use the right mouse (LegionMouseButton_Right=1) for the Popup Context functions.
    // (*) You can use IsItemHovered(LegionHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.
    Legion_API void          OpenPopup(const char* str_id);                                      // call to mark popup as open (don't call every frame!). popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. By default, Selectable()/MenuItem() are calling CloseCurrentPopup(). Popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
    Legion_API bool          BeginPopup(const char* str_id, LegionWindowFlags flags = 0);                                             // return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returns true!
    Legion_API bool          BeginPopupContextItem(const char* str_id = NULL, LegionMouseButton mouse_button = 1);                    // helper to open and begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
    Legion_API bool          BeginPopupContextItemV(ImTextureID texture = NULL, const char* str_id = NULL, LegionMouseButton mouse_button = 1);                    // helper to open and begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
    Legion_API bool          BeginPopupContextWindow(const char* str_id = NULL, LegionMouseButton mouse_button = 1, bool also_over_items = true);  // helper to open and begin popup when clicked on current window.
    Legion_API bool          BeginPopupContextWindowV(ImTextureID texture, ImTextureID scrollbg, ImTextureID scrollbar, const char* str_id, LegionMouseButton mouse_button = 0, bool also_over_items = true);  // helper to open and begin popup when clicked on current window.
    Legion_API bool          BeginPopupContextVoid(const char* str_id = NULL, LegionMouseButton mouse_button = 1);                    // helper to open and begin popup when clicked in void (where there are no Legion windows).
    Legion_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, LegionWindowFlags flags = 0);                     // modal dialog (regular window with title bar, block interactions behind the modal window, can't close the modal window by clicking outside)
    Legion_API void          EndPopup();                                                                                             // only call EndPopup() if BeginPopupXXX() returns true!
    Legion_API bool          OpenPopupOnItemClick(const char* str_id = NULL, LegionMouseButton mouse_button = 1);                     // helper to open popup when clicked on last item (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors). return true when just opened.
    Legion_API bool          IsPopupOpen(const char* str_id);                                    // return true if the popup is open at the current begin-ed level of the popup stack.
    Legion_API void          CloseCurrentPopup();                                                // close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.

    // Columns
    // - You can also use SameLine(pos_x) to mimic simplified columns.
    // - The columns API is work-in-progress and rather lacking (columns are arguably the worst part of dear Legion at the moment!)
    // - There is a maximum of 64 columns.
    // - Currently working on new 'Tables' api which will replace columns around Q2 2020 (see GitHub #2957).
    Legion_API void          Columns(int count = 1, const char* id = NULL, bool border = true);
    Legion_API void          NextColumn();                                                       // next column, defaults to current row or next row if the current row is finished
    Legion_API int           GetColumnIndex();                                                   // get current column index
    Legion_API float         GetColumnWidth(int column_index = -1);                              // get column width (in pixels). pass -1 to use current column
    Legion_API void          SetColumnWidth(int column_index, float width);                      // set column width (in pixels). pass -1 to use current column
    Legion_API float         GetColumnOffset(int column_index = -1);                             // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
    Legion_API void          SetColumnOffset(int column_index, float offset_x);                  // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
    Legion_API int           GetColumnsCount();

    // Tab Bars, Tabs
    Legion_API bool          BeginTabBar(const char* str_id, LegionTabBarFlags flags = 0);        // create and append into a TabBar
    Legion_API void          EndTabBar();                                                        // only call EndTabBar() if BeginTabBar() returns true!
    Legion_API bool          BeginTabItem(const char* label, bool* p_open = NULL, LegionTabItemFlags flags = 0);// create a Tab. Returns true if the Tab is selected.
    Legion_API void          EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
    Legion_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

    // Logging/Capture
    // - All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
    Legion_API void          LogToTTY(int auto_open_depth = -1);                                 // start logging to tty (stdout)
    Legion_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start logging to file
    Legion_API void          LogToClipboard(int auto_open_depth = -1);                           // start logging to OS clipboard
    Legion_API void          LogFinish();                                                        // stop logging (close file, etc.)
    Legion_API void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
    Legion_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        // pass text data straight to log (without being displayed)

    // Drag and Drop
    // - [BETA API] API may evolve!
    Legion_API bool          BeginDragDropSource(LegionDragDropFlags flags = 0);                                      // call when the current item is active. If this return true, you can call SetDragDropPayload() + EndDragDropSource()
    Legion_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, LegionCond cond = 0);  // type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear Legion internal types. Data is copied and held by Legion.
    Legion_API void          EndDragDropSource();                                                                    // only call EndDragDropSource() if BeginDragDropSource() returns true!
    Legion_API bool                  BeginDragDropTarget();                                                          // call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
    Legion_API const LegionPayload*   AcceptDragDropPayload(const char* type, LegionDragDropFlags flags = 0);          // accept contents of a given type. If LegionDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
    Legion_API void                  EndDragDropTarget();                                                            // only call EndDragDropTarget() if BeginDragDropTarget() returns true!
    Legion_API const LegionPayload*   GetDragDropPayload();                                                           // peek directly into the current payload from anywhere. may return NULL. use LegionPayload::IsDataType() to test for the payload type.

    // Clipping
    Legion_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    Legion_API void          PopClipRect();

    // Focus, Activation
    // - Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item"
    Legion_API void          SetItemDefaultFocus();                                              // make last item the default focused item of a window.
    Legion_API void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.
    Legion_API void          SetKeyboardFocusHere(int offset, const char* name);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

    // Item/Widgets Utilities
    // - Most of the functions are referring to the last/previous item we submitted.
    // - See Demo Window under "Widgets->Querying Status" for an interactive visualization of most of those functions.
    Legion_API bool          IsItemHovered(LegionHoveredFlags flags = 0);                         // is the last item hovered? (and usable, aka not blocked by a popup, etc.). See LegionHoveredFlags for more options.
    Legion_API bool          IsItemActive();                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
    Legion_API bool          IsItemActive(const char* name);                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
    Legion_API bool          IsItemFocused();                                                    // is the last item focused for keyboard/gamepad navigation?
    Legion_API bool          IsItemFocused(const char* name);                                                    // is the last item focused for keyboard/gamepad navigation?
    Legion_API bool          IsItemClicked(LegionMouseButton mouse_button = 0);                   // is the last item clicked? (e.g. button/node just clicked on) == IsMouseClicked(mouse_button) && IsItemHovered()
    Legion_API bool          IsItemVisible();                                                    // is the last item visible? (items may be out of sight because of clipping/scrolling)
    Legion_API bool          IsItemEdited();                                                     // did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets.
    Legion_API bool          IsItemActivated();                                                  // was the last item just made active (item was previously inactive).
    Legion_API bool          IsItemActivated(const char* name);                                                  // was the last item just made active (item was previously inactive).
    Legion_API bool          IsItemDeactivated();                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
    Legion_API bool          IsItemDeactivated(const char* name);                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
    Legion_API bool          IsItemDeactivatedAfterEdit();                                       // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
    Legion_API bool          IsItemDeactivatedAfterEdit(const char* name);                                       // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
    Legion_API bool          IsItemToggledOpen();                                                // was the last item open state toggled? set by TreeNode().
    Legion_API bool          IsAnyItemHovered();                                                 // is any item hovered?
    Legion_API bool          IsAnyItemActive();                                                  // is any item active?
    Legion_API bool          IsAnyItemFocused();                                                 // is any item focused?
    Legion_API ImVec2        GetItemRectMin();                                                   // get upper-left bounding rectangle of the last item (screen space)
    Legion_API ImVec2        GetItemRectMax();                                                   // get lower-right bounding rectangle of the last item (screen space)
    Legion_API ImVec2        GetItemRectSize();                                                  // get size of last item
    Legion_API void          SetItemAllowOverlap();                                              // allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.

    // Miscellaneous Utilities
    Legion_API bool          IsRectVisible(const ImVec2& size);                                  // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
    Legion_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
    Legion_API double        GetTime();                                                          // get global Legion time. incremented by io.DeltaTime every frame.
    Legion_API int           GetFrameCount();                                                    // get global Legion frame count. incremented by 1 every frame.
    Legion_API ImDrawList*   GetBackgroundDrawList();                                            // this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear Legion contents.
    Legion_API ImDrawList*   GetForegroundDrawList();                                            // this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear Legion contents.
    Legion_API ImDrawListSharedData* GetDrawListSharedData();                                    // you may use this when creating your own ImDrawList instances.
    Legion_API const char*   GetStyleColorName(LegionCol idx);                                    // get a string corresponding to the enum value (for display, saving, etc.).
    Legion_API void          SetStateStorage(LegionStorage* storage);                             // replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
    Legion_API LegionStorage* GetStateStorage();
    Legion_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);    // calculate coarse clipping for large list of evenly sized items. Prefer using the LegionListClipper higher-level helper if you can.
    Legion_API bool          BeginChildFrame(LegionID id, const ImVec2& size, LegionWindowFlags flags = 0); // helper to create a child window / scrolling region that looks like a normal widget frame
    Legion_API void          EndChildFrame();                                                    // always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)

    // Text Utilities
    Legion_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
    Legion_API ImVec2        CalcTextSizeV(const char* text, ImFont* font, float font_size, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    // Color Utilities
    Legion_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
    Legion_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
    Legion_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    Legion_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    // Inputs Utilities: Keyboard
    // - For 'int user_key_index' you can use your own indices/enums according to how your backend/engine stored them in io.KeysDown[].
    // - We don't know the meaning of those value. You can use GetKeyIndex() to map a LegionKey_ value into the user index.
    Legion_API int           GetKeyIndex(LegionKey Legion_key);                                    // map LegionKey_* values into user's key index. == io.KeyMap[key]
    Legion_API bool          IsKeyDown(int user_key_index);                                      // is key being held. == io.KeysDown[user_key_index].
    Legion_API bool          IsKeyPressed(int user_key_index, bool repeat = true);               // was key pressed (went from !Down to Down)? if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
    Legion_API bool          IsKeyReleased(int user_key_index);                                  // was key released (went from Down to !Down)?
    Legion_API int           GetKeyPressedAmount(int key_index, float repeat_delay, float rate); // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
    Legion_API void          CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);    // attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.

    // Inputs Utilities: Mouse
    // - To refer to a mouse button, you may use named enums in your code e.g. LegionMouseButton_Left, LegionMouseButton_Right.
    // - You can also use regular integer: it is forever guaranteed that 0=Left, 1=Right, 2=Middle.
    // - Dragging operations are only reported after mouse has moved a certain distance away from the initial clicking position (see 'lock_threshold' and 'io.MouseDraggingThreshold')
    Legion_API bool          IsMouseDown(LegionMouseButton button);                               // is mouse button held?
    Legion_API bool          IsMouseClicked(LegionMouseButton button, bool repeat = false);       // did mouse button clicked? (went from !Down to Down)
    Legion_API bool          IsMouseReleased(LegionMouseButton button);                           // did mouse button released? (went from Down to !Down)
    Legion_API bool          IsMouseDoubleClicked(LegionMouseButton button);                      // did mouse button double-clicked? a double-click returns false in IsMouseClicked(). uses io.MouseDoubleClickTime.
    Legion_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);// is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
    Legion_API bool          IsMousePosValid(const ImVec2* mouse_pos = NULL);                    // by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse available
    Legion_API bool          IsAnyMouseDown();                                                   // is any mouse button held?
    Legion_API ImVec2        GetMousePos();                                                      // shortcut to Legion::GetIO().MousePos provided by user, to be consistent with other calls
    Legion_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                 // retrieve mouse position at the time of opening popup we have BeginPopup() into (helper to avoid user backing that value themselves)
    Legion_API bool          IsMouseDragging(LegionMouseButton button, float lock_threshold = -1.0f);         // is mouse dragging? (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
    Legion_API ImVec2        GetMouseDragDelta(LegionMouseButton button = 0, float lock_threshold = -1.0f);   // return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once (if lock_threshold < -1.0f, uses io.MouseDraggingThreshold)
    Legion_API void          ResetMouseDragDelta(LegionMouseButton button = 0);                   //
    Legion_API LegionMouseCursor GetMouseCursor();                                                // get desired cursor type, reset in Legion::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor Legion will render those for you
    Legion_API void          SetMouseCursor(LegionMouseCursor cursor_type);                       // set desired cursor type
    Legion_API void          CaptureMouseFromApp(bool want_capture_mouse_value = true);          // attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse_value;" after the next NewFrame() call.

    // Clipboard Utilities
    // - Also see the LogToClipboard() function to capture GUI into clipboard, or easily output text data to the clipboard.
    Legion_API const char*   GetClipboardText();
    Legion_API void          SetClipboardText(const char* text);

    // Settings/.Ini Utilities
    // - The disk functions are automatically called if io.IniFilename != NULL (default is "Legion.ini").
    // - Set io.IniFilename to NULL to load/save manually. Read io.WantSaveIniSettings description about handling .ini saving manually.
    Legion_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  // call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
    Legion_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size=0); // call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.
    Legion_API void          SaveIniSettingsToDisk(const char* ini_filename);                    // this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).
    Legion_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               // return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.

    // Debug Utilities
    Legion_API bool          DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx); // This is called by Legion_CHECKVERSION() macro.

    // Memory Allocators
    // - All those functions are not reliant on the current context.
    // - If you reload the contents of Legion.cpp at runtime, you may need to call SetCurrentContext() + SetAllocatorFunctions() again because we use global storage for those.
    Legion_API void          SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL);
    Legion_API void*         MemAlloc(size_t size);
    Legion_API void          MemFree(void* ptr);

} // namespace Legion

//-----------------------------------------------------------------------------
// Flags & Enumerations
//-----------------------------------------------------------------------------

// Flags for Legion::Begin()
enum LegionWindowFlags_
{
    LegionWindowFlags_None                   = 0,
    LegionWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    LegionWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
    LegionWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
    LegionWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    LegionWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    LegionWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it
    LegionWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
    LegionWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    LegionWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
    LegionWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    LegionWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    LegionWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in Legion_demo in the "Horizontal Scrolling" section.
    LegionWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    LegionWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    LegionWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    LegionWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    LegionWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
    LegionWindowFlags_NoNavInputs            = 1 << 18,  // No gamepad/keyboard navigation within the window
    LegionWindowFlags_NoNavFocus             = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
    LegionWindowFlags_UnsavedDocument        = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
    LegionWindowFlags_NoNav                  = LegionWindowFlags_NoNavInputs | LegionWindowFlags_NoNavFocus,
    LegionWindowFlags_NoDecoration           = LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoResize | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoCollapse,
    LegionWindowFlags_NoInputs               = LegionWindowFlags_NoMouseInputs | LegionWindowFlags_NoNavInputs | LegionWindowFlags_NoNavFocus,
    LegionWindowFlags_Custom = /*LegionWindowFlags_NoNavInputs | LegionWindowFlags_NoNavFocus | LegionWindowFlags_NoNav |*/ LegionWindowFlags_NoBackground | LegionWindowFlags_NoTitleBar | LegionWindowFlags_NoSavedSettings | LegionWindowFlags_NoResize | LegionWindowFlags_NoCollapse | LegionWindowFlags_NoMove | LegionWindowFlags_NoScrollbar | LegionWindowFlags_NoScrollWithMouse,

    // [Internal]
    LegionWindowFlags_NavFlattened           = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross over parent border to this child (only use on child that have no scrolling!)
    LegionWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
    LegionWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
    LegionWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
    LegionWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
    LegionWindowFlags_ChildMenu              = 1 << 28   // Don't use! For internal use by BeginMenu()

    // [Obsolete]
    //LegionWindowFlags_ShowBorders          = 1 << 7,   // --> Set style.FrameBorderSize=1.0f or style.WindowBorderSize=1.0f to enable borders around items or windows.
    //LegionWindowFlags_ResizeFromAnySide    = 1 << 17,  // --> Set io.ConfigWindowsResizeFromEdges=true and make sure mouse cursors are supported by back-end (io.BackendFlags & LegionBackendFlags_HasMouseCursors)
};

// Flags for Legion::InputText()
enum LegionInputTextFlags_
{
    LegionInputTextFlags_None                = 0,
    LegionInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
    LegionInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDEFabcdef
    LegionInputTextFlags_CharsUppercase      = 1 << 2,   // Turn a..z into A..Z
    LegionInputTextFlags_CharsNoBlank        = 1 << 3,   // Filter out spaces, tabs
    LegionInputTextFlags_AutoSelectAll       = 1 << 4,   // Select entire text when first taking mouse focus
    LegionInputTextFlags_EnterReturnsTrue    = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
    LegionInputTextFlags_CallbackCompletion  = 1 << 6,   // Callback on pressing TAB (for completion handling)
    LegionInputTextFlags_CallbackHistory     = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
    LegionInputTextFlags_CallbackAlways      = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
    LegionInputTextFlags_CallbackCharFilter  = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
    LegionInputTextFlags_AllowTabInput       = 1 << 10,  // Pressing TAB input a '\t' character into the text field
    LegionInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
    LegionInputTextFlags_NoHorizontalScroll  = 1 << 12,  // Disable following the cursor horizontally
    LegionInputTextFlags_AlwaysInsertMode    = 1 << 13,  // Insert mode
    LegionInputTextFlags_ReadOnly            = 1 << 14,  // Read-only mode
    LegionInputTextFlags_Password            = 1 << 15,  // Password mode, display all characters as '*'
    LegionInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
    LegionInputTextFlags_CharsScientific     = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
    LegionInputTextFlags_CallbackResize      = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/Legion_stdlib.h for an example of using this)
    // [Internal]
    LegionInputTextFlags_Multiline           = 1 << 20,  // For internal use by InputTextMultiline()
    LegionInputTextFlags_NoMarkEdited        = 1 << 21,   // For internal use by functions using InputText() before reformatting data
    LegionInputTextFlags_CharsHexadecimalNoBlank = 1 << 22   // For internal use by functions using InputText() before reformatting data
};

// Flags for Legion::TreeNodeEx(), Legion::CollapsingHeader*()
enum LegionTreeNodeFlags_
{
    LegionTreeNodeFlags_None                 = 0,
    LegionTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
    LegionTreeNodeFlags_Framed               = 1 << 1,   // Full colored frame (e.g. for CollapsingHeader)
    LegionTreeNodeFlags_AllowItemOverlap     = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
    LegionTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    LegionTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
    LegionTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
    LegionTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need double-click to open node
    LegionTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only open when clicking on the arrow part. If LegionTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
    LegionTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
    LegionTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow
    LegionTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
    LegionTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
    LegionTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (bypass the indented area).
    LegionTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
    //LegionTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
    LegionTreeNodeFlags_CollapsingHeader     = LegionTreeNodeFlags_Framed | LegionTreeNodeFlags_NoTreePushOnOpen | LegionTreeNodeFlags_NoAutoOpenOnLog
};

// Flags for Legion::Selectable()
enum LegionSelectableFlags_
{
    LegionSelectableFlags_None               = 0,
    LegionSelectableFlags_DontClosePopups    = 1 << 0,   // Clicking this don't close parent popup window
    LegionSelectableFlags_SpanAllColumns     = 1 << 1,   // Selectable frame can span all columns (text will still fit in current column)
    LegionSelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate press events on double clicks too
    LegionSelectableFlags_Disabled           = 1 << 3,   // Cannot be selected, display grayed out text
    LegionSelectableFlags_AllowItemOverlap   = 1 << 4    // (WIP) Hit testing to allow subsequent widgets to overlap this one
};

// Flags for Legion::BeginCombo()
enum LegionComboFlags_
{
    LegionComboFlags_None                    = 0,
    LegionComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup toward the left by default
    LegionComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    LegionComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible (default)
    LegionComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    LegionComboFlags_HeightLargest           = 1 << 4,   // As many fitting items as possible
    LegionComboFlags_NoArrowButton           = 1 << 5,   // Display on the preview box without the square arrow button
    LegionComboFlags_NoPreview               = 1 << 6,   // Display only a square arrow button
    LegionComboFlags_HeightMask_             = LegionComboFlags_HeightSmall | LegionComboFlags_HeightRegular | LegionComboFlags_HeightLarge | LegionComboFlags_HeightLargest
};

// Flags for Legion::BeginTabBar()
enum LegionTabBarFlags_
{
    LegionTabBarFlags_None                           = 0,
    LegionTabBarFlags_Reorderable                    = 1 << 0,   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
    LegionTabBarFlags_AutoSelectNewTabs              = 1 << 1,   // Automatically select new tabs when they appear
    LegionTabBarFlags_TabListPopupButton             = 1 << 2,   // Disable buttons to open the tab list popup
    LegionTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    LegionTabBarFlags_NoTabListScrollingButtons      = 1 << 4,   // Disable scrolling buttons (apply when fitting policy is LegionTabBarFlags_FittingPolicyScroll)
    LegionTabBarFlags_NoTooltip                      = 1 << 5,   // Disable tooltips when hovering a tab
    LegionTabBarFlags_FittingPolicyResizeDown        = 1 << 6,   // Resize tabs when they don't fit
    LegionTabBarFlags_FittingPolicyScroll            = 1 << 7,   // Add scroll buttons when tabs don't fit
    LegionTabBarFlags_FittingPolicyMask_             = LegionTabBarFlags_FittingPolicyResizeDown | LegionTabBarFlags_FittingPolicyScroll,
    LegionTabBarFlags_FittingPolicyDefault_          = LegionTabBarFlags_FittingPolicyResizeDown
};

// Flags for Legion::BeginTabItem()
enum LegionTabItemFlags_
{
    LegionTabItemFlags_None                          = 0,
    LegionTabItemFlags_UnsavedDocument               = 1 << 0,   // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. Also: tab is selected on closure and closure is deferred by one frame to allow code to undo it without flicker.
    LegionTabItemFlags_SetSelected                   = 1 << 1,   // Trigger flag to programmatically make the tab selected when calling BeginTabItem()
    LegionTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
    LegionTabItemFlags_NoPushId                      = 1 << 3    // Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem()
};

// Flags for Legion::IsWindowFocused()
enum LegionFocusedFlags_
{
    LegionFocusedFlags_None                          = 0,
    LegionFocusedFlags_ChildWindows                  = 1 << 0,   // IsWindowFocused(): Return true if any children of the window is focused
    LegionFocusedFlags_RootWindow                    = 1 << 1,   // IsWindowFocused(): Test from root window (top most parent of the current hierarchy)
    LegionFocusedFlags_AnyWindow                     = 1 << 2,   // IsWindowFocused(): Return true if any window is focused. Important: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
    LegionFocusedFlags_RootAndChildWindows           = LegionFocusedFlags_RootWindow | LegionFocusedFlags_ChildWindows
};

// Flags for Legion::IsItemHovered(), Legion::IsWindowHovered()
// Note: if you are trying to check whether your mouse should be dispatched to Dear Legion or to your app, you should use 'io.WantCaptureMouse' instead! Please read the FAQ!
// Note: windows with the LegionWindowFlags_NoInputs flag are ignored by IsWindowHovered() calls.
enum LegionHoveredFlags_
{
    LegionHoveredFlags_None                          = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
    LegionHoveredFlags_ChildWindows                  = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
    LegionHoveredFlags_RootWindow                    = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
    LegionHoveredFlags_AnyWindow                     = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
    LegionHoveredFlags_AllowWhenBlockedByPopup       = 1 << 3,   // Return true even if a popup window is normally blocking access to this item/window
    //LegionHoveredFlags_AllowWhenBlockedByModal     = 1 << 4,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
    LegionHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 5,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
    LegionHoveredFlags_AllowWhenOverlapped           = 1 << 6,   // Return true even if the position is obstructed or overlapped by another window
    LegionHoveredFlags_AllowWhenDisabled             = 1 << 7,   // Return true even if the item is disabled
    LegionHoveredFlags_RectOnly                      = LegionHoveredFlags_AllowWhenBlockedByPopup | LegionHoveredFlags_AllowWhenBlockedByActiveItem | LegionHoveredFlags_AllowWhenOverlapped,
    LegionHoveredFlags_RootAndChildWindows           = LegionHoveredFlags_RootWindow | LegionHoveredFlags_ChildWindows
};

// Flags for Legion::BeginDragDropSource(), Legion::AcceptDragDropPayload()
enum LegionDragDropFlags_
{
    LegionDragDropFlags_None                         = 0,
    // BeginDragDropSource() flags
    LegionDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   // By default, a successful call to BeginDragDropSource opens a tooltip so you can display a preview or description of the source contents. This flag disable this behavior.
    LegionDragDropFlags_SourceNoDisableHover         = 1 << 1,   // By default, when dragging we clear data so that IsItemHovered() will return false, to avoid subsequent user code submitting tooltips. This flag disable this behavior so you can still call IsItemHovered() on the source item.
    LegionDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   // Disable the behavior that allows to open tree nodes and collapsing header by holding over them while dragging a source item.
    LegionDragDropFlags_SourceAllowNullID            = 1 << 3,   // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear Legion ecosystem and so we made it explicit.
    LegionDragDropFlags_SourceExtern                 = 1 << 4,   // External source (from outside of dear Legion), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active simultaneously.
    LegionDragDropFlags_SourceAutoExpirePayload      = 1 << 5,   // Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)
    // AcceptDragDropPayload() flags
    LegionDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
    LegionDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  // Do not draw the default highlight rectangle when hovering over target.
    LegionDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
    LegionDragDropFlags_AcceptPeekOnly               = LegionDragDropFlags_AcceptBeforeDelivery | LegionDragDropFlags_AcceptNoDrawDefaultRect  // For peeking ahead and inspecting the payload before delivery.
};

// Standard Drag and Drop payload types. You can define you own payload types using short strings. Types starting with '_' are defined by Dear Legion.
#define Legion_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    // float[3]: Standard type for colors, without alpha. User code may use this type.
#define Legion_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    // float[4]: Standard type for colors. User code may use this type.

// A primary data type
enum LegionDataType_
{
    LegionDataType_S8,       // signed char / char (with sensible compilers)
    LegionDataType_U8,       // unsigned char
    LegionDataType_S16,      // short
    LegionDataType_U16,      // unsigned short
    LegionDataType_S32,      // int
    LegionDataType_U32,      // unsigned int
    LegionDataType_S64,      // long long / __int64
    LegionDataType_U64,      // unsigned long long / unsigned __int64
    LegionDataType_Float,    // float
    LegionDataType_Double,   // double
    LegionDataType_COUNT
};

// A cardinal direction
enum LegionDir_
{
    LegionDir_None    = -1,
    LegionDir_Left    = 0,
    LegionDir_Right   = 1,
    LegionDir_Up      = 2,
    LegionDir_Down    = 3,
    LegionDir_COUNT
};

// User fill LegionIO.KeyMap[] array with indices into the LegionIO.KeysDown[512] array
enum LegionKey_
{
    LegionKey_Tab,
    LegionKey_LeftArrow,
    LegionKey_RightArrow,
    LegionKey_UpArrow,
    LegionKey_DownArrow,
    LegionKey_PageUp,
    LegionKey_PageDown,
    LegionKey_Home,
    LegionKey_End,
    LegionKey_Insert,
    LegionKey_Delete,
    LegionKey_Backspace,
    LegionKey_Space,
    LegionKey_Enter,
    LegionKey_Escape,
    LegionKey_KeyPadEnter,
    LegionKey_A,                 // for text edit CTRL+A: select all
    LegionKey_C,                 // for text edit CTRL+C: copy
    LegionKey_V,                 // for text edit CTRL+V: paste
    LegionKey_X,                 // for text edit CTRL+X: cut
    LegionKey_Y,                 // for text edit CTRL+Y: redo
    LegionKey_Z,                 // for text edit CTRL+Z: undo
    LegionKey_COUNT
};

// To test io.KeyMods (which is a combination of individual fields io.KeyCtrl, io.KeyShift, io.KeyAlt set by user/back-end)
enum LegionKeyModFlags_
{
    LegionKeyModFlags_None       = 0,
    LegionKeyModFlags_Ctrl       = 1 << 0,
    LegionKeyModFlags_Shift      = 1 << 1,
    LegionKeyModFlags_Alt        = 1 << 2,
    LegionKeyModFlags_Super      = 1 << 3
};

// Gamepad/Keyboard navigation
// Keyboard: Set io.ConfigFlags |= LegionConfigFlags_NavEnableKeyboard to enable. NewFrame() will automatically fill io.NavInputs[] based on your io.KeysDown[] + io.KeyMap[] arrays.
// Gamepad:  Set io.ConfigFlags |= LegionConfigFlags_NavEnableGamepad to enable. Back-end: set LegionBackendFlags_HasGamepad and fill the io.NavInputs[] fields before calling NewFrame(). Note that io.NavInputs[] is cleared by EndFrame().
// Read instructions in Legion.cpp for more details. Download PNG/PSD at http://goo.gl/9LgVZW.
enum LegionNavInput_
{
    // Gamepad Mapping
    LegionNavInput_Activate,      // activate / open / toggle / tweak value       // e.g. Cross  (PS4), A (Xbox), A (Switch), Space (Keyboard)
    LegionNavInput_Cancel,        // cancel / close / exit                        // e.g. Circle (PS4), B (Xbox), B (Switch), Escape (Keyboard)
    LegionNavInput_Input,         // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
    LegionNavInput_Menu,          // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
    LegionNavInput_DpadLeft,      // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
    LegionNavInput_DpadRight,     //
    LegionNavInput_DpadUp,        //
    LegionNavInput_DpadDown,      //
    LegionNavInput_LStickLeft,    // scroll / move window (w/ PadMenu)            // e.g. Left Analog Stick Left/Right/Up/Down
    LegionNavInput_LStickRight,   //
    LegionNavInput_LStickUp,      //
    LegionNavInput_LStickDown,    //
    LegionNavInput_FocusPrev,     // next window (w/ PadMenu)                     // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    LegionNavInput_FocusNext,     // prev window (w/ PadMenu)                     // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
    LegionNavInput_TweakSlow,     // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
    LegionNavInput_TweakFast,     // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

    // [Internal] Don't use directly! This is used internally to differentiate keyboard from gamepad inputs for behaviors that require to differentiate them.
    // Keyboard behavior that have no corresponding gamepad mapping (e.g. CTRL+TAB) will be directly reading from io.KeysDown[] instead of io.NavInputs[].
    LegionNavInput_KeyMenu_,      // toggle menu                                  // = io.KeyAlt
    LegionNavInput_KeyLeft_,      // move left                                    // = Arrow keys
    LegionNavInput_KeyRight_,     // move right
    LegionNavInput_KeyUp_,        // move up
    LegionNavInput_KeyDown_,      // move down
    LegionNavInput_COUNT,
    LegionNavInput_InternalStart_ = LegionNavInput_KeyMenu_
};

// Configuration flags stored in io.ConfigFlags. Set by user/application.
enum LegionConfigFlags_
{
    LegionConfigFlags_None                   = 0,
    LegionConfigFlags_NavEnableKeyboard      = 1 << 0,   // Master keyboard navigation enable flag. NewFrame() will automatically fill io.NavInputs[] based on io.KeysDown[].
    LegionConfigFlags_NavEnableGamepad       = 1 << 1,   // Master gamepad navigation enable flag. This is mostly to instruct your Legion back-end to fill io.NavInputs[]. Back-end also needs to set LegionBackendFlags_HasGamepad.
    LegionConfigFlags_NavEnableSetMousePos   = 1 << 2,   // Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=true. If enabled you MUST honor io.WantSetMousePos requests in your binding, otherwise Legion will react as if the mouse is jumping around back and forth.
    LegionConfigFlags_NavNoCaptureKeyboard   = 1 << 3,   // Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.
    LegionConfigFlags_NoMouse                = 1 << 4,   // Instruct Legion to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the back-end.
    LegionConfigFlags_NoMouseCursorChange    = 1 << 5,   // Instruct back-end to not alter mouse cursor shape and visibility. Use if the back-end cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from Legion by reading GetMouseCursor() yourself instead.

    // User storage (to allow your back-end/engine to communicate to code that may be shared between multiple projects. Those flags are not used by core Dear Legion)
    LegionConfigFlags_IsSRGB                 = 1 << 20,  // Application is SRGB-aware.
    LegionConfigFlags_IsTouchScreen          = 1 << 21   // Application is using a touch screen instead of a mouse.
};

// Back-end capabilities flags stored in io.BackendFlags. Set by Legion_impl_xxx or custom back-end.
enum LegionBackendFlags_
{
    LegionBackendFlags_None                  = 0,
    LegionBackendFlags_HasGamepad            = 1 << 0,   // Back-end Platform supports gamepad and currently has one connected.
    LegionBackendFlags_HasMouseCursors       = 1 << 1,   // Back-end Platform supports honoring GetMouseCursor() value to change the OS cursor shape.
    LegionBackendFlags_HasSetMousePos        = 1 << 2,   // Back-end Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if LegionConfigFlags_NavEnableSetMousePos is set).
    LegionBackendFlags_RendererHasVtxOffset  = 1 << 3    // Back-end Renderer supports ImDrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.
};

// Enumeration for PushStyleColor() / PopStyleColor()
enum LegionCol_
{
    LegionCol_Text,
    LegionCol_TextDisabled,
    LegionCol_WindowBg,              // Background of normal windows
    LegionCol_ChildBg,               // Background of child windows
    LegionCol_PopupBg,               // Background of popups, menus, tooltips windows
    LegionCol_Border,
    LegionCol_BorderShadow,
    LegionCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    LegionCol_FrameBgHovered,
    LegionCol_FrameBgActive,
    LegionCol_TitleBg,
    LegionCol_TitleBgActive,
    LegionCol_TitleBgCollapsed,
    LegionCol_MenuBarBg,
    LegionCol_ScrollbarBg,
    LegionCol_ScrollbarGrab,
    LegionCol_ScrollbarGrabHovered,
    LegionCol_ScrollbarGrabActive,
    LegionCol_CheckMark,
    LegionCol_SliderGrab,
    LegionCol_SliderGrabActive,
    LegionCol_Button,
    LegionCol_ButtonHovered,
    LegionCol_ButtonActive,
    LegionCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    LegionCol_HeaderHovered,
    LegionCol_HeaderActive,
    LegionCol_Separator,
    LegionCol_SeparatorHovered,
    LegionCol_SeparatorActive,
    LegionCol_ResizeGrip,
    LegionCol_ResizeGripHovered,
    LegionCol_ResizeGripActive,
    LegionCol_Tab,
    LegionCol_TabHovered,
    LegionCol_TabActive,
    LegionCol_TabUnfocused,
    LegionCol_TabUnfocusedActive,
    LegionCol_PlotLines,
    LegionCol_PlotLinesHovered,
    LegionCol_PlotHistogram,
    LegionCol_PlotHistogramHovered,
    LegionCol_TextSelectedBg,
    LegionCol_DragDropTarget,
    LegionCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
    LegionCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
    LegionCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    LegionCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    LegionCol_COUNT

    // Obsolete names (will be removed)
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    , LegionCol_ModalWindowDarkening = LegionCol_ModalWindowDimBg                      // [renamed in 1.63]
    //, LegionCol_CloseButton, LegionCol_CloseButtonActive, LegionCol_CloseButtonHovered// [unused since 1.60+] the close button now uses regular button colors.
#endif
};

// Enumeration for PushStyleVar() / PopStyleVar() to temporarily modify the LegionStyle structure.
// - The enum only refers to fields of LegionStyle which makes sense to be pushed/popped inside UI code. 
//   During initialization or between frames, feel free to just poke into LegionStyle directly.
// - Tip: Use your programming IDE navigation facilities on the names in the _second column_ below to find the actual members and their description.
//   In Visual Studio IDE: CTRL+comma ("Edit.NavigateTo") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
//   With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.
// - When changing this enum, you need to update the associated internal table GStyleVarInfo[] accordingly. This is where we link enum values to members offset/type.
enum LegionStyleVar_
{
    // Enum name --------------------- // Member in LegionStyle structure (see LegionStyle for descriptions)
    LegionStyleVar_Alpha,               // float     Alpha
    LegionStyleVar_WindowPadding,       // ImVec2    WindowPadding
    LegionStyleVar_WindowRounding,      // float     WindowRounding
    LegionStyleVar_WindowBorderSize,    // float     WindowBorderSize
    LegionStyleVar_WindowMinSize,       // ImVec2    WindowMinSize
    LegionStyleVar_WindowTitleAlign,    // ImVec2    WindowTitleAlign
    LegionStyleVar_ChildRounding,       // float     ChildRounding
    LegionStyleVar_ChildBorderSize,     // float     ChildBorderSize
    LegionStyleVar_PopupRounding,       // float     PopupRounding
    LegionStyleVar_PopupBorderSize,     // float     PopupBorderSize
    LegionStyleVar_FramePadding,        // ImVec2    FramePadding
    LegionStyleVar_FrameRounding,       // float     FrameRounding
    LegionStyleVar_FrameBorderSize,     // float     FrameBorderSize
    LegionStyleVar_ItemSpacing,         // ImVec2    ItemSpacing
    LegionStyleVar_ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
    LegionStyleVar_IndentSpacing,       // float     IndentSpacing
    LegionStyleVar_ScrollbarSize,       // float     ScrollbarSize
    LegionStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
    LegionStyleVar_GrabMinSize,         // float     GrabMinSize
    LegionStyleVar_GrabRounding,        // float     GrabRounding
    LegionStyleVar_TabRounding,         // float     TabRounding
    LegionStyleVar_ButtonTextAlign,     // ImVec2    ButtonTextAlign
    LegionStyleVar_SelectableTextAlign, // ImVec2    SelectableTextAlign
    LegionStyleVar_COUNT

    // Obsolete names (will be removed)
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    , LegionStyleVar_Count_ = LegionStyleVar_COUNT                    // [renamed in 1.60]
#endif
};

// Flags for ColorEdit3() / ColorEdit4() / ColorPicker3() / ColorPicker4() / ColorButton()
enum LegionColorEditFlags_
{
    LegionColorEditFlags_None            = 0,
    LegionColorEditFlags_NoAlpha         = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
    LegionColorEditFlags_NoPicker        = 1 << 2,   //              // ColorEdit: disable picker when clicking on colored square.
    LegionColorEditFlags_NoOptions       = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
    LegionColorEditFlags_NoSmallPreview  = 1 << 4,   //              // ColorEdit, ColorPicker: disable colored square preview next to the inputs. (e.g. to show only the inputs)
    LegionColorEditFlags_NoInputs        = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview colored square).
    LegionColorEditFlags_NoTooltip       = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
    LegionColorEditFlags_NoLabel         = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
    LegionColorEditFlags_NoSidePreview   = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small colored square preview instead.
    LegionColorEditFlags_NoDragDrop      = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
    LegionColorEditFlags_NoBorder        = 1 << 10,  //              // ColorButton: disable border (which is enforced by default)

    // User Options (right-click on widget to change some of them).
    LegionColorEditFlags_AlphaBar        = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
    LegionColorEditFlags_AlphaPreview    = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
    LegionColorEditFlags_AlphaPreviewHalf= 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
    LegionColorEditFlags_HDR             = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f limits in RGBA edition (note: you probably want to use LegionColorEditFlags_Float flag as well).
    LegionColorEditFlags_DisplayRGB      = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
    LegionColorEditFlags_DisplayHSV      = 1 << 21,  // [Display]    // "
    LegionColorEditFlags_DisplayHex      = 1 << 22,  // [Display]    // "
    LegionColorEditFlags_Uint8           = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
    LegionColorEditFlags_Float           = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
    LegionColorEditFlags_PickerHueBar    = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
    LegionColorEditFlags_PickerHueWheel  = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
    LegionColorEditFlags_InputRGB        = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
    LegionColorEditFlags_InputHSV        = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.

    // Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
    // override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
    LegionColorEditFlags__OptionsDefault = LegionColorEditFlags_Uint8|LegionColorEditFlags_DisplayRGB|LegionColorEditFlags_InputRGB|LegionColorEditFlags_PickerHueBar,

    // [Internal] Masks
    LegionColorEditFlags__DisplayMask    = LegionColorEditFlags_DisplayRGB|LegionColorEditFlags_DisplayHSV|LegionColorEditFlags_DisplayHex,
    LegionColorEditFlags__DataTypeMask   = LegionColorEditFlags_Uint8|LegionColorEditFlags_Float,
    LegionColorEditFlags__PickerMask     = LegionColorEditFlags_PickerHueWheel|LegionColorEditFlags_PickerHueBar,
    LegionColorEditFlags__InputMask      = LegionColorEditFlags_InputRGB|LegionColorEditFlags_InputHSV

    // Obsolete names (will be removed)
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    , LegionColorEditFlags_RGB = LegionColorEditFlags_DisplayRGB, LegionColorEditFlags_HSV = LegionColorEditFlags_DisplayHSV, LegionColorEditFlags_HEX = LegionColorEditFlags_DisplayHex  // [renamed in 1.69]
#endif
};

// Identify a mouse button.
// Those values are guaranteed to be stable and we frequently use 0/1 directly. Named enums provided for convenience.
enum LegionMouseButton_
{
    LegionMouseButton_Left = 0,
    LegionMouseButton_Right = 1,
    LegionMouseButton_Middle = 2,
    LegionMouseButton_COUNT = 5
};

// Enumeration for GetMouseCursor()
// User code may request binding to display given cursor by calling SetMouseCursor(), which is why we have some cursors that are marked unused here
enum LegionMouseCursor_
{
    LegionMouseCursor_None = -1,
    LegionMouseCursor_Arrow = 0,
    LegionMouseCursor_TextInput,         // When hovering over InputText, etc.
    LegionMouseCursor_ResizeAll,         // (Unused by Dear Legion functions)
    LegionMouseCursor_ResizeNS,          // When hovering over an horizontal border
    LegionMouseCursor_ResizeEW,          // When hovering over a vertical border or a column
    LegionMouseCursor_ResizeNESW,        // When hovering over the bottom-left corner of a window
    LegionMouseCursor_ResizeNWSE,        // When hovering over the bottom-right corner of a window
    LegionMouseCursor_Hand,              // (Unused by Dear Legion functions. Use for e.g. hyperlinks)
    LegionMouseCursor_NotAllowed,        // When hovering something with disallowed interaction. Usually a crossed circle.
    LegionMouseCursor_COUNT

    // Obsolete names (will be removed)
#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    , LegionMouseCursor_Count_ = LegionMouseCursor_COUNT      // [renamed in 1.60]
#endif
};

// Enumeration for Legion::SetWindow***(), SetNextWindow***(), SetNextItem***() functions
// Represent a condition.
// Important: Treat as a regular enum! Do NOT combine multiple values using binary operators! All the functions above treat 0 as a shortcut to LegionCond_Always.
enum LegionCond_
{
    LegionCond_Always        = 1 << 0,   // Set the variable
    LegionCond_Once          = 1 << 1,   // Set the variable once per runtime session (only the first call with succeed)
    LegionCond_FirstUseEver  = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
    LegionCond_Appearing     = 1 << 3    // Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};

//-----------------------------------------------------------------------------
// Helpers: Memory allocations macros
// IM_MALLOC(), IM_FREE(), IM_NEW(), IM_PLACEMENT_NEW(), IM_DELETE()
// We call C++ constructor on own allocated memory via the placement "new(ptr) Type()" syntax.
// Defining a custom placement new() with a dummy parameter allows us to bypass including <new> which on some platforms complains when user has disabled exceptions.
//-----------------------------------------------------------------------------

struct ImNewDummy {};
inline void* operator new(size_t, ImNewDummy, void* ptr) { return ptr; }
inline void  operator delete(void*, ImNewDummy, void*)   {} // This is only required so we can use the symmetrical new()
#define IM_ALLOC(_SIZE)                     Legion::MemAlloc(_SIZE)
#define IM_FREE(_PTR)                       Legion::MemFree(_PTR)
#define IM_PLACEMENT_NEW(_PTR)              new(ImNewDummy(), _PTR)
#define IM_NEW(_TYPE)                       new(ImNewDummy(), Legion::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void IM_DELETE(T* p)   { if (p) { p->~T(); Legion::MemFree(p); } }

//-----------------------------------------------------------------------------
// Helper: ImVector<>
// Lightweight std::vector<>-like class to avoid dragging dependencies (also, some implementations of STL with debug enabled are absurdly slow, we bypass it so our code runs fast in debug).
//-----------------------------------------------------------------------------
// - You generally do NOT need to care or use this ever. But we need to make it available in Legion.h because some of our public structures are relying on it.
// - We use std-like naming convention here, which is a little unusual for this codebase.
// - Important: clear() frees memory, resize(0) keep the allocated buffer. We use resize(0) a lot to intentionally recycle allocated buffers across frames and amortize our costs.
// - Important: our implementation does NOT call C++ constructors/destructors, we treat everything as raw data! This is intentional but be extra mindful of that,
//   Do NOT use this class as a std::vector replacement in your own code! Many of the structures used by dear Legion can be safely initialized by a zero-memset.
//-----------------------------------------------------------------------------

template<typename T>
struct ImVector
{
    int                 Size;
    int                 Capacity;
    T*                  Data;

    // Provide standard typedefs but we don't use them ourselves.
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    // Constructors, destructor
    inline ImVector()                                       { Size = Capacity = 0; Data = NULL; }
    inline ImVector(const ImVector<T>& src)                 { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline ImVector<T>& operator=(const ImVector<T>& src)   { clear(); resize(src.Size); memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~ImVector()                                      { if (Data) IM_FREE(Data); }

    inline bool         empty() const                       { return Size == 0; }
    inline int          size() const                        { return Size; }
    inline int          size_in_bytes() const               { return Size * (int)sizeof(T); }
    inline int          capacity() const                    { return Capacity; }
    inline T&           operator[](int i)                   { IM_ASSERT(i < Size); return Data[i]; }
    inline const T&     operator[](int i) const             { IM_ASSERT(i < Size); return Data[i]; }

    inline void         clear()                             { if (Data) { Size = Capacity = 0; IM_FREE(Data); Data = NULL; } }
    inline T*           begin()                             { return Data; }
    inline const T*     begin() const                       { return Data; }
    inline T*           end()                               { return Data + Size; }
    inline const T*     end() const                         { return Data + Size; }
    inline T&           front()                             { IM_ASSERT(Size > 0); return Data[0]; }
    inline const T&     front() const                       { IM_ASSERT(Size > 0); return Data[0]; }
    inline T&           back()                              { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T&     back() const                        { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(ImVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const        { int new_capacity = Capacity ? (Capacity + Capacity/2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size)                { IM_ASSERT(new_size <= Size); Size = new_size; } // Resize a vector to a smaller size, guaranteed not to cause a reallocation
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }

    // NB: It is illegal to call push_back/push_front/insert with a reference pointing inside the ImVector data itself! e.g. v.push_back(v[10]) is forbidden.
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back()                          { IM_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v)              { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T*           erase(const T* it)                  { IM_ASSERT(it >= Data && it < Data+Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T*           erase(const T* it, const T* it_last){ IM_ASSERT(it >= Data && it < Data+Size && it_last > it && it_last <= Data+Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T*           erase_unsorted(const T* it)         { IM_ASSERT(it >= Data && it < Data+Size);  const ptrdiff_t off = it - Data; if (it < Data+Size-1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data+Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const          { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T*           find(const T& v)                    { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T*     find(const T& v) const              { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline bool         find_erase(const T& v)              { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v)     { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
};

//-----------------------------------------------------------------------------
// LegionStyle
// You may modify the Legion::GetStyle() main instance during initialization and before NewFrame().
// During the frame, use Legion::PushStyleVar(LegionStyleVar_XXXX)/PopStyleVar() to alter the main style values,
// and Legion::PushStyleColor(LegionCol_XXX)/PopStyleColor() for colors.
//-----------------------------------------------------------------------------

struct LegionStyle
{
    float       Alpha;                      // Global alpha applies to everything in Dear Legion.
    ImVec2      WindowPadding;              // Padding within a window.
    float       WindowRounding;             // Radius of window corners rounding. Set to 0.0f to have rectangular windows.
    float       WindowBorderSize;           // Thickness of border around windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      WindowMinSize;              // Minimum window size. This is a global setting. If you want to constraint individual windows, use SetNextWindowSizeConstraints().
    ImVec2      WindowTitleAlign;           // Alignment for title bar text. Defaults to (0.0f,0.5f) for left-aligned,vertically centered.
    LegionDir    WindowMenuButtonPosition;   // Side of the collapsing/docking button in the title bar (None/Left/Right). Defaults to LegionDir_Left.
    float       ChildRounding;              // Radius of child window corners rounding. Set to 0.0f to have rectangular windows.
    float       ChildBorderSize;            // Thickness of border around child windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    float       PopupRounding;              // Radius of popup window corners rounding. (Note that tooltip windows use WindowRounding)
    float       PopupBorderSize;            // Thickness of border around popup/tooltip windows. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      FramePadding;               // Padding within a framed rectangle (used by most widgets).
    float       FrameRounding;              // Radius of frame corners rounding. Set to 0.0f to have rectangular frame (used by most widgets).
    float       FrameBorderSize;            // Thickness of border around frames. Generally set to 0.0f or 1.0f. (Other values are not well tested and more CPU/GPU costly).
    ImVec2      ItemSpacing;                // Horizontal and vertical spacing between widgets/lines.
    ImVec2      ItemInnerSpacing;           // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label).
    ImVec2      TouchExtraPadding;          // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    float       IndentSpacing;              // Horizontal indentation when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    float       ColumnsMinSpacing;          // Minimum horizontal spacing between two columns. Preferably > (FramePadding.x + 1).
    float       ScrollbarSize;              // Width of the vertical scrollbar, Height of the horizontal scrollbar.
    float       ScrollbarRounding;          // Radius of grab corners for scrollbar.
    float       GrabMinSize;                // Minimum width/height of a grab box for slider/scrollbar.
    float       GrabRounding;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    float       TabRounding;                // Radius of upper corners of a tab. Set to 0.0f to have rectangular tabs.
    float       TabBorderSize;              // Thickness of border around tabs.
    LegionDir    ColorButtonPosition;        // Side of the color button in the ColorEdit4 widget (left/right). Defaults to LegionDir_Right.
    ImVec2      ButtonTextAlign;            // Alignment of button text when button is larger than text. Defaults to (0.5f, 0.5f) (centered).
    ImVec2      SelectableTextAlign;        // Alignment of selectable text. Defaults to (0.0f, 0.0f) (top-left aligned). It's generally important to keep this left-aligned if you want to lay multiple items on a same line.
    ImVec2      DisplayWindowPadding;       // Window position are clamped to be visible within the display area by at least this amount. Only applies to regular windows.
    ImVec2      DisplaySafeAreaPadding;     // If you cannot see the edges of your screen (e.g. on a TV) increase the safe area padding. Apply to popups/tooltips as well regular windows. NB: Prefer configuring your TV sets correctly!
    float       MouseCursorScale;           // Scale software rendered mouse cursor (when io.MouseDrawCursor is enabled). May be removed later.
    bool        AntiAliasedLines;           // Enable anti-aliasing on lines/borders. Disable if you are really tight on CPU/GPU.
    bool        AntiAliasedFill;            // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    float       CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo() without a specific number of segments. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
    float       CircleSegmentMaxError;      // Maximum error (in pixels) allowed when using AddCircle()/AddCircleFilled() or drawing rounded corner rectangles with no explicit segment count specified. Decrease for higher quality but more geometry.
    ImVec4      Colors[LegionCol_COUNT];

    Legion_API LegionStyle();
    Legion_API void ScaleAllSizes(float scale_factor);
};

//-----------------------------------------------------------------------------
// LegionIO
// Communicate most settings and inputs/outputs to Dear Legion using this structure.
// Access via Legion::GetIO(). Read 'Programmer guide' section in .cpp file for general usage.
//-----------------------------------------------------------------------------

struct LegionIO
{
    //------------------------------------------------------------------
    // Configuration (fill once)                // Default value
    //------------------------------------------------------------------

    LegionConfigFlags   ConfigFlags;             // = 0              // See LegionConfigFlags_ enum. Set by user/application. Gamepad/keyboard navigation options, etc.
    LegionBackendFlags  BackendFlags;            // = 0              // See LegionBackendFlags_ enum. Set by back-end (Legion_impl_xxx files or custom back-end) to communicate features supported by the back-end.
    ImVec2      DisplaySize;                    // <unset>          // Main display size, in pixels.
    float       DeltaTime;                      // = 1.0f/60.0f     // Time elapsed since last frame, in seconds.
    float       IniSavingRate;                  // = 5.0f           // Minimum time between saving positions/sizes to .ini file, in seconds.
    const char* IniFilename;                    // = "Legion.ini"    // Path to .ini file. Set NULL to disable automatic .ini loading/saving, if e.g. you want to manually load/save from memory.
    const char* LogFilename;                    // = "Legion_log.txt"// Path to .log file (default parameter to Legion::LogToFile when no file is specified).
    float       MouseDoubleClickTime;           // = 0.30f          // Time for a double-click, in seconds.
    float       MouseDoubleClickMaxDist;        // = 6.0f           // Distance threshold to stay in to validate a double-click, in pixels.
    float       MouseDragThreshold;             // = 6.0f           // Distance threshold before considering we are dragging.
    int         KeyMap[LegionKey_COUNT];         // <unset>          // Map of indices into the KeysDown[512] entries array which represent your "native" keyboard state.
    float       KeyRepeatDelay;                 // = 0.250f         // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
    float       KeyRepeatRate;                  // = 0.050f         // When holding a key/button, rate at which it repeats, in seconds.
    void*       UserData;                       // = NULL           // Store your own data for retrieval by callbacks.

    ImFontAtlas*Fonts;                          // <auto>           // Font atlas: load, rasterize and pack one or more fonts into a single texture.
    float       FontGlobalScale;                // = 1.0f           // Global scale all fonts
    bool        FontAllowUserScaling;           // = false          // Allow user scaling text of individual window with CTRL+Wheel.
    ImFont*     FontDefault;                    // = NULL           // Font to use on NewFrame(). Use NULL to uses Fonts->Fonts[0].
    ImVec2      DisplayFramebufferScale;        // = (1, 1)         // For retina display or other situations where window coordinates are different from framebuffer coordinates. This generally ends up in ImDrawData::FramebufferScale.

    // Miscellaneous options
    bool        MouseDrawCursor;                // = false          // Request Legion to draw a mouse cursor for you (if you are on a platform without a mouse cursor). Cannot be easily renamed to 'io.ConfigXXX' because this is frequently used by back-end implementations.
    bool        ConfigMacOSXBehaviors;          // = defined(__APPLE__) // OS X style: Text editing cursor movement using Alt instead of Ctrl, Shortcuts using Cmd/Super instead of Ctrl, Line/Text Start and End using Cmd+Arrows instead of Home/End, Double click selects by word instead of selecting whole text, Multi-selection in lists uses Cmd/Super instead of Ctrl (was called io.OptMacOSXBehaviors prior to 1.63)
    bool        ConfigInputTextCursorBlink;     // = true           // Set to false to disable blinking cursor, for users who consider it distracting. (was called: io.OptCursorBlink prior to 1.63)
    bool        ConfigWindowsResizeFromEdges;   // = true           // Enable resizing of windows from their edges and from the lower-left corner. This requires (io.BackendFlags & LegionBackendFlags_HasMouseCursors) because it needs mouse cursor feedback. (This used to be a per-window LegionWindowFlags_ResizeFromAnySide flag)
    bool        ConfigWindowsMoveFromTitleBarOnly; // = false       // [BETA] Set to true to only allow moving windows when clicked+dragged from the title bar. Windows without a title bar are not affected.
    float       ConfigWindowsMemoryCompactTimer;// = 60.0f          // [BETA] Compact window memory usage when unused. Set to -1.0f to disable.

    //------------------------------------------------------------------
    // Platform Functions
    // (the Legion_impl_xxxx back-end files are setting those up for you)
    //------------------------------------------------------------------

    // Optional: Platform/Renderer back-end name (informational only! will be displayed in About Window) + User data for back-end/wrappers to store their own stuff.
    const char* BackendPlatformName;            // = NULL
    const char* BackendRendererName;            // = NULL
    void*       BackendPlatformUserData;        // = NULL           // User data for platform back-end
    void*       BackendRendererUserData;        // = NULL           // User data for renderer back-end
    void*       BackendLanguageUserData;        // = NULL           // User data for non C++ programming language back-end

    // Optional: Access OS clipboard
    // (default to use native Win32 clipboard on Windows, otherwise uses a private clipboard. Override to access OS clipboard on other architectures)
    const char* (*GetClipboardTextFn)(void* user_data);
    void        (*SetClipboardTextFn)(void* user_data, const char* text);
    void*       ClipboardUserData;

    // Optional: Notify OS Input Method Editor of the screen position of your cursor for text input position (e.g. when using Japanese/Chinese IME on Windows)
    // (default to use native imm32 api on Windows)
    void        (*ImeSetInputScreenPosFn)(int x, int y);
    void*       ImeWindowHandle;                // = NULL           // (Windows) Set this to your HWND to get automatic IME cursor positioning.

#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    // [OBSOLETE since 1.60+] Rendering function, will be automatically called in Render(). Please call your rendering function yourself now!
    // You can obtain the ImDrawData* by calling Legion::GetDrawData() after Render(). See example applications if you are unsure of how to implement this.
    void        (*RenderDrawListsFn)(ImDrawData* data);
#else
    // This is only here to keep LegionIO the same size/layout, so that Legion_DISABLE_OBSOLETE_FUNCTIONS can exceptionally be used outside of imconfig.h.
    void*       RenderDrawListsFnUnused;
#endif

    //------------------------------------------------------------------
    // Input - Fill before calling NewFrame()
    //------------------------------------------------------------------

    ImVec2      MousePos;                       // Mouse position, in pixels. Set to ImVec2(-FLT_MAX,-FLT_MAX) if mouse is unavailable (on another screen, etc.)
    bool        MouseDown[5];                   // Mouse buttons: 0=left, 1=right, 2=middle + extras. Legion itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
    float       MouseWheel;                     // Mouse wheel Vertical: 1 unit scrolls about 5 lines text.
    float       MouseWheelH;                    // Mouse wheel Horizontal. Most users don't have a mouse with an horizontal wheel, may not be filled by all back-ends.
    bool        KeyCtrl;                        // Keyboard modifier pressed: Control
    bool        KeyShift;                       // Keyboard modifier pressed: Shift
    bool        KeyAlt;                         // Keyboard modifier pressed: Alt
    bool        KeySuper;                       // Keyboard modifier pressed: Cmd/Super/Windows
    bool        KeysDown[512];                  // Keyboard keys that are pressed (ideally left in the "native" order your engine has access to keyboard keys, so you can use your own defines/enums for keys).
    float       NavInputs[LegionNavInput_COUNT]; // Gamepad inputs. Cleared back to zero by EndFrame(). Keyboard keys will be auto-mapped and be written here by NewFrame().

    // Functions
    Legion_API void  AddInputCharacter(unsigned int c);          // Queue new character input
    Legion_API void  AddInputCharacterUTF16(ImWchar16 c);        // Queue new character input from an UTF-16 character, it can be a surrogate
    Legion_API void  AddInputCharactersUTF8(const char* str);    // Queue new characters input from an UTF-8 string
    Legion_API void  ClearInputCharacters();                     // Clear the text input buffer manually

    //------------------------------------------------------------------
    // Output - Updated by NewFrame() or EndFrame()/Render()
    // (when reading from the io.WantCaptureMouse, io.WantCaptureKeyboard flags to dispatch your inputs, it is
    //  generally easier and more correct to use their state BEFORE calling NewFrame(). See FAQ for details!)
    //------------------------------------------------------------------

    bool        WantCaptureMouse;               // Set when Dear Legion will use mouse inputs, in this case do not dispatch them to your main game/application (either way, always pass on mouse inputs to Legion). (e.g. unclicked mouse is hovering over an Legion window, widget is active, mouse was clicked over an Legion window, etc.).
    bool        WantCaptureKeyboard;            // Set when Dear Legion will use keyboard inputs, in this case do not dispatch them to your main game/application (either way, always pass keyboard inputs to Legion). (e.g. InputText active, or an Legion window is focused and navigation is enabled, etc.).
    bool        WantTextInput;                  // Mobile/console: when set, you may display an on-screen keyboard. This is set by Dear Legion when it wants textual keyboard input to happen (e.g. when a InputText widget is active).
    bool        WantSetMousePos;                // MousePos has been altered, back-end should reposition mouse on next frame. Rarely used! Set only when LegionConfigFlags_NavEnableSetMousePos flag is enabled.
    bool        WantSaveIniSettings;            // When manual .ini load/save is active (io.IniFilename == NULL), this will be set to notify your application that you can call SaveIniSettingsToMemory() and save yourself. Important: clear io.WantSaveIniSettings yourself after saving!
    bool        NavActive;                      // Keyboard/Gamepad navigation is currently allowed (will handle LegionKey_NavXXX events) = a window is focused and it doesn't use the LegionWindowFlags_NoNavInputs flag.
    bool        NavVisible;                     // Keyboard/Gamepad navigation is visible and allowed (will handle LegionKey_NavXXX events).
    float       Framerate;                      // Application framerate estimate, in frame per second. Solely for convenience. Rolling average estimation based on io.DeltaTime over 120 frames.
    int         MetricsRenderVertices;          // Vertices output during last call to Render()
    int         MetricsRenderIndices;           // Indices output during last call to Render() = number of triangles * 3
    int         MetricsRenderWindows;           // Number of visible windows
    int         MetricsActiveWindows;           // Number of active windows
    int         MetricsActiveAllocations;       // Number of active allocations, updated by MemAlloc/MemFree based on current context. May be off if you have multiple Legion contexts.
    ImVec2      MouseDelta;                     // Mouse delta. Note that this is zero if either current or previous position are invalid (-FLT_MAX,-FLT_MAX), so a disappearing/reappearing mouse won't have a huge delta.

    //------------------------------------------------------------------
    // [Internal] Dear Legion will maintain those fields. Forward compatibility not guaranteed!
    //------------------------------------------------------------------

    LegionKeyModFlags KeyMods;                   // Key mods flags (same as io.KeyCtrl/KeyShift/KeyAlt/KeySuper but merged into flags), updated by NewFrame()
    ImVec2      MousePosPrev;                   // Previous mouse position (note that MouseDelta is not necessary == MousePos-MousePosPrev, in case either position is invalid)
    ImVec2      MouseClickedPos[5];             // Position at time of clicking
    double      MouseClickedTime[5];            // Time of last click (used to figure out double-click)
    bool        MouseClicked[5];                // Mouse button went from !Down to Down
    bool        MouseDoubleClicked[5];          // Has mouse button been double-clicked?
    bool        MouseReleased[5];               // Mouse button went from Down to !Down
    bool        MouseDownOwned[5];              // Track if button was clicked inside a dear Legion window. We don't request mouse capture from the application if click started outside Legion bounds.
    bool        MouseDownWasDoubleClick[5];     // Track if button down was a double-click
    float       MouseDownDuration[5];           // Duration the mouse button has been down (0.0f == just clicked)
    float       MouseDownDurationPrev[5];       // Previous time the mouse button has been down
    ImVec2      MouseDragMaxDistanceAbs[5];     // Maximum distance, absolute, on each axis, of how much mouse has traveled from the clicking point
    float       MouseDragMaxDistanceSqr[5];     // Squared maximum distance of how much mouse has traveled from the clicking point
    float       KeysDownDuration[512];          // Duration the keyboard key has been down (0.0f == just pressed)
    float       KeysDownDurationPrev[512];      // Previous duration the key has been down
    float       NavInputsDownDuration[LegionNavInput_COUNT];
    float       NavInputsDownDurationPrev[LegionNavInput_COUNT];
    ImWchar16   InputQueueSurrogate;            // For AddInputCharacterUTF16
    ImVector<ImWchar> InputQueueCharacters;     // Queue of _characters_ input (obtained by platform back-end). Fill using AddInputCharacter() helper.

    Legion_API   LegionIO();
};

//-----------------------------------------------------------------------------
// Misc data structures
//-----------------------------------------------------------------------------

// Shared state of InputText(), passed as an argument to your callback when a LegionInputTextFlags_Callback* flag is used.
// The callback function should return 0 by default.
// Callbacks (follow a flag name and see comments in LegionInputTextFlags_ declarations for more details)
// - LegionInputTextFlags_CallbackCompletion:  Callback on pressing TAB
// - LegionInputTextFlags_CallbackHistory:     Callback on pressing Up/Down arrows
// - LegionInputTextFlags_CallbackAlways:      Callback on each iteration
// - LegionInputTextFlags_CallbackCharFilter:  Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
// - LegionInputTextFlags_CallbackResize:      Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow.
struct LegionInputTextCallbackData
{
    LegionInputTextFlags EventFlag;      // One LegionInputTextFlags_Callback*    // Read-only
    LegionInputTextFlags Flags;          // What user passed to InputText()      // Read-only
    void*               UserData;       // What user passed to InputText()      // Read-only

    // Arguments for the different callback events
    // - To modify the text buffer in a callback, prefer using the InsertChars() / DeleteChars() function. InsertChars() will take care of calling the resize callback if necessary.
    // - If you know your edits are not going to resize the underlying buffer allocation, you may modify the contents of 'Buf[]' directly. You need to update 'BufTextLen' accordingly (0 <= BufTextLen < BufSize) and set 'BufDirty'' to true so InputText can update its internal state.
    ImWchar             EventChar;      // Character input                      // Read-write   // [CharFilter] Replace character with another one, or set to zero to drop. return 1 is equivalent to setting EventChar=0;
    LegionKey            EventKey;       // Key pressed (Up/Down/TAB)            // Read-only    // [Completion,History]
    char*               Buf;            // Text buffer                          // Read-write   // [Resize] Can replace pointer / [Completion,History,Always] Only write to pointed data, don't replace the actual pointer!
    int                 BufTextLen;     // Text length (in bytes)               // Read-write   // [Resize,Completion,History,Always] Exclude zero-terminator storage. In C land: == strlen(some_text), in C++ land: string.length()
    int                 BufSize;        // Buffer size (in bytes) = capacity+1  // Read-only    // [Resize,Completion,History,Always] Include zero-terminator storage. In C land == ARRAYSIZE(my_char_array), in C++ land: string.capacity()+1
    bool                BufDirty;       // Set if you modify Buf/BufTextLen!    // Write        // [Completion,History,Always]
    int                 CursorPos;      //                                      // Read-write   // [Completion,History,Always]
    int                 SelectionStart; //                                      // Read-write   // [Completion,History,Always] == to SelectionEnd when no selection)
    int                 SelectionEnd;   //                                      // Read-write   // [Completion,History,Always]

    // Helper functions for text manipulation.
    // Use those function to benefit from the CallbackResize behaviors. Calling those function reset the selection.
    Legion_API LegionInputTextCallbackData();
    Legion_API void      DeleteChars(int pos, int bytes_count);
    Legion_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    bool                HasSelection() const { return SelectionStart != SelectionEnd; }
};

// Resizing callback data to apply custom constraint. As enabled by SetNextWindowSizeConstraints(). Callback is called during the next Begin().
// NB: For basic min/max size constraint on each axis you don't need to use the callback! The SetNextWindowSizeConstraints() parameters are enough.
struct LegionSizeCallbackData
{
    void*   UserData;       // Read-only.   What user passed to SetNextWindowSizeConstraints()
    ImVec2  Pos;            // Read-only.   Window position, for reference.
    ImVec2  CurrentSize;    // Read-only.   Current window size.
    ImVec2  DesiredSize;    // Read-write.  Desired size, based on user's mouse position. Write to this field to restrain resizing.
};

// Data payload for Drag and Drop operations: AcceptDragDropPayload(), GetDragDropPayload()
struct LegionPayload
{
    // Members
    void*           Data;               // Data (copied and owned by dear Legion)
    int             DataSize;           // Data size

    // [Internal]
    LegionID         SourceId;           // Source item id
    LegionID         SourceParentId;     // Source parent id (if available)
    int             DataFrameCount;     // Data timestamp
    char            DataType[32+1];     // Data type tag (short user-supplied string, 32 characters max)
    bool            Preview;            // Set when AcceptDragDropPayload() was called and mouse has been hovering the target item (nb: handle overlapping drag targets)
    bool            Delivery;           // Set when AcceptDragDropPayload() was called and mouse button is released over the target item.

    LegionPayload()  { Clear(); }
    void Clear()    { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
    bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
    bool IsPreview() const                  { return Preview; }
    bool IsDelivery() const                 { return Delivery; }
};

//-----------------------------------------------------------------------------
// Obsolete functions (Will be removed! Read 'API BREAKING CHANGES' section in Legion.cpp for details)
// Please keep your copy of dear Legion up to date! Occasionally set '#define Legion_DISABLE_OBSOLETE_FUNCTIONS' in imconfig.h to stay ahead.
//-----------------------------------------------------------------------------

#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
namespace Legion
{
    // OBSOLETED in 1.72 (from July 2019)
    static inline void  TreeAdvanceToLabelPos()               { SetCursorPosX(GetCursorPosX() + GetTreeNodeToLabelSpacing()); }
    // OBSOLETED in 1.71 (from June 2019)
    static inline void  SetNextTreeNodeOpen(bool open, LegionCond cond = 0) { SetNextItemOpen(open, cond); }
    // OBSOLETED in 1.70 (from May 2019)
    static inline float GetContentRegionAvailWidth()          { return GetContentRegionAvail().x; }
    // OBSOLETED in 1.69 (from Mar 2019)
    static inline ImDrawList* GetOverlayDrawList()            { return GetForegroundDrawList(); }
    // OBSOLETED in 1.66 (from Sep 2018)
    static inline void  SetScrollHere(float center_ratio=0.5f){ SetScrollHereY(center_ratio); }
    // OBSOLETED in 1.63 (between Aug 2018 and Sept 2018)
    static inline bool  IsItemDeactivatedAfterChange()        { return IsItemDeactivatedAfterEdit(); }
    // OBSOLETED in 1.61 (between Apr 2018 and Aug 2018)
    Legion_API bool      InputFloat(const char* label, float* v, float step, float step_fast, int decimal_precision, LegionInputTextFlags flags = 0); // Use the 'const char* format' version instead of 'decimal_precision'!
    Legion_API bool      InputFloat2(const char* label, float v[2], int decimal_precision, LegionInputTextFlags flags = 0);
    Legion_API bool      InputFloat3(const char* label, float v[3], int decimal_precision, LegionInputTextFlags flags = 0);
    Legion_API bool      InputFloat4(const char* label, float v[4], int decimal_precision, LegionInputTextFlags flags = 0);
    // OBSOLETED in 1.60 (between Dec 2017 and Apr 2018)
    static inline bool  IsAnyWindowFocused()                  { return IsWindowFocused(LegionFocusedFlags_AnyWindow); }
    static inline bool  IsAnyWindowHovered()                  { return IsWindowHovered(LegionHoveredFlags_AnyWindow); }
    static inline ImVec2 CalcItemRectClosestPoint(const ImVec2& pos, bool on_edge = false, float outward = 0.f) { IM_UNUSED(on_edge); IM_UNUSED(outward); IM_ASSERT(0); return pos; }
}
typedef LegionInputTextCallback      LegionTextEditCallback;    // OBSOLETED in 1.63 (from Aug 2018): made the names consistent
typedef LegionInputTextCallbackData  LegionTextEditCallbackData;
#endif

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

// Helper: Unicode defines
#define IM_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid Unicode code point (standard value).
#ifdef Legion_USE_WCHAR32
#define IM_UNICODE_CODEPOINT_MAX     0x10FFFF   // Maximum Unicode code point supported by this build.
#else
#define IM_UNICODE_CODEPOINT_MAX     0xFFFF     // Maximum Unicode code point supported by this build.
#endif

// Helper: Execute a block of code at maximum once a frame. Convenient if you want to quickly create an UI within deep-nested code that runs multiple times every frame.
// Usage: static LegionOnceUponAFrame oaf; if (oaf) Legion::Text("This will be called only once per frame");
struct LegionOnceUponAFrame
{
    LegionOnceUponAFrame() { RefFrame = -1; }
    mutable int RefFrame;
    operator bool() const { int current_frame = Legion::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};

// Helper: Parse and apply text filters. In format "aaaaa[,bbbb][,ccccc]"
struct LegionTextFilter
{
    Legion_API           LegionTextFilter(const char* default_filter = "");
    Legion_API bool      Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);  // Helper calling InputText+Build
    Legion_API bool      PassFilter(const char* text, const char* text_end = NULL) const;
    Legion_API void      Build();
    void                Clear()          { InputBuf[0] = 0; Build(); }
    bool                IsActive() const { return !Filters.empty(); }

    // [Internal]
    struct LegionTextRange
    {
        const char*     b;
        const char*     e;

        LegionTextRange()                                { b = e = NULL; }
        LegionTextRange(const char* _b, const char* _e)  { b = _b; e = _e; }
        bool            empty() const                   { return b == e; }
        Legion_API void  split(char separator, ImVector<LegionTextRange>* out) const;
    };
    char                    InputBuf[256];
    ImVector<LegionTextRange>Filters;
    int                     CountGrep;
};

// Helper: Growable text buffer for logging/accumulating text
// (this could be called 'LegionTextBuilder' / 'LegionStringBuilder')
struct LegionTextBuffer
{
    ImVector<char>      Buf;
    Legion_API static char EmptyString[1];

    LegionTextBuffer()   { }
    inline char         operator[](int i) const { IM_ASSERT(Buf.Data != NULL); return Buf.Data[i]; }
    const char*         begin() const           { return Buf.Data ? &Buf.front() : EmptyString; }
    const char*         end() const             { return Buf.Data ? &Buf.back() : EmptyString; }   // Buf is zero-terminated, so end() will point on the zero-terminator
    int                 size() const            { return Buf.Size ? Buf.Size - 1 : 0; }
    bool                empty() const           { return Buf.Size <= 1; }
    void                clear()                 { Buf.clear(); }
    void                reserve(int capacity)   { Buf.reserve(capacity); }
    const char*         c_str() const           { return Buf.Data ? Buf.Data : EmptyString; }
    Legion_API void      append(const char* str, const char* str_end = NULL);
    Legion_API void      appendf(const char* fmt, ...) IM_FMTARGS(2);
    Legion_API void      appendfv(const char* fmt, va_list args) IM_FMTLIST(2);
};

// Helper: Key->Value storage
// Typically you don't have to worry about this since a storage is held within each Window.
// We use it to e.g. store collapse state for a tree (Int 0/1)
// This is optimized for efficient lookup (dichotomy into a contiguous buffer) and rare insertion (typically tied to user interactions aka max once a frame)
// You can use it as custom user storage for temporary values. Declare your own storage if, for example:
// - You want to manipulate the open/close state of a particular sub-tree in your interface (tree node uses Int 0/1 to store their state).
// - You want to store custom debug data easily without adding or editing structures in your code (probably not efficient, but convenient)
// Types are NOT stored, so it is up to you to make sure your Key don't collide with different types.
struct LegionStorage
{
    // [Internal]
    struct LegionStoragePair
    {
        LegionID key;
        union { int val_i; float val_f; void* val_p; };
        LegionStoragePair(LegionID _key, int _val_i)      { key = _key; val_i = _val_i; }
        LegionStoragePair(LegionID _key, float _val_f)    { key = _key; val_f = _val_f; }
        LegionStoragePair(LegionID _key, void* _val_p)    { key = _key; val_p = _val_p; }
    };

    ImVector<LegionStoragePair>      Data;

    // - Get***() functions find pair, never add/allocate. Pairs are sorted so a query is O(log N)
    // - Set***() functions find pair, insertion on demand if missing.
    // - Sorted insertion is costly, paid once. A typical frame shouldn't need to insert any new pair.
    void                Clear() { Data.clear(); }
    Legion_API int       GetInt(LegionID key, int default_val = 0) const;
    Legion_API void      SetInt(LegionID key, int val);
    Legion_API bool      GetBool(LegionID key, bool default_val = false) const;
    Legion_API void      SetBool(LegionID key, bool val);
    Legion_API float     GetFloat(LegionID key, float default_val = 0.0f) const;
    Legion_API void      SetFloat(LegionID key, float val);
    Legion_API void*     GetVoidPtr(LegionID key) const; // default_val is NULL
    Legion_API void      SetVoidPtr(LegionID key, void* val);

    // - Get***Ref() functions finds pair, insert on demand if missing, return pointer. Useful if you intend to do Get+Set.
    // - References are only valid until a new value is added to the storage. Calling a Set***() function or a Get***Ref() function invalidates the pointer.
    // - A typical use case where this is convenient for quick hacking (e.g. add storage during a live Edit&Continue session if you can't modify existing struct)
    //      float* pvar = Legion::GetFloatRef(key); Legion::SliderFloat("var", pvar, 0, 100.0f); some_var += *pvar;
    Legion_API int*      GetIntRef(LegionID key, int default_val = 0);
    Legion_API bool*     GetBoolRef(LegionID key, bool default_val = false);
    Legion_API float*    GetFloatRef(LegionID key, float default_val = 0.0f);
    Legion_API void**    GetVoidPtrRef(LegionID key, void* default_val = NULL);

    // Use on your own storage if you know only integer are being stored (open/close all tree nodes)
    Legion_API void      SetAllInt(int val);

    // For quicker full rebuild of a storage (instead of an incremental one), you may add all your contents and then sort once.
    Legion_API void      BuildSortByKey();
};

// Helper: Manually clip large list of items.
// If you are submitting lots of evenly spaced items and you have a random access to the list, you can perform coarse clipping based on visibility to save yourself from processing those items at all.
// The clipper calculates the range of visible items and advance the cursor to compensate for the non-visible items we have skipped.
// Legion already clip items based on their bounds but it needs to measure text size to do so. Coarse clipping before submission makes this cost and your own data fetching/submission cost null.
// Usage:
//     LegionListClipper clipper(1000);  // we have 1000 elements, evenly spaced.
//     while (clipper.Step())
//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
//             Legion::Text("line number %d", i);
// - Step 0: the clipper let you process the first element, regardless of it being visible or not, so we can measure the element height (step skipped if we passed a known height as second arg to constructor).
// - Step 1: the clipper infer height from first element, calculate the actual range of elements to display, and position the cursor before the first element.
// - (Step 2: dummy step only required if an explicit items_height was passed to constructor or Begin() and user call Step(). Does nothing and switch to Step 3.)
// - Step 3: the clipper validate that we have reached the expected Y position (corresponding to element DisplayEnd), advance the cursor to the end of the list and then returns 'false' to end the loop.
struct LegionListClipper
{
    int     DisplayStart, DisplayEnd;
    int     ItemsCount;

    // [Internal]
    int     StepNo;
    float   ItemsHeight;
    float   StartPosY;

    // items_count:  Use -1 to ignore (you can call Begin later). Use INT_MAX if you don't know how many items you have (in which case the cursor won't be advanced in the final step).
    // items_height: Use -1.0f to be calculated automatically on first step. Otherwise pass in the distance between your items, typically GetTextLineHeightWithSpacing() or GetFrameHeightWithSpacing().
    // If you don't specify an items_height, you NEED to call Step(). If you specify items_height you may call the old Begin()/End() api directly, but prefer calling Step().
    LegionListClipper(int items_count = -1, float items_height = -1.0f)  { Begin(items_count, items_height); } // NB: Begin() initialize every fields (as we allow user to call Begin/End multiple times on a same instance if they want).
    ~LegionListClipper()                                                 { IM_ASSERT(ItemsCount == -1); }      // Assert if user forgot to call End() or Step() until false.

    Legion_API bool Step();                                              // Call until it returns false. The DisplayStart/DisplayEnd fields will be set and you can process/draw those items.
    Legion_API void Begin(int items_count, float items_height = -1.0f);  // Automatically called by constructor if you passed 'items_count' or by Step() in Step 1.
    Legion_API void End();                                               // Automatically called on the last call of Step() that returns false.
};

// Helpers macros to generate 32-bit encoded colors
#ifdef Legion_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT    16
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    0
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#else
#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#endif
#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255,255,255,255)  // Opaque white = 0xFFFFFFFF
#define IM_COL32_BLACK       IM_COL32(0,0,0,255)        // Opaque black
#define IM_COL32_BLACK_TRANS IM_COL32(0,0,0,0)          // Transparent black = 0x00000000

// Helper: ImColor() implicitly converts colors to either ImU32 (packed 4x1 byte) or ImVec4 (4x1 float)
// Prefer using IM_COL32() macros if you want a guaranteed compile-time ImU32 for usage with ImDrawList API.
// **Avoid storing ImColor! Store either u32 of ImVec4. This is not a full-featured color class. MAY OBSOLETE.
// **None of the Legion API are using ImColor directly but you can use it as a convenience to pass colors in either ImU32 or ImVec4 formats. Explicitly cast to ImU32 or ImVec4 if needed.
struct ImColor
{
    ImVec4              Value;

    ImColor()                                                       { Value.x = Value.y = Value.z = Value.w = 0.0f; }
    ImColor(int r, int g, int b, int a = 255)                       { float sc = 1.0f/255.0f; Value.x = (float)r * sc; Value.y = (float)g * sc; Value.z = (float)b * sc; Value.w = (float)a * sc; }
    ImColor(ImU32 rgba)                                             { float sc = 1.0f/255.0f; Value.x = (float)((rgba>>IM_COL32_R_SHIFT)&0xFF) * sc; Value.y = (float)((rgba>>IM_COL32_G_SHIFT)&0xFF) * sc; Value.z = (float)((rgba>>IM_COL32_B_SHIFT)&0xFF) * sc; Value.w = (float)((rgba>>IM_COL32_A_SHIFT)&0xFF) * sc; }
    ImColor(float r, float g, float b, float a = 1.0f)              { Value.x = r; Value.y = g; Value.z = b; Value.w = a; }
    ImColor(const ImVec4& col)                                      { Value = col; }
    inline operator ImU32() const                                   { return Legion::ColorConvertFloat4ToU32(Value); }
    inline operator ImVec4() const                                  { return Value; }

    // FIXME-OBSOLETE: May need to obsolete/cleanup those helpers.
    inline void    SetHSV(float h, float s, float v, float a = 1.0f){ Legion::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }
    static ImColor HSV(float h, float s, float v, float a = 1.0f)   { float r,g,b; Legion::ColorConvertHSVtoRGB(h, s, v, r, g, b); return ImColor(r,g,b,a); }
};

//-----------------------------------------------------------------------------
// Draw List API (ImDrawCmd, ImDrawIdx, ImDrawVert, ImDrawChannel, ImDrawListSplitter, ImDrawListFlags, ImDrawList, ImDrawData)
// Hold a series of drawing commands. The user provides a renderer for ImDrawData which essentially contains an array of ImDrawList.
//-----------------------------------------------------------------------------

// ImDrawCallback: Draw callbacks for advanced uses [configurable type: override in imconfig.h]
// NB: You most likely do NOT need to use draw callbacks just to create your own widget or customized UI rendering,
// you can poke into the draw list for that! Draw callback may be useful for example to:
//  A) Change your GPU render state,
//  B) render a complex 3D scene inside a UI element without an intermediate texture/render target, etc.
// The expected behavior from your rendering function is 'if (cmd.UserCallback != NULL) { cmd.UserCallback(parent_list, cmd); } else { RenderTriangles() }'
// If you want to override the signature of ImDrawCallback, you can simply use e.g. '#define ImDrawCallback MyDrawCallback' (in imconfig.h) + update rendering back-end accordingly.
#ifndef ImDrawCallback
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
#endif

// Special Draw callback value to request renderer back-end to reset the graphics/render state.
// The renderer back-end needs to handle this special value, otherwise it will crash trying to call a function at this address.
// This is useful for example if you submitted callbacks which you know have altered the render state and you want it to be restored.
// It is not done by default because they are many perfectly useful way of altering render state for Legion contents (e.g. changing shader/blending settings before an Image call).
#define ImDrawCallback_ResetRenderState     (ImDrawCallback)(-1)

// Typically, 1 command = 1 GPU draw call (unless command is a callback)
// Pre 1.71 back-ends will typically ignore the VtxOffset/IdxOffset fields. When 'io.BackendFlags & LegionBackendFlags_RendererHasVtxOffset'
// is enabled, those fields allow us to render meshes larger than 64K vertices while keeping 16-bit indices.
struct ImDrawCmd
{
    unsigned int    ElemCount;              // Number of indices (multiple of 3) to be rendered as triangles. Vertices are stored in the callee ImDrawList's vtx_buffer[] array, indices in idx_buffer[].
    ImVec4          ClipRect;               // Clipping rectangle (x1, y1, x2, y2). Subtract ImDrawData->DisplayPos to get clipping rectangle in "viewport" coordinates
    ImTextureID     TextureId;              // User-provided texture ID. Set by user in ImfontAtlas::SetTexID() for fonts or passed to Image*() functions. Ignore if never using images or multiple fonts atlas.
    unsigned int    VtxOffset;              // Start offset in vertex buffer. Pre-1.71 or without LegionBackendFlags_RendererHasVtxOffset: always 0. With LegionBackendFlags_RendererHasVtxOffset: may be >0 to support meshes larger than 64K vertices with 16-bit indices.
    unsigned int    IdxOffset;              // Start offset in index buffer. Always equal to sum of ElemCount drawn so far.
    ImDrawCallback  UserCallback;           // If != NULL, call the function instead of rendering the vertices. clip_rect and texture_id will be set normally.
    void*           UserCallbackData;       // The draw callback code can access this.

    ImDrawCmd() { ElemCount = 0; TextureId = (ImTextureID)NULL; VtxOffset = IdxOffset = 0;  UserCallback = NULL; UserCallbackData = NULL; }
};

// Vertex index, default to 16-bit
// To allow large meshes with 16-bit indices: set 'io.BackendFlags |= LegionBackendFlags_RendererHasVtxOffset' and handle ImDrawCmd::VtxOffset in the renderer back-end (recommended).
// To use 32-bit indices: override with '#define ImDrawIdx unsigned int' in imconfig.h.
#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;
#endif

// Vertex layout
#ifndef Legion_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
    ImVec2  pos;
    ImVec2  uv;
    ImU32   col;
};
#else
// You can override the vertex format layout by defining Legion_OVERRIDE_DRAWVERT_STRUCT_LAYOUT in imconfig.h
// The code expect ImVec2 pos (8 bytes), ImVec2 uv (8 bytes), ImU32 col (4 bytes), but you can re-order them or add other fields as needed to simplify integration in your engine.
// The type has to be described within the macro (you can either declare the struct or use a typedef). This is because ImVec2/ImU32 are likely not declared a the time you'd want to set your type up.
// NOTE: Legion DOESN'T CLEAR THE STRUCTURE AND DOESN'T CALL A CONSTRUCTOR SO ANY CUSTOM FIELD WILL BE UNINITIALIZED. IF YOU ADD EXTRA FIELDS (SUCH AS A 'Z' COORDINATES) YOU WILL NEED TO CLEAR THEM DURING RENDER OR TO IGNORE THEM.
Legion_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

// For use by ImDrawListSplitter.
struct ImDrawChannel
{
    ImVector<ImDrawCmd>         _CmdBuffer;
    ImVector<ImDrawIdx>         _IdxBuffer;
};

// Split/Merge functions are used to split the draw list into different layers which can be drawn into out of order.
// This is used by the Columns api, so items of each column can be batched together in a same draw call.
struct ImDrawListSplitter
{
    int                         _Current;    // Current channel number (0)
    int                         _Count;      // Number of active channels (1+)
    ImVector<ImDrawChannel>     _Channels;   // Draw channels (not resized down so _Count might be < Channels.Size)

    inline ImDrawListSplitter()  { Clear(); }
    inline ~ImDrawListSplitter() { ClearFreeMemory(); }
    inline void                 Clear() { _Current = 0; _Count = 1; } // Do not clear Channels[] so our allocations are reused next frame
    Legion_API void              ClearFreeMemory();
    Legion_API void              Split(ImDrawList* draw_list, int count);
    Legion_API void              Merge(ImDrawList* draw_list);
    Legion_API void              SetCurrentChannel(ImDrawList* draw_list, int channel_idx);
};

enum ImDrawCornerFlags_
{
    ImDrawCornerFlags_None      = 0,
    ImDrawCornerFlags_TopLeft   = 1 << 0, // 0x1
    ImDrawCornerFlags_TopRight  = 1 << 1, // 0x2
    ImDrawCornerFlags_BotLeft   = 1 << 2, // 0x4
    ImDrawCornerFlags_BotRight  = 1 << 3, // 0x8
    ImDrawCornerFlags_Top       = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,   // 0x3
    ImDrawCornerFlags_Bot       = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,   // 0xC
    ImDrawCornerFlags_Left      = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,    // 0x5
    ImDrawCornerFlags_Right     = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight,  // 0xA
    ImDrawCornerFlags_All       = 0xF     // In your function calls you may use ~0 (= all bits sets) instead of ImDrawCornerFlags_All, as a convenience
};

enum ImDrawListFlags_
{
    ImDrawListFlags_None             = 0,
    ImDrawListFlags_AntiAliasedLines = 1 << 0,  // Lines are anti-aliased (*2 the number of triangles for 1.0f wide line, otherwise *3 the number of triangles)
    ImDrawListFlags_AntiAliasedFill  = 1 << 1,  // Filled shapes have anti-aliased edges (*2 the number of vertices)
    ImDrawListFlags_AllowVtxOffset   = 1 << 2   // Can emit 'VtxOffset > 0' to allow large meshes. Set when 'LegionBackendFlags_RendererHasVtxOffset' is enabled.
};

// Draw command list
// This is the low-level list of polygons that Legion:: functions are filling. At the end of the frame,
// all command lists are passed to your LegionIO::RenderDrawListFn function for rendering.
// Each dear Legion window contains its own ImDrawList. You can use Legion::GetWindowDrawList() to
// access the current window draw list and draw custom primitives.
// You can interleave normal Legion:: calls and adding primitives to the current draw list.
// All positions are generally in pixel coordinates (top-left at (0,0), bottom-right at io.DisplaySize), but you are totally free to apply whatever transformation matrix to want to the data (if you apply such transformation you'll want to apply it to ClipRect as well)
// Important: Primitives are always added to the list and not culled (culling is done at higher-level by Legion:: functions), if you use this API a lot consider coarse culling your drawn objects.
struct ImDrawList
{
    // This is what you have to render
    ImVector<ImDrawCmd>     CmdBuffer;          // Draw commands. Typically 1 command = 1 GPU draw call, unless the command is a callback.
    ImVector<ImDrawIdx>     IdxBuffer;          // Index buffer. Each command consume ImDrawCmd::ElemCount of those
    ImVector<ImDrawVert>    VtxBuffer;          // Vertex buffer.
    ImDrawListFlags         Flags;              // Flags, you may poke into these to adjust anti-aliasing settings per-primitive.

    // [Internal, used while building lists]
    const ImDrawListSharedData* _Data;          // Pointer to shared draw data (you can use Legion::GetDrawListSharedData() to get the one from current Legion context)
    const char*             _OwnerName;         // Pointer to owner window's name for debugging
    unsigned int            _VtxCurrentOffset;  // [Internal] Always 0 unless 'Flags & ImDrawListFlags_AllowVtxOffset'.
    unsigned int            _VtxCurrentIdx;     // [Internal] Generally == VtxBuffer.Size unless we are past 64K vertices, in which case this gets reset to 0.
    ImDrawVert*             _VtxWritePtr;       // [Internal] point within VtxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
    ImDrawIdx*              _IdxWritePtr;       // [Internal] point within IdxBuffer.Data after each add command (to avoid using the ImVector<> operators too much)
    ImVector<ImVec4>        _ClipRectStack;     // [Internal]
    ImVector<ImTextureID>   _TextureIdStack;    // [Internal]
    ImVector<ImVec2>        _Path;              // [Internal] current path building
    ImDrawListSplitter      _Splitter;          // [Internal] for channels api

    // If you want to create ImDrawList instances, pass them Legion::GetDrawListSharedData() or create and use your own ImDrawListSharedData (so you can use ImDrawList without Legion)
    ImDrawList(const ImDrawListSharedData* shared_data) { _Data = shared_data; _OwnerName = NULL; Clear(); }
    ~ImDrawList() { ClearFreeMemory(); }
    Legion_API void  PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false);  // Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level Legion::PushClipRect() to affect logic (hit-testing and widget culling)
    Legion_API void  PushClipRectFullScreen();
    Legion_API void  PopClipRect();
    Legion_API void  PushTextureID(ImTextureID texture_id);
    Legion_API void  PopTextureID();
    inline ImVec2   GetClipRectMin() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.x, cr.y); }
    inline ImVec2   GetClipRectMax() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.z, cr.w); }

    // Primitives
    // - For rectangular primitives, "p_min" and "p_max" represent the upper-left and lower-right corners.
    // - For circle primitives, use "num_segments == 0" to automatically calculate tessellation (preferred).
    //   In future versions we will use textures to provide cheaper and higher-quality circles.
    //   Use AddNgon() and AddNgonFilled() functions if you need to guaranteed a specific number of sides.
    Legion_API void  AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);
    Legion_API void  AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All, float thickness = 1.0f);   // a: upper-left, b: lower-right (== upper-left + size), rounding_corners_flags: 4 bits corresponding to which corner to round
    Legion_API void  AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);                     // a: upper-left, b: lower-right (== upper-left + size)
    Legion_API void  AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    Legion_API void  AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f);
    Legion_API void  AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col);
    Legion_API void  AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
    Legion_API void  AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);
    Legion_API void  AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
    Legion_API void  AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 12);
    Legion_API void  AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f);
    Legion_API void  AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments);
    Legion_API void  AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
    Legion_API void  AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
    Legion_API void  AddPolyline(const ImVec2* points, int num_points, ImU32 col, bool closed, float thickness);
    Legion_API void  AddConvexPolyFilled(const ImVec2* points, int num_points, ImU32 col); // Note: Anti-aliased filling requires points to be in clockwise order.
    Legion_API void  AddBezierCurve(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0);

    // Image primitives
    // - Read FAQ to understand what ImTextureID is.
    // - "p_min" and "p_max" represent the upper-left and lower-right corners of the rectangle.
    // - "uv_min" and "uv_max" represent the normalized texture coordinates to use for those corners. Using (0,0)->(1,1) texture coordinates will generally display the entire texture.
    Legion_API void  AddImage(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE);
    Legion_API void  AddImageQuad(ImTextureID user_texture_id, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1 = ImVec2(0, 0), const ImVec2& uv2 = ImVec2(1, 0), const ImVec2& uv3 = ImVec2(1, 1), const ImVec2& uv4 = ImVec2(0, 1), ImU32 col = IM_COL32_WHITE);
    Legion_API void  AddImageRounded(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);

    // Stateful path API, add points then finish with PathFillConvex() or PathStroke()
    inline    void  PathClear()                                                 { _Path.Size = 0; }
    inline    void  PathLineTo(const ImVec2& pos)                               { _Path.push_back(pos); }
    inline    void  PathLineToMergeDuplicate(const ImVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size-1], &pos, 8) != 0) _Path.push_back(pos); }
    inline    void  PathFillConvex(ImU32 col)                                   { AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }  // Note: Anti-aliased filling requires points to be in clockwise order.
    inline    void  PathStroke(ImU32 col, bool closed, float thickness = 1.0f)  { AddPolyline(_Path.Data, _Path.Size, col, closed, thickness); _Path.Size = 0; }
    Legion_API void  PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments = 10);
    Legion_API void  PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12);                                            // Use precomputed angles for a 12 steps circle
    Legion_API void  PathBezierCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0);
    Legion_API void  PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All);

    // Advanced
    Legion_API void  AddCallback(ImDrawCallback callback, void* callback_data);  // Your rendering function must check for 'UserCallback' in ImDrawCmd and call the function instead of rendering triangles.
    Legion_API void  AddDrawCmd();                                               // This is useful if you need to forcefully create a new draw call (to allow for dependent rendering / blending). Otherwise primitives are merged into the same draw-call as much as possible
    Legion_API ImDrawList* CloneOutput() const;                                  // Create a clone of the CmdBuffer/IdxBuffer/VtxBuffer.

    // Advanced: Channels
    // - Use to split render into layers. By switching channels to can render out-of-order (e.g. submit FG primitives before BG primitives)
    // - Use to minimize draw calls (e.g. if going back-and-forth between multiple clipping rectangles, prefer to append into separate channels then merge at the end)
    // - FIXME-OBSOLETE: This API shouldn't have been in ImDrawList in the first place!
    //   Prefer using your own persistent copy of ImDrawListSplitter as you can stack them.
    //   Using the ImDrawList::ChannelsXXXX you cannot stack a split over another.
    inline void     ChannelsSplit(int count)    { _Splitter.Split(this, count); }
    inline void     ChannelsMerge()             { _Splitter.Merge(this); }
    inline void     ChannelsSetCurrent(int n)   { _Splitter.SetCurrentChannel(this, n); }

    // Internal helpers
    // NB: all primitives needs to be reserved via PrimReserve() beforehand!
    Legion_API void  Clear();
    Legion_API void  ClearFreeMemory();
    Legion_API void  PrimReserve(int idx_count, int vtx_count);
    Legion_API void  PrimUnreserve(int idx_count, int vtx_count);
    Legion_API void  PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);      // Axis aligned rectangle (composed of two triangles)
    Legion_API void  PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
    Legion_API void  PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
    inline    void  PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col){ _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
    inline    void  PrimWriteIdx(ImDrawIdx idx)                                 { *_IdxWritePtr = idx; _IdxWritePtr++; }
    inline    void  PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)     { PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); }
    Legion_API void  UpdateClipRect();
    Legion_API void  UpdateTextureID();
};

// All draw data to render a Dear Legion frame
// (NB: the style and the naming convention here is a little inconsistent, we currently preserve them for backward compatibility purpose,
// as this is one of the oldest structure exposed by the library! Basically, ImDrawList == CmdList)
struct ImDrawData
{
    bool            Valid;                  // Only valid after Render() is called and before the next NewFrame() is called.
    ImDrawList**    CmdLists;               // Array of ImDrawList* to render. The ImDrawList are owned by LegionContext and only pointed to from here.
    int             CmdListsCount;          // Number of ImDrawList* to render
    int             TotalIdxCount;          // For convenience, sum of all ImDrawList's IdxBuffer.Size
    int             TotalVtxCount;          // For convenience, sum of all ImDrawList's VtxBuffer.Size
    ImVec2          DisplayPos;             // Upper-left position of the viewport to render (== upper-left of the orthogonal projection matrix to use)
    ImVec2          DisplaySize;            // Size of the viewport to render (== io.DisplaySize for the main viewport) (DisplayPos + DisplaySize == lower-right of the orthogonal projection matrix to use)
    ImVec2          FramebufferScale;       // Amount of pixels for each unit of DisplaySize. Based on io.DisplayFramebufferScale. Generally (1,1) on normal display, (2,2) on OSX with Retina display.

    // Functions
    ImDrawData()    { Valid = false; Clear(); }
    ~ImDrawData()   { Clear(); }
    void Clear()    { Valid = false; CmdLists = NULL; CmdListsCount = TotalVtxCount = TotalIdxCount = 0; DisplayPos = DisplaySize = FramebufferScale = ImVec2(0.f, 0.f); } // The ImDrawList are owned by LegionContext!
    Legion_API void  DeIndexAllBuffers();                    // Helper to convert all buffers from indexed to non-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
    Legion_API void  ScaleClipRects(const ImVec2& fb_scale); // Helper to scale the ClipRect field of each ImDrawCmd. Use if your final output buffer is at a different scale than Dear Legion expects, or if there is a difference between your window resolution and framebuffer resolution.
};

//-----------------------------------------------------------------------------
// Font API (ImFontConfig, ImFontGlyph, ImFontAtlasFlags, ImFontAtlas, ImFontGlyphRangesBuilder, ImFont)
//-----------------------------------------------------------------------------

struct ImFontConfig
{
    void*           FontData;               //          // TTF/OTF data
    int             FontDataSize;           //          // TTF/OTF data size
    bool            FontDataOwnedByAtlas;   // true     // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
    int             FontNo;                 // 0        // Index of font within TTF/OTF file
    float           SizePixels;             //          // Size in pixels for rasterizer (more or less maps to the resulting font height).
    int             OversampleH;            // 3        // Rasterize at higher quality for sub-pixel positioning. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
    int             OversampleV;            // 1        // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;             // false    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
    ImVec2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
    ImVec2          GlyphOffset;            // 0, 0     // Offset all glyphs from this font input.
    const ImWchar*  GlyphRanges;            // NULL     // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
    float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
    float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
    bool            MergeMode;              // false    // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
    unsigned int    RasterizerFlags;        // 0x00     // Settings for custom font rasterizer (e.g. LegionFreeType). Leave as zero if you aren't using one.
    float           RasterizerMultiply;     // 1.0f     // Brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable.
    ImWchar         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

    // [Internal]
    char            Name[40];               // Name (strictly to ease debugging)
    ImFont*         DstFont;

    Legion_API ImFontConfig();
};

// Hold rendering data for one glyph.
// (Note: some language parsers may fail to convert the 31+1 bitfield members, in this case maybe drop store a single u32 or we can rework this)
struct ImFontGlyph
{
    unsigned int    Codepoint : 31;     // 0x0000..0xFFFF
    unsigned int    Visible : 1;        // Flag to allow early out when rendering
    float           AdvanceX;           // Distance to next character (= data from font + ImFontConfig::GlyphExtraSpacing.x baked in)
    float           X0, Y0, X1, Y1;     // Glyph corners
    float           U0, V0, U1, V1;     // Texture coordinates
};

// Helper to build glyph ranges from text/string data. Feed your application strings/characters to it then call BuildRanges().
// This is essentially a tightly packed of vector of 64k booleans = 8KB storage.
struct ImFontGlyphRangesBuilder
{
    ImVector<ImU32> UsedChars;            // Store 1-bit per Unicode code point (0=unused, 1=used)

    ImFontGlyphRangesBuilder()              { Clear(); }
    inline void     Clear()                 { int size_in_bytes = (IM_UNICODE_CODEPOINT_MAX + 1) / 8; UsedChars.resize(size_in_bytes / (int)sizeof(ImU32)); memset(UsedChars.Data, 0, (size_t)size_in_bytes); }
    inline bool     GetBit(size_t n) const  { int off = (int)(n >> 5); ImU32 mask = 1u << (n & 31); return (UsedChars[off] & mask) != 0; }  // Get bit n in the array
    inline void     SetBit(size_t n)        { int off = (int)(n >> 5); ImU32 mask = 1u << (n & 31); UsedChars[off] |= mask; }               // Set bit n in the array
    inline void     AddChar(ImWchar c)      { SetBit(c); }                      // Add character
    Legion_API void  AddText(const char* text, const char* text_end = NULL);     // Add string (each character of the UTF-8 string are added)
    Legion_API void  AddRanges(const ImWchar* ranges);                           // Add ranges, e.g. builder.AddRanges(ImFontAtlas::GetGlyphRangesDefault()) to force add all of ASCII/Latin+Ext
    Legion_API void  BuildRanges(ImVector<ImWchar>* out_ranges);                 // Output new ranges
};

// See ImFontAtlas::AddCustomRectXXX functions.
struct ImFontAtlasCustomRect
{
    unsigned int    ID;             // Input    // User ID. Use < 0x110000 to map into a font glyph, >= 0x110000 for other/internal/custom texture data.
    unsigned short  Width, Height;  // Input    // Desired rectangle dimension
    unsigned short  X, Y;           // Output   // Packed position in Atlas
    float           GlyphAdvanceX;  // Input    // For custom font glyphs only (ID < 0x110000): glyph xadvance
    ImVec2          GlyphOffset;    // Input    // For custom font glyphs only (ID < 0x110000): glyph display offset
    ImFont*         Font;           // Input    // For custom font glyphs only (ID < 0x110000): target font
    ImFontAtlasCustomRect()         { ID = 0xFFFFFFFF; Width = Height = 0; X = Y = 0xFFFF; GlyphAdvanceX = 0.0f; GlyphOffset = ImVec2(0,0); Font = NULL; }
    bool IsPacked() const           { return X != 0xFFFF; }
};

enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1    // Don't build software mouse cursors into the atlas
};

// Load and rasterize multiple TTF/OTF fonts into a same texture. The font atlas will build a single texture holding:
//  - One or more fonts.
//  - Custom graphics data needed to render the shapes needed by Dear Legion.
//  - Mouse cursor shapes for software cursor rendering (unless setting 'Flags |= ImFontAtlasFlags_NoMouseCursors' in the font atlas).
// It is the user-code responsibility to setup/build the atlas, then upload the pixel data into a texture accessible by your graphics api.
//  - Optionally, call any of the AddFont*** functions. If you don't call any, the default font embedded in the code will be loaded for you.
//  - Call GetTexDataAsAlpha8() or GetTexDataAsRGBA32() to build and retrieve pixels data.
//  - Upload the pixels data into a texture within your graphics system (see Legion_impl_xxxx.cpp examples)
//  - Call SetTexID(my_tex_id); and pass the pointer/identifier to your texture in a format natural to your graphics API.
//    This value will be passed back to you during rendering to identify the texture. Read FAQ entry about ImTextureID for more details.
// Common pitfalls:
// - If you pass a 'glyph_ranges' array to AddFont*** functions, you need to make sure that your array persist up until the
//   atlas is build (when calling GetTexData*** or Build()). We only copy the pointer, not the data.
// - Important: By default, AddFontFromMemoryTTF() takes ownership of the data. Even though we are not writing to it, we will free the pointer on destruction.
//   You can set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed,
// - Even though many functions are suffixed with "TTF", OTF data is supported just as well.
// - This is an old API and it is currently awkward for those and and various other reasons! We will address them in the future!
struct ImFontAtlas
{
    Legion_API ImFontAtlas();
    Legion_API ~ImFontAtlas();
    Legion_API ImFont*           AddFont(const ImFontConfig* font_cfg);
    Legion_API ImFont*           AddFontDefault(const ImFontConfig* font_cfg = NULL);
    Legion_API ImFont*           AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    Legion_API ImFont*           AddFontFromFileTTFV(const char* filename, float size_pixels, unsigned int style, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    Legion_API ImFont*           AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to ImFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    Legion_API ImFont*           AddFontFromMemoryTTFV(void* font_data, int font_size, float size_pixels, unsigned int style, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Note: Transfer ownership of 'ttf_data' to ImFontAtlas! Will be deleted after destruction of the atlas. Set font_cfg->FontDataOwnedByAtlas=false to keep ownership of your data and it won't be freed.
    Legion_API ImFont*           AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // 'compressed_font_data' still owned by caller. Compress with binary_to_compressed_c.cpp.
    Legion_API ImFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);              // 'compressed_font_data_base85' still owned by caller. Compress with binary_to_compressed_c.cpp with -base85 parameter.
    Legion_API void              ClearInputData();           // Clear input data (all ImFontConfig structures including sizes, TTF data, glyph ranges, etc.) = all the data used to build the texture and fonts.
    Legion_API void              ClearTexData();             // Clear output texture data (CPU side). Saves RAM once the texture has been copied to graphics memory.
    Legion_API void              ClearFonts();               // Clear output font data (glyphs storage, UV coordinates).
    Legion_API void              Clear();                    // Clear all input and output.

    // Build atlas, retrieve pixel data.
    // User is in charge of copying the pixels into graphics memory (e.g. create a texture with your engine). Then store your texture handle with SetTexID().
    // The pitch is always = Width * BytesPerPixels (1 or 4)
    // Building in RGBA32 format is provided for convenience and compatibility, but note that unless you manually manipulate or copy color data into
    // the texture (e.g. when using the AddCustomRect*** api), then the RGB pixels emitted will always be white (~75% of memory/bandwidth waste.
    Legion_API bool              Build();                    // Build pixels data. This is called automatically for you by the GetTexData*** functions.
    Legion_API void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 1 byte per-pixel
    Legion_API void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 4 bytes-per-pixel
    bool                        IsBuilt() const             { return Fonts.Size > 0 && (TexPixelsAlpha8 != NULL || TexPixelsRGBA32 != NULL); }
    void                        SetTexID(ImTextureID id)    { TexID = id; }

    //-------------------------------------------
    // Glyph Ranges
    //-------------------------------------------

    // Helpers to retrieve list of common Unicode ranges (2 value per range, values are inclusive, zero-terminated list)
    // NB: Make sure that your string are UTF-8 and NOT in your local code page. In C++11, you can create UTF-8 string literal using the u8"Hello world" syntax. See FAQ for details.
    // NB: Consider using ImFontGlyphRangesBuilder to build glyph ranges from textual data.
    Legion_API const ImWchar*    GetGlyphRangesDefault();                // Basic Latin, Extended Latin
    Legion_API const ImWchar*    GetGlyphRangesKorean();                 // Default + Korean characters
    Legion_API const ImWchar*    GetGlyphRangesJapanese();               // Default + Hiragana, Katakana, Half-Width, Selection of 1946 Ideographs
    Legion_API const ImWchar*    GetGlyphRangesChineseFull();            // Default + Half-Width + Japanese Hiragana/Katakana + full set of about 21000 CJK Unified Ideographs
    Legion_API const ImWchar*    GetGlyphRangesChineseSimplifiedCommon();// Default + Half-Width + Japanese Hiragana/Katakana + set of 2500 CJK Unified Ideographs for common simplified Chinese
    Legion_API const ImWchar*    GetGlyphRangesCyrillic();               // Default + about 400 Cyrillic characters
    Legion_API const ImWchar*    GetGlyphRangesThai();                   // Default + Thai characters
    Legion_API const ImWchar*    GetGlyphRangesVietnamese();             // Default + Vietnamese characters

    //-------------------------------------------
    // [BETA] Custom Rectangles/Glyphs API
    //-------------------------------------------

    // You can request arbitrary rectangles to be packed into the atlas, for your own purposes.
    // After calling Build(), you can query the rectangle position and render your pixels.
    // You can also request your rectangles to be mapped as font glyph (given a font + Unicode point),
    // so you can render e.g. custom colorful icons and use them as regular glyphs.
    // Read docs/FONTS.txt for more details about using colorful icons.
    Legion_API int               AddCustomRectRegular(unsigned int id, int width, int height);                                                                   // Id needs to be >= 0x110000. Id >= 0x80000000 are reserved for Legion and ImDrawList
    Legion_API int               AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0,0));   // Id needs to be < 0x110000 to register a rectangle to map into a specific font.
    const ImFontAtlasCustomRect*GetCustomRectByIndex(int index) const { if (index < 0) return NULL; return &CustomRects[index]; }

    // [Internal]
    Legion_API void              CalcCustomRectUV(const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) const;
    Legion_API bool              GetMouseCursorTexData(LegionMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]);

    //-------------------------------------------
    // Members
    //-------------------------------------------

    bool                        Locked;             // Marked as Locked by Legion::NewFrame() so attempt to modify the atlas will assert.
    ImFontAtlasFlags            Flags;              // Build flags (see ImFontAtlasFlags_)
    ImTextureID                 TexID;              // User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the ImDrawCmd structure.
    int                         TexDesiredWidth;    // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
    int                         TexGlyphPadding;    // Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0.

    // [Internal]
    // NB: Access texture data via GetTexData*() calls! Which will setup a default font for you.
    unsigned char*              TexPixelsAlpha8;    // 1 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight
    unsigned int*               TexPixelsRGBA32;    // 4 component per pixel, each component is unsigned 8-bit. Total size = TexWidth * TexHeight * 4
    int                         TexWidth;           // Texture width calculated during Build().
    int                         TexHeight;          // Texture height calculated during Build().
    ImVec2                      TexUvScale;         // = (1.0f/TexWidth, 1.0f/TexHeight)
    ImVec2                      TexUvWhitePixel;    // Texture coordinates to a white pixel
    ImVector<ImFont*>           Fonts;              // Hold all the fonts returned by AddFont*. Fonts[0] is the default font upon calling Legion::NewFrame(), use Legion::PushFont()/PopFont() to change the current font.
    ImVector<ImFontAtlasCustomRect> CustomRects;    // Rectangles for packing custom texture data into the atlas.
    ImVector<ImFontConfig>      ConfigData;         // Internal data
    int                         CustomRectIds[1];   // Identifiers of custom texture rectangle used by ImFontAtlas/ImDrawList

#ifndef Legion_DISABLE_OBSOLETE_FUNCTIONS
    typedef ImFontAtlasCustomRect    CustomRect;         // OBSOLETED in 1.72+
    typedef ImFontGlyphRangesBuilder GlyphRangesBuilder; // OBSOLETED in 1.67+
#endif
};

// Font runtime data and rendering
// ImFontAtlas automatically loads a default embedded font for you when you call GetTexDataAsAlpha8() or GetTexDataAsRGBA32().
struct ImFont
{
    // Members: Hot ~20/24 bytes (for CalcTextSize)
    ImVector<float>             IndexAdvanceX;      // 12-16 // out //            // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextSize functions which only this this info, and are often bottleneck in large UI).
    float                       FallbackAdvanceX;   // 4     // out // = FallbackGlyph->AdvanceX
    float                       FontSize;           // 4     // in  //            // Height of characters/line, set during loading (don't change after loading)

    // Members: Hot ~36/48 bytes (for CalcTextSize + render loop)
    ImVector<ImWchar>           IndexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
    ImVector<ImFontGlyph>       Glyphs;             // 12-16 // out //            // All glyphs.
    const ImFontGlyph*          FallbackGlyph;      // 4-8   // out // = FindGlyph(FontFallbackChar)
    ImVec2                      DisplayOffset;      // 8     // in  // = (0,0)    // Offset font rendering by xx pixels

    // Members: Cold ~32/40 bytes
    ImFontAtlas*                ContainerAtlas;     // 4-8   // out //            // What we has been loaded into
    const ImFontConfig*         ConfigData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
    short                       ConfigDataCount;    // 2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
    ImWchar                     FallbackChar;       // 2     // in  // = '?'      // Replacement character if a glyph isn't found. Only set via SetFallbackChar()
    ImWchar                     EllipsisChar;       // 2     // out // = -1       // Character used for ellipsis rendering.
    bool                        DirtyLookupTables;  // 1     // out //
    float                       Scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
    float                       Ascent, Descent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
    int                         MetricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
    ImU8                        Used4kPagesMap[(IM_UNICODE_CODEPOINT_MAX+1)/4096/8]; // 2 bytes if ImWchar=ImWchar16, 34 bytes if ImWchar==ImWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations accross all used codepoints.

    // Methods
    Legion_API ImFont();
    Legion_API ~ImFont();
    Legion_API const ImFontGlyph*FindGlyph(ImWchar c) const;
    Legion_API const ImFontGlyph*FindGlyphNoFallback(ImWchar c) const;
    float                       GetCharAdvance(ImWchar c) const     { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
    bool                        IsLoaded() const                    { return ContainerAtlas != NULL; }
    const char*                 GetDebugName() const                { return ConfigData ? ConfigData->Name : "<unknown>"; }

    // 'max_width' stops rendering after a certain width (could be turned into a 2d size). FLT_MAX to disable.
    // 'wrap_width' enable automatic word-wrapping across multiple lines to fit into given width. 0.0f to disable.
    Legion_API ImVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
    Legion_API const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    Legion_API void              RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c) const;
    Legion_API void              RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    // [Internal] Don't use!
    Legion_API void              BuildLookupTable();
    Legion_API void              ClearOutputData();
    Legion_API void              GrowIndex(int new_size);
    Legion_API void              AddGlyph(ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    Legion_API void              AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
    Legion_API void              SetGlyphVisible(ImWchar c, bool visible);
    Legion_API void              SetFallbackChar(ImWchar c);
    Legion_API bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// Include Legion_user.h at the end of Legion.h (convenient for user to only explicitly include vanilla Legion.h)
#ifdef Legion_INCLUDE_Legion_USER_H
#include "Legion_user.h"
#endif

#endif // #ifndef Legion_DISABLE

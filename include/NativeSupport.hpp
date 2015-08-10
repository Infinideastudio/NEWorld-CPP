//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_NATIVE_SUPPORT_HPP
#define NEWORLD_NATIVE_SUPPORT_HPP

#include <string>

#include <SDL2/SDL.h>

/**
 * 载入底层支持
 * （载入SDL2）
 */
void InitNativeSupport();

/**
 * 销毁底层支持
 */
void DestroyNativeSupport();

/**
 * 获取平台名称
 */
std::string GetPlatform();

typedef int StatusType;

/**
 * 表示成功的值
 * 如果小于该值表示失败
 * 使用GetError函数获得具体描述
 */
constexpr StatusType SUCCESS = 0;
std::string GetError();

constexpr int WINDOWPOS_CENTERED       = SDL_WINDOWPOS_CENTERED;
constexpr int WINDOWPOS_UNDEFINED      = SDL_WINDOWPOS_UNDEFINED;
constexpr int WINDOWPOS_UNDEFINED_MASK = SDL_WINDOWPOS_UNDEFINED_MASK;

enum class WindowFlags {
    Fullscreen        = SDL_WINDOW_FULLSCREEN,
    FullscreenDesktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    OpenGL            = SDL_WINDOW_OPENGL,
    Hidden            = SDL_WINDOW_HIDDEN,
    Borderless        = SDL_WINDOW_BORDERLESS,
    Resizable         = SDL_WINDOW_RESIZABLE,
    Minimized         = SDL_WINDOW_MINIMIZED,
    Maximized         = SDL_WINDOW_MAXIMIZED,
    InputGrabbed      = SDL_WINDOW_INPUT_GRABBED,
    AllowHighDP       = SDL_WINDOW_ALLOW_HIGHDPI
};  // enum class WindowFlags

enum class GLAttribute {
    RedSize                 = SDL_GL_RED_SIZE,
    GreenSize               = SDL_GL_GREEN_SIZE,
    BlueSize                = SDL_GL_BLUE_SIZE,
    AlphaSize               = SDL_GL_ALPHA_SIZE,
    BufferSize              = SDL_GL_BUFFER_SIZE,
    Doublebuffer            = SDL_GL_DOUBLEBUFFER,
    DepthSize               = SDL_GL_DEPTH_SIZE,
    StencilSize             = SDL_GL_STENCIL_SIZE,
    AccumRedSize            = SDL_GL_ACCUM_RED_SIZE,
    AccumGreenSize          = SDL_GL_ACCUM_GREEN_SIZE,
    AccumBlueSize           = SDL_GL_ACCUM_BLUE_SIZE,
    AccumAlphaSize          = SDL_GL_ACCUM_ALPHA_SIZE,
    Stereo                  = SDL_GL_STEREO,
    MultisampleBuffers      = SDL_GL_MULTISAMPLEBUFFERS,
    MultisampleSamples      = SDL_GL_MULTISAMPLESAMPLES,
    AcceleratedVisual       = SDL_GL_ACCELERATED_VISUAL,
    ContextMajorVersion     = SDL_GL_CONTEXT_MAJOR_VERSION,
    ContextMinorVersion     = SDL_GL_CONTEXT_MINOR_VERSION,
    ContextFlags            = SDL_GL_CONTEXT_FLAGS,
    ContextProfile_mask     = SDL_GL_CONTEXT_PROFILE_MASK,
    ShareWithCurrentContext = SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
    FramebufferSrgbCapable  = SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
    // context_release_behavior = SDL_GL_CONTEXT_RELEASE_BEHAVIOR  // SDL 2.0.4
};  // enum class GLAttribute


/**
 * SDL2中的表面
 * 注意：只提供RAII和移动构造支持
 */
typedef SDL_Surface* SurfacePtr;
struct Surface {
    Surface();
    Surface(const SurfacePtr pSurface);
    Surface(const Surface &other) = delete;
    Surface(Surface &&other);
    ~Surface();

    Surface &operator=(const Surface &other) = delete;
    Surface &operator=(Surface &&other);
    operator SurfacePtr();

    SurfacePtr GetInstance();

private:
    SurfacePtr m_pSurface = nullptr;
};  // struct Surface



/**
 * 表示一个窗口
 * RAII支持
 */
struct Window final {
    Window();
    Window(
        const std::string &windowTitle,
        int windowX, int windowY,
        int windowWitdth, int windowHeight,
        WindowFlags windowFlags
    );
    Window(const Window &other) = delete; // 禁止窗口复制
    Window(Window &&other);
    ~Window();

    Window &operator=(const Window &other) = delete;
    Window &operator=(Window &&other);

    // 共享函数
    Window Current();

    Uint32 GetID() const;

    WindowFlags GetFlags() const;

    bool IsInputGrabbed() const;
    void SetInputGrabbed(bool value);

    int GetX() const;
    int GetY() const;
    void SetX(int x);
    void SetY(int y);
    void SetPosition(int x, int y);

    int GetWidth() const;
    int GetHeight() const;
    void SetWidth(int width);
    void SetHeight(int height);
    void SetSize(int width, int height);

    std::string GetTitle()const;
    void SetTitle(const std::string &title);

    void Hide();
    void Show();
    void Raise();

    void Maximize();
    void Minimize();
    void Restore();

    void Fullscreen(WindowFlags mode);

    void SetIcon(Surface icon);

    void SetMousePosition(int x, int y);

private:
    friend struct GLContext;

    SDL_Window *m_pWindow = nullptr;
};  // struct Window

/**
 * 表示OpenGL上下文
 * RAII支持
 */
struct GLContext {
    GLContext();
    GLContext(Window &targetWindow);
    GLContext(const GLContext &other) = delete;
    GLContext(GLContext &&other);
    ~GLContext();

    GLContext &operator=(const GLContext &other) = delete;
    GLContext &operator=(GLContext &&other);

    // 共享函数
    static GLContext Current();

    bool IsSupportExtension(const std::string &extensionName) const;

    int GetAttribute(GLAttribute attr) const;
    StatusType SetAttribute(GLAttribute attr);

    void ResetAllAttribute();

    void SwapWindow();

    StatusType MakeCurrent();

private:
    SDL_GLContext *m_pOpenGL = nullptr;
    Window &m_window;
};  // struct GLContext

/**
 * 剪切板支持
 */
class Clipboard {
public:
    //This is a singleton:
    Clipboard() = delete;
    ~Clipboard() = delete;
    Clipboard(const Clipboard&) = delete;
    Clipboard(Clipboard&&) = delete;
    Clipboard &operator=(const Clipboard&) = delete;
    Clipboard &operator=(Clipboard&&) = delete;

    static bool IsEmpty();
    static std::string Get();
    static StatusType Set(const std::string &value);
};  // singleton class Clipboard

/**
 * 表示一个之指针
 * RAII支持
 */
struct Cursor {
    Cursor() = delete;
    Cursor(Surface &img, int hotX, int hotY);
    Cursor(const Cursor &other) = delete;
    Cursor(Cursor &&other);
    ~Cursor();

    Cursor &operator=(const Cursor &other) = delete;
    Cursor &operator=(Cursor &&other);

    // 共享函数
    static void ShowCursor();
    static void HideCursor();
    static bool IsHidden();
    static bool IsShown();

    void SetCursor();

private:
    SDL_Cursor *m_pCursor = nullptr;
};  // struct Cursor

#endif  // NEWORLD_NATIVE_SUPPORT_HPP

//
// Copyright 2015 infinideas
//

#include "../include/NativeSupport.hpp"

#include "../include/LogSystem.hpp"

#include <stdexcept>

#include <GL/glew.h>

using namespace std;

bool InitNativeSupport() {
    int status = SDL_Init(SDL_INIT_VIDEO);

    return status >= SUCCESS ? true : false;
}


void DestroyNativeSupport() {
    SDL_Quit();
}


string GetPlatform() {
    return string(SDL_GetPlatform());
}


string GetError() {
    return string(SDL_GetError());
}


// NativeHandler
NativeHandler::NativeHandler() {
    if (SDL_Init(SDL_INIT_VIDEO) < SUCCESS) {
        m_isVaild = false;
    } else {
        m_isVaild = true;
    }
}


NativeHandler::~NativeHandler() {
    if (m_isVaild) {
        SDL_Quit();
        m_isVaild = false;

        LogSystem::Debug("NativeHandler destroyed.");
    }
}


bool NativeHandler::IsVaild() const {
    return m_isVaild;
}
// END NativeHandler


// Surface
Surface::Surface() {

}


Surface::Surface(const SurfacePtr pSurface): m_pSurface(pSurface) {

}


Surface::Surface(Surface &&other): m_pSurface(other.m_pSurface) {
    other.m_pSurface = nullptr;  // 移动构造
}


Surface::~Surface() {
    if (m_pSurface != nullptr) {
        SDL_FreeSurface(m_pSurface);
        m_pSurface = nullptr;

        LogSystem::Debug("Surface destroyed.");
    }
}


Surface &Surface::operator=(Surface &&other) {
    m_pSurface = other.m_pSurface;
    other.m_pSurface = nullptr;

    return *this;
}


bool Surface::operator==(const Surface &target) {
    return m_pSurface == target.m_pSurface;
}


bool Surface::operator!=(const Surface &target) {
    return m_pSurface != target.m_pSurface;
}


Surface::operator SurfacePtr() {
    return m_pSurface;
}


SurfacePtr Surface::GetInstance() {
    return m_pSurface;
}
// END Surface


// Window
Window::Window() {

}


Window::Window(
    const string &windowTitle,
    int windowX, int windowY,
    int windowWitdth, int windowHeight,
    unsigned windowFlags
) {
    m_pWindow = SDL_CreateWindow(windowTitle.c_str(),
                                 windowX, windowY,
                                 windowWitdth, windowHeight,
                                 static_cast<Uint32>(windowFlags)
                                );

    // 如果未创建成功
    if (m_pWindow == nullptr) {
        throw runtime_error("Cannot create window.");
    }
}


Window::Window(Window &&other) {
    m_pWindow = other.m_pWindow;
    other.m_pWindow = nullptr;
}


Window::~Window() {
    if (m_pWindow != nullptr) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;

        LogSystem::Debug("Window destroyed.");
    }
}


Window &Window::operator=(Window &&other) {
    m_pWindow = other.m_pWindow;
    other.m_pWindow = nullptr;

    return *this;
}


bool Window::operator!=(const Window &target) {
    return m_pWindow == target.m_pWindow;
}


bool Window::operator==(const Window &target) {
    return m_pWindow != target.m_pWindow;
}


Window Window::Current() {
    Window wnd;
    wnd.m_pWindow = SDL_GL_GetCurrentWindow();

    return wnd;
}


Uint32 Window::GetID() const {
    return SDL_GetWindowID(m_pWindow);
}


unsigned Window::GetFlags() const {
    return SDL_GetWindowFlags(m_pWindow);
}


bool Window::IsInputGrabbed() const {
    return SDL_GetWindowGrab(m_pWindow);
}


void Window::SetInputGrab(bool value) {
    SDL_SetWindowGrab(m_pWindow, static_cast<SDL_bool>(value));
}


int Window::GetX() const {
    int x;
    SDL_GetWindowPosition(m_pWindow, &x, nullptr);

    return x;
}


int Window::GetY() const {
    int y;
    SDL_GetWindowPosition(m_pWindow, nullptr, &y);

    return y;
}


void Window::SetX(int x) {
    SDL_SetWindowPosition(m_pWindow, x, GetY());
}


void Window::SetY(int y) {
    SDL_SetWindowPosition(m_pWindow, GetX(), y);
}


void Window::SetPosition(int x, int y) {
    SDL_SetWindowPosition(m_pWindow, x, y);
}


int Window::GetWidth() const {
    int w;
    SDL_GetWindowSize(m_pWindow, &w, nullptr);

    return w;
}


int Window::GetHeight() const {
    int h;
    SDL_GetWindowSize(m_pWindow, nullptr, &h);

    return h;
}


void Window::SetWidth(int width) {
    SDL_SetWindowSize(m_pWindow, width, GetHeight());
}


void Window::SetHeight(int height) {
    SDL_SetWindowSize(m_pWindow, GetWidth(), height);
}


void Window::SetSize(int width, int height) {
    SDL_SetWindowSize(m_pWindow, width, height);
}


string Window::GetTitle()const {
    return string(SDL_GetWindowTitle(m_pWindow));
}


void Window::SetTitle(const string &title) {
    SDL_SetWindowTitle(m_pWindow, title.c_str());
}


void Window::Hide() {
    SDL_HideWindow(m_pWindow);
}


void Window::Show() {
    SDL_ShowWindow(m_pWindow);
}


void Window::Raise() {
    SDL_RaiseWindow(m_pWindow);
}


void Window::Maximize() {
    SDL_MaximizeWindow(m_pWindow);
}


void Window::Minimize() {
    SDL_MinimizeWindow(m_pWindow);
}


void Window::Restore() {
    SDL_RestoreWindow(m_pWindow);
}


void Window::Fullscreen(unsigned mode) {
    SDL_SetWindowFullscreen(m_pWindow, static_cast<Uint32>(mode));
}


void Window::SetIcon(Surface icon) {
    SDL_SetWindowIcon(m_pWindow, icon);
}


void Window::SetMousePosition(int x, int y) {
    SDL_WarpMouseInWindow(m_pWindow, x, y);
}
// END Window


// GLContext
GLContext::GLContext() {

}


GLContext::GLContext(Window &targetWindow) {
    if (targetWindow.m_pWindow == nullptr) {
        throw runtime_error("Invaild window object.");
    }

    m_window = &targetWindow;
    m_pOpenGL = SDL_GL_CreateContext(targetWindow.m_pWindow);

    if (m_pOpenGL == nullptr) {
        throw runtime_error("Cannot create OpenGL context.");
    }

    if (
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != SUCCESS
    ) {
        throw runtime_error("Cannot initialize OpenGL context.");
    }
}


GLContext::GLContext(GLContext &&other)
    : m_pOpenGL(other.m_pOpenGL),
      m_window(other.m_window)
{
    other.m_pOpenGL = nullptr;
    other.m_window = nullptr;
}


GLContext::~GLContext() {
    if (m_pOpenGL != nullptr) {
        SDL_GL_DeleteContext(m_pOpenGL);
        m_pOpenGL = nullptr;

        LogSystem::Debug("OpenGL context destroyed.");
    }
}


GLContext &GLContext::operator=(GLContext &&other) {
    m_pOpenGL = other.m_pOpenGL;
    m_window = other.m_window;
    other.m_pOpenGL = nullptr;
    other.m_window = nullptr;

    return *this;
}


bool GLContext::operator==(const GLContext &target) {
    return m_pOpenGL == target.m_pOpenGL;
}


bool GLContext::operator!=(const GLContext &target) {
    return m_pOpenGL != target.m_pOpenGL;
}


GLContext GLContext::Current() {
    GLContext context;
    context.m_pOpenGL = SDL_GL_GetCurrentContext();

    return context;
}


StatusType GLContext::SetOpenGLVersion(int verMajor, int verMinor) {
    bool status = true;

    status = status && SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, verMajor);
    status = status && SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, verMinor);

    return status;
}


bool GLContext::IsSupportExtension(const string &extensionName) {
    return SDL_GL_ExtensionSupported(extensionName.c_str());
}


int GLContext::GetAttribute(GLAttribute attr) {
    int value;
    SDL_GL_GetAttribute(static_cast<SDL_GLattr>(attr), &value);

    return value;
}


StatusType GLContext::SetAttribute(GLAttribute attr, int value) {
    return SDL_GL_SetAttribute(static_cast<SDL_GLattr>(attr), value);
}


void GLContext::ResetAllAttribute() {
    SDL_GL_ResetAttributes();
}


void GLContext::SwapWindow() {
    SDL_GL_SwapWindow(m_window->m_pWindow);
}


StatusType GLContext::MakeCurrent() {
    if (Current().m_pOpenGL != m_pOpenGL) {
        return SDL_GL_MakeCurrent(m_window->m_pWindow, m_pOpenGL);
    } else {
        return SUCCESS;
    }
}
// END GLContext


// Clipboard
bool Clipboard::IsEmpty() {
    return SDL_HasClipboardText();
}


string Clipboard::Get() {
    return string(SDL_GetClipboardText());
}


StatusType Clipboard::Set(const string &value) {
    return SDL_SetClipboardText(value.c_str());
}
// END Clipboard


// Cursor
Cursor::Cursor() {

}


Cursor::Cursor(Surface &img, int hotX, int hotY) {
    m_pCursor = SDL_CreateColorCursor(img.m_pSurface, hotX, hotY);

    if (m_pCursor == nullptr) {
        throw runtime_error("Cannot create cursor.");
    }
}


Cursor::Cursor(Cursor &&other): m_pCursor(other.m_pCursor) {
    other.m_pCursor = nullptr;
}


Cursor::~Cursor() {
    if (m_pCursor != nullptr) {
        SDL_FreeCursor(m_pCursor);
        m_pCursor = nullptr;

        LogSystem::Debug("Cursor destroyed.");
    }
}


Cursor &Cursor::operator=(Cursor &&other) {
    m_pCursor = other.m_pCursor;
    other.m_pCursor = nullptr;

    return *this;
}


bool Cursor::operator==(const Cursor &target) {
    return m_pCursor == target.m_pCursor;
}


bool Cursor::operator!=(const Cursor &target) {
    return m_pCursor != target.m_pCursor;
}


constexpr int SHOWCURSOR_SHOW  = 1;
constexpr int SHOWCURSOR_HIDE  = 0;
constexpr int SHOWCURSOR_QUERY = -1;


void Cursor::ShowCursor() {
    SDL_ShowCursor(SHOWCURSOR_SHOW);
}


void Cursor::HideCursor() {
    SDL_ShowCursor(SHOWCURSOR_HIDE);
}


bool Cursor::IsHidden() {
    return SDL_ShowCursor(SHOWCURSOR_QUERY) == SHOWCURSOR_HIDE ? true : false;
}


bool Cursor::IsShown() {
    return SDL_ShowCursor(SHOWCURSOR_QUERY) == SHOWCURSOR_SHOW ? true : false;
}


void Cursor::SetCursor() {
    SDL_SetCursor(m_pCursor);
}
// END Cursor

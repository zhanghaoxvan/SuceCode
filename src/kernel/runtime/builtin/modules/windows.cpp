#include <windows.h>
#include "windows.hpp"
#include "reflect_impl"
#include "kernel\function.hpp"
#include "kernel\object.hpp"
#include "kernel\variant.hpp"
#include "module_db.hpp"

static std::unordered_map<HWND, kernel::Ref<kernel::Variant>> g_WndProc;
static std::unordered_map<HWND, kernel::Ref<kernel::Variant>> g_ButtonCallBack;
static std::unordered_map<HWND, COLORREF> g_WindowBackgrounColors;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto it = g_WndProc.find(hwnd);
    if (it != g_WndProc.end() && it->second->type() == kernel::Variant::Type::FUNCTION) {
        kernel::Function func = it->second->seeAs<kernel::Function>();
        std::vector<kernel::Ref<kernel::Variant>> args;
        args.push_back(kernel::Ref<kernel::Variant>(new kernel::Variant((kernel::integer)hwnd)));
        args.push_back(kernel::Ref<kernel::Variant>(new kernel::Variant((kernel::integer)uMsg)));
        args.push_back(kernel::Ref<kernel::Variant>(new kernel::Variant((kernel::integer)wParam)));
        args.push_back(kernel::Ref<kernel::Variant>(new kernel::Variant((kernel::integer)lParam)));
        kernel::Ref<kernel::Variant> ret = func.call(args);

        if (ret->type() == kernel::Variant::Type::INT) {
            return static_cast<LRESULT>(static_cast<kernel::integer>(*ret));
        }
    }

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            HWND btnHwnd = reinterpret_cast<HWND>(lParam);
            auto it = g_ButtonCallBack.find(btnHwnd);
            if (it != g_ButtonCallBack.end() && it->second->type() == kernel::Variant::Type::FUNCTION) {
                kernel::Function func = it->second->seeAs<kernel::Function>();
                std::vector<kernel::Ref<kernel::Variant>> args;
                args.push_back(kernel::Ref<kernel::Variant>(new kernel::Variant((kernel::integer)(INT_PTR)btnHwnd)));
                func.call(args);
            }
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void kernel::modules::UILib::setWindowProc(kernel::Ref<kernel::Variant> window, kernel::Ref<kernel::Variant> callback) {
    if (window->type() != Variant::Type::INT || callback->type() != Variant::Type::FUNCTION) {
        return;
    }

    HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*window));
    g_WndProc[hwnd] = callback;
}

namespace kernel {
    namespace modules {
        static ModuleRegister __{MODULE_NAME, CLASS_NAME::instance};

        Ref<Variant> UILib::createWindow(const string &title, integer width, integer height) {
            std::wstring wtitle(title.begin(), title.end());
            LPCWSTR lpTitle = wtitle.c_str();

            static bool registered = false;
            if (!registered) {
                WNDCLASSEXW wc = {0};
                wc.cbSize = sizeof(WNDCLASSEXW);
                wc.lpfnWndProc = WndProc;
                wc.hInstance = GetModuleHandle(NULL);
                wc.lpszClassName = L"SuceCodeKernelWindowClass";
                RegisterClassExW(&wc);
                registered = true;
            }

            HWND hwnd = CreateWindowExW(
                0,
                L"SuceCodeKernelWindowClass",
                lpTitle,
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                NULL, NULL, GetModuleHandle(NULL), NULL
            );

            if(hwnd == NULL) {
                return Ref<Variant>(new Variant("Failed to create window"));
            }

            return Ref<Variant>(new Variant((integer)(INT_PTR)hwnd));
        }

        Ref<Variant> UILib::createButton(Ref<Variant> parent, const string &text, integer x, integer y, integer width, integer height) {
            if (parent->type() != Variant::Type::INT) {
                return Ref<Variant>(new Variant("Parent must be an integer window handle"));
            }

            HWND parentHwnd = (HWND)(INT_PTR)(integer)*parent;
            std::wstring wtext(text.begin(), text.end());
            LPCWSTR lpText = wtext.c_str();

            HWND buttonHwnd = CreateWindowExW(
                0,
                L"BUTTON",
                lpText,
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                x, y, width, height,
                parentHwnd, NULL, GetModuleHandle(NULL), NULL
            );

            if(buttonHwnd == NULL) {
                return Ref<Variant>(new Variant("Failed to create button"));
            }

            return Ref<Variant>(new Variant((integer)(INT_PTR)buttonHwnd));
        }

        void UILib::setOnClick(Ref<Variant> control, Ref<Variant> callback) {
            if (control->type() == Variant::Type::INT && callback->type() == Variant::Type::FUNCTION) {
                HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*control));
                g_ButtonCallBack[hwnd] = callback;
            }
        }

        void UILib::show(Ref<Variant> window) {
            if (window->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*window));
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);

            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0) > 0) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        void UILib::closeWindow(Ref<Variant> window) {
            if (window->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*window));
            CloseWindow(hwnd);
        }

        void UILib::minimizeWindow(Ref<Variant> window) {
            if (window->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*window));
            ShowWindow(hwnd, SW_MINIMIZE);
        }

        void UILib::maximizeWindow(Ref<Variant> window) {
            if (window->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*window));
            ShowWindow(hwnd, SW_MAXIMIZE);
        }

        Ref<Variant> UILib::getWindowSize(Ref<Variant> win) {
            if (win->type() != Variant::Type::INT) {
                return Ref<Variant>(new Variant("Window must be an integer handle"));
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            RECT rect;
            if (GetWindowRect(hwnd, &rect)) {
                integer width = rect.right - rect.left;
                integer height = rect.bottom - rect.top;

                Ref<Object> obj = new Object();
                obj->set("width", Ref<Variant>(new Variant(width)));
                obj->set("height", Ref<Variant>(new Variant(height)));
                return Ref<Variant>(new Variant(obj));
            }

            return Ref<Variant>(new Variant("Failed to get window size"));
        }

        void UILib::setWindowSize(Ref<Variant> win, integer width, integer height) {
            if (win->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
        }

        void UILib::setWindowTitle(Ref<Variant> win, const string &title) {
            if (win->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            std::wstring wtitle(title.begin(), title.end());
            SetWindowTextW(hwnd, wtitle.c_str());
        }

        void UILib::resizeWindow(Ref<Variant> win, integer width, integer height) {
            if (win->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            RECT rect;
            GetWindowRect(hwnd, &rect);
            SetWindowPos(hwnd, NULL, rect.left, rect.top, width, height, SWP_NOZORDER);
        }

        void UILib::setWindowBackgroundcolor(Ref<Variant> win, integer r, integer g, integer b) {
            if (win->type() != Variant::Type::INT) {
                return;
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            COLORREF color = RGB(r, g, b);
            g_WindowBackgrounColors[hwnd] = color;
            InvalidateRect(hwnd, NULL, TRUE);
        }

        Ref<Variant> UILib::getWindowBackgroundcolor(Ref<Variant> win) {
            if (win->type() != Variant::Type::INT) {
                return Ref<Variant>(new Variant("Window must be an integer handle"));
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            auto it = g_WindowBackgrounColors.find(hwnd);
            if (it != g_WindowBackgrounColors.end()) {
                COLORREF color = it->second;
                integer r = GetRValue(color);
                integer g = GetGValue(color);
                integer b = GetBValue(color);

                Ref<Object> obj = new Object();
                obj->set("r", Ref<Variant>(new Variant(r)));
                obj->set("g", Ref<Variant>(new Variant(g)));
                obj->set("b", Ref<Variant>(new Variant(b)));
                return Ref<Variant>(new Variant(obj));
            }

            return Ref<Variant>(new Variant("No background color set"));
        }

        Ref<Variant> UILib::getWindowTitle(Ref<Variant> win) {
            if (win->type() != Variant::Type::INT) {
                return Ref<Variant>(new Variant("Window must be an integer handle"));
            }

            HWND hwnd = reinterpret_cast<HWND>(static_cast<integer>(*win));
            WCHAR title[512] = {0};
            if (GetWindowTextW(hwnd, title, 511) > 0) {
                std::wstring wtitle(title);
                std::string title_str(wtitle.begin(), wtitle.end());
                return Ref<Variant>(new Variant(title_str));
            }

            return Ref<Variant>(new Variant(""));
        }
    }
}

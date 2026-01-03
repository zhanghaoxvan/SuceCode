#pragma once

#include <kernel/typedef.hpp>  
#include <kernel/variant.hpp>  
#include <kernel/object.hpp>   

#undef CLASS_NAME
#define CLASS_NAME UILib
#undef MODULE_NAME
#define MODULE_NAME "windows"
#undef MESSAGE_BODY
#define MESSAGE_BODY \
    FIELD_METHOD(createWindow, Ref<Variant>, const string &title, integer width, integer height); \
    FIELD_METHOD(createButton, Ref<Variant>, Ref<Variant> parent, const string &text, integer x, integer y, integer width, integer height); \
    FIELD_METHOD(setOnClick, void, Ref<Variant> control, Ref<Variant> callback); \
    FIELD_METHOD(show, void, Ref<Variant> window); \
    FIELD_METHOD(closeWindow, void, Ref<Variant> window); \
    FIELD_METHOD(minimizeWindow, void, Ref<Variant> window); \
    FIELD_METHOD(maximizeWindow, void, Ref<Variant> window); \
    FIELD_METHOD(setWindowProc, void, Ref<Variant> window, Ref<Variant> callback); \
    FIELD_METHOD(getWindowSize, Ref<Variant>, Ref<Variant> window); \ 
    FIELD_METHOD(setWindowSize, void, Ref<Variant> window, integer width, integer height);\
    FIELD_METHOD(setWindowTitle, void, Ref<Variant> window, const string &title); \ 
    FIELD_METHOD(getWindowTitle, Ref<Variant>, Ref<Variant> window); \
    FIELD_METHOD(setWindowBackgroundcolor, void, Ref<Variant> window, integer r, integer g, integer b); \
    FIELD_METHOD(getWindowBackgroundcolor, Ref<Variant>, Ref<Variant> window); \ 
    FIELD_METHOD(resizeWindow, void, Ref<Variant> window, integer width, integer height);

#include "reflect_header"
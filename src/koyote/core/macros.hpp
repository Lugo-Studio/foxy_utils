//
// Created by galex on 8/21/2022.
//

#pragma once

#define MAKE_UNIQUE(x) std::make_unique<decltype(x)>(x)
#define PIMPL(x) class x; std::unique_ptr<x>

#define BIT(x) (1 << x)
#define FOXY_BIT_CHECK(x, y) (x & y) == y
#define FOXY_STRINGIFY_VAL(x) FOXY_STRINGIFY(x)
#define FOXY_STRINGIFY(x) #x
#define FOXY_LAMBDA_INS(fn, instance) [objPtr = instance](auto&&... args) { return objPtr->fn(std::forward<decltype(args)>(args)...); }
#define FOXY_LAMBDA(fn) FOXY_LAMBDA_INS(fn, this)

#if defined(_WIN32) and not defined(FOXY_DEBUG_MODE)
#define REDIRECT_WINMAIN_TO_MAIN \
auto main(int, char**) -> int;\
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {\
  return main(__argc, __argv);\
}
#else
#define REDIRECT_WINMAIN_TO_MAIN auto __fubuki_is_cute() -> int { return 0; }
#endif

#if defined(_WIN32) and not defined(FOXY_DEBUG_MODE)
#define REDIRECT_WINMAIN_TO_KOYOTE_MAIN \
auto main(fx::i32, fx::i8**) -> fx::i32;\
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {\
  return main(__argc, reinterpret_cast<std::int8_t**>(__argv));\
}
#else
#define REDIRECT_WINMAIN_TO_KOYOTE_MAIN auto __fubuki_is_cute() -> kyt::i32 { return 0; }
#endif
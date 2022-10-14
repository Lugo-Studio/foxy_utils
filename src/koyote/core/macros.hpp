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
#define FOXY_ENUMERATE(x) std::views::zip(std::views::iota(0, static_cast<i32>(x.size())), x)
// for (std::array arr{ 1, 2, 3, 4, 5 }; const auto & [index, element]: FOXY_ENUMERATE(arr)) {
//
// }

#if defined(_MSC_VER)
#define FOXY_DISABLE_WARNINGS __pragma(warning( push, 1 ))
#define FOXY_ENABLE_WARNINGS  __pragma(warning( pop ))

#define FOXY_DISABLE_WARNING_PUSH           __pragma(warning( push ))
#define FOXY_DISABLE_WARNING_POP            __pragma(warning( pop )) 
#define FOXY_DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

#define FOXY_DISABLE_WARNING_UNINITIALIZED_VARIABLE FOXY_DISABLE_WARNING(26495)
// #define FOXY_DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    FOXY_DISABLE_WARNING(4100)
// other warnings you want to deactivate...
#elif defined(__GNUC__) or defined(__clang__)
#define FOXY_DO_PRAGMA(X) _Pragma(#X)

#define FOXY_DISABLE_WARNINGS \
  FOXY_DO_PRAGMA(GCC diagnostic push) \
  FOXY_DO_PRAGMA(GCC diagnostic ignored "-Wall") \
  FOXY_DO_PRAGMA(clang diagnostic ignored "-Wextra") \
  FOXY_DO_PRAGMA(clang diagnostic ignored "-Wtautological-compare")
#define FOXY_ENABLE_WARNINGS  __pragma(warning( pop ))

#define FOXY_DISABLE_WARNING_PUSH         FOXY_DO_PRAGMA(GCC diagnostic push)
#define FOXY_DISABLE_WARNING_POP          FOXY_DO_PRAGMA(GCC diagnostic pop) 
#define FOXY_DISABLE_WARNING(warningName) FOXY_DO_PRAGMA(GCC diagnostic ignored #warningName)

#define FOXY_DISABLE_WARNING_UNINITIALIZED_VARIABLE FOXY_DISABLE_WARNING(-Wuninitialized)
// #define FOXY_DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER    FOXY_DISABLE_WARNING(-Wunused-parameter)
// other warnings you want to deactivate... 
#else
#define FOXY_DISABLE_WARNINGS
#define FOXY_ENABLE_WARNINGS 

#define FOXY_DISABLE_WARNING_PUSH
#define FOXY_DISABLE_WARNING_POP

#define FOXY_DISABLE_WARNING_UNINITIALIZED_VARIABLE
// #define FOXY_DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
// other warnings you want to deactivate... 
#endif

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
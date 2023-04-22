export module foxy_main;

import std.core;
import foxy_helpers;

namespace fx {
  export auto main(const fx::arg_list& args) -> int32_t;
}

export auto main(int32_t argc, char* argv[]) -> int32_t
{
  return fx::main({ argv, argv + argc });
}
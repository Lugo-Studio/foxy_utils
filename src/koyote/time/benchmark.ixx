module;

#include <std/core.hpp>
#include <std/filesystem.hpp>
#include "koyote/helpers.hpp"
#include "koyote/json.hpp"

export module koyote_time:benchmark;

import :time;
import koyote_types;

namespace fx {
  export class ProfilerSession {
  public:
    ProfilerSession(std::string name):
      name_{ std::move(name) }
    {
    
    }
    
    ~ProfilerSession()
    {
    
    }
  
  private:
    std::string name_;
  };
  
  export class Profiler {
  public:
    Profiler(std::string name, std::filesystem::path out_path = "tmp/logs/profiler.json"):
      out_path_{ std::move(out_path) },
      session_{ std::make_unique<ProfilerSession>(std::move(name)) }
    {
      json_file_["otherData"] = json::object();
      json_file_["traceEvents"] = json::array();
    }
    
    ~Profiler()
    {
      if (std::ofstream file{ out_path_ })
        profile_count_ = 0;
    }
  
  private:
    json json_file_;
    std::filesystem::path out_path_;
    unique<ProfilerSession> session_;
    std::uint32_t profile_count_;
  };
}
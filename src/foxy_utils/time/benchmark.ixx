module;

#include <std/core.hpp>
#include <std/filesystem.hpp>
#include <foxy_utils/json.hpp>

export module foxy_time:benchmark;

import :time;

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
  
  // class Profiler {
  // public:
  //   Profiler(std::string name, std::filesystem::path out_path = "tmp/logs/profiler.json"):
  //     out_path_{ std::move(out_path) },
  //     session_{ std::make_unique<ProfilerSession>(std::move(name)) }
  //   {
  //     json_file_["otherData"] = json::object();
  //     json_file_["traceEvents"] = json::array();
  //   }
  //
  //   virtual ~Profiler()
  //   {
  //     if (std::ofstream file{ out_path_ })
  //       profile_count_ = 0;
  //   }
  // private:
  //   json json_file_;
  //   std::filesystem::path out_path_;
  //   std::unique_ptr<ProfilerSession> session_;
  //   std::uint32_t profile_count_;
  // };
}

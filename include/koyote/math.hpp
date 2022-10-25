#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

namespace fx {
  // Floating point single precision
  using vec2 = glm::vec2;
  using vec3 = glm::vec3;
  using vec4 = glm::vec4;
  using mat3 = glm::mat3;
  using mat4 = glm::mat4;
  // Floating point double precision
  using dvec2 = glm::dvec2;
  using dvec3 = glm::dvec3;
  using dvec4 = glm::dvec4;
  using dmat3 = glm::dmat3;
  using dmat4 = glm::dmat4;
  // Integer
  using ivec2 = glm::ivec2;
  using ivec3 = glm::ivec3;
  using ivec4 = glm::ivec4;
  // Unsigned integer
  using uvec2 = glm::uvec2;
  using uvec3 = glm::uvec3;
  using uvec4 = glm::uvec4;

  // Misc
  using quat = glm::quat;

  const mat4 MatID{ mat4{1.0f} };

  struct rect {
    vec2 position;
    vec2 size;

    [[nodiscard]] float width() const { return size.x; }
    [[nodiscard]] float height() const { return size.y; }
    [[nodiscard]] float half_width() const { return size.x * .5f; }
    [[nodiscard]] float half_height() const { return size.y * .5f; }
    [[nodiscard]] vec2 center() const { return vec2{ position.x + half_width(), position.y + half_height() }; }
    [[nodiscard]] vec2 left() const { return position - vec2{ half_width(), 0 }; }
    [[nodiscard]] vec2 right() const { return position + vec2{ half_width(), 0 }; }
    [[nodiscard]] vec2 top() const { return position + vec2{ 0, half_height() }; }
    [[nodiscard]] vec2 bottom() const { return position - vec2{ 0, half_height() }; }
  };

  struct bounds {
    vec3 center;
    vec3 size;

    [[nodiscard]] float width() const { return size.x; }
    [[nodiscard]] float height() const { return size.y; }
    [[nodiscard]] float depth() const { return size.z; }
    [[nodiscard]] float half_width() const { return size.x * .5f; }
    [[nodiscard]] float half_height() const { return size.y * .5f; }
    [[nodiscard]] float half_depth() const { return size.z * .5f; }
    [[nodiscard]] vec3 anchor_corner() const { return vec3{ center.x - half_width(), center.y - half_height(), center.z - half_depth() }; }
    [[nodiscard]] vec3 left() const { return center - vec3{ half_width(), 0, 0 }; }
    [[nodiscard]] vec3 right() const { return center + vec3{ half_width(), 0, 0 }; }
    [[nodiscard]] vec3 top() const { return center + vec3{ 0, half_height(), 0 }; }
    [[nodiscard]] vec3 bottom() const { return center - vec3{ 0, half_height(), 0 }; }
    [[nodiscard]] vec3 front() const { return center - vec3{ 0, 0, half_depth() }; }
    [[nodiscard]] vec3 back() const { return center + vec3{ 0, 0, half_depth() }; }
  };
}

/**
 * @file version.hpp
 * @brief Version information for the kuukan library
 * 
 * Defines version macros and constants that can be used to check the
 * library version at compile time or runtime.
 */

#pragma once

/// @brief Major version number (incremented for incompatible API changes)
#define KUUKAN_VERSION_MAJOR 0

/// @brief Minor version number (incremented for new features, backward compatible)
#define KUUKAN_VERSION_MINOR 1

/// @brief Patch version number (incremented for bug fixes, backward compatible)
#define KUUKAN_VERSION_PATCH 0

namespace kuukan {

/// @brief Major version number as a compile-time constant
inline constexpr int version_major = KUUKAN_VERSION_MAJOR;

/// @brief Minor version number as a compile-time constant
inline constexpr int version_minor = KUUKAN_VERSION_MINOR;

/// @brief Patch version number as a compile-time constant
inline constexpr int version_patch = KUUKAN_VERSION_PATCH;

} // namespace kuukan

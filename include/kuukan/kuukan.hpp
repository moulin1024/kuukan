/**
 * @file kuukan.hpp
 * @brief Main header file for the kuukan library - includes all core components
 * 
 * @mainpage kuukan: Abstract Functional Analysis Interfaces in C++
 * 
 * @section intro_sec Introduction
 * 
 * kuukan is a header-only C++20 library that provides abstract interfaces for
 * functional analysis concepts. It allows you to define mathematical structures
 * (vector spaces, metric spaces, normed spaces) through "operation injection" -
 * you provide functors that implement the operations, and the library composes
 * them into complete mathematical structures.
 * 
 * @section features_sec Key Features
 * 
 * - **Abstract Vector Spaces**: Define vector spaces over any element type
 *   (including infinite-dimensional spaces like function spaces)
 * - **Metric Spaces**: Define distance functions between elements
 * - **Normed Spaces**: Define norms and automatically derive metrics
 * - **Type-Safe**: Uses C++20 concepts for compile-time type checking
 * - **Header-Only**: No linking required, just include the headers
 * 
 * @section usage_sec Usage Example
 * 
 * @code{.cpp}
 * // Define a vector space of functions
 * using FunctionSpace = kuukan::VectorSpace<
 *     RealFunction,    // Element type
 *     double,          // Scalar type
 *     FunctionAdd,     // Addition functor
 *     FunctionScale,   // Scalar multiplication
 *     FunctionNegate,  // Negation
 *     FunctionZero,    // Zero element
 *     FunctionEqual    // Equality
 * >;
 * 
 * // Use the vector space operations
 * auto sum = FunctionSpace::addition(f, g);
 * auto scaled = FunctionSpace::scalar_action(2.0, f);
 * @endcode
 * 
 * @section structure_sec Library Structure
 * 
 * - **Concepts** (`concepts/core_concepts.hpp`): Type requirements (FieldLike, OrderedMeasure, etc.)
 * - **VectorSpace** (`vector/vector_space.hpp`): Abstract vector space interface
 * - **MetricSpace** (`metric/metric_space.hpp`): Abstract metric space interface
 * - **NormedSpace** (`norm/normed_space.hpp`): Normed space that induces a metric
 * 
 * @version 0.1.0
 * @author kuukan contributors
 */

#pragma once

#include "version.hpp"
#include "concepts/core_concepts.hpp"
#include "vector/vector_space.hpp"
#include "metric/metric_space.hpp"
#include "norm/normed_space.hpp"

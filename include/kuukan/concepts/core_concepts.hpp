/**
 * @file core_concepts.hpp
 * @brief Core C++20 concepts for type requirements in kuukan
 * 
 * This file defines the fundamental type concepts used throughout the kuukan
 * library to ensure type safety and correct operation signatures at compile time.
 */

#pragma once
#include <concepts>
#include <type_traits>
#include <utility>
#include <functional>

namespace kuukan {

/**
 * @brief Concept for scalar field types
 * 
 * @tparam ScalarType The type to be checked
 * 
 * A type satisfies FieldLike if it supports the basic field operations:
 * - Addition (+)
 * - Subtraction (-)
 * - Multiplication (*)
 * - Division (/)
 * - Equality comparison (==)
 * 
 * This concept is used to ensure that scalar types used in vector spaces
 * behave like a mathematical field. Note that this is not limited to built-in
 * numeric types - you can use custom types (e.g., rational numbers, symbolic
 * expressions) as long as they implement these operations.
 * 
 * @note The division operation is required, meaning types must support division
 *       by non-zero elements. Zero division is undefined behavior.
 * 
 * @example
 * @code{.cpp}
 * static_assert(FieldLike<double>);
 * static_assert(FieldLike<float>);
 * static_assert(FieldLike<int>);  // Note: integer division
 * @endcode
 */
template <typename ScalarType>
concept FieldLike =
    requires(ScalarType lhs, ScalarType rhs) {
        { lhs + rhs } -> std::same_as<ScalarType>;
        { lhs - rhs } -> std::same_as<ScalarType>;
        { lhs * rhs } -> std::same_as<ScalarType>;
        { lhs / rhs } -> std::same_as<ScalarType>;
        { lhs == rhs } -> std::convertible_to<bool>;
    };

/**
 * @brief Concept for ordered measure types used in metrics and norms
 * 
 * @tparam MeasureType The type to be checked
 * 
 * A type satisfies OrderedMeasure if it supports:
 * - Addition (+)
 * - Equality comparison (==)
 * - Strict less-than comparison (<)
 * - Less-than-or-equal comparison (<=)
 * 
 * This concept is used for the return types of distance functions in
 * MetricSpace and norm functions in NormedSpace. The measure type must be
 * additive (for triangle inequality) and comparable (for ordering distances).
 * 
 * Typical examples include `double`, `float`, or custom precision types.
 * The type must form a total order with addition defined.
 * 
 * @note The ordering is required for metrics to satisfy the ordering axioms.
 * 
 * @example
 * @code{.cpp}
 * static_assert(OrderedMeasure<double>);
 * static_assert(OrderedMeasure<float>);
 * // Custom types can satisfy this if they implement the required operations
 * @endcode
 */
template <typename MeasureType>
concept OrderedMeasure =
    requires(MeasureType u, MeasureType v) {
        { u + v }  -> std::same_as<MeasureType>;
        { u == v } -> std::convertible_to<bool>;
        { u < v }  -> std::convertible_to<bool>;
        { u <= v } -> std::convertible_to<bool>;
    };

/**
 * @brief Concept for callable types with specific signature
 * 
 * @tparam F The callable type (function, functor, lambda, etc.)
 * @tparam R The expected return type
 * @tparam Args The argument types
 * 
 * A type satisfies CallableLike<F, R, Args...> if:
 * - F can be called with arguments Args...
 * - The return type is convertible to R
 * - F is regular invocable (can be called multiple times with same result)
 * 
 * This concept is used extensively to verify that operation functors
 * (addition, scalar multiplication, etc.) have the correct signatures
 * for use in VectorSpace and related structures.
 * 
 * @note Regular invocable means the function has no side effects that
 *       would cause different results for the same inputs.
 * 
 * @example
 * @code{.cpp}
 * struct Add {
 *     int operator()(int a, int b) const { return a + b; }
 * };
 * static_assert(CallableLike<Add, int, int, int>);
 * @endcode
 */
template <typename F, typename R, typename... Args>
concept CallableLike =
    std::regular_invocable<F, Args...> &&
    std::convertible_to<std::invoke_result_t<F, Args...>, R>;

} // namespace kuukan

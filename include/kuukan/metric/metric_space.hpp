/**
 * @file metric_space.hpp
 * @brief Abstract metric space interface
 * 
 * This file provides the MetricSpace template, which defines a metric space
 * by injecting a distance function. A metric space is a set equipped with a
 * distance function that measures the "distance" between any two elements.
 */

#pragma once
#include <type_traits>
#include <utility>
#include "kuukan/concepts/core_concepts.hpp"

namespace kuukan {

/**
 * @brief Abstract metric space structure
 * 
 * @tparam ElementType The type of elements in the metric space
 * @tparam DistanceFunction Functor type for distance: (ElementType, ElementType) -> MeasureType
 * 
 * A metric space is a set equipped with a distance function (metric) that
 * satisfies the following axioms:
 * 
 * 1. **Non-negativity**: distance(a, b) >= 0
 * 2. **Identity of indiscernibles**: distance(a, b) = 0 if and only if a = b
 * 3. **Symmetry**: distance(a, b) = distance(b, a)
 * 4. **Triangle inequality**: distance(a, c) <= distance(a, b) + distance(b, c)
 * 
 * Your DistanceFunction functor must implement these axioms. The library
 * verifies the type signature but cannot enforce the mathematical axioms
 * at compile time.
 * 
 * @section measure_type Measure Type
 * 
 * The return type of DistanceFunction must satisfy OrderedMeasure, meaning:
 * - It can be added (for triangle inequality)
 * - It can be compared (for non-negativity and ordering)
 * 
 * Typical examples: `double`, `float`, or custom precision types.
 * 
 * @section metric_space_usage Usage
 * 
 * @code{.cpp}
 * // Define a distance function
 * struct EuclideanDistance {
 *     double operator()(const Point& a, const Point& b) const {
 *         double dx = a.x - b.x;
 *         double dy = a.y - b.y;
 *         return std::sqrt(dx*dx + dy*dy);
 *     }
 * };
 * 
 * // Create metric space
 * using PointSpace = MetricSpace<Point, EuclideanDistance>;
 * 
 * // Compute distance
 * Point p1{0, 0}, p2{3, 4};
 * double d = PointSpace::distance(p1, p2);  // d = 5.0
 * @endcode
 * 
 * @note This structure is very abstract and does not assume any particular
 *       representation of elements or computation method for distance.
 * 
 * @see NormedSpace for a metric space induced by a norm
 */
template <typename ElementType, typename DistanceFunction>
requires std::default_initializable<DistanceFunction> &&
         OrderedMeasure<std::invoke_result_t<DistanceFunction,
                                             const ElementType&, const ElementType&>>
struct MetricSpace {
    /// @brief Type alias for elements of this metric space
    using element_type = ElementType;
    
    /// @brief Type alias for the distance measure type
    using measure_type =
        std::invoke_result_t<DistanceFunction, const ElementType&, const ElementType&>;

    /// @brief Static instance of the distance functor
    static inline constexpr DistanceFunction distance{};

    /**
     * @brief Compute the distance between two elements
     * 
     * This is a convenience alias for the distance functor.
     * Equivalent to: distance(element_left, element_right)
     * 
     * @param element_left The first element
     * @param element_right The second element
     * @return The distance between the two elements
     * 
     * @note The distance function must satisfy metric space axioms.
     *       Negative distances are not allowed (must return >= 0).
     */
    static constexpr measure_type dist(const element_type& element_left,
                                       const element_type& element_right) {
        return distance(element_left, element_right);
    }
};

/**
 * @brief Concept for types that behave like a MetricSpace
 * 
 * @tparam T The type to check
 * 
 * A type satisfies MetricSpaceLike if it has:
 * - `element_type` and `measure_type` type aliases
 * - A static `distance` function: (element_type, element_type) -> measure_type
 * 
 * This concept allows template functions to work with any MetricSpace-like
 * structure, including custom implementations or extensions.
 * 
 * @note MetricSpace automatically satisfies this concept.
 * 
 * @see MetricSpace
 */
template <typename T>
concept MetricSpaceLike = requires {
    typename T::element_type;
    typename T::measure_type;
    { T::distance(std::declval<const typename T::element_type&>(),
                  std::declval<const typename T::element_type&>()) }
        -> std::same_as<typename T::measure_type>;
};

} // namespace kuukan

/**
 * @file normed_space.hpp
 * @brief Normed space that automatically induces a metric space
 * 
 * This file provides the NormedSpace template, which extends a VectorSpace
 * with a norm function. The norm automatically induces a metric (distance
 * function) through the standard formula: distance(a, b) = norm(a - b).
 */

#pragma once
#include <type_traits>
#include <utility>
#include "kuukan/concepts/core_concepts.hpp"
#include "kuukan/vector/vector_space.hpp"
#include "kuukan/metric/metric_space.hpp"

namespace kuukan {

/**
 * @brief Normed space that induces a metric space
 * 
 * @tparam VS The base vector space (must satisfy VectorSpaceLike)
 * @tparam NormFunction Functor type for norm: (element_type) -> measure_type
 * 
 * A normed space is a vector space equipped with a norm function that assigns
 * a non-negative "length" or "size" to each vector. The norm must satisfy:
 * 
 * 1. **Non-negativity**: norm(v) >= 0
 * 2. **Definiteness**: norm(v) = 0 if and only if v = 0
 * 3. **Absolute homogeneity**: norm(α * v) = |α| * norm(v) for scalar α
 * 4. **Triangle inequality**: norm(u + v) <= norm(u) + norm(v)
 * 
 * From a norm, we can automatically derive a metric (distance function) using:
 * 
 *     distance(a, b) = norm(a - b)
 * 
 * This induced metric automatically satisfies all metric space axioms if the
 * norm satisfies the norm axioms above.
 * 
 * @section inheritance Inheritance from VectorSpace
 * 
 * NormedSpace inherits from VS, so all vector space operations (addition,
 * scalar multiplication, etc.) are available directly. Additionally, it
 * provides:
 * - A `norm` function to compute the norm of an element
 * - A `distance` function that uses the induced metric
 * 
 * @section induced_metric Induced Metric
 * 
 * The metric is automatically constructed as an internal type `InducedDistance`
 * that computes: norm(VS::difference(left, right)). This is equivalent to
 * norm(left - right) in mathematical notation.
 * 
 * @section normed_space_usage Usage
 * 
 * @code{.cpp}
 * // Start with a vector space
 * using MyVectorSpace = VectorSpace<MyVector, double, ...>;
 * 
 * // Define a norm function
 * struct EuclideanNorm {
 *     double operator()(const MyVector& v) const {
 *         return std::sqrt(v.x*v.x + v.y*v.y);
 *     }
 * };
 * 
 * // Create normed space
 * using MyNormedSpace = NormedSpace<MyVectorSpace, EuclideanNorm>;
 * 
 * // Use vector space operations
 * MyVector v1{1, 2}, v2{3, 4};
 * auto sum = MyNormedSpace::addition(v1, v2);
 * 
 * // Use norm
 * double n = MyNormedSpace::norm(v1);  // norm of v1
 * 
 * // Use induced distance
 * double d = MyNormedSpace::distance(v1, v2);  // distance between v1 and v2
 * @endcode
 * 
 * @note This structure maintains complete abstraction. It does not assume:
 *       - Finite dimensions
 *       - Enumerability of elements
 *       - Numerical implementation of the norm (could be symbolic)
 * 
 * @see VectorSpace for the base vector space structure
 * @see MetricSpace for the induced metric structure
 */
template <VectorSpaceLike VS, typename NormFunction>
requires std::default_initializable<NormFunction> &&
         OrderedMeasure<std::invoke_result_t<NormFunction,
                                             const typename VS::element_type&>>
struct NormedSpace : VS {
    /// @brief Type alias for elements (inherited from vector space)
    using element_type = typename VS::element_type;
    
    /// @brief Type alias for the norm measure type
    using measure_type = std::invoke_result_t<NormFunction, const element_type&>;

    /// @brief Static instance of the norm functor
    static inline constexpr NormFunction norm{};

    /**
     * @brief Internal functor for the distance induced by the norm
     * 
     * This functor implements the standard metric induced by a norm:
     * distance(a, b) = norm(a - b)
     * 
     * It is used internally to construct the metric_space type.
     */
    struct InducedDistance {
        /**
         * @brief Compute the distance between two elements using the norm
         * 
         * @param element_left The first element
         * @param element_right The second element
         * @return The distance: norm(element_left - element_right)
         */
        constexpr measure_type operator()(const element_type& element_left,
                                          const element_type& element_right) const {
            return norm( VS::difference(element_left, element_right) );
        }
    };

    /// @brief Type alias for the metric space induced by this norm
    using metric_space = MetricSpace<element_type, InducedDistance>;
    
    /// @brief Alternative name for metric_space
    using metric_type  = metric_space;

    /**
     * @brief Compute the distance between two elements
     * 
     * Uses the metric induced by the norm: distance(a, b) = norm(a - b)
     * 
     * @param element_left The first element
     * @param element_right The second element
     * @return The distance between the elements
     * 
     * @note This distance automatically satisfies metric space axioms if
     *       the norm satisfies norm axioms.
     */
    static constexpr measure_type distance(const element_type& element_left,
                                           const element_type& element_right) {
        return metric_space::dist(element_left, element_right);
    }
};

} // namespace kuukan

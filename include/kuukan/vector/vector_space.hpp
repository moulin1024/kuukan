/**
 * @file vector_space.hpp
 * @brief Abstract vector space interface using operation injection
 * 
 * This file provides the VectorSpace template, which allows you to define
 * abstract vector spaces by injecting operation functors. This approach is
 * extremely flexible and works with finite-dimensional vectors, infinite-dimensional
 * function spaces, symbolic expressions, and more.
 */

#pragma once
#include <type_traits>
#include <utility>
#include "kuukan/concepts/core_concepts.hpp"

namespace kuukan {

/**
 * @brief Abstract vector space structure using operation injection
 * 
 * @tparam ElementType The type of elements in the vector space
 * @tparam ScalarType The scalar type (must satisfy FieldLike concept)
 * @tparam Addition Functor type for vector addition: (ElementType, ElementType) -> ElementType
 * @tparam ScalarAction Functor type for scalar multiplication: (ScalarType, ElementType) -> ElementType
 * @tparam Negation Functor type for negation: (ElementType) -> ElementType
 * @tparam ZeroSupplier Functor type for zero element: () -> ElementType
 * @tparam Equality Functor type for equality: (ElementType, ElementType) -> bool
 * 
 * VectorSpace is a compile-time structure that represents a mathematical vector space
 * by composing operation functors. Instead of requiring elements to have specific
 * operators, you provide functors that implement the vector space operations.
 * 
 * @section vector_space_ops Vector Space Operations
 * 
 * A vector space must satisfy the following axioms (your functors must implement these):
 * - **Addition**: Associative, commutative, with identity (zero)
 * - **Scalar Multiplication**: Distributive over addition and field addition
 * - **Negation**: Each element has an additive inverse
 * - **Zero**: There exists a unique zero element
 * 
 * @section operation_injection Operation Injection
 * 
 * The "operation injection" pattern means you provide the operations as separate
 * functor types rather than requiring them as methods on the element type. This
 * allows you to:
 * - Work with types you don't control (e.g., std::function)
 * - Define multiple vector space structures over the same element type
 * - Use symbolic or lazy evaluation without modifying element types
 * 
 * @section type_requirements Type Requirements
 * 
 * All functor types must be:
 * - Default constructible (they will be instantiated as static members)
 * - Regular invocable (same inputs produce same outputs)
 * - Have the exact signatures specified in the template parameters
 * 
 * @section vector_space_usage Usage
 * 
 * @code{.cpp}
 * // Define operation functors
 * struct VecAdd {
 *     MyVector operator()(const MyVector& a, const MyVector& b) const {
 *         return MyVector{a.x + b.x, a.y + b.y};
 *     }
 * };
 * 
 * struct VecScale {
 *     MyVector operator()(double s, const MyVector& v) const {
 *         return MyVector{s * v.x, s * v.y};
 *     }
 * };
 * 
 * // Create vector space
 * using MyVectorSpace = VectorSpace<
 *     MyVector,    // Element type
 *     double,      // Scalar type
 *     VecAdd,      // Addition
 *     VecScale,    // Scalar multiplication
 *     // ... other functors
 * >;
 * 
 * // Use operations
 * MyVector v1{1, 2}, v2{3, 4};
 * auto sum = MyVectorSpace::addition(v1, v2);
 * auto scaled = MyVectorSpace::scalar_action(2.0, v1);
 * @endcode
 * 
 * @note This structure does not assume finite dimensions or enumerability.
 *       It works equally well with function spaces, symbolic expressions, etc.
 * 
 * @see NormedSpace for extending this to a normed space
 */
template <
    typename ElementType,
    FieldLike ScalarType,
    typename Addition,
    typename ScalarAction,
    typename Negation,
    typename ZeroSupplier,
    typename Equality
>
requires std::default_initializable<Addition> &&
         std::default_initializable<ScalarAction> &&
         std::default_initializable<Negation> &&
         std::default_initializable<ZeroSupplier> &&
         std::default_initializable<Equality> &&
         CallableLike<Addition,     ElementType, const ElementType&, const ElementType&> &&
         CallableLike<ScalarAction, ElementType, const ScalarType&,  const ElementType&> &&
         CallableLike<Negation,     ElementType, const ElementType&> &&
         CallableLike<ZeroSupplier, ElementType> &&
         CallableLike<Equality,     bool,        const ElementType&, const ElementType&>
struct VectorSpace {
    /// @brief Type alias for elements of this vector space
    using element_type = ElementType;
    
    /// @brief Type alias for scalars of this vector space
    using scalar_type  = ScalarType;

    /// @brief Static instance of the addition functor
    static inline constexpr Addition      addition{};
    
    /// @brief Static instance of the scalar multiplication functor
    static inline constexpr ScalarAction  scalar_action{};
    
    /// @brief Static instance of the negation functor
    static inline constexpr Negation      negation{};
    
    /// @brief Static instance of the zero element supplier functor
    static inline constexpr ZeroSupplier  zero_supplier{};
    
    /// @brief Static instance of the equality functor
    static inline constexpr Equality      equality{};

    /**
     * @brief Compute the difference of two elements
     * 
     * Computes element_left - element_right using addition and negation.
     * This is equivalent to: addition(element_left, negation(element_right))
     * 
     * @param element_left The left operand
     * @param element_right The right operand
     * @return The difference element_left - element_right
     * 
     * @note This is a convenience function. The difference operation is
     *       automatically derived from addition and negation.
     */
    static constexpr element_type difference(const element_type& element_left,
                                             const element_type& element_right) {
        return addition(element_left, negation(element_right));
    }
};

/**
 * @brief Concept for types that behave like a VectorSpace
 * 
 * @tparam T The type to check
 * 
 * A type satisfies VectorSpaceLike if it has:
 * - `element_type` and `scalar_type` type aliases
 * - Static members: `addition`, `scalar_action`, `negation`, `zero_supplier`, `equality`
 * - A static `difference` function with the correct signature
 * 
 * This concept allows template functions to work with any VectorSpace-like
 * structure, including custom implementations or extensions.
 * 
 * @note VectorSpace automatically satisfies this concept.
 * 
 * @see VectorSpace
 */
template <typename T>
concept VectorSpaceLike = requires {
    typename T::element_type;
    typename T::scalar_type;
    { T::addition }      ;
    { T::scalar_action } ;
    { T::negation }      ;
    { T::zero_supplier } ;
    { T::equality }      ;
    { T::difference(std::declval<const typename T::element_type&>(),
                    std::declval<const typename T::element_type&>()) }
        -> std::same_as<typename T::element_type>;
};

} // namespace kuukan

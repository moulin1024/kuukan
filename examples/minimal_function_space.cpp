/**
 * @file minimal_function_space.cpp
 * @brief Example demonstrating a function space as a vector space
 * 
 * This example shows how to use kuukan to create a vector space where the
 * elements are functions (specifically, real-valued functions of a real variable).
 * The operations are defined pointwise, and we use symbolic identities for
 * equality checking.
 * 
 * This demonstrates the power of kuukan's abstraction: we can treat infinite-
 * dimensional function spaces exactly like finite-dimensional vector spaces.
 */

#include <iostream>
#include <functional>
#include <string>
#include <cmath>

#include <kuukan/kuukan.hpp>

using ScalarType = double;

/**
 * @brief Element of a function space
 * 
 * Represents a function from reals to reals, along with a symbolic identity
 * string for abstract equality checking. The evaluator is a callable that
 * computes the function value at a point.
 */
struct RealFunctionElement {
    /// @brief Callable that evaluates the function at a point
    std::function<ScalarType(ScalarType)> evaluator;
    
    /// @brief Symbolic representation of the function (for equality checking)
    std::string symbolic_identity;
};

// —— Inject vector space operations (all pointwise definitions, keeping abstraction) ——

/**
 * @brief Functor for adding two functions pointwise
 * 
 * Implements function addition: (f + g)(x) = f(x) + g(x)
 * The symbolic identity is constructed by concatenating the operands.
 */
struct FunctionAddition {
    RealFunctionElement operator()(const RealFunctionElement& function_left,
                                   const RealFunctionElement& function_right) const {
        return RealFunctionElement{
            .evaluator = [lf = function_left.evaluator, rg = function_right.evaluator](ScalarType x) {
                return lf(x) + rg(x);
            },
            .symbolic_identity = "(" + function_left.symbolic_identity + " + " + function_right.symbolic_identity + ")"
        };
    }
};

/**
 * @brief Functor for scalar multiplication of functions
 * 
 * Implements scalar multiplication: (α * f)(x) = α * f(x)
 * The symbolic identity includes the scalar value.
 */
struct FunctionScalarAction {
    /**
     * @brief Multiply a function by a scalar
     * 
     * @param scalar_value The scalar multiplier
     * @param function_value The function to scale
     * @return A new function representing the scaled function
     */
    RealFunctionElement operator()(const ScalarType& scalar_value,
                                   const RealFunctionElement& function_value) const {
        return RealFunctionElement{
            .evaluator = [s = scalar_value, f = function_value.evaluator](ScalarType x) {
                return s * f(x);
            },
            .symbolic_identity = "(" + std::to_string(scalar_value) + " * " + function_value.symbolic_identity + ")"
        };
    }
};

/**
 * @brief Functor for negating a function
 * 
 * Implements negation: (-f)(x) = -f(x)
 * The symbolic identity is the negated function's identity.
 */
struct FunctionNegation {
    RealFunctionElement operator()(const RealFunctionElement& function_value) const {
        return RealFunctionElement{
            .evaluator = [f = function_value.evaluator](ScalarType x) {
                return -f(x);
            },
            .symbolic_identity = "(-" + function_value.symbolic_identity + ")"
        };
    }
};

/**
 * @brief Functor for the zero function
 * 
 * Returns the zero function: 0(x) = 0 for all x
 */
struct FunctionZeroSupplier {
    /**
     * @brief Create the zero function
     * 
     * @return A function that always returns zero
     */
    RealFunctionElement operator()() const {
        return RealFunctionElement{
            .evaluator = [](ScalarType){ return ScalarType{0}; },
            .symbolic_identity = "0"
        };
    }
};

/**
 * @brief Functor for function equality
 * 
 * Compares two functions by their symbolic identity. In a real implementation,
 * you might want to check functional equality (e.g., by comparing values at
 * sample points or using symbolic simplification).
 */
struct FunctionEquality {
    bool operator()(const RealFunctionElement& function_left,
                    const RealFunctionElement& function_right) const {
        return function_left.symbolic_identity == function_right.symbolic_identity;
    }
};

/**
 * @brief Function vector space type
 * 
 * Assembles a complete vector space structure for functions. The operations
 * are all pointwise, and this works with infinite-dimensional function spaces
 * (no dimension assumption).
 * 
 * This is a type alias that instantiates VectorSpace with our function
 * element type and operation functors.
 */
using FunctionVectorSpace = kuukan::VectorSpace<
    RealFunctionElement,             // ElementType
    ScalarType,                      // ScalarType
    FunctionAddition,                // Addition
    FunctionScalarAction,            // ScalarAction
    FunctionNegation,                // Negation
    FunctionZeroSupplier,            // ZeroSupplier
    FunctionEquality                 // Equality
>;

/**
 * @brief Placeholder norm functor
 * 
 * This is a placeholder implementation that demonstrates the interface.
 * A real implementation would compute the supremum norm (sup norm) of a
 * function, which is the maximum absolute value over the domain:
 * 
 *     ||f||_∞ = sup |f(x)|
 * 
 * Computing this typically requires numerical integration or symbolic
 * analysis. Here we just return 0.0 as a placeholder.
 * 
 * @note In a real application, you would implement this using numerical
 *       methods (e.g., sampling and optimization) or symbolic methods
 *       (e.g., symbolic differentiation to find extrema).
 */
struct SupNormPlaceholder {
    /**
     * @brief Compute the norm of a function (placeholder)
     * 
     * @param element The function element
     * @return The norm value (0.0 in this placeholder)
     */
    double operator()(const RealFunctionElement& element) const {
        // Real implementation can be done at a higher level using symbolic/numerical methods; 
        // here only demonstrates type and interface.
        return 0.0;
    }
};

/**
 * @brief Normed function space
 * 
 * Extends the function vector space with a norm, which automatically induces
 * a metric. The distance between two functions f and g is:
 * 
 *     distance(f, g) = ||f - g||
 * 
 * where ||·|| is the norm (sup norm in this case).
 */
using FunctionSupNormedSpace = kuukan::NormedSpace<FunctionVectorSpace, SupNormPlaceholder>;

/**
 * @brief Main function demonstrating function space usage
 * 
 * Creates two functions (sin and cos), performs vector space operations on them,
 * and demonstrates the induced metric interface.
 */
int main() {
    // Create function elements representing sin(x) and cos(x)
    RealFunctionElement sin_function {
        .evaluator = [](ScalarType x){ return std::sin(x); },
        .symbolic_identity = "sin"
    };
    RealFunctionElement cos_function {
        .evaluator = [](ScalarType x){ return std::cos(x); },
        .symbolic_identity = "cos"
    };

    // Perform vector space operations
    // Add functions: sin + cos
    auto sum_function = FunctionVectorSpace::addition(sin_function, cos_function);
    
    // Negate function: -cos
    auto neg_function = FunctionVectorSpace::negation(cos_function);

    // Display symbolic identities
    std::cout << "symbol(sum): " << sum_function.symbolic_identity << "\n";
    std::cout << "symbol(neg): " << neg_function.symbolic_identity << "\n";

    // Compute distance using the induced metric
    // distance(sin, cos) = ||sin - cos|| (placeholder: returns 0.0)
    double abstract_distance = FunctionSupNormedSpace::distance(sin_function, cos_function);
    std::cout << "abstract distance(sin, cos) = " << abstract_distance << "\n";

    return 0;
}

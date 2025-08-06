/**
 * @file Utils.hpp
 * @brief Utility classes and functions for the AvSynth audio plugin
 * @author [Author Name]
 * @date [Date]
 * @version 1.0
 */

#pragma once
#include <type_traits>

/**
 * @brief A simple linear ramp generator for smooth transitions between two values without any branching
 *
 * This template class provides a branchless linear interpolation mechanism that can be used
 * to create smooth transitions between two values over a specified number of steps. It's particularly
 * useful in audio processing where branching can cause performance issues.
 *
 * @tparam T The data type for the ramp values (must be arithmetic or support required operations)
 *
 * @note The type T must either be arithmetic or support the following operations:
 *       - Addition (a + b)
 *       - Subtraction (a - b)
 *       - Multiplication with float (a * f)
 *       - Division by float (a / f)
 */
template <typename T>
    requires std::is_arithmetic_v<T> || requires(T a, T b, float f) {
        { a + b };
        { a - b };
        { a *f };
        { a / f };
    }
class LinearRamp {
  public:
    /**
     * @brief Initializes the ramp with start and end values over a specified number of steps
     *
     * Sets up the linear ramp to transition from the start value to the end value
     * over the given number of steps. The increment per step is calculated automatically.
     *
     * @param start The initial value of the ramp
     * @param end The final value of the ramp
     * @param steps The number of steps to complete the transition (must be > 0 for active ramp)
     *
     * @note If steps <= 0, the ramp will be inactive and return the start value indefinitely
     */
    void reset(T start, T end, int steps) {
        current = start;
        increment = (steps > 0) ? (end - start) / static_cast<std::common_type_t<T, float>>(steps) : T{};
        remainingSteps = steps;
    }

    /**
     * @brief Gets the next value in the ramp sequence
     *
     * Returns the current ramp value and advances the internal state for the next call.
     * Uses branchless logic to avoid performance penalties from conditional statements.
     * When the ramp is complete (remainingSteps <= 0), it continues to return the final value.
     *
     * @return The current ramp value before advancing to the next step
     *
     * @note This method uses branchless programming techniques for optimal performance
     *       in real-time audio processing contexts
     */
    T getNext() {
        T value = current;

        // Active flag: 0 or 1 (branchless approach)
        int active = (remainingSteps > 0);
        auto fActive = static_cast<typename std::common_type<T, float>::type>(active);

        // Only accumulate if active (multiply by 0 or 1)
        current += increment * fActive;
        remainingSteps -= active;

        return value;
    }

  private:
    T current{};           ///< Current ramp value
    T increment{};         ///< Increment value per step
    int remainingSteps = 0; ///< Number of steps remaining in the ramp
};
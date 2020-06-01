#pragma once

#include <string>

namespace vcl
{

/** \ingroup math
 * @{
*/
bool is_equal(int a, int b);                    /**< a==b */
bool is_equal(unsigned int a, unsigned int b);  /**< a==b */
bool is_equal(size_t a, size_t b);              /**< a==b */
bool is_equal(float a, float b);                /**< |a-b|<epsilon */
bool is_equal(double a, double b);              /**< |a-b|<epsilon */


constexpr size_t size(int a);                   /**< 1 */
constexpr size_t size(unsigned int a);          /**< 1 */
constexpr size_t size(unsigned long a);         /**< 1 */
constexpr size_t size(float a);                 /**< 1 */
constexpr size_t size(double a);                /**< 1 */

/** max(min(x, x_max), x_min) */
template <typename T1, typename T2> T1 clamp(T1 x, T2 x_min, T2 x_max);

/** @} */

}

// Template implementation
namespace vcl
{
template <typename T1, typename T2>
T1 clamp(T1 x, T2 x_min, T2 x_max)
{
    if( x<x_min )
        return x_min;
    if( x>x_max )
        return x_max;
    return x;
}



}

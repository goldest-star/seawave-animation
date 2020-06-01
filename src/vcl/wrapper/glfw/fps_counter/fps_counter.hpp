#pragma once

#include "vcl/wrapper/glfw/glfw.hpp"

namespace vcl
{

/** Helper to computer FPS using time value from GLFW library
 *
 * The class store the number of drawn frame (frame count) and the last time since frame count has been reset (time_previous).
 * The current elapsed time is updated internally using glfwGetTime()
 * The time between two update (in seconds) is parameterized with the variable time_update.
 *
 * Usage:
 * - Call update at each drawing call (increase the number of frame_count)
 * - If update returns true:
 * - - Print the current fps
 * - - Restart (reset frame_count and time_previous)
*/
class glfw_fps_counter
{
public:
    glfw_fps_counter();

    /** Add one to frame_count and check if elapsed time is greater the time_update
     * Return true if new fps can be drawn and the class can be reset.  */
    bool update();

    /** Return the number of frame drawn since last time */
    int fps() const;

    /** Reset frame_count to 0, and time_previous to current time.
     * Should be called every time update returns true. */
    void reset();

private:

    /** Minimum amount of time allowed between two updates (in seconds) */
    static constexpr double time_update = 3.0;

    /** Store the number of frame drawn (increased using update()) */
    int frame_count;
    /** Store the time at the last reset (or creation) */
    double time_previous;

};


}

#pragma once

#include "../curve_drawable/curve_drawable.hpp"

namespace vcl
{

/** Structure storing a FIFO curve.
 * The structure add points to the curve until reaching max_size.
 * After reaching max_size, the older points are removed when a new one is added.
 * The structure can typically be used to draw a portion of trajectory of a point through time. */
class curve_dynamic_drawable: public curve_drawable
{
public:

    curve_dynamic_drawable(size_t max_size = 20);

    /** Clear all stored points */
    void clear();

    /** Add a new point (remove older one to keep a size <= max_size) */
    void add_point(const vec3& p);

    /** Display the curve */
    void draw(GLuint shader, const camera_scene& camera);

private:

    /** Vector of points currently stored */
    std::vector<vec3> position_stored;
    /** Maximum number of points stored in the curve*/
    size_t max_size;
    /** Value set to true after addition of a new point. Indicates that the VBO need to be updated. */
    bool need_update;
    /** Value set to true only once at the creation of the object. Allows to intialize VBO before the first draw. */
    bool first_time;
};

}

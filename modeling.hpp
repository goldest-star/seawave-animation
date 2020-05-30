#pragma once

#include "main/scene_base/base.hpp"

#ifdef SCENE_3D_GRAPHICS

struct vec3t {
    vcl::vec3 p; // position
    float t;     // time
};

// Stores some parameters that can be set from the GUI
struct gui_scene_structure
{
    bool wireframe = false;
    bool display_keyframe = true;
    bool display_polygon = true;
    float height = 0.6f;
    float scaling = 3.0f;
    int octave = 7;
    float persistency = 0.4f;
};

struct particle_structure
{
    vcl::vec3 p; // Position
    vcl::vec3 v; // Speed
};

struct scene_model : scene_base
{

    /** A part must define two functions that are called from the main function:
     * setup_data: called once to setup data before starting the animation loop
     * frame_draw: called at every displayed frame within the animation loop
     *
     * These two functions receive the following parameters
     * - shaders: A set of shaders.
     * - scene: Contains general common object to define the 3D scene. Contains in particular the camera.
     * - data: The part-specific data structure defined previously
     * - gui: The GUI structure allowing to create/display buttons to interact with the scene.
    */

    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void set_gui();

    // visual representation of a surface
    vcl::mesh_drawable terrain;
    vcl::mesh_drawable island;
    std::vector<vcl::vec3> box_position;
    vcl::mesh_drawable box;
    std::vector<vcl::vec3> fish_position;
    vcl::mesh_drawable fish;
    vcl::mesh_drawable boat;
    vcl::mesh_drawable sky;
    vcl::mesh_drawable flag;
    vcl::mesh_drawable missle;

    std::list<particle_structure> particles; // Storage of all currently active particles

    const int N_box = 30;
    const int N_fish = 30;

    GLuint texture_id;
    GLuint fish_id;
    GLuint box_id;
    GLuint boat_id;
    GLuint skybox_id;
    GLuint island_id;
    GLuint flag_id;
    

    void update_terrain();
    void update_island();
    void update_box();
    void update_fish();

    void set_creature_rotation(float t_creature);
    void set_data_creature_animation(std::map<std::string, GLuint>& shaders);
    void set_plane_rotation(float t_creature);
    void set_data_plane_animation(std::map<std::string, GLuint>& shaders);
    

    // Data (p_i,t_i)

    vcl::buffer<vec3t> keyframes_creature; // Given (position,time)
    vcl::buffer<vec3t> keyframes_plane;
  
    vcl::hierarchy_mesh_drawable creature;
    vcl::hierarchy_mesh_drawable plane;

    gui_scene_structure gui_scene;

    vcl::timer_interval timer_scaling;
    vcl::timer_interval timer_height;
    vcl::timer_interval timer_creature;
    vcl::timer_interval timer_plane;
    vcl::timer_event timer_missle;
};

#endif
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
    std::vector<vcl::vec3> tree_position;
    vcl::mesh_drawable tree;
    std::vector<vcl::vec3> mush_position;
    vcl::mesh_drawable mush;
    vcl::mesh_drawable sky;

    const int N_tree = 30;
    const int N_mushroom = 50;
    const int N_grass = 30;

    GLuint texture_id;
    GLuint grass_id;
    GLuint skybox_id;

    vcl::mesh_drawable grass;
    std::vector<vcl::vec3> grass_position;

    void update_terrain();
    void update_tree();
    void update_mushroom();
    void update_grass();

    void set_creature_rotation(float t_creature);
    void set_data_creature_animation(std::map<std::string, GLuint>& shaders);
    void set_plane_rotation(float t_creature);
    void set_data_plane_animation(std::map<std::string, GLuint>& shaders);
    
    // Called every time the mouse is clicked
    void mouse_click(scene_structure& scene, GLFWwindow* window, int button, int action, int mods);
    // Called every time the mouse is moved
    void mouse_move(scene_structure& scene, GLFWwindow* window);

    // Data (p_i,t_i)

    vcl::buffer<vec3t> keyframes_creature; // Given (position,time)
    vcl::buffer<vec3t> keyframes_plane;

    vcl::mesh_drawable point_visual;                       // moving point
    vcl::mesh_drawable keyframe_visual;                    // keyframe samples
    vcl::mesh_drawable keyframe_picked;                    // showing the picked sample
    vcl::segment_drawable_immediate_mode segment_drawer;   // used to draw segments between keyframe samples
    vcl::curve_dynamic_drawable trajectory;                // Draw the trajectory of the moving point as a curve
    
    vcl::hierarchy_mesh_drawable creature;
    vcl::hierarchy_mesh_drawable plane;
    // Store the index of a selected sphere
    int picked_object;

    gui_scene_structure gui_scene;

    vcl::timer_interval timer_scaling;
    vcl::timer_interval timer_height;
    vcl::timer_interval timer_creature;
    vcl::timer_interval timer_plane;

};

#endif



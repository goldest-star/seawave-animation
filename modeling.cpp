
#include "modeling.hpp"


#ifdef SCENE_3D_GRAPHICS

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;



static size_t index_at_value(float t, const vcl::buffer<vec3t>& v);
static vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float K);
static vec3 cardinal_spline_interpolation_der(float t, float t0, float t1, float t2, float t3, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float K);

float evaluate_terrain_z(float u, float v);
float evaluate_perlin_terrain_z(float u, float v, const gui_scene_structure& gui_scene);
vec3 evaluate_terrain(float u, float v);
vec3 evaluate_perlin_terrain(float u, float v, const gui_scene_structure& gui_scene);
mesh create_terrain(const gui_scene_structure& gui_scene);
mesh create_cylinder(float radius, float height);
mesh create_cone(float radius, float height, float z_offset);
mesh create_tree_foliage(float radius, float height, float z_offset, float cylinder_height);
mesh create_tree(float cylinder_rad, float cylinder_height, float tree_rad, float tree_height, float tree_z_offset, const vec3& tree_col, const vec3& cylin_col);
mesh create_mushroom(float cylinder_rad, float cylinder_height, float cone_rad, float cone_height, const vec3& cylin_col, const vec3& cone_col);
mesh create_grass(float length, float width);
mesh create_sky(float b);
hierarchy_mesh_drawable create_creature();
hierarchy_mesh_drawable create_plane();

/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_model::setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& ){
    update_terrain();

    //Create moving creature
    creature = create_creature();
    creature.set_shader_for_all_elements(shaders["mesh"]);

    //Create moving plane
    plane = create_plane();
    plane.set_shader_for_all_elements(shaders["mesh"]);

    //Fill in grass position
    for (int i = 0; i < N_grass; ++i) {
        float u = rand_interval(0, 1);
        float v = rand_interval(0, 1);
        vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_perlin_terrain_z(u,v,gui_scene)};
        grass_position.push_back(pos);
    }

    //create grass
    grass = create_grass(0.2f, 0.4f);
    grass.uniform.shading = { 1,0,0 }; // set pure ambiant component (no diffuse, no specular) - allow to only see the color of the texture

    // Load a texture (with transparent background)
    grass_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/billboard_grass.png"), GL_REPEAT, GL_REPEAT);

    //Fill in tree position
    for (int i = 0; i < N_tree; ++i) {
        float u = rand_interval(0, 1);
        float v = rand_interval(0, 1);
        vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_perlin_terrain_z(u,v,gui_scene) };
        tree_position.push_back(pos);
    }

    //Create tree 
    const vec3 tree_col = { 0.0f, 0.85f, 0.5f };
    const vec3 cylin_col = { 0.87f, 0.72f, 0.52f };
    tree = create_tree(0.1f,0.5f,0.4f,0.5f,0.2f,tree_col,cylin_col);
    tree.uniform.shading.specular = 0.0f;

    //Fill in mush position
    for (int i = 0; i < N_mushroom; ++i) {
        float u = rand_interval(0, 1);
        float v = rand_interval(0, 1);
        vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_perlin_terrain_z(u,v,gui_scene) };
        mush_position.push_back(pos);
    }

    //Create mushroom
    const vec3 cone_col = { 0.9f, 0.1f, 0.0f };
    const vec3 mush_col = { 0.9f, 1.0f, 0.5f };
    mush = create_mushroom(0.03f, 0.1f, 0.1f, 0.05f, mush_col, cone_col);
    mush.uniform.shading.specular = 0.0f;

    // Load a texture image on GPU and stores its ID
    texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/sea2.png"));
    
    timer_scaling.t_min = 1;
    timer_scaling.t_max = 100;
    timer_scaling.t = timer_scaling.t_min;
    timer_height.t_min = 100;
    timer_height.t_max = 200;
    timer_height.t = timer_height.t_min;

    set_data_creature_animation(shaders);
    set_data_plane_animation(shaders);

    //Create skybox
    skybox_id = create_texture_gpu(image_load_png("scenes/3D_graphics/02_texture/assets/sky.png"));
    sky = mesh_drawable(create_sky(50));
    sky.uniform.shading = { 1,0,0 };
    sky.uniform.color = { 1.0f,1.0f,1.0f };
    sky.uniform.transform.translation = vec3(0, 0, -25.0);


    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 15.0f;
    scene.camera.apply_rotation(0,0,1.0f,1.2f);

    
}



/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_model::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    timer_scaling.update();
    const float t_scaling = timer_scaling.t/10;
    gui_scene.scaling = t_scaling;
    
    timer_height.update();
    const float t_height = timer_height.t/200;
    gui_scene.height = 1.5 - t_height;

    timer_creature.update();
    const float t_creature = timer_creature.t;

    timer_plane.update();
    const float t_plane = timer_plane.t;

    set_gui();

    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe

    // Display terrain
    // Before displaying a textured surface: bind the associated texture id
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glPolygonOffset( 1.0, 1.0 );
    draw(terrain, scene.camera, shaders["mesh"]);


    for (int i = 0; i < N_tree; ++i) {
        tree.uniform.transform.translation = tree_position[i];
        draw(tree, scene.camera, shaders["mesh"]);
    }
    for (int i = 0; i < N_mushroom; ++i) {
        mush.uniform.transform.translation = mush_position[i];
        draw(mush, scene.camera, shaders["mesh"]);
    }
    // After the surface is displayed it is safe to set the texture id to a white image
    //  Avoids to use the previous texture for another object
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    
    // Display skybox
    glBindTexture(GL_TEXTURE_2D, skybox_id);
    draw(sky, scene.camera, shaders["mesh"]);
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if( gui_scene.wireframe ) { // wireframe if asked from the GUI
        glPolygonOffset( 1.0, 1.0 );
        draw(terrain, scene.camera, shaders["wireframe"]);
        for (int i = 0; i < N_tree; ++i) {
            tree.uniform.transform.translation = tree_position[i];
            draw(tree, scene.camera, shaders["mesh"]);
        }
        for (int i = 0; i < N_mushroom; ++i) {
            mush.uniform.transform.translation = mush_position[i];
            draw(mush, scene.camera, shaders["mesh"]);
        }
    }

    // Enable use of alpha component as color blending for transparent elements
    //  new color = previous color + (1-alpha) current color
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);


    glBindTexture(GL_TEXTURE_2D, grass_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // avoids sampling artifacts

    // Display a billboard always facing the camera direction
    // ********************************************************** //
    grass.uniform.transform.rotation = scene.camera.orientation;
    for (int i = 0; i < N_grass; ++i) {
        grass.uniform.transform.translation = grass_position[i];
        draw(grass, scene.camera, shaders["mesh"]);
    }
    if (gui_scene.wireframe)
        for (int i = 0; i < N_grass; ++i) {
            grass.uniform.transform.translation = grass_position[i];
            draw(grass, scene.camera, shaders["mesh"]);
        }
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    update_terrain();
    update_tree();
    update_mushroom();
    update_grass();
    
    const size_t N = keyframes_creature.size();
    
    set_creature_rotation(t_creature);
    set_plane_rotation(t_plane);

    // Draw current position
    //point_visual.uniform.transform.translation = p;

    draw(creature, scene.camera, shaders["mesh"]);
    draw(plane, scene.camera, shaders["mesh"]);
    // Draw moving point trajectory



    // Draw sphere at each keyframe position
    if (gui_scene.display_keyframe) {
        for (size_t k = 0; k < N; ++k) {
            const vec3& p_keyframe = keyframes_creature[k].p;
            keyframe_visual.uniform.transform.translation = p_keyframe;
            draw(keyframe_visual, scene.camera);
        }
    }

    // Draw selected sphere in red
    if (picked_object != -1) {
        const vec3& p_keyframe = keyframes_creature[picked_object].p;
        keyframe_picked.uniform.transform.translation = p_keyframe;
        draw(keyframe_picked, scene.camera);
    }


    // Draw segments between each keyframe
    if (gui_scene.display_polygon) {
        for (size_t k = 0; k < keyframes_creature.size() - 1; ++k) {
            const vec3& pa = keyframes_creature[k].p;
            const vec3& pb = keyframes_creature[k + 1].p;
            segment_drawer.uniform_parameter.p1 = pa;
            segment_drawer.uniform_parameter.p2 = pb;
            segment_drawer.draw(shaders["segment_im"], scene.camera);
        }
    }


    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);
}

void scene_model::update_terrain()
{
    // Clear memory in case of pre-existing terrain
    terrain.clear();

    // Create visual terrain surface
    terrain = create_terrain(gui_scene);
    terrain.uniform.color = { 1.0f, 1.0f, 1.0f };
    terrain.uniform.shading.specular = 0.0f;
}
void scene_model::update_tree() {
    
    for (int i = 0; i < N_tree; ++i) {
        float u = tree_position[i][0];
        float v = tree_position[i][1];
        vec3 pos = { u, v,  evaluate_perlin_terrain_z(u/20+0.5f,v/20+0.5f,gui_scene) };
        tree_position[i] = pos;
    }
}

void scene_model::update_mushroom() {

    for (int i = 0; i < N_mushroom; ++i) {
        float u = mush_position[i][0];
        float v = mush_position[i][1];
        vec3 pos = { u, v,  evaluate_perlin_terrain_z(u / 20 + 0.5f,v / 20 + 0.5f,gui_scene) };
        mush_position[i] = pos;
    }
}

void scene_model::update_grass() {

    for (int i = 0; i < N_grass; ++i) {
        float u = grass_position[i][0];
        float v = grass_position[i][1];
        vec3 pos = { u, v,  evaluate_perlin_terrain_z(u / 20 + 0.5f,v / 20 + 0.5f,gui_scene) };
        grass_position[i] = pos;
    }
}
// Evaluate height of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_z(float u, float v)
{
    const int N = 4;
    const vec2 u0[4] = { {0.0f,0.0f},{0.5f,0.5f},{0.2f,0.7f},{0.8f,0.7f} };
    const float h[4] = { 3.0f,-1.5f,1.0f,2.0f };
    const float sigma[4] = { 0.5f,0.15f,0.2f,0.2f };
    float z = 0;
    for (int i = 0; i < N; ++i) {
        float d = norm(vec2(u, v) - u0[i]) / sigma[i];
        z += h[i] * std::exp(-d * d);
    }
    return z;
}

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
vec3 evaluate_terrain(float u, float v)
{
    const float x = 20*(u-0.5f);
    const float y = 20*(v-0.5f);
    const float z = evaluate_terrain_z(u,v);

    return {x,y,z};
}

float evaluate_perlin_terrain_z(float u, float v, const gui_scene_structure& gui_scene) {
    // get gui parameters
    const float scaling = gui_scene.scaling;
    const int octave = gui_scene.octave;
    const float persistency = gui_scene.persistency;
    const float height = gui_scene.height;

    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);
    const float z = height * noise;
    return z;
}

vec3 evaluate_perlin_terrain(float u, float v, const gui_scene_structure& gui_scene) {
    const float x = 20 * (u - 0.5f);
    const float y = 20 * (v - 0.5f);
    const float z = evaluate_perlin_terrain_z(u, v,gui_scene);
    return{ x,y,z };
}
// Generate terrain mesh
mesh create_terrain(const gui_scene_structure& gui_scene)
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku) {
        for(size_t kv=0; kv<N; ++kv) {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            //terrain.position[kv+N*ku] = evaluate_terrain(u,v);
            terrain.position[kv + N * ku] = evaluate_perlin_terrain(u,v,gui_scene);
            terrain.texture_uv[kv + N * ku] = {5*ku/(float)N,5*kv/(float)N};
         
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for(unsigned int ku=0; ku<Ns-1; ++ku) {
        for(unsigned int kv=0; kv<Ns-1; ++kv) {
            const unsigned int idx = kv + N*ku; // current vertex offset

            const uint3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const uint3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}

mesh create_cylinder(float radius, float height) {
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    for (size_t k = 0; k < N; ++k) {
        const float u = k / float(N);
        const vec3 p = { radius * std::cos(2 * 3.14f * u), radius * std::sin(2 * 3.14f * u), 0.0f };
        m.position.push_back(p);
        m.position.push_back(p + vec3(0, 0, height));
    }

    // Connectivity
    for (size_t k = 0; k < N; ++k) {
        const unsigned int u00 = 2 * k;
        const unsigned int u01 = (2 * k + 1) % (2 * N);
        const unsigned int u10 = (2 * (k + 1)) % (2 * N);
        const unsigned int u11 = (2 * (k + 1) + 1) % (2 * N);

        const uint3 t1 = { u00, u10, u11 };
        const uint3 t2 = { u00, u11, u01 };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }

    return m;
}

mesh create_cone(float radius, float height, float z_offset) {
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    
    for (size_t k = 0; k < N; ++k) {
        const float u = k / float(N);
        const vec3 p = { radius * std::cos(2 * 3.14f * u), radius * std::sin(2 * 3.14f * u), z_offset };
        m.position.push_back(p);
    }

    const vec3 base = { 0.0f, 0.0f, z_offset };
    m.position.push_back(base);
    const vec3 top = { 0.0f, 0.0f, height + z_offset };
    m.position.push_back(top);

    // Connectivity
    for (size_t k = 0; k < N; ++k) {
        const unsigned int base = N;
        const unsigned int top = N + 1;
        const unsigned int u0 = k % N;
        const unsigned int u1 = (k + 1) % N;

        const uint3 t1 = { u0, u1, base };
        const uint3 t2 = { u0, u1, top };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }
    return m;
}

mesh create_tree_foliage(float radius, float height, float z_offset, float cylinder_height) {
    mesh m = create_cone(radius, height, cylinder_height);
    m.push_back(create_cone(radius, height, cylinder_height + z_offset));
    m.push_back(create_cone(radius, height, cylinder_height + 2 * z_offset));

    return m;
}

mesh create_tree(float cylinder_rad, float cylinder_height, float tree_rad, float tree_height, float tree_z_offset, const vec3& tree_col, const vec3& cylin_col) {
    mesh m = create_cylinder(cylinder_rad,cylinder_height);
    m.fill_color_uniform(cylin_col);
    mesh n = create_tree_foliage(tree_rad, tree_height, tree_z_offset, cylinder_height);
    n.fill_color_uniform(tree_col);
    m.push_back(n);
    return m;
}

mesh create_mushroom(float cylinder_rad, float cylinder_height, float cone_rad, float cone_height, const vec3& cylin_col, const vec3& cone_col) {
    mesh m = create_cylinder(cylinder_rad, cylinder_height);
    m.fill_color_uniform(cylin_col);
    mesh n = create_cone(cone_rad, cone_height, cylinder_height);
    n.fill_color_uniform(cone_col);
    m.push_back(n);
    return m;
}

mesh create_grass(float length, float width) {
    // Create a quad with (u,v)-texture coordinates
    mesh m;
    m.position = { {-length,0,0}, { length,0,0}, { length, width,0}, {-length, width,0} };
    m.texture_uv = { {0,1}, {1,1}, {1,0}, {0,0} };
    m.connectivity = { {0,1,2}, {0,2,3} };
    return m;
}

hierarchy_mesh_drawable create_creature() {
    hierarchy_mesh_drawable hierarchy;
    const float radius_body = 0.25f;
    const float radius_arm = 0.05f;
    const float length_arm = 0.2f;
    const float radius_leg = 0.1f;
    const float length_leg = 0.5f;
    // The geometry of the body is a sphere
    mesh_drawable body = mesh_drawable(mesh_primitive_sphere(radius_body, { 0,0,0 }, 40, 40));
    mesh_drawable bigbody = mesh_drawable(mesh_primitive_sphere(2 * radius_body, { 0,0,0 }, 40, 40));
    bigbody.uniform.color = { 0.55f,0.09f,0.09f };

    // Geometry of the eyes: black spheres
    mesh_drawable eye = mesh_drawable(mesh_primitive_sphere(0.05f, { 0,0,0 }, 20, 20));
    eye.uniform.color = { 0,0,0 };

    mesh_drawable mouth = mesh_drawable(mesh_primitive_cone());
    mouth.uniform.color = { 0.2f,0.3f,0.0f };

    // Shoulder part and arm are displayed as cylinder
    mesh_drawable shoulder = mesh_primitive_quad({ 0,0,0 }, { length_leg,0,0 });
    mesh_drawable arm = mesh_primitive_torus(8 * radius_arm, radius_arm, { 0,0,0 }, { 0,0,length_arm});
    shoulder.uniform.color = { 0.0f,0.3f,0.4f };
    arm.uniform.color = { 0.76f,0.1f,0.0f };
    
    // An elbow displayed as a sphere
    mesh_drawable elbow = mesh_primitive_sphere(0.055f);

    mesh_drawable leg = mesh_primitive_cylinder(radius_leg, { 0,0,0 }, { -length_leg, 0, 0 });
    leg.uniform.color = { 1.0f,0.49f,0.0f };
    mesh_drawable leg2 = mesh_primitive_cylinder(radius_leg, { 0,0,0 }, { -3 * length_leg, -3 * length_leg, 0 });
    leg2.uniform.color = { 1.0f,0.49f,0.0f };
    mesh_drawable knee = mesh_primitive_sphere(0.15f);
    knee.uniform.color = { 0.55f,0.47f,0.14f };
    
    // Build the hierarchy:
    // Syntax to add element
    //   hierarchy.add(visual_element, element_name, parent_name, (opt)[translation, rotation])
    hierarchy.add(bigbody, "bigbody");
    hierarchy.add(body, "body", "bigbody", 2 * radius_body * vec3(1 / 3.0f, 1 / 2.0f, 1 / 1.5f));

    // Eyes positions are set with respect to some ratio of the
    hierarchy.add(eye, "eye_left", "body", radius_body * vec3(1 / 3.0f, 1 / 2.0f, 1 / 1.5f));
    hierarchy.add(eye, "eye_right", "body", radius_body * vec3(-1 / 3.0f, 1 / 2.0f, 1 / 1.5f));
    hierarchy.add(mouth, "mouth", "body", radius_body * vec3(0.0f, 0.1f, 0.1f));

    // Set the left part of the body arm: shoulder-elbow-arm
    hierarchy.add(shoulder, "shoulder_left", "bigbody", { -2 * radius_body + 0.05f,0,0 }); // extremity of the spherical body
    hierarchy.add(elbow, "elbow_left", "shoulder_left", { -length_arm,0,0 });     // place the elbow the extremity of the "shoulder cylinder"
    hierarchy.add(arm, "arm_bottom_left", "elbow_left");                        // the arm start at the center of the elbow

    // Set the right part of the body arm: similar to the left part excepted a symmetry is applied along x direction for the shoulder
    hierarchy.add(shoulder, "shoulder_right", "bigbody", { {2 * radius_body - 0.05f,0,0}, {-1,0,0, 0,1,0, 0,0,1}/*Symmetry*/ });
    hierarchy.add(elbow, "elbow_right", "shoulder_right", { -length_arm,0,0 });
    hierarchy.add(arm, "arm_bottom_right", "elbow_right");

    hierarchy.add(leg, "leg_left", "bigbody", { { -0.1f,-2 * radius_body + 0.05f,0 },{0.6f,-0.8f,0, 0.8f,0.6f,0, 0,0,1} });
    hierarchy.add(knee, "knee_left", "leg_left", { -length_leg,0,0 });
    hierarchy.add(leg2, "leg2_left", "knee_left", { {0,0,0} });
    hierarchy.add(knee, "knee2_left", "leg2_left", { -3 * length_leg,-3 * length_leg,0 });
    hierarchy.add(leg2, "leg3_left", "knee2_left", { 0,0,0 });
    hierarchy.add(leg, "leg_right", "bigbody", { { 0.1f,-2 * radius_body + 0.05f,0 },{-0.6f,-0.8f,0, 0.8f,-0.6f,0, 0,0,1} });
    hierarchy.add(knee, "knee_right", "leg_right", { -length_leg,0,0 });
    hierarchy.add(leg2, "leg2_right", "knee_right", { {0,0,0} });
    hierarchy.add(knee, "knee2_right", "leg2_right", { -3 * length_leg, -3 * length_leg,0 });
    hierarchy.add(leg2, "leg3_right", "knee2_right", { 0,0,0 });

    return hierarchy;
}

hierarchy_mesh_drawable create_plane() {
    hierarchy_mesh_drawable hierarchy;
    const float radius_body = 0.4f;
    const float height_body = 0.8f;
    const float height_wing = 0.3f;
    mesh_drawable bigbody = mesh_drawable(create_cone(radius_body, height_body, 0));
    bigbody.uniform.transform.scaling_axis = { 0.2f,1.0f,1.0f };
    bigbody.uniform.color = { 0.5f,0.3f,0.1f };
    mesh_drawable head = mesh_drawable(create_cone(radius_body / 2.0f, height_body / 2.0f, 2 / 3.0f * height_body));
    head.uniform.transform.scaling_axis = { 0.2f,1.0f,1.0f };

    mesh wing1;
    wing1.position = { vec3(0,radius_body / 2,0), vec3(0,radius_body / 2,height_body / 2), vec3(height_wing,radius_body / 2,0) };         // 3D-coordinates
    wing1.connectivity = { {0,1,2} };
    mesh wing2;
    wing2.position = { vec3(0,-radius_body / 2,0), vec3(0,-radius_body / 2,height_body / 2), vec3(height_wing,-radius_body / 2,0) };         // 3D-coordinates
    wing2.connectivity = { {0,1,2} };

    hierarchy.add(bigbody, "bigbody");
    hierarchy.add(head, "head", "bigbody");
    hierarchy.add(wing1, "wing1", "bigbody");
    hierarchy.add(wing2, "wing2", "bigbody");

    return hierarchy;
}
mesh create_sky(float b) {
    mesh sky;

    // Geometry (adding points)
    const vec3 p1 = { -b / 2 , -b / 2, 0 };
    const vec3 p2 = { -b / 2 , -b / 2 , b };
    const vec3 p3 = { -b / 2 , b / 2 , 0 };
    const vec3 p4 = { -b / 2 , b / 2, b };
    const vec3 p5 = { b / 2 , -b / 2 , 0 };
    const vec3 p6 = { b / 2 , -b / 2 , b };
    const vec3 p7 = { b / 2 , b / 2 , 0 };
    const vec3 p8 = { b / 2 , b / 2 , b };

    // Adding each vertex 3 times
    for (int i = 0; i < 3; i++) {
        sky.position.push_back(p1);
        sky.position.push_back(p2);
        sky.position.push_back(p3);
        sky.position.push_back(p4);
        sky.position.push_back(p5);
        sky.position.push_back(p6);
        sky.position.push_back(p7);
        sky.position.push_back(p8);
    }

    // Connectivity
    const uint3 t1 = { 0,1,2 };
    const uint3 t2 = { 1,2,3 };
    sky.connectivity.push_back(t1);
    sky.connectivity.push_back(t2);
    const uint3 t3 = { 4,5,6 };
    const uint3 t4 = { 5,6,7 };
    sky.connectivity.push_back(t3);
    sky.connectivity.push_back(t4);

    const uint3 t5 = { 8,12,13 };
    const uint3 t6 = { 8,13,9 };
    sky.connectivity.push_back(t5);
    sky.connectivity.push_back(t6);
    const uint3 t7 = { 10,11,14 };
    const uint3 t8 = { 11,14,15 };
    sky.connectivity.push_back(t7);
    sky.connectivity.push_back(t8);

    const uint3 t9 = { 17,19,21 };
    const uint3 t10 = { 19,21,23 };
    sky.connectivity.push_back(t9);
    sky.connectivity.push_back(t10);
    const uint3 t11 = { 16,18,20 };
    const uint3 t12 = { 18,20,22 };
    sky.connectivity.push_back(t11);
    sky.connectivity.push_back(t12);

    // texture covering
    sky.texture_uv = { {0.252,0.66}, {0.252,0.333}, {0.5,0.66}, {0.499,0.335},
    {0.998,0.66}, {0.998,0.335}, {0.75,0.66}, {0.75,0.335},
    {0.252,0.66}, {0.252,0.335}, {0.5,0.66}, {0.499,0.335},
    {0.002,0.66}, {0.002,0.335}, {0.75,0.66}, {0.75,0.335},
    {0.252,0.66}, {0.252,0.335}, {0.499,0.66}, {0.499,0.335},
    {0.252,1.0}, {0.252,0.002}, {0.499,1.0}, {0.499,0.002}
    };
    return sky;
}
void scene_model::mouse_click(scene_structure& scene, GLFWwindow* window, int, int, int) {
    // Mouse click is used to select a position of the control polygon
    // ******************************************************************** //

    // Cursor coordinates
    const vec2 cursor = glfw_cursor_coordinates_window(window);

    // Check that the mouse is clicked (drag and drop)
    const bool mouse_click_left = glfw_mouse_pressed_left(window);
    const bool key_shift = glfw_key_shift_pressed(window);

    // Check if shift key is pressed
    if (mouse_click_left && key_shift) {
        // Create the 3D ray passing by the selected point on the screen
        const ray r = picking_ray(scene.camera, cursor);

        // Check if this ray intersects a position (represented by a sphere)
        //  Loop over all positions and get the intersected position (the closest one in case of multiple intersection)
        const size_t N = keyframes_creature.size();
        picked_object = -1;
        float distance_min = 0.0f;
        for (size_t k = 0; k < N; ++k) {
            const vec3 c = keyframes_creature[k].p;
            const picking_info info = ray_intersect_sphere(r, c, 0.1f);

            if (info.picking_valid) {// the ray intersects a sphere
                const float distance = norm(info.intersection - r.p); // get the closest intersection
                if (picked_object == -1 || distance < distance_min) {
                    distance_min = distance;
                    picked_object = k;
                }
            }
        }
    }

}

void scene_model::mouse_move(scene_structure& scene, GLFWwindow* window) {

    const bool mouse_click_left = glfw_mouse_pressed_left(window);
    const bool key_shift = glfw_key_shift_pressed(window);
    if (mouse_click_left && key_shift && picked_object != -1) {
        // Translate the selected object to the new pointed mouse position within the camera plane
        // ************************************************************************************** //

        // Get vector orthogonal to camera orientation
        const mat4 M = scene.camera.camera_matrix();
        const vec3 n = { M(0,2),M(1,2),M(2,2) };

        // Compute intersection between current ray and the plane orthogonal to the view direction and passing by the selected object
        const vec2 cursor = glfw_cursor_coordinates_window(window);
        const ray r = picking_ray(scene.camera, cursor);
        vec3& p0 = keyframes_creature[picked_object].p;
        const picking_info info = ray_intersect_plane(r, n, p0);

        // translate the position
        p0 = info.intersection;

    }
}

void scene_model::set_data_creature_animation(std::map<std::string, GLuint>& shaders){
    // Initial Keyframe data vector of (position, time)
    keyframes_creature = { { {-10,0,2}   , 0.0f  },
                  { {5,-7.5,2} , 10.0f  },
                  { {0,0,2}    , 20.0f  },
                  { {0,5,1.8}    , 25.0f  },
                  { {5,5,2}    , 30.0f  },
                  { {5,5,3}    , 35.0f  },
                  { {5,-5,2.3}  , 37.5f  },
                  { {2.5,5, 2} , 45.0f  },
                  { {2.5,-3,2} , 50.0f  },
                  { {-5,-3,2}   , 60.0f  },
                  { {5,-7.5,2} , 70.0f },
                  { {-10,-7.5,2}, 80.0f },
    };

    // Set timer bounds
    // You should adapt these extremal values to the type of interpolation
    timer_creature.t_min = keyframes_creature[1].t;                   // first time of the keyframe
    timer_creature.t_max = keyframes_creature[keyframes_creature.size() - 2].t;  // last time of the keyframe
    timer_creature.t = timer_creature.t_min;

    // Prepare the visual elements
    point_visual = mesh_primitive_sphere();
    point_visual.shader = shaders["mesh"];
    point_visual.uniform.color = { 0,0,1 };
    point_visual.uniform.transform.scaling = 0.04f;

    keyframe_visual = mesh_primitive_sphere();
    keyframe_visual.shader = shaders["mesh"];
    keyframe_visual.uniform.color = { 1,1,1 };
    keyframe_visual.uniform.transform.scaling = 0.05f;

    keyframe_picked = mesh_primitive_sphere();
    keyframe_picked.shader = shaders["mesh"];
    keyframe_picked.uniform.color = { 1,0,0 };
    keyframe_picked.uniform.transform.scaling = 0.055f;

    segment_drawer.init();
    timer_creature.scale = 0.5f;
}
void scene_model::set_data_plane_animation(std::map<std::string, GLuint>& shaders) {
    keyframes_plane = { { {-2,2,2.5}   , 0.0f  },
                  { {0,2,2.5}    , 1.0f  },
                  { {2,2,2.5}    , 2.0f  },
                  { {2,4,2.5}    , 2.5f  },
                  { {4,4,2.5}    , 3.0f  },
                  { {4,4,3.5}    , 3.5f  },
                  { {4,0,4}  , 3.75f  },
                  { {3,-2,3.5} , 4.5f  },
                  { {3,-2,2.5} , 5.0f  },
                  { {2,-2,2.5}   , 6.0f  },
                  { {0,2,2.5} , 7.0f },
                  { {-2,-1,2.5}, 8.0f },
    };
    timer_plane.t_min = keyframes_plane[1].t;                   // first time of the keyframe
    timer_plane.t_max = keyframes_plane[keyframes_plane.size() - 2].t;  // last time of the keyframe
    timer_plane.t = timer_plane.t_min;

    timer_plane.scale = 0.5f;
}

void scene_model::set_creature_rotation(float t_creature){
    /***************************************************/
    /*****************Begin Animation*******************/
    const int idx = index_at_value(t_creature, keyframes_creature);

    // Assume a closed curve trajectory

    const float t1 = keyframes_creature[idx].t;
    const float t0 = keyframes_creature[idx - 1].t;
    const float t2 = keyframes_creature[idx + 1].t;
    const float t3 = keyframes_creature[idx + 2].t;
    const vec3& p1 = keyframes_creature[idx].p;
    const vec3& p0 = keyframes_creature[idx - 1].p;
    const vec3& p2 = keyframes_creature[idx + 1].p;
    const vec3& p3 = keyframes_creature[idx + 2].p;

    const vec3 p = cardinal_spline_interpolation(t_creature, t0, t1, t2, t3, p0, p1, p2, p3, 1.0);
    const vec3 p_der = cardinal_spline_interpolation_der(t_creature, t0, t1, t2, t3, p0, p1, p2, p3, 1.0);
    // Store current trajectory of point p

    /** *************************************************************  **/
    /** Compute the (animated) transformations applied to the elements **/
    /** *************************************************************  **/

    // The body oscillate along the z direction
    creature["bigbody"].transform.translation = p;
    creature["bigbody"].transform.rotation = rotation_between_vector_mat3({ 0,-1,0 }, p_der)*rotation_from_axis_angle_mat3({ 1,0,0 }, 1);  //make it to z vertical orientation
    // Rotation of the shoulder around the y axis
    mat3 const R_shoulder = rotation_from_axis_angle_mat3({ 0,1,0 }, std::sin(2 * 3.14f * (t_creature - 0.4f)));
    // Rotation of the arm around the y axis (delayed with respect to the shoulder)
    mat3 const R_arm = rotation_from_axis_angle_mat3({ 0,1,0 }, std::sin(2 * 3.14f * (t_creature - 0.6f)));
    // Symmetry in the x-direction between the left/right parts
    mat3 const Symmetry = { -1,0,0, 0,1,0, 0,0,1 };

    mat3 const R_leg = rotation_from_axis_angle_mat3({ 1,1,0 }, std::sin(2 * 3.14f * (t_creature - 0.4f)));
    mat3 const R_leg2 = rotation_from_axis_angle_mat3({ 1,0,0 }, std::sin(2 * 3.14f * (t_creature - 0.7f)));
    mat3 const R_leg3 = rotation_from_axis_angle_mat3({ 1,0,0 }, std::sin(2 * 3.14f * (t_creature - 1.0f)));
    mat3 const Symmetry_leg = { -1,0,0, 0,1,0, 0,0,-1 };

    // Set the rotation to the elements in the hierarchy
    creature["shoulder_left"].transform.rotation = R_shoulder;
    creature["arm_bottom_left"].transform.rotation = R_arm;

    creature["shoulder_right"].transform.rotation = Symmetry * R_shoulder; // apply the symmetry
    creature["arm_bottom_right"].transform.rotation = R_arm; //note that the symmetry is already applied by the parent element

    creature["leg_left"].transform.rotation = R_leg;
    creature["leg2_left"].transform.rotation = R_leg2;
    creature["leg3_left"].transform.rotation = R_leg3;
    creature["leg_right"].transform.rotation = Symmetry_leg * R_leg;
    creature["leg2_right"].transform.rotation = R_leg2;
    creature["leg3_right"].transform.rotation = R_leg3;
    creature.update_local_to_global_coordinates();
}

void scene_model::set_plane_rotation(float t_creature) {
    /***************************************************/
    /*****************Begin Animation*******************/
    const int idx = index_at_value(t_creature, keyframes_plane);

    // Assume a closed curve trajectory

    const float t1 = keyframes_plane[idx].t;
    const float t0 = keyframes_plane[idx - 1].t;
    const float t2 = keyframes_plane[idx + 1].t;
    const float t3 = keyframes_plane[idx + 2].t;
    const vec3& p1 = keyframes_plane[idx].p;
    const vec3& p0 = keyframes_plane[idx - 1].p;
    const vec3& p2 = keyframes_plane[idx + 1].p;
    const vec3& p3 = keyframes_plane[idx + 2].p;

    const vec3 p = cardinal_spline_interpolation(t_creature, t0, t1, t2, t3, p0, p1, p2, p3, 1.0);
    const vec3 p_der = cardinal_spline_interpolation_der(t_creature, t0, t1, t2, t3, p0, p1, p2, p3, 1.0);
    // Store current trajectory of point p

    /** *************************************************************  **/
    /** Compute the (animated) transformations applied to the elements **/
    /** *************************************************************  **/

    // The body oscillate along the z direction
    plane["bigbody"].transform.translation = p;
    plane["bigbody"].transform.rotation = rotation_between_vector_mat3({ 0,0,1 }, p_der);
    plane.update_local_to_global_coordinates();
}

void scene_model::set_gui() {
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);

    ImGui::Separator();
    ImGui::Text("Perlin parameters");

    /*float height_min = 0.1f;
    float height_max = 2.0f;
    if (ImGui::SliderScalar("Height", ImGuiDataType_Float, &gui_scene.height, &height_min, &height_max))
    {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }
    float scaling_min = 0.1f;
    float scaling_max = 10.0f;
    if (ImGui::SliderScalar("(u,v) Scaling", ImGuiDataType_Float, &gui_scene.scaling, &scaling_min, &scaling_max))
    {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }*/

    int octave_min = 1;
    int octave_max = 10;
    if (ImGui::SliderScalar("Octave", ImGuiDataType_S32, &gui_scene.octave, &octave_min, &octave_max)) {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }

    float persistency_min = 0.1f;
    float persistency_max = 0.9f;
    if (ImGui::SliderScalar("Persistency", ImGuiDataType_Float, &gui_scene.persistency, &persistency_min, &persistency_max)) {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }
}


static size_t index_at_value(float t, vcl::buffer<vec3t> const& v) {
    const size_t N = v.size();
    assert(v.size() >= 2);
    assert(t >= v[0].t);
    assert(t < v[N - 1].t);
    size_t k = 0;
    while (v[k + 1].t < t)
        ++k;
    return k;
}

static vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float K) {
    const vec3 d1 = 2 * K / (t2 - t0) * (p2 - p0);
    const vec3 d2 = 2 * K / (t3 - t1) * (p3 - p1);
    const float s = (t - t1) / (t2 - t1);
    const float s2 = s * s;
    const float s3 = s2 * s;
    const vec3 p = (2 * s3 - 3 * s2 + 1) * p1 + (s3 - 2 * s2 + s) * d1 + (-2 * s3 + 3 * s2) * p2 + (s3 - s2) * d2;
    return p;
}

static vec3 cardinal_spline_interpolation_der(float t, float t0, float t1, float t2, float t3, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float K) {
    const vec3 d1 = 2 * K / (t2 - t0) * (p2 - p0);
    const vec3 d2 = 2 * K / (t3 - t1) * (p3 - p1);
    const float s = (t - t1) / (t2 - t1);
    const float s2 = s * s;
    const vec3 p_der = (1 / (t2 - t1))*( (6 * s2 - 6 * s) * p1 + (3 * s2 - 4 * s + 1) * d1 + (-6 * s2 + 6 * s) * p2 + (3 * s2 - 2 * s) * d2 );
    return p_der;
}
#endif



#include "modeling.hpp"


#ifdef SCENE_3D_GRAPHICS

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;




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

/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_model::setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    // Create visual terrain surface
    /*terrain = create_terrain(gui_scene);
    terrain.uniform.color = {0.6f,0.85f,0.5f};
    terrain.uniform.shading.specular = 0.0f;*/ // non-specular terrain material

    update_terrain();


    //Fill in grass position
    for (int i = 0; i < N_grass; ++i) {
        float u = rand_interval(0, 1);
        float v = rand_interval(0, 1);
        //vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_terrain_z(u,v) };
        vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_perlin_terrain_z(u,v,gui_scene) };

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
        //vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_terrain_z(u,v) };
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
        //vec3 pos = { 20 * (u - 0.5f), 20 * (v - 0.5f),  evaluate_terrain_z(u,v) };
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

    
    // Initial Keyframe data vector of (position, time)
    keyframes = { { {-1,1,0}   , 0.0f  },
                  { {0,1,0}    , 1.0f  },
                  { {1,1,0}    , 2.0f  },
                  { {1,2,0}    , 2.5f  },
                  { {2,2,0}    , 3.0f  },
                  { {2,2,1}    , 3.5f  },
                  { {2,0,1.5}  , 3.75f  },
                  { {1.5,-1,1} , 4.5f  },
                  { {1.5,-1,0} , 5.0f  },
                  { {1,-1,0}   , 6.0f  },
                  { {0,-0.5,0} , 7.0f },
                  { {-1,-0.5,0}, 8.0f },
    };

    // Set timer bounds
    // You should adapt these extremal values to the type of interpolation
    timer_scaling.t_min = 1;
    timer_scaling.t_max = 100;
    timer_scaling.t = timer_scaling.t_min;
    timer_height.t_min = 100;                   
    timer_height.t_max = 200;
    timer_height.t = timer_height.t_min;

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

    trajectory = curve_dynamic_drawable(100); // number of steps stored in the trajectory
    trajectory.uniform.color = { 0,0,1 };

    picked_object = -1;

    
    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

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

    if( gui_scene.wireframe ){ // wireframe if asked from the GUI
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
    //grass.uniform.transform.translation = { 0.25f,0,-0.5f };
    for (int i = 0; i < N_grass; ++i) {
        grass.uniform.transform.translation = grass_position[i];
        draw(grass, scene.camera, shaders["mesh"]);
    }
    if (gui_scene.wireframe)
        for (int i = 0; i < N_grass; ++i) {
            grass.uniform.transform.translation = grass_position[i];
            draw(grass, scene.camera, shaders["mesh"]);
        }

    update_terrain();
    update_tree();
    update_mushroom();
    update_grass();
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
    /*const vec2 u0 = {0.5f, 0.5f};
    const float h0 = 2.0f;
    const float sigma0 = 0.15f;

    const float d = norm(vec2(u,v)-u0)/sigma0;

    const float z = h0*std::exp(-d*d);*/
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
    terrain.position.resize(N*N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            //terrain.position[kv+N*ku] = evaluate_terrain(u,v);
            terrain.position[kv + N * ku] = evaluate_perlin_terrain(u,v,gui_scene);
            terrain.texture_uv[kv + N * ku] = {5*ku/(float)N,5*kv/(float)N};
            //terrain.color[kv + N * ku] = { c,c,c,1.0f };
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for(unsigned int ku=0; ku<Ns-1; ++ku)
    {
        for(unsigned int kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + N*ku; // current vertex offset

            const uint3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const uint3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
            
        }
    }

    return terrain;
}

mesh create_cylinder(float radius, float height)
{
    mesh m;

    // Number of samples
    const size_t N = 20;

    // Geometry
    for (size_t k = 0; k < N; ++k)
    {
        const float u = k / float(N);
        const vec3 p = { radius * std::cos(2 * 3.14f * u), radius * std::sin(2 * 3.14f * u), 0.0f };
        m.position.push_back(p);
        m.position.push_back(p + vec3(0, 0, height));
    }

    // Connectivity
    for (size_t k = 0; k < N; ++k)
    {
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
    
    for (size_t k = 0; k < N; ++k)
    {
        const float u = k / float(N);
        const vec3 p = { radius * std::cos(2 * 3.14f * u), radius * std::sin(2 * 3.14f * u), z_offset };
        m.position.push_back(p);
    }

    const vec3 base = { 0.0f, 0.0f, z_offset };
    m.position.push_back(base);
    const vec3 top = { 0.0f, 0.0f, height + z_offset };
    m.position.push_back(top);

    // Connectivity
    for (size_t k = 0; k < N; ++k)
    {
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
mesh create_tree_foliage(float radius, float height, float z_offset)
{
    return mesh();
}

mesh create_tree_foliage(float radius, float height, float z_offset, float cylinder_height)
{
    mesh m = create_cone(radius, height, cylinder_height);
    m.push_back(create_cone(radius, height, cylinder_height + z_offset));
    m.push_back(create_cone(radius, height, cylinder_height + 2 * z_offset));

    return m;
}

mesh create_tree(float cylinder_rad, float cylinder_height, float tree_rad, float tree_height, float tree_z_offset, const vec3& tree_col, const vec3& cylin_col)
{
    mesh m = create_cylinder(cylinder_rad,cylinder_height);
    m.fill_color_uniform(cylin_col);
    mesh n = create_tree_foliage(tree_rad, tree_height, tree_z_offset, cylinder_height);
    n.fill_color_uniform(tree_col);
    m.push_back(n);
    return m;
}

mesh create_mushroom(float cylinder_rad, float cylinder_height, float cone_rad, float cone_height, const vec3& cylin_col, const vec3& cone_col)
{
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
void scene_model::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);

    ImGui::Separator();
    ImGui::Text("Perlin parameters");

    float height_min = 0.1f;
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
    }

    int octave_min = 1;
    int octave_max = 10;
    if (ImGui::SliderScalar("Octave", ImGuiDataType_S32, &gui_scene.octave, &octave_min, &octave_max))
    {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }

    float persistency_min = 0.1f;
    float persistency_max = 0.9f;
    if (ImGui::SliderScalar("Persistency", ImGuiDataType_Float, &gui_scene.persistency, &persistency_min, &persistency_max))
    {
        update_terrain();
        update_tree();
        update_mushroom();
        update_grass();
    }
}



#endif


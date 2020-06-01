## Table of Contents
- [How to run](#how-to-run)
- [General description](#general-description)
- [Implementation details](#implementation-details)
- [Demonstration of the scene](#demonstration-of-the-scene)
- [Contributors](#contributors)
- [License](#license)

How to run
==========
```shell
cd src
make
./pgm
```
General description
=====================

In this project, we describe a scene on the sea. It is a place where
hide a lot of reef, and where a fight just happened. The fight
killed a lot of fish and destroyed boats, so we see a
shipwreck and planks floating on the sea. Again, there is a
fighter who whirls around this marine area, and launches missiles without
stops. In the sky there is also a bizarre and giant bird, which
chaisse the fighter.

On the code side, we try to use all the techniques introduced
in the course, in particular simulation, animation and texture etc. We
find a way to represent the wave and vary the
noise of Perlin over time.

In fact, this bird comes from science fiction *Hyperion*, which is a
human-killer and therefore the symbol of war. We hope this scene
can make people understand that war is cruel and ugly.

Implementation details
=========================

The wave
--------

To create the effect of the wave. We start from the initial terrain which
uses Perlin noise.

So far the terrain remains static. To simulate a
wave. We varied the parameters depending on the
time. In this
way we manage to create a wave effect.

We make a small remark that, so that the variations of wave are
fluids, we add boolean variables, which allows us at the end of the
process of repeating its variations in reverse, and start again
the process in the direct sense.

The functions concerned are:

```C++
float evaluate_perlin_terrain_z (float u, float v, const gui_scene_structure & gui_scene)

vec3 evaluate_perlin_terrain (float u, float v, const gui_scene_structure & gui_scene)

void scene_model :: set_gui ()
```
The Unknown Creature
--------------------

The unknown creature takes its concept from science fiction *Hyperion*,
which is a human-killer and therefore the symbol of war.

We use hierarchical animation to create and animate a creature
unknown, wearing two weird wings and two long legs and
thin. The hierarchical animation is mainly done on the two
parts of the body. To simulate walking, we add the time delay
in the rotation of the different parts of the legs.

It also follows a descriptive animation in the space that we are going to
discuss later.

3 functions are concerned:

```C++
hierarchy_mesh_drawable create_creature()

void set_creature_rotation(float t_creature)

void set_data_creature_animation(std::map<std::string, GLuint>& shaders)
```
The fighter
-------------

The fighter who flies in the animation is also built by the
hierarchical construction. However, this object does not make a
hierarchical animation. However, it is designed for animation
descriptive.

We fixed a few points in space and we made
interpolation by cardinal spline. Notice that if we use
this way of interpolation, the first point and the last point do not
have not passed through the object.

The cardinal spline interpolation details can be found in our report. 

We have chosen *K = 1* here. After calculating the *p(t)* at each
moment we can translate the object to
follow the interpolation.

However, this is obviously not enough. Because the simple translation
cannot simulate the object's orientation. For
improve, we must take into account the derivative of *p(t)*.

What remains is to rotate the vector oriented by the object at
vector *p'(t)*.

3 functions are concerned:

```C++
hierarchy_mesh_drawable create_plane()

const vcl::vec3 set_plane_rotation(float t_creature)

const vcl::vec3 set_plane_rotation(float t_creature)
```

Skybox
------

A skybox is a cube surrounding the stage on which a texture
environment is plated. The skybox gives the illusion that
the scene is immersed in a wider environment. The texture
used must correspond to the case of an unfolded cube, and the colors
level of the edges must be consistent on all the faces.

Once the cube is created, it is placed so as to encompass the
scene, and permanently centered around the camera position. The
size is specified using a $ b $ parameter passed in the function

```C++
mesh create_sky (float  b)
```
Missle
------

The missile is a combination of a cylinder and a cone, to which
we apply the simulation, so that it leaves the fighter and falls towards
the sea with gravity.

For initialization, we create the missile together with the fighter at
end of having the same speed and direction. Once the missle is created,
we use the following formulas to renew its speed and
position. Here *F* corresponds to gravity.

The physical model can be found in the report.

The missile has the following functions:

```C++
void set_missle_animation(const vcl::vec3& p_der)

const vcl::vec3 set_plane_rotation(float t_creature)
```
Other objects
-------------

The other objects that appear in our model are mainly
static, including dead fish, shipwreck, planks and
rocks.

- The rocks are created in the same way as the wave, but static,
    that is to say, without varying the parameters in the noise of
    Perlin.

- Dead fish is created thanks to the billboard which consists of
    use images of objects on a transparent background as
    texture to represent objects of complex appearance on a
    simple geometry. We also modify fish so that they
    rotate with the camera. In addition, to have a floater effect on
    the sea, the fish have a *timer* to change their position
    together with the wave in time.

- The boards are rectangular parallelepipeds which float
    in the sea, the effect of which is carried out like dead fish.

- The sinking is simply created by adding points and
    connections in its corresponding mesh.

In order to add likelihood, the objects on it all have their
clean texture.

The corresponding functions are:

```C++
mesh create_island(const gui_scene_structure& gui_scene)

mesh create_fish(float length, float width)

mesh create_box(float hight, float width, float length)

mesh create_boat(float length, float width, float height)
```

Demonstration of the scene
=======================

We will then give two screenshots of our animation screen:
one is in the shader  \"mesh\" configuration and the other in
\"wireframe\", in which you can find all the objects
presented above.

![Demo](https://github.com/DyeKuu/INF443/blob/master/report/screenshot.png)

![](https://github.com/DyeKuu/INF443/blob/master/report/screenshot1.png)

Contributors
============
This project exists thanks to all the people who contribute.

[qym7](https://github.com/qym7)
[DyeKuu](https://github.com/DyeKuu)

License
==========
[MIT License](https://github.com/DyeKuu/OpenGL-Seawave-Animation-INF443/master/LICENSE)

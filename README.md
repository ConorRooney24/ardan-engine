# Ardán Engine (AE)
A 3D voxel-based CPU rendering engine.

Ardan is currently minimal, but I plan to greatly improve the engine's features, performance, and usability over time. I may also move away from voxels and eventually rewrite the engine as a rasterizer. Currently, Ardan is fully CPU based and does not use the GPU, making it pretty slow.

## Building
As of May 2026, the engine officially supports Linux only. Windows support is currently untested.

To build the project after cloning the repository, run:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Once built, run the engine with:

```bash
./ardan
```

## Key components
There are 4 most important parts of AE are the Map, Caster, Camera, and Renderer.
### Map
The Map is represented as a 3D grid of Voxels. Each Voxel has a material and texture that dictates how that voxel looks, and interacts with light.
### Caster
The caster contains the logic needed for ray traversal around the map. It will take a ray and its trajectory, and then move that ray through the map keeping track of the voxels it collides with.
### Camera
The Camera acts as a middle man between the different key components of AE. The Camera will summon a ray for every pixel on the screen. For each of its rays, it will calculate what direction that ray should be moving in. It will then tell give the caster a Map to use, and tell the caster to send each of its rays through that map. Once the caster has finished its job, The camera will take the journey data for each of the rays, and send it to the renderer.
### Renderer (Termicast)
Once all the other components have worked together to send the rays through the map, The camera will have sent the journey data for each ray to a buffer in the renderer. The renderer will then take that data, and turn it into colour information for each pixel on the screen. Now that the renderer knows what colour each pixel should be, it will print the pixels to the terminal, displaying the final image.

## Position, Rotation, and Velocity
Position, Rotation, and Velocity are used to describe where objects are, where objects are looking, and where objects are going relative to the map.

Static objects (such as voxels on the map) will usually just have a position. Mobile objects (such as the Camera) will have a Position, Rotation, And a velocity.
### Position (pos)
The position of an object describes it's current location relative to world space. Position is stored as a Vec3 of x, y, and z.

In this program, When using 3D space, The coordinate system is as follows: +x means moving right, -x means moving left, +y means moving up (into the air), -y means moving down (towards the floor), +z means moving forward, and -z means moving back. When dealing with 2D space (such as pixel buffers), +x means moving right, and +y means moving down. In the 2D system x0, y0 would be the top left.

Coordinate systems can either be relative to the map, or relative to another object. going 1 unit forward in the Z axis relative to the map would mean the same thing for every object regardless of their rotation. They would all move in the same direction along the map. However, Moving and object 1 unit forward in the Z axis relative to that objects own self would just that the object moves forward in whatever direction it is facing. If that object was facing towards the +y direction relative to the map, and it moved +1 unit in the z axis relative to itself, it would move 1 unit in the y axis relative to the world.
### Rotation (rot)
The rotation of an object currently describes an objects pitch and yaw. Rotation is stored in a Vec2 of x, and y. x refers to the objects pitch going from +z towards +y. Meaning a pitch of 0 degrees would be looking dead on, a pitch of 90 degrees would be straight up, and a pitch of 180 degrees would be straight behind. Similarly, y refers to an objects yaw starting from +z going towards +x. A yaw of 0 degrees would mean straight on, 90 would mean to the right, 180, would mean behind, and 270 would mean to the left.

Rotation vectors are stored in radian instead of degrees for compatibility with trigonometry functions.
### Velocity (vel)
The velocity of an object describes how far it is moving each frame relative to world space, not relative to itself. Velocity is stored as a Vec3.
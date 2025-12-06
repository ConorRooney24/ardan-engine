# Termicast
A Fully 3D Terminal based Ray-Caster engine written in C.

## Conventions
### Coordinates
In this program, When using 3D space, The coordinate system is as follows: +x means moving right, -x means moving left, +y means moving up (into the air), -y means moving down (towards the floor), +z means moving forward, and -z means moving back.

Coordinate systems can either be relative to the map, or relative to another object. going 1 unit forward in the Z axis relative to the map would mean the same thing for every object regardless of their rotation. They would all move in the same direction along the map. However, Moving and object 1 unit forward in the Z axis relative to that objects own self would just that the object moves forward in whatever direction it is facing. If that object was facing towards the +y direction relative to the map, and it moved +1 unit in the z axis relative to itself, it would move 1 unit in the y axis relative to the world.

### Position (pos)
The position of an object describes it's current location relative to world space. Position is stored as a Vec3 of x, y, and z.
### Rotation (rot)
The rotation of an object currently describes an objects pitch and yaw. Rotation is stored in a Vec2 of x, and y. x refers to the objects pitch going from +z towards +y. Meaning a pitch of 0 degrees would be looking dead on, a pitch of 90 degrees would be straight up, and a pitch of 180 degrees would be straight behind. Similarly, y refers to an objects yaw starting from +z going towards +x. A yaw of 0 degrees would mean straight on, 90 would mean to the right, 180, would mean behind, and 270 would mean to the left.

Rotation vectors are stored in radian, not degrees.
### Velocity (vel)
The velocity of an object describes how far it is moving each frame relative to world space, not relative to itself. Velocity is stored as a Vec3.
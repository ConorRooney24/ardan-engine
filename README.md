# Termicast
A Fully 3D Terminal based Ray-Caster written in C.

## Conventions
### Position & Direction
In 3D Space, +x is right, +y is up, and +z is forward. If an objects z increases, it means it's moving forward relative to global space. If its y increases, it's floating up. If its x increases, it's moving right.

The position of objects in 3D space is stored in a Vec3 where x, y, and z are a given objects global coordinates.

The direction of an object refers to where it is facing. Direction is stored in 3 different ways throughout the program:
- A Vec3 which represents a translation between the objects current position and a translation which would be added to that position if it were to move in the direction it is facing. This is the primary method of storing direction.
- A Vec2 where x and y represent the pitch and yaw in radian respectively. This is used for trig functions and compatibility with math.h.
- A Vec2 where x and y represent the pitch and yaw in degrees respectively. This is used for human readability and is usually translated into Radian or a translation Vec3 later for actual computation.
### Pitch, Roll, and Yaw
When we are dealing with Pitch, Roll, and Yaw for say an objects direction (In which case it would just be pitch and yaw given objects do not yet support roll) instead of a Vec3 translation vector, We measure as follows:
- Pitch is measured as the angle (in radian or degrees) from +z towards +y. A pitch of 0 would mean straight forward towards z. A pitch of 90 degrees would mean facing straight up towards y.
- Yaw is the angle from +y towards +x, so a yaw of 0 would be even, and a yaw of 90 degrees would mean tilted 90 degrees clockwise.
- Roll is measured as the angle from +z towards +x.

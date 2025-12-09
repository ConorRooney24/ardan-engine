# Map
The AE Map is a structure that represents Voxels in space. It is a 3D grid of Voxels.

## Map Files
There are two types of map files in AE. Human readable YAML map files, and easily parsable .mapa files.

The reason for the separation is to allow for a human to easily edit a map in a text editor without the need for a complicated parser that can parse the human readable file. Instead, the human readable YAML is converted to a .mapa using a python tool since python makes it very easy to parse YAML's with libraries.

### YAML Map Files
The YAML map files are very easy to read and edit. Maps can be constructed by hand using the YAML.

The structure of the YAML looks like this:

``` YAML
name: "Test Map"
size:
  x: 5
  y: 4
  z: 5
spawn:
  position:
    x: 2.5
    y: 2
    z: 2.5
  rotation:
    x: 0
    y: 0
voxels:
  # y3
  # x0 x1 x2 x3 x4
  - [4, 4, 4, 4, 4] # z4
  - [4, 4, 4, 4, 4] # z3
  - [4, 4, 4, 4, 4] # z2
  - [4, 4, 4, 4, 4] # z1
  - [4, 4, 4, 4, 4] # z0

  # y2:
  # x0 x1 x2 x3 x4
  - [2, 2, 2, 2, 2] # z4
  - [2, 0, 0, 0, 2] # z3
  - [2, 0, 0, 0, 2] # z2
  - [2, 0, 0, 0, 2] # z1
  - [2, 2, 2, 2, 2] # z0

  # y1
  # x0 x1 x2 x3 x4
  - [2, 2, 2, 2, 2] # z4
  - [2, 0, 0, 0, 2] # z3
  - [2, 0, 0, 0, 2] # z2
  - [2, 0, 0, 0, 2] # z1
  - [2, 2, 2, 2, 2] # z0

  # y0
  # x0 x1 x2 x3 x4
  - [3, 4, 4, 3, 3] # z4
  - [3, 4, 3, 4, 3] # z3
  - [3, 4, 4, 3, 3] # z2
  - [3, 4, 3, 4, 3] # z1
  - [3, 4, 3, 4, 3] # z0
```

Name is the title of the map, the spawn position xyz will set the default/recommended spawn point for that map, the spawn rotation xy will set the default/recommended spawn pitch and yaw for that map, and then the voxels section describes the physical layout of the map. The numbers within the rows correlate to a Voxel ID.

### .mapa Map Files
The .mapa map files are designed not for human readability, but to be very easy to parse in C.

Here is the rough structure of a .mapa file:

```
map_name
size_x
size_y
size_z
spawn_pos_x
spawn_pos_y
spawn_pos_z
spawn_rot_x
spawn_rot_y
--- (Header over / start of voxel data indicator)
x,y,z,vox_id 
x,y,z,vox_id 
x,y,z,vox_id 
x,y,z,vox_id 
x,y,z,vox_id 
(and so on...)
```

The above structure just illustrates what each line in a .mapa file corresponds to, not what an actual working .mapa would contain. 

The values in the file are split up into header values, and non header values. Header values are anything above the '---' which indicates the end of a header. below the header is the voxel data where every single possible x, y, and z on the map is given a specific vox_id. 

Converting the example YAML map file in the YAML section to a .mapa using the yaml_to_mapa.py tool would give the following file content:

```
Test Map
5
4
5
2.5
2.0
2.5
0.0
0.0
---
0,0,0,3
1,0,0,4
2,0,0,3
3,0,0,4
4,0,0,3
0,0,1,3
1,0,1,4
2,0,1,3
3,0,1,4
4,0,1,3
0,0,2,3
1,0,2,4
2,0,2,4
3,0,2,3
4,0,2,3
0,0,3,3
1,0,3,4
2,0,3,3
3,0,3,4
4,0,3,3
0,0,4,3
1,0,4,4
2,0,4,4
3,0,4,3
4,0,4,3
0,1,0,2
1,1,0,2
2,1,0,2
3,1,0,2
4,1,0,2
0,1,1,2
1,1,1,0
2,1,1,0
3,1,1,0
4,1,1,2
0,1,2,2
1,1,2,0
2,1,2,0
3,1,2,0
4,1,2,2
0,1,3,2
1,1,3,0
2,1,3,0
3,1,3,0
4,1,3,2
0,1,4,2
1,1,4,2
2,1,4,2
3,1,4,2
4,1,4,2
0,2,0,2
1,2,0,2
2,2,0,2
3,2,0,2
4,2,0,2
0,2,1,2
1,2,1,0
2,2,1,0
3,2,1,0
4,2,1,2
0,2,2,2
1,2,2,0
2,2,2,0
3,2,2,0
4,2,2,2
0,2,3,2
1,2,3,0
2,2,3,0
3,2,3,0
4,2,3,2
0,2,4,2
1,2,4,2
2,2,4,2
3,2,4,2
4,2,4,2
0,3,0,4
1,3,0,4
2,3,0,4
3,3,0,4
4,3,0,4
0,3,1,4
1,3,1,4
2,3,1,4
3,3,1,4
4,3,1,4
0,3,2,4
1,3,2,4
2,3,2,4
3,3,2,4
4,3,2,4
0,3,3,4
1,3,3,4
2,3,3,4
3,3,3,4
4,3,3,4
0,3,4,4
1,3,4,4
2,3,4,4
3,3,4,4
4,3,4,4
```
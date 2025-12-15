// map.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "map.h"
#include "material.h"
#include "../../util/str.h"

Map map_init(Vec3i size)
{
        Map map;
        map.name = "Undefined";
        map.size = size;
        map.spawn_pos = (Vec3){0, 0, 0};
        map.spawn_rot = (Vec2){0, 0};

        map.vox = (Vox***)malloc(size.x * sizeof(Vox**));

        for (int x = 0; x < size.x; x++)
        {
                map.vox[x] = (Vox**)malloc(size.y * sizeof(Vox*));

                for (int y = 0; y < size.y; y++)
                {
                        map.vox[x][y] = (Vox *)malloc(size.z * sizeof(Vox));

                        for (int z = 0; z < size.z; z++)
                        {                                
                                map.vox[x][y][z] = voxs[VOX_UNDEFINED];
                        }
                }
        }
        return map;
}

void map_destroy(Map *map)
{
        for (int x = 0; x < map->size.x; x++)
        {
                for (int y = 0; y < map->size.y; y++)
                {
                        free(map->vox[x][y]);
                }

                free(map->vox[x]);
        }

        free(map->vox);

        map->vox = NULL;
        map->size = (Vec3i){0,0,0};
}

Map map_load_file(const char *file)
{
        Map map;

        const int MAX_LINE_LENGTH = 4096;
        const int NUM_HEADER_VALUES = 8; // excluding name

        FILE *map_file = fopen(file, "r");
        if (map_file == NULL)
        {
                col_print_set(COL_RED);
                printf("Error: Unable to read map file '%s'.", file);
                col_print_reset();
                printf("\n");
                return map;
        }

        char *line = malloc((size_t)MAX_LINE_LENGTH);


        char *name_buf = NULL;
        int  size_x = 0, size_y = 0, size_z = 0;
        double spawn_pos_x = 0.0, spawn_pos_y = 0.0, spawn_pos_z = 0.0;
        double spawn_rot_x = 0.0, spawn_rot_y = 0.0;

        int header_values_read = 0;
        bool name_read = false;
        bool header_over = false;


        while (fgets(line, MAX_LINE_LENGTH, map_file)) // Read lines until end of header indicator (---)
        {
                str_whitespace_remove(line);

                if (line[0] == '\0' || line[0] == '\n')
                {
                        continue;
                }

                if (line[0] == '-' && line[1] == '-' && line[2] == '-') // If we find the header
                {
                        header_over = true;
                        break;
                }

                if (!name_read)
                {
                        name_buf = strdup(line);
                        name_read = true;
                        continue;
                }

                // 1 size_x
                // 2 size_y
                // 3 size_z
                // 4 spawn_pos_x
                // 5 spawn_pos_y
                // 6 spawn_pos_z
                // 7 spawn_rot_x
                // 8 spawn_rot_y

                char *endptr = NULL;
                if (header_values_read == 0)
                {
                        size_x = (int)strtol(line, &endptr, 10);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid size_x given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 1)
                {
                        size_y = (int)strtol(line, &endptr, 10);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid size_y given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 2)
                {
                        size_z = (int)strtol(line, &endptr, 10);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid size_z given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 3)
                {
                        spawn_pos_x = strtod(line, &endptr);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid spawn_pos_x given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 4)
                {
                        spawn_pos_y = strtod(line, &endptr);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid spawn_pos_y given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 5)
                {
                        spawn_pos_z = strtod(line, &endptr);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid spawn_pos_z given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 6)
                {
                        spawn_rot_x = strtod(line, &endptr);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid spawn_rot_x given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else if (header_values_read == 7)
                {
                        spawn_rot_y = strtod(line, &endptr);
                        if (endptr == line)
                        {
                                col_print_set(COL_RED);
                                printf("Error: Invalid spawn_rot_y given in map header line '%s' in map file '%s'", line, file);
                                col_print_reset();
                                printf("\n");

                                free(name_buf);
                                free(line);
                                fclose(map_file);
                                return map;
                        }
                }
                else
                {
                        col_print_set(COL_RED);
                        printf("Error: Invalid value given in map header line '%s' in map file '%s' (Likely too many values)", line, file);
                        col_print_reset();
                        printf("\n");
                }

                header_values_read++;
        }

        if (!header_over)
        {
                col_print_set(COL_RED);
                printf("Error: Missing header over indicator (---) in map file '%s'.", file);
                col_print_reset();
                printf("\n");

                free(name_buf);
                free(line);
                fclose(map_file);
                return map;
        }

        if (!name_read || header_values_read != NUM_HEADER_VALUES)
        {
                col_print_set(COL_RED);
                printf("Error: Invalid number of header values given in map file '%s'. Expected a name and %d numeric header lines.", file, NUM_HEADER_VALUES);
                col_print_reset();
                printf("\n");

                free(name_buf);
                free(line);
                fclose(map_file);
                return map;
        }

        if (size_x <= 0 || size_y <= 0 || size_z <= 0)
        {
                col_print_set(COL_RED);
                printf("Error: Invalid map size given in map file '%s'.", file);
                col_print_reset();
                printf("\n");

                free(name_buf);
                free(line);
                fclose(map_file);
                return map;
        }


        size_t total_voxels = (size_t)size_x * (size_t)size_y * (size_t)size_z;
        uint8_t *voxel_buffer = malloc(total_voxels);


        memset(voxel_buffer, 0, total_voxels);

        // parse voxel lines x,y,z,id
        while (fgets(line, MAX_LINE_LENGTH, map_file))
        {
                str_whitespace_remove(line);

                if (line[0] == '\0' || line[0] == '\n') // skip empty lines
                {
                        continue;
                }

                // parse four ints separated by commas allowing spaces
                int vx = 0, vy = 0, vz = 0, vid = 0;

                int num_matches = sscanf(line, " %d , %d , %d , %d", &vx, &vy, &vz, &vid);

                if (num_matches != 4)
                {
                        col_print_set(COL_RED);
                        printf("Error: Invalid voxel data given on line '%s' in map file '%s'.", line, file);
                        col_print_reset();
                        printf("\n");
                        return map;
                }

                if (vx < 0 || vx >= size_x || vy < 0 || vy >= size_y || vz < 0 || vz >= size_z) // validating
                {
                        col_print_set(COL_RED);
                        printf("Error: Voxel coordinate (x%d, y%d, z%d) does not match map size on line '%s' in map file '%s'.", vx, vy, vz, line, file);
                        col_print_reset();
                        printf("\n");
                        return map;
                }

                size_t index = ((size_t)vy * (size_t)size_z + (size_t)vz) * (size_t)size_x + (size_t)vx;
                voxel_buffer[index] = (uint8_t)vid;
        }


        fclose(map_file);
        free(line);

        
        map.name = name_buf;
        map.size.x = size_x;
        map.size.y = size_y;
        map.size.z = size_z;
        map.spawn_pos.x = (float)spawn_pos_x;
        map.spawn_pos.y = (float)spawn_pos_y;
        map.spawn_pos.z = (float)spawn_pos_z;
        map.spawn_rot.x = (float)spawn_rot_x;
        map.spawn_rot.y = (float)spawn_rot_y;

        // Initialise the return maps vox's
        map.vox = (Vox***)malloc(map.size.x * sizeof(Vox**));
        for (int x = 0; x < map.size.x; x++)
        {
                map.vox[x] = (Vox**)malloc(map.size.y * sizeof(Vox*));

                for (int y = 0; y < map.size.y; y++)
                {
                        map.vox[x][y] = (Vox *)malloc(map.size.z * sizeof(Vox));

                        for (int z = 0; z < map.size.z; z++)
                        {                                
                                map.vox[x][y][z] = voxs[VOX_UNDEFINED];
                        }
                }
        }
        
        // populate the return maps vox's
        for (int x = 0; x < map.size.x; x++)
        {
                for (int y = 0; y < map.size.y; y++)
                {
                        for (int z = 0; z < map.size.z; z++)
                        {
                                size_t index = (size_t)y * map.size.z * map.size.x + (size_t)z * map.size.x + (size_t)x;
                                map.vox[x][y][z] = voxs[voxel_buffer[index]];
                        }
                        
                }
                
        }

        return map;

        free(name_buf);
        free(voxel_buffer);
}
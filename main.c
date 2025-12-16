#include <stdio.h>
#include <math.h>

#include "engine/ae_engine.h"

#define RES_X 800/2
#define RES_Y 600/2
#define FRAMETIME_MIN 0.0f

int main(void)
{
        config.max_ray_distance = 300;
        config.ray_step_scalar_coarse = 1.0f;
        config.ray_step_scalar_fine = 0.01f;
        config.raycast_multithreading = true;

        materials_init(materials);
        text_init();
        map_voxs_init();

        Buffer pbuff = buffer_init(RES_X, RES_Y);
        RayBuffer rbuff = ray_buffer_init(RES_X, RES_Y);
        Map map = map_load_file("../res/map/MAP_BRICK_HOUSE.mapa");


        Vec3 pos;
        Vec2 dir;
        pos.x = map.spawn_pos.x;
        pos.y = map.spawn_pos.y;
        pos.z = map.spawn_pos.z;
        dir.x = map.spawn_rot.x;
        dir.y = map.spawn_rot.y;

        Cam c = cam_init(pos, dir, RES_X, RES_Y, deg_to_rad(80));
        cam_calculate_vFOV(&c);

        printf("\033[2J"); //cls

        TimeStamp frame_start;
        TimeStamp frame_end;

        //input_init();
        bool running = true;
        bool gravity_enabled = true;

        AEIO_SDL_Context aeio_sdl = aeio_sdl_init("Test", RES_X, RES_Y, 2);

        while (running)
        {

                frame_start = record_time();
                //input_update();

                SDL_Event sdl_event;
                while (SDL_PollEvent(&sdl_event))
                {
                        if (sdl_event.type == SDL_QUIT)
                        {
                                running = false;
                        }
                        else if (sdl_event.type == SDL_KEYDOWN && !sdl_event.key.repeat)
                        {
                                if (sdl_event.key.keysym.scancode == SDL_SCANCODE_G)
                                {
                                        gravity_enabled = !gravity_enabled;
                                }
                        }
                }

                const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
                Vec3 vec_buf;

#if 0
                if(input_key_pressed('0'))
                {
                        running = false;
                }

                Vec3 vec_buf;

                if(input_key_pressed('w'))
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y - deg_to_rad(90));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(90));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(input_key_pressed('a'))
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y  + deg_to_rad(180));
                        vec_buf.z = -sin(c.rot.y + deg_to_rad(180));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        //c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(input_key_pressed('s'))
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y - deg_to_rad(90));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(90));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x -= vec_buf.x;
                        c.vel.y -= vec_buf.y;
                        c.vel.z -= vec_buf.z;
                }
                if(input_key_pressed('d'))
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = -cos(c.rot.y  + deg_to_rad(180));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(180));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        //c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(input_key_pressed('q'))
                {
                        c.vel.y -= 0.1;
                }
                if(input_key_pressed('e'))
                {
                        c.vel.y += 0.1;
                }
#endif

                if(keyboard_state[SDL_SCANCODE_0])
                {
                        running = false;
                }
                if(keyboard_state[SDL_SCANCODE_W])
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y - deg_to_rad(90));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(90));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(keyboard_state[SDL_SCANCODE_A])
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y  + deg_to_rad(180));
                        vec_buf.z = -sin(c.rot.y + deg_to_rad(180));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        //c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(keyboard_state[SDL_SCANCODE_S])
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = cos(c.rot.y - deg_to_rad(90));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(90));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x -= vec_buf.x;
                        c.vel.y -= vec_buf.y;
                        c.vel.z -= vec_buf.z;
                }
                if(keyboard_state[SDL_SCANCODE_D])
                {
                        vec_buf.y = sin(c.rot.x);
                        vec_buf.x = -cos(c.rot.y  + deg_to_rad(180));
                        vec_buf.z = sin(c.rot.y + deg_to_rad(180));

                        vec_buf = vec3_normalize(vec_buf);
                        vec_buf = vec3_scale(vec_buf, 0.2f);

                        c.vel.x += vec_buf.x;
                        //c.vel.y += vec_buf.y;
                        c.vel.z += vec_buf.z;
                }
                if(keyboard_state[SDL_SCANCODE_Q])
                {
                        c.vel.y -= 0.1;
                }
                if(keyboard_state[SDL_SCANCODE_E])
                {
                        c.vel.y += 0.1;
                }
                
                Vec2 rot_vel;
                rot_vel.x = 0;
                rot_vel.y = 0;

#if 0
                if(input_key_pressed('i'))
                {
                        rot_vel.x += 0.05;
                }
                if(input_key_pressed('k'))
                {
                        rot_vel.x -= 0.05;
                }
                if(input_key_pressed('l'))
                {
                        rot_vel.y += 0.05;
                }
                if(input_key_pressed('j'))
                {
                        rot_vel.y -= 0.05;
                }
                if (input_key_pressed('-'))
                {
                        c.hFOV += 0.01;
                        cam_calculate_vFOV(&c);
                }
                if (input_key_pressed('='))
                {
                        c.hFOV -= 0.01;
                        cam_calculate_vFOV(&c);
                }
                if (input_key_pressed('g'))
                {
                        if (gravity_enabled)
                        {
                                gravity_enabled = false;
                        }
                        else
                        {
                                gravity_enabled = true;
                        }
                
                }
#endif

                if(keyboard_state[SDL_SCANCODE_I])
                {
                        rot_vel.x += 0.05;
                }
                if(keyboard_state[SDL_SCANCODE_K])
                {
                        rot_vel.x -= 0.05;
                }
                if(keyboard_state[SDL_SCANCODE_L])
                {
                        rot_vel.y += 0.05;
                }
                if(keyboard_state[SDL_SCANCODE_J])
                {
                        rot_vel.y -= 0.05;
                }
                if (keyboard_state[SDL_SCANCODE_MINUS])
                {
                        c.hFOV += 0.01;
                        cam_calculate_vFOV(&c);
                }
                if (keyboard_state[SDL_SCANCODE_EQUALS])
                {
                        c.hFOV -= 0.01;
                        cam_calculate_vFOV(&c);
                }
                


                cam_rays_reset(&c);
                cam_calculate_ray_trajectories(&c);
                capture_and_pass(&c, &rbuff, &map);
                
                ray_buffer_translate(&rbuff, &pbuff);
               
                //terminal_out_print(&pbuff, ' ');

                aeio_sdl_draw(&aeio_sdl, &pbuff);
                

                frame_end = record_time();
                printf("| FPS: %.1f | Frametime: %.1fms | Min Frametime: %.1f | Cam Pos: x%f, y%f, z%f | Cam Rot: x%f, y%f |\n", time_frame_time_to_fps(time_diff(frame_start, frame_end)), timestamp_to_ms(time_diff(frame_start, frame_end)), FRAMETIME_MIN, c.pos.x, c.pos.y, c.pos.z, c.rot.x, c.rot.y);
                
                if (timestamp_to_ms(time_diff(frame_start, frame_end)) < FRAMETIME_MIN)
                {
                        sleep_ms(FRAMETIME_MIN - timestamp_to_ms(time_diff(frame_start, frame_end))); 
                }

                const float g_friction = 0.6; // The friction on the floor
                const float a_friction = 0.9; // The friction in the air
                const float gravity = 0.1; // The force of gravity applied

                const float rot_vel_friction = 1;
                

                if (c.pos.y <= 2.5) // If we are touching the floor
                {
                        c.pos.y = 2.5;

                        if (c.vel.y < 0)
                        {
                                c.vel.y = 0;
                        }
                        

                        c.vel.x *= g_friction;
                        c.vel.z *= g_friction;
                }
                else // if we are in the air
                {
                        if (gravity_enabled)
                        {
                                c.vel.y -= gravity;
                        }

                        c.vel.x *= a_friction;
                        c.vel.z *= a_friction;
                        c.vel.y *= a_friction;
                }

                c.rot.x += rot_vel.x;
                c.rot.y += rot_vel.y;

                rot_vel.x *= rot_vel_friction;
                rot_vel.y *= rot_vel_friction;
                
                c.pos = vec3_add(c.pos, c.vel);
                
        }

        //input_restore();
        
        printf("\033[2J");

        cam_destroy(&c);
        buffer_destroy(&pbuff);
        ray_buffer_destroy(&rbuff);

        aeio_sdl_destroy(&aeio_sdl);

        return 0;
}

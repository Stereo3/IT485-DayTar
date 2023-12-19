#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gfc_audio.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_particle.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "world.h"
#include "weapon.h"
#include "resource.h"
#include "enemy.h"

//#include "player.c"

extern int __DEBUG;

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    TextBlock playerStatuses;
    TextBlock gatheredResources;
    
    //Sprite *mouse = NULL;
    int mousex,mousey;
    //Uint32 then;
    float mouseFrame = 0;
    World *w;
    Entity *agu;
    Entity *player;
    //Entity *weapon;
    //Entity *zombie;
    Mix_Music *ambiance;
    //Entity *gun;
    //Particle particle[100];
    Matrix4 skyMat;
    Model *sky;
    Uint8 mainMenuBool = 1;
    Sprite *mainMenuImg;
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL);

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }
    
    gfc_audio_init(128,32,0,32,1,0);
    init_logger("gf3d.log",0);    
    gfc_input_init("config/input.cfg");
    slog("gf3d begin");
    gf3d_vgraphics_init("config/setup.cfg");
    gf2d_font_init("config/font.cfg");
    gf2d_draw_manager_init(1000);
    
    slog_sync();
    
    entity_system_init(1024);
    
    //mouse = gf2d_sprite_load("images/pointer.png",32,32, 16);
    mainMenuImg = gf2d_sprite_load_image("images/mainmenu.png");

    w = world_load("config/testworld.json");

    agu = gfc_list_get_nth(w->entityList,2);
    if (agu)agu->selected = 1;
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));

    player = gfc_list_get_nth(w->entityList, 0);
    //weapon = gfc_list_get_nth(w->entityList, 1);
    // woodLog = gfc_list_get_nth(w->entityList, 3);
    // cementBlock = gfc_list_get_nth(w->entityList, 4);
    // metalBarrel = gfc_list_get_nth(w->entityList, 5);
    // metalBarrel->scale = vector3d(1,1,1);
    // jerryCan = gfc_list_get_nth(w->entityList, 6);
    // jerryCan->scale = vector3d(5,5,5);
    // waterWell = gfc_list_get_nth(w->entityList, 7);
    // waterWell->scale = vector3d(1,1,1);
    // /zombie = gfc_list_get_nth(w->entityList, 8);

    //slog("RAHHHHHHHHHHHHHHHHHHHHHHHHH: %s", woodLog->entityName);


    //test_string = sj_string_new_text("test: ", 0);


    /*
    for (a = 0; a < 100; a++)
    {
        particle[a].position = vector3d(gfc_crandom() * 100,gfc_crandom() * 100,gfc_crandom() * 100);
        particle[a].color = gfc_color(0,0,0,1);
        particle[a].color = gfc_color(gfc_random(),gfc_random(),gfc_random(),1);
        particle[a].size = 100 * gfc_random();
    }
    a = 0;
    */
    sky = gf3d_model_load("models/sky.model");
    gfc_matrix_identity(skyMat);
    gfc_matrix_scale(skyMat,vector3d(100,100,100));
    
    ambiance = gfc_sound_load_music("music/wind.mp3");

    //ambiance = Mix_LoadMUS("music/wind.mp3");

    if(!ambiance)
    {
        slog("AMBIANCE IS NULL RAHHHHHHHHH \n");
        return NULL;
    }

    Mix_PlayMusic(ambiance, -1);
    Mix_VolumeMusic(32);




    // main game loop
    slog("gf3d main loop begin");
    while(!done)
    {
        gfc_input_update();
        gf2d_font_update();
        SDL_GetMouseState(&mousex,&mousey);
        
        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;
        world_run_updates(w);
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        //rlStatuses workPlease = *((rlStatuses*)player->customData);

        //slog("calefaction check: %f", workPlease.calefaction);

        gfc_block_sprintf(playerStatuses
        ,"Calefaction: %f | Hydration: %i | Satiation: %i | Sanityation: %i | Defication: %i"
        ,player->calefaction
        ,player->hydration
        ,player->satiation
        ,player->sanityation
        ,player->defication);

        gfc_block_sprintf(gatheredResources
        ,"Wood: %i | Concrete: %i | Metal: %i | Fuel: %i | Water: %i"
        ,player->wood
        ,player->concrete
        ,player->metal
        ,player->fuel
        ,player->water);

        //slog("Volume: %i", Mix_VolumeMusic(-1));

        gf3d_vgraphics_render_start();

        if(mainMenuBool == 1)
        {
            if(!mainMenuImg)
            {
                slog("No Image for you!");
                return NULL;
            }
            gf3d_model_draw_sky(sky,skyMat,gfc_color(1,1,1,1));
            gf2d_sprite_draw(mainMenuImg,vector2d(500,100),vector2d(2,2),vector3d(0,0,0),gfc_color(1,1,1,1),0);
            if (keys[SDL_SCANCODE_3])
            {
                done = 1;
            }
            else if(keys[SDL_SCANCODE_1])
            {
                mainMenuBool = 0;
            }
        }
        else
        {
            entity_think_all();
            entity_update_all();
            world_draw(w);
            entity_draw_all();
            /*
            for (a = 0; a < 100; a++)
            {
                gf3d_particle_draw(&particle[a]);
            }
            */
            //2D draws
            //gf2d_draw_rect_filled(gfc_rect(10 ,10,1000,32),gfc_color8(128,128,128,255));
            gf2d_font_draw_line_tag(playerStatuses,FT_H1,gfc_color(1,0,1,1), vector2d(10,10));
            gf2d_font_draw_line_tag(gatheredResources,FT_Normal,gfc_color(0,1,0,1), vector2d(10,30));

            //gf2d_font_draw_line_tag(hydrationValue,FT_H1,gfc_color(1,1,1,1), vector2d(10,30));

            //gf2d_draw_rect(gfc_rect(10 ,10,1000,32),gfc_color8(255,255,255,255));

            // gf2d_sprite_draw(mouse,vector2d(mousex,mousey),vector2d(2,2),vector3d(8,8,0),gfc_color(0.3,.9,1,0.9),(Uint32)mouseFrame);
            //slog("X: %f Y: %f \n", player->position.x, player->position.y);
        }
            // //3D draws
            //     gf3d_model_draw_sky(sky,skyMat,gfc_color(1,1,1,1));
        gf3d_vgraphics_render_end();

        if (gfc_input_command_down("exit"))done = 1; // e if (keys[SDL_SCANCODE_S])xit condition
    }    
    
    world_delete(w);
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());
    Mix_FreeMusic(ambiance);
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}
/*eol@eof*/

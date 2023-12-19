#include "simple_logger.h"
#include "gfc_types.h"
#include <math.h>
#include "gfc_vector.h"

#include "resource.h"
#include "world.h"
#include "player.h"

void projectile_think(Entity *self);
void projectile_update(Entity *self);
//Vector3D vector3d_from_angles(float azimuth, float elevation, float radius);
// float proj_velocity = 0;
Entity *projCollisionPartner;


// Vector3D vector3d_from_angles(float azimuth, float elevation, float radius)
// {
//   Vector3D out;
//   out.x = radius * cos(elevation) * cos(azimuth);
//   out.y = radius * cos(elevation) * sin(azimuth);
//   out.z = radius * sin(elevation);
//   return out;
// }

Entity *projectile_new(const char *modelToLoad, const char *name){
    Entity *proj = NULL;

    Entity *player;
    player = entity_get_player();

    proj = entity_new();

    if(!proj)
    {
        slog("Failed to create projectile");
        return NULL;
    }


    proj->selectedColor = gfc_color(0,0,0,1);
    proj->color = gfc_color(1,1,1,1);
    proj->model = gf3d_model_load(modelToLoad);

    proj->think = projectile_think;
    proj->update = projectile_update;
    proj->isProjectile = 1;
    proj->position = (player->position);
    proj->entityName = name;
    proj->proj_ttl = 2000;
    proj->proj_spawntime = SDL_GetTicks64();

    if(gfc_stricmp(name, "AKbullet") == 0)
    {
        proj->proj_velocity = 0.715;
        proj->proj_damage = 20;
    }
    else if(gfc_stricmp(name, "1911bullet") == 0)
    {
        proj->proj_velocity = 0.253;
        proj->proj_damage = 15;
    }
    else if(gfc_stricmp(name, "MP5bullet") == 0)
    {
        proj->proj_velocity = 0.4;
        proj->proj_damage = 15;
    }
    else if(gfc_stricmp(name, "M700bullet") == 0)
    {
        proj->proj_velocity = 0.943;
        proj->proj_damage = 50;
    }
    else if(gfc_stricmp(name, "870bullet") == 0)
    {
        proj->proj_velocity = 0.437;
        proj->proj_damage = 35;
    }
    proj->w = vector2d_from_angle(player->rotation.z);
    //proj->ww = vector3d_from_angles(player->rotation.x, player->rotation.y, player->rotation.z);

    return proj;
}

void projectile_think(Entity *self){

    Entity *player;
    player = entity_get_player();
    Vector3D forward = {0};


    if(!self)return;
    if(!player)return;

     forward.x = ((self->w.x)*(self->proj_velocity));
     forward.y = ((self->w.y)*(self->proj_velocity));
    // forward.x = ((self->w.x)*(0.0625));
    // forward.y = ((self->w.y)*(0.0625));

     // forward.x = ((self->ww.x)*(self->proj_velocity));
     // forward.y = ((self->ww.y)*(self->proj_velocity));
     // forward.z = ((self->ww.z)*(self->proj_velocity));

    vector3d_add(self->position,self->position,forward);

    projCollisionPartner = entity_get_collision_partner(self);
    if (projCollisionPartner != NULL)
        {
            if(projCollisionPartner->isEnemy == 1)
            {
                projCollisionPartner->health -= self->proj_damage;
                entity_free(self);
                slog("Enemy Health: %i", projCollisionPartner->health);
            }
        }
}


void projectile_update(Entity *self){

    //float height;
    Uint64 currentTime;
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    currentTime = SDL_GetTicks64();

    if(currentTime - self->proj_spawntime >= self->proj_ttl)
    {
        entity_free(self);
    }

/*
    height = world_get_collision_height(self->position);
    if(self->position.z > height){
        self->velocity.z -= 0.0000098;
    }
    vector3d_add(self->position,self->position,self->velocity);

    if(self->position.z < height){
        self->position.z = height;
        if(self->velocity.z < 0)self->velocity.z = 0;
    }*/
}



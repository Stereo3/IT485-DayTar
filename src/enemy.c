#include "simple_logger.h"
#include "gfc_types.h"
#include "stdlib.h"

#include "enemy.h"
#include "world.h"


void enemy_think(Entity *self);
void enemy_update(Entity *self);
void enemy_free(Entity *self);

Entity *enemy_new(Vector3D position, const char *modelToLoad, const char *name){

    Entity *enemy = NULL;
    Entity *player = NULL;

    enemy = entity_new();
    player = entity_get_player();

    if(!enemy){
        slog("could not create enemy!");
        return NULL;
    }

    enemy->selectedColor = gfc_color(0,0,0,1);
    enemy->color = gfc_color(1,1,1,1);
    enemy->model = gf3d_model_load(modelToLoad);
    enemy->scale = vector3d(0.0625,0.0625,0.0625);
    //enemy->scale = vector3d(1,1,1);
    enemy->think = enemy_think;
    enemy->update = enemy_update;
    enemy->free = enemy_free;
    enemy->isEnemy = 1;
    vector3d_copy(enemy->position, position);
    //enemy->rotation.y = GFC_HALF_PI;
    enemy->entityName = name;
    enemy->target = player;
    enemy->targetRadius.x = 50;
    enemy->targetRadius.y = 50;
    enemy->targetRadius.z = 50;
    enemy->targetRadius.r = 50;


    if(gfc_stricmp(name, "walker") == 0)
    {
        enemy->health = 100;
    }
    else if(gfc_stricmp(name, "armoured") == 0)
    {
        enemy->health = 200;
    }
    else if(gfc_stricmp(name, "jess") == 0)
    {
        enemy->health = 150;
    }
    return enemy;
}


void enemy_think(Entity *self){

    Entity *player;
    player = entity_get_player();
    Vector3D forward = {0};
    Vector3D right = {0};
    Vector2D w;

    if(!self)return;
    if(!player)return;

    w = vector2d_from_angle(self->rotation.z);
    if(gfc_stricmp(self->entityName, "jess") == 0)
    {
        forward.x = ((w.x)*0.5);
        forward.y = ((w.y)*0.5);
        right.x = ((w.x)*0.5);
        right.y = ((w.y)*0.5);
    }
    else
    {
        forward.x = ((w.x)*0.125);
        forward.y = ((w.y)*0.125);
        right.x = ((w.x)*0.125);
        right.y = ((w.y)*0.125);
    }

    //slog("Distance Difference X: %f", ((player->position.x)*2 - (self->position.x)*2));
    //slog("Distance Difference Y: %f", ((player->position.y)*2 - (self->position.y)*2));

    if( vector3d_magnitude_between(player->position, self->position) < 50)
    {
        self->rotation.z = player->rotation.z;
        vector3d_add(self->position,self->position,-forward);
        if(gfc_stricmp(self->entityName, "walker") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie_aggro.model");
            self->isChasing = 1;
        }
        else if(gfc_stricmp(self->entityName, "armoured") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie2_aggro.model");
            self->isChasing = 1;
        }
        else if(gfc_stricmp(self->entityName, "jess") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie3_aggro.model");
            self->isChasing = 1;
        }
    }
    else if(vector3d_magnitude_between(player->position, self->position) > 50 && self->isChasing == 1)
    {

        if(gfc_stricmp(self->entityName, "walker") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie.model");
            self->isChasing = 0;
        }
        else if(gfc_stricmp(self->entityName, "armoured") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie2.model");
            self->isChasing = 0;
        }
        else if(gfc_stricmp(self->entityName, "jess") == 0)
        {
            gf3d_model_free(self->model);
            self->model = gf3d_model_load("models/zombie3.model");
            self->isChasing = 0;
        }
    }

}

void enemy_update(Entity *self){

    float height;
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }


    height = world_get_collision_height(self->position);
    if(self->position.z > height){
        self->velocity.z -= 0.0000098;
    }
    vector3d_add(self->position,self->position,self->velocity);

    if(self->position.z < height){
        self->position.z = height;
        if(self->velocity.z < 0)self->velocity.z = 0;
    }

    if(self->health <= 0)
    {
        entity_free(self);
    }
}

void enemy_free(Entity *self){
    if(!self)return;
}

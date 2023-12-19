#include "simple_logger.h"
#include "gfc_types.h"

#include "weapon.h"
#include "gfc_audio.h"

void weapon_think(Entity *self);
void weapon_update(Entity *self);
void weapon_free(Entity *self);
Sound *weaponsfx;

Entity *weapon_new(const char *modelToLoad, const char *name){

    Entity *wep = NULL;
    Entity *player = NULL;

    wep = entity_new();
    player = entity_get_player();

    if(!wep){
        slog("You should not own a waepon");
        return NULL;
    }

    wep->selectedColor = gfc_color(0,0,0,1);
    wep->color = gfc_color(1,1,1,1);
    wep->model = gf3d_model_load(modelToLoad);
    wep->think = weapon_think;
    wep->update = weapon_update;
    wep->free = weapon_free;
    wep->isWeapon = 1;
    wep->isFired = 0;
    wep->bounds.x = 0;
    wep->bounds.y = 0;
    wep->bounds.z = 0;
    wep->bounds.r = 0;
    vector3d_copy(wep->position, player->position);
    wep->entityName = name;

    if(gfc_stricmp(name, "AK") == 0)
    {
        wep->scale = vector3d(0.25,0.25,0.25);
        //wep->wep_firerate = 1000;
    }
    else if(gfc_stricmp(name, "1911") == 0)
    {
        wep->scale = vector3d(0.125,0.125,0.125);
    }
    else if(gfc_stricmp(name, "MP5") == 0)
    {
        wep->scale = vector3d(0.25,0.25,0.25);
    }
    else if(gfc_stricmp(name, "M700") == 0)
    {
        wep->scale = vector3d(1,1,1);
    }
    else if(gfc_stricmp(name, "870") == 0)
    {
        wep->scale = vector3d(1,1,1);
    }

    return wep;
}

void weapon_think(Entity *self){

    Entity *player;
    player = entity_get_player();

    if(!self)return;
    if(!player)return;
    self->rotation.z = player->rotation.z;
    //self->rotation.x = player->rotation.x;

    if(gfc_stricmp(self->entityName, "AK") == 0)
    {
       self->position.z = ((player->position.z) - 1);
    }
    else if(gfc_stricmp(self->entityName, "1911") == 0)
    {
        self->position.z = ((player->position.z) - 5);
    }
    else if(gfc_stricmp(self->entityName, "MP5") == 0)
    {
        self->position.z = ((player->position.z) - 2);
    }
    else if(gfc_stricmp(self->entityName, "M700") == 0)
    {
        self->position.z = ((player->position.z) - 2);
    }
    else if(gfc_stricmp(self->entityName, "870") == 0)
    {
        self->position.z = ((player->position.z) - 4);
    }
}

void weapon_update(Entity *self){
    Entity *player;
    player = entity_get_player();

    if(!self)return;
    if(!player)return;

     self->position.y = player->position.y;
     self->position.x = player->position.x;
}

void weapon_fire(Entity *self, const char *name)
{
    if(!self)
    {
        slog("no self pointer provided");
    }

    if(gfc_stricmp(self->entityName, "AK") == 0)
    {
        gf3d_model_free(self->model);
        self->model = gf3d_model_load("models/ak47_fire.model");
        weaponsfx = gfc_sound_load("sfx/ak47.wav",.75,0);
        gfc_sound_play(weaponsfx,0,1,0,0);
        gfc_sound_free(weaponsfx);
        //wep_model_play_anim(self, "models/ak47fireanim/ak47_fire.model");
        self->isFired = 1;
    }
    else if(gfc_stricmp(self->entityName, "1911") == 0)
    {
        self->model = gf3d_model_load("models/1911_fire.model");
        self->isFired = 1;
    }
    else if(gfc_stricmp(self->entityName, "MP5") == 0)
    {
        self->model = gf3d_model_load("models/mp5_fire.model");
        self->isFired = 1;
    }
    else if(gfc_stricmp(self->entityName, "M700") == 0)
    {
        self->model = gf3d_model_load("models/m700_fire.model");
        weaponsfx = gfc_sound_load("sfx/m700.wav",.75,0);
        gfc_sound_play(weaponsfx,0,1,0,0);
        gfc_sound_free(weaponsfx);
        self->isFired = 1;
    }
    else if(gfc_stricmp(self->entityName, "870") == 0)
    {
        self->model = gf3d_model_load("models/870_fire.model");
        self->isFired = 1;
    }
}

void weapon_fire_done(Entity *self, const char *name)
{

    if(gfc_stricmp(self->entityName, "AK") == 0)
    {
        gf3d_model_free(self->model);
        self->model = gf3d_model_load("models/ak47.model");
        self->isFired = 0;
        self->secondHasPassed = 0;
    }
    else if(gfc_stricmp(self->entityName, "1911") == 0)
    {
        self->model = gf3d_model_load("models/1911.model");
        self->isFired = 0;
        self->secondHasPassed = 0;
    }
    else if(gfc_stricmp(self->entityName, "MP5") == 0)
    {
        self->model = gf3d_model_load("models/mp5.model");
        self->isFired = 0;
        self->secondHasPassed = 0;
    }
    else if(gfc_stricmp(self->entityName, "M700") == 0)
    {
        self->model = gf3d_model_load("models/m700.model");
        self->isFired = 0;
        self->secondHasPassed = 0;
    }
    else if(gfc_stricmp(self->entityName, "870") == 0)
    {
        self->model = gf3d_model_load("models/870.model");
        self->isFired = 0;
        self->secondHasPassed = 0;
    }
}



void wep_model_play_anim(Entity *wepToAnim, const char *filename)
{
    SJson *json,*config;
    Model *model;
    char modelStr[7];

    if (!filename)
    {
        slog("Cannot play play this animation file");
    }
    json = sj_load(filename);
    if (!json)
    {
        slog("failed to create json from anim file");
        sj_free(json);
    }
    for(int i = 0; i <= 11; i++)
    {
        sprintf(modelStr, "model%i", i);
        slog("modelStr: %s", modelStr);
        config = sj_object_get_value(json, modelStr);
        if (!config)
        {
            slog("file %s contains no model object",filename);
            continue;
        }
        gf3d_model_free(wepToAnim->model);
        wepToAnim->model = gf3d_model_load_from_config(config);
    }

    sj_free(json);
}


void weapon_free(Entity *self){
    if(!self)return;
}

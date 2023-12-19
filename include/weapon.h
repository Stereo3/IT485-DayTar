#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "entity.h"
#include <stdio.h>
#include "simple_json.h"


/**
 * @brief Create a new weapon entity
 * @param modelToLoad what model to load for the weapon
 * @param name the name of the weapon
 * @return NULL on error, or an weapon entity pointer on success
 */
Entity *weapon_new(const char *modelToLoad, const char *name);

/**
 * @brief Function to trigger the firing animation
 * @param self a pointer to the weapon to fire
 * @param name the name of the weapon to fire
 * @return Does not return any value
 */
void weapon_fire(Entity *self, const char *name);

void weapon_fire_done(Entity *self, const char *name);


void gf3d_model_play_anim(Entity *self, const char *filename);


#endif



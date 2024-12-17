#ifndef RAYLIB_TEXT_BOX_EFFECTS_MAIN_H
#define RAYLIB_TEXT_BOX_EFFECTS_MAIN_H


#include "../config.h"
#include "../main.h"


TextBoxEffect * text_box_effect_typewriter_make(float);
void text_box_effect_typewriter_destroy(TextBoxEffect *);
int text_box_effect_typewriter_has_finished(TextBoxEffect const *);
void text_box_effect_typewriter_cancel(TextBoxEffect *);


#endif

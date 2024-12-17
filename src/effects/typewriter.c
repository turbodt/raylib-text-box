#include "./main.h"


typedef struct TypewriterEffect {
    TextBoxEffect base;
    size_t current_display_id;
    size_t current_display_count;
    double start_time;
    double time_offset;
    float chars_per_sec;
} TypewriterEffect;


static void typewriter_effect_fn(TextBoxEffectProps);

TextBoxEffect * text_box_effect_typewriter_make(float chars_per_sec) {
    TypewriterEffect * effect = RAYLIB_TEXT_BOX_ALLOC(TypewriterEffect, 1);
    if (effect == NULL) {
        return &effect->base;
    }
    effect->base.call = typewriter_effect_fn;
    effect->current_display_id = (size_t)(-1);
    effect->current_display_count = 0;
    effect->start_time = 0.0;
    effect->chars_per_sec = chars_per_sec < 0 ? 0.0f : chars_per_sec;

    return &effect->base;
};


void text_box_effect_typewriter_destroy(TextBoxEffect *effect) {
    if (effect == NULL) {
        return;
    }
    RAYLIB_TEXT_BOX_FREE(effect);
};


int text_box_effect_typewriter_has_finished(TextBoxEffect const *effect) {
    TypewriterEffect const * impl = (void const *) effect;

    double animation_time =
        GetTime() - impl->start_time - impl->time_offset;

    return (
        (double)(impl->current_display_count)
        < impl->chars_per_sec*animation_time
    );
};

void text_box_effect_typewriter_cancel(TextBoxEffect *effect) {
    TypewriterEffect * impl = (void *) effect;
    impl->time_offset =
        GetTime() - impl->start_time - (double)impl->current_display_count;
};

void typewriter_effect_fn(TextBoxEffectProps props) {
    TypewriterEffect * effect = (TypewriterEffect *) props.effect;
    TextBoxProps const * tb_props = props.text_box_props;
    size_t index = props.codepoint_byte_index - props.display_byte_offset;
    int const codepoint = props.codepoint;
    Vector2 const tl_pos = props.tl_pos;

    if (props.display_byte_offset != effect->current_display_id) {
        effect->current_display_id = props.display_byte_offset;
        effect->current_display_count = props.display_byte_count;
        effect->start_time = GetTime();
        effect->time_offset = 0.0;
    }

    double animation_time =
        GetTime() - effect->start_time - effect->time_offset;

    if ((double)index > effect->chars_per_sec*animation_time) {
        return;
    }

    DrawTextCodepoint(
        tb_props->font,
        codepoint,
        tl_pos,
        tb_props->font_size,
        tb_props->color
    );
};

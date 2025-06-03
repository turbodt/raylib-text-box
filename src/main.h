#ifndef RAYLIB_TEXT_BOX_MAIN_H
#define RAYLIB_TEXT_BOX_MAIN_H


#include <raylib.h>
#include "./config.h"


typedef struct TextBox TextBox;
typedef struct TextBoxEffectProps TextBoxEffectProps;
typedef struct TextBoxEffect TextBoxEffect;

typedef enum TextBoxAlignment {
    TEXT_BOX_ALIGNMENT__UNDEF = 1,
    TEXT_BOX_ALIGNMENT__START = 1,
    TEXT_BOX_ALIGNMENT__CENTER,
    TEXT_BOX_ALIGNMENT__END,
} TextBoxAlignment;

typedef struct TextBoxProps {
    int font_size;
    int spacing;
    int line_spacing;
    Font font;
    Color color;
    struct {
        TextBoxAlignment horizontal;
        TextBoxAlignment vertical;
        TextBoxAlignment line;
    } alignment;
    TextBoxEffect *effect;
} TextBoxProps;

struct TextBoxEffect {
    void (*call)(TextBoxEffectProps);
};

struct TextBoxEffectProps {
    TextBoxEffect *effect;
    TextBoxProps const *text_box_props;
    size_t display_byte_count;
    size_t line_byte_count;
    size_t display_byte_offset;
    size_t line_byte_offset;
    size_t codepoint_byte_index;
    Vector2 tl_pos;
    int codepoint;
};


UPTR(TextBox) text_box_make(TextBoxProps);
UPTR(char const) text_box_release_and_destroy(UPTR(TextBox));

TextBoxProps text_box_props_get(TextBox const *);
size_t text_box_display_get_byte_count(TextBox const *);
size_t text_box_display_get_byte_offset(TextBox const *);
size_t text_box_display_get_next_byte_offset(TextBox const *);
int text_box_display_is_last(TextBox const *);
TextBoxEffect const * text_box_effect_getc(TextBox const *);
TextBoxEffect * text_box_effect_get(TextBox *);

int text_box_props_set(TextBox *, TextBoxProps);
UPTR(char const) text_box_text_update(TextBox *, UPTR(char const));
int text_box_size_set(TextBox *, int, int, int, int);
void text_box_color_set(TextBox *, Color);
int text_box_display_from(TextBox *, size_t);
int text_box_display_next(TextBox *);

int text_box_render(TextBox *);


#endif

#ifndef RAYLIB_TEXT_BOX_MAIN_H
#define RAYLIB_TEXT_BOX_MAIN_H


#include <raylib.h>
#include "./config.h"


typedef struct TextBox TextBox;
typedef struct TextBoxProps {
    int font_size;
    int spacing;
    int line_spacing;
    Font font;
    Color color;
} TextBoxProps;


UPTR(TextBox) text_box_make(TextBoxProps);
UPTR(char const) text_box_release_and_destroy(UPTR(TextBox));

TextBoxProps text_box_props_get(TextBox const *);
size_t text_box_display_get_byte_len(TextBox const *);
size_t text_box_display_get_byte_index(TextBox const *);
size_t text_box_display_get_next_byte_index(TextBox const *);
int text_box_display_is_last(TextBox const *);

int text_box_props_set(TextBox *, TextBoxProps);
UPTR(char const) text_box_text_update(TextBox *, UPTR(char const));
int text_box_size_set(TextBox *, int, int, int, int);
int text_box_display_from(TextBox *, size_t);
int text_box_display_next(TextBox *);

int text_box_render(TextBox *);


#endif

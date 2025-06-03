#include "raylib.h"
#include "raylib-text-box.h"
#include <math.h>


#define EFFECT_COUNT 4
#define COLS 2
#define ROWS ((int)(EFFECT_COUNT / COLS) + ((EFFECT_COUNT % COLS) ? 1 : 0))

#define TYPEWRITER_EFFECT_CHARS_PER_SEC 40

static void effect_rumble_call(TextBoxEffectProps);
static TextBoxEffect effect_rumble = {.call=&effect_rumble_call};
static TextBoxEffect * effect_typewriter_make(void);
static void effect_typewriter_destroy(TextBoxEffect *);
static TextBoxEffect * effect_wave_make(void);
static void effect_wave_destroy(TextBoxEffect *);
static TextBoxEffect * effect_color_diag_make(void);
static void effect_color_diag_destroy(TextBoxEffect *);

int main(void) {
    InitWindow(800, 600, "Effects example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TextBoxEffect *effects[EFFECT_COUNT] = {
        &effect_rumble,
        effect_typewriter_make(),
        effect_wave_make(),
        effect_color_diag_make(),
    };

    TextBox * tb[EFFECT_COUNT];
    for (int i = 0; i < EFFECT_COUNT; i++) {
        tb[i] = text_box_make((TextBoxProps) {
            .font = GetFontDefault(),
            .font_size = 2*GetFontDefault().baseSize,
            .spacing = 1,
            .line_spacing = 1,
            .color = DARKGRAY,
            .effect = effects[i],
        });
    }
    text_box_text_update(
        tb[0],
        "This is the rumble effect.\n"
        "\nThis effect does not require to save any state,"
        " so we can reuse the same instance for all the text boxes.\n"
        "In consequence, no memory allocation or deallocation is required."
    );
    text_box_text_update(
        tb[1],
        "This is the typewriter effect.\n"
        "\nThis effect require to save the check box state,"
        " so we cannot reuse the same instance for all the text boxes.\n"
        "In consequence, memory allocation and deallocation is required."
    );
    text_box_text_update(
        tb[2],
        "This is the wave effect.\n"
        "\nThis is another example of a custom effect."
        "Here we reuse the typewriter's state.\n"
        "\nLook at the example's code to figure out how to make your own!."
    );
    text_box_text_update(
        tb[3],
        "This is a custom color diagonal effect.\n"
        "\nThis is another example of a custom effect."
        "Here we show how, for each character we can use information"
        " from the line position as well from display's position.\n"
        "\nLook at the example's code to figure out how to make your own!."
    );

    int padding = GetFontDefault().baseSize *2;

    int screenWidth = -1, screenHeight = -1;
    while (!WindowShouldClose()) {
        int has_screen_resized =
            GetScreenWidth() != screenWidth
            || GetScreenHeight() != screenHeight;

        if (has_screen_resized) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            for (int i = 0; i < EFFECT_COUNT; i++) {
                int row = i / COLS;
                int col = i % COLS;
                text_box_size_set(
                    tb[i],
                    col*screenWidth/COLS + padding,
                    row*screenHeight/ROWS + padding,
                    screenWidth/COLS -2*padding,
                    screenHeight/ROWS -2*padding
                );
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            for (int i = 0; i < EFFECT_COUNT; i++) {
                int row = i / COLS;
                int col = i % COLS;
                DrawRectangleLines(
                    col*screenWidth/COLS + padding,
                    row*screenHeight/ROWS + padding,
                    screenWidth/COLS - 2*padding,
                    screenHeight/ROWS - 2*padding,
                    BLUE
                );
                int err = text_box_render(tb[i]);
                if (err) {break;}
            }
        };
        EndDrawing();
    }

    for (int i = 0; i < EFFECT_COUNT; i++) {
        text_box_release_and_destroy(tb[i]);
    }
    effect_typewriter_destroy(effects[1]);
    effect_wave_destroy(effects[2]);
    effect_color_diag_destroy(effects[3]);

    CloseWindow();

    return 0;
}

// rumble effect

void effect_rumble_call(TextBoxEffectProps props) {
    TextBoxProps const * tb_props = props.text_box_props;
    int const codepoint = props.codepoint;
    Vector2 tl_pos = props.tl_pos;

    int dice = rand() % 100;
    if (dice > 90) {
        tl_pos.x++;
    } else if (dice < 10) {
        tl_pos.x--;
    };

    DrawTextCodepoint(
        tb_props->font,
        codepoint,
        tl_pos,
        tb_props->font_size,
        tb_props->color
    );
};

// typewriter effect

typedef struct TypewriterEffect {
    TextBoxEffect base;
    size_t current_display;
    double start_time;
} TypewriterEffect;
static void typewriter_effect_fn(TextBoxEffectProps);

TextBoxEffect * effect_typewriter_make(void) {
    TypewriterEffect * effect = RAYLIB_TEXT_BOX_ALLOC(TypewriterEffect, 1);
    if (effect == NULL) {
        return &effect->base;
    }
    effect->base.call = typewriter_effect_fn;
    effect->current_display = (size_t)(-1);
    effect->start_time = 0.0;

    return &effect->base;
};


void effect_typewriter_destroy(TextBoxEffect *effect) {
    if (effect == NULL) {
        return;
    }
    RAYLIB_TEXT_BOX_FREE(effect);
};

void typewriter_effect_fn(TextBoxEffectProps props) {
    TypewriterEffect * effect = (TypewriterEffect *) props.effect;
    TextBoxProps const * tb_props = props.text_box_props;
    size_t index = props.codepoint_byte_index - props.display_byte_offset;
    int const codepoint = props.codepoint;
    Vector2 const tl_pos = props.tl_pos;

    if (props.display_byte_offset != effect->current_display) {
        effect->current_display = props.display_byte_offset;
        effect->start_time = GetTime();
    }
    double animation_time = GetTime() - effect->start_time;

    if ((double)index > TYPEWRITER_EFFECT_CHARS_PER_SEC*animation_time) {
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


// wave effect

static void wave_effect_fn(TextBoxEffectProps);

TextBoxEffect * effect_wave_make(void) {
    TypewriterEffect * effect = (TypewriterEffect *) effect_typewriter_make();
    if (effect == NULL) {
        return &effect->base;
    }
    effect->base.call = wave_effect_fn;

    return &effect->base;
};


inline void effect_wave_destroy(TextBoxEffect *effect) {
    effect_typewriter_destroy(effect);
};

void wave_effect_fn(TextBoxEffectProps props) {
    TypewriterEffect * effect = (TypewriterEffect *) props.effect;
    TextBoxProps const * tb_props = props.text_box_props;
    size_t index = props.codepoint_byte_index - props.display_byte_offset;
    int const codepoint = props.codepoint;
    Vector2 tl_pos = props.tl_pos;

    if (props.display_byte_offset != effect->current_display) {
        effect->current_display = props.display_byte_offset;
        effect->start_time = GetTime();
    }
    double animation_time = GetTime() - effect->start_time;

    double h = cos(0.1*((double)index) + 10*animation_time);
    tl_pos.y -= tb_props->font_size * h * h / 4;

    DrawTextCodepoint(
        tb_props->font,
        codepoint,
        tl_pos,
        tb_props->font_size,
        tb_props->color
    );
};

// color diag effect

static void effect_color_diag_fn(TextBoxEffectProps);

TextBoxEffect * effect_color_diag_make(void) {
    TypewriterEffect * effect = (TypewriterEffect *) effect_typewriter_make();
    if (effect == NULL) {
        return &effect->base;
    }
    effect->base.call = effect_color_diag_fn;

    return &effect->base;
};


inline void effect_color_diag_destroy(TextBoxEffect *effect) {
    effect_typewriter_destroy(effect);
};

void effect_color_diag_fn(TextBoxEffectProps props) {
    TypewriterEffect * effect = (TypewriterEffect *) props.effect;
    TextBoxProps const * tb_props = props.text_box_props;
    size_t line_index = props.codepoint_byte_index - props.line_byte_offset;
    size_t display_index =
        props.codepoint_byte_index - props.display_byte_offset;
    int const codepoint = props.codepoint;
    Vector2 const tl_pos = props.tl_pos;
    Color color = tb_props->color;
    int const score = line_index + (int)(display_index / props.line_byte_count);

    if (props.display_byte_offset != effect->current_display) {
        effect->current_display = props.display_byte_offset;
        effect->start_time = GetTime();
    }
    int animation_time = (int)(5*(GetTime() - effect->start_time));

    if (animation_time%15 == (score % 15)) {
        color = RED;
    }

    DrawTextCodepoint(
        tb_props->font,
        codepoint,
        tl_pos,
        tb_props->font_size,
        color
    );
};

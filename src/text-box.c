#include "./main.h"
#include "./protected.h"


typedef struct {
    char const * text;
    size_t offset;
    size_t index_count;
    Vector2 size;
} ChunkInfo;
struct TextBox {
    TextBoxProps props;
    char const * text;
    Vector2 tl_pos;
    Vector2 box_size;
    ChunkInfo current_chunk;
};


static int recalcule_state(TextBox *, size_t);
static void draw_chunk_at(TextBoxProps const *, ChunkInfo, Vector2);
static void draw_line_at(TextBoxProps const *, ChunkInfo, ChunkInfo, Vector2);
static ChunkInfo calcule_chunk(
    TextBoxProps const *,
    Vector2,
    char const *,
    size_t
);
static ChunkInfo calcule_line(TextBoxProps const *, int, char const *, size_t);
static size_t rtrim_advance(char const *);
static int get_codepoint_width(Font const, int, float);
static int get_codepoint_height(Font const, int, int);
static int is_codepoint_trimable(int);
static int is_codepoint_word_break(int);
static int is_codepoint_line_break(int);


UPTR(TextBox) text_box_make(TextBoxProps props) {
    TextBox * impl = ALLOC(TextBox, 1);
    if (impl == NULL) {
        return NULL;
    }

    impl->props = props;
    impl->box_size = (Vector2){0,0};
    impl->tl_pos = (Vector2){0,0};
    impl->text = NULL;
    impl->current_chunk = (ChunkInfo) {
        .text = NULL,
        .offset = 0,
        .index_count = 0,
        .size = (Vector2){0,0},
    };

    return impl;
};


UPTR(char const) text_box_release_and_destroy(UPTR(TextBox) impl) {
    char const * text = impl->text;
    FREE(impl);
    return text;
}


inline TextBoxProps text_box_props_get(TextBox const *impl) {
    return impl->props;
};


inline size_t text_box_display_get_byte_len(TextBox const *impl) {
    return impl->current_chunk.index_count;
};


inline size_t text_box_display_get_byte_index(TextBox const *impl) {
    return impl->current_chunk.offset;
};


inline size_t text_box_display_get_next_byte_index(TextBox const *impl) {
    return text_box_display_get_byte_index(impl)
        + text_box_display_get_byte_len(impl);
};


inline int text_box_display_is_last(TextBox const *impl) {
    if (impl->text == NULL) {
        return 1;
    }
    size_t next_index = text_box_display_get_next_byte_index(impl);
    return impl->text[next_index] == '\0';
};


inline int text_box_props_set(TextBox *impl, TextBoxProps props) {
    int should_recalcule =
        props.font_size != impl->props.font_size
        || props.font.recs != impl->props.font.recs
        || props.font.baseSize != impl->props.font.baseSize
        || props.spacing != impl->props.spacing
        || props.line_spacing != impl->props.line_spacing
    ;
    impl->props = props;

    if (!should_recalcule) {
        return 0;
    }

    return recalcule_state(impl, impl->current_chunk.offset);
};


UPTR(char const) text_box_text_update(TextBox *impl, UPTR(char const)new_text) {
    char const * text = impl->text;
    impl->text = new_text;

    int err = recalcule_state(impl, 0);
    if (err) {
        impl->text = text;
        text = new_text;
    }

    return text;
};


inline int text_box_size_set(TextBox *impl, int x, int y, int w, int h) {
    impl->tl_pos.x = x;
    impl->tl_pos.y = y;
    impl->box_size.x = w;
    impl->box_size.y = h;
    return recalcule_state(impl, impl->current_chunk.offset);
};


int text_box_display_from(TextBox *impl, size_t index) {
    return recalcule_state(impl, index);
};


inline int text_box_display_next(TextBox *impl) {
    if (text_box_display_is_last(impl)) {
        return 1; // TODO: Return error code
    }
    size_t next_index = text_box_display_get_next_byte_index(impl);
    return text_box_display_from(impl, next_index);
};


int text_box_render(TextBox *impl) {
    float margin_h = impl->box_size.x - impl->current_chunk.size.x;
    float margin_v = impl->box_size.y - impl->current_chunk.size.y;

    margin_h = MAX(margin_h, 0);
    margin_v = MAX(margin_v, 0);

    Vector2 tl_pos = impl->tl_pos;
    switch (impl->props.alignment.horizontal) {
        case TEXT_BOX_ALIGNMENT__CENTER:
            tl_pos.x += margin_h / 2;
        break;
        case TEXT_BOX_ALIGNMENT__END:
            tl_pos.x += margin_h;
        break;
        default:
        break;
    };
    switch (impl->props.alignment.vertical) {
        case TEXT_BOX_ALIGNMENT__CENTER:
            tl_pos.y += margin_v / 2;
        break;
        case TEXT_BOX_ALIGNMENT__END:
            tl_pos.y += margin_v;
        break;
        default:
        break;
    };

    draw_chunk_at(&impl->props, impl->current_chunk, tl_pos);
    return 0; // TODO: Return error code
};


int recalcule_state(TextBox *impl, size_t offset) {
    if (impl->text == NULL) {
        return 1;
    }

    impl->current_chunk = calcule_chunk(
        &impl->props,
        impl->box_size,
        impl->text,
        offset
    );

    return 0;
};


void draw_chunk_at(
    TextBoxProps const *props,
    ChunkInfo chunk,
    Vector2 tl_pos
) {
    size_t index_count = 0;
    Vector2 curr_pos = tl_pos;
    while (index_count < chunk.index_count) {
        size_t const curr_index = chunk.offset + index_count;
        ChunkInfo line_info = calcule_line(
            props,
            chunk.size.x,
            chunk.text,
            curr_index
        );

        float line_margin = chunk.size.x - line_info.size.x;
        line_margin = MAX(line_margin, 0);

        switch (props->alignment.line) {
            case TEXT_BOX_ALIGNMENT__CENTER:
                curr_pos.x += line_margin / 2;
            break;
            case TEXT_BOX_ALIGNMENT__END:
                curr_pos.x += line_margin;
            break;
            default:
            break;
        };

        draw_line_at(props, chunk, line_info, curr_pos);
        index_count += line_info.index_count;
        curr_pos.x = tl_pos.x;
        curr_pos.y += line_info.size.y + props->line_spacing;
    }
};


void draw_line_at(
    TextBoxProps const *props,
    ChunkInfo const chunk,
    ChunkInfo line_info,
    Vector2 tl_pos
) {
    float const scale_factor = (float)props->font_size / props->font.baseSize;
    Vector2 curr_pos = tl_pos;

    size_t line_index_count = rtrim_advance(&line_info.text[line_info.offset]);
    while (line_index_count < line_info.index_count) {
        size_t const curr_index = line_info.offset + line_index_count;
        char const * curr_text = &line_info.text[curr_index];

        int cp_byte_count = 0;
        int cp = GetCodepointNext(curr_text, &cp_byte_count);
        int cp_index = GetGlyphIndex(props->font, cp);
        int const cp_width = get_codepoint_width(
            props->font,
            cp_index,
            scale_factor
        );

        if (is_codepoint_line_break(cp)) {
            // Do nothing
        } else if (props->effect == NULL) {
            DrawTextCodepoint(
                props->font,
                cp,
                curr_pos,
                props->font_size,
                props->color
            );
        } else {
            TextBoxEffectProps e_props = {
                .effect = props->effect,
                .text_box_props = props,
                .codepoint = cp,
                .tl_pos = curr_pos,
                .display_byte_count = chunk.index_count,
                .line_byte_count = line_info.index_count,
                .display_byte_offset = chunk.offset,
                .line_byte_offset = line_info.offset,
                .codepoint_byte_index = curr_index
            };
            props->effect->call(e_props);
        }
        curr_pos.x += cp_width + props->spacing;
        line_index_count += cp_byte_count;
    }
};


ChunkInfo calcule_chunk(
    TextBoxProps const *props,
    Vector2 const max_size,
    char const *text,
    size_t const offset
) {
    ChunkInfo ret = {.text = text, .offset=offset, .index_count=0, .size={0}};

    ChunkInfo tentative = ret;

    int is_first_printed_line = 1;
    int chunk_must_end = 0;
    while (!chunk_must_end) {
        size_t const curr_index = tentative.offset + tentative.index_count;
        char const * const curr_text = &tentative.text[curr_index];

        if (tentative.size.y > max_size.y) {
            chunk_must_end = 1;
            continue;
        }

        ret = tentative;

        if (*curr_text == '\0') {
            chunk_must_end = 1;
            continue;
        }

        ChunkInfo line_info = calcule_line(
            props,
            max_size.x,
            tentative.text,
            curr_index
        );
        if (line_info.index_count == 0) {
            chunk_must_end = 1;
            continue;
        }

        tentative.size.x = MAX(tentative.size.x, line_info.size.x);
        tentative.size.y += line_info.size.y;
        if (is_first_printed_line) {
            is_first_printed_line = 0;
        } else {
            tentative.size.y += props->line_spacing;
        }
        tentative.index_count += line_info.index_count;

    }
    return ret;
}


ChunkInfo calcule_line(
    TextBoxProps const *props,
    int const max_width,
    char const *text,
    size_t offset
) {
    float const scale_factor = (float)props->font_size
        / (float)props->font.baseSize;

    ChunkInfo ret = {.text = text, .offset=offset, .index_count=0, .size = {0}};

    ChunkInfo tentative = ret;
    tentative.index_count = rtrim_advance(&text[offset]);

    int is_first_printable_char = 1;
    int line_must_end = 0;
    while (!line_must_end) {
        size_t const curr_index = tentative.offset + tentative.index_count;
        char const * curr_text = &tentative.text[curr_index];

        if (tentative.size.x > max_width) {
            line_must_end = 1;
            continue;
        }

        if (*curr_text == '\0') {
            ret = tentative;
            line_must_end = 1;
            continue;
        }

        int cp_byte_count = 0;
        int const cp = GetCodepointNext(curr_text, &cp_byte_count);

        if (is_codepoint_word_break(cp)) {
            ret = tentative;
        }

        int const cp_index = GetGlyphIndex(props->font, cp);
        int const cp_width = get_codepoint_width(
            props->font,
            cp_index,
            scale_factor
        );
        int const cp_height = get_codepoint_height(
            props->font,
            cp_index,
            props->font_size
        );

        tentative.size.y = MAX(tentative.size.y, cp_height);
        tentative.index_count += cp_byte_count;

        if (is_codepoint_line_break(cp)) {
            ret = tentative;
            line_must_end = 1;
            continue;
        } else if (is_first_printable_char) {
            is_first_printable_char = 0;
        } else {
            tentative.size.x += props->spacing;
        }

        tentative.size.x += cp_width;
    }

    if (ret.index_count == 0) {
        ret = tentative;
    }

    return ret;
};


inline size_t rtrim_advance(char const *curr_text) {
    size_t byte_count = 0;
    while (*curr_text != '\0') {
        int cp_byte_count = 0;
        int const cp = GetCodepointNext(curr_text, &cp_byte_count);
        if (!is_codepoint_trimable(cp)) {
            break;
        }
        curr_text += cp_byte_count;
        byte_count += cp_byte_count;
    }
    return byte_count;
}


inline int get_codepoint_width(
    Font const font,
    int const cp_index,
    float const scale_factor
) {
    if ((char) font.glyphs[cp_index].value == '\n') {
        return 0;
    } else if (font.glyphs[cp_index].advanceX == 0) {
        return (float)(font.recs[cp_index].width)*scale_factor;
    } else {
        return (float)(font.glyphs[cp_index].advanceX)*scale_factor;
    }
};


inline int get_codepoint_height(
    Font const font,
    int const cp_index,
    int const font_size
) {
    (void)cp_index;
    (void) font;
    return font_size;
};


inline int is_codepoint_trimable(int codepoint) {
    return
        codepoint == ' '
        || codepoint == '\t'
    ;
};


inline int is_codepoint_word_break(int codepoint) {
    return
        codepoint == ' '
        || is_codepoint_line_break(codepoint)
        || codepoint == '\t'
    ;
};


inline int is_codepoint_line_break(int codepoint) {
    return codepoint == '\n' || codepoint == '\0';
};

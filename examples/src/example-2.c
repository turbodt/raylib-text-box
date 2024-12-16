#include "raylib.h"
#include "raylib-text-box.h"


int main(void) {
    InitWindow(800, 450, "Align example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TextBox * tb[9];
    for (int i = 0; i < 9; i++) {
        int row = i/3;
        int col = i%3;
        TextBoxAlignment align_h = TEXT_BOX_ALIGNMENT__START + col;
        TextBoxAlignment align_v = TEXT_BOX_ALIGNMENT__START + row;
        tb[i] = text_box_make((TextBoxProps) {
            .font = GetFontDefault(),
            .font_size = 2*GetFontDefault().baseSize,
            .spacing = 1,
            .line_spacing = 1,
            .color = DARKGRAY,
            .alignment = {align_h, align_v}
        });
    }
    text_box_text_update(tb[0], "Top Left");
    text_box_text_update(tb[1], "Top Center");
    text_box_text_update(tb[2], "Top Right");
    text_box_text_update(tb[3], "Center Left");
    text_box_text_update(tb[4], "Center Center");
    text_box_text_update(tb[5], "Center Right");
    text_box_text_update(tb[6], "Bottom Left");
    text_box_text_update(tb[7], "Bottom Center");
    text_box_text_update(tb[8], "Bottom Right");

    int padding = GetFontDefault().baseSize *2;


    int screenWidth = -1, screenHeight = -1;
    while (!WindowShouldClose()) {
        int has_screen_resized =
            GetScreenWidth() != screenWidth
            || GetScreenHeight() != screenHeight;

        if (has_screen_resized) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            for (int i = 0; i < 9; i++) {
                int row = i/3;
                int col = i%3;
                text_box_size_set(
                    tb[i],
                    col*screenWidth/3 + padding,
                    row*screenHeight/3 + padding,
                    screenWidth/3 -2*padding,
                    screenHeight/3 -2*padding
                );
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            for (int i = 0; i < 9; i++) {
                int row = i/3;
                int col = i%3;
                DrawRectangleLines(
                    col*screenWidth/3,
                    row*screenHeight/3,
                    screenWidth/3,
                    screenHeight/3,
                    BLUE
                );
                int err = text_box_render(tb[i]);
                if (err) {break;}
            }
        };
        EndDrawing();
    }

    for (int i = 0; i < 9; i++) {
        text_box_release_and_destroy(tb[i]);
    }
    CloseWindow();

    return 0;
}

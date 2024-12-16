#include "raylib.h"
#include "raylib-text-box.h"


int main(void) {
    InitWindow(800, 450, "Align line example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TextBox * tb[3];
    for (int i = 0; i < 3; i++) {
        tb[i] = text_box_make((TextBoxProps) {
            .font = GetFontDefault(),
            .font_size = 2*GetFontDefault().baseSize,
            .spacing = 1,
            .line_spacing = 1,
            .color = DARKGRAY,
            .alignment = {
                TEXT_BOX_ALIGNMENT__START + i,
                TEXT_BOX_ALIGNMENT__CENTER,
                TEXT_BOX_ALIGNMENT__START + i
            }
        });
    }
    text_box_text_update(tb[0], "The alignment of\n this lines are\n Left");
    text_box_text_update(tb[1], "The alignment of\n this lines are\n Center");
    text_box_text_update(tb[2], "The alignment of\n this lines are\n Right");

    int padding = GetFontDefault().baseSize *2;


    int screenWidth = -1, screenHeight = -1;
    while (!WindowShouldClose()) {
        int has_screen_resized =
            GetScreenWidth() != screenWidth
            || GetScreenHeight() != screenHeight;

        if (has_screen_resized) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            for (int i = 0; i < 3; i++) {
                text_box_size_set(
                    tb[i],
                    i*screenWidth/3 + padding,
                    padding,
                    screenWidth/3 -2*padding,
                    screenHeight -2*padding
                );
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            for (int i = 0; i < 3; i++) {
                DrawRectangleLines(
                    i*screenWidth/3 + padding,
                    0 + padding,
                    screenWidth/3 - 2*padding,
                    screenHeight - 2*padding,
                    BLUE
                );
                int err = text_box_render(tb[i]);
                if (err) {break;}
            }
        };
        EndDrawing();
    }

    for (int i = 0; i < 3; i++) {
        text_box_release_and_destroy(tb[i]);
    }
    CloseWindow();

    return 0;
}

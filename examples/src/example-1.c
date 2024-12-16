#include "raylib.h"
#include "raylib-text-box.h"


static const char long_text[4096];


int main(void) {
    InitWindow(800, 450, "Text Box Example");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    TextBox * tb = text_box_make((TextBoxProps) {
        .font = GetFontDefault(),
        .font_size = 2*GetFontDefault().baseSize,
        .spacing = 1,
        .line_spacing = 1,
        .color = DARKGRAY,
        .alignment = {0},
    });
    TextBoxProps tb_props = text_box_props_get(tb);
    char const * dummy = text_box_text_update(tb, long_text);
    if (dummy == long_text) exit(1);

    const float p = 0.8f;
    int padding = 0*tb_props.font_size;


    Rectangle frame;
    int screenWidth = -1, screenHeight = -1;
    while (!WindowShouldClose()) {
        int has_screen_resized =
            GetScreenWidth() != screenWidth
            || GetScreenHeight() != screenHeight;

        if (has_screen_resized) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            frame = (Rectangle){
                .x=(int)((1-p)*screenWidth/2),
                .y=(int)((1-p)*screenHeight/2),
                .width =(int) (p*screenWidth),
                .height =(int) (p*screenHeight),
            };

            text_box_size_set(
                tb,
                (int)frame.x + padding,
                (int)frame.y + padding,
                (int)frame.width -2*padding,
                (int)frame.height -2*padding
            );
        }

        if (!IsKeyReleased(KEY_SPACE)) {
        } else if (text_box_display_is_last(tb)) {
            text_box_display_from(tb, 0);
        } else {
            text_box_display_next(tb);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawRectangleLines(frame.x, frame.y, frame.width, frame.height, BLUE);
            int err = text_box_render(tb);
            if (err) {break;}
            if (!text_box_display_is_last(tb)) {
                DrawTextEx(
                    tb_props.font,
                    "Press <space>",
                    (Vector2){frame.x, frame.y + frame.height + tb_props.line_spacing},
                    tb_props.font_size,
                    tb_props.spacing,
                    tb_props.color
                );
            }

        };
        EndDrawing();
    }

    text_box_release_and_destroy(tb);
    CloseWindow();

    return 0;
}


static const char long_text[4096] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse luctus, diam nec fermentum tincidunt, arcu risus ultricies tellus, nec mollis nibh quam a sapien. Donec pulvinar nibh velit, ut congue felis volutpat sed. In hac habitasse platea dictumst. Praesent eleifend feugiat lacus id sagittis. Duis tempus turpis consequat laoreet maximus. Integer non semper orci. In vel felis libero. Nulla vitae vestibulum neque. Nunc a dui sed lectus sodales hendrerit id eu urna. Etiam volutpat a justo ut eleifend. Quisque ullamcorper magna ac lorem eleifend ullamcorper. Etiam pellentesque diam non euismod pellentesque. In sed ultricies arcu, vulputate dignissim dolor. Vivamus tempus ante at mi semper imperdiet.\n"
"\nPhasellus imperdiet orci molestie augue scelerisque, vel imperdiet velit elementum. Ut bibendum dignissim nunc sit amet posuere. Curabitur tristique turpis id enim eleifend porta. Vestibulum eget sem a magna congue laoreet vel nec quam. Nullam neque metus, blandit eget tellus sollicitudin, pretium pulvinar nulla. Aliquam fermentum tincidunt nunc id tincidunt. Nulla mi mi, placerat et leo a, fermentum dignissim augue. Vivamus eleifend dignissim tempus. Curabitur condimentum tempus metus ut iaculis. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc a urna dignissim, imperdiet ante eget, ullamcorper risus. Curabitur vulputate sed nisi ut volutpat.\n"
"\nProin id pharetra orci, eget fermentum felis. Aenean quis libero sit amet magna euismod tempus. Proin ut consequat turpis, mattis rutrum erat. Etiam placerat metus a diam malesuada dignissim. Vivamus ac vulputate ante, vel pulvinar nulla. Nam et orci nec nisi porttitor luctus eu in lectus. Aenean sit amet eros varius, sodales nisl sit amet, convallis mi. Sed a nulla fringilla, mollis est et, mollis urna. Nunc molestie varius neque, sit amet laoreet justo vehicula eget. Vivamus sagittis pellentesque libero a scelerisque. In a hendrerit nisl.\n"
"\nClass aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Donec ut augue tincidunt, consectetur tortor eget, malesuada metus. Etiam molestie neque sit amet ipsum dictum congue. Fusce vehicula ac lectus et lacinia. Suspendisse lectus sapien, aliquet a facilisis nec, rhoncus porttitor tellus. Ut consectetur a libero in lobortis. Donec sagittis lectus vitae massa iaculis pellentesque. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Maecenas venenatis luctus laoreet. Suspendisse potenti. Ut in nulla risus. Praesent nunc ante, commodo quis lacus eget, facilisis dictum velit. Proin accumsan lorem neque, sit amet malesuada diam sodales a. Sed nec pellentesque diam, eget aliquam turpis. Nulla sapien lectus, maximus ac arcu vel, facilisis volutpat odio.\n"
"\nNam lobortis neque lacus, nec tempus lacus feugiat vel. Pellentesque eget nibh vitae elit vestibulum facilisis aliquet id neque. Duis scelerisque dolor non risus molestie, eu iaculis erat placerat. Suspendisse pulvinar, erat vel porttitor convallis, erat purus lobortis sapien, sed congue nibh ligula eu nibh. Quisque nisl sem, aliquam et odio ac, imperdiet vestibulum eros. Curabitur purus metus, varius ut vehicula at, imperdiet vel ante. Donec laoreet ullamcorper felis, et sollicitudin dolor condimentum pretium. Sed commodo auctor ipsum, eget dictum nibh convallis at. Suspendisse lobortis ex turpis, quis blandit sapien pellentesque vel. Pellentesque vestibulum dignissim sem, sed facilisis tortor cursus aliquet. Cras ut pharetra tortor, vel tincidunt massa. Integer quis nibh at nisl fringilla pulvinar. Maecenas lacinia nisl justo, tincidunt faucibus erat varius in. Nulla vehicula tellus id odio pellentesque sagittis. Proin non lacus molestie, aliquet nibh at, tempus felis. Fusce hendrerit mi non turpis consectetur consectetur.";

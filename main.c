#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "mandelbrot_set.h"

int HEIGHT_AND_WIDTH = 800;
double x_centre = -0.75, y_centre = 0;
double scale = 0.005;

void draw(ALLEGRO_DISPLAY *display)
{
    double disp_x_left = x_centre - HEIGHT_AND_WIDTH / 2.0 * scale;
    double disp_y_up = y_centre + HEIGHT_AND_WIDTH / 2.0 * scale;

    ALLEGRO_LOCKED_REGION *bitmap = al_lock_bitmap(al_get_backbuffer(display), ALLEGRO_PIXEL_FORMAT_RGB_888, ALLEGRO_LOCK_READWRITE);

    // debug
    printf("x_centre: %f, y_centre: %f\n", x_centre, y_centre);
    printf("disp_x_left: %f, disp_y_up: %f\n", disp_x_left, disp_y_up);
    printf("scale: %f\n", scale);

    mandelbrot_set(bitmap->data, bitmap->pitch, HEIGHT_AND_WIDTH, scale, disp_x_left, disp_y_up);

    al_unlock_bitmap(al_get_backbuffer(display));
    al_flip_display();
}

int main()
{
     // initialize allegro and modules
    al_init();
    al_install_mouse();
    al_install_keyboard();

    // creating the display
    ALLEGRO_DISPLAY *display = al_create_display(HEIGHT_AND_WIDTH, HEIGHT_AND_WIDTH);
    al_set_window_title(display, "Mandelbrot Set");

    // creating event queue
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    ALLEGRO_EVENT event;

    int mouse_button = 0;
    bool draw_again = true;

    while (true)
    {
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;  // quit if closed the window

        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && event.type == ALLEGRO_EVENT_KEY_DOWN)
            break;  // quit if pressed escape

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            mouse_button = event.mouse.button;
            //mouse_button = 1 if left mouse button clicked
            //mouse_button = 2 if right mouse button clicked

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            mouse_button = 0;

        // left click to change x_centre and y_centre
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            if (mouse_button == 1)
            {
                x_centre -= event.mouse.dx * scale;
                y_centre += event.mouse.dy * scale;
                draw_again = true;
            }
            // scroll, zoom in - positive, zoom out - negative
            if (event.mouse.dz != 0)
            {
                scale -= scale * event.mouse.dz * 0.1;
                draw_again = true;
            }
        }

        if (draw_again && al_event_queue_is_empty(event_queue))
        {
            draw_again = false;
            draw(display);
        }
    }

    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();

    return 0;
}

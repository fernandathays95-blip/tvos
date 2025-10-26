#include "cursor.hal.h"
#include "hal.h"
#include <cstring>

namespace tvos::hal {

static uint32_t* framebuffer = nullptr;
static int fb_width = 0;
static int fb_height = 0;
static Cursor* current_cursor = nullptr;

// Função interna para desenhar um frame com alpha blending simples
static void draw_frame(const CursorFrame& frame, int x, int y) {
    for (int fy = 0; fy < frame.height; fy++) {
        int py = y + fy;
        if (py < 0 || py >= fb_height) continue;
        for (int fx = 0; fx < frame.width; fx++) {
            int px = x + fx;
            if (px < 0 || px >= fb_width) continue;

            uint32_t pixel = frame.pixels[fy * frame.width + fx];
            uint8_t alpha = (pixel >> 24) & 0xFF;
            if (alpha == 0) continue; // pixel transparente

            // alpha blending simples: mistura com o fundo
            uint32_t bg = framebuffer[py * fb_width + px];
            uint8_t r = ((pixel >> 16 & 0xFF) * alpha + (bg >> 16 & 0xFF) * (255 - alpha)) / 255;
            uint8_t g = ((pixel >> 8 & 0xFF) * alpha + (bg >> 8 & 0xFF) * (255 - alpha)) / 255;
            uint8_t b = ((pixel & 0xFF) * alpha + (bg & 0xFF) * (255 - alpha)) / 255;

            framebuffer[py * fb_width + px] = (r << 16) | (g << 8) | b;
        }
    }
}

void cursor_init() {
    framebuffer = hal::get_framebuffer();
    fb_width = hal::get_framebuffer_width();
    fb_height = hal::get_framebuffer_height();
}

void cursor_show(Cursor* cur) {
    if (!cur) return;
    current_cursor = cur;
    cur->visible = true;
    draw_frame(cur->frames[cur->current_frame], cur->x, cur->y);
}

void cursor_hide(Cursor* cur) {
    if (!cur || !cur->visible) return;
    // redesenha fundo preto (ou cor HAL)
    CursorFrame clear_frame = cur->frames[cur->current_frame];
    for (int fy = 0; fy < clear_frame.height; fy++)
        for (int fx = 0; fx < clear_frame.width; fx++)
            framebuffer[(cur->y + fy) * fb_width + (cur->x + fx)] = hal::get_background_color();
    cur->visible = false;
}

void cursor_move(Cursor* cur, int new_x, int new_y) {
    if (!cur) return;
    if (cur->visible) cursor_hide(cur);
    cur->x = new_x;
    cur->y = new_y;
    if (cur->visible) cursor_show(cur);
}

void cursor_update(Cursor* cur, int frame_delta) {
    if (!cur || !cur->visible) return;
    cursor_hide(cur);
    cur->current_frame = (cur->current_frame + frame_delta) % cur->frame_count;
    cursor_show(cur);
}

} // namespace tvos::hal

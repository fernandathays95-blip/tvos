#ifndef CURSOR_HAL_H
#define CURSOR_HAL_H

#include <stdint.h>

namespace tvos::hal {

struct CursorFrame {
    const uint32_t* pixels; // ponteiro para pixels RGBA
    int width;
    int height;
};

struct Cursor {
    int x;
    int y;
    int current_frame;   // índice do frame atual
    int frame_count;     // número de frames
    const CursorFrame* frames; // array de frames
    bool visible;
};

// Inicializa HAL do cursor
void cursor_init();

// Mostra o cursor
void cursor_show(Cursor* cur);

// Esconde o cursor
void cursor_hide(Cursor* cur);

// Move o cursor para nova posição
void cursor_move(Cursor* cur, int new_x, int new_y);

// Atualiza o cursor (para animação)
void cursor_update(Cursor* cur, int frame_delta=1);

} // namespace tvos::hal

#endif

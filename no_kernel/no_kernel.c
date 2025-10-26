#include "hal.h"        // HAL genérico do TVOS
#include "cursor.hal.h" // Para desenhar o quadrado
#include <stdint.h>

// ===================================================================
// AVISO: Este arquivo roda quando não há kernel instalado.
// Apenas exibe mensagem de alerta e teste visual.
// ===================================================================

// Funções HAL usadas
extern void hal_console_write(const char* msg); // escreve na tela
extern uint32_t* hal_get_framebuffer();         // ponteiro para framebuffer
extern int hal_get_framebuffer_width();
extern int hal_get_framebuffer_height();
extern void hal_sleep_ms(int ms);              // pausa para animação

// Função principal do "no_kernel"
void no_kernel_main() {

    // ================================================================
    // Mensagem de alerta
    // ================================================================
    hal_console_write("===========================================\n");
    hal_console_write("ATENÇÃO: sua TV não tem base (kernel).\n");
    hal_console_write("Você pode instalar um kernel como o Linux.\n");
    hal_console_write("===========================================\n");

    // ================================================================
    // Variáveis do quadrado
    // ================================================================
    uint32_t* fb = hal_get_framebuffer();
    int width = hal_get_framebuffer_width();
    int height = hal_get_framebuffer_height();

    int sq_size = 50;                  // tamanho do quadrado
    int x = width / 2 - sq_size / 2;   // posição inicial X
    int y = height / 2 - sq_size / 2;  // posição inicial Y

    int dx = 2;                        // velocidade horizontal simulada
    int dy = 2;                        // velocidade vertical simulada
    int color_on = 0xFF0000;           // vermelho
    int color_off = 0x000000;          // fundo preto
    int visible = 1;                   // estado do quadrado (pisca)

    // ================================================================
    // Loop principal
    // ================================================================
    while (1) {

        // Apaga quadrado anterior (se visível)
        uint32_t color = visible ? color_on : color_off;
        for (int yy = 0; yy < sq_size; yy++) {
            for (int xx = 0; xx < sq_size; xx++) {
                int px = x + xx;
                int py = y + yy;
                if (px >= 0 && px < width && py >= 0 && py < height) {
                    fb[py * width + px] = color;
                }
            }
        }

        // Alterna estado para piscar
        visible = !visible;

        // Simula "problema detectado" movendo quadrado
        x += dx;
        y += dy;

        // Rebote nas bordas da tela
        if (x < 0 || x + sq_size > width) dx = -dx;
        if (y < 0 || y + sq_size > height) dy = -dy;

        // Pausa para animação (~100ms)
        hal_sleep_ms(100);

        // =================================================================
        // Comentários: aqui você poderia adicionar checagem de hardware,
        // detecção de memória, input do usuário, etc.
        // Tudo comentado, seguro para TV sem kernel.
        // =================================================================
    }
}

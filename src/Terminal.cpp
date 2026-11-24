#include "Terminal.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

bool Terminal::ativo = true;

void Terminal::setAtivo(bool valor) { ativo = valor; }
bool Terminal::isAtivo() { return ativo; }

void Terminal::limparTela(std::ostream& saida) {
    if (!ativo) return;
#ifdef _WIN32
    // Console do Windows: liga o processamento de sequencias ANSI (ignorado se nao der).
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &modo)) {
        SetConsoleMode(h, modo | 0x0004);   // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    }
#endif
    saida << "\033[2J\033[3J\033[H" << std::flush;   // apaga a tela e o historico, cursor no topo
}

bool Terminal::pausar(std::istream& entrada, std::ostream& saida) {
    if (!ativo) return true;
    saida << "\nPressione ENTER para continuar. . ." << std::flush;
    std::string descarte;
    return static_cast<bool>(std::getline(entrada, descarte));
}

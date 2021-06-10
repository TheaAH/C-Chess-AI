
#ifndef GAME_MODES_HEADER
#define GAME_MODES_HEADER

#include <iostream>
#include <vector>
#include <random>

#include "chess.h"
#include "chess_board.h"

namespace game_mode
{
 
void human_vs_human();

void random();

void vs_ai();

std::string name_savefile();

void menu();

} // namespace game_mode

#endif
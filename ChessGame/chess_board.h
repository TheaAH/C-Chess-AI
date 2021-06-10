
#ifndef CHESS_BOARD_HEADER
#define CHESS_BOARD_HEADER

#include <iostream>
#include <vector>
#include <memory>
#include <iosfwd>
#include <tuple>

#include "chess.h"
#include "ai.h"

namespace chess
{

  class board
  {
  private:
    int white_score{};
    int black_score{};
    std::vector<bool> en_passant;
    std::array<square, 64> squares;

  public:

    ~board() {}
    board();

    std::vector<bool> colour_bitboard(char color);

    void print_board();

    void make_move(move input_move);

    std::vector<move> generate_moves(char colour_to_play);

    int evaluate(char evaluate_for_color);

    move generate_random_move(char color_to_play);

    board& operator=(const board &copied_board);
    board(const board &copied_board);

    std::vector<bool> occupied_bitboard();

    std::vector<bool> unmoved_bitboard();

    move prepare_move(char color_to_play);

    bool handle_castling(char color_to_play, move requested_move);

    bool check(char color_to_play);
    bool check_mate(char color_to_play);

    move ask_for_move(char color_to_play);

    void promote_pawn(char color_to_play, move requested_move);

    void handle_en_passant(std::vector<bool> en_passant, move requested_move);

    void handle_pawns(char color_to_play, move requested_move);

    bool puts_in_check(char color_to_play, move requested_move);

    std::pair<move, int> minimax(char color_to_maximize, int depth, int alpha, int beta);

    void test_func();

  };  


}
  
#endif 

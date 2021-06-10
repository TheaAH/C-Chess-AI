
#ifndef CHESS_PIECE_HEADER
#define CHESS_PIECE_HEADER

#include <iostream>
#include <vector>
#include <memory>
#include <iosfwd>

//#include "chess_board.h"

namespace chess
{

  std::string answer_to_lowercase(std::string &answer);
  bool ask_to_play();
  char color_of_opponent(char color_to_play);
  char convert_to_char(int row);
  void print_bitboard(std::vector<bool> bitboard);

class coordinates // A coordinate indicates a square on the board
{
private:
  char row{'a'};
  int column{0};

public:
  ~coordinates() {}
  coordinates() = default;
  coordinates(char entered_row, int entered_column) : row{entered_row}, column{entered_column} {}

  friend class board;
  friend class move;

  friend std::ostream& operator<<(std::ostream &output, const coordinates &input_coordinates);

  friend std::istream& operator>>(std::istream &input, coordinates &input_coordinates);

  int convert_to_int();
  
  int row_int();
  int column_int();

};

std::ostream& operator<<(std::ostream &output, const coordinates &input_coordinates);

std::istream& operator>>(std::istream &input, coordinates &input_coordinates);


class move // A move is a pair of coordinates.
{
private:

  coordinates start{'a', 0};
  coordinates end{'a', 0};

public:

  ~move() {}
  move() = default;
  move(const move &copied_move);
  move(coordinates starting, coordinates ending) : start{starting}, end{ending} {}
  move(char starting_row, int starting_column, char ending_row, int ending_column) : start{starting_row, starting_column}, end{ending_row, ending_column} {}

  friend class board;

  int convert_start_to_int();

  int convert_end_to_int();

  friend std::ostream& operator<<(std::ostream &output, const move &input_coordinates);

  friend std::istream& operator>>(std::istream &input, move &input_coordinates);

  void write_to_file(std::string file_name);

};


std::ostream& operator<<(std::ostream &output, const move &input_coordinates);

std::istream& operator>>(std::istream &input, move &input_coordinates);



class chess_piece
  {
  private:
    char represented{' '};
    const int value{};
  protected:
    bool first_move{true};
    const char color{};
    bool en_passant_vulnerable{false};
  public:

    virtual ~chess_piece() {}    
    chess_piece(char starting_color, int strategic_value) : color{starting_color}, value{strategic_value} {}

    friend class square;
    friend class board;

    virtual char get_representation() = 0;
    char get_color();

    virtual std::unique_ptr<chess_piece> clone() = 0;

    virtual std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard) = 0;

  };

  class pawn : public chess_piece
  {
  private:
    const char represented{'p'};

  public:
    pawn(char starting_color) : chess_piece{starting_color, 1} { };

    ~pawn() {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();
    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);
  };

  class knight : public chess_piece
  {
  private:
    char represented{'k'};
  public:
    knight(char starting_color) : chess_piece{starting_color, 3} {}
    ~knight() {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();
    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);
  };



  class bishop : public chess_piece
  {
  private:
    char represented{'b'};
  public:
    ~bishop() {}
    bishop(char starting_color) : chess_piece{starting_color, 3} {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();  
    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);  
  };

  class rook : public chess_piece
  {
  private:
    char represented{'r'};
  public:
    rook(char starting_color) : chess_piece{starting_color, 5} {}
    ~rook() {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();
    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);
  };

  class queen : public chess_piece
  {
  private:
    char represented{'Q'};  
  public:
    queen(char starting_color) : chess_piece{starting_color, 9} {}
    ~queen() {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();

    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);
  };


  class king : public chess_piece
  {
  private:
    char represented{'K'};
  public:
    king(char starting_color) : chess_piece{starting_color, 100} { }
    ~king() {}

    std::unique_ptr<chess_piece> clone();

    friend class board;
    char get_representation();

    std::vector<bool> valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard);

  };


  class square // a square can hold a piece on the board. It is an interface to stop us trying to access a pointer
  // to a piece that doesn't exist
  {
  private:
    bool inhabited{false};    
    std::unique_ptr<chess_piece> inhabitant{nullptr};
    bool en_passant{false};

  public:

    friend class board;
    friend std::ostream& operator<<(std::ostream &output, const square &inserted_piece);


    ~square() { }
    square() = default;
    square(std::unique_ptr<chess_piece> placed_piece);
    
    square& operator=(square&& copied_square);

    void empty();
    char get_representation();
    bool const get_inhabited();

  };

  std::ostream& operator<<(std::ostream &output, const square &inserted_piece);

};

//using namespace chess;

#endif
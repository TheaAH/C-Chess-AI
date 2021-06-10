
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <vector>
#include <typeinfo>
#include <memory>
#include <iosfwd>
#include <random>
#include <ctime>
#include <exception>

#include "chess_board.h"

using namespace chess; // Merged namespace


board::board()
{
  en_passant = std::vector<bool>(64, false);

  for (int row{}; row<8; row++){
    for (int column{2}; column<6; column++){
      squares[row*8 + column] = square{}; // Middle squares start out empty
    }
    squares[row*8 + 1] = square{std::make_unique<pawn>('w')}; // two rows of pawns closest to the centre
    squares[row*8 + 6] = square{std::make_unique<pawn>('b')};
  }

  squares[0*8 + 0] = square{std::make_unique<rook>('w')}; // other pieces at the edges.
  squares[1*8 + 0] = square{std::make_unique<knight>('w')};
  squares[2*8 + 0] = square{std::make_unique<bishop>('w')};
  squares[3*8 + 0] = square{std::make_unique<queen>('w')};
  squares[4*8 + 0] = square{std::make_unique<king>('w')};
  squares[5*8 + 0] = square{std::make_unique<bishop>('w')};
  squares[6*8 + 0] = square{std::make_unique<knight>('w')};
  squares[7*8 + 0] = square{std::make_unique<rook>('w')};

  squares[0*8 + 7] = square{std::make_unique<rook>('b')};
  squares[1*8 + 7] = square{std::make_unique<knight>('b')};
  squares[2*8 + 7] = square{std::make_unique<bishop>('b')};
  squares[3*8 + 7] = square{std::make_unique<queen>('b')};
  squares[4*8 + 7] = square{std::make_unique<king>('b')};
  squares[5*8 + 7] = square{std::make_unique<bishop>('b')};
  squares[6*8 + 7] = square{std::make_unique<knight>('b')};
  squares[7*8 + 7] = square{std::make_unique<rook>('b')};
}


void board::print_board()
{
  char grid_letters[8]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::cout<<"     1    2    3    4    5    6    7    8"<<std::endl; // number edges
  for (int row{}; row<8; row++){
    std::cout<<"   ";
    for (int column{}; column<8; column++){
      std::cout<<"-----"; // make sides
    }
    std::cout<<std::endl<<grid_letters[row]<<"  "; // a letter at the beginning & end of each row
    for (int column{}; column<8; column++){
      std::cout<<"| "<<squares[row*8 + column]<<" "; // Output inhabitant symbol
    }
    std::cout<<"|  "<<grid_letters[row]<<std::endl;
  }
  std::cout<<"   ";
  for (int column{}; column<8; column++){
    std::cout<<"-----";
  }
  std::cout<<std::endl;
  std::cout<<"     1    2    3    4    5    6    7    8"<<std::endl;
}


void board::make_move(move input_move) // overload to take a move
{
  std::vector<bool> inhabited = occupied_bitboard(); // Find occupied squares
  
  if (inhabited[input_move.convert_end_to_int()]){ // If we are taking a piece
    if (squares[input_move.convert_end_to_int()].inhabitant->color == 'w'){ // add to the opposite colours score on the board
      black_score += squares[input_move.convert_end_to_int()].inhabitant->value;
    } else if (squares[input_move.convert_end_to_int()].inhabitant->color == 'b'){
      white_score += squares[input_move.convert_end_to_int()].inhabitant->value;
    }
  }

  squares[input_move.convert_end_to_int()].inhabitant.swap(squares[input_move.convert_start_to_int()].inhabitant); //Swap pointers

  squares[input_move.convert_end_to_int()].inhabited = true; // Flag the squares appropriately
  squares[input_move.convert_start_to_int()].inhabited = false;

  squares[input_move.convert_end_to_int()].inhabitant->first_move = false; // It's not this piece's first move anymore
  squares[input_move.convert_start_to_int()].empty(); // delete whatever's in the old square
}

board::board(const board &copied_board)
{
  white_score = 0;
  black_score = 0;
  
  en_passant = std::vector<bool>(64, false);

  for (int space{}; space<64; space++){
    en_passant[space] = copied_board.en_passant[space];
  }

  white_score = copied_board.white_score;
  black_score = copied_board.black_score;

  //squares = new square[64];

  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (copied_board.squares[row*8 + column].inhabited){
        squares[row*8 + column] = square{copied_board.squares[row*8 + column].inhabitant->clone()};
      } else {
        squares[row*8 + column] = square{};
      }
    }
  }
}

board& board::operator=(const board &copied_board) // deep copies the board
{
  if (&copied_board == this){
    return *this;
  }
  
  en_passant = std::vector<bool>(64, false);

  for (int space{}; space<64; space++){
    en_passant[space] = copied_board.en_passant[space];
  }

  white_score = copied_board.white_score;
  black_score = copied_board.black_score;

  //squares = new square[64];

  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (copied_board.squares[row*8 + column].inhabited){
        squares[row*8 + column] = square{copied_board.squares[row*8 + column].inhabitant->clone()};
      } else {
        squares[row*8 + column] = square{};
      }
    }
  }
  return *this;
}

move board::generate_random_move(char color_to_play)
{
  std::vector<move> possible_moves = generate_moves(color_to_play);

  std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
  int number_moves_possible = possible_moves.size();
  std::uniform_int_distribution<int> distribution(0, number_moves_possible-1);
  int selected_index = distribution(mersenne); 
  move selected_move = possible_moves[selected_index];
  return selected_move;
}


std::vector<bool> board::colour_bitboard(char color) //find squares belonging to pieces of a certain colour.
{
  std::vector<bool> colour_bitboard;
  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (squares[row*8 + column].inhabited == false){
        colour_bitboard.push_back(false);
        //continue;
      } else if (squares[row*8 + column].inhabitant->color == color){
        colour_bitboard.push_back(true);
      } else {
        colour_bitboard.push_back(false);
      }
    }
  }
  return colour_bitboard;    
}

std::vector<bool> board::occupied_bitboard() // Find squares that are occupied
{
  std::vector<bool> occupation_bitboard;
  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      occupation_bitboard.push_back(squares[row*8 + column].inhabited);
    }
  }
  return occupation_bitboard;
}

std::vector<bool> board::unmoved_bitboard() // find pieces that havne't been moved yet
{
  std::vector<bool> occupation_bitboard;
  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (squares[row*8 + column].inhabited){
        occupation_bitboard.push_back(squares[row*8 + column].inhabitant->first_move);
      } else {
        occupation_bitboard.push_back(false);
      }
    }
  }
  return occupation_bitboard;
}


bool board::check(char color_to_play)
{
  char opponent_color = color_of_opponent(color_to_play);

  int king_row;
  int king_column;

  // First we find the king
  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (squares[row*8 + column].inhabited){
        if (squares[row*8 + column].inhabitant->color == color_to_play && squares[row*8 + column].inhabitant->get_representation() == 'K'){
          king_row = row;
          king_column = column;
        }
      }
    }
  }

  std::vector<bool> friends_bitboard = colour_bitboard(color_to_play); // because opposite color is moving
  std::vector<bool> foes_bitboard = colour_bitboard(opponent_color);

  for (int row{}; row<8; row++){
    for (int column{}; column<8; column++){
      if (squares[row*8 + column].inhabited && foes_bitboard[row*8 + column]){ // look at moves made by opponent
        std::vector<bool> moves = squares[row*8 + column].inhabitant->valid_move(row, column, friends_bitboard, foes_bitboard);
        if (moves[king_row*8 + king_column]){ // If one of the moves intersects with the king, then the player is in check.
          return true;
        }
      }
    }
  } // If we exhaust all moves and none intersect with the king, we are safe.
  return false;
}


bool board::check_mate(char color_to_play)
{
  char opponent_color = color_of_opponent(color_to_play);

  std::vector<bool> friends_bitboard = colour_bitboard(color_to_play);
  std::vector<bool> foes_bitboard = colour_bitboard(opponent_color);

  for (int from_row{}; from_row<8; from_row++){
    for (int from_column{}; from_column<8; from_column++){
      if (squares[from_row*8 + from_column].inhabited && friends_bitboard[from_row*8 + from_column]){

        std::vector<bool> moves = squares[from_row*8 + from_column].inhabitant->valid_move(from_row, from_column, foes_bitboard, friends_bitboard);

        for (int to_row{}; to_row<8; to_row++){
          for (int to_column{}; to_column<8; to_column++){
            if (moves[to_row*8 + to_column]){ // Up to this point, same as the find all moves function
              board temp_board{}; // Make a temporary board
              temp_board = *this;

              char start_row_char = convert_to_char(from_row);
              char end_row_char = convert_to_char(to_row);

              move possible_move{start_row_char, from_column+1, end_row_char, to_column+1};
              temp_board.make_move(possible_move); // make move on the temporary board

              if (!temp_board.check(color_to_play)){
                return false; // check status of temporary board
              }
            }
          }
        }
      }
    }
  }
  return true;
}

move board::ask_for_move(char color_to_play)
{
  move requested_move;
  std::cout<<"Which piece to move, and where to? write this in standard chess notation (a3 b2): ";
  std::cin;
  std::cin>>requested_move;


  while (std::cin.fail() || (requested_move.start.row == requested_move.end.row && requested_move.start.column == requested_move.end.column ) || !squares[requested_move.convert_start_to_int()].inhabited || squares[requested_move.convert_start_to_int()].inhabitant->color != color_to_play){
    if (requested_move.start.row == requested_move.end.row && requested_move.start.column == requested_move.end.column ){
      std::cout<<"You can't stay in one spot ";
    } else if (!squares[requested_move.convert_start_to_int()].inhabited){
      std::cout<<"There's no piece here. "<<std::endl;
    } else if (squares[requested_move.convert_start_to_int()].inhabitant->color != color_to_play){
      std::cout<<"You're trying to play the wrong color. "<<std::endl;
    }
    std::cout<<"Try again: "<<std::endl;      
    std::cin.clear ();
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin;
    std::cin>>requested_move;
  }
  return requested_move;
}

bool board::handle_castling(char color_to_play, move requested_move)
{
  std::vector<bool> unmoved_pieces = unmoved_bitboard();
  std::vector<bool> occupied_squares = occupied_bitboard();

  if (requested_move.start.row == 'e' && (requested_move.start.column == 1 ||requested_move.start.column == 8) && occupied_squares[requested_move.start.row_int()*8 + requested_move.start.column_int()] && std::abs(requested_move.start.row_int() - requested_move.end.row_int()) == 2 && requested_move.start.column_int() == requested_move.end.column_int()){

  //if (squares[requested_move.start.row_int()*8 + requested_move.start.column_int()].get_representation() == 'K' && squares[requested_move.start.row_int()*8 + requested_move.start.column_int()].inhabitant->first_move && std::abs(requested_move.start.row_int() - requested_move.end.row_int()) == 2 && requested_move.start.column_int() == requested_move.end.column_int()){
    // If the piece we want to move is a king, AND it's the kings first move, AND the king is moving two spaces either way vertically, but not moving horizontally
    // Then that means that the king is eligible to castle.

    if (requested_move.end.row_int() == 6 && unmoved_pieces[requested_move.start.column_int() + 8 *7] && !occupied_squares[requested_move.start.column_int() + 8 *6] && !occupied_squares[requested_move.start.column_int() + 8 *5]){
      // If we want to move the king DOWN, then we need the last piece to be unmoved (if it's unmoved it MUST be a rook) and the squares between to be free.

      board temp;
      temp = *this; // copy the board so we can make the move and then check the status of the result.

      temp.print_board();
      temp.make_move(requested_move); // move king
      move rook_move{'h', requested_move.start.column, 'f', requested_move.start.column};
      temp.make_move(rook_move); // move rook
      temp.print_board();
      if (temp.check(color_to_play)){ //check status on fake board
        std::cout<<"This move puts you in check!!!! Try again "<<std::endl;
        return false;
      }
      make_move(requested_move);
      make_move(rook_move);
      return true;
    }

    if (requested_move.end.row_int() == 2 && unmoved_pieces[requested_move.start.column_int() + 8 *0] && !occupied_squares[requested_move.start.column_int() + 8 *1] && !occupied_squares[requested_move.start.column_int() + 8 *2] && !occupied_squares[requested_move.start.column_int() + 8 *3]){

      board temp;
      temp = *this;
      move rook_move{'a', requested_move.start.column, 'd', requested_move.start.column};
      temp.make_move(requested_move);
      temp.make_move(rook_move);

      if (temp.check(color_to_play)){
        std::cout<<"This move puts you in check!!!! Try again "<<std::endl;
        return false;
      }
      
      make_move(requested_move);
      make_move(rook_move);
      return true;
    }
  }
  return false;
}

void board::promote_pawn(char color_to_play, move requested_move)
{
  std::string promoted;

  std::cout<<"What piece do you want to promote this pawn to? Please type the full name in lowercase letters (eg. 'queen', 'rook', 'bishop', 'knight') "<<std::endl;
  std::cin;
  std::cin>>promoted;

  if (std::cin.fail()){
    promoted = answer_to_lowercase(promoted);
  }

  while (std::cin.fail() || !(promoted == "rook" || promoted == "queen" || promoted == "bishop" || promoted == "knight")){
    if (std::cin.fail()){
      std::cout<<"You must type in a word. Please try again: ";
    } else if (!(promoted == "rook" || promoted == "queen" || promoted == "bishop" || promoted == "knight")){
      std::cout<<"That's not a chess piece that a pawn can turn into. Try again: ";
    }
    std::cin.clear ();
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin;
    std::cin>>promoted;
  }

  if (promoted == "rook"){ // change pointer for the relevent square depending on the requested type
    squares[requested_move.convert_start_to_int()].inhabitant = std::make_unique<rook> (color_to_play);
  } else if (promoted == "queen"){
    squares[requested_move.convert_start_to_int()].inhabitant = std::make_unique<queen> (color_to_play);
  } else if (promoted == "bishop"){
    squares[requested_move.convert_start_to_int()].inhabitant = std::make_unique<bishop> (color_to_play);
  } else if (promoted == "knight"){
    squares[requested_move.convert_start_to_int()].inhabitant = std::make_unique<knight> (color_to_play);
  }
}

void board::handle_en_passant(std::vector<bool> en_passant, move requested_move)
{
  if (en_passant[requested_move.convert_end_to_int()] && squares[requested_move.start.column_int() + 2 + 8*requested_move.start.row_int()].inhabitant->en_passant_vulnerable){
    squares[requested_move.start.column_int() + 2 + 8*requested_move.start.row_int()].empty();
  }

  if (squares[requested_move.convert_start_to_int()].get_representation() == 'p'){
    //squares[requested_move.convert_start_to_int()].inhabitant->manage_en_passant(requested_move); 
    if (std::abs(requested_move.start.column_int() - requested_move.end.column_int()) ==2){
      en_passant[requested_move.start.column_int() + 1 + 8*requested_move.start.row_int()] = true;
    }
  }
}

void board::handle_pawns(char color_to_play, move requested_move)
{
  if (squares[requested_move.convert_start_to_int()].get_representation() == 'p'){
    //handle_en_passant(en_passant, requested_move);
    if (requested_move.end.column_int() == 0 || requested_move.end.column_int() == 7){
      promote_pawn(color_to_play, requested_move);
    }
  }
}

bool board::puts_in_check(char color_to_play, move requested_move)
{
  board temp{};
  temp = *this;
  temp.make_move(requested_move);
  if (temp.check(color_to_play)){
    std::cout<<"This move puts you in check!!!! Try again "<<std::endl;
    return true;
  }
  return false;
}

move board::prepare_move(char color_to_play)
{
  char opponent_color = color_of_opponent(color_to_play);
  bool move_made{false};
  move requested_move{};

  while(!move_made){
    requested_move = ask_for_move(color_to_play);

    std::vector<bool> friends_bitboard = colour_bitboard(color_to_play);
    std::vector<bool> foes_bitboard = colour_bitboard(opponent_color);
    // Acceptable moves for the selected piece
    std::vector<bool> acceptable_moves = squares[requested_move.convert_start_to_int()].inhabitant->valid_move(requested_move.start.row_int(), requested_move.start.column_int(), foes_bitboard, friends_bitboard);

    bool castled = handle_castling(color_to_play, requested_move);
    if (castled){
      move_made = true;
      break;
    }

    if (acceptable_moves[requested_move.convert_end_to_int()]){

      handle_pawns(color_to_play, requested_move);

      make_move(requested_move);
      std::cout<<"Move accepted"<<std::endl;
      move_made = true;
      break; // redundant, I know

    } else {
      std::cout<<"Not an acceptable move for this piece"<<std::endl;
    }
  }
  return requested_move;
}

std::vector<move> board::generate_moves(char colour_to_play)
{
  char opponent_color = color_of_opponent(colour_to_play);
  std::vector<bool> friends_bitboard = colour_bitboard(colour_to_play);
  std::vector<bool> foes_bitboard = colour_bitboard(opponent_color);

  std::vector<move> possible_moves;

  for (int from_row{}; from_row<8; from_row++){
    for (int from_column{}; from_column<8; from_column++){ // All possible starting positions
      if (squares[from_row*8 + from_column].inhabited && friends_bitboard[from_row*8 + from_column]){ // if they contain a playable piece
        std::vector<bool> moves = squares[from_row*8 + from_column].inhabitant->valid_move(from_row, from_column, foes_bitboard, friends_bitboard);
        for (int to_row{}; to_row<8; to_row++){
          for (int to_column{}; to_column<8; to_column++){ // all possible end positions
            if (moves[to_row*8 + to_column]){ // If these end positions are accessible from the considered start position
              char start_row_char = convert_to_char(from_row);
              char end_row_char = convert_to_char(to_row);
              move possible_move{start_row_char, from_column+1, end_row_char, to_column+1};
              possible_moves.push_back(possible_move); // Add the move to the list of possible moves.
            }
          }
        }
      }
    }
  } 
  return possible_moves;
}

int board::evaluate(char evaluate_for_color) // Give the score for the required player
{
  if (evaluate_for_color == 'w'){
    return white_score - black_score;
  } else if (evaluate_for_color == 'b'){
    return black_score - white_score;
  }
  throw std::invalid_argument("Trying to evaluate the score of a color that doesn't exist ");
}

std::pair<move, int> board::minimax(char color_to_maximize, int depth, int alpha, int beta)
{
  char opponent_color = color_of_opponent(color_to_maximize);

  if (depth == 0){ // at the deepest point, we just want to know the status of the board; don't care about move.
    int current_score = evaluate(color_to_maximize);
    move throw_away{'a', 1, 'a', 1};
    std::pair<move, int> pass_on(throw_away, current_score);
    return pass_on;
  }

  std::vector<move> possible_moves = generate_moves(color_to_maximize);

  // Generate a random "best move" to begin with
  std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
  int number_moves_possible = possible_moves.size();
  std::uniform_int_distribution<int> distribution(0, number_moves_possible-1);
  int selected_index = distribution(mersenne); 
  move throw_away = possible_moves[selected_index];

  std::pair<move, int> best_move(throw_away, -std::numeric_limits<int>::max());

  for (auto possible_move = possible_moves.begin(); possible_move < possible_moves.end(); possible_move++){ // Iterate through our
    // generated moves.
    board temp_board{}; // Create a temporary board to test this move.
    temp_board = *this;

    temp_board.make_move(*possible_move);

    std::pair<move, int> move_result = temp_board.minimax(opponent_color, depth-1, -beta, -alpha); // Next turn is the other player;
    // We want to evaluate what's best for them (ie, the move they will pick). This is the exact opposite of what is good for us.

    move_result.first = (*possible_move); // we want the move that was just made to be passed back to be used, not the board before that.
    move_result.second = -move_result.second; // What is good for the other player is bad for me.
    if(move_result.second > best_move.second){
      best_move = move_result;
    }

    alpha = (alpha > move_result.second) ? alpha : move_result.second;

    if (beta <= alpha){ // If we won't take this branch anyway because it
      break;
    }

  }
  return best_move;
}





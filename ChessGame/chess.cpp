
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <vector>
#include <typeinfo>
#include <memory>
#include <iosfwd>
#include <fstream>
#include <algorithm>
#include <random>
#include <exception>

#include "chess.h"

using namespace chess;



std::string chess::answer_to_lowercase(std::string &answer) // to convert input to lowercase so it is always accepted
{
  std::for_each(answer.begin(), answer.end(), [](char &char_in_array) {
      char_in_array = ::tolower(char_in_array);
  });
  return answer;
}

bool ask_to_play() // not used in the game, in the end
{
  //char answer{};
  std::string answer{};
  std::cout<<"Do you want to play a game of chess? (please type 'yes' or 'no' in lowercase) "<<std::endl;
  std::cin;
  std::cin>>answer;

  if (!std::cin.fail()){
    answer = answer_to_lowercase(answer);
  }

  while (std::cin.fail() || (answer != "yes" && answer != "no")){
    std::cin.clear ();
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');        
    std::cout<<"Try again ! Do you want to play a game of chess? "<<std::endl;
    std::cin;
    std::cin>>answer;
  }
  if (answer == "yes"){
    return true;
  } else if (answer == "no"){
    return false;
  }
  throw std::invalid_argument("Reply is accepted at verification but not at triage ");
}

char chess::color_of_opponent(char color_to_play)
{
  if (color_to_play == 'w'){
    return 'b';
  } else if (color_to_play == 'b'){
    return 'w';
  }
  throw std::invalid_argument("Trying to find opponent color when player is neither white nor black ");
}

void chess::print_bitboard(std::vector<bool> bitboard) // For testing purposes
{
  char grid_letters[8]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::cout<<"     1   2   3   4   5   6   7   8"<<std::endl;
  for (int row{}; row<8; row++){
    std::cout<<"   ";
    for (int column{}; column<8; column++){
      std::cout<<"----";
    }
    std::cout<<std::endl<<grid_letters[row]<<"  ";
    for (int column{}; column<8; column++){
      if (bitboard[column + 8*row]){
        std::cout<<"| t ";
      } else {
        std::cout<<"| f ";
      }
    }
    std::cout<<"|  "<<grid_letters[row]<<std::endl;
  }
  std::cout<<"   ";
  for (int column{}; column<8; column++){
    std::cout<<"----";
  }
  std::cout<<std::endl;
  std::cout<<"     1   2   3   4   5   6   7   8"<<std::endl;

}

int coordinates::convert_to_int() //convert a coordinate into its corresponding 1d index
{
  if (row == 'a'){
    return 0 *8 +column -1;
  } else if (row == 'b'){
    return 1*8 + column -1;
  } else if (row == 'c'){
    return 2*8 + column -1;
  } else if (row == 'd'){
    return 3*8 + column -1;
  } else if (row == 'e'){
    return 4*8 + column -1;
  } else if (row == 'f'){
    return 5*8 + column -1;
  } else if (row == 'g'){
    return 6*8 + column -1;
  } else if (row == 'h'){
    return 7*8 + column -1;
  } else {
    throw std::out_of_range("Trying to convert a coordinate that isn't on the board");
  }
}

char chess::convert_to_char(int row)
{
  if (row == 0){
    return 'a';
  } else if (row == 1){
    return 'b';
  } else if (row == 2){
    return 'c';
  } else if (row == 3){
    return 'd';
  } else if (row == 4){
    return 'e';
  } else if (row == 5){
    return 'f';
  } else if (row == 6){
    return 'g';
  } else if (row == 7){
    return 'h';
  } else {
    throw std::out_of_range("Trying to convert an int that isn't on the board to char ");
  }
}

int coordinates::row_int()
{
  if (row == 'a'){
    return 0;
  } else if (row == 'b'){
    return 1;
  } else if (row == 'c'){
    return 2;
  } else if (row == 'd'){
    return 3;
  } else if (row == 'e'){
    return 4;
  } else if (row == 'f'){
    return 5;
  } else if (row == 'g'){
    return 6;
  } else if (row == 'h'){
    return 7;
  } else {
    throw std::out_of_range("Trying to convert a char that isn't on the board to int ");
  }
}

int coordinates::column_int()
{
  return column - 1;
}

std::ostream& chess::operator<<(std::ostream &output, const coordinates &output_coordinates)
{
  output<<output_coordinates.row;
  output<<output_coordinates.column;
  return output;
}

std::istream& chess::operator>>(std::istream &input, coordinates &input_coordinates)
{
  input>>input_coordinates.row;
  if (input_coordinates.row != 'a' && input_coordinates.row != 'b' && input_coordinates.row != 'c' && input_coordinates.row != 'd' && input_coordinates.row != 'e' && input_coordinates.row != 'f' && input_coordinates.row != 'g' && input_coordinates.row != 'h'){
    input.setstate(std::ios::failbit);
  }
  input>>input_coordinates.column;
  if (input_coordinates.column<1 || input_coordinates.column>8){
    input.setstate(std::ios::failbit);
  }
  return input;
} 

move::move(const move &copied_move)
{
  start.row = copied_move.start.row;
  start.column = copied_move.start.column;
  end.row = copied_move.end.row;
  end.column = copied_move.end.column;
}

int move::convert_start_to_int()
{
  return start.convert_to_int();
}

int move::convert_end_to_int()
{
  return end.convert_to_int();
}

void move::write_to_file(std::string file_name)
{
  std::ofstream moves_file(file_name, std::ios_base::app);
  moves_file<<*this<<std::endl;
  moves_file.close();
}

std::ostream& chess::operator<<(std::ostream &output, const move &output_move)
{
  output<<output_move.start;
  output<<" ";
  output<<output_move.end;
  return output;
}

std::istream& chess::operator>>(std::istream &input, move &input_move)
{
  input>>input_move.start;
  input>>input_move.end;
  return input;
}


char king::get_representation()
{
  return represented;
}

std::vector<bool> king::valid_move(int start_row, int start_column, std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  std::vector<bool> valid_moves(64, false);
  for (int row_move{-1}; row_move<2; row_move++){
    int temp_row = start_row + row_move;
    for (int column_move{-1}; column_move<2; column_move++){
      int temp_column = start_column + column_move;
      if (temp_row<8 && temp_row>-1 && temp_column<8 && temp_column>-1 && !friend_bitboard[temp_column + 8* temp_row]){
        valid_moves[temp_column + 8* temp_row] = true;
      }
    }
  }
  return valid_moves;
}

std::unique_ptr<chess_piece> king::clone()
{
  return std::make_unique<king>(*this);
}

char queen::get_representation()
{
  return represented;
}

std::unique_ptr<chess_piece> queen::clone()
{
  return std::make_unique<queen>(*this);
}

std::vector<bool> queen::valid_move(int start_row, int start_column,  std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  std::vector<bool> valid_moves(64, false);

  for (int north{1}; north<8; north++){
    if ((start_row - north) < 0 || (start_row - north) > 7 || friend_bitboard[start_column + 8 * (start_row - north)]){
      break;
    } else if (foe_bitboard[start_column + 8 * (start_row - north)]){
      valid_moves[start_column + 8 * (start_row - north)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[start_column + 8 * (start_row - north)] = true;
    }
  }
  for (int south{1}; south<8; south++){
    if ((start_row + south) < 0 || (start_row + south) > 7 || friend_bitboard[start_column + 8 * (start_row + south)]){
      break;
    } else if (foe_bitboard[start_column + 8 * (start_row + south)]){
      valid_moves[start_column + 8 * (start_row + south)] = true;
      break;
    } else {
      valid_moves[start_column + 8 * (start_row + south)] = true;
    }
  }
  for (int east{1}; east<8; east++){
    if ((start_column + east) < 0 || (start_column + east) > 7 || friend_bitboard[start_column + east + 8 * start_row] ){
      break;
    } else if (foe_bitboard[start_column + east + 8 * start_row]){
      valid_moves[start_column + east + 8 * start_row] = true; 
      break;
    } else {
      valid_moves[start_column + east + 8 * start_row] = true;
    }
  }
  for (int west{1}; west<8; west++){
    if ((start_column - west) < 0 || (start_column - west) > 7 || friend_bitboard[start_column - west + 8 * start_row]){
      break;
    } else if (foe_bitboard[start_column - west + 8 * start_row]){
      valid_moves[start_column - west + 8 * start_row] = true; 
      break;
    } else {
      valid_moves[start_column - west + 8 * start_row] = true;
    }
  }

  for (int north_east{1}; north_east<8; north_east++){
    if ((start_row - north_east) < 0 || (start_row - north_east) > 7 ||(start_column + north_east) < 0 || (start_column + north_east) > 7 || friend_bitboard[(start_column + north_east) + 8 * (start_row - north_east)]){
      break;
    } else if (foe_bitboard[(start_column + north_east) + 8 * (start_row - north_east)]){
      valid_moves[(start_column + north_east) + 8 * (start_row - north_east)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column + north_east) + 8 * (start_row - north_east)] = true;
    }
  }

  for (int south_east{1}; south_east<8; south_east++){
    if ((start_row + south_east) < 0 || (start_row + south_east) > 7 ||(start_column + south_east) < 0 || (start_column + south_east) > 7 || friend_bitboard[(start_column + south_east) + 8 * (start_row + south_east)]){
      break;
    } else if (foe_bitboard[(start_column + south_east) + 8 * (start_row + south_east)]){
      valid_moves[(start_column + south_east) + 8 * (start_row + south_east)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column + south_east) + 8 * (start_row + south_east)] = true;
    }
  }

  for (int south_west{1}; south_west<8; south_west++){
    if ((start_row + south_west) < 0 || (start_row + south_west) > 7 ||(start_column - south_west) < 0 || (start_column - south_west) > 7 || friend_bitboard[(start_column - south_west) + 8 * (start_row + south_west)]){
      break;
    } else if (foe_bitboard[(start_column - south_west) + 8 * (start_row + south_west)]){
      valid_moves[(start_column - south_west) + 8 * (start_row + south_west)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column - south_west) + 8 * (start_row + south_west)] = true;
    }
  }

  for (int north_west{1}; north_west<8; north_west++){
    if ((start_row - north_west) < 0 || (start_row - north_west) > 7 ||(start_column - north_west) < 0 || (start_column - north_west) > 7 || friend_bitboard[(start_column - north_west) + 8 * (start_row - north_west)]){
      break;
    } else if (foe_bitboard[(start_column - north_west) + 8 * (start_row - north_west)]){
      valid_moves[(start_column - north_west) + 8 * (start_row - north_west)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column - north_west) + 8 * (start_row - north_west)] = true;
    }
  }
  return valid_moves;
}

char rook::get_representation()
{
  return represented;
}

std::unique_ptr<chess_piece> rook::clone()
{
  return std::make_unique<rook>(*this);
}

std::vector<bool> rook::valid_move(int start_row, int start_column,  std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  std::vector<bool> valid_moves(64, false);
  for (int north{1}; north<8; north++){
    if ((start_row - north) < 0 || (start_row - north) > 7 || friend_bitboard[start_column + 8 * (start_row - north)]){
      break;
    } else if (foe_bitboard[start_column + 8 * (start_row - north)]){
      valid_moves[start_column + 8 * (start_row - north)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[start_column + 8 * (start_row - north)] = true;
    }
  }
  for (int south{1}; south<8; south++){
    if ((start_row + south) < 0 || (start_row + south) > 7 || friend_bitboard[start_column + 8 * (start_row + south)]){
      break;
    } else if (foe_bitboard[start_column + 8 * (start_row + south)]){
      valid_moves[start_column + 8 * (start_row + south)] = true;
      break;
    } else {
      valid_moves[start_column + 8 * (start_row + south)] = true;
    }
  }
  for (int east{1}; east<8; east++){
    if ((start_column + east) < 0 || (start_column + east) > 7 || friend_bitboard[start_column + east + 8 * start_row] ){
      break;
    } else if (foe_bitboard[start_column + east + 8 * start_row]){
      valid_moves[start_column + east + 8 * start_row] = true; 
      break;
    } else {
      valid_moves[start_column + east + 8 * start_row] = true;
    }
  }
  for (int west{1}; west<8; west++){
    if ((start_column - west) < 0 || (start_column - west) > 7 || friend_bitboard[start_column - west + 8 * start_row]){
      break;
    } else if (foe_bitboard[start_column - west + 8 * start_row]){
      valid_moves[start_column - west + 8 * start_row] = true; 
      break;
    } else {
      valid_moves[start_column - west + 8 * start_row] = true;
    }
  }
  return valid_moves;
}

char bishop::get_representation()
{
  return represented;
}

std::unique_ptr<chess_piece> bishop::clone()
{
  return std::make_unique<bishop>(*this);
}

std::vector<bool> bishop::valid_move(int start_row, int start_column,  std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  std::vector<bool> valid_moves(64, false);
  for (int north_east{1}; north_east<8; north_east++){
    if ((start_row - north_east) < 0 || (start_row - north_east) > 7 ||(start_column + north_east) < 0 || (start_column + north_east) > 7 || friend_bitboard[(start_column + north_east) + 8 * (start_row - north_east)]){
      break;
    } else if (foe_bitboard[(start_column + north_east) + 8 * (start_row - north_east)]){
      valid_moves[(start_column + north_east) + 8 * (start_row - north_east)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column + north_east) + 8 * (start_row - north_east)] = true;
    }
  }

  for (int south_east{1}; south_east<8; south_east++){
    if ((start_row + south_east) < 0 || (start_row + south_east) > 7 ||(start_column + south_east) < 0 || (start_column + south_east) > 7 || friend_bitboard[(start_column + south_east) + 8 * (start_row + south_east)]){
      break;
    } else if (foe_bitboard[(start_column + south_east) + 8 * (start_row + south_east)]){
      valid_moves[(start_column + south_east) + 8 * (start_row + south_east)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column + south_east) + 8 * (start_row + south_east)] = true;
    }
  }

  for (int south_west{1}; south_west<8; south_west++){
    if ((start_row + south_west) < 0 || (start_row + south_west) > 7 ||(start_column - south_west) < 0 || (start_column - south_west) > 7 || friend_bitboard[(start_column - south_west) + 8 * (start_row + south_west)]){
      break;
    } else if (foe_bitboard[(start_column - south_west) + 8 * (start_row + south_west)]){
      valid_moves[(start_column - south_west) + 8 * (start_row + south_west)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column - south_west) + 8 * (start_row + south_west)] = true;
    }
  }

  for (int north_west{1}; north_west<8; north_west++){
    if ((start_row - north_west) < 0 || (start_row - north_west) > 7 ||(start_column - north_west) < 0 || (start_column - north_west) > 7 || friend_bitboard[(start_column - north_west) + 8 * (start_row - north_west)]){
      break;
    } else if (foe_bitboard[(start_column - north_west) + 8 * (start_row - north_west)]){
      valid_moves[(start_column - north_west) + 8 * (start_row - north_west)] = true; // Marked as a valid move as we can
      // take this piece if it is the opposite color
      break;
    } else {
      valid_moves[(start_column - north_west) + 8 * (start_row - north_west)] = true;
    }
  }
  return valid_moves;
}

char knight::get_representation()
{
  return represented;
}

std::unique_ptr<chess_piece> knight::clone()
{
  return std::make_unique<knight>(*this);
}

std::vector<bool> knight::valid_move(int start_row, int start_column,  std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  std::vector<bool> valid_moves(64, false);
  for (int i{-1}; i<2; i++){
    if (i == 0){
      continue;
    }
    int temp_row_one = start_row + 2*i;
    int temp_column_two = start_column + 2*i;
    for (int j{-1}; j<2; j++){
      if (j == 0){
        continue;
      }
      int temp_column_one = start_column + j;
      int temp_row_two = start_row + j;
      if (temp_column_one<8 && temp_column_one>=0 && temp_row_one<8 && temp_row_one>=0 && !friend_bitboard[temp_column_one + 8*temp_row_one]){
        valid_moves[temp_column_one + 8*temp_row_one] = true;
      }
      if (temp_column_two<8 && temp_column_two>=0 && temp_row_two<8 && temp_row_two>=0 && !friend_bitboard[temp_column_two + 8*temp_row_two]){
        valid_moves[temp_column_two + 8*temp_row_two] = true;
      }
    }
  }
  return valid_moves;
}


char pawn::get_representation()
{
  return represented;
}

std::unique_ptr<chess_piece> pawn::clone()
{
  return std::make_unique<pawn>(*this);
}

std::vector<bool> pawn::valid_move(int start_row, int start_column,  std::vector<bool> foe_bitboard, std::vector<bool> friend_bitboard)
{
  // This will create segmentation errors in certain circumstances: 
  std::vector<bool> valid_moves(64, false);

  if (first_move){
    if (color == 'w'){
      if (!foe_bitboard[start_column + 2 + 8*start_row] && !friend_bitboard[start_column + 2 + 8*start_row]){
        valid_moves[start_column + 2 + 8*start_row] = true;
        //std::cout<<"two "<<std::endl;
      }
    } else if (color == 'b'){
      //std::cout<<"forbidden "<<std::endl;
      if (!foe_bitboard[start_column - 2 + 8*start_row] && !friend_bitboard[start_column - 2 + 8*start_row]){
        valid_moves[start_column - 2 + 8*start_row] = true;
        
      }
    }
  }

  if (color == 'w'){
    if (!foe_bitboard[start_column + 1 + 8*start_row] && !friend_bitboard[start_column + 1 + 8*start_row]){
      valid_moves[start_column + 1 + 8*start_row] = true;
      //std::cout<<"three "<<std::endl;
    }
    if((start_row-1)>=0){ // protect from segmentation error accessing non-existent square
      if (foe_bitboard[start_column + 1 + 8*(start_row-1)]){
        valid_moves[start_column + 1 + 8*(start_row-1)] = true;
        //std::cout<<"four "<<std::endl;
      }
    }
    if((start_row+1)<8){
      if (foe_bitboard[start_column + 1 + 8*(start_row+1)]){
        valid_moves[start_column + 1 + 8*(start_row+1)] = true;
        //std::cout<<"five "<<std::endl;
      }
    }
  } else if (color == 'b'){
    //std::cout<<"forbidden "<<std::endl;
    if (!foe_bitboard[start_column - 1 + 8*start_row] && !friend_bitboard[start_column - 1 + 8*start_row]){
      valid_moves[start_column - 1 + 8*start_row] = true;
    }
    if((start_row-1)>=0){ 
      if (foe_bitboard[start_column - 1 + 8*(start_row-1)]){
        valid_moves[start_column - 1 + 8*(start_row-1)] = true;
      }
    }
    if((start_row+1)<8){
      if (foe_bitboard[start_column - 1 + 8*(start_row+1)]){
        valid_moves[start_column - 1 + 8*(start_row+1)] = true;
      }
    }
  }
  //std::cout<<"end "<<std::endl;
  return valid_moves;
}


char chess_piece::get_color()
{
  return color;
}


square::square(std::unique_ptr<chess_piece> placed_piece) : inhabitant{std::move(placed_piece)}
{
  inhabited = true; // if we place a piece on the square when it is created, then it is inhabited
}

square& square::operator=(square&& copied_square)
{
  std::swap(inhabitant, copied_square.inhabitant);
  std::swap(inhabited, copied_square.inhabited);
  return *this;
}


void square::empty() // Enpty a square
{
  inhabited = false;
  inhabitant = nullptr;
}

char square::get_representation()
{
  if (inhabited){
    return (*inhabitant).get_representation(); // THIS IS CHANGED!!! IF PRiNT DOENS'T WORK LOOK HERE
  } else if (! inhabited){
    return ' ';
  } else {
    throw("Square of unknown ownership");
  }
}

std::ostream& chess::operator<<(std::ostream &output, const square &inserted_piece)
{ 
  if (inserted_piece.inhabited){
    output<<(*inserted_piece.inhabitant).get_representation()<<(*inserted_piece.inhabitant).get_color();
  } else {
    output<<"  ";
  }
  return output;
}







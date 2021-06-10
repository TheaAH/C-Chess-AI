

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <limits>

#include "game_modes.h"

using namespace game_mode;

std::string game_mode::name_savefile()
{
  std::ofstream moves_file{"move_log.txt"};

  auto save_time = std::chrono::system_clock::now();
  std::time_t converted_time = std::chrono::system_clock::to_time_t(save_time); // Add date and time as though that's useful
  moves_file<<"New savefile opened"<<std::endl;
  moves_file<<std::ctime(&converted_time)<<std::endl;
  moves_file.close();

  return "move_log.txt";
}

void game_mode::human_vs_human()
{
  std::string file_name = name_savefile();
  chess::move accepted_move{};
  chess::board board_in_play{};
  board_in_play.print_board();
  while (!board_in_play.check_mate('w') && !board_in_play.check_mate('b')){

    accepted_move = board_in_play.prepare_move('w');
    accepted_move.write_to_file(file_name);
    board_in_play.print_board();

    if (board_in_play.check_mate('b')){
      board_in_play.print_board();
      std::cout<<"White wins!! Black is in check mate "<<std::endl;
      break;
    }

    accepted_move = board_in_play.prepare_move('b');
    accepted_move.write_to_file(file_name);
    board_in_play.print_board();

    if (board_in_play.check_mate('w')){
      board_in_play.print_board();
      std::cout<<"Black wins!! White is in check mate "<<std::endl;
    }
  }
}

void game_mode::random() // king will NOT respond to being in check!!! So the game ends when black is in check
{
  std::string file_name = name_savefile();
  chess::move accepted_move{};
  chess::move random_move{};
  chess::board board_in_play{};
  while (!board_in_play.check_mate('w') && !board_in_play.check('b')){

    board_in_play.print_board();
    accepted_move = board_in_play.prepare_move('w');
    accepted_move.write_to_file(file_name);

    if (board_in_play.check('b')){
      board_in_play.print_board();
      std::cout<<"You win!! Black is in check "<<std::endl;
      break;
    }

    random_move = board_in_play.generate_random_move('b');
    random_move.write_to_file(file_name);
    board_in_play.make_move(random_move);

    if (board_in_play.check_mate('w')){
      board_in_play.print_board();
      std::cout<<"Black wins!! You are in checkmate "<<std::endl;
    }

  }
}

int choose_level()
{
  int ai_depth;
  std::cout<<"What 'level' AI do you want to play? (corresponds to the depth of the AI) Choices are 1-4: "<<std::endl;
  std::cin;
  std::cin>>ai_depth;

  while (std::cin.fail() || ai_depth<1 || ai_depth>4){
    std::cout<<"Whoops!!! that's not a valid level. Give me a number between 1 and 4 (1 is easy, 4 is 'hard'): "<<std::endl;
    std::cin.clear ();
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin;
    std::cin>>ai_depth;        
  }
  return ai_depth;
}

void game_mode::vs_ai()
{
  int ai_depth = choose_level();

  std::string file_name = name_savefile();
  chess::move accepted_move{};
  std::pair<chess::move, int> ai_move{};
  chess::board board_in_play{};
  while (!board_in_play.check_mate('w') && !board_in_play.check('b')){

    board_in_play.print_board();
    accepted_move = board_in_play.prepare_move('w');
    accepted_move.write_to_file(file_name);

    if (board_in_play.check_mate('b')){
      board_in_play.print_board();
      std::cout<<"You win!! Black is in check "<<std::endl;
      break;
    }

    ai_move = board_in_play.minimax('b', ai_depth, -std::numeric_limits<int>::max(), +std::numeric_limits<int>::max());

    ai_move.first.write_to_file(file_name);
    board_in_play.make_move(ai_move.first);

    if (board_in_play.check_mate('w')){
      board_in_play.print_board();
      std::cout<<"Black wins!! You are in checkmate "<<std::endl;
    }

  }
}

void game_mode::menu()
{
  std::cout<<"Hi! welcome to the chess game!!! to play, use chess moves notation, ie. c2 c4 if you want to open by moving the white queenside rooks pawn forward two squares. "<<std::endl;
  std::string mode_choice{};

  std::cout<<"Do you want to play against another human (type 'human'), against random moves (type 'random), or against a basic AI (type ai)? "<<std::endl;
  std::cin;
  std::cin>>mode_choice;

  if (!std::cin.fail()){ // don't want to try to convert a thing that's not right anyway
    mode_choice = chess::answer_to_lowercase(mode_choice);
  }
  while (std::cin.fail() || (mode_choice != "human" && mode_choice != "random" && mode_choice != "ai")){
    std::cin.clear ();
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');        
    std::cout<<"Try again ! Do you want to play a game of chess? "<<std::endl;
    std::cin;
    std::cin>>mode_choice;
    if (!std::cin.fail()){ // don't want to try to convert a thing that's not right anyway
      mode_choice = chess::answer_to_lowercase(mode_choice);
  }
  }

  if (mode_choice == "human"){
    game_mode::human_vs_human();
  } else if (mode_choice == "random"){
    game_mode::random();
  } else if (mode_choice == "ai"){
    game_mode::vs_ai();
  }
}












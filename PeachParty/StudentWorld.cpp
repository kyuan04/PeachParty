#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    //TODO:
    //1. initialize data structures used to keep track of my game's world
    Board b;
    string board_file = assetPath() + "board01.txt";
    Board::LoadResult result = b.loadBoard(board_file);
    
    if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
    } else if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n"; //FIX THIS IT'S HARDCODED RN
    } else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
    }
    
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Board::GridEntry ge = b.getContentsOf(i, j);
            
            switch (ge) {
                case Board::empty:
                    break;
                case Board::player:
                    peach = new Peach(this, i, j);
                    break;
                case Board::blue_coin_square:
                    actors.push_back(new CoinSquare(this, i, j));
                    break;
                case Board::red_coin_square:
                    break;
                case Board::left_dir_square:
                    break;
                case Board::right_dir_square:
                    break;
                case Board::up_dir_square:
                    break;
                case Board::down_dir_square:
                    break;
                case Board::event_square:
                    break;
                case Board::bank_square:
                    break;
                case Board::star_square:
                    break;
                case Board::bowser:
                    break;
                case Board::boo:
                    break;
                    
            }
        }
    }
    //2. Allocate and insert Peach and Yoshi objects into the game world. Every time a game begins, Peach and Yoshi start out initialized in their starting location asspecified by the current board data file
    
    
    //3. Allocate and insert all of the other objects (e.g., squares, baddies, etc.) into the game world as described below
    //4. Start the countdown timer for the 99-second game.
	startCountdownTimer(5);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

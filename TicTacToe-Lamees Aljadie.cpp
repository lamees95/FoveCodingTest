/*********************************
 * Fove C++ Programming Test -----
 *********************************

 This test simulates a Tic Tac Toe (noughts and crosses) game server.

 The goal is to implement the following functions (each one documented further down):
   CreateGame()
   AddPlayer()
   MakeMove()

 Already-implemented tests call these functions and check that they work.
 Initially, all the tests fail. Your goal is to make them all pass.
 The tests check both valid and invalid inputs, ensuring that error handling is correct.

 Take as much time as you need (within reason), but in general this is expected to take about an hour.

 The rules of tic tac toe are:
   - Two players take turns. Each turn someone takes a spot on a 3x3 grid.
   - The first player to take 3 collinear adjacent spots (vertically, horizontally, or diagonally) wins.
   - The game can end in a draw if all 9 spots are taken and no one wins.

 Notes:
   - Use any C or C++ language features that you like. This base code requires at least C++11.
   - Use only the standard library. Don't add any other third party dependencies.
   - You will not be judged on the code style. Use any style that works for you.
   - Do not worry about threading, assume single threaded.
   - Design your implementation cleanly, as if someone else were to maintain this code after you implement it.
   - Comments are encouraged where useful, but generally things should be simple enough to not warrant many.

 How to build:
   Since this is a single-file project, you can simply invoke the compiler directly.
   Alternatively you can set up a project in your favorite tool.

   Here's an example on Linux:
	 user@somepc:~/programming-test$ c++ TicTacToe.cpp && ./a.out
	   [FAILED] testCreateGame: Negative game id
	   [PASSED] testInvalidGameIds
	   [FAILED] testAddPlayer: Negative game id
	   [FAILED] testPlayer2Win: Negative game id
	   [FAILED] testPlayer2Win: Negative game id
	   [FAILED] testPlayer1Win: Negative game id
	   [FAILED] testPlayer1Win: Negative game id
	   [FAILED] testDraw: Negative game id
	   [FAILED] testDraw: Negative game id

	 In the initial state, the tests will fail. By the end, all tests should pass:

	 user@somepc:~/programming-test$ c++ TicTacToeSolution.cpp && ./a.out
	   [PASSED] testCreateGame
	   [PASSED] testInvalidGameIds
	   [PASSED] testAddPlayer
	   [PASSED] testPlayer2Win
	   [PASSED] testPlayer2Win
	   [PASSED] testPlayer1Win
	   [PASSED] testPlayer1Win
	   [PASSED] testDraw
	   [PASSED] testDraw
*/
#include <map>
#include <random>
using namespace std;
// Aliases
// These double as result codes for simplicity of the API
using GameId = int;   // Valid game ids are >= 0
using PlayerId = int; // Valid player ids are >= 0

// Result codes
// These are listed in order: if multiple are applicable, the higher one takes precedence
// For example, if the game & the player both don't exist, GAME_DOESNT_EXIST is expected (instead of PLAYER_DOESNT_EXIST)
constexpr int GAME_DOESNT_EXIST = -2;
constexpr int GAME_NOT_STARTED = -3;
constexpr int GAME_ENDED = -4;
constexpr int GAME_ONGOING = -5;
constexpr int PLAYER_DOESNT_EXIST = -6;
constexpr int WRONG_TURN = -7;
constexpr int INVALID_LOCATION = -8;

// Creates a new game. Multiple games may be running simultaneously.
//
// Returns:
//   - A valid unique ID for the new game
// Errors:
//   None
struct game{
    GameId g_id = GAME_DOESNT_EXIST;
    int status = GAME_DOESNT_EXIST;
    PlayerId p1 = PLAYER_DOESNT_EXIST;
    PlayerId p2 = PLAYER_DOESNT_EXIST;
    int next_player = 0;
    int game_board [3][3] = {{0,0,0},{0,0,0},{0,0,0}};
};
PlayerId p = 0;
static map<GameId,game> gameMap;

GameId CreateGame() noexcept
{
	// IMPLEMENT ME!
	random_device device;
    mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0,1000000);
    GameId game_id = distribution(generator);
    while(gameMap.find(game_id)!=gameMap.end()){
        game_id = distribution(generator);
    }
	game new_game;
	new_game.g_id = game_id;
	new_game.status = GAME_NOT_STARTED;
	gameMap.insert({game_id, new_game});
	return (GameId)game_id;
}

// Adds a player to a game that has been created, but not started.
// This function starts the game automatically once the 2nd player has joined.
// Once the game starts, the first player's turn begins (the one identified first call to AddPlayer)
//
// Returns:
//   A valid ID for the new player, unique to this game, which may be any integer greater than zero
//
//   GAME_DOESNT_EXIST if the game id does not identify a valid game
//   GAME_ENDED if the game has ended
//   GAME_ONGOING if the game has already begun
PlayerId AddPlayer(GameId gameId) noexcept
{
	// IMPLEMENT ME!
	if(gameMap.find(gameId) == gameMap.end()){
        return GAME_DOESNT_EXIST;
	}
	if(gameMap[gameId].status == GAME_ENDED || gameMap[gameId].status == GAME_ONGOING){
        return gameMap[gameId].status;
    }
    if(gameMap[gameId].p1 == PLAYER_DOESNT_EXIST){
        gameMap[gameId].p1 = ++p;
        gameMap[gameId].next_player = p;
    } else {
        gameMap[gameId].p2 = ++p;
        gameMap[gameId].status = GAME_ONGOING;
    }
	return (PlayerId)p;
}

// Allows a player to make a move
//
// After each valid move, the turn switches to the other player.
// If the move completes the game, the game status shall be considered ended.
// No early detection of draws is done. Game must fully play out (9 moves) to reach a draw.
//
// Returns:
//   GAME_ONGOING if no one has won yet
//   The id of the current player if he won with this move (game is then ended)
//   The id of the other player if the game ended in a draw (game is then ended)
//
//   GAME_DOESNT_EXIST if the game id does not identify a valid game
//   GAME_NOT_STARTED if the game has not started
//   GAME_ENDED if the game has already ended before this was called
//   PLAYER_DOESNT_EXIST if the player id is not valid for this game
//   WRONG_TURN if this is not player A turn
//   INVALID_LOCATION if boardX or boardY is outside the range of [0, 2], or if that spot has been used already
//

int check_game(GameId gameId, PlayerId playerId){
    int result = GAME_ONGOING;
    if( ( gameMap[gameId].game_board[0][0]== playerId&& gameMap[gameId].game_board[0][1]== playerId && gameMap[gameId].game_board[0][2]== playerId )||
        ( gameMap[gameId].game_board[1][0]== playerId&& gameMap[gameId].game_board[1][1]== playerId && gameMap[gameId].game_board[1][2]== playerId )||
        ( gameMap[gameId].game_board[2][0]== playerId&& gameMap[gameId].game_board[2][1]== playerId && gameMap[gameId].game_board[2][2]== playerId )||
        ( gameMap[gameId].game_board[0][0]== playerId&& gameMap[gameId].game_board[1][0]== playerId && gameMap[gameId].game_board[2][0]== playerId )||
        ( gameMap[gameId].game_board[0][1]== playerId&& gameMap[gameId].game_board[1][1]== playerId && gameMap[gameId].game_board[2][1]== playerId )||
        ( gameMap[gameId].game_board[0][2]== playerId&& gameMap[gameId].game_board[1][2]== playerId && gameMap[gameId].game_board[2][2]== playerId )||
        ( gameMap[gameId].game_board[0][0]== playerId&& gameMap[gameId].game_board[1][1]== playerId && gameMap[gameId].game_board[2][2]== playerId )||
        ( gameMap[gameId].game_board[0][2]== playerId&& gameMap[gameId].game_board[1][1]== playerId && gameMap[gameId].game_board[2][0]== playerId )
       ){
        result = playerId;
        gameMap[gameId].status = GAME_ENDED;
    } else if( gameMap[gameId].game_board[0][0] !=0 &&
               gameMap[gameId].game_board[0][1] !=0 &&
               gameMap[gameId].game_board[0][2] !=0 &&
               gameMap[gameId].game_board[1][0] !=0 &&
               gameMap[gameId].game_board[1][1] !=0 &&
               gameMap[gameId].game_board[1][2] !=0 &&
               gameMap[gameId].game_board[2][0] !=0 &&
               gameMap[gameId].game_board[2][1] !=0 &&
               gameMap[gameId].game_board[2][2] !=0 ){
                    if(gameMap[gameId].p1 == playerId){
                        result = gameMap[gameId].p2;
                    } else {
                        result = gameMap[gameId].p1;
                    }
                    gameMap[gameId].status = GAME_ENDED;
    }
    return result;
}
PlayerId MakeMove(GameId gameId, PlayerId playerId, int boardX, int boardY) noexcept
{
	// IMPLEMENT ME!
	if(gameMap.find(gameId)== gameMap.end()){
        return GAME_DOESNT_EXIST;
	}
	if(gameMap[gameId].status == GAME_NOT_STARTED || gameMap[gameId].status == GAME_ENDED){
        return gameMap[gameId].status;
	}
	if(gameMap[gameId].p1 != playerId && gameMap[gameId].p2 != playerId){
        return PLAYER_DOESNT_EXIST;
	}
	if(gameMap[gameId].next_player != playerId){
        return WRONG_TURN;
	}
	if(boardX < 0 || boardX >2 || boardY < 0 || boardY > 2 || gameMap[gameId].game_board[boardX][boardY]!=0){
        return INVALID_LOCATION;
	}
	gameMap[gameId].game_board[boardX][boardY]= playerId;
	int result = check_game(gameId,playerId);
	if(gameMap[gameId].p1 == playerId){
        gameMap[gameId].next_player = gameMap[gameId].p2;
	} else {
	    gameMap[gameId].next_player = gameMap[gameId].p1;
	}
	return (PlayerId)result;
}


//////////////////////////////////////////////////////
// Nothing below this point needs to be changed ------
// Below is main() and tests -------------------------
//////////////////////////////////////////////////////

#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace Test
{

bool IsValidId(int id) { return id >= 0; }

struct GameInfo
{
	PlayerId players[2] = {-1, -1};
};

std::map<GameId, GameInfo> testGames;

// Simple assert helper
void Check(const bool expr, const char* const text)
{
	if (!expr)
		throw text; // Using c-string throws as a simple mechanism to indicate failure
}

GameId CreateTestGame() // Wrapper for createGame that does some basic checks
{
	const GameId gameId = CreateGame();
	Check(gameId >= 0, "Negative game id");
	Check(testGames.emplace(gameId, GameInfo{}).second, "Duplicate game id");
	return gameId;
}

GameId InvalidGameId() // Returns an unused game id
{
	for (GameId i = (GameId)200;; ++i)
		if (testGames.find(i) == testGames.end())
			return i;
}

PlayerId AddTestPlayer(GameId gameId)
{
	PlayerId playerId = AddPlayer(gameId);
	if (playerId < 0)
		return playerId;

	const auto gameIt = testGames.find(gameId);
	Check(gameIt != testGames.end(), "Invalid gameID accepted to addPlayer");
	GameInfo& game = gameIt->second;

	if (!IsValidId(game.players[0]))
		game.players[0] = playerId;
	else if (game.players[0] == playerId)
		throw "Duplicate player id in game";
	else if (!IsValidId(game.players[1]))
		game.players[1] = playerId;
	else
		throw "Received a player id from a full game";

	return playerId;
}

void TestCreateGame()
{
	for (int i = 0; i < 10; ++i)
		CreateTestGame();
}

void TestInvalidGameIds()
{
	// Test some negative ids
	for (GameId i = (GameId)(-1); i > (GameId)(-10); --i)
		Check(AddTestPlayer(i) == (PlayerId)GAME_DOESNT_EXIST, "Negative game id should not be valid");

	// Test an invalid positive id
	Check(AddTestPlayer(InvalidGameId()) == (PlayerId)GAME_DOESNT_EXIST, "Invalid game id was accepted");

	// Test making a move on an invalid game
	Check(MakeMove(InvalidGameId(), 0, 0, 0) == (PlayerId)GAME_DOESNT_EXIST, "Wrong player move accepted");
}

void TestAddPlayer()
{
	GameId gameId = CreateTestGame();

	// Make sure we can't make a move before adding players
	Check(MakeMove(gameId, 0, 0, 0) == (PlayerId)GAME_NOT_STARTED, "Made a move with no players added");

	// Add the first player
	PlayerId player1Id;
	Check(IsValidId(player1Id = AddTestPlayer(gameId)), "Negative game id should not be valid");

	// Make sure we can't make a move after adding the first player
	Check(MakeMove(gameId, player1Id, 0, 0) == (PlayerId)GAME_NOT_STARTED, "Made a move with only one player added");

	// Add the second player
	PlayerId player2Id;
	Check(IsValidId(player2Id = AddTestPlayer(gameId)), "Negative game id should not be valid");

	// Make sure we can't move with the 2nd player, but that the game has started
	Check(MakeMove(gameId, player2Id, 0, 0) == (PlayerId)WRONG_TURN, "Made a move with only one player added");

	// Make sure that we can't add more players after that
	for (int i = 0; i < 10; ++i)
		Check(AddTestPlayer(gameId) == (PlayerId)GAME_ONGOING, "Adding players after the second should return GAME_ONGOING");
}

// A series of full where play
const std::vector<std::pair<int, int>> wins[] =
	{
		// xo_
		// xo_
		// x__
		{
			{0, 0},
			{1, 0},
			{0, 1},
			{1, 1},
			{0, 2},
		},

		// xox
		// oxo
		// x__
		{
			{2, 0},
			{2, 1},
			{0, 0},
			{1, 0},
			{0, 2},
			{0, 1},
			{1, 1},
		},

		// oxx
		// ooo
		// xx_
		{
			{0, 2},
			{1, 1},
			{2, 0},
			{0, 1},
			{1, 2},
			{0, 0},
			{1, 0},
			{2, 1},
		},

		// ox_
		// _o_
		// xx_
		{
			{1, 0},
			{1, 1},
			{0, 2},
			{0, 0},
			{1, 2},
			{2, 2},
		},
};

const std::vector<std::pair<int, int>> draws[] =
	{
		// xxo
		// oox
		// xxo
		{
			{1, 2},
			{1, 1},
			{0, 2},
			{2, 2},
			{0, 0},
			{0, 1},
			{2, 1},
			{2, 0},
			{1, 0},
		},

		// oxo
		// xxo
		// xox
		{
			{1, 1},
			{0, 0},
			{0, 2},
			{2, 0},
			{1, 0},
			{1, 2},
			{0, 1},
			{2, 1},
			{2, 2},
		},
};

void TestFullGame(const std::vector<std::pair<int, int>>& game, const bool isDraw)
{
	GameId gameId = CreateTestGame();

	PlayerId player1Id = AddTestPlayer(gameId);
	PlayerId b = AddTestPlayer(gameId);

	for (size_t i = 0; i < game.size(); ++i)
	{
		PlayerId currentPlayer = i % 2 == 0 ? player1Id : b;
		PlayerId otherPlayer = i % 2 == 0 ? b : player1Id;
		std::pair<int, int> nextMove = game[i];

		// Test that we can add players at no point during the game
		Check(AddTestPlayer(gameId) == (PlayerId)GAME_ONGOING, "Adding players during the game should return GAME_ONGOING");

		// Test that the wrong player can't move
		Check(MakeMove(gameId, otherPlayer, nextMove.first, nextMove.second) == WRONG_TURN, "Wrong player move accepted");

		// Test that the right player can't move to any previously used spot
		for (size_t i2 = 0; i2 < i; ++i2)
			Check(MakeMove(gameId, currentPlayer, game[i2].first, game[i2].second) == INVALID_LOCATION, "Wrong player move accepted");

		// Test that the correct player can't move to a spot outside the board
		Check(MakeMove(gameId, currentPlayer, -1 - static_cast<int>(i), 0) == INVALID_LOCATION, "Invalid board location accepted");
		Check(MakeMove(gameId, currentPlayer, 0, static_cast<int>(i) + 3) == INVALID_LOCATION, "Invalid board location accepted");

		// Make move
		if (i + 1 < game.size())
		{
			// Not-final move
			Check(MakeMove(gameId, currentPlayer, nextMove.first, nextMove.second) == GAME_ONGOING, "Valid move rejected");
		}
		else
		{
			PlayerId expectedResult = otherPlayer; // Returning the other player means draw
			const char* text = "Game should have been a draw";
			if (!isDraw)
			{
				expectedResult = currentPlayer;
				text = i % 2 == 0 ? "Player 1 should have won" : "Player 2 should have won";
			}

			// Final move of the game
			Check(MakeMove(gameId, currentPlayer, nextMove.first, nextMove.second) == expectedResult, text);

			// Test that after the game is complete, addTestPlayer and makeMove return GAME_ENDED
			Check(AddTestPlayer(gameId) == (PlayerId)GAME_ENDED, "Adding a player after the game ended should return GAME_ENDED");
			Check(MakeMove(gameId, i % 2 == 0 ? b : player1Id, game[i].first, game[i].second) == GAME_ENDED, "Making a move after the game ended shoudl return GAME_ENDED");
		}
	}
}

template <typename TestFunc, typename... Args>
bool RunTest(const char* const name, const TestFunc func, Args&&... args)
{
	try
	{
		func(std::forward<Args>(args)...);
		std::cout << "[PASSED] " << name << std::endl;
		return false;
	}
	catch (const char* text)
	{
		std::cout << "[FAILED] " << name << ": " << text << std::endl;
		return true;
	}
}

int RunTests()
{
	bool failed = RunTest("testCreateGame", &TestCreateGame);
	failed = RunTest("testInvalidGameIds", &TestInvalidGameIds) || failed;
	failed = RunTest("testAddPlayer", &TestAddPlayer) || failed;

	for (auto& game : wins)
		failed = RunTest(game.size() % 2 == 0 ? "testPlayer1Win" : "testPlayer2Win", &TestFullGame, game, false) || failed;

	for (auto& game : draws)
		failed = RunTest("testDraw", &TestFullGame, game, true) || failed;

	return failed ? 1 : 0; // Return nonzero if at least one test failed
}

} // namespace Test

int main()
{
	return Test::RunTests();
}

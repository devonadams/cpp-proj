/*					CONSOLE-CHESS v0.59
		TO DO
debug:

	create directories for common/debug for saving files.

	fixed:
		color randomly changing			// due to the below issue - memory overwrite due to improper array index
		random high values being assigned to integers	//	due to not checking if move INT was between 0 and iBoardSize
		Queen recursive path not travelling full distance.		//	due to bad logic
		Fix mcheck command for KING, QUEEN, ROOK, BISHOP, KNIGHT, and PAWN
		Added excessive comments on global variables for better understanding
		fixed GameNumber not working properly

features:

	Add options for how player wants mcheck to be displayed******************************

	Pawn Promotion
		~use input to initialize new piece, global varialbe starting from how many default pieces there are.
	Pawn "en passant"

	write "checkmate" command

	Add proper piece capturing and create a more dynamic scoring system

	files:
		make GameNumber work properly
		save user settings
		save game moves
		input move files
			resume previous game or
			watch game move by move

			complete:
					debug levels 0, 1, 2, 3 (mostly)
					Added support for text only consoles (no ANSI codes)
					ensure pawn capturing works properly
					change piece color
					castling
					write mcheck command
					Make board colors more dynamic based on what is trying to be told
					Add text only support with mcheck
					Use b**PieceName**Logic() instead of seperate functions for check and move.
					mcheck - Show move is valid when piece is opposite color
					added input sanitization for commands
					added functions for CharToInt and IntToChar
					Added ChessLogic.h and moved logic

gameplay:
	Check if king is in check
		Add functionality for checking if a piece of the checked color can move to block the check
			IF false, then checkmate

		~add functionality for only spawning certain pieces for debugging

	Add single player functionality (chess AI)

		complete:
				multiplayer (2 player back and forth)
				verify all peices execute properly	(mcheck logic)
				Create game win
				Create game draw	(stalemate)
				Create game lose	(singleplayer)

extras:
	Add different gamemodes??
		~Chess960
		~King Of The Hill
		~Bughouse
		~Crazyhouse
		~3-Check
*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <climits>

#include <cctype>
#include <iomanip>
#include <fstream>
#include <filesystem>

#include <ChessLogic.h>
#include <ChessAI.h>

//	Initalize GLOBAL VARIBALES
	//	Initalize Board Dimentions
const unsigned int iBoardWidth = 8;
const unsigned int iBoardHeight = 8;
const unsigned int iBoardSize = (iBoardWidth * iBoardHeight) - 1;

	//	Initalize ARRAYS
char iBoard			[ 8 * 8 ] = { ' ' };	//	Holds values the player sees
std::string sBoard	[ 8 * 8 ] = { " " };	//	Holds class name of piece and serves as a search function for objects within the class
char CheckBoard		[ 8 * 8 ] = { ' ' };	//	Holds value for where pieces can move -- Used for checking for check / Checking valid moves
char CheckmateBoard	[ 8 * 8 ] = { ' ' };	//	Holds value for checkmate (Can the king move?)
char cMoveBoard		[ 8 * 8 ] = { ' ' };	//	When cmove issued, CheckBoard is copied to this

	//	Initalize VECTORS
std::vector<std::string> dHistory;		//	Stores Debug History
std::vector<std::string> mHistory;		//	Stores Move History
std::vector<std::string> sCaptured;		//	Stores Captured Pieces
std::vector<std::string> sChecking;		//	Stores which pieces are checking the king (not implemented)
std::vector<std::string> sSidePrint;	//	Stores strings printed to the side of the board
std::string sErrorMsg = "";				//	Stores Error Message when something is invalid

	// uncode piece codes for the future
	//char const* special_character[] = { "\u2654", "\u2655", "\u2656", "\u2657", "\u2658", "\u2659", //	WHITE
	//		"\u265A", "\u265b", "\u265c", "\u265d", "\u265e", "\u265f" };	//	BLACK

unsigned int wMoves = 0;				//	Stores total WHITE moves			--	UN-NEEDED unless purpose is found	(due to this->iMoves)
unsigned int bMoves = 0;				//	Stores total BLACK moves			--	UN-NEEDED unless purpose is found	(due to this->iMoves)
unsigned int iWhiteScore = 39;			//	Stores WHITE score based on number of pieces	--	For ChessAI
unsigned int iBlackScore = 39;			//	Stores BLACK score based on number of pieces	--	For ChessAI

	//	Background Game Variables
unsigned int GameNumber = 0;			//	Stores GameNumber for file writing
unsigned int mHistoryNumber = 0;		//	Stores Move History Number incase of multiple saves in one process
unsigned int mHistoryReadNumber = 0;	//	Stores the Move History Number that was last read
unsigned int dHistoryReadNumber = 0;	//	Stores the Debug History Read Number that was last read
unsigned int dHistoryNumber = 0;		//	Stores the Debug History Number incase of multiple saves in one process

	//	Player settings
unsigned int iDebugLevel = 0;			//	Stores the process debug level
unsigned int iPieceColor = 94;			//	Stores the process Piece Color value
char boardType = 'f';					//	Stores selected Board Type - l for large / s for small
bool bGraphics = false;					//	Stores whether player said ANSI esacpe codes worked, and enables/disables them

	//	Check Logic
bool bWhiteKingInCheck = false;			//	Is the WHITE king in check?
bool bBlackKingInCheck = false;			//	Is the BLACK king in check?
bool bCheckmate = false;				//	Is the game a checkmate?
unsigned int iWhiteKingLocation = 60;	//	Stores WHITE king location for Check if Check validation		---		UN-NEEDED if using wKing GetPostion
unsigned int iBlackKingLocation = 4;	//	Stores BLACK king location for Check if Check validation		---		UN-NEEDED if using bKing GetPostion
	//	Move Logic
bool bMoveToWhite = false;				//	Find color of Piece the player is attempting to move to	--	UN-NEEDED GLOBAL due to move to HEADER file
bool bCapturePiece = false;				//	Determine if move to capture a Piece is attempted			--	UN-NEEDED GLOBAL due to move to HEADER file
bool CurrentColorIsWhite = true;		//	Current move is WHITE
bool bCurrentlyCastling = false;		//	CASTLE move was attempted	--	used in Rook Logic
bool bCastleSideQueen = false;			//	CASTLE move was QUEENSIDE	--	used in Rook Logic
unsigned int iMoveFrom = 0;				//	Player selected Piece is Moving From
unsigned int iMoveTo = 0;				//	Player selected Piece is attempting to Move To

	//	Game Status
bool bGameStatus = true;				//	Stores if a singleplayer / multiplayer match is occuring
bool bRematch = false;					//	Used for 'rematch' command to skip re-inputing game properties

	//	Save this-> values to variable for use inside bLogic function ( NO LONGER IN USE )
unsigned int iThisPos = 0;				//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iPosition
unsigned int iThisMoves = 0;			//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iMoves
bool iThisWhite = true;					//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iWhitePiece
unsigned int pieceType = 0;				//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iType
bool bMoveCheck = false;				//	----------------------------------------------------------------------	POSSIBLY UN-NEEDED due to local "CheckForCheck" value in stack


	//	Functions
bool bSinglePlayer();			//	Player selected Single Player game (vs ChessAI)
bool bMultiPlayer();			//	Player selected Multiplayer game (local 1v1)

void vGameWin();				//	Called when player wins the game
void vGameLose();				//	Called when player loses the game (Only when vs ChessAI)

void vInputSanitization(std::string*);		//	Called inside vUsrInput to convert all UPPERCASE to lowercase, thus sanitizing user input
int iFromCharToInt(std::string, int*);		//	Called to convert player inputted STRING [A7] to its Integer board vaue [48]
char cFromIntToChar(unsigned int, int*);				//	Called to convert background Integer to player-readable STRING value

char cInputValidation();		//	Sanitizes and validates user input inside main() for selection Game Options		--	POSSIBLY UN-NEEDED if variable is passed to vUsrInput 
void printBoard();				//	Calls function to print board based off selected Game Options

void vUsrInput();				//	Called to convert input getline to commands

void vDebug();			//	Called after sucess or failure of command to preform the selected iDebugLevel options		
void vPause()			//	Called after a command is issued where the screen needs to pause to show user output and clears buffer
{
	std::cout << "\n\nPress any key to continue..." << std::endl;
	std::cin.get();
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	return;
}	//	END vPause()
void vGameInit()	//	INITALIZE brand new game
{
	// initalize debug files / necessary files
	//std::ifstream 
	if (!bRematch)
	{
		//std::error_code ec;
		//bool dirWrite = std::filesystem::create_directories("common/debug");	-	-	-	-	-	-	-	-	-	-	-		=	=	=	=	=	-	-	-	-	=	-=	-	=-	=	-	=-	=	-	=-	=	
		//if (!dirWrite) { dHistory.push_back("dirWrite: " + ec.message()); }
		for (int x = 0; x < 500; x++)
		{
			std::ifstream checkfiles("common/GN" + std::to_string(x), std::ios::in);	//	sets GameNumber based on GN files
			if (!checkfiles.is_open())
			{
				GameNumber = x;
				break;
			}
			else
			{
				checkfiles.close();
			}
		}
	dHistory.push_back("Current game number is " + std::to_string(GameNumber) + ".");
	}
	for (int i = 0; i < iBoardSize; i++)
	{
		iBoard[i] = ' ';
		CheckBoard[i] = ' ';
		sBoard[i] = " ";

		CheckmateBoard[i] = ' ';
		cMoveBoard[i] = ' ';
	}
	sSidePrint.push_back("\tGame Commands\t\tM O V E  H I S T O R Y");
	sSidePrint.push_back("\thelp\t\t");
	sSidePrint.push_back("\tmove 'from' 'to'");
	sSidePrint.push_back("\tmcheck 'from'\t");
	//	SetPiece (PieceType , IsWhite , PieceNumber)

	ChessLogic_H::vPieceInit();

	CurrentColorIsWhite = true;	//	Reset game variables
	wMoves = 0;								//	UN-NEEDED if GLOBAL wMoves is removed	------------------
	bMoves = 0;								//	UN-NEEDED if GLOBAL bMoves is removed	------------------
	bWhiteKingInCheck = false;
	bBlackKingInCheck = false;
	bCheckmate = false;
	return;
}	//	END vGameInit()


int main( void )
{
	bool bGame = true;
	bool bGameWin = false;

	char cUsrInput = '0';	//	UN-NEEDED if cInputValidation is moved to vUsrInput	--------------
	char gameMode = '0';


	while (bGame)
	{
		bool InputFalse = true;

		for (int i = 0; i < mHistory.size(); i++)
		{
			mHistory.pop_back();
		}
		for (int i = 0; i < sSidePrint.size(); i++)
		{
			sSidePrint.pop_back();
		}
		if (!bRematch)
		{
			std::cout << "Weclome to DA Console Chess!" << std::endl;
			std::cout << "Is the below text readable? (Yes/No)" << std::endl;
			std::cout << "\t\033[4;31mT\033[32me\033[34ms\033[37mt\033[0m" << std::endl;

			std::cout << "This choice will effect the entire game." << std::endl;
			cUsrInput = cInputValidation();
			if (cUsrInput == 'y' || cUsrInput == 'Y')
			{
				bGraphics = true;
			}
			else
			{
				bGraphics = false;
			}
			dHistory.push_back("INFO: bGrapics is " + std::to_string(bGraphics));


			while (InputFalse)
			{
				if (bGraphics)
				{
					std::cout << "Would you like a \033[4mS\033[0mmall or \033[4mL\033[0marge game board?" << std::endl;
				}
				else
				{
					std::cout << "Would you like a Small or Large game board?" << std::endl;
				}
				cUsrInput = cInputValidation();
				if (cUsrInput == 'l')
				{
					boardType = 'l';
					dHistory.push_back("INFO: boardType: 'l'");
					InputFalse = false;
				}
				else if (cUsrInput == 's')
				{
					boardType = 's';
					dHistory.push_back("INFO: boardType: 's'");
					InputFalse = false;
				}
				else
				{
					std::cerr << "You need to enter either S or L!" << std::endl;
					dHistory.push_back("ERR: Invalid boardType.");
				}
			}	//	END InputFalse
		}
		vGameInit();
		dHistory.push_back("Game Initalized.");
		if (!bRematch)
		{
			InputFalse = true;
			while (InputFalse)
			{
				if (bGraphics)
				{
					std::cout << "Would you like \033[4ms\033[0mingle or \033[4mm\033[0multiplayer?" << std::endl;
				}
				else
				{
					std::cout << "Would you like Single or Multiplayer?" << std::endl;
				}
				cUsrInput = cInputValidation();
				if (cUsrInput == 's' || cUsrInput == 'S' || cUsrInput == '1')
				{
					gameMode = 's';
					dHistory.push_back("INFO: gameMode: 's'");
					InputFalse = false;
				}
				else if (cUsrInput == 'm' || cUsrInput == 'M' || cUsrInput == '2')
				{
					gameMode = 'm';
					dHistory.push_back("INFO: gameMode: 'm'");
					InputFalse = false;
				}
				else
				{
					std::cerr << "That gamemode doesn't exist!\n\n" << std::endl;
					dHistory.push_back("ERR: Invalid gameMode.");
				}
			}	//	END InputFalse
		}
		bRematch = false;
		if (gameMode == 's')	//	SINGLE PLAYER
		{
			bGameWin = bSinglePlayer();
		}
		if (gameMode == 'm')	//	MULTI PLAYER
		{
			bGameWin = bMultiPlayer();
		}

		if (!bRematch)
		{
			std::cout << "Would you like to keep playing?" << std::endl;
			cUsrInput = cInputValidation();
			if (cUsrInput == 'y' || cUsrInput == 'Y')
			{
				bGame = true;
			}
			else
			{
				std::ofstream SaveGame("common/GN" + std::to_string(GameNumber), std::ios::out);
				if (SaveGame.is_open())
				{
					SaveGame << "Thanks for playing!\n";
					SaveGame << "I should add some cool stuff here...\n";
					SaveGame.close();
				}

				std::cout << "Thanks for playing!\n\n" << std::endl;
				bGame = false;
			}
		}

	}	//	END of GAME LOOP

	return 0;

}	//	END OF MAIN



char cInputValidation()		//	---------- UN-NEEDED if moved to vUsrInput()
{
	char choice = '0';
	std::cin >> std::setw(1) >> choice;


	while (!std::cin.good())	// error check
	{
		std::cout << "Invalid Input! Try again." << std::endl;

		//	clear stream
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		std::cout << "Enter your choice again." << std::endl;
		std::cin >> std::setw(1) >> choice;

	}
	//clear stream
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	return choice;
}	//	END cInputValidation()		//		END cInputValidation()		//		END cInputValidation()

void vDebug()
{
	if (iDebugLevel == 1)
	{
		for (unsigned int i = 0; i < dHistory.size(); i++)
		{
			std::cout << "\t" << i << ".\t" << dHistory.at(i) << std::endl;
		}
	}
	else if (iDebugLevel == 2)
	{
		std::ofstream debugmain;
		debugmain.open("common/debug/GN" + std::to_string(GameNumber) + " DEBUG", std::fstream::app);
		if (debugmain.is_open())
		{
			for (unsigned int i = dHistoryReadNumber; i < dHistory.size(); i++)
			{
				debugmain << "\t" << i << ".\t" << dHistory.at(i) << std::endl;
				dHistoryReadNumber = i;
			}
			debugmain.close();
			dHistoryNumber++;
		}
		else
		{
			dHistory.push_back("ERR: Failed to open debug file");
			std::cerr << "Failed to open debug file!" << std::endl;
		}
	}
	else if (iDebugLevel == 3)
	{
		std::ofstream debugmain;
		debugmain.open("common/debug/main.dbg");
		if (debugmain.is_open())
		{
			for (unsigned int i = 0; i < dHistory.size(); i++)
			{
				std::cout << "\t" << i << ".\t" << dHistory.at(i) << std::endl;
				debugmain << "\t" << i << ".\t" << dHistory.at(i) << std::endl;
			}
			debugmain.close();
		}
		else
		{
			dHistory.push_back("ERR: Failed to open debug file");
			std::cerr << "Failed to open debug file!" << std::endl;
		}
	}
}	//		END vDebug()		//			END vDebug()			//			END vDebug()

bool bSinglePlayer()		//	--------------------------	SINGLE PLAYER GAME	-------------------------------------------------------	SINGLE PLAYER GAME --------------------------
{
	bGameStatus = true;
	int GameStatusLoop = 0;
	while (bGameStatus == true)
	{
		dHistory.push_back("--------------------------------------------------- Loop number: " + std::to_string(GameStatusLoop) + " ----------------------------");
		dHistory.push_back("bGameStatus loop: " + std::to_string(GameStatusLoop));
		printBoard();
		std::cout << "\n\tEnter a command > ";
		vUsrInput();
		GameStatusLoop++;
		vDebug();
		if (bCheckmate == true)
		{
			printBoard();
			if (bWhiteKingInCheck == true)
			{
				vGameLose();
			}
			else if (bBlackKingInCheck == true)
			{
				vGameWin();
			}
			bGameStatus = false;
		}
		
	}
	dHistory.push_back("Game Ended on loop " + std::to_string(GameStatusLoop));
	return 0;
}	//		END bSinglePlayer()			//		END bSinglePlayer()			//		END bSingplePlayer()

bool bMultiPlayer()			//	--------------------------	MULTI-PLAYER GAME	-------------------------------------------------------	MULTI-PLAYER GAME --------------------------
{
	bGameStatus = true;
	int GameStatusLoop = 0;
	while (bGameStatus == true)
	{
		dHistory.push_back("--------------------------------------------------- Loop number: " + std::to_string(GameStatusLoop) + " -----------------------------");
		dHistory.push_back("bGameStatus loop: " + std::to_string(GameStatusLoop));
		printBoard();
		std::cout << "\n\tEnter a command > ";
		vUsrInput();
		GameStatusLoop++;
		vDebug();
		if (bCheckmate == true)
		{
			printBoard();
			vGameWin();
			bGameStatus = false;
		}
	}
	dHistory.push_back("Game Ended on loop " + std::to_string(GameStatusLoop));
	return 0;
}	//		END bMultiPlayer()			//			END bMultiPlayer()			//		END bMultiPlayer()

void vGameWin()		//-------------vGameWin()------------------//-------------vGameWin()------------------//-------------vGameWin()------------------
{
	std::cout << std::endl;
	if (bWhiteKingInCheck == true)
	{
		std::cout << "Black ";
	}
	else if (bBlackKingInCheck == true)
	{
		std::cout << "White ";
	}
	std::cout << "W O N the game!" << std::endl;
}	//		END vGameWin()			//			END vGameWin()			//		END vGameWin()
void vGameLose()	//-------------vGameLose()------------------//-------------vGameLose()------------------//-------------vGameLose()------------------
{
	std::cout << std::endl;

	if (bWhiteKingInCheck == true)
	{
		std::cout << "Black ";
	}
	else if (bBlackKingInCheck == true)
	{
		std::cout << "White ";
	}
	std::cout << "lost the game!" << std::endl;
	std::cout << "My AI is too good for ya!" << std::endl;
}	//		END vGameLose()			//			END vGameLose()			//		END vGameLose()

void vUsrInput()	//-------------vUsrInput()------------------//-------------vUsrInput()------------------//-------------vUsrInput()------------------
{

	std::string cOne;
	std::string cTwo;
	std::string cThree;
	std::string input;
	int iEndInput = 0;

	std::getline(std::cin, input);
	dHistory.push_back("User input is: \"" + input + "\"");
	size_t lastPhrase = (input.find_last_of(' '));
	size_t nLastPhrase = (input.find_last_of(' '));

	if (!input.empty())
	{
		if (lastPhrase > input.size())
		{
			cThree = input;
			iEndInput = 1;
			input.erase();
		}
		else
		{
			cThree = input.substr(lastPhrase + 1, input.size());

			while (nLastPhrase == lastPhrase)
			{
				input.pop_back();
				nLastPhrase = (input.find_last_of(' '));
			}
		}
		vInputSanitization(&cThree);
	}
	else
	{
		sErrorMsg = "You didn't type anything!";
		return;
	}

	if (!input.empty())
	{
		lastPhrase = (input.find_last_of(' '));


		if (lastPhrase > input.size())
		{
			cTwo = input;
			iEndInput = 2;
			input.erase();
		}
		else
		{
			cTwo = input.substr(lastPhrase + 1, input.size());

			while (nLastPhrase == lastPhrase)
			{
				input.pop_back();
				nLastPhrase = (input.find_last_of(' '));
			}
		}
		vInputSanitization(&cTwo);
	}
	else
	{
		cOne = cThree;
		cTwo = "";
		cThree = "";

	}

	if (!input.empty())
	{
		lastPhrase = (input.find_last_of(' '));


		if (lastPhrase > input.size())
		{
			cOne = input;
			input.erase();
		}
		else
		{
			cOne = input;

			while (nLastPhrase == lastPhrase)
			{
				input.pop_back();
				nLastPhrase = (input.find_last_of(' '));
			}

		}
		vInputSanitization(&cOne);
	}
	else
	{
		if (iEndInput != 1)
		{
			cOne = cTwo;
			cTwo = cThree;
			cThree = "";
		}
	}

	dHistory.push_back("INFO: cOne: \"" + cOne + "\"\tcTwo: \"" + cTwo + "\"\tcThree: \"" + cThree + "\".");

	if (cOne == "exit")			//		BEGIN COMMANDS			//			BEGIN COMMANDS			//		BEGIN COMMANDS		//
	{
		std::cout << "Are you sure you want to end this game?" << std::endl;
		char input = cInputValidation();
		if (input == 'n' || input == 'N')
		{
			return;
		}
		else
		{
			bGameStatus = false;
			return;
		}

	}
	else if (cOne == "help")
	{
		if (cTwo == "")
		{
			dHistory.push_back("command HELP issued.");
			std::cout << "\n\n\t\tFull list of commands:" << std::endl;
			std::cout << "\t    the ' character indicates a variable input, like A5 or a command" << std::endl;
			std::cout << "\tcheckmate\tChecks for checkmate, and ends the match" << std::endl;
			std::cout << "\tcolor\t\tChanges piece color on board" << std::endl;
			std::cout << "\tdebug\t\tControls debug settings" << std::endl;
			std::cout << "\texit\t\tExit's to the main menu, and loses all game progress" << std::endl;
			std::cout << "\thelp 'command'\tLists all commands, and provides context for commmands" << std::endl;
			std::cout << "\thistory\t\tPrints full game history to screen" << std::endl;
			std::cout << "\tmove 'from' to'\tMoves the 'from' piece to the 'to' location. Example: move A7 A5." << std::endl;
			std::cout << "\tcmove 'from'\tCheck Move - Shows the valid moves of the 'from' piece. Example: cmove A7" << std::endl;
			std::cout << "\trematch\t\tEnds the current match and starts a new one." << std::endl;
			std::cout << "\twhat\t\t(debug) Says name of piece at current location on all boards" << std::endl;
		}
		else if (cTwo == "checkmate")
		{
			dHistory.push_back("command HELP CHECKMATE issued.");
			std::cout << "\n\n\t\t'checkmate' Command help:" << std::endl;
			std::cout << "\tThe 'checkmate' command is used to check for checkmate, and end the current match." << std::endl;
			std::cout << "\tThe syntax for the checkmate command is as follows:" << std::endl;
			std::cout << "\t\tcheckmate" << std::endl;
			std::cout << "\tI believe in you." << std::endl;
		}
		else if (cTwo == "color")
		{
			if (bGraphics == true)
			{
				if (cThree == "options")
				{
					dHistory.push_back("command HELP COLOR OPTIONS issued.");
					std::cout << "\tblack tile:\twhite tile:" << std::endl;
					std::cout << "\t1.\033[1;40;31m red		\033[0m|\t\033[1;47;31mred		\033[0m" << std::endl;
					std::cout << "\t2.\033[1;40;32m green	\033[0m|\t\033[1;47;32mgreen	\033[0m" << std::endl;
					std::cout << "\t3.\033[1;40;33m yellow	\033[0m|\t\033[1;47;33myellow	\033[0m" << std::endl;
					std::cout << "\t4.\033[1;40;34m blue	\033[0m\t|\t\033[1;47;34mblue		\033[0m" << std::endl;
					std::cout << "\t5.\033[1;40;35m magenta	\033[0m|\t\033[1;47;35mmagenta	\033[0m" << std::endl;
					std::cout << "\t6.\033[1;40;36m cyan	\033[0m\t|\t\033[1;47;36mcyan		\033[0m" << std::endl;
					std::cout << "\t7.\033[1;40;91m bRed	\033[0m\t|\t\033[1;47;91mbRed		\033[0m" << std::endl;
					std::cout << "\t8.\033[1;40;92m bGreen	\033[0m|\t\033[1;47;92mbGreen	\033[0m" << std::endl;
					std::cout << "\t9.\033[1;40;93m bYellow	\033[0m|\t\033[1;47;93mbYellow	\033[0m" << std::endl;
					std::cout << "\t10.\033[1;40;94m bBlue	\033[0m|\t\033[1;47;94mbBlue	\033[0m" << std::endl;
					std::cout << "\t11.\033[1;40;95m bMagenta\033[0m\t|\t\033[1;47;95mbMagenta	\033[0m" << std::endl;
					std::cout << "\t12.\033[1;40;96m bCyan	\033[0m|\t\033[1;47;96mbCyan	\033[0m" << std::endl;
				}
				else
				{
					dHistory.push_back("command HELP COLOR issued.");
					std::cout << "\n\n\t\t'color' Command help:" << std::endl;
					std::cout << "\tThe 'color' command is used to change the color of the pieces to your liking." << std::endl;
					std::cout << "\tThe syntax for the color command is as follows:" << std::endl;
					std::cout << "\t\tcolor color to change to" << std::endl;
					std::cout << "\t\tcolor      blue" << std::endl;
					std::cout << "\tType 'help color options' to view all color options!" << std::endl;
				}
			}
			else
			{
				std::cerr << "Your current configuration doesn't support color changing!" << std::endl;
				dHistory.push_back("ERR: Color cannot be changed while bGraphics is false!");
			}
		}
		else if (cTwo == "debug")
		{
			dHistory.push_back("command HELP DEBUG issued.");
			std::cout << "\n\n\t\t'debug' Command help:" << std::endl;
			std::cout << "\tThe 'debug' command is used to change the debug output levels" << std::endl;
			std::cout << "\tThe syntax for the checkmate command is as follows:" << std::endl;
			std::cout << "\t\tdebug level (default is 0)" << std::endl;
			std::cout << "\t\tdebug  0" << std::endl;
			std::cout << "\tDebug level 0 will output nothing." << std::endl;
			std::cout << "\tDebug level 1 will output to the command line." << std::endl;
			std::cout << "\tDebug level 2 will output to a file." << std::endl;
			std::cout << "\tDebug level 3 will output to both" << std::endl;

			std::cout << "\tTHIS HASN'T BEEN IMPLEMENTED YET" << std::endl;

		}
		else if (cTwo == "exit")
		{
			dHistory.push_back("command HELP EXIT issued.");
			std::cout << "\n\n\t\t'exit' Command help:" << std::endl;
			std::cout << "\tThe 'exit' command is used to return to the main menu, and end the current match." << std::endl;
			std::cout << "\tThe syntax for the exit command is as follows:" << std::endl;
			std::cout << "\t\texit" << std::endl;
			std::cout << "\tI believe in you." << std::endl;
		}
		else if (cTwo == "history")
		{
			dHistory.push_back("command HELP HISTORY issued.");
			std::cout << "\n\n\t\t'history' Command help:" << std::endl;
			std::cout << "\tThe 'history' command is used to display all events of the current match" << std::endl;
			std::cout << "\tThe syntax for the move command is as follows:" << std::endl;
			std::cout << "\t\thistory option" << std::endl;
			std::cout << "\t\thistory debug" << std::endl;
			std::cout << "\t\thistory save" << std::endl;
			std::cout << "\t\tSpecial Uses:" << std::endl;
			std::cout << "\t'Save to Files'" << std::endl;
			std::cout << "\t in the future I'll write what is supposed to be here." << std::endl;
		}
		else if (cTwo == "move")
		{
			dHistory.push_back("command HELP MOVE issued.");
			std::cout << "\n\n\t\t'move' Command help:" << std::endl;
			std::cout << "\tThe 'move' command is used to move any piece from one location to another" << std::endl;
			std::cout << "\tThe syntax for the move command is as follows:" << std::endl;
			std::cout << "\t\tmove 'from'\t'to'" << std::endl;
			std::cout << "\t\tmove   A7 \tA5" << std::endl;
			std::cout << "\tThis moves the piece at location 'A7' to the 'A5'" << std::endl;
			std::cout << "\tThis command has input validation, and will ensure your move is legal." << std::endl;
			std::cout << "\t\tSpecial Uses:" << std::endl;
			std::cout << "\t'castling'" << std::endl;
			std::cout << "\t\tAliases: mov, mv" << std::endl;
			std::cout << "\t in the future I'll write what is supposed to be here." << std::endl;
		}
		else if (cTwo == "mcheck" || cTwo == "cmove")
		{
			dHistory.push_back("command HELP CMOVE issued.");
			std::cout << "\n\n\t\t'cmove' Command help:" << std::endl;
			std::cout << "\tThe 'cmove' or 'check move' command is used to check the available moves for the selected piece." << std::endl;
			std::cout << "\tThe syntax for the cmove command is as follows:" << std::endl;
			std::cout << "\t\tcmove 'from'" << std::endl;
			std::cout << "\t\tcmove   A7" << std::endl;
			std::cout << "\tThis will show you all of the available moves for the piece located at A7." << std::endl;
			std::cout << "\t\tSpecial Uses:" << std::endl;
			std::cout << "\tGetting a fresh view of the board" << std::endl;
		}
		else if (cTwo == "rematch")
		{
			dHistory.push_back("command HELP REMATCH issued.");
			std::cout << "\n\n\t\t'rematch' Command help:" << std::endl;
			std::cout << "\tThe 'rematch' command is used to end the current game, and start a new one." << std::endl;
			std::cout << "\tThe syntax for the rematch command is as follows:" << std::endl;
			std::cout << "\t\trematch" << std::endl;
			std::cout << "\tI Believe in you." << std::endl;
		}
		else if (cTwo == "what")
		{
			dHistory.push_back("command HELP WHAT issued.");
			std::cout << "\n\n\t\t'what' Command help:" << std::endl;
			std::cout << "\tThe 'what' command is a debug command to show what values are at specific locations on the board" << std::endl;
			std::cout << "\tThe syntax for the what command is as follows:" << std::endl;
			std::cout << "\t\twhat location" << std::endl;
			std::cout << "\t\twhat    c0" << std::endl;
			std::cout << "\tThat's it." << std::endl;
		}
		else
		{
			dHistory.push_back("command HELP \"" + cTwo + "\" was not found.");
			sErrorMsg = "Your command wasn't found.";
			return;
		}
		dHistory.push_back("command HELP " + cTwo + " issued.");
		vPause();
		return;
	}
	else if (cOne == "color")
	{
		if (bGraphics)
		{
			if (cTwo == "")
			{
				dHistory.push_back("command COLOR issued.");
				std::cout << "Current Color is: \033[" << iPieceColor << "m!\033[0m or " << iPieceColor << "." << std::endl;
				return;
			}
			else if (cTwo == "r" || cTwo == "red" || cTwo == "31" || cTwo == "1")
			{
				iPieceColor = 31;
			}
			else if (cTwo == "g" || cTwo == "green" || cTwo == "32" || cTwo == "2")
			{
				iPieceColor = 32;
			}
			else if (cTwo == "y" || cTwo == "yellow" || cTwo == "33" || cTwo == "3")
			{
				iPieceColor = 33;
			}
			else if (cTwo == "b" || cTwo == "blue" || cTwo == "34" || cTwo == "4")
			{
				iPieceColor = 34;
			}
			else if (cTwo == "m" || cTwo == "magenta" || cTwo == "35" || cTwo == "5")
			{
				iPieceColor = 35;
			}
			else if (cTwo == "c" || cTwo == "cyan" || cTwo == "36" || cTwo == "6")
			{
				iPieceColor = 36;
			}
			else if (cTwo == "br" || cTwo == "bred" || cTwo == "91" || cTwo == "7")
			{
				iPieceColor = 91;
			}
			else if (cTwo == "bg" || cTwo == "bgreen" || cTwo == "92" || cTwo == "8")
			{
				iPieceColor = 92;
			}
			else if (cTwo == "by" || cTwo == "byellow" || cTwo == "93" || cTwo == "9")
			{
				iPieceColor = 93;
			}
			else if (cTwo == "bb" || cTwo == "bblue" || cTwo == "94" || cTwo == "10")
			{
				iPieceColor = 94;
			}
			else if (cTwo == "bm" || cTwo == "bmagenta" || cTwo == "95" || cTwo == "11")
			{
				iPieceColor = 95;
			}
			else if (cTwo == "bc" || cTwo == "bcyan" || cTwo == "96" || cTwo == "12")
			{
				iPieceColor = 96;
			}
			else
			{
				std::cerr << "That color could not be found!" << std::endl;
				std::cout << "Type 'help color options' to see all available colors." << std::endl;
				vPause();
				return;
			}
			std::cout << "Piece color changed to \033[" << iPieceColor << "m!\033[0m" << std::endl;
			dHistory.push_back("command COLOR issued as " + std::to_string(iPieceColor));
			vPause();
			return;
		}
		else
		{
			sErrorMsg = "Your settings don't allow for color";
		}
	}
	else if (cOne == "checkmate")
	{
		dHistory.push_back("command CHECKMATE issued.");
		//	the stuff for checkmate should go here	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	
		sErrorMsg = "That command hasn't been implemented yet.";
		return;
	}
	else if (cOne == "debug")
	{
		dHistory.push_back("command DEBUG issued.");
		if (cTwo == "0" || cTwo == "zero")
		{
			iDebugLevel = 0;
			dHistory.push_back("iDebugLevel set to 0.");
			std::cout << "Debug level set to 0!" << std::endl;
		}
		else if (cTwo == "1" || cTwo == "one")
		{
			iDebugLevel = 1;
			dHistory.push_back("iDebugLevel set to 1.");
			std::cout << "Debug level set to 1" << std::endl;
		}
		else if (cTwo == "2" || cTwo == "two")
		{
			iDebugLevel = 2;
			dHistory.push_back("iDebugLevel set to 2.");
			std::cout << "Debug level set to 2" << std::endl;
		}
		else if (cTwo == "3" || cTwo == "three")
		{
			iDebugLevel = 3;
			dHistory.push_back("iDebugLevel set to 3.");
			std::cout << "Debug level set to 3" << std::endl;
		}
		else if (cTwo == "print")
		{
			if (cThree == "check")
			{
				std::cout << "\t      A  B  C  D  E  F  G  H  "<< std::endl;
				std::cout << "\t    ==========================";
				for (int height = 0; height < iBoardHeight; height++)
				{
					std::cout << std::endl;
					std::cout << "\t " << height + 1 << " | ";
					for (int width = 0; width < iBoardWidth; width++)
					{
						int iBoardPrint = ((width + 1) + (height * iBoardHeight)) - 1;
						std::cout << " " << CheckBoard[iBoardPrint] << " ";
					}
				}
			}

		}
		else
		{
			sErrorMsg = "Improper Debug level";
		}
		vPause();
		return;
	}	//	end DEBUG
	else if (cOne == "history" || cOne == "hist" || cOne == "h")
	{
		if (cTwo == "")
		{
			if (mHistory.size() != 0)
			{
				for (long unsigned int i = 0; i < mHistory.size(); i++)
				{
					std::cout << "\t" << i + 1 << ".\t" << mHistory.at(i) << std::endl;
				}
				dHistory.push_back("command HISTORY issued successfully.");
			}
			else
			{
				dHistory.push_back("ERR: No move history to display!");
				sErrorMsg = "You have no history to display.";
			}
		}
		else if (cTwo == "save" || cTwo == "s")
		{
			dHistory.push_back("command HISTORY SAVE issued.");

			std::ofstream historymain;
			historymain.open("GN" + std::to_string(GameNumber) + " HN " + std::to_string(mHistoryNumber));

			if (historymain.is_open())
			{
				for (unsigned int i = mHistoryReadNumber; i < mHistory.size(); i++)
				{
					historymain << "\t" << i << ".\t" << mHistory.at(i) << std::endl;
					mHistoryReadNumber = i;
				}
				historymain.close();
				mHistoryNumber++;
			}
			else
			{
				dHistory.push_back("ERR: Failed to open history file. GN" + std::to_string(GameNumber) + " HN " + std::to_string(mHistoryNumber));
				sErrorMsg = "Failed to open file!";
				return;
			}
			return;
		}
		else if (cTwo == "clear" || cTwo == "c")
		{
			if (mHistory.size() != 0)
			{
				dHistory.push_back("command HISTORY CLEAR issued.");
				for (size_t j = (mHistory.size()); j > 0; j--)
				{
					mHistory.pop_back();
				}
				mHistoryReadNumber = 0;
			}
			else
			{
				dHistory.push_back("ERR: No move history to clear!");
				sErrorMsg = "You have no history to clear!";
				vPause();
				return;
			}
		}
		else if (cTwo == "debug")
		{
			if (cThree == "save" || cThree == "s")
			{
				std::cout << "To enable saving debug history to a file, use the command 'debug 2' or 'help debug'" << std::endl;
				return;
			}
			else if (cThree == "clear" || cThree == "c")
			{
				if (dHistory.size() != 0)
				{
					dHistory.push_back("command HISTORY DEBUG CLEAR issued.");
					for (size_t j = (dHistory.size()); j > 0; j--)
					{
						dHistory.pop_back();
					}
					dHistoryReadNumber = 0;
					dHistory.push_back("command HISTORY DEBUG CLEAR issued sucessfully.");
				}
				else
				{
					sErrorMsg = "You have no history to clear!";
					dHistory.push_back("ERR: No debug history to clear!");
					return;
				}
			}
			else
			{
				dHistory.push_back("command HISTORY DEBUG issued.");
				for (long unsigned int i = 0; i < dHistory.size(); i++)
				{
					std::cout << "\t" << i + 1 << ".\t" << dHistory.at(i) << std::endl;
				}
			}
		}
		vPause();
		return;
	}
	else if (cOne == "mcheck" || cOne == "cmove" || cOne == "cmoves")
	{
		int iAtIndex = 0;
		if (cTwo.size() == 2)
		{
			int iMovefWidth = 0;
			int iMovefHeight = 0;

			iMovefWidth = iFromCharToInt(cTwo, &iAtIndex);
			if (iAtIndex == 0)
			{
				iMovefHeight = cTwo.at(1) - '0';
			}
			else if (iAtIndex == 1)
			{
				iMovefHeight = cTwo.at(0) - '0';
			}
			iMoveFrom = ((iMovefWidth)+((iMovefHeight - 1) * iBoardHeight)) - 1;
			dHistory.push_back("command CMOVE issued.\tiMoveFrom: " + std::to_string(iMoveFrom) + " from Width: " + std::to_string(iMovefWidth) + " and Height: " + std::to_string(iMovefHeight));

			ChessLogic_H::vMoveCheck();
			return;
		}
		else
		{
			dHistory.push_back("ERR: command CMOVE issued improperly.");
			sErrorMsg = "Improper use of the cmove command.";
			return;
		}
	}
	else if (cOne == "rematch")
	{
		dHistory.push_back("command REMATCH issued.");
		bGameStatus = false;
		bRematch = true;
		return;
	}
	else if (cOne == "move" || cOne == "mov" || cOne == "mv" || cOne == "m")
	{
	dHistory.push_back("command MOVE issued.");
		int iMovefWidth = 0;
		int iMovefHeight = 0;
		int iMovetWidth = 0;
		int iMovetHeight = 0;
		int iAtIndex = 0;

		if (cTwo.size() == 2 && cThree.size() == 2)
		{
			iMovefWidth = iFromCharToInt(cTwo, &iAtIndex);
			if (iAtIndex == 0)
			{
				iMovefHeight = cTwo.at(1) - '0';
			}
			else if (iAtIndex == 1)
			{
				iMovefHeight = cTwo.at(0) - '0';
			}
			iMoveFrom = ((iMovefWidth)+((iMovefHeight - 1) * iBoardHeight)) - 1;

			iMovetWidth = iFromCharToInt(cThree, &iAtIndex);
			if (iAtIndex == 0)
			{
				iMovetHeight = cThree.at(1) - '0';
			}
			else if (iAtIndex == 1)
			{
				iMovetHeight = cThree.at(0) - '0';
			}
			iMoveTo = ((iMovetWidth)+((iMovetHeight - 1) * iBoardHeight)) - 1;

			dHistory.push_back("INFO: iMoveFrom: " + std::to_string(iMoveFrom) + " from Width: " + std::to_string(iMovefWidth) + " and Height: " + std::to_string(iMovefHeight) + "\tiMoveTo: " + std::to_string(iMoveTo) + " from Width: " + std::to_string(iMovetWidth) + " and Height: " + std::to_string(iMovetHeight));

			ChessLogic_H::bSearchObj();	//	call function for moving piece
			return;
		}
		else
		{
			dHistory.push_back("ERR: Invalid move location " + cTwo + " to " + cThree + ".");
			sErrorMsg = "Invalid move location.";
			return;
		}
	}	//	END IF command is "move" 
	else if (cOne == "what")
	{
		dHistory.push_back("command WHAT issued");
		int iCheckLocation = 0;
		int iCheckWidth = 0;
		int iCheckHeight = 0;
		int iAtIndex = 0;

		if (cTwo.size() != 2)
		{
			dHistory.push_back("ERR: Invalid board location");
			sErrorMsg = "Invalid location";
			return;
		}

		iCheckWidth = iFromCharToInt(cTwo, &iAtIndex);
		if (iAtIndex == 0)
		{
			iCheckHeight = cTwo.at(1) - '0';
		}
		else if (iAtIndex == 1)
		{
			iCheckHeight = cTwo.at(0) - '0';
		}

		iCheckLocation = ((iCheckWidth)+((iCheckHeight - 1) * iBoardHeight)) - 1;
		std::cout << "Location: " << iCheckLocation << " on board:" << std::endl;
		std::cout << "iBoard: \"" << iBoard[iCheckLocation] << "\"" << std::endl;
		std::cout << "sBoard: \"" << sBoard[iCheckLocation] << "\"" << std::endl;
		std::cout << "CheckBoard: \"" << CheckBoard[iCheckLocation] << "\"" << std::endl;
		std::cout << "cMoveCheck: \"" << cMoveBoard[iCheckLocation] << "\"" << std::endl;
		vPause();
		return;
	}


	//clear stream
	dHistory.push_back("ERR: Unknown command issued...");
	std::cerr << "\tUnknown command!" << std::endl;
	std::cout << "\tTry typing 'help' for instructions." << std::endl;
	vPause();

	return;


}		//		END vUsrInput()			//			END vUsrInput()			//		END vUsrInput()		//

void vInputSanitization(std::string *cInput)	//----------vInputSanitization()----------//----------vInputSanitization----------
{
	std::string cValue = *cInput;
	std::string tmpString = "";
	for (int i = 0; i < cValue.size(); i++)
	{
		if (cValue.at(i) == 'A')
			tmpString = tmpString + 'a';
		else if (cValue.at(i) == 'B')
			tmpString = tmpString + 'b';
		else if (cValue.at(i) == 'C')
			tmpString = tmpString + 'c';
		else if (cValue.at(i) == 'D')
			tmpString = tmpString + 'd';
		else if (cValue.at(i) == 'E')
			tmpString = tmpString + 'e';
		else if (cValue.at(i) == 'F')
			tmpString = tmpString + 'f';
		else if (cValue.at(i) == 'G')
			tmpString = tmpString + 'g';
		else if (cValue.at(i) == 'H')
			tmpString = tmpString + 'h';
		else if (cValue.at(i) == 'I')
			tmpString = tmpString + 'i';
		else if (cValue.at(i) == 'J')
			tmpString = tmpString + 'j';
		else if (cValue.at(i) == 'K')
			tmpString = tmpString + 'k';
		else if (cValue.at(i) == 'L')
			tmpString = tmpString + 'l';
		else if (cValue.at(i) == 'M')
			tmpString = tmpString + 'm';
		else if (cValue.at(i) == 'N')
			tmpString = tmpString + 'n';
		else if (cValue.at(i) == 'O')
			tmpString = tmpString + 'o';
		else if (cValue.at(i) == 'P')
			tmpString = tmpString + 'p';
		else if (cValue.at(i) == 'Q')
			tmpString = tmpString + 'q';
		else if (cValue.at(i) == 'R')
			tmpString = tmpString + 'r';
		else if (cValue.at(i) == 'S')
			tmpString = tmpString + 's';
		else if (cValue.at(i) == 'T')
			tmpString = tmpString + 't';
		else if (cValue.at(i) == 'U')
			tmpString = tmpString + 'u';
		else if (cValue.at(i) == 'V')
			tmpString = tmpString + 'v';
		else if (cValue.at(i) == 'W')
			tmpString = tmpString + 'w';
		else if (cValue.at(i) == 'X')
			tmpString = tmpString + 'x';
		else if (cValue.at(i) == 'Y')
			tmpString = tmpString + 'y';
		else if (cValue.at(i) == 'Z')
			tmpString = tmpString + 'z';
		else
			tmpString = tmpString + cValue.at(i);
	}
	*cInput = tmpString;
}//		END vInputSanitization()			//			END vInputSanitization()			//		END vInputSanitization()

int iFromCharToInt(std::string cValue, int* iAtIndex)	//----------iFromCharToInt()----------//----------iFromCharToInt----------
{
	int iMoveWidth = 0;
	for (int i = 0; i < 2; i++)
	{
		char cCompare = cValue.at(i);
		if (cCompare == 'A' || cCompare == 'a')
		{
			*iAtIndex = i;
			iMoveWidth = 1;
		}
		else if (cCompare == 'B' || cCompare == 'b')
		{
			*iAtIndex = i;
			iMoveWidth = 2;
		}
		else if (cCompare == 'C' || cCompare == 'c')
		{
			*iAtIndex = i;
			iMoveWidth = 3;
		}
		else if (cCompare == 'D' || cCompare == 'd')
		{
			*iAtIndex = i;
			iMoveWidth = 4;
		}
		else if (cCompare == 'E' || cCompare == 'e')
		{
			*iAtIndex = i;
			iMoveWidth = 5;
		}
		else if (cCompare == 'F' || cCompare == 'f')
		{
			*iAtIndex = i;
			iMoveWidth = 6;
		}
		else if (cCompare == 'G' || cCompare == 'g')
		{
			*iAtIndex = i;
			iMoveWidth = 7;
		}
		else if (cCompare == 'H' || cCompare == 'h')
		{
			*iAtIndex = i;
			iMoveWidth = 8;
		}
	}
	return iMoveWidth;
}	//		END iFromCharToInt()			//			END iFromCharToInt()			//		END iFromCharToInt()
char cFromIntToChar(unsigned int cValue, int* cHeight)		//----------cFromIntToChar()----------//----------cFromIntToChar()----------
{
	char cWidth = '0';

	for (int i = 1; i <= iBoardHeight; i++)
	{
		if (cValue <= (i * iBoardHeight) - 1)
		{
			*cHeight = i;
		}
	}

	if (cValue % iBoardWidth == 0)
	{
		cWidth = 'A';
	}
	else if (cValue % iBoardWidth == 1)
	{
		cWidth = 'B';
	}
	else if (cValue % iBoardWidth == 2)
	{
		cWidth = 'C';
	}
	else if (cValue % iBoardWidth == 3)
	{
		cWidth = 'D';
	}
	else if (cValue % iBoardWidth == 4)
	{
		cWidth = 'E';
	}
	else if (cValue % iBoardWidth == 5)
	{
		cWidth = 'F';
	}
	else if (cValue % iBoardWidth == 6)
	{
		cWidth = 'G';
	}
	else if (cValue % iBoardWidth == 7)
	{
		cWidth = 'H';
	}
	return cWidth;
}//		END cFromIntToChar()			//			END cFromIntToChar()			//		END cFromIntToChar()

void printBoard()	//----------printBoard()----------//----------printBoard()----------//----------printBoard()----------
{	
	std::string sTurn;
	std::string sScore;
	dHistory.push_back("bMoveCheck: " + std::to_string(bMoveCheck) + "\twCheck: " + std::to_string(bWhiteKingInCheck) + "\tbCheck: " + std::to_string(bBlackKingInCheck));
	if (bGraphics == true)
	{
		sScore = "\tScore:\t \033[1;107;90mWHITE : " + std::to_string(iWhiteScore) + "\033[0m\t\033[1;100;97mBLACK : " + std::to_string(iBlackScore) + "\033[0m";
	}
	else
	{
		sScore = "\tScore:\t WHITE : " + std::to_string(iWhiteScore) + "\tBLACK : " + std::to_string(iBlackScore);
	}
	if (CurrentColorIsWhite == true)
	{
		if (bGraphics == true)
		{
			sTurn = "\033[1;107;90mWHITE\033[0m (Uppercase)";
		}
		else
		{
			sTurn = "WHITE (Uppercase)";
		}
		dHistory.push_back("INFO: CurrentColorIsWhite is TRUE");
	}
	else
	{
		if (bGraphics == true)
		{
			sTurn = "\033[1;100;97mBLACK\033[0m (lowercase)";
		}
		else
		{
			sTurn = "black (lowercase)";
		}
		dHistory.push_back("INFO: CurrentColorIsWhite is FALSE");
	}



	if (boardType == 'l')	//-	- - - - - - - - - boardType Large - - - - - - - - - - // - - - - - - - - - - - boardType Large - - - - - - - - - -
	{
		long unsigned int l = 0;
		bool whiteSpace = false;
		int iSpaceNum = 0;
		if (bGraphics == true) { std::cout << "\t\033[1;31m" << sErrorMsg << "\033[0m" << std::endl; }
		else { std::cout << "\t" << sErrorMsg << std::endl; }
		std::cout << "\tc u r r e n t  t u r n: " << sTurn << std::endl;
		std::cout << "\t       A    B    C    D    E    F    G    H    " << sScore << std::endl;
		std::cout << "\t    ========================================== "<< sSidePrint[0];

		for (int height = 0; height < iBoardHeight * 3; height++)
		{
			std::cout << std::endl;
			if (((height) / 3) % 2 == 0)
			{
				whiteSpace = true;
			}
			else
			{
				whiteSpace = false;
			}

			if ((height - 1) % 3 == 0)
			{
				std::cout << "\t " << (height / 3) + 1 << " | ";
			}
			else
			{
				std::cout << "\t   | ";
				iSpaceNum++;
			}

			for (int width = 0; width < iBoardWidth; width++)
			{
				int iBoardPrint = ((width + 1) + ((height / 3) * iBoardHeight)) - 1;

				if (whiteSpace == false)
				{

					if ((height - 1) % 3 == 0)
					{
						if (bGraphics == true)
						{
							if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
							{
								std::cout << "\033[1;41m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
							{
								std::cout << "\033[1;41m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x') && (iBoard[iBoardPrint] == ' '))
							{
								std::cout << "\033[1;42m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else
							{
								std::cout << "\033[1;47m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
						}
						else
						{
							if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
							{
								std::cout << "##" << iBoard[iBoardPrint] << "##";
							}
							else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
							{
								std::cout << "##" << iBoard[iBoardPrint] << "##";
							}
							else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x') && (iBoard[iBoardPrint] == ' '))
							{
								std::cout << "  *  ";
							}
							else
							{
								std::cout << "  " << iBoard[iBoardPrint] << "  ";
							}
						}
					}
					else
					{
						if (bGraphics == true)
						{
							if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
							{
								if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
										std::cout << "\033[1;41m \033[0m   \033[1;41m \033[0m";
									else
										std::cout << "\033[1;47m     \033[0m";
								}
								else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
										std::cout << "\033[1;41m \033[0m   \033[1;41m \033[0m";
									else
										std::cout << "\033[1;47m     \033[0m";
								}
								else
								{
									std::cout << "\033[1;47m     \033[0m";
								}
							}
							else
							{
								std::cout << "\033[1;47m     \033[0m";
							}
						}
						else
						{
							if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
							{
								if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
										std::cout << "*   *";
									else
										std::cout << "     ";
								}
								else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
										std::cout << "*   *";
									else
										std::cout << "     ";
								}
								else
								{
									std::cout << "     ";
								}
							}
							else
							{
								std::cout << "     ";
							}
						}
					}

					whiteSpace = true;
				}
				else if (whiteSpace == true)
				{
					if ((height - 1) % 3 == 0)
					{
						if (bGraphics == true)
						{
							if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
							{
								std::cout << "\033[1;41m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
							{
								std::cout << "\033[1;41m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x') && (iBoard[iBoardPrint] == ' '))
							{
								std::cout << "\033[1;42m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
							else
							{
								std::cout << "\033[1;40m  \033[1;" << iPieceColor << "m" << iBoard[iBoardPrint] << "  \033[0m";
							}
						}
						else
						{
							if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
							{
								std::cout << "##" << iBoard[iBoardPrint] << "##";
							}
							else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
							{
								std::cout << "##" << iBoard[iBoardPrint] << "##";
							}
							else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x') && (iBoard[iBoardPrint] == ' '))
							{
								std::cout << "- * -";
							}
							else
							{
								std::cout << "- " << iBoard[iBoardPrint] << " -";
							}
						}
					}
					else
					{
						if (bGraphics == true)
						{
							if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
							{
								if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
										std::cout << "\033[1;41m \033[0m   \033[1;41m \033[0m";
									else
										std::cout << "\033[1;40m     \033[0m";
								}
								else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
										std::cout << "\033[1;41m \033[0m   \033[1;41m \033[0m";
									else
										std::cout << "\033[1;40m     \033[0m";
								}
								else
								{
									std::cout << "\033[1;40m     \033[0m";
								}
							}
							else
							{
								std::cout << "\033[1;40m     \033[0m";
							}
						}
						else
						{
							if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
							{
								if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
										std::cout << "*---*";
									else
										std::cout << "-----";
								}
								else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
								{
									if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
										std::cout << "*---*";
									else
										std::cout << "-----";
								}
								else
								{
									std::cout << "-----";
								}
							}
							else
							{
								std::cout << "-----";
							}
						}
					}

					whiteSpace = false;
				}
				if (width == iBoardWidth)
				{
					std::cout << " |";
				}
			}
			if (height < iBoardHeight && height + 1 < sSidePrint.size())
				std::cout << sSidePrint[height + 1];
			else
			{
				std::cout << "\t\t\t";
			}

			if (height > 0)
			{
				if (mHistory.size() == 0)
				{
					std::cout << "\tMake a move!";
				}
				else if (mHistory.size() < iBoardHeight * 3)
				{
					if (l < mHistory.size())
					{
						std::cout << l + 1 << ". " << mHistory.at(l);
						l++;
					}
					else
					{
						std::cout << "\tMake a move!";
					}
				}
				else
				{
					int l = (mHistory.size() - iBoardHeight) + height;
					std::cout << "\t " << l + 1 << ". " << mHistory.at(l);
				}
			}

		}

		std::cout << std::endl;
		dHistory.push_back("Board printed sucessfully");
		for (unsigned int i = 0; i < iBoardSize; i++) cMoveBoard[i] = ' '; bMoveCheck = false;
		sErrorMsg = "";
		return;
	}	//                      END boardType Large                     //                     END boardType Large

	if (boardType == 's')	// - - - - - - - - - - boardType Small - - - - - - - - - - - - // - - - - - - - - - - - - - boardType Small - - - - - - - - - - - - -
	{
		long unsigned int l = 0;
		bool whiteSpace = false;
		std::cout << "\n\n\n\n" << std::endl;
		if (bGraphics == true) {std::cout << "\t\033[1;31m" << sErrorMsg << "\033[0m" << std::endl; }
		else {std::cout << "\t" << sErrorMsg << std::endl; }

		std::cout << "\tcurrent  turn: " << sTurn << std::endl;;
		std::cout << "\t      A  B  C  D  E  F  G  H   " << sScore << std::endl;
		std::cout << "\t    ========================== " << sSidePrint[0];

		for (int height = 0; height < iBoardHeight; height++)
		{
			if (height % 2 == 0)
			{
				whiteSpace = true;
			}
			else
			{
				whiteSpace = false;
			}

			std::cout << std::endl;
			std::cout << "\t " << height + 1 << " | ";
			for (int width = 0; width < iBoardWidth; width++)
			{
				int iBoardPrint = ((width + 1) + (height * iBoardHeight)) - 1;
				if (whiteSpace == false)
				{
					if (bGraphics == true)
					{
						if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
						{
							std::cout << "\033[1;41m\033[1;" << iPieceColor << "m";
						}
						else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
						{
							std::cout << "\033[1;41m\033[1;" << iPieceColor << "m";
						}
						else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
						{
							if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
									std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
								else
									std::cout << "\033[1;47m\033[1;" << iPieceColor << "m";
							}
							else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
									std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
								else
									std::cout << "\033[1;47m\033[1;" << iPieceColor << "m";
							}
							else
							{
								std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
							}
						}
						else
						{
							std::cout << "\033[1;47m\033[1;" << iPieceColor << "m";
						}
						std::cout << " " << iBoard[iBoardPrint] << " ";
						std::cout << "\033[0m";
					}
					else
					{
						if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
						{
							std::cout << "#" << iBoard[iBoardPrint] << "#";
						}
						else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
						{
							std::cout << "#" << iBoard[iBoardPrint] << "#";
						}
						else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
						{
							if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
									std::cout << "*" << iBoard[iBoardPrint] << "*";
								else
									std::cout << " " << iBoard[iBoardPrint] << " ";
							}
							else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
									std::cout << "*" << iBoard[iBoardPrint] << "*";
								else
									std::cout << " " << iBoard[iBoardPrint] << " ";
							}
							else
							{
								std::cout << " * ";
							}
						}
						else
						{
							std::cout << " " << iBoard[iBoardPrint] << " ";
						}
					}
					whiteSpace = true;
				}
				else if (whiteSpace == true)
				{
					if (bGraphics == true)
					{
						if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
						{
							std::cout << "\033[1;41m\033[1;" << iPieceColor << "m";
						}
						else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
						{
							std::cout << "\033[1;41m\033[1;" << iPieceColor << "m";
						}
						else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
						{
							if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
									std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
								else
									std::cout << "\033[1;40m\033[1;" << iPieceColor << "m";
							}
							else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
									std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
								else
									std::cout << "\033[1;40m\033[1;" << iPieceColor << "m";
							}
							else
							{
								std::cout << "\033[1;42m\033[1;" << iPieceColor << "m";
							}
						}
						else
						{
							std::cout << "\033[1;40m\033[1;" << iPieceColor << "m";
						}
						std::cout << " " << iBoard[iBoardPrint] << " ";
						std::cout << "\033[0m";
					}
					else
					{
						if (bWhiteKingInCheck == true && iBoard[iBoardPrint] == 'K')
						{
							std::cout << "#" << iBoard[iBoardPrint] << "#";
						}
						else if (bBlackKingInCheck == true && iBoard[iBoardPrint] == 'k')
						{
							std::cout << "#" << iBoard[iBoardPrint] << "#";
						}
						else if ((bMoveCheck == true) && (cMoveBoard[iBoardPrint] == 'x'))
						{
							if (iThisWhite == true && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'k' || iBoard[iBoardPrint] == 'q' || iBoard[iBoardPrint] == 'r' || iBoard[iBoardPrint] == 'b' || iBoard[iBoardPrint] == 'n' || iBoard[iBoardPrint] == 'p')
									std::cout << "*" << iBoard[iBoardPrint] << "*";
								else
									std::cout << "=" << iBoard[iBoardPrint] << "=";
							}
							else if (iThisWhite == false && iBoard[iBoardPrint] != ' ')
							{
								if (iBoard[iBoardPrint] == 'K' || iBoard[iBoardPrint] == 'Q' || iBoard[iBoardPrint] == 'R' || iBoard[iBoardPrint] == 'B' || iBoard[iBoardPrint] == 'N' || iBoard[iBoardPrint] == 'P')
									std::cout << "*" << iBoard[iBoardPrint] << "*";
								else
									std::cout << "=" << iBoard[iBoardPrint] << "=";
							}
							else
								std::cout << " * ";
						}
						else
						{
							std::cout << "=" << iBoard[iBoardPrint] << "=";
						}
						
					}
					whiteSpace = false;
				}

			}
			if (height < iBoardHeight && height + 1 < sSidePrint.size())
				std::cout << sSidePrint[height + 1];
			else
			{
				std::cout << "\t\t\t";
			}

			if (height > 0)
			{
				if (mHistory.size() == 0)
				{
					std::cout << "\tMake a move!";
				}
				else if (mHistory.size() < iBoardHeight)
				{
					if (l < mHistory.size())
					{
						std::cout << l + 1 << ". " << mHistory.at(l);
						l++;
					}
					else
						std::cout << "\tMake a move!";
				}
				else
				{
					int l = (mHistory.size() - iBoardHeight) + height;
					std::cout << "\t " << l + 1 << ". " << mHistory.at(l);
				}
			}
		}
		std::cout << "\n\n\n\n\n\n\n\n\n\n" << std::endl;
		dHistory.push_back("Board printed sucessfully");
		for (unsigned int i = 0; i < iBoardSize; i++) {cMoveBoard[i] = ' ';}
		bMoveCheck = false;
		sErrorMsg = "";
		return;
	}	//		END printType SMALL		//		END printType SMALL		//	END printType SMALL		//		END printType SMALL
}	//		END printBoard()		//		END printBoard()		//			END printBoard()		//			END printBoard()
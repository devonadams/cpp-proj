#pragma once
#define ChessLogic_H
#include <iostream>
#include <array>
#include <string>

// Initalize GLOBAL VARIBALES
//	Initalize Board Dimentions
extern const unsigned int iBoardWidth;
extern const unsigned int iBoardHeight;
extern const unsigned int iBoardSize;

//	Initalize ARRAYS
extern char iBoard[8 * 8];			//	Holds values the player sees
extern std::string sBoard[8 * 8];	//	Holds class name of piece and serves as a search function for objects within the class
extern char CheckBoard[8 * 8];		//	Holds value for where pieces can move -- Used for checking for check / Checking valid moves
extern char CheckmateBoard[8 * 8];	//	Holds value for checkmate (Can the king move?)
extern char cMoveBoard[8 * 8];		//	When cmove issued, CheckBoard is copied to this

	//	Initalize VECTORS
extern std::vector<std::string> dHistory;		//	Stores Debug History
extern std::vector<std::string> mHistory;		//	Stores Move History
extern std::vector<std::string> sCaptured;		//	Stores Captured Pieces
extern std::vector<std::string> sChecking;		//	Stores which pieces are checking the king (not implemented)
extern std::vector<std::string> sSidePrint;	//	Stores strings printed to the side of the board
extern std::string sErrorMsg;				//	Stores Error Message when something is invalid



extern unsigned int wMoves;					//	Stores total WHITE moves			--	UN-NEEDED unless purpose is found	(due to this->iMoves)
extern unsigned int bMoves;					//	Stores total BLACK moves			--	UN-NEEDED unless purpose is found	(due to this->iMoves)
extern unsigned int iWhiteScore;			//	Stores WHITE score based on number of pieces	--	For ChessAI
extern unsigned int iBlackScore;			//	Stores BLACK score based on number of pieces	--	For ChessAI

	//	Background Game Variables
extern unsigned int GameNumber;			//	Stores GameNumber for file writing
extern unsigned int mHistoryNumber;		//	Stores Move History Number incase of multiple saves in one process
extern unsigned int mHistoryReadNumber;	//	Stores the Move History Number that was last read
extern unsigned int dHistoryReadNumber;	//	Stores the Debug History Read Number that was last read
extern unsigned int dHistoryNumber;		//	Stores the Debug History Number incase of multiple saves in one process

	//	Player settings
extern unsigned int iDebugLevel;			//	Stores the process debug level
extern unsigned int iPieceColor;			//	Stores the process Piece Color value
extern char boardType;					//	Stores selected Board Type - l for large / s for small
extern bool bGraphics;				//	Stores whether player said ANSI esacpe codes worked, and enables/disables them

	//	Check Logic
extern bool bWhiteKingInCheck;			//	Is the WHITE king in check?
extern bool bBlackKingInCheck;			//	Is the BLACK king in check?
extern bool bCheckmate;			//	Is the game a checkmate?
extern unsigned int iWhiteKingLocation;	//	Stores WHITE king location for Check if Check validation		---		UN-NEEDED if using wKing GetPostion
extern unsigned int iBlackKingLocation;	//	Stores BLACK king location for Check if Check validation		---		UN-NEEDED if using bKing GetPostion
	//	Move Logic
extern bool bMoveToWhite;				//	Find color of Piece the player is attempting to move to	--	UN-NEEDED GLOBAL due to move to HEADER file
extern bool bCapturePiece;				//	Determine if move to capture a Piece is attempted			--	UN-NEEDED GLOBAL due to move to HEADER file
extern bool CurrentColorIsWhite;	//	Current move is WHITE
extern bool bCurrentlyCastling;		//	CASTLE move was attempted	--	used in Rook Logic
extern bool bCastleSideQueen;			//	CASTLE move was QUEENSIDE	--	used in Rook Logic
extern unsigned int iMoveFrom;				//	Player selected Piece is Moving From
extern unsigned int iMoveTo;				//	Player selected Piece is attempting to Move To

	//	Game Status
extern bool bGameStatus;				//	Stores if a singleplayer / multiplayer match is occuring
extern bool bRematch;					//	Used for 'rematch' command to skip re-inputing game properties

	//	Save this-> values to variable for use inside bLogic function ( NO LONGER IN USE )
extern unsigned int iThisPos;				//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iPosition
extern unsigned int iThisMoves;				//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iMoves
extern bool iThisWhite;						//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iWhitePiece
extern unsigned int pieceType;				//	------------------------------------------------------------------------------	UN-NEEDED when ChessLogic uses this->iType
extern bool bMoveCheck;

/*
	//				                W I D T H
	//			       A    B    C    D    E    F    G    H   
	//			    ==========================================
	//			   | #####-----#####-----#####-----#####----- |
	//			 1 | # 0 #- 1 -# 2 #- 3 -# 4 #- 5 -# 6 #- 7 - | 1
	//			   | #####-----#####-----#####-----#####----- |
	//		H	   | -----#####-----#####-----#####-----##### |		//		      A  B  C  D  E  F  G  H   
	//			 2 | - 8 -# 9 #- 10-# 11#- 12-# 13#- 14-# 15# |	2	//		    _________________________
	//		E	   | -----#####-----#####-----#####-----##### |		//		 1 |  0  1  2  3  4  5  6  7	1
	//			   | #####-----#####-----#####-----#####----- |		//		 2 |  8  9 10 11 12 13 14 15	2
	//		I	 3 | # 16#- 17-# 18#- 19-# 20#- 21-# 22#- 23- |	3	//		 3 | 16 17 18 19 20 21 22 23	3
	//			   | #####-----#####-----#####-----#####----- |		//		 4 | 24 25 26 27 28 29 30 31	4
	//		G	   | -----#####-----#####-----#####-----##### |		//		 5 | 32 33 34 35 36 37 38 39	5
	//			 4 | - 24-# 25#- 26-# 27#- 28-# 29#- 30-# 31# |	4	//		 6 | 40 41 42 43 44 45 46 47	6
	//		H	   | -----#####-----#####-----#####-----##### | 	//		 7 | 48 49 50 51 52 53 54 55	7
	//			   | #####-----#####-----#####-----#####----- |		//		 8 | 56 57 58 59 60 61 62 63	8
	//		T	 5 | # 32#- 33-# 34#- 35-# 36#- 37-# 38#- 39- |	5	//
	//			   | #####-----#####-----#####-----#####----- |		//			  A  B  C  D  E  F  G  H
	//			   | -----#####-----#####-----#####-----##### |
	//			 6 | - 40-# 41#- 42-# 43#- 44-# 45#- 46-# 47# |	6	
	//			   | -----#####-----#####-----#####-----##### |
	//			   | #####-----#####-----#####-----#####----- |
	//			 7 | # 48#- 49-# 50#- 51-# 52#- 53-# 54#- 55- |	7	
	//			   | #####-----#####-----#####-----#####----- |
	//			   | -----#####-----#####-----#####-----##### |
	//			 8 | - 56-# 57#- 58-# 59#- 60-# 61#- 62-# 63# |	8	
	//			   | -----#####-----#####-----#####-----##### |
	//			    ==========================================	
	//			       A    B    C    D    E    F    G    H

	Type 0: King	Value: infinite
	Type 1: Queen	Value: 9

	Type 2: Rook	Value: 5

	Type 3: Bishop	Value: 3
	Type 4: Knight	Value: 3

	Type 5: Pawn	Value:1

*/
namespace ChessLogic_H
{
	void vPieceInit();
	bool bSearchObj();				//	Finds the Piece object at iMoveFrom and calls logic for that Piece
	void vMoveCheck();				//	Called when player uses 'cmove' to call only the specific piece requested
	bool bIsKingInCheck();			//	Writes all pieces of current color to CheckBoard and tests if opposite king location is on a valid move
	bool bIsKingInCheckmate();		//	Logic for determining if king is in CHECK or CHECKMATE
	bool bRookCastle();				//	Calls Rook piece for castling

	class Piece {
	private:
		unsigned int iPosition;
		unsigned int iValue;
		unsigned int iType;
		unsigned int iMoves;
		bool bWhitePiece;
		bool bInGame;
		char cVisual;

	public:
		//	Construtors
		Piece()
		{
			this->iPosition = 0;
			this->iValue = 0;
			this->iType = 0;
			this->bWhitePiece = true;
			this->iMoves = 0;
			this->bInGame = true;
			this->cVisual = ' ';
		}


		//	Destructors
		~Piece(void) {}

		//	Setters
		bool SetPosition()
		{
			bool bMoveValid = false;
			bCapturePiece = false;
			dHistory.push_back("INFO: bMoveCheck : " + std::to_string(bMoveCheck) + "\tiPos: " + std::to_string(this->iPosition) + "\tiMoves: " + std::to_string(this->iMoves) + "\tiType: " + std::to_string(this->iType) + "\tbWhitePiece: " + std::to_string(this->bWhitePiece));
			if (!bMoveCheck)
			{
				std::string sMoveToPiece = sBoard[iMoveTo];

				if (sMoveToPiece != " ")
				{
					bCapturePiece = true;

					if (sMoveToPiece.at(0) == 'w')
					{
						bMoveToWhite = true;
					}
					else if (sMoveToPiece.at(0) == 'b')
					{
						bMoveToWhite = false;
					}
					else
					{
						dHistory.push_back("CRITICAL: sMoveToPiece function failed due to input \"" + sMoveToPiece + "\".");
						sErrorMsg = "Something went wrong";
						return false;
					}
					dHistory.push_back("INFO: bCapturePiece: TRUE \tbMoveToWhite: " + std::to_string(bMoveToWhite));
				}
				else
				{
					dHistory.push_back("INFO: bCapturePiece: FALSE");
					bCapturePiece = false;
				}
				if (iMoveTo > iBoardSize)
				{
					dHistory.push_back("ERR: Move was outside of board boundaries");
					sErrorMsg = "Move was out of board boundaries";
					bMoveValid = false;
				}
				else if (this->bWhitePiece != CurrentColorIsWhite)
				{
					dHistory.push_back("ERR: It's not that color's turn!");
					sErrorMsg = "It's not that color turn!";
					return false;
				}
				else if (bCapturePiece == true)
				{
					if (this->bWhitePiece == bMoveToWhite && this->iType != 0)
					{
						dHistory.push_back("ERR: You can't capture your own piece!");
						sErrorMsg = "You can't capture your own piece!";
						return false;
					}
				}
			}


			if (this->iType == 0)	//---------------------------------------	KING LOGIC	-------------------------------------------	KING LOGIC	----------------------------------	-------------------------------------------	KING LOGIC	----------------------------------
			{
				bool bKingInCheck = true;

				unsigned int pEquationDown = this->iPosition + 8;
				unsigned int pEquationUp = this->iPosition - 8;

				unsigned int pEquationLeft = this->iPosition + 1;
				unsigned int pEquationRight = this->iPosition - 1;

				unsigned int pEquationUpLeft = this->iPosition - 9;
				unsigned int pEquationUpRight = this->iPosition - 7;

				unsigned int pEquationDownLeft = this->iPosition + 7;
				unsigned int pEquationDownRight = this->iPosition + 9;


				if (pEquationDown >= 0 && pEquationDown <= iBoardSize)
				{
					if (bMoveCheck)
					{
						CheckBoard[pEquationDown] = 'x';
					}
					else
					{
						if (iMoveTo == pEquationDown && this->bWhitePiece != bMoveToWhite)	//	move down
						{
							dHistory.push_back("INFO: King moved down");
							bMoveValid = true;
						}
					}
				}
				if (pEquationUp >= 0 && pEquationUp <= iBoardSize)
				{
					if (bMoveCheck)
					{
						CheckBoard[pEquationUp] = 'x';
					}
					else
					{
						if (iMoveTo == pEquationUp && this->bWhitePiece != bMoveToWhite)	//	move up
						{
							dHistory.push_back("INFO: King moved up");
							bMoveValid = true;
						}
					}
				}
				if (pEquationLeft >= 0 && pEquationLeft <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationLeft >= i * iBoardWidth) && (pEquationLeft <= (i * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationLeft] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationLeft && this->bWhitePiece != bMoveToWhite)	//	move left
								{
									dHistory.push_back("INFO: King moved left");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationRight >= 0 && pEquationRight <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationRight >= i * iBoardWidth) && (pEquationRight <= (i * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationRight] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationRight && this->bWhitePiece != bMoveToWhite)	//	move right
								{
									dHistory.push_back("INFO: King moved right");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationDownLeft >= 0 && pEquationDownLeft <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationDownLeft >= (i + 1) * iBoardWidth) && (pEquationDownLeft <= ((i + 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationDownLeft] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationDownLeft && this->bWhitePiece != bMoveToWhite)	//	move down left
								{
									dHistory.push_back("INFO: King moved down left");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationDownRight >= 0 && pEquationDownRight <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationDownRight >= (i + 1) * iBoardWidth) && (pEquationDownRight <= ((i + 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationDownRight] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationDownRight && this->bWhitePiece != bMoveToWhite)	//	move down right
								{
									dHistory.push_back("INFO: King moved down right");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationUpLeft >= 0 && pEquationUpLeft <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationUpLeft >= (i - 1) * iBoardWidth) && (pEquationUpLeft <= ((i - 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationUpLeft] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationUpLeft && this->bWhitePiece != bMoveToWhite)	//	move up left
								{
									dHistory.push_back("INFO: King moved up left");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationUpRight >= 0 && pEquationUpRight <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardSize; i++)
					{
						if ((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1)) &&
							(pEquationUpRight >= (i - 1) * iBoardWidth) && (pEquationUpRight <= ((i - 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationUpRight] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationUpRight && this->bWhitePiece != bMoveToWhite)	//	move up right
								{
									dHistory.push_back("INFO: King moved up right");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (this->iMoves == 0)	// Queenside castling
				{
					bool lValidMove = false;
					if (iMoveTo >= (this->iPosition - 4) && iMoveTo <= (this->iPosition - 2))
					{
						if (sBoard[this->iPosition - 3] == " " && sBoard[this->iPosition - 2] == " " && sBoard[this->iPosition - 1] == " ")
						{
							dHistory.push_back("Attempting queenside castle...");
							bCastleSideQueen = true;
							bCurrentlyCastling = true;
							lValidMove = bRookCastle();
							if (lValidMove)
							{
								iMoveTo = this->iPosition - 2;
								bMoveValid = true;
							}
						}
						else
						{
							sErrorMsg = "There can't be any pieces between the King and Rook!";
							bMoveValid = false;
						}
					}
					else if (iMoveTo >= (this->iPosition + 2) && iMoveTo <= (this->iPosition + 3))	//Kingside	castling
					{
						if (sBoard[this->iPosition + 1] == " " && sBoard[this->iPosition + 2] == " ")
						{
							dHistory.push_back("Attempting kingside castle...");
							bCastleSideQueen = false;
							bCurrentlyCastling = true;
							lValidMove = bRookCastle();
							if (lValidMove)
							{
								iMoveTo = this->iPosition + 1;
								bMoveValid = true;
							}
						}
						else
						{
							sErrorMsg = "There can't be any pieces between the King and Rook!";
							bMoveValid = false;
						}
					}	//	END Kingside White
				}
			}	//	END bKingLogic
			else if (this->iType == 1)	//---------------------------------------	QUEEN LOGIC	-------------------------------------------	QUEEN LOGIC	----------------------------------	-------------------------------------------	QUEEN LOGIC	----------------------------------
			{
				unsigned int iThroughPiece = 0;
				unsigned int iLoopNum = 0;
				bool bThroughCalc = false;
				bool bDown = false;
				bool bUp = false;
				bool bLeft = false;
				bool bRight = false;
				bool bUpLeft = false;
				bool bUpRight = false;
				bool bDownLeft = false;
				bool bDownRight = false;

				for (unsigned int i = 1; i < iBoardWidth; i++)
				{
					unsigned int pEquationDown = this->iPosition + (i * 8);
					unsigned int pEquationUp = this->iPosition - (i * 8);

					unsigned int pEquationLeft = this->iPosition - (i);
					unsigned int pEquationRight = this->iPosition + (i);

					unsigned int pEquationUpLeft = this->iPosition - (i * 9);
					unsigned int pEquationUpRight = this->iPosition - (i * 7);

					unsigned int pEquationDownLeft = this->iPosition + (i * 7);
					unsigned int pEquationDownRight = this->iPosition + (i * 9);


					if (bDown == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDown == true) { break; }
							unsigned int pEquationDown = this->iPosition + (i * 8);
							if (pEquationDown >= 0 && pEquationDown <= iBoardSize)
							{
								if (sBoard[pEquationDown] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDown] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDown] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDown)
											{
												dHistory.push_back("Queen moved down");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDown = this->iPosition + ((i - 1) * 8);
											if (iMoveTo == pEquationDown)
											{
												dHistory.push_back("Queen moved down");
												bMoveValid = true;
											}
										}
									}
									bDown = true;
								}
							}
							else { bDown = true; }
						}
					}
					if (bUp == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUp == true) { break; }
							unsigned int pEquationUp = this->iPosition - (i * 8);
							if (pEquationUp >= 0 && pEquationUp <= iBoardSize)
							{
								if (sBoard[pEquationUp] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUp] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUp] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUp)
											{
												dHistory.push_back("Queen moved up");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUp = this->iPosition - ((i - 1) * 8);
											if (iMoveTo == pEquationUp)
											{
												dHistory.push_back("Queen moved up");
												bMoveValid = true;
											}
										}
									}
									bUp = true;
								}
							}
							else { bUp = true; }
						}
					}
					if (bLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bLeft == true) { break; }
							unsigned int pEquationLeft = this->iPosition - (i);
							if (pEquationLeft >= 0 && pEquationLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationLeft == (j * iBoardWidth))
									{
										bLeft = true;
									}
								}
								if (sBoard[pEquationLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationLeft)
											{
												dHistory.push_back("Queen moved left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationLeft = this->iPosition - (i - 1);
											if (iMoveTo == pEquationLeft)
											{
												dHistory.push_back("Queen moved left");
												bMoveValid = true;
											}
										}
									}
									bLeft = true;
								}
							}
							else { bLeft = true; }
						}
					}
					if (bRight == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bRight == true) { break; }
							unsigned int pEquationRight = this->iPosition + (i);
							if (pEquationRight >= 0 && pEquationRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationRight == (j * iBoardWidth) - 1)
									{
										bRight = true;
									}
								}
								if (sBoard[pEquationRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationRight)
											{
												dHistory.push_back("Queen moved right");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationRight = this->iPosition + (i - 1);
											if (iMoveTo == pEquationRight)
											{
												dHistory.push_back("Queen moved right");
												bMoveValid = true;
											}
										}
									}
									bRight = true;
								}
							}
							else { bRight = true; }
						}
					}
					if (bUpLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUpLeft == true) { break; }
							unsigned int pEquationUpLeft = this->iPosition - (i * 9);
							if (pEquationUpLeft >= 0 && pEquationUpLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationUpLeft == (j * iBoardWidth))
									{
										bUpLeft = true;
									}
								}
								if (sBoard[pEquationUpLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUpLeft)
											{
												dHistory.push_back("Queen moved up left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUpLeft = this->iPosition - ((i - 1) * 9);
											if (iMoveTo == pEquationUpLeft)
											{
												dHistory.push_back("Queen moved up left");
												bMoveValid = true;
											}
										}
									}
									bUpLeft = true;
								}
							}
							else { bUpLeft = true; }
						}
					}
					if (bUpRight == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUpRight == true) { break; }
							unsigned int pEquationUpRight = this->iPosition - (i * 7);
							if (pEquationUpRight >= 0 && pEquationUpRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationUpRight == (j * iBoardWidth) - 1)
									{
										bUpRight = true;
									}
								}
								if (sBoard[pEquationUpRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUpRight)
											{
												dHistory.push_back("Queen moved up right");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUpRight = this->iPosition - ((i - 1) * 7);
											if (iMoveTo == pEquationUpRight)
											{
												dHistory.push_back("Queen moved up right");
												bMoveValid = true;
											}
										}
									}
									bUpRight = true;
								}
							}
							else { bUpRight = true; }
						}
					}
					if (bDownLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDownLeft == true) { break; }
							unsigned int pEquationDownLeft = this->iPosition + (i * 7);
							if (pEquationDownLeft >= 0 && pEquationDownLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationDownLeft == (j * iBoardWidth))
									{
										bDownLeft = true;
									}
								}
								if (sBoard[pEquationDownLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDownLeft)
											{
												dHistory.push_back("Queen moved down left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDownLeft = this->iPosition + ((i - 1) * 7);
											if (iMoveTo == pEquationDownLeft)
											{
												dHistory.push_back("Queen moved down left");
												bMoveValid = true;
											}
										}
									}
									bDownLeft = true;
								}
							}
							else { bDownLeft = true; }
						}
					}
					if (bDownRight == false)
					{
						iLoopNum = i;
						bThroughCalc = false;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDownRight == true) { break; }
							unsigned int pEquationDownRight = this->iPosition + (i * 9);
							if (pEquationDownRight >= 0 && pEquationDownRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationDownRight == (j * iBoardWidth) - 1)
									{
										bDownRight = true;
									}
								}
								if (sBoard[pEquationDownRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDownRight)
											{
												dHistory.push_back("Queen moved down left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDownRight = this->iPosition + ((i - 1) * 9);
											if (iMoveTo == pEquationDownRight)
											{
												dHistory.push_back("Queen moved down left");
												bMoveValid = true;
											}
										}
									}
									bDownRight = true;
								}
							}
							else { bDownRight = true; }
						}
					}
				}
			}	//	END QUEEN logic
			else if (this->iType == 2)	//---------------------------------------	ROOK LOGIC	-------------------------------------------	ROOK LOGIC	----------------------------------	-------------------------------------------	ROOK LOGIC	----------------------------------
			{
				unsigned int iThroughPiece = 0;
				unsigned int iLoopNum = 0;
				bool bThroughCalc = false;

				if (bCurrentlyCastling == true)
				{
					if (this->iMoves == 0)
					{
						if (CurrentColorIsWhite == true)
						{
							if (bCastleSideQueen == true)
							{
								this->iMoves++;
								this->iPosition = 59;
								sBoard[59] = sBoard[56];
								iBoard[59] = iBoard[56];
								sBoard[56] = " ";
								iBoard[56] = ' ';
								dHistory.push_back("INFO: Queenside WHITE Rook castled sucessfully!");
								bMoveValid = true;
							}
							else    // Castle side is king
							{
								this->iMoves++;
								this->iPosition = 61;
								sBoard[61] = sBoard[63];
								iBoard[61] = iBoard[63];
								sBoard[63] = " ";
								iBoard[63] = ' ';
								dHistory.push_back("INFO: Kingside WHITE Rook castled sucessfully!");
								bMoveValid = true;
							}
						}
						else    //CurrentColor is black
						{
							if (bCastleSideQueen == true)
							{
								this->iMoves++;
								this->iPosition = 3;
								sBoard[3] = sBoard[0];
								iBoard[3] = iBoard[0];
								sBoard[0] = " ";
								iBoard[0] = ' ';
								dHistory.push_back("INFO: Queenside BLACK Rook castled sucessfully!");
								bMoveValid = true;
							}
							else    // Castle side is king
							{
								this->iMoves++;
								this->iPosition = 5;
								sBoard[5] = sBoard[7];
								iBoard[5] = iBoard[7];
								sBoard[7] = " ";
								iBoard[7] = ' ';
								dHistory.push_back("INFO: Kingside BLACK Rook castled sucessfully!");
								bMoveValid = true;
							}
						}
					}
					else
					{
						dHistory.push_back("ERR: Rook must have 0 moves to castle!");
						sErrorMsg = "Rook must have 0 moves to castle";
						bMoveValid = false;
					}
				}	//	End bCurrentlyCastling

				bool bDown = false;
				bool bUp = false;
				bool bLeft = false;
				bool bRight = false;

				for (unsigned int i = 1; i < iBoardWidth; i++)
				{
					unsigned int pEquationDown = this->iPosition + (i * 8);
					unsigned int pEquationUp = this->iPosition - (i * 8);

					unsigned int pEquationLeft = this->iPosition - (i);
					unsigned int pEquationRight = this->iPosition + (i);

					if (bDown == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDown == true) { break; }
							unsigned int pEquationDown = this->iPosition + (i * 8);
							if (pEquationDown >= 0 && pEquationDown <= iBoardSize)
							{
								if (sBoard[pEquationDown] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDown] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDown] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDown)
											{
												dHistory.push_back("Rook moved down");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDown = this->iPosition + ((i - 1) * 8);
											if (iMoveTo == pEquationDown)
											{
												dHistory.push_back("Rook moved down");
												bMoveValid = true;
											}
										}
									}
									bDown = true;
								}
							}
							else { bDown = true; }
						}
					}
					if (bUp == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUp == true) { break; }
							unsigned int pEquationUp = this->iPosition - (i * 8);
							if (pEquationUp >= 0 && pEquationUp <= iBoardSize)
							{
								if (sBoard[pEquationUp] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUp] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUp] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUp)
											{
												dHistory.push_back("Rook moved up");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUp = this->iPosition - ((i - 1) * 8);
											if (iMoveTo == pEquationUp)
											{
												dHistory.push_back("Rook moved up");
												bMoveValid = true;
											}
										}
									}
									bUp = true;
								}
							}
							else { bUp = true; }
						}
					}
					if (bLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bLeft == true) { break; }
							unsigned int pEquationLeft = this->iPosition - (i);
							if (pEquationLeft >= 0 && pEquationLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationLeft == (j * iBoardWidth))
									{
										bLeft = true;
									}
								}
								if (sBoard[pEquationLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationLeft)
											{
												dHistory.push_back("Rook moved left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationLeft = this->iPosition - (i - 1);
											if (iMoveTo == pEquationLeft)
											{
												dHistory.push_back("Rook moved left");
												bMoveValid = true;
											}
										}
									}
									bLeft = true;
								}
							}
							else { bLeft = true; }
						}
					}
					if (bRight == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bRight == true) { break; }
							unsigned int pEquationRight = this->iPosition + (i);
							if (pEquationRight >= 0 && pEquationRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationRight == (j * iBoardWidth) - 1)
									{
										bRight = true;
									}
								}
								if (sBoard[pEquationRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationRight)
											{
												dHistory.push_back("Rook moved right");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationRight = this->iPosition + (i - 1);
											if (iMoveTo == pEquationRight)
											{
												dHistory.push_back("Rook moved right");
												bMoveValid = true;
											}
										}
									}
									bRight = true;
								}
							}
							else { bRight = true; }
						}
					}
				}
			}	//	END QUEEN LOGIC
			else if (this->iType == 3)	//---------------------------------------	BISHOP LOGIC	-------------------------------------------	BISHOP LOGIC	----------------------------------	-------------------------------------------	BISHOP LOGIC	----------------------------------
			{
				unsigned int iThroughPiece = 0;
				unsigned int iLoopNum = 0;
				bool bThroughCalc = false;
				bool bUpLeft = false;
				bool bUpRight = false;
				bool bDownLeft = false;
				bool bDownRight = false;

				for (unsigned int i = 1; i < iBoardWidth; i++)
				{

					unsigned int pEquationUpLeft = this->iPosition - (i * 9);
					unsigned int pEquationUpRight = this->iPosition - (i * 7);

					unsigned int pEquationDownLeft = this->iPosition + (i * 7);
					unsigned int pEquationDownRight = this->iPosition + (i * 9);

					if (bUpLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUpLeft == true) { break; }
							unsigned int pEquationUpLeft = this->iPosition - (i * 9);
							if (pEquationUpLeft >= 0 && pEquationUpLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationUpLeft == (j * iBoardWidth))
									{
										bUpLeft = true;
									}
								}
								if (sBoard[pEquationUpLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUpLeft)
											{
												dHistory.push_back("Bishop moved up left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUpLeft = this->iPosition - ((i - 1) * 9);
											if (iMoveTo == pEquationUpLeft)
											{
												dHistory.push_back("Rook moved up left");
												bMoveValid = true;
											}
										}
									}
									bUpLeft = true;
								}
							}
							else { bUpLeft = true; }
						}
					}
					if (bUpRight == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bUpRight == true) { break; }
							unsigned int pEquationUpRight = this->iPosition - (i * 7);
							if (pEquationUpRight >= 0 && pEquationUpRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationUpRight == (j * iBoardWidth) - 1)
									{
										bUpRight = true;
									}
								}
								if (sBoard[pEquationUpRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationUpRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationUpRight)
											{
												dHistory.push_back("Bishop moved up right");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationUpRight = this->iPosition - ((i - 1) * 7);
											if (iMoveTo == pEquationUpRight)
											{
												dHistory.push_back("Rook moved up right");
												bMoveValid = true;
											}
										}
									}
									bUpRight = true;
								}
							}
							else { bUpRight = true; }
						}
					}
					if (bDownLeft == false)
					{
						iLoopNum = i;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDownLeft == true) { break; }
							unsigned int pEquationDownLeft = this->iPosition + (i * 7);
							if (pEquationDownLeft >= 0 && pEquationDownLeft <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationDownLeft == (j * iBoardWidth))
									{
										bDownLeft = true;
									}
								}
								if (sBoard[pEquationDownLeft] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownLeft] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownLeft] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDownLeft)
											{
												dHistory.push_back("Bishop moved down left");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDownLeft = this->iPosition + ((i - 1) * 7);
											if (iMoveTo == pEquationDownLeft)
											{
												dHistory.push_back("Rook moved down left");
												bMoveValid = true;
											}
										}
									}
									bDownLeft = true;
								}
							}
							else { bDownLeft = true; }
						}
					}
					if (bDownRight == false)
					{
						iLoopNum = i;
						bThroughCalc = false;
						for (unsigned int i = 1; i < iBoardWidth; i++)
						{
							if (bDownRight == true) { break; }
							unsigned int pEquationDownRight = this->iPosition + (i * 9);
							if (pEquationDownRight >= 0 && pEquationDownRight <= iBoardSize)
							{
								for (unsigned int j = 0; j < iBoardWidth; j++)
								{
									if (pEquationDownRight == (j * iBoardWidth) - 1)
									{
										bDownRight = true;
									}
								}
								if (sBoard[pEquationDownRight] == " ")
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownRight] = 'x';
									}
								}
								else
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDownRight] = 'x';
									}
									else
									{
										if (bCapturePiece == true)
										{
											if (iMoveTo == pEquationDownRight)
											{
												dHistory.push_back("Bishop moved down right");
												bMoveValid = true;
											}
										}
										else
										{
											int pEquationDownRight = this->iPosition + ((i - 1) * 9);
											if (iMoveTo == pEquationDownRight)
											{
												dHistory.push_back("Rook moved down right");
												bMoveValid = true;
											}
										}
									}
									bDownRight = true;
								}
							}
							else { bDownRight = true; }
						}
					}
				}
			}	//	END BISHOP logic
			else if (this->iType == 4)	//---------------------------------------	KNIGHT LOGIC	-------------------------------------------	KNIGHT LOGIC	----------------------------------	-------------------------------------------	KNIGHT LOGIC	----------------------------------
			{
				unsigned int pEquationDownLeft = this->iPosition + 15;	//	Down
				unsigned int pEquationDownRight = this->iPosition + 17;

				unsigned int pEquationUpLeft = this->iPosition - 17;		// Up
				unsigned int pEquationUpRight = this->iPosition - 15;

				unsigned int pEquationLeftDown = this->iPosition + 10;	//	left / right down
				unsigned int pEquationRightDown = this->iPosition + 6;

				unsigned int pEquationLeftUp = this->iPosition - 10;		// left / right up
				unsigned int pEquationRightUp = this->iPosition - 6;

				if (pEquationDownLeft >= 0 && pEquationDownLeft <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationDownLeft >= ((i + 2) * iBoardWidth) - 1) && pEquationDownLeft <= (((i + 2) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationDownLeft] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationDownLeft)
								{
									dHistory.push_back("Knight moved down left");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationDownRight >= 0 && pEquationDownRight <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationDownRight >= ((i + 2) * iBoardWidth)) && pEquationDownRight <= (((i + 2) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationDownRight] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationDownRight)
								{
									dHistory.push_back("Knight moved down right");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationUpLeft >= 0 && pEquationUpLeft <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationUpLeft >= ((i - 2) * iBoardWidth)) && pEquationUpLeft <= (((i - 2) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationUpLeft] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationUpLeft)
								{
									dHistory.push_back("Knight moved up left");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationUpRight >= 0 && pEquationUpRight <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationUpRight >= ((i - 2) * iBoardWidth)) && pEquationUpRight <= (((i - 2) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationUpRight] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationUpRight)
								{
									dHistory.push_back("Knight moved up right");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationLeftDown >= 0 && pEquationLeftDown <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationLeftDown >= ((i + 1) * iBoardWidth)) && pEquationLeftDown <= (((i + 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationLeftDown] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationLeftDown)
								{
									dHistory.push_back("Knight moved left down");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationRightDown >= 0 && pEquationRightDown <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationRightDown >= ((i + 1) * iBoardWidth)) && pEquationRightDown <= (((i + 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationRightDown] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationRightDown)
								{
									dHistory.push_back("Knight moved right down");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationLeftUp >= 0 && pEquationLeftUp <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationLeftUp >= ((i - 1) * iBoardWidth)) && pEquationLeftUp <= (((i - 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationLeftUp] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationLeftUp)
								{
									dHistory.push_back("Knight moved left up");
									bMoveValid = true;
								}
							}
						}
					}
				}
				if (pEquationRightUp >= 0 && pEquationRightUp <= iBoardSize)
				{
					for (unsigned int i = 0; i < iBoardWidth; i++)
					{
						if (this->iPosition >= (i * iBoardWidth) && (this->iPosition <= ((i * iBoardWidth) + (iBoardWidth - 1))) &&
							(pEquationRightUp >= ((i - 1) * iBoardWidth)) && pEquationRightUp <= (((i - 1) * iBoardWidth) + (iBoardWidth - 1)))
						{
							if (bMoveCheck)
							{
								CheckBoard[pEquationRightUp] = 'x';
							}
							else
							{
								if (iMoveTo == pEquationRightUp)
								{
									dHistory.push_back("Knight moved right up");
									bMoveValid = true;
								}
							}
						}
					}
				}
			}	//	END KNIGHT logic
			else if (this->iType == 5)	//---------------------------------------	PAWN LOGIC	-------------------------------------------	PAWN LOGIC	----------------------------------	-------------------------------------------	PAWN LOGIC	----------------------------------
			{
				unsigned int pEquationUp = 0;
				unsigned int pEquationDouble = 0;
				unsigned int pEquationRightCapture = 0;
				unsigned int pEquationLeftCapture = 0;
				if (this->bWhitePiece == true)
				{
					pEquationUp = this->iPosition - 8;
					pEquationDouble = this->iPosition - 16;
					pEquationRightCapture = this->iPosition - 7;
					pEquationLeftCapture = this->iPosition - 9;

				}
				else // color is black
				{
					pEquationUp = this->iPosition + 8;
					pEquationDouble = this->iPosition + 16;
					pEquationRightCapture = this->iPosition + 7;
					pEquationLeftCapture = this->iPosition + 9;
				}
				for (unsigned int i = 0; i < iBoardWidth; i++)
				{
					if (this->bWhitePiece == true)
					{
						if (((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1))))
						{
							if (((pEquationLeftCapture >= 0) && (pEquationLeftCapture <= iBoardSize)) &&
								((pEquationLeftCapture >= ((i - 1) * iBoardWidth)) && (pEquationLeftCapture <= (((i - 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationLeftCapture] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationLeftCapture)
									{
										dHistory.push_back("Pawn captured left");
										bMoveValid = true;
									}
								}
							}
							if (((pEquationRightCapture >= 0) && (pEquationRightCapture <= iBoardSize)) &&
								((pEquationRightCapture >= ((i - 1) * iBoardWidth)) && (pEquationRightCapture <= (((i - 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationRightCapture] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationRightCapture)
									{
										dHistory.push_back("Pawn captured right");
										bMoveValid = true;
									}
								}
							}
							if (((pEquationUp >= 0) && (pEquationUp <= iBoardSize)) &&
								((pEquationUp >= ((i - 1) * iBoardWidth)) && (pEquationUp <= (((i - 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationUp] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationUp)
									{
										dHistory.push_back("Pawn moved up");
										bMoveValid = true;
									}
								}
								if (this->iMoves == 0)
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDouble] = 'x';
									}
									else
									{
										if (iMoveTo == pEquationDouble)
										{
											dHistory.push_back("Pawn moved up double");
											bMoveValid = true;
										}
									}
								}
							}
						}
					}
					else    // piece is  black
					{
						if (((this->iPosition >= i * iBoardWidth) && (this->iPosition <= (i * iBoardWidth) + (iBoardWidth - 1))))
						{
							if (((pEquationLeftCapture >= 0) && (pEquationLeftCapture <= iBoardSize)) &&
								((pEquationLeftCapture >= ((i + 1) * iBoardWidth)) && (pEquationLeftCapture <= (((i + 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationLeftCapture] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationLeftCapture)
									{
										dHistory.push_back("Pawn captured left");
										bMoveValid = true;
									}
								}
							}
							if (((pEquationRightCapture >= 0) && (pEquationRightCapture <= iBoardSize)) &&
								((pEquationRightCapture >= ((i + 1) * iBoardWidth)) && (pEquationRightCapture <= (((i + 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationRightCapture] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationRightCapture)
									{
										dHistory.push_back("Pawn captured right");
										bMoveValid = true;
									}
								}
							}
							if (((pEquationUp >= 0) && (pEquationUp <= iBoardSize)) &&
								((pEquationUp >= ((i + 1) * iBoardWidth)) && (pEquationUp <= (((i + 1) * iBoardWidth) + (iBoardWidth - 1)))))
							{
								if (bMoveCheck)
								{
									CheckBoard[pEquationUp] = 'x';
								}
								else
								{
									if (iMoveTo == pEquationUp)
									{
										dHistory.push_back("Pawn moved down");
										bMoveValid = true;
									}
								}
								if (this->iMoves == 0)
								{
									if (bMoveCheck)
									{
										CheckBoard[pEquationDouble] = 'x';
									}
									else
									{
										if (iMoveTo == pEquationDouble)
										{
											dHistory.push_back("Pawn moved down double");
											bMoveValid = true;
										}
									}
								}
							}
						}
					}
				}
			}	//---------------------------------------	PAWN LOGIC END	-------------------------------------------	PAWN LOGIC END	----------------------------------	-------------------------------------------	PAWN LOGIC END	----------------------------------

			if (bMoveCheck) { return false; }
			if (bMoveValid == true)
			{
				bool CheckKingCheck = true;

				if (this->iType != 0)
				{
					if (bCapturePiece == true)
					{
						mHistory.push_back("\t" + sBoard[iMoveFrom] + " from " + std::to_string(iMoveFrom) + " captured " + sBoard[iMoveTo] + " at " + std::to_string(iMoveTo));
						dHistory.push_back("INFO: " + sBoard[iMoveFrom] + " from " + std::to_string(iMoveFrom) + " captured " + sBoard[iMoveTo] + " at " + std::to_string(iMoveTo));
						sCaptured.push_back(sBoard[iMoveTo]);
					}
					else if (bCurrentlyCastling == true)
					{
						mHistory.push_back("\t" + sBoard[iMoveFrom] + " castled from " + std::to_string(iMoveFrom) + " to " + std::to_string(iMoveTo));
						dHistory.push_back("INFO: " + sBoard[iMoveFrom] + " castled from " + std::to_string(iMoveFrom) + " to " + std::to_string(iMoveTo));
						bCurrentlyCastling = false;
					}
					else
					{
						mHistory.push_back("\t" + sBoard[iMoveFrom] + " moved from " + std::to_string(iMoveFrom) + " to " + std::to_string(iMoveTo));
						dHistory.push_back("INFO: " + sBoard[iMoveFrom] + " moved from " + std::to_string(iMoveFrom) + " to " + std::to_string(iMoveTo));
					}
					this->iMoves++;
					this->iPosition = iMoveTo;

					iBoard[iMoveTo] = this->cVisual;
					iBoard[iMoveFrom] = ' ';

					sBoard[iMoveTo] = sBoard[iMoveFrom];
					sBoard[iMoveFrom] = ' ';

					CheckKingCheck = bIsKingInCheck();
					bool CheckMateCheck = false;

					if (CheckKingCheck == true)
					{
						if (CurrentColorIsWhite == true)
						{
							dHistory.push_back("INFO: bBlackKingInCheck is TRUE");
							bBlackKingInCheck = true;
							bCheckmate = bIsKingInCheckmate();
						}
						else
						{
							dHistory.push_back("INFO: bWhiteKingInCheck is TRUE");
							bWhiteKingInCheck = true;
							bCheckmate = bIsKingInCheckmate();
						}
					}
					else
					{
						if (CurrentColorIsWhite == true)
						{
							dHistory.push_back("INFO: bBlackKingInCheck is FALSE");
							bWhiteKingInCheck = false;
						}
						else
						{
							dHistory.push_back("INFO: bWhiteKingInCheck is FALSE");
							bBlackKingInCheck = false;
						}
					}

					return true;
				}
				else
				{
					iBoard[this->iPosition] = this->cVisual;
					iBoard[iMoveFrom] = ' ';

					sBoard[iMoveTo] = sBoard[iMoveFrom];
					sBoard[iMoveFrom] = ' ';

					this->iMoves++;
					this->iPosition = iMoveTo;

					if (CurrentColorIsWhite == true)
					{
						CurrentColorIsWhite = false;
						CheckKingCheck = bIsKingInCheck();
						CurrentColorIsWhite = true;
					}
					else
					{
						CurrentColorIsWhite = true;
						CheckKingCheck = bIsKingInCheck();
						CurrentColorIsWhite = false;
					}
					std::cout << "CheckKingCheck: " << std::to_string(CheckKingCheck) << std::endl;
					dHistory.push_back("King is in check: " + std::to_string(CheckKingCheck));
					if (CheckKingCheck == true)
					{
						if (CurrentColorIsWhite == true)
						{
							if (CheckBoard[iMoveTo] == 'x')
							{
								dHistory.push_back("ERR: You can't move your king into check!");
								sErrorMsg = "You can't move your king into check!";

								this->iMoves--;
								this->iPosition = iMoveFrom;

								iBoard[this->iPosition] = this->cVisual;
								iBoard[iMoveTo] = ' ';

								sBoard[iMoveFrom] = sBoard[iMoveTo];
								sBoard[iMoveTo] = ' ';
								return false;
							}
							else
							{
								iWhiteKingLocation = iMoveTo;
								dHistory.push_back("White King Location: " + std::to_string(iWhiteKingLocation));
								return true;
							}
						}
						else
						{
							if (CheckBoard[iMoveTo] == 'x')
							{
								dHistory.push_back("ERR: You can't move your king into check!");
								sErrorMsg = "You can't move your king into check!";
								this->iMoves--;
								this->iPosition = iMoveFrom;

								iBoard[this->iPosition] = this->cVisual;
								iBoard[iMoveTo] = ' ';

								sBoard[iMoveFrom] = sBoard[iMoveTo];
								sBoard[iMoveTo] = ' ';
								return false;
							}
							else
							{
								iBlackKingLocation = iMoveTo;
								dHistory.push_back("Black King Location: " + std::to_string(iBlackKingLocation));
								return true;
							}
						}
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}

		}	//	END SetPosition
		void SetPiece(int iType, bool bWhitePiece, int iPieceNum)			//				SetPiece
		{
			if (iType == 0)	//	for KING pieces
			{
				this->iType = 0;
				this->iValue = 0;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 60;
					iBoard[this->iPosition] = 'K';
					this->cVisual = 'K';
					sBoard[this->iPosition] = "wKing";
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 4;
					iBoard[this->iPosition] = 'k';
					this->cVisual = 'k';
					sBoard[this->iPosition] = "bKing";
				}

			}
			else if (iType == 1)	//	for QUEEN pieces
			{
				this->iType = 1;
				this->iValue = 9;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 59;
					iBoard[this->iPosition] = 'Q';
					this->cVisual = 'Q';
					sBoard[this->iPosition] = "wQueen";
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 3;
					iBoard[this->iPosition] = 'q';
					this->cVisual = 'q';
					sBoard[this->iPosition] = "bQueen";
				}
			}
			else if (iType == 2)	//	for ROOK pieces
			{
				this->iType = 2;
				this->iValue = 5;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 56;
					if (iPieceNum == 2)
					{
						this->iPosition += 7;
					}
					iBoard[this->iPosition] = 'R';
					this->cVisual = 'R';
					sBoard[this->iPosition] = "wRook" + std::to_string(iPieceNum);
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 0;
					if (iPieceNum == 2)
					{
						this->iPosition += 7;
					}
					iBoard[this->iPosition] = 'r';
					this->cVisual = 'r';
					sBoard[this->iPosition] = "bRook" + std::to_string(iPieceNum);
				}
			}
			else if (iType == 3)	//	for BISHOP pieces
			{
				this->iType = 3;
				this->iValue = 3;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 58;
					if (iPieceNum == 2)
					{
						this->iPosition += 3;
					}
					iBoard[this->iPosition] = 'B';
					this->cVisual = 'B';
					sBoard[this->iPosition] = "wBishop" + std::to_string(iPieceNum);
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 2;
					if (iPieceNum == 2)
					{
						this->iPosition += 3;
					}
					iBoard[this->iPosition] = 'b';
					this->cVisual = 'b';
					sBoard[this->iPosition] = "bBishop" + std::to_string(iPieceNum);
				}

			}
			else if (iType == 4)	//	for KNIGHT pieces
			{
				this->iType = 4;
				this->iValue = 3;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 57;
					if (iPieceNum == 2)
					{
						this->iPosition += 5;
					}
					iBoard[this->iPosition] = 'N';
					this->cVisual = 'N';
					sBoard[this->iPosition] = "wKnight" + std::to_string(iPieceNum);
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 1;
					if (iPieceNum == 2)
					{
						this->iPosition += 5;
					}
					iBoard[this->iPosition] = 'n';
					this->cVisual = 'n';
					sBoard[this->iPosition] = "bKnight" + std::to_string(iPieceNum);
				}
			}
			else if (iType == 5)	//	for PAWN pieces
			{
				this->iType = 5;
				this->iValue = 1;
				this->bWhitePiece = bWhitePiece;
				this->iMoves = 0;

				if (bWhitePiece == true)
				{
					this->iPosition = 48;
					if (iPieceNum > 1)
					{
						this->iPosition += iPieceNum - 1;
					}
					iBoard[this->iPosition] = 'P';
					this->cVisual = 'P';
					sBoard[this->iPosition] = "wPawn" + std::to_string(iPieceNum);
				}
				else if (bWhitePiece == false)
				{
					this->iPosition = 8;
					if (iPieceNum > 1)
					{
						this->iPosition += iPieceNum - 1;
					}
					iBoard[this->iPosition] = 'p';
					this->cVisual = 'p';
					sBoard[this->iPosition] = "bPawn" + std::to_string(iPieceNum);
				}
			}
			else
			{
				std::cerr << "This piece could not be initalized. Please try again." << std::endl;
			}

		}	//	END SetPiece

		//	Getters
		int GetPosition(void)
		{
			return(this->iPosition);
		}
		int GetValue(void)
		{
			return(this->iValue);
		}
		int GetType(void)
		{
			return(this->iType);
		}
		char GetCVisual(void)
		{
			return(this->cVisual);
		}

		//	Other Functions

	} // Declare WHITE ChessLogic_H::Piece Objects
		wKing,
		wQueen,

		wRook1,
		wRook2,

		wBishop1,
		wBishop2,

		wKnight1,
		wKnight2,

		wPawn1,
		wPawn2,
		wPawn3,
		wPawn4,
		wPawn5,
		wPawn6,
		wPawn7,
		wPawn8,

		// Declare BLACK Objects			------- ADD extras for PAWN PROMOTION
		bKing,
		bQueen,

		bRook1,
		bRook2,

		bBishop1,
		bBishop2,

		bKnight1,
		bKnight2,

		bPawn1,
		bPawn2,
		bPawn3,
		bPawn4,
		bPawn5,
		bPawn6,
		bPawn7,
		bPawn8;	//	END class Piece

	void vPieceInit()
	{
		// Initalize White Pieces
		wKing.SetPiece(0, true, 0);
		wQueen.SetPiece(1, true, 0);
		iWhiteKingLocation = 60;

		wRook1.SetPiece(2, true, 1);
		wRook2.SetPiece(2, true, 2);

		wBishop1.SetPiece(3, true, 1);
		wBishop2.SetPiece(3, true, 2);

		wKnight1.SetPiece(4, true, 1);
		wKnight2.SetPiece(4, true, 2);

		wPawn1.SetPiece(5, true, 1);
		wPawn2.SetPiece(5, true, 2);
		wPawn3.SetPiece(5, true, 3);
		wPawn4.SetPiece(5, true, 4);
		wPawn5.SetPiece(5, true, 5);
		wPawn6.SetPiece(5, true, 6);
		wPawn7.SetPiece(5, true, 7);
		wPawn8.SetPiece(5, true, 8);

		// Initalize Black Pieces
		bKing.SetPiece(0, false, 0);
		bQueen.SetPiece(1, false, 0);
		iBlackKingLocation = 4;

		bRook1.SetPiece(2, false, 1);
		bRook2.SetPiece(2, false, 2);

		bBishop1.SetPiece(3, false, 1);
		bBishop2.SetPiece(3, false, 2);

		bKnight1.SetPiece(4, false, 1);
		bKnight2.SetPiece(4, false, 2);

		bPawn1.SetPiece(5, false, 1);
		bPawn2.SetPiece(5, false, 2);
		bPawn3.SetPiece(5, false, 3);
		bPawn4.SetPiece(5, false, 4);
		bPawn5.SetPiece(5, false, 5);
		bPawn6.SetPiece(5, false, 6);
		bPawn7.SetPiece(5, false, 7);
		bPawn8.SetPiece(5, false, 8);
	}
	bool bSearchObj()	//----------bSearchObj()----------//----------bSearchObj()----------//----------bSearchObj()----------
	{
		bool lValidMove = false;
		bMoveCheck = false;
		std::string pName = sBoard[iMoveFrom];

		if (pName == "wKing")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wKing.SetPosition();
		}
		else if (pName == "bKing")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bKing.SetPosition();
		}
		else if (pName == "wQueen")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wQueen.SetPosition();
		}
		else if (pName == "bQueen")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bQueen.SetPosition();
		}
		else if (pName == "wRook1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wRook1.SetPosition();
		}
		else if (pName == "wRook2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wRook2.SetPosition();
		}
		else if (pName == "bRook1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bRook1.SetPosition();
		}
		else if (pName == "bRook2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bRook2.SetPosition();
		}
		else if (pName == "wBishop1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wBishop1.SetPosition();
		}
		else if (pName == "wBishop2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wBishop2.SetPosition();
		}
		else if (pName == "bBishop1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bBishop1.SetPosition();
		}
		else if (pName == "bBishop2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bBishop2.SetPosition();
		}
		else if (pName == "wKnight1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wKnight1.SetPosition();
		}
		else if (pName == "wKnight2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wKnight2.SetPosition();
		}
		else if (pName == "bKnight1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bKnight1.SetPosition();
		}
		else if (pName == "bKnight2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bKnight2.SetPosition();
		}
		else if (pName == "wPawn1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn1.SetPosition();
		}
		else if (pName == "wPawn2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn2.SetPosition();
		}
		else if (pName == "wPawn3")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn3.SetPosition();
		}
		else if (pName == "wPawn4")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn4.SetPosition();
		}
		else if (pName == "wPawn5")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn5.SetPosition();
		}
		else if (pName == "wPawn6")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn6.SetPosition();
		}
		else if (pName == "wPawn7")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn7.SetPosition();
		}
		else if (pName == "wPawn8")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = wPawn8.SetPosition();
		}
		else if (pName == "bPawn1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn1.SetPosition();
		}
		else if (pName == "bPawn2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn2.SetPosition();
		}
		else if (pName == "bPawn3")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn3.SetPosition();
		}
		else if (pName == "bPawn4")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn4.SetPosition();
		}
		else if (pName == "bPawn5")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn5.SetPosition();
		}
		else if (pName == "bPawn6")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn6.SetPosition();
		}
		else if (pName == "bPawn7")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn7.SetPosition();
		}
		else if (pName == "bPawn8")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			lValidMove = bPawn8.SetPosition();
		}
		else
		{
			dHistory.push_back("ERR: \"" + pName + "\" could not be moved.");
			sErrorMsg = "There isn't a piece there!";
			return false;
		}

		if (lValidMove == true)
		{
			if (CurrentColorIsWhite == true)
			{
				CurrentColorIsWhite = false;
				wMoves++;
			}
			else if (CurrentColorIsWhite == false)
			{
				CurrentColorIsWhite = true;
				bMoves++;
			}

			return true;
		}
		return false;
	}//		END bSearchObj()			//			END bSearchObj()			//		END bSearchObj()

	void vMoveCheck()	//----------vMoveCheck()----------//-----------vMoveCheck()----------//----------vMoveCheck()-----------//----------vMoveCheck()---------
	{
		bool lValidMove = false;
		bMoveCheck = true;
		std::string pName = sBoard[iMoveFrom];

		for (unsigned int i = 0; i < iBoardSize; i++)
		{
			CheckBoard[i] = ' ';
			cMoveBoard[i] = ' ';
		}

		if (pName == "wKing")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wKing.SetPosition();
		}
		else if (pName == "bKing")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bKing.SetPosition();
		}
		else if (pName == "wQueen")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wQueen.SetPosition();
		}
		else if (pName == "bQueen")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bQueen.SetPosition();
		}
		else if (pName == "wRook1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wRook1.SetPosition();
		}
		else if (pName == "wRook2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wRook2.SetPosition();
		}
		else if (pName == "bRook1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bRook1.SetPosition();
		}
		else if (pName == "bRook2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bRook2.SetPosition();
		}
		else if (pName == "wBishop1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wBishop1.SetPosition();
		}
		else if (pName == "wBishop2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wBishop2.SetPosition();
		}
		else if (pName == "bBishop1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bBishop1.SetPosition();
		}
		else if (pName == "bBishop2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bBishop2.SetPosition();
		}
		else if (pName == "wKnight1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wKnight1.SetPosition();
		}
		else if (pName == "wKnight2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wKnight2.SetPosition();
		}
		else if (pName == "bKnight1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bKnight1.SetPosition();
		}
		else if (pName == "bKnight2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bKnight2.SetPosition();
		}
		else if (pName == "wPawn1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn1.SetPosition();
		}
		else if (pName == "wPawn2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn2.SetPosition();
		}
		else if (pName == "wPawn3")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn3.SetPosition();
		}
		else if (pName == "wPawn4")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn4.SetPosition();
		}
		else if (pName == "wPawn5")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn5.SetPosition();
		}
		else if (pName == "wPawn6")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn6.SetPosition();
		}
		else if (pName == "wPawn7")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn7.SetPosition();
		}
		else if (pName == "wPawn8")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			wPawn8.SetPosition();
		}
		else if (pName == "bPawn1")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn1.SetPosition();
		}
		else if (pName == "bPawn2")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn2.SetPosition();
		}
		else if (pName == "bPawn3")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn3.SetPosition();
		}
		else if (pName == "bPawn4")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn4.SetPosition();
		}
		else if (pName == "bPawn5")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn5.SetPosition();
		}
		else if (pName == "bPawn6")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn6.SetPosition();
		}
		else if (pName == "bPawn7")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn7.SetPosition();
		}
		else if (pName == "bPawn8")
		{
			dHistory.push_back("INFO: Piece at iMoveFrom is: \"" + pName + "\".");
			bPawn8.SetPosition();
		}
		else
		{
			dHistory.push_back("ERR: No piece at " + std::to_string(iMoveFrom));
			sErrorMsg = "There isn't a piece there!";
		}

		for (unsigned int i = 0; i < iBoardSize; i++)
		{
			cMoveBoard[i] = CheckBoard[i];
		}
	}	//		END vMoveCheck()			//			END vMoveCheck()			//		END vMoveCheck()

	bool bIsKingInCheck()	//----------bIsKingInCheck()----------//---------bIsKingInCheck()------------//----------bIsKingInCheck()----------
	{
		for (unsigned int i = 0; i < iBoardSize; i++)
		{
			CheckBoard[i] = ' ';
		}
		dHistory.push_back("Is King in Check?");
		bMoveCheck = true;

		if (CurrentColorIsWhite)
		{
			for (unsigned int i = 0; i < iBoardSize; i++)
			{
				std::string pName = sBoard[i];
				if (pName == "wKing")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wKing.SetPosition();
				}
				else if (pName == "wQueen")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wQueen.SetPosition();
				}
				else if (pName == "wRook1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wRook1.SetPosition();
				}
				else if (pName == "wRook2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wRook2.SetPosition();
				}
				else if (pName == "wBishop1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wBishop1.SetPosition();
				}
				else if (pName == "wBishop2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wBishop2.SetPosition();
				}
				else if (pName == "wKnight1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wKnight1.SetPosition();
				}
				else if (pName == "wKnight2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wKnight2.SetPosition();
				}
				else if (pName == "wPawn1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn1.SetPosition();
				}
				else if (pName == "wPawn2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn2.SetPosition();
				}
				else if (pName == "wPawn3")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn3.SetPosition();
				}
				else if (pName == "wPawn4")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn4.SetPosition();
				}
				else if (pName == "wPawn5")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn5.SetPosition();
				}
				else if (pName == "wPawn6")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn6.SetPosition();
				}
				else if (pName == "wPawn7")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn7.SetPosition();
				}
				else if (pName == "wPawn8")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					wPawn8.SetPosition();
				}
			}
			if (CheckBoard[iBlackKingLocation] == 'x')
			{
				dHistory.push_back("INFO: BLACK King is in check!");
				return true;
			}
		}
		else
		{
			for (unsigned int i = 0; i < iBoardSize; i++)
			{
				std::string pName = sBoard[i];

				if (pName == "bKing")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bKing.SetPosition();
				}
				else if (pName == "bQueen")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bQueen.SetPosition();
				}
				else if (pName == "bRook1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bRook1.SetPosition();
				}
				else if (pName == "bRook2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bRook2.SetPosition();
				}
				else if (pName == "bBishop1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bBishop1.SetPosition();
				}
				else if (pName == "bBishop2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bBishop2.SetPosition();
				}
				else if (pName == "bKnight1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bKnight1.SetPosition();
				}
				else if (pName == "bKnight2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bKnight2.SetPosition();
				}
				else if (pName == "bPawn1")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn1.SetPosition();
				}
				else if (pName == "bPawn2")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn2.SetPosition();
				}
				else if (pName == "bPawn3")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn3.SetPosition();
				}
				else if (pName == "bPawn4")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn4.SetPosition();
				}
				else if (pName == "bPawn5")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn5.SetPosition();
				}
				else if (pName == "bPawn6")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn6.SetPosition();
				}
				else if (pName == "bPawn7")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn7.SetPosition();
				}
				else if (pName == "bPawn8")
				{
					dHistory.push_back("INFO: Piece at " + std::to_string(i) + " is: \"" + pName + "\".");
					bPawn8.SetPosition();
				}
			}
			if (CheckBoard[iWhiteKingLocation] == 'x')
			{
				dHistory.push_back("INFO: WHITE King in in check!");
				return true;
			}
		}
		return false;
	}	//		END bIsKingInCheck()			//			END bIsKingInCheck()			//		END bIsKingInCheck()

	bool bIsKingInCheckmate()	//----------bIsKingInCheckmate()-------//----------bIsKingInCheckmate()----------//----------bIsKingInCheckmate()----------
	{
		bool lCheckmate = false;
		bMoveCheck = true;
		for (unsigned int i = 0; i < iBoardSize; i++)
		{
			CheckmateBoard[i] = CheckBoard[i];
			CheckBoard[i] = ' ';
		}

		if (bWhiteKingInCheck == true)
		{
			wKing.SetPosition();
			for (unsigned int i = 0; i < iBoardSize; i++)
			{
				if (CheckBoard[i] == 'x' && CheckmateBoard[i] == ' ')
				{
					dHistory.push_back("Possible move at " + std::to_string(i));
				}
				else
				{
					lCheckmate = true;
				}
			}
		}
		else if (bBlackKingInCheck == true)
		{
			bKing.SetPosition();
			for (unsigned int i = 0; i < iBoardSize; i++)
			{
				if (CheckBoard[i] == 'x' && CheckmateBoard[i] == ' ')
				{
					dHistory.push_back("Possible move at " + std::to_string(i));
				}
				else
				{
					lCheckmate = true;
				}
			}
		}
		for (unsigned int i = 0; i < iBoardSize; i++)
		{
			CheckBoard[i] = CheckmateBoard[i];
			CheckmateBoard[i] = ' ';
		}

		if (lCheckmate == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}	//		END bIsKingInCheckmate()			//			END bIsKingInCheckmate()			//		END bIsKingInCheckmate()

	bool bRookCastle()
	{
		bool lValidMove = false;
		if (bCastleSideQueen)
		{
			if (CurrentColorIsWhite)
				lValidMove = wRook1.SetPosition();
			else
				lValidMove = bRook1.SetPosition();
		}
		else
		{
			if (CurrentColorIsWhite)
				lValidMove = wRook2.SetPosition();
			else
				lValidMove = bRook2.SetPosition();
		}
		return lValidMove;
	}
} // END namspace ChessLogic_H
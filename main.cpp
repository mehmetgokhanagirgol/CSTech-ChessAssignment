#include <iostream>
#include <vector>
#include <string>

using namespace std;


/* We will take inputs from the text file and convert it to a vector<vector<box>>
I use box here to check if a piece was already attacked by another piece, so that I dont keep halving the value of the piece
Next , I calculate the total points of the pieces without threats. Then I substract the threatpenalties to come with the result.
*/

struct box{
    string piece;
    bool underThreat;
};

bool inBounds(int i, int j){
    return i >= 0 && i < 8 && j >= 0 && j < 8;
}

double checkPiece(char piece){
    double piecePoint;
    switch (piece){
        case 'p':
            piecePoint = 1;
            break;
        case 'a':
            piecePoint = 3;
            break;
        case 'f':
            piecePoint = 3;
            break;
        case 'k':
            piecePoint = 5;
            break;
        case 'v':
            piecePoint = 9;
            break;
        case 's':
            piecePoint = 100;
            break;
        default :
            piecePoint = 0;
            break;
    }
    return piecePoint;
}

double checkQueenDirection(int i, int j, char side, vector<vector<box>> &board, double penalty, int directionX, int directionY){
    double piecePoint;
    // go in the direction given till out of bounds or hitting another piece
    // if we hit check which side, if ally do nothing if enemy threathen it
    while(inBounds(i+=directionX, j+=directionY)){
            if(board[i][j].piece.compare("--")){
                //hit
                if(board[i][j].piece[1] == side)
                    break;
                else{
                    if(!board[i][j].underThreat){
                        piecePoint = checkPiece(board[i][j].piece[0]);
                        penalty += piecePoint/2;
                        board[i][j].underThreat = true;
                    }
                    break; 
                }
            }
        }
    return penalty;
}


double checkPawnThreat(int i, int j, char side, vector<vector<box>> &board){
    // it will be easier to check pawn point by point since it has no piercing action
    double piecePoint;
    double penalty = 0;
    // check pawnthreat squares if the piece was already under threat do nothing
    if(side == 's'){
        if(inBounds(i+1, j+1) && board[i+1][j+1].piece[1] == 'b'){
            if(!board[i+1][j+1].underThreat){
                piecePoint = checkPiece(board[i+1][j+1].piece[0]);
                penalty += piecePoint/2;
                board[i+1][j+1].underThreat = true;
            }
        }
        if(inBounds(i+1, j-1) && board[i+1][j-1].piece[1] == 'b'){
            if(!board[i+1][j-1].underThreat){
                piecePoint = checkPiece(board[i+1][j-1].piece[0]);
                penalty += piecePoint/2;
                board[i+1][j-1].underThreat = true;
            }
        }
    }
    else{
        if(inBounds(i-1, j+1) && board[i-1][j+1].piece[1] == 's'){
            if(!board[i-1][j+1].underThreat){
                piecePoint = checkPiece(board[i-1][j+1].piece[0]);
                penalty += piecePoint/2;
                board[i-1][j+1].underThreat = true;
            }
        }
        if(inBounds(i-1, j-1) && board[i-1][j-1].piece[1] == 's'){
            if(!board[i-1][j-1].underThreat){
                piecePoint = checkPiece(board[i-1][j-1].piece[0]);
                penalty += piecePoint/2;
                board[i-1][j-1].underThreat = true;
            }
        }
    }
    return penalty;
}

double checkKnightThreat(int i, int j, char side, vector<vector<box>> &board){
    // it will be easier to check knight point by point since it has no piercing action
    // But 8 if's would be too much for readability, lets loop through threat square vector that we define
    double piecePoint;
    double penalty = 0;
    char oppositeSide;
    if(side == 'b')
        oppositeSide = 's';
    else
        oppositeSide = 'b';
    // knight threat vector = ktv, knight goes L shape so combinations of 2,1
    vector<pair<int, int>> ktv = {{2, 1},{2, -1},{-2, 1},{-2, -1},{1, 2},{1, -2},{-1, -2},{-1, 2}};
    for(unsigned long k = 0; k < ktv.size(); ++k){
        if(inBounds(i+ktv[k].first, j+ktv[k].second) && board[i+ktv[k].first][j+ktv[k].second].piece[1] == oppositeSide){
            if(!board[i+ktv[k].first][j+ktv[k].second].underThreat){
                piecePoint = checkPiece(board[i+ktv[k].first][j+ktv[k].second].piece[0]);
                penalty += piecePoint/2;
                board[i+ktv[k].first][j+ktv[k].second].underThreat = true;
            }
        }
    }


    return penalty;
}

double checkQueenThreat(int i, int j, char side, vector<vector<box>> &board){
    // this could be tricky as queen has a range not points for attack
    // when hit comes we should break out of the diagonal and go next
    // we should check right,left,up,down and diagonals seperately when we hit a piece if its opposite add a penalty and break, if not just break.
    double penalty = 0;
    vector<pair<int, int>> directions = {{-1,0},{1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};

    for(unsigned long k = 0; k < directions.size(); ++k){
        penalty = checkQueenDirection(i, j, side, board, penalty, directions[k].first, directions[k].second);
    }
        
    return penalty;
}

void takeInput(vector<vector<box>> &board){
    int row = 8;
    int col = 8;
    string piece;
    for(auto i = 0; i < row; ++i){
        board.push_back({});
        for(auto j = 0; j < col; ++j){
            cin >> piece;
            board[i].push_back({piece, false});
        }
    }
}

pair<double, double> calculatePoints(vector<vector<box>> board){
    double whitePoints = 0;
    double blackPoints = 0;
    double piecePoint;
    // for every piece add its point to its color
    for(auto i = 0; i < 8; ++i){
        for(auto j = 0; j < 8; ++j){
            if(board[i][j].piece[1] == 's'){
                piecePoint = checkPiece(board[i][j].piece[0]);
                blackPoints += piecePoint;
            }
            else if(board[i][j].piece[1] == 'b'){
                piecePoint = checkPiece(board[i][j].piece[0]);
                whitePoints += piecePoint;
            }
        }
    }
    return make_pair(whitePoints, blackPoints);
}

double threatPenalty(char side, vector<vector<box>> &board){
    // this function will only take care of queen, pawn and knight threats as indicated on the assignment mail
    char piece;
    double penalty = 0;
    // for every black queen, pawn and knight check radius if hit opponent increase the penalty by half value of the piece
    for(auto i = 0; i < 8; ++i){
        for(auto j = 0; j < 8; ++j){
            if(board[i][j].piece[1] == side){
                piece = board[i][j].piece[0];
                switch (piece){
                    case 'p':
                        penalty += checkPawnThreat(i, j, side, board);
                        break;
                    case 'a':
                        penalty += checkKnightThreat(i, j, side, board);
                        break;
                    case 'v':
                        penalty += checkQueenThreat(i, j, side, board);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    return penalty;
}


int main(){
    vector<vector<box>> board;
    pair<double, double> points;
    double threatPenaltyWhite;
    double threatPenaltyBlack;
    // taking the input from the input.txt file
    takeInput(board);
    // calculate piece points for each player
    points = calculatePoints(board);
    // substract threat penalties from each party
    // I will use 0 to indicate white and 1 to indicate black
    threatPenaltyWhite = threatPenalty('s', board);
    threatPenaltyBlack = threatPenalty('b', board);
    cout << "Siyah:" << points.second - threatPenaltyBlack << " " << "Beyaz:" << points.first - threatPenaltyWhite << endl;
    return 0;
}
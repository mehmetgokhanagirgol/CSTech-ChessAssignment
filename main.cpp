#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

double checkPawnThreat(int i, int j, int side, vector<vector<box>> &board){
    // it will be easier to check pawn point by point since it has no piercing action
    double piecePoint;
    double penalty = 0;
    // check pawnthreat squares if the piece was already under threat do nothing
    if(side == 1){
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

double checkKnightThreat(int i, int j, int side, vector<vector<box>> &board){
    // it will be easier to check knight point by point since it has no piercing action
    // But 8 if's would be too much for readability, lets loop through threat square vector that we define
    double piecePoint;
    double penalty = 0;
    // knight threat vector = ktv
    vector<pair<int, int>> ktv = {{2, 1},{2, -1},{-2, 1},{-2, -1},{1, 2},{1, -2},{-1, -2},{-1, 2}};
    if(side == 1){
        for(unsigned long k = 0; k < ktv.size(); ++k){
            if(inBounds(i+ktv[k].first, j+ktv[k].second) && board[i+ktv[k].first][j+ktv[k].second].piece[1] == 'b'){
                if(!board[i+ktv[k].first][j+ktv[k].second].underThreat){
                    piecePoint = checkPiece(board[i+ktv[k].first][j+ktv[k].second].piece[0]);
                    penalty += piecePoint/2;
                    board[i+ktv[k].first][j+ktv[k].second].underThreat = true;
                }
            }
        }
    }
    else{
        for(unsigned long k = 0; k < ktv.size(); ++k){
            if(inBounds(i+ktv[k].first, j+ktv[k].second) && board[i+ktv[k].first][j+ktv[k].second].piece[1] == 's'){
                if(!board[i+ktv[k].first][j+ktv[k].second].underThreat){
                    piecePoint = checkPiece(board[i+ktv[k].first][j+ktv[k].second].piece[0]);
                    penalty += piecePoint/2;
                    board[i+ktv[k].first][j+ktv[k].second].underThreat = true;
                }
            }
        }
    }
    return penalty;
}

double checkQueenThreat(int i, int j, int side, vector<vector<box>> &board){
    return 0;
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

double threatPenalty(int side, vector<vector<box>> &board){
    // this function will only take care of queen, pawn and knight threats as indicated on the assignment mail
    // white
    char piece;
    double penalty = 0;
    if(side == 1){
        // for every black queen, pawn and knight check radius if hit opponent increase the penalty by half value of the piece
        for(auto i = 0; i < 8; ++i){
            for(auto j = 0; j < 8; ++j){
                if(board[i][j].piece[1] == 's'){
                    piece = board[i][j].piece[0];
                    switch (piece){
                        case 'p':
                            penalty += checkPawnThreat(i, j, 1, board);
                            break;
                        case 'a':
                            penalty += checkKnightThreat(i, j, 1, board);
                            break;
                        case 'v':
                            penalty += checkQueenThreat(i, j, 1, board);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    // black
    else{
        for(auto i = 0; i < 8; ++i){
            for(auto j = 0; j < 8; ++j){
                if(board[i][j].piece[1] == 'b'){
                    piece = board[i][j].piece[0];
                    switch (piece){
                        case 'p':
                            penalty += checkPawnThreat(i, j, 0, board);
                            break;
                        case 'a':
                            penalty += checkKnightThreat(i, j, 0, board);
                            break;
                        case 'v':
                            penalty += checkQueenThreat(i, j, 0, board);
                            break;
                        default:
                            break;
                    }
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
    threatPenaltyWhite = threatPenalty(1, board);
    threatPenaltyBlack = threatPenalty(0, board);
    cout << points.first - threatPenaltyWhite << " " << points.second - threatPenaltyBlack << endl;
    return 0;
}
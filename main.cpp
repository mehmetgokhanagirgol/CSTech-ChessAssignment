#include <iostream>
#include <vector>
#include <string>

using namespace std;

int checkPiece(char piece){
    int piecePoint;
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

void takeInput(vector<vector<string>> &board){
    int row = 8;
    int col = 8;
    string piece;
    for(auto i = 0; i < row; ++i){
        board.push_back({});
        for(auto j = 0; j < col; ++j){
            cin >> piece;
            board[i].push_back(piece);
        }
    }
}

pair<double, double> calculatePoints(vector<vector<string>> board){
    double whitePoints = 0;
    double blackPoints = 0;
    int piecePoint;
    for(auto i = 0; i < 8; ++i){
        for(auto j = 0; j < 8; ++j){
            if(board[i][j][1] == 's'){
                piecePoint = checkPiece(board[i][j][0]);
                blackPoints += piecePoint;
            }
            else if(board[i][j][1] == 'b'){
                piecePoint = checkPiece(board[i][j][0]);
                whitePoints += piecePoint;
            }
        }
    }
    return make_pair(whitePoints, blackPoints);
}


int main(){
    vector<vector<string>> board;
    pair<double, double> points;
    // taking the input from the input.txt file
    takeInput(board);
    // calculate piece points for each player
    points = calculatePoints(board);
    cout << points.first << " " << points.second << endl;
    return 0;
}
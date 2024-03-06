#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;




struct GameState {
    float uctVal = 1000;
    bool childExists = false;
    int moveNum;
    char ourMove = 'X';
    int numOfVisits = 0;
    int numOfWins = 0;
    int numOfChildren;
    float exploration = 1.414;
    GameState* parentsPtr;
    GameState* thisPtr;
    vector<GameState*> childrenPtr;
    string currentGameState;


    int UCTcalAndFinder(){

        int highestIndex = 0;
        float highestVal = 0;

        for (int i = 0 ; i < childrenPtr.size() ; i++) {
            if(childrenPtr[i]->numOfVisits == 0){
                childrenPtr[i]->uctVal = 2000;
            }
            else{
                childrenPtr[i]->uctVal = childrenPtr[i]->numOfWins/childrenPtr[i]->numOfVisits + exploration * sqrt(log(numOfVisits)/childrenPtr[i]->numOfVisits);
            }
            if (childrenPtr[i]->uctVal > highestVal){
                highestVal = childrenPtr[i]->uctVal;
                highestIndex = i;
            }
        }

        return highestIndex;
    }



};



class Bot{

    public:

    string gameState;
    int legalMoves[7] = {-1,-1,-1,-1,-1,-1,-1};
    int numOfLegalMoves;

    int movesCheck[7][6] = {{35,28,21,14,7,0},{36,29,22,15,8,1},{37,30,23,16,9,2},{38,31,24,17,10,3},
                            {39,32,25,18,11,4},{40,33,26,19,12,5},{41,34,27,20,13,6}};

    void arrayPrinter(int arr[],int lenn){
        for (int i = 0 ; i < lenn ; i++){
            cout << arr[i] << " ";
        }
    }


    void legalMovesFinder(string GS){
        numOfLegalMoves = 0;
        for (int i = 0 ; i < 7 ; i++){
            
            if (GS[i] != '-'){
                continue;
            }
            
            for (int j = 0 ; j < 6 ; j++){
                if (GS[movesCheck[i][j]] == '-'){
                    legalMoves[numOfLegalMoves] = movesCheck[i][j];
                    break;
                } 
            }
            numOfLegalMoves += 1;
        }
    }


    void nodesCreator(GameState* ptrr){
        if (ptrr->childExists == false){
            legalMovesFinder(ptrr->currentGameState);
            for (int i = 0 ; i < numOfLegalMoves ; i++){
                GameState* newPtrr = new GameState;
                newPtrr->parentsPtr = ptrr;
                if (ptrr->ourMove == 'X'){
                    newPtrr->ourMove = 'O';
                }
                newPtrr->thisPtr = newPtrr;
                newPtrr->moveNum = legalMoves[i];

                newPtrr->currentGameState = ptrr->currentGameState;
                newPtrr->currentGameState[legalMoves[i]] = newPtrr->ourMove;

                ptrr->childrenPtr.push_back(newPtrr);
            }
        ptrr->childExists = true;
        }
    }


    int simulator(){
        GameState firstState;
        firstState.currentGameState = gameState;
        firstState.numOfVisits = 0;
        int numOfsimul = 4000;
        int daChild;

        legalMovesFinder(gameState);
        // for (int j = 0 ; j < numOfLegalMoves ; j++){
        //     cout << legalMoves[j] << " ";
        // }

        
        // cout << endl;

        for (int k = 0 ; k < numOfsimul ; k++){

            GameState* currentPtr = &firstState;
            firstState.numOfVisits += 1;
            int count = 0;


            while (checkGameWon(currentPtr->currentGameState) == false){
                count += 1;
                if (checkGameEnd(currentPtr->currentGameState) == true){
                    break;
                }
                nodesCreator(currentPtr);
                daChild = currentPtr->UCTcalAndFinder();

                //MAKE THE MOVE
                currentPtr = currentPtr->childrenPtr[daChild];
                currentPtr->numOfVisits += 1;

            };

            if (checkGameWon(currentPtr->currentGameState) == true){
                GameState* test;
                while (currentPtr != &firstState){
                    currentPtr->numOfWins += 1;
                    if(currentPtr->parentsPtr == &firstState){
                        break;
                    }
                    test = currentPtr->parentsPtr;
                    currentPtr = test->parentsPtr;
                }
            }


        }

        
        return firstState.childrenPtr[firstState.UCTcalAndFinder()]->moveNum;
        




    }


     Bot(string gs){
    // Bot(vector<string> gamestate, string gs){
        // for (int i = 0 ; i < 42 ; i++){
        //     gameStatee.push_back(gamestate[i]);
        // }    
        gameState = gs;
        // cout << "Has someone already won : " << checkGameWon(gameState) << endl;
        // cout << "Best move is " << simulator() << endl;
    }


    bool checkGameEnd(string gameStatee){
        for (int i = 0 ; i < 42 ; i++){
            if (gameStatee[i] == '-'){
                return false;
            }
        }

        return true;
    }
    // bool checkGameWon(const string& gameState) {

    bool checkGameWon(string gameState) {

    int rows = 6;
    int cols = 7;

    vector<vector<char>> grid(rows, vector<char>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = gameState[i * cols + j];
        }
    }

    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col] == '-') {
                continue;
            }
            for (const auto& direction : directions) {
                int dr = direction.first;
                int dc = direction.second;
                for (int step = 1; step <= 3; ++step) {
                    int r = row + step * dr;
                    int c = col + step * dc;
                    if (r >= 0 && r < rows && c >= 0 && c < cols) {
                        if (grid[row][col] == grid[r][c]) {
                            if (step == 3) {
                                return true; 
                            }
                        } else {
                            break;  
                        }
                    }
                }
            }
        }
    }

    return false;  

    }


};




inline int botMove(string gS){
    
Bot ourBot(gS);
return ourBot.simulator() % 7 ;


}
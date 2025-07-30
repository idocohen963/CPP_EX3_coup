// idocohen963@gmail.com
//idc Email: idcohen770@gmail.com
#include "game.hpp"
#include "PLAYER/player.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace coup;

int main() {
    try {
        // Creating the game instance (Singleton)
        Game& game = Game::getInstance();
        
        // Adding players to the game (using Factory instead of direct constructors)
        Player* moshe = game.addPlayer("Moshe", "Governor");
        Player* yossi = game.addPlayer("Yossi", "Spy");
        Player* meirav = game.addPlayer("Meirav", "Baron");
        Player* reut = game.addPlayer("Reut", "General");
        Player* gilad = game.addPlayer("Gilad", "Judge");
        
        // Starting the game
        game.startGame();
        
        vector<string> players = game.getActivePlayersName();
        
        // Expected output:
        // Moshe
        // Yossi
        // Meirav
        // Reut
        // Gilad
        for(string name : players){
            cout << name << endl;
        }

        // Expected output: Moshe
        game.turn();

        cout << "\n=== Round 1: Basic gathering (1 coin each) ===" << endl;
        moshe->gather();
        yossi->gather();
        meirav->gather();
        reut->gather();
        gilad->gather();

        cout << "\n=== Testing: Attempt to play out of turn ===" << endl;
        // Expected exception - Not spy's turn
        try{
            yossi->gather();
        } catch (const std::exception &e){
            cout << "Expected exception - " << e.what() << endl;
        }

        cout << "\n=== Round 2: Governor ability demonstration ===" << endl;
        moshe->gather();
        cout << "Yossi performs tax (should get 2 coins)..." << endl;
        yossi->tax();
        cout << "\n=== Coin status after tax and gather  ===" << endl;
        cout << moshe->getCoins() << endl; // Expected: 2
        cout << yossi->getCoins() << endl; // Expected: 3

        cout << "\n=== Testing: Judge trying to cancel tax (should fail) ===" << endl;
        // Expected exception - Judge cannot undo tax
        try{
            gilad->cancel(*moshe);
        } catch (const std::exception &e) {
            cout << "Expected exception - " << e.what() << endl;
        }

        cout << "\n=== Governor cancels Yossi's tax (special ability) ===" << endl;
        moshe->cancel(*yossi); // Governor undo tax
        cout << yossi->getCoins() << endl; // Expected: 1 (tax was cancelled)

        cout << "\n=== Round 3: More actions ===" << endl;
        meirav->tax();
        reut->gather();
        gilad->gather(); 

        moshe->tax();
        yossi->gather();
        
        // Baron investment - show before/after for special ability
        cout << "\n=== Baron Investment (Special Ability) ===" << endl;
        cout << "Before investment - Meirav: " << meirav->getCoins() << " coins" << endl;
        meirav->invest(); // Baron traded its 3 coins and got 6 
        cout << "After investment - Meirav: " << meirav->getCoins() << " coins" << endl;
        
        reut->gather();
        gilad->gather();
        
        cout << "\n=== preparations for coup ===" << endl;
        moshe->tax();
        yossi->gather();
        meirav->gather();
        reut->gather();
        gilad->gather();

        moshe->tax();
        yossi->gather();
        cout << "Baron's coins before coup: " << meirav->getCoins() << endl; // Expected: 7
        
        // Show coup result - important game-changing action
        cout << "\n=== Coup Action (Game Changing) ===" << endl;
        cout << "Before coup - Active players: " << game.getActivePlayersName().size() << endl;
        meirav->coup(*moshe); // Coup against governor
        cout << "After coup - Active players: " << game.getActivePlayersName().size() << endl;
        
        reut->gather();
        gilad->gather();
        
        cout << "\n=== Continuing the game to 10 coins ===" << endl;
        yossi->gather();
        meirav->gather();
        reut->gather();
        gilad->gather();
        
        // Continue building coins
        yossi->tax();    // Yossi: 6 coins
        meirav->gather();
        reut->gather();
        gilad->tax();    // Gilad: 4 coins
        
        yossi->tax();    // Yossi: 8 coins
        meirav->gather();
        reut->tax();     // Reut: 4 coins
        gilad->gather();
        
        yossi->tax();    // Yossi: 10 coins

        cout << "\n=== Testing 10+ coins rule ===" << endl;
        meirav->gather();
        reut->gather();
        gilad->gather();
        
        // Now it's Yossi's turn with 10 coins - he must coup!
        cout << "Yossi has 10+ coins and must coup..." << endl;
        try {
            yossi->gather(); // This should fail
        } catch (const std::exception &e) {
            cout << "Expected exception: " << e.what() << endl;
        }
        
        cout << "Yossi is forced to coup..." << endl;
        yossi->coup(*meirav); // Must perform coup
        cout << "After coup - Active players: " << game.getActivePlayersName().size() << endl;

        cout << "\n=== Final active players ===" << endl;
        players = game.getActivePlayersName();
        // Since no one blocked the Baron, the expected output is:
        // Yossi
        // Reut
        // Gilad
        for (string name : players) {
            cout << name << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
// idocohen963@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <vector>
#include <string>
#include "GAME/game.hpp"
#include "PLAYER/player.hpp"
#include "PLAYER/governor.hpp"
#include "PLAYER/spy.hpp"
#include "PLAYER/general.hpp"
#include "PLAYER/judge.hpp"
#include "PLAYER/merchant.hpp"
#include "PLAYER/baron.hpp"

using namespace coup;

// ============================================================
// HELPER FUNCTIONS FOR TESTS
// ============================================================

/**
 * Helper function to reset the game to initial state
 * Creates a new instance and clears all players
 */
void resetGame() {
    Game& game = Game::getInstance();
    game.reset(); // Use the reset method to clean state
}

/**
 * Helper function to create a simple 2-player game for testing
 */
void createSimpleGame(Game& game) {
    game.addPlayer("Alice", "Governor");
    game.addPlayer("Bob", "Spy");
    game.startGame();
}

/**
 * Helper function to create a full 6-player game for testing
 */
void createFullGame(Game& game) {
    game.addPlayer("Player1", "Governor");
    game.addPlayer("Player2", "Spy");
    game.addPlayer("Player3", "General");
    game.addPlayer("Player4", "Judge");
    game.addPlayer("Player5", "Merchant");
    game.addPlayer("Player6", "Baron");
    game.startGame();
}

// ============================================================
// BASIC FUNCTIONALITY TESTS 
// ============================================================

TEST_CASE("Game creation using Singleton pattern") {
    resetGame(); // Reset game state before test
    
    // Test that we can get a game instance and it's the same instance
    Game& game1 = Game::getInstance();
    Game& game2 = Game::getInstance();
    
    CHECK_EQ(&game1, &game2); // Should be the same instance
}

TEST_CASE("Adding players within allowed limits - 2 players") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    // Add minimum number of players (2) and start game
    Player* player1 = game.addPlayer("Alice", "Governor");
    Player* player2 = game.addPlayer("Bob", "Spy");
    
    CHECK(player1 != nullptr);
    CHECK(player2 != nullptr);
    CHECK_EQ(game.getNumPlayers(), 2);
    
    // Should be able to start game with 2 players
    game.startGame();
    CHECK_EQ(game.getNumPlayers(), 2);
}

TEST_CASE("Adding players within allowed limits - 6 players") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    // Add maximum number of players (6) and start game
    game.addPlayer("Player1", "Governor");
    game.addPlayer("Player2", "Spy");
    game.addPlayer("Player3", "General");
    game.addPlayer("Player4", "Judge");
    game.addPlayer("Player5", "Merchant");
    game.addPlayer("Player6", "Baron");
    
    CHECK_EQ(game.getNumPlayers(), 6);
    
    // Should be able to start game with 6 players
    game.startGame();
    CHECK_EQ(game.getNumPlayers(), 6);
}

TEST_CASE("Initial turn order - first player starts") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // First player added should have the first turn
    CHECK_EQ(game.getCurrentPlayerIndex(), 0);
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Alice");
}

TEST_CASE("Turn progression works correctly") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // Initially Alice's turn (index 0)
    CHECK_EQ(game.getCurrentPlayerIndex(), 0);
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Alice");
    
    // After nextTurn, should be Bob's turn (index 1)
    game.nextTurn();
    CHECK_EQ(game.getCurrentPlayerIndex(), 1);
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Bob");
    
    // After another nextTurn, should cycle back to Alice (index 0)
    game.nextTurn();
    CHECK_EQ(game.getCurrentPlayerIndex(), 0);
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Alice");
}

TEST_CASE("Winner declaration with one player remaining") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // Remove one player from the game (simulate coup or elimination)
    Player* bob = game.getCurrentPlayer(); // Get Bob
    if (bob->getName() != "Bob") {
        game.nextTurn(); // Switch to Bob if needed
        bob = game.getCurrentPlayer();
    }
    
    // Manually set Bob as inactive and update player count
    bob->setActive(false);
    game.setNumPlayers(1);
    
    // Now Alice should be the winner
    std::string winnerName = game.winner();
    CHECK_EQ(winnerName, "Alice");
}

TEST_CASE("Get active players list") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    std::vector<std::string> activeNames = game.getActivePlayersName();
    CHECK_EQ(activeNames.size(), 2);
    CHECK_EQ(activeNames[0], "Alice");
    CHECK_EQ(activeNames[1], "Bob");
}

// ============================================================
// ERROR HANDLING TESTS (Exceptional Cases)
// ============================================================

TEST_CASE("Adding seventh player should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    // Add 6 players first
    createFullGame(game);
    
    // Trying to add 7th player should throw runtime_error
    CHECK_THROWS_AS(game.addPlayer("Player7", "Governor"), std::runtime_error);
}

TEST_CASE("Starting game with less than 2 players should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    // Try starting game with no players
    CHECK_THROWS_AS(game.startGame(), std::runtime_error);
    
    // Add one player and try starting
    game.addPlayer("Lonely", "Governor");
    CHECK_THROWS_AS(game.startGame(), std::runtime_error);
}

TEST_CASE("Adding player after game started should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game); // This calls startGame()
    
    // Trying to add player after game started should throw runtime_error
    CHECK_THROWS_AS(game.addPlayer("Latecomer", "General"), std::runtime_error);
}

TEST_CASE("Declaring winner before game ends should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // With 2 active players, calling winner() should throw runtime_error
    CHECK_THROWS_AS(game.winner(), std::runtime_error);
}

TEST_CASE("Adding player with duplicate name should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    game.addPlayer("Alice", "Governor");
    
    // Trying to add another player with same name should throw runtime_error
    CHECK_THROWS_AS(game.addPlayer("Alice", "Spy"), std::runtime_error);
}

TEST_CASE("Calling functions on empty game should throw exception") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    // Calling turn() with no players should throw runtime_error
    CHECK_THROWS_AS(game.turn(), std::runtime_error);
    
    // Calling getActivePlayersName() with no players should throw runtime_error  
    CHECK_THROWS_AS(game.getActivePlayersName(), std::runtime_error);
}

// ============================================================
// EDGE CASES AND BOUNDARY TESTS
// ============================================================

TEST_CASE("Two player game complete flow") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // Verify initial state
    CHECK_EQ(game.getNumPlayers(), 2);
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Alice");
    
    // Simulate some turns
    game.nextTurn(); // Bob's turn
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Bob");
    
    game.nextTurn(); // Back to Alice
    CHECK_EQ(game.getCurrentPlayer()->getName(), "Alice");
    
    // Simulate elimination of Bob
    game.getPlayers()[1]->setActive(false); // Bob becomes inactive
    game.setNumPlayers(1);
    
    // Alice should be winner
    CHECK_EQ(game.winner(), "Alice");
}

TEST_CASE("Six player game turn progression") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createFullGame(game);
    
    // Verify all 6 players are active
    CHECK_EQ(game.getNumPlayers(), 6);
    std::vector<std::string> names = game.getActivePlayersName();
    CHECK_EQ(names.size(), 6);
    
    // Test turn progression through all players
    std::vector<std::string> expectedOrder = {"Player1", "Player2", "Player3", "Player4", "Player5", "Player6"};
    
    for (int i = 0; i < 12; i++) { // Test 2 full cycles
        int expectedIndex = i % 6;
        CHECK_EQ(game.getCurrentPlayer()->getName(), expectedOrder[expectedIndex]);
        if (i < 11) game.nextTurn(); // Don't advance after last check
    }
}

TEST_CASE("Turn skipping inactive players") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createFullGame(game);
    
    // Make Player2 and Player4 inactive (simulate elimination)
    game.getPlayers()[1]->setActive(false); // Player2
    game.getPlayers()[3]->setActive(false); // Player4
    game.setNumPlayers(4);
    
    // Expected order should skip inactive players: Player1 -> Player3 -> Player5 -> Player6
    std::vector<std::string> expectedActiveOrder = {"Player1", "Player3", "Player5", "Player6"};
    
    for (int i = 0; i < 8; i++) { // Test 2 full cycles
        int expectedIndex = i % 4;
        CHECK_EQ(game.getCurrentPlayer()->getName(), expectedActiveOrder[expectedIndex]);
        if (i < 7) game.nextTurn();
    }
}

TEST_CASE("Immediate winner declaration when one player remains") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // Eliminate Bob immediately
    game.getPlayers()[1]->setActive(false);
    game.setNumPlayers(1);
    
    // Should be able to declare winner immediately, regardless of whose "turn" it is
    CHECK_EQ(game.winner(), "Alice");
}

// ============================================================
// STATE AND INTERACTION TESTS
// ============================================================

TEST_CASE("Merchant bonus with 3 or more coins") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    game.addPlayer("MerchantPlayer", "Merchant");
    game.addPlayer("OtherPlayer", "Governor");
    game.startGame();
    
    Player* merchant = game.getCurrentPlayer();
    if (merchant->getName() != "MerchantPlayer") {
        game.nextTurn();
        merchant = game.getCurrentPlayer();
    }
    
    // Test with exactly 3 coins
    merchant->setCoins(3);
    game.nextTurn(); // Merchant should get bonus when turn advances
    
    // The bonus is applied when nextTurn() is called FROM the merchant
    // So we need to go back to merchant's turn to see the effect
    game.nextTurn(); // Back to merchant
    // Since merchant had 3+ coins when nextTurn was called, should have received bonus
}

TEST_CASE("Merchant bonus with less than 3 coins") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    game.addPlayer("MerchantPlayer", "Merchant");
    game.addPlayer("OtherPlayer", "Governor");
    game.startGame();
    
    Player* merchant = game.getCurrentPlayer();
    if (merchant->getName() != "MerchantPlayer") {
        game.nextTurn();
        merchant = game.getCurrentPlayer();
    }
    
    // Test with 2 coins (less than 3)
    merchant->setCoins(2);
    int coinsBefore = merchant->getCoins();
    game.nextTurn();
    
    // Check that coins didn't increase (no bonus)
    CHECK_EQ(merchant->getCoins(), coinsBefore);
}

TEST_CASE("Bribed flag reset after turn") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    Player* currentPlayer = game.getCurrentPlayer();
    
    // Set player as bribed
    currentPlayer->setIsBribed(true);
    CHECK(currentPlayer->getIsBribed());
    
    // After nextTurn, the bribed flag should be reset
    game.nextTurn();
    CHECK_FALSE(currentPlayer->getIsBribed());
}

TEST_CASE("Player count updates correctly") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createFullGame(game);
    
    // Initially 6 players
    CHECK_EQ(game.getNumPlayers(), 6);
    
    // Simulate successful coup (decrease count)
    game.setNumPlayers(5);
    CHECK_EQ(game.getNumPlayers(), 5);
    
    // Simulate cancelled coup by General (increase count back)
    game.setNumPlayers(6);
    CHECK_EQ(game.getNumPlayers(), 6);
}

TEST_CASE("Last action tracking") {
    resetGame(); // Reset game state before test
    Game& game = Game::getInstance();
    
    createSimpleGame(game);
    
    // Test setting and getting last action
    game.setLastStep(ActionType::Coup);
    CHECK_EQ(game.getLastStep(), ActionType::Coup);
    
    game.setLastStep(ActionType::Bribe);
    CHECK_EQ(game.getLastStep(), ActionType::Bribe);
    
    game.setLastStep(ActionType::Tax);
    CHECK_EQ(game.getLastStep(), ActionType::Tax);
}



#include "doctest.h"
#include <vector>
#include <string>
#include <algorithm>
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
 * Helper function to reset the game to initial state (defined in testGame.cpp)
 */
extern void resetGame();

/**
 * Helper function to create a simple 2-player game for testing
 * Returns pointers to the created players
 */
std::pair<Player*, Player*> createSimpleGame() {
    Game& game = Game::getInstance();
    Player* alice = game.addPlayer("Alice", "Governor");
    Player* bob = game.addPlayer("Bob", "Spy");
    game.startGame();
    return {alice, bob};
}

/**
 * Helper function to create a 3-player game for more complex testing
 */
std::vector<Player*> createThreePlayerGame() {
    Game& game = Game::getInstance();
    Player* alice = game.addPlayer("Alice", "Governor");
    Player* bob = game.addPlayer("Bob", "Spy");
    Player* charlie = game.addPlayer("Charlie", "General");
    game.startGame();
    return {alice, bob, charlie};
}

// ============================================================
// PLAYER CLASS BASIC FUNCTIONALITY TESTS
// ============================================================

TEST_CASE("Player Basic Actions - Happy Path") {
    resetGame();
    
    SUBCASE("Gather action - player receives one coin and turn advances") {
        resetGame(); // Additional reset for each subcase
        auto [alice, bob] = createSimpleGame();
        
        // Alice starts with 0 coins
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice);
        
        // Alice performs gather
        alice->gather();
        
        // Alice should have 1 coin and turn should advance to Bob
        CHECK_EQ(alice->getCoins(), 1);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), bob);
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Gather);
    }
    
    SUBCASE("Tax action - player receives two coins and turn advances") {
        resetGame(); // Additional reset for each subcase
        auto [alice, bob] = createSimpleGame();
        
        // Alice starts with 0 coins
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice);
        
        // Alice performs tax
        alice->tax();
        
        // Alice should have 3 coins (Governor gets 3 instead of 2) and turn should advance to Bob
        CHECK_EQ(alice->getCoins(), 3);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), bob);
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Tax);
    }
}

TEST_CASE("Player Coup Action - Happy Path") {
    resetGame();
    
    SUBCASE("Coup with exactly 7 coins removes target player") {
        resetGame(); // Additional reset for each subcase
        auto players = createThreePlayerGame(); // Use 3 players so game doesn't end
        Player* alice = players[0];
        Player* bob = players[1];
        // Player* charlie = players[2]; // Not used in this test
        
        // Give Alice exactly 7 coins for coup
        alice->setCoins(7);
        CHECK_EQ(alice->getCoins(), 7);
        CHECK(bob->isActive());
        
        // Alice performs coup on Bob
        alice->coup(*bob);
        
        // Alice should lose 7 coins, Bob should be inactive
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_FALSE(bob->isActive());
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Coup);
        // Game should continue with 2 active players (Alice and Charlie)
        CHECK_EQ(Game::getInstance().getNumPlayers(), 2);
    }
    
    SUBCASE("Coup with more than 7 coins works correctly") {
        auto players = createThreePlayerGame(); // Use 3 players so game doesn't end
        Player* alice = players[0];
        Player* bob = players[1];
        
        // Give Alice 10 coins
        alice->setCoins(10);
        
        // Alice performs coup on Bob
        alice->coup(*bob);
        
        // Alice should have 3 coins left, Bob should be inactive
        CHECK_EQ(alice->getCoins(), 3);
        CHECK_FALSE(bob->isActive());
    }
}

TEST_CASE("Player Sanction Action - Happy Path") {
    resetGame();
    
    SUBCASE("Sanction with exactly 3 coins works correctly") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice exactly 3 coins for sanction
        alice->setCoins(3);
        CHECK_FALSE(bob->isSanctioned());
        
        // Alice performs sanction on Bob
        alice->sanction(*bob);
        
        // Alice should lose 3 coins, Bob should be sanctioned
        CHECK_EQ(alice->getCoins(), 0);
        CHECK(bob->isSanctioned());
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Sanction);
    }
}

TEST_CASE("Player Arrest Action - Happy Path") {
    resetGame();
    
    SUBCASE("Basic arrest - attacker gains 1 coin, target loses 1 coin") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Bob 1 coin to be arrested
        bob->setCoins(1);
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_EQ(bob->getCoins(), 1);
        
        // Alice arrests Bob
        alice->arrest(*bob);
        
        // Alice should gain 1 coin, Bob should lose 1 coin
        CHECK_EQ(alice->getCoins(), 1);
        CHECK_EQ(bob->getCoins(), 0);
        CHECK(bob->isLastArrested());
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Arrest);
    }
}

TEST_CASE("Player Bribe Action - Happy Path") {
    resetGame();
    
    SUBCASE("Bribe with exactly 4 coins - turn does not advance") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice exactly 4 coins for bribe
        alice->setCoins(4);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice);
        
        // Alice performs bribe
        alice->bribe();
        
        // Alice should lose 4 coins, turn should NOT advance (still Alice's turn)
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice);
        CHECK(alice->getIsBribed());
        CHECK_EQ(Game::getInstance().getLastStep(), ActionType::Bribe);
    }
    
    SUBCASE("Bribe followed by additional action") {
        resetGame(); // Explicit reset for this specific subcase
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* bob = game.addPlayer("Bob", "Spy");
        game.startGame();
        
        // Verify initial state
        CHECK_EQ(game.getCurrentPlayer(), alice);
        
        // Give Alice 6 coins (4 for bribe + 2 for two more actions)
        alice->setCoins(6);
        
        // Alice performs bribe
        alice->bribe();
        CHECK_EQ(alice->getCoins(), 2);
        CHECK_EQ(game.getCurrentPlayer(), alice);
        CHECK(alice->getIsBribed());
        
        // Alice can perform first action after bribe (gather)
        alice->gather();
        CHECK_EQ(alice->getCoins(), 3);
        CHECK_EQ(game.getCurrentPlayer(), alice); // Turn should still be Alice's
        
        // Alice can perform second action after bribe (gather again)
        alice->gather();
        CHECK_EQ(alice->getCoins(), 4);
        
        // Now turn should advance to Bob
        CHECK_EQ(game.getCurrentPlayer(), bob);
    }
}

// ============================================================
// ERROR HANDLING TESTS - WRONG TURN
// ============================================================

TEST_CASE("Player Actions - Wrong Turn Error Handling") {
    resetGame();
    
    SUBCASE("Gather when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        
        // It's Alice's turn, Bob tries to gather
        CHECK_THROWS_AS(bob->gather(), std::runtime_error);
    }
    
    SUBCASE("Tax when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        
        // It's Alice's turn, Bob tries to tax
        CHECK_THROWS_AS(bob->tax(), std::runtime_error);
    }
    
    SUBCASE("Coup when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        bob->setCoins(7); // Give Bob enough coins
        
        // It's Alice's turn, Bob tries to coup
        CHECK_THROWS_AS(bob->coup(*alice), std::runtime_error);
    }
    
    SUBCASE("Arrest when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(1); // Give Alice coins to be arrested
        
        // It's Alice's turn, Bob tries to arrest
        CHECK_THROWS_AS(bob->arrest(*alice), std::runtime_error);
    }
    
    SUBCASE("Sanction when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        bob->setCoins(3); // Give Bob enough coins
        
        // It's Alice's turn, Bob tries to sanction
        CHECK_THROWS_AS(bob->sanction(*alice), std::runtime_error);
    }
    
    SUBCASE("Bribe when not player's turn throws exception") {
        auto [alice, bob] = createSimpleGame();
        bob->setCoins(4); // Give Bob enough coins
        
        // It's Alice's turn, Bob tries to bribe
        CHECK_THROWS_AS(bob->bribe(), std::runtime_error);
    }
}

// ============================================================
// ERROR HANDLING TESTS - INACTIVE PLAYER
// ============================================================

TEST_CASE("Player Actions - Inactive Player Error Handling") {
    resetGame();
    
    SUBCASE("Inactive player cannot gather") {
        auto [alice, bob] = createSimpleGame();
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to gather while inactive
        CHECK_THROWS_AS(alice->gather(), std::runtime_error);
    }
    
    SUBCASE("Inactive player cannot tax") {
        auto [alice, bob] = createSimpleGame();
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to tax while inactive
        CHECK_THROWS_AS(alice->tax(), std::runtime_error);
    }
    
    SUBCASE("Inactive player cannot coup") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(7);
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to coup while inactive
        CHECK_THROWS_AS(alice->coup(*bob), std::runtime_error);
    }
    
    SUBCASE("Inactive player cannot arrest") {
        auto [alice, bob] = createSimpleGame();
        bob->setCoins(1);
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to arrest while inactive
        CHECK_THROWS_AS(alice->arrest(*bob), std::runtime_error);
    }
    
    SUBCASE("Inactive player cannot sanction") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(3);
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to sanction while inactive
        CHECK_THROWS_AS(alice->sanction(*bob), std::runtime_error);
    }
    
    SUBCASE("Inactive player cannot bribe") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(4);
        
        // Make Alice inactive
        alice->setActive(false);
        
        // Alice tries to bribe while inactive
        CHECK_THROWS_AS(alice->bribe(), std::runtime_error);
    }
}

// ============================================================
// ERROR HANDLING TESTS - SANCTIONED PLAYER
// ============================================================

TEST_CASE("Player Actions - Sanctioned Player Error Handling") {
    resetGame();
    
    SUBCASE("Sanctioned player cannot gather") {
        auto [alice, bob] = createSimpleGame();
        
        // Sanction Alice
        alice->setSanctioned(true);
        
        // Alice tries to gather while sanctioned
        CHECK_THROWS_AS(alice->gather(), std::runtime_error);
    }
    
    SUBCASE("Sanctioned player cannot tax") {
        auto [alice, bob] = createSimpleGame();
        
        // Sanction Alice
        alice->setSanctioned(true);
        
        // Alice tries to tax while sanctioned
        CHECK_THROWS_AS(alice->tax(), std::runtime_error);
    }
    
    SUBCASE("Sanctioned player can still coup") {
        auto players = createThreePlayerGame(); // Use 3 players so game doesn't end
        Player* alice = players[0];
        Player* bob = players[1];
        alice->setCoins(7);
        
        // Sanction Alice
        alice->setSanctioned(true);
        
        // Alice can still coup despite being sanctioned
        CHECK_NOTHROW(alice->coup(*bob));
        CHECK_FALSE(bob->isActive());
    }
    
    SUBCASE("Sanctioned player can still arrest") {
        auto [alice, bob] = createSimpleGame();
        bob->setCoins(1);
        
        // Sanction Alice
        alice->setSanctioned(true);
        
        // Alice can still arrest despite being sanctioned
        CHECK_NOTHROW(alice->arrest(*bob));
        CHECK_EQ(bob->getCoins(), 0);
    }
}

// ============================================================
// ERROR HANDLING TESTS - INSUFFICIENT FUNDS
// ============================================================

TEST_CASE("Player Actions - Insufficient Funds Error Handling") {
    resetGame();
    
    SUBCASE("Coup with less than 7 coins throws exception") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice only 6 coins (1 less than required)
        alice->setCoins(6);
        
        // Alice tries to coup without enough coins
        CHECK_THROWS_AS(alice->coup(*bob), std::runtime_error);
    }
    
    SUBCASE("Sanction with less than 3 coins throws exception") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice only 2 coins (1 less than required)
        alice->setCoins(2);
        
        // Alice tries to sanction without enough coins
        CHECK_THROWS_AS(alice->sanction(*bob), std::runtime_error);
    }
    
    SUBCASE("Bribe with less than 4 coins throws exception") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice only 3 coins (1 less than required)
        alice->setCoins(3);
        
        // Alice tries to bribe without enough coins
        CHECK_THROWS_AS(alice->bribe(), std::runtime_error);
    }
}

// ============================================================
// ERROR HANDLING TESTS - SELF-TARGETING
// ============================================================

TEST_CASE("Player Actions - Self-Targeting Error Handling") {
    resetGame();
    
    SUBCASE("Player cannot coup themselves") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(7);
        
        // Alice tries to coup herself
        CHECK_THROWS_AS(alice->coup(*alice), std::runtime_error);
    }
    
    SUBCASE("Player cannot arrest themselves") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(1);
        
        // Alice tries to arrest herself
        CHECK_THROWS_AS(alice->arrest(*alice), std::runtime_error);
    }
    
    SUBCASE("Player cannot sanction themselves") {
        auto [alice, bob] = createSimpleGame();
        alice->setCoins(3);
        
        // Alice tries to sanction herself
        CHECK_THROWS_AS(alice->sanction(*alice), std::runtime_error);
    }
}

// ============================================================
// ERROR HANDLING TESTS - ARREST SPECIFIC
// ============================================================

TEST_CASE("Player Arrest - Specific Error Handling") {
    resetGame();
    
    SUBCASE("Cannot arrest player with 0 coins") {
        auto [alice, bob] = createSimpleGame();
        
        // Bob has 0 coins (default)
        CHECK_EQ(bob->getCoins(), 0);
        
        // Alice tries to arrest Bob who has no coins
        CHECK_THROWS_AS(alice->arrest(*bob), std::runtime_error);
    }
    
    SUBCASE("Cannot arrest same player twice in consecutive turns") {
        auto players = createThreePlayerGame();
        Player* alice = players[0];
        Player* bob = players[1];
        Player* charlie = players[2];
        
        // Give Bob some coins
        bob->setCoins(2);
        
        // Alice arrests Bob
        alice->arrest(*bob);
        CHECK(bob->isLastArrested());
        CHECK_EQ(bob->getCoins(), 1);
        
        // Now it's Bob's turn, then Charlie's turn
        // Charlie tries to arrest Bob again (should fail due to lastArrested flag)
        bob->gather(); // Bob's turn (to advance to Charlie)
        CHECK_THROWS_AS(charlie->arrest(*bob), std::runtime_error);
    }
}

// ============================================================
// EDGE CASES - 10 COINS RULE
// ============================================================

TEST_CASE("Player 10+ Coins Rule - Must Coup") {
    resetGame();
    
    SUBCASE("Player with 10 coins cannot gather - must coup") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice 10 coins
        alice->setCoins(10);
        
        // Alice tries to gather but has 10+ coins
        CHECK_THROWS_AS(alice->gather(), std::runtime_error);
    }
    
    SUBCASE("Player with 10 coins cannot tax - must coup") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice 10 coins
        alice->setCoins(10);
        
        // Alice tries to tax but has 10+ coins
        CHECK_THROWS_AS(alice->tax(), std::runtime_error);
    }
    
    SUBCASE("Player with 10 coins cannot bribe - must coup") {
        auto [alice, bob] = createSimpleGame();
        
        // Give Alice 10 coins
        alice->setCoins(10);
        
        // Alice tries to bribe but has 10+ coins
        CHECK_THROWS_AS(alice->bribe(), std::runtime_error);
    }
    
    SUBCASE("Player with 10 coins can coup") {
        auto players = createThreePlayerGame(); // Use 3 players so game doesn't end
        Player* alice = players[0];
        Player* bob = players[1];
        
        // Give Alice 10 coins
        alice->setCoins(10);
        
        // Alice can coup with 10+ coins
        CHECK_NOTHROW(alice->coup(*bob));
        CHECK_EQ(alice->getCoins(), 3);
        CHECK_FALSE(bob->isActive());
    }
}

// ============================================================
// ROLE-SPECIFIC ARREST INTERACTIONS
// ============================================================

TEST_CASE("Arrest Interactions with Specific Roles") {
    resetGame();
    
    SUBCASE("Arresting Merchant - target loses 2 coins, attacker gains nothing") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* merchant = game.addPlayer("Bob", "Merchant");
        game.startGame();
        
        // Give merchant 3 coins
        merchant->setCoins(3);
        CHECK_EQ(alice->getCoins(), 0);
        
        // Alice arrests merchant
        alice->arrest(*merchant);
        
        // Merchant loses 2 coins to treasury, Alice gains nothing
        CHECK_EQ(merchant->getCoins(), 1);
        CHECK_EQ(alice->getCoins(), 0);
        CHECK(merchant->isLastArrested());
    }
    
    SUBCASE("Arresting Merchant with less than 2 coins throws exception") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* merchant = game.addPlayer("Bob", "Merchant");
        game.startGame();
        
        // Give merchant only 1 coin
        merchant->setCoins(1);
        
        // Alice tries to arrest merchant who doesn't have enough coins
        CHECK_THROWS_AS(alice->arrest(*merchant), std::runtime_error);
    }
    
    SUBCASE("Arresting General - attacker gains 1 coin, target loses nothing") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* general = game.addPlayer("Bob", "General");
        game.startGame();
        
        // Give general some coins
        general->setCoins(5);
        CHECK_EQ(alice->getCoins(), 0);
        
        // Alice arrests general
        alice->arrest(*general);
        
        // General loses nothing, Alice gains 1 coin
        CHECK_EQ(general->getCoins(), 5);
        CHECK_EQ(alice->getCoins(), 1);
        CHECK(general->isLastArrested());
    }
}

// ============================================================
// ROLE-SPECIFIC SANCTION INTERACTIONS
// ============================================================

TEST_CASE("Sanction Interactions with Specific Roles") {
    resetGame();
    
    SUBCASE("Sanctioning Baron - target receives 1 coin compensation") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* baron = game.addPlayer("Bob", "Baron");
        game.startGame();
        
        // Give Alice 3 coins for sanction
        alice->setCoins(3);
        CHECK_EQ(baron->getCoins(), 0);
        
        // Alice sanctions baron
        alice->sanction(*baron);
        
        // Alice loses 3 coins, Baron gets 1 coin compensation
        CHECK_EQ(alice->getCoins(), 0);
        CHECK_EQ(baron->getCoins(), 1);
        CHECK(baron->isSanctioned());
    }
    
    SUBCASE("Sanctioning Judge - attacker must pay 4 coins instead of 3") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* judge = game.addPlayer("Bob", "Judge");
        game.startGame();
        
        // Give Alice exactly 4 coins (required for judge sanction)
        alice->setCoins(4);
        
        // Alice sanctions judge
        alice->sanction(*judge);
        
        // Alice loses 4 coins instead of 3
        CHECK_EQ(alice->getCoins(), 0);
        CHECK(judge->isSanctioned());
    }
    
    SUBCASE("Sanctioning Judge with only 3 coins throws exception") {
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* judge = game.addPlayer("Bob", "Judge");
        game.startGame();
        
        // Give Alice only 3 coins (not enough for judge sanction)
        alice->setCoins(3);
        
        // Alice tries to sanction judge without enough coins
        CHECK_THROWS_AS(alice->sanction(*judge), std::runtime_error);
    }
}

// ============================================================
// STATE MANAGEMENT TESTS
// ============================================================

TEST_CASE("State Management and Flag Resets") {
    resetGame();
    
    SUBCASE("Sanction and arrest flags reset properly after turn") {
        auto players = createThreePlayerGame();
        Player* alice = players[0];
        Player* bob = players[1];
        Player* charlie = players[2];
        
        // Give Alice coins to sanction Bob
        alice->setCoins(3);
        CHECK_FALSE(bob->isSanctioned());
        
        // Alice sanctions Bob
        alice->sanction(*bob);
        CHECK(bob->isSanctioned());
        
        // Bob's turn - he cannot gather due to sanction
        CHECK_THROWS_AS(bob->gather(), std::runtime_error);
        
        // Bob can still coup (not economic action)
        bob->setCoins(7);
        bob->coup(*charlie); // Bob coups Charlie to end his turn
        
        // Now it's Alice's turn again, Bob's sanction should be reset when he tries an action next time
        alice->gather(); // Alice's turn
        bob->gather(); // Bob should be able to gather now (sanction reset)
        CHECK_FALSE(bob->isSanctioned());
    }
    
    SUBCASE("Bribe status affects turn progression correctly") {
        resetGame(); // Explicit reset for this specific subcase
        Game& game = Game::getInstance();
        Player* alice = game.addPlayer("Alice", "Governor");
        Player* bob = game.addPlayer("Bob", "Spy");
        game.startGame();
        
        // Give Alice enough coins for bribe + additional actions
        alice->setCoins(6);
        
        // Alice bribes
        alice->bribe();
        CHECK(alice->getIsBribed());
        CHECK_EQ(game.getCurrentPlayer(), alice); // Still Alice's turn
        
        // Alice performs first action (gather) after bribe
        alice->gather();
        CHECK_EQ(game.getCurrentPlayer(), alice); // Still Alice's turn
        
        // Alice performs second action (gather) after bribe  
        alice->gather();
        CHECK_EQ(game.getCurrentPlayer(), bob); // Now Bob's turn
    }
}

TEST_CASE("Available Actions List") {
    resetGame();
    
    SUBCASE("Basic player has all standard actions available") {
        auto [alice, bob] = createSimpleGame();
        
        std::vector<ActionType> actions = alice->getAvailableActions();
        
        // Check that all basic actions are available
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Gather), actions.end());
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Tax), actions.end());
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Bribe), actions.end());
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Arrest), actions.end());
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Coup), actions.end());
        CHECK_NE(std::find(actions.begin(), actions.end(), ActionType::Sanction), actions.end());
    }
}

// ============================================================
// COMPLEX SCENARIO TESTS
// ============================================================

TEST_CASE("Complex Game Scenarios") {
    resetGame();
    
    SUBCASE("Multiple actions after bribe") {
        auto players = createThreePlayerGame();
        Player* alice = players[0];
        Player* bob = players[1];
        // Player* charlie = players[2]; // Not used in this test
        
        // Give Alice enough coins for bribe + 2 more actions
        alice->setCoins(6);
        
        // Alice bribes
        alice->bribe();
        CHECK_EQ(alice->getCoins(), 2);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice);
        
        // Alice gathers (first additional action)
        alice->gather();
        CHECK_EQ(alice->getCoins(), 3);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), alice); // Still Alice's turn
        
        // Alice gathers again (second additional action)
        alice->gather();
        CHECK_EQ(alice->getCoins(), 4);
        CHECK_EQ(Game::getInstance().getCurrentPlayer(), bob); // Now turn advances
    }
    
    SUBCASE("Arrest then sanction same target") {
        auto players = createThreePlayerGame();
        Player* alice = players[0];
        Player* bob = players[1];
        Player* charlie = players[2];
        
        // Give Bob coins and Alice moderate coins for actions (not 10+)
        bob->setCoins(5);
        alice->setCoins(7); // Not 10+ so she can arrest
        
        // Alice arrests Bob
        alice->arrest(*bob);
        CHECK_EQ(bob->getCoins(), 4);
        CHECK(bob->isLastArrested());
        
        // Bob's turn - he gathers
        bob->gather();
        CHECK_EQ(bob->getCoins(), 5);
        
        // Charlie's turn - Charlie sanctions Bob
        charlie->setCoins(3);
        charlie->sanction(*bob);
        CHECK(bob->isSanctioned());
    }
}

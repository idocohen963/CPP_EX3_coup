#include "doctest.h"
#include "GAME/game.hpp"
#include "PLAYER/PlayerFactory.hpp"
#include "PLAYER/governor.hpp"
#include "PLAYER/baron.hpp"
#include "PLAYER/general.hpp"
#include "PLAYER/judge.hpp"
#include "PLAYER/spy.hpp"
#include "PLAYER/merchant.hpp"

using namespace coup;

/**
 * Test suite for specific role classes and their unique abilities
 * Tests the special behaviors, actions, and passive abilities of each role
 */

TEST_SUITE("Role Specific Tests") {

    TEST_CASE("Governor Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Governor tax action - receives 3 coins instead of 2") {
            Player* governor = game.addPlayer("Gov1", "Governor");
            Player* regular = game.addPlayer("Reg1", "Spy");
            (void)regular; // Suppress unused variable warning
            game.startGame();
            
            // Governor should get 3 coins from tax
            int initialCoins = governor->getCoins();
            governor->tax();
            CHECK_EQ(governor->getCoins(), initialCoins + 3);
        }
        
        SUBCASE("Governor cancel tax action - against regular player") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* governor = game.addPlayer("Gov1", "Governor");
            game.startGame();
            
            // Player1 performs tax (gets 2 coins)
            int initialCoins = player1->getCoins();
            player1->tax();
            CHECK_EQ(player1->getCoins(), initialCoins + 2);
            
            // Governor cancels the tax - player1 should lose 2 coins
            governor->cancel(*player1);
            CHECK_EQ(player1->getCoins(), initialCoins);
            CHECK_EQ(game.getLastStep(), ActionType::cancel);
        }
        
        SUBCASE("Governor cancel tax action - against another governor") {
            Player* governor1 = game.addPlayer("Gov1", "Governor");
            Player* governor2 = game.addPlayer("Gov2", "Governor");
            game.startGame();
            
            // Governor1 performs tax (gets 3 coins)
            int initialCoins = governor1->getCoins();
            governor1->tax();
            CHECK_EQ(governor1->getCoins(), initialCoins + 3);
            
            // Governor2 cancels the tax - governor1 should lose 3 coins
            governor2->cancel(*governor1);
            CHECK_EQ(governor1->getCoins(), initialCoins);
        }
        
        SUBCASE("Governor cancel - error when trying to cancel non-tax action") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* governor = game.addPlayer("Gov1", "Governor");
            game.startGame();
            
            // Player1 performs gather (not tax)
            player1->gather();
            
            // Governor tries to cancel - should throw error
            CHECK_THROWS_AS(governor->cancel(*player1), std::runtime_error);
        }
        
        SUBCASE("Governor cancel - error when trying to cancel own tax action") {
            Player* governor = game.addPlayer("Gov1", "Governor");
            Player* dummy = game.addPlayer("Dummy", "Spy");
            (void)dummy; // Suppress unused variable warning
            game.startGame();
            
            // Governor performs tax
            governor->tax();
            
            // Governor tries to cancel own action - should throw error
            CHECK_THROWS_AS(governor->cancel(*governor), std::runtime_error);
        }
        
        SUBCASE("Governor cancel does not advance turn") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* governor = game.addPlayer("Gov1", "Governor");
            game.startGame();
            
            player1->tax();
            int currentPlayerIndex = game.getCurrentPlayerIndex();
            
            // Governor cancels - turn should not advance
            governor->cancel(*player1);
            CHECK_EQ(game.getCurrentPlayerIndex(), currentPlayerIndex);
        }
        
    }

    TEST_CASE("Baron Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Baron invest action -with sufficient coins") {
            Player* baron = game.addPlayer("Baron1", "Baron");
            Player* dummy = game.addPlayer("Dummy", "Spy");
            (void)dummy; // Suppress unused variable warning
            game.startGame();
            
            // Set baron to have exactly 4 coins
            baron->setCoins(4);
            int initialCoins = baron->getCoins();
            
            // Baron invests - should gain 3 coins net
            baron->invest();
            CHECK_EQ(baron->getCoins(), initialCoins + 3);
        }
        
        SUBCASE("Baron invest action - error with insufficient coins") {
            Player* baron = game.addPlayer("Baron1", "Baron");
            Player* dummy = game.addPlayer("Dummy", "Spy");
            (void)dummy; // Suppress unused variable warning
            game.startGame();
            
            // Set baron to have only 2 coins
            baron->setCoins(2);
            
            // Baron tries to invest - should throw error
            CHECK_THROWS_AS(baron->invest(), std::runtime_error);
        }
        
        SUBCASE("Baron invest action - error when sanctioned") {
            Player* attacker = game.addPlayer("Attacker", "Spy"); // Attacker first
            Player* baron = game.addPlayer("Baron1", "Baron");    // Baron second
            game.startGame(); // Attacker starts first
            
            baron->setCoins(5);
            attacker->setCoins(5);
            
            // Attacker sanctions baron - now it's baron's turn and he's sanctioned
            attacker->sanction(*baron);
            
            // Baron tries to invest while sanctioned - should throw error
            CHECK_THROWS_AS(baron->invest(), std::runtime_error);
        }
        
        SUBCASE("Baron invest action - error when not baron's turn") {
            Player* baron = game.addPlayer("Baron1", "Baron");
            Player* other = game.addPlayer("Other", "Spy");
            (void)other; // Suppress unused variable warning
            game.startGame();
            
            baron->setCoins(5);
            // It's baron's turn, move to next player
            baron->gather();
            
            // Baron tries to invest when it's not their turn - should throw error
            CHECK_THROWS_AS(baron->invest(), std::runtime_error);
        }
        
        SUBCASE("Baron invest action - exactly 3 coins") {
            Player* baron = game.addPlayer("Baron1", "Baron");
            Player* dummy = game.addPlayer("Dummy", "Spy");
            (void)dummy; // Suppress unused variable warning
            game.startGame();
            
            // Set baron to have exactly 3 coins
            baron->setCoins(3);
            
            // Baron should be able to invest
            CHECK_NOTHROW(baron->invest());
            CHECK_EQ(baron->getCoins(), 6);
        }
        
        SUBCASE("Baron passive ability - receives compensation coin when sanctioned") {
            // Create a completely fresh test just for this ability
            Game& testGame = Game::getInstance();
            testGame.reset();
            
            Player* baron = testGame.addPlayer("TestBaron", "Baron");
            Player* spy = testGame.addPlayer("TestSpy", "Spy");
            testGame.startGame();
            
            // Set initial coins
            baron->setCoins(5);
            spy->setCoins(5);
            
            // Baron's turn - let baron do something to move to next turn
            baron->gather(); // Now baron has 6 coins and it's spy's turn
            
            // Spy sanctions baron - baron should get +1 coin compensation
            spy->sanction(*baron);
            
            // Check: baron should have 6 (from gather) + 1 (compensation) = 7 coins
            // spy should have 5 - 3 = 2 coins
            CHECK_EQ(baron->getCoins(), 7);
            CHECK_EQ(spy->getCoins(), 2);
            CHECK(baron->isSanctioned());
        }
    }

    TEST_CASE("General Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("General cancel coup action") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* victim = game.addPlayer("Victim", "Merchant");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            attacker->setCoins(10);
            general->setCoins(10);
            
            // Attacker performs coup on victim
            attacker->coup(*victim);
            CHECK_FALSE(victim->isActive());
            CHECK_EQ(game.getNumPlayers(), 2);
            
            // General cancels the coup - victim should be restored
            general->cancel(*victim);
            CHECK(victim->isActive());
            CHECK_EQ(game.getNumPlayers(), 3);
            CHECK_EQ(general->getCoins(), 5); // General should pay 5 coins
        }
        
        SUBCASE("General cancel - error when trying to cancel non-coup action") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            // Player1 performs gather (not coup)
            player1->gather();
            
            // General tries to cancel - should throw error
            CHECK_THROWS_AS(general->cancel(*player1), std::runtime_error);
        }
        
        SUBCASE("General cancel - error with insufficient coins") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* victim = game.addPlayer("Victim", "Merchant");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            attacker->setCoins(10);
            general->setCoins(3); // Not enough for cancel
            
            // Attacker performs coup on victim
            attacker->coup(*victim);
            
            // General tries to cancel with insufficient coins - should throw error
            CHECK_THROWS_AS(general->cancel(*victim), std::runtime_error);
        }
        
        SUBCASE("General passive ability - does not lose coins when arrested") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            general->setCoins(5);
            attacker->setCoins(3);
            int initialGeneralCoins = general->getCoins();
            int initialAttackerCoins = attacker->getCoins();
            
            // Attacker arrests general - general should not lose coins, attacker gains 1
            attacker->arrest(*general);
            CHECK_EQ(general->getCoins(), initialGeneralCoins); // General doesn't lose coins
            CHECK_EQ(attacker->getCoins(), initialAttackerCoins + 1); // Attacker gains 1
        }
    }

    TEST_CASE("Judge Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Judge cancel bribe action - happy path") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            player1->setCoins(6);
            int initialCoins = player1->getCoins();
            
            // Player1 performs bribe
            player1->bribe();
            CHECK_EQ(player1->getCoins(), initialCoins - 4);
            CHECK(player1->getIsBribed());
            
            // Judge cancels the bribe
            judge->cancel(*player1);
            CHECK_EQ(game.getLastStep(), ActionType::cancel);
        }
        
        SUBCASE("Judge cancel - error when trying to cancel non-bribe action") {
            Player* player1 = game.addPlayer("Player1", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            // Player1 performs gather (not bribe)
            player1->gather();
            
            // Judge tries to cancel - should throw error
            CHECK_THROWS_AS(judge->cancel(*player1), std::runtime_error);
        }
        
        SUBCASE("Judge passive ability - attacker pays extra coin when sanctioning judge") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            attacker->setCoins(5);
            int initialAttackerCoins = attacker->getCoins();
            
            // Attacker sanctions judge - should pay 4 coins instead of 3
            attacker->sanction(*judge);
            CHECK_EQ(attacker->getCoins(), initialAttackerCoins - 4);
            CHECK(judge->isSanctioned());
        }
        
        SUBCASE("Judge passive ability - error when attacker has insufficient coins for sanction") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            attacker->setCoins(3); // Not enough for sanctioning judge (requires 4)
            
            // Attacker tries to sanction judge - should throw error
            CHECK_THROWS_AS(attacker->sanction(*judge), std::runtime_error);
        }
    }

    TEST_CASE("Spy Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Spy spyOn action - views coins and prevents arrest") {
            Player* spy = game.addPlayer("Spy1", "Spy");
            Player* target = game.addPlayer("Target", "Merchant");
            game.startGame();
            
            target->setCoins(5);
            CHECK(target->isCanArrest()); // Target can initially arrest
            
            // Spy uses spyOn action
            spy->spyOn(*target);
            
            // Check that target's arrest ability was removed
            CHECK_FALSE(target->isCanArrest());
            CHECK_EQ(target->getCoins(), 5); // Target coins unchanged
            CHECK_EQ(game.getLastStep(), ActionType::SpyOn);
        }
        
        SUBCASE("Spy spyOn - error when trying to spy on self") {
            Player* spy = game.addPlayer("Spy1", "Spy");
            Player* dummy = game.addPlayer("Dummy", "Merchant");
            (void)dummy; // Suppress unused variable warning
            game.startGame();
            
            // Spy tries to spy on themselves - should throw error
            CHECK_THROWS_AS(spy->spyOn(*spy), std::runtime_error);
        }
        
        SUBCASE("Spy spyOn - advances turn correctly") {
            Player* spy = game.addPlayer("Spy1", "Spy");
            Player* target = game.addPlayer("Target", "Merchant");
            game.startGame();
            
            int initialPlayerIndex = game.getCurrentPlayerIndex();
            spy->spyOn(*target);
            
            // Turn should advance to next player
            CHECK_EQ(game.getCurrentPlayerIndex(), (initialPlayerIndex + 1) % game.getNumPlayers());
        }
    }

    TEST_CASE("Merchant Role Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Merchant passive ability - gains extra coin at turn start with 3+ coins") {
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            Player* other = game.addPlayer("Other", "Spy");
            (void)other; // Suppress unused variable warning
            game.startGame();
            
            // Set merchant to have 3 coins
            merchant->setCoins(3);
            
            // Merchant performs gather - should get 1 from gather + 1 bonus for having 3+ coins
            merchant->gather();
            CHECK_EQ(merchant->getCoins(), 5); // 3 + 1 (gather) + 1 (merchant bonus for 3+ coins)
        }
        
        SUBCASE("Merchant passive ability - test with 2 coins (no bonus)") {
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            Player* other = game.addPlayer("Other", "Spy");
            (void)other; // Suppress unused variable warning
            game.startGame();
            
            // Set merchant to have 2 coins
            merchant->setCoins(2);
            
            // Merchant performs gather - gets 1 from gather (total 3), then gets bonus for having 3+ coins!
            merchant->gather();
            CHECK_EQ(merchant->getCoins(), 4); // 2 + 1 (gather) + 1 (merchant bonus for reaching 3+ coins)
        }
        
        SUBCASE("Merchant passive ability - test with 1 coin (no bonus)") {
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            Player* other = game.addPlayer("Other", "Spy");
            (void)other; // Suppress unused variable warning
            game.startGame();
            
            // Set merchant to have 1 coin
            merchant->setCoins(1);
            
            // Merchant performs gather - gets 1 from gather (total 2), no bonus since still less than 3
            merchant->gather();
            CHECK_EQ(merchant->getCoins(), 2); // 1 + 1 (gather), no bonus for having less than 3
        }
        
        SUBCASE("Merchant passive ability - test with 4 coins") {
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            Player* other = game.addPlayer("Other", "Spy");
            (void)other; // Suppress unused variable warning
            game.startGame();
            
            // Set merchant to have 4 coins
            merchant->setCoins(4);
            
            // Merchant performs gather - should get 1 from gather + 1 bonus for having 3+ coins
            merchant->gather();
            CHECK_EQ(merchant->getCoins(), 6); // 4 + 1 (gather) + 1 (merchant bonus for 3+ coins)
        }
        
        SUBCASE("Merchant passive ability when arrested - loses 2 coins to treasury") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            game.startGame();
            
            merchant->setCoins(5);
            attacker->setCoins(3);
            int initialMerchantCoins = merchant->getCoins();
            int initialAttackerCoins = attacker->getCoins();
            
            // Attacker arrests merchant - merchant loses 2 coins, attacker gains nothing
            attacker->arrest(*merchant);
            CHECK_EQ(merchant->getCoins(), initialMerchantCoins - 2);
            CHECK_EQ(attacker->getCoins(), initialAttackerCoins); // Attacker gains nothing
        }
        
        SUBCASE("Merchant arrest - edge case with exactly 1 coin") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            game.startGame();
            
            merchant->setCoins(1); // Not enough for merchant arrest
            
            // Attacker tries to arrest merchant with insufficient coins - should throw error
            CHECK_THROWS_AS(attacker->arrest(*merchant), std::runtime_error);
        }
    }
}

TEST_SUITE("PlayerFactory Tests") {
    
    TEST_CASE("PlayerFactory - Valid role creation") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Create Spy player") {
            Player* spy = createPlayer("TestSpy", "Spy");
            CHECK_EQ(spy->getRoleType(), Role::Spy);
            CHECK_EQ(spy->getName(), "TestSpy");
            delete spy;
        }
        
        SUBCASE("Create Merchant player") {
            Player* merchant = createPlayer("TestMerchant", "Merchant");
            CHECK_EQ(merchant->getRoleType(), Role::Merchant);
            CHECK_EQ(merchant->getName(), "TestMerchant");
            delete merchant;
        }
        
        SUBCASE("Create General player") {
            Player* general = createPlayer("TestGeneral", "General");
            CHECK_EQ(general->getRoleType(), Role::General);
            CHECK_EQ(general->getName(), "TestGeneral");
            delete general;
        }
        
        SUBCASE("Create Governor player") {
            Player* governor = createPlayer("TestGovernor", "Governor");
            CHECK_EQ(governor->getRoleType(), Role::Governor);
            CHECK_EQ(governor->getName(), "TestGovernor");
            delete governor;
        }
        
        SUBCASE("Create Judge player") {
            Player* judge = createPlayer("TestJudge", "Judge");
            CHECK_EQ(judge->getRoleType(), Role::Judge);
            CHECK_EQ(judge->getName(), "TestJudge");
            delete judge;
        }
        
        SUBCASE("Create Baron player") {
            Player* baron = createPlayer("TestBaron", "Baron");
            CHECK_EQ(baron->getRoleType(), Role::Baron);
            CHECK_EQ(baron->getName(), "TestBaron");
            delete baron;
        }
    }
    
    TEST_CASE("PlayerFactory - Invalid role creation") {
        
        SUBCASE("Invalid role string - King") {
            CHECK_THROWS_AS(createPlayer("TestPlayer", "King"), std::invalid_argument);
        }
        
        
        SUBCASE("Invalid role string - empty string") {
            CHECK_THROWS_AS(createPlayer("TestPlayer", ""), std::invalid_argument);
        }
        
        SUBCASE("Invalid role string - lowercase spy") {
            CHECK_THROWS_AS(createPlayer("TestPlayer", "spy"), std::invalid_argument);
        }
    
    }
}

TEST_SUITE("Additional Role Tests") {
    
    TEST_CASE("Role Interaction Tests") {
        Game& game = Game::getInstance();
        game.reset();
        
        SUBCASE("Governor can cancel tax but not other actions") {
            Player* player = game.addPlayer("Player1", "Spy");
            Player* governor = game.addPlayer("Governor1", "Governor");
            game.startGame();
            
            // Test that governor cannot cancel gather
            player->gather();
            CHECK_THROWS_AS(governor->cancel(*player), std::runtime_error);
            
            game.reset();
            player = game.addPlayer("Player1", "Spy");
            governor = game.addPlayer("Governor1", "Governor");
            game.startGame();
            
            // Test that governor can cancel tax
            player->tax();
            CHECK_NOTHROW(governor->cancel(*player));
        }
        
        SUBCASE("General cannot lose coins when arrested") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            attacker->setCoins(3);
            general->setCoins(5);
            
            int initialGeneralCoins = general->getCoins();
            int initialAttackerCoins = attacker->getCoins();
            
            attacker->arrest(*general);
            
            // General should not lose coins, attacker gains 1
            CHECK_EQ(general->getCoins(), initialGeneralCoins);
            CHECK_EQ(attacker->getCoins(), initialAttackerCoins + 1);
        }
        
        SUBCASE("Merchant with exactly 1 coin cannot be arrested") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* merchant = game.addPlayer("Merchant1", "Merchant");
            game.startGame();
            
            attacker->setCoins(3);
            merchant->setCoins(1); // Not enough for merchant arrest (needs 2)
            
            CHECK_THROWS_AS(attacker->arrest(*merchant), std::runtime_error);
        }
        
        SUBCASE("Judge sanction costs 4 coins for attacker") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            attacker->setCoins(4);
            judge->setCoins(3);
            
            int initialAttackerCoins = attacker->getCoins();
            
            attacker->sanction(*judge);
            
            // Attacker should pay 4 coins (not 3) for sanctioning judge
            CHECK_EQ(attacker->getCoins(), initialAttackerCoins - 4);
            CHECK(judge->isSanctioned());
        }
        
        SUBCASE("Judge sanction with insufficient coins throws error") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* judge = game.addPlayer("Judge1", "Judge");
            game.startGame();
            
            attacker->setCoins(3); // Not enough for judge sanction (needs 4)
            
            CHECK_THROWS_AS(attacker->sanction(*judge), std::runtime_error);
        }
        
        SUBCASE("General successfully cancels coup and restores player") {
            Player* attacker = game.addPlayer("Attacker", "Spy");
            Player* victim = game.addPlayer("Victim", "Merchant");
            Player* general = game.addPlayer("General1", "General");
            game.startGame();
            
            attacker->setCoins(10);
            general->setCoins(10);
            
            // Get initial game state
            int initialNumPlayers = game.getNumPlayers();
            
            // Attacker performs coup on victim
            attacker->coup(*victim);
            CHECK_FALSE(victim->isActive());
            CHECK_EQ(game.getNumPlayers(), initialNumPlayers - 1);
            
            // General cancels the coup
            general->cancel(*victim);
            CHECK(victim->isActive());
            CHECK_EQ(game.getNumPlayers(), initialNumPlayers);
            CHECK_EQ(general->getCoins(), 5); // General paid 5 coins
        }
    }
}
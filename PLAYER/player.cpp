#include "player.hpp"
#include "GAME/game.hpp"

/**
 * @file player.cpp
 * @brief Implementation of the base Player class for the Coup game.
 * 
 * This file contains the implementations of the base player class methods.
 * Including basic actions like gather, tax, arrest, etc.
 */

namespace coup{
    /**
     * @brief Constructor for the Player class
     * 
     * Initializes all player fields to their initial state.
     * 
     * @param name The player's name
     */
    Player::Player(std::string name): name(name) {
        active = true;        // Player is active
        canArrest = true;     // Player can arrest
        playerCoins = 0;      // No coins at start
        sanctioned = false;   // Player is not under sanction
        lastArrested = false; // Player was not arrested in the last turn
        isBribed = false;     // Player did not perform bribe
    } 

    /**
     * @brief Default destructor for the Player class
     */
    Player::~Player() = default;

    /**
     * @brief Checks if the player is under sanction
     * 
     * @throws std::runtime_error if the player is under sanction and cannot perform actions
     */
    void Player::checkSanctioned() const {
        if (sanctioned) throw std::runtime_error("Player is sanctioned");
    }
    
    /**
     * @brief Checks if the player is active in the game
     * 
     * @throws std::runtime_error if the player is removed and cannot perform actions
     */
    void Player::checkActive() const{
        if(!active) throw std::runtime_error("Player is not active");
    }
    
    /**
     * @brief Checks if it's the player's turn to play
     * 
     * Checks with the game manager (Game) if the current player is the one whose turn it is to play.
     * 
     * @throws std::runtime_error if it's not the player's turn
     */
    void Player::checkTurn() const{
        if(Game::getInstance().getCurrentPlayer() != this) throw std::runtime_error("wrong player's turn");
    }

    /**
     * @brief Checks if the player must perform coup (10+ coins)
     * 
     * @throws std::runtime_error if the player must perform coup
     */
    void Player::checkMustCoup() const {
        if (this->playerCoins >= 10) {
            throw std::runtime_error("Player must perform coup action.");
        }
    }

    
/**
 * @brief Implementation of the gather action
 * 
 * Adds one coin to the player if they are active, not under sanction, and it's their turn.
 * Advances turn if the previous action was not a bribe.
 */
void Player::gather(){
    this->checkActive(); // Check that the player is active
    this->checkSanctioned(); // Check that the player is not under sanction
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    this->playerCoins+=1; // Player gets one coin
    if (Game::getInstance().getLastStep() != ActionType::Bribe) { // If the last action was not a bribe, advance to next turn
        Game::getInstance().nextTurn();
        this->canArrest= true; // Reset the ability to arrest for the next turn
    } 
    Game::getInstance().setLastStep(ActionType::Gather); // Update the last action to Gather
}

/**
 * @brief Implementation of the tax action
 * 
 * Adds two coins to the player if they are active, not under sanction, and it's their turn.
 * Advances turn if the previous action was not a bribe.
 */
void Player:: tax() {
    this->checkActive(); // Check that the player is active
    this->checkSanctioned(); // Check that the player is not under sanction
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    playerCoins += 2; // Player gets two coins
    if (Game::getInstance().getLastStep() != ActionType::Bribe) { // If the last action was not a bribe, advance to next turn
        Game::getInstance().nextTurn();
        this->canArrest = true; // Reset the ability to arrest for the next turn
        this->sanctioned = false; // Reset sanction status for the next turn
    }
    Game::getInstance().setLastStep(ActionType::Tax); // Update the last action to Tax
}

/**
 * @brief Implementation of the arrest action
 * 
 * Allows a player to arrest another player and take coins from them.
 * Behavior varies according to the role of the arrested player:
 * - Merchant: loses 2 coins to the treasury
 * - General: the arresting player gets an extra coin
 * - Other roles: lose one coin and the arresting player receives it
 * 
 * @param other The player to be arrested
 * @throws std::runtime_error if the player tries to arrest themselves, if the other player has no coins,
 *                           if the other player was already arrested in the last turn, or if the player cannot arrest
 */
void Player::arrest(Player& other) {
    
    this->checkActive(); // Check that the player is active
    other.checkActive(); // Check that the other player is active
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    if(this->getName()==other.getName()){ // Check that the player is not trying to arrest themselves
        throw std::runtime_error("Player cannot arrest themselves");
    }
    if (other.playerCoins==0){ // Check that the other player has coins
        throw std::runtime_error("Player has no coins to lose");
    }
    if(other.lastArrested){ // Check that the other player was not arrested in the last turn
        throw std::runtime_error("Player has already been arrested");
    }
    if(!this->canArrest){ // Check that the player can arrest
        throw std::runtime_error("Player cannot arrest");
    } 
    // Reset previous arrest status for all players
    for(auto& player : Game::getInstance().getPlayers()) {
        if(player->lastArrested){
            player->lastArrested= false; // Reset previous arrest status
        }
    }

    // Handle according to the arrested player's role
    if(other.role==Role::Merchant){ // If the other player is a merchant
        if(other.playerCoins<2){ // Check that the other player has enough coins to lose
            throw std::runtime_error("Player has no coins to lose");
        }
        other.playerCoins -= 2;     // The merchant loses 2 coins to the treasury (not to the arresting player)
        other.lastArrested = true;  // Mark that the player was arrested in this turn
        
    } else if (other.role == Role::General) {
        // General: the arresting player gets an extra coin (the general doesn't lose)
        this->playerCoins += 1;     // The arresting player gets a coin
        other.lastArrested = true;  // Mark that the player was arrested in this turn
        
    } else {
        // All other roles: the arresting player gets one coin, the arrested player loses one coin
        other.playerCoins -= 1;     // The other player loses a coin
        other.lastArrested = true;  // Mark that the player was arrested in this turn
        this->playerCoins += 1;     // The arresting player receives the coin
    }
    
    // Update game status and advance to next turn
    if (Game::getInstance().getLastStep() != ActionType::Bribe) {
        Game::getInstance().nextTurn();
        this->canArrest = true;    // Reset the ability to arrest for the next turn
        this->sanctioned = false;  // Reset sanction status for the next turn
    }
    
    Game::getInstance().setLastStep(ActionType::Arrest);  // Update the last action to Arrest
    return;
}

/**
 * @brief Implementation of the coup action
 * 
 * Allows a player to remove another player from the game permanently.
 * This action costs 7 coins and reduces the number of active players in the game.
 * The turn advances to the next player only if there are at least 2 players remaining
 * and the last action was not a bribe. If only 1 player remains, the game ends.
 * 
 * @param other The player to be removed
 * @throws std::runtime_error if the player tries to coup themselves or doesn't have enough coins (7)
 */
void Player:: coup(Player& other) {
    this->checkActive(); // Check that the player is active
    other.checkActive();  // Check that the other player is active
    this->checkTurn(); // Check that it's the player's turn
    if(this->getName()==other.getName()){ // Check that the player is not trying to coup themselves
        throw std::runtime_error("Player cannot coup themselves");
    }
    if(this->playerCoins<7){ // Check that the player has enough coins to perform coup
        throw std::runtime_error("Player does not have enough coins to coup");
    }
    other.active= false; // Mark the other player as inactive = removal from the game
    this->playerCoins-=7; // The player loses 7 coins
    Game::getInstance().setNumPlayers(Game::getInstance().getNumPlayers()-1); // Reduce the number of players in the game
    
    // Only advance turn if there are enough players left and last action wasn't bribe
    // This prevents nextTurn() from being called when the game should end (1 player remaining)
    if (Game::getInstance().getLastStep() != ActionType::Bribe && Game::getInstance().getNumPlayers() > 1) {
        Game::getInstance().nextTurn();
        this->canArrest = true; // Reset the ability to arrest for the next turn
        this->sanctioned = false; // Reset sanction status for the next turn
    }
    Game::getInstance().setLastStep(ActionType::Coup); // Update the last action to Coup
}
/**
 * @brief Implementation of the sanction action
 * 
 * Imposes a sanction on another player, preventing them from performing economic actions (gather, tax)
 * in their next turn. The action usually costs 3 coins.
 * Behavior varies according to the sanctioned player's role:
 * - Baron: receives one coin as compensation
 * - Judge: the sanctioning player pays an extra coin (total of 4)
 * 
 * @param other The player on whom to impose the sanction
 * @throws std::runtime_error if the player tries to sanction themselves, doesn't have enough coins,
 *                           or if the other player is already sanctioned
 */
void Player::sanction(Player& other) {
    // Basic checks
    this->checkActive(); // Check that the player is active
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    if(this->getName()==other.getName()){ // Check that the player is not trying to sanction themselves
        throw std::runtime_error("Player cannot sanction themselves");
    }
    if(this->playerCoins<3){ // Check that the player has enough coins to impose sanction
        throw std::runtime_error("Player does not have enough coins to sanction");
    }
    other.checkActive(); // Check that the other player is active
    if(other.sanctioned){ // Check that the other player is not already sanctioned
        throw std::runtime_error("Player has already been sanctioned");
    }
    
    // Handle according to the sanctioned player's role
    if(other.role==Role::Baron){ 
        // Baron: the other player receives one coin, the sanctioning player pays 3 coins
        other.playerCoins+=1;   // The Baron receives one coin as compensation
        this->playerCoins-=3;   // The sanctioning player pays 3 coins
        
    } else if(other.role==Role::Judge){ 
        // Judge: the sanctioning player pays an extra coin (total of 4 instead of 3)
        if(this->playerCoins<4){ // Check that the player has enough coins to sanction a Judge
            throw std::runtime_error("Player does not have enough coins to sanction");
        }
        this->playerCoins-=4;   // The sanctioning player pays 4 coins
        
    } else {
        // All other roles: regular sanction
        this->playerCoins-=3;   // The sanctioning player pays 3 coins
    }

    other.sanctioned= true;     // Mark the other player as sanctioned
    // Update game status and advance to next turn
    if (Game::getInstance().getLastStep() != ActionType::Bribe) {
        Game::getInstance().nextTurn();
        this->canArrest= true;    // Reset the ability to arrest for the next turn
        this->sanctioned= false;  // Reset sanction status for the next turn
    }
    Game::getInstance().setLastStep(ActionType::Sanction);  // Update the last action to Sanction
    return;
}

/**
 * @brief Implementation of the bribe action
 * 
 * Allows a player to pay 4 coins to get an additional turn (doesn't advance the turn to the next player).
 * The player is marked as "bribed", and this action allows the Judge to cancel it.
 * 
 * @throws std::runtime_error if the player is not active, if it's not their turn, or if they don't have enough coins (4)
 */
void Player::bribe(){
    this->checkActive(); // Check that the player is active
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    if(this->playerCoins<4){ // Check that the player has enough coins to perform bribe
        throw std::runtime_error("Player does not have enough coins to bribe");
    }
    this->playerCoins-=4; // The player loses 4 coins
    this->isBribed= true; // Mark the player as bribed
    Game::getInstance().setLastStep(ActionType::Bribe); // Update the last action to Bribe and don't advance to next turn
}

/**
 * @brief Returns the list of available actions for the player
 * 
 * Provides the list of basic actions that every player can perform.
 * Derived classes for specific roles can add additional actions to this list.
 * 
 * @return Vector containing the action types the player can perform
 */
std::vector<ActionType> Player::getAvailableActions() const{
     return {
        ActionType::Gather,  // Gather
        ActionType::Tax,     // Tax
        ActionType::Bribe,   // Bribe
        ActionType::Arrest,  // Arrest
        ActionType::Coup,    // Coup
        ActionType::Sanction // Sanction
    };
}
}
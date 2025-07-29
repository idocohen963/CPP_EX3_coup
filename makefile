# Makefile for the Coup game project
# This Makefile compiles the main game, test executable, and demo executable.
# It uses SFML for graphics and window management.
# It also includes rules for cleaning up build artifacts and running tests with Valgrind.

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -g -I. -IPLAYER -IGAME -IGUI
# Source directories
PLAYER_DIR = PLAYER
GAME_DIR = GAME
GUI_DIR = GUI
TEST_DIR = TEST

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Player source files
PLAYER_SRCS = $(PLAYER_DIR)/player.cpp $(PLAYER_DIR)/governor.cpp $(PLAYER_DIR)/spy.cpp \
              $(PLAYER_DIR)/baron.cpp $(PLAYER_DIR)/general.cpp $(PLAYER_DIR)/judge.cpp \
              $(PLAYER_DIR)/merchant.cpp $(PLAYER_DIR)/PlayerFactory.cpp

# Game source files
GAME_SRCS = $(GAME_DIR)/game.cpp

# Test source files
TEST_SRCS = $(TEST_DIR)/testGame.cpp $(TEST_DIR)/testPlayer.cpp $(TEST_DIR)/testRole.cpp

# GUI source files
GUI_SRCS = $(GUI_DIR)/GameGUI.cpp

# Common object files
COMMON_OBJS = $(PLAYER_SRCS:.cpp=.o) $(GAME_SRCS:.cpp=.o)

# Demo
DEMO_SRCS = $(GAME_DIR)/demo.cpp
DEMO_OBJS = $(DEMO_SRCS:.cpp=.o) $(COMMON_OBJS)
DEMO_TARGET = demo_exec

# Test
TEST_OBJS = $(TEST_SRCS:.cpp=.o) $(COMMON_OBJS)
TEST_TARGET = test_exec

# GUI Demo
GUI_DEMO_SRCS = $(GUI_DIR)/gui_demo.cpp
GUI_DEMO_OBJS = $(GUI_DEMO_SRCS:.cpp=.o) $(GUI_SRCS:.cpp=.o) $(COMMON_OBJS)
GUI_TARGET = gui_exec

# Default target
all: demo test gui

# Compilation rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Demo target
demo: $(DEMO_TARGET)
	./$(DEMO_TARGET)

$(DEMO_TARGET): $(DEMO_OBJS)
	$(CXX) $(CXXFLAGS) -o $(DEMO_TARGET) $(DEMO_OBJS) $(SFML_LIBS)

# Test target
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

# GUI target
gui: $(GUI_TARGET)
	./$(GUI_TARGET)

$(GUI_TARGET): $(GUI_DEMO_OBJS)
	$(CXX) $(CXXFLAGS) -o $(GUI_TARGET) $(GUI_DEMO_OBJS) $(SFML_LIBS)

# Valgrind targets
valgrind: $(TEST_TARGET) $(DEMO_TARGET)
	@echo "=== Running Valgrind on Tests ==="
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_TARGET)
	@echo "=== Running Valgrind on Demo ==="
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(DEMO_TARGET)

# Clean target
clean:
	rm -f $(PLAYER_DIR)/*.o $(GAME_DIR)/*.o $(GUI_DIR)/*.o $(TEST_DIR)/*.o *.o
	rm -f $(DEMO_TARGET) $(TEST_TARGET) $(GUI_TARGET)

.PHONY: all demo test gui valgrind clean

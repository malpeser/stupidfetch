CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = sfetch
BUILD_DIR = build
SRC_DIR = src
INSTALL_DIR = /usr/local/bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
FLAGS = -lcurl

all:
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(BUILD_DIR)/$(TARGET) $(FLAGS)
install: all
	cp $(BUILD_DIR)/$(TARGET) $(INSTALL_DIR)/$(TARGET)
clean:
	rm -r $(BUILD_DIR)
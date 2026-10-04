CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

TARGET = pulse_generator

SRC = simulator/pulse_generator.cpp \
      src/energy_calculator.cpp \
      src/analytics.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

driver:
	$(MAKE) -C driver

clean:
	rm -f $(TARGET)
	rm -f *.o
	$(MAKE) -C driver clean

.PHONY: all driver clean

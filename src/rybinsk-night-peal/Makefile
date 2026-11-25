BUILD := build

.PHONY: all test configure clean

all: test

configure:
	cmake -S . -B $(BUILD) -DCMAKE_BUILD_TYPE=Debug

test: configure
	cmake --build $(BUILD) -j
	./$(BUILD)/rybinsk_tests

clean:
	rm -rf $(BUILD)

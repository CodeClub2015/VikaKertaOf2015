#include <vector>
#include <iostream>
#include <Windows.h>
#include <algorithm> // std::for_each
#include <string>
class GameState {
public:
	GameState() {}
	virtual ~GameState() {}
	virtual void update() = 0;
	virtual void draw() = 0;
};
class StackMachine {
private:
	std::vector<GameState*> states;
	StackMachine() {}
	StackMachine(const StackMachine&) = delete;
	StackMachine(const StackMachine&&) = delete;
	StackMachine& operator=(const StackMachine&) = delete;
	StackMachine& operator=(const StackMachine&&) = delete;
public:
	// t‰‰ vaan sen takia ett‰ pysyy simppelin‰
	static StackMachine& getInstance() {
		static StackMachine instance;
		return instance;
	}
	~StackMachine() {}
	void update() {
		if (states.empty()) exit(0);
		// pp‰ivit‰ vikaa taas
		states.back()->update();
	}
	void draw() {
		if (states.empty()) return;
		// piirr‰ vika
		states.back()->draw();
	}
	void push(GameState* newState) {
		states.push_back(newState);
	}
	void pop() {
		delete states.back();
		states.pop_back(); // poista vika
	}
	// joskus myˆs change(GameState*) => pop & push
};
class GameplayScreen : public GameState {
public:
	GameplayScreen() : GameState() {}
	~GameplayScreen() {}
	void update() override {
		std::cin.ignore();
		std::cin.ignore();
		// luetaan vaan konsolia ja menn‰‰ takaisin menuun
		StackMachine::getInstance().pop();
	}
	void draw() override {
		std::cout << "@GameplayScreen" << std::endl;
	}
};
class OptionsScreen : public GameState {
public:
	OptionsScreen() : GameState() {}
	~OptionsScreen() {}
	void update() override {}
	void draw() override {}
};


class MainMenuScreen : public GameState {
public:
	MainMenuScreen() : GameState() {}
	~MainMenuScreen() {}
	void update() override {
		bool valid = false;
		do {
			valid = true;
			char c = '\0';
			std::cin >> c;
			switch (c) {
				case 'a': 
					StackMachine::getInstance().push(new GameplayScreen);
					break;
				case 'b': 
					StackMachine::getInstance().push(new OptionsScreen);
					break;
				case 'x': 
					// koska pistettiin updateen ett‰ jos
					// ei ole stateja => niin exit
					StackMachine::getInstance().pop();
					break;
				default:
					valid = false;
					break;
			}
		} while (valid == false);
	}
	void draw() override {
		std::cout << "what do you want to do?" << std::endl;
		std::cout << "a) start game" << std::endl;
		std::cout << "b) goto options " << std::endl;
		std::cout << "x) exit" << std::endl;
	}
};




int main() {
	StackMachine& instance = StackMachine::getInstance();
	instance.push(new MainMenuScreen); // peli l‰htee p‰‰lle main menusta
	while (true) {
		instance.draw();
		instance.update(); // update joskus tappaa loopin
	}
	return 0;
}
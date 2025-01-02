#include <iostream>
#include <string>
#include "game.h"

using namespace CnD::Cards;
using namespace CnD::Players;

int InputInt(const std::string& prompt)
{
	int output = 0;
	bool validInput = false;
	std::string userInput;
	
	std::cout << prompt;
	while (!validInput)
	{
		std::cin >> userInput;
		try 
		{ 
			output = std::stoi(userInput); 
			std::cout << std::endl;
			return output;
		}
		catch (std::exception& e) { std::cout << "Invalid input: "; }
	}

	std::cout << std::endl;
	return output;
}

int main()
{
	int playerCount = InputInt("No. of players (includes dealer): ");
	int initialBank = InputInt("Initial amount in bank: ");
	int initialWallet = InputInt("Initial amount in player wallet: ");

	Game game(playerCount, initialBank, initialWallet);
	game.Run();

	return 0;
}
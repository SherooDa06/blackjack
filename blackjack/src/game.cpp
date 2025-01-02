#include "game.h"
#include <iostream>
#include <string>

using namespace CnD::Players;

namespace Blackjack
{
	struct CardValues { int totalValueA, totalValueB; };
	
	CardValues CalculateCardTotal(std::vector<CnD::Cards::Card*>& deck)
	{
		using namespace CnD::Cards;
		
		CardValues values = { 0, 0 };
		for (CnD::Cards::Card* cardInHand : deck)
		{
			if (
				cardInHand->GetValue() == Value::Jack ||
				cardInHand->GetValue() == Value::Queen ||
				cardInHand->GetValue() == Value::King
				)
			{
				values.totalValueA += 10;
				values.totalValueB += 10;
			}
			else
			{
				values.totalValueA += (int)(cardInHand->GetValue()) + 1;
				if (cardInHand->GetValue() == Value::Ace) values.totalValueB += 11;
				else values.totalValueB += (int)(cardInHand->GetValue()) + 1;
			}
		}

		return values;
	}

	Player::Player(const int& id) : m_PlayerID(id), m_Wallet(0), m_Bet(0) {}

	bool Player::DetectBust()
	{
		using namespace CnD::Cards;

		CardValues totalValues = CalculateCardTotal(m_Hand);

		//__debugbreak();
		return totalValues.totalValueA > 21 && totalValues.totalValueB > 21;
	}

	void Player::PrintHand()
	{
		std::cout << std::endl;
		std::cout << "Player " << m_PlayerID << "'s hand: " << std::endl;
		for (CnD::Cards::Card* card : m_Hand) card->Print();
	}

	void Player::ClearHand() { m_Hand.clear(); }

	int Player::GetWallet() const { return m_Wallet; }
	void Player::SetWallet(const int& value) { m_Wallet = value; }
	int Player::GetBet() const { return m_Bet; }

	void Player::SetBet()
	{
		std::string betInput;
		bool betSuccessful = false;

		while (!betSuccessful)
		{
			std::cout << std::endl;
			std::cout << "You have " << m_Wallet << " chips." << std::endl;
			std::cout << "Place your bets: ";
			std::cin >> betInput;

			try
			{
				int betValue = std::stoi(betInput);
				m_Wallet -= betValue;
				m_Bet = betValue;
				betSuccessful = true;
			}
			catch (std::exception& e)
			{
				std::cout << "Invalid bet value" << std::endl;
			}
			std::cout << std::endl;
		}
	}

	int Player::GetPlayerID() const { return m_PlayerID; }

	void Dealer::DealToSelf(std::vector<CnD::Cards::Card*>* deckPointer)
	{
		using namespace CnD::Cards;

		std::cout << "Dealer's hand: " << std::endl;
		(*deckPointer)[0]->Unhide();
		(*deckPointer)[0]->Print();
		Deal(*deckPointer, *this);
		(*deckPointer)[0]->Print();
		Deal(*deckPointer, *this);
	}

	bool Dealer::DetectBust()
	{
		using namespace CnD::Cards;

		CardValues totalValues = CalculateCardTotal(m_Hand);

		return totalValues.totalValueA > 21 && totalValues.totalValueB > 21;
	}

	void Dealer::ClearHand() { m_Hand.clear(); }

	int Dealer::GetBank() const { return m_Bank; }
	void Dealer::SetBank(const int& value) { m_Bank = value; }
	void Dealer::PayPlayer(Blackjack::Player& payedPlayer, const int& amount) 
		{ payedPlayer.SetWallet(payedPlayer.GetBet() + amount); }
}

Game::Game(const int& playerCount, const int& initialBank, const int& initialWallet)
	: m_NonDealers({}), m_Dealer(), p_DrawPile(nullptr), p_DiscardPile(nullptr), m_DefaultBank(initialBank)
{
	for (int playerIndex = 0; playerIndex < (playerCount - 1); playerIndex++)
	{
		Blackjack::Player newPlayer(playerIndex + 1);
		m_NonDealers.push_back(newPlayer);
	}

	m_Dealer.Initialize(false);

	p_DrawPile = CnD::GetDrawPilePointer();
	p_DiscardPile = CnD::GetDiscardPilePointer();

	m_Dealer.SetBank(initialBank);
	for (Blackjack::Player& player : m_NonDealers)
	{
		player.SetWallet(initialWallet);
		m_Dealer.SetBank(m_Dealer.GetBank() - initialWallet);
	}
}

void Game::Run()
{
	bool playerIsBroke = false;

	while (!playerIsBroke)
	{
		Bet();
		FirstDeal(2);

		for (Blackjack::Player& player : m_NonDealers) player.PrintHand();
		for (Blackjack::Player& player : m_NonDealers)
		{
			Play(player);
			
			playerIsBroke = player.GetWallet() == 0 || m_Dealer.GetBank() == 0;
			if (playerIsBroke) break;
		}

		DealerPlay();
		CalculateWinnings();
		Reset();
	}
}

void Game::Bet()
{
	for (Blackjack::Player& player : m_NonDealers)
	{
		std::cout << "Player " << player.GetPlayerID() << "'s turn to bet.";
		player.SetBet();
	}
}

void Game::FirstDeal(const int& cardInHandCount)
{
	m_Dealer.DealToSelf(p_DrawPile);

	for (int cardIterator = 0; cardIterator < cardInHandCount; cardIterator++)
	{
		for (Blackjack::Player& player : m_NonDealers) 
		{ 
			(*p_DrawPile)[0]->Unhide();
			m_Dealer.Deal(*p_DrawPile, player); 
		}
	}
}

void Game::Play(Blackjack::Player& playingPlayer)
{
	std::cout << std::endl;
	std::cout << "Player " << playingPlayer.GetPlayerID() << "'s turn:";
	playingPlayer.PrintHand();
	std::string prompt;

	while (prompt != "s")
	{
		std::cout << "Hit or stand? (h/s): ";
		std::cin >> prompt;

		if (prompt == "h")
		{
			(*p_DrawPile)[0]->Unhide();
			m_Dealer.Deal(*p_DrawPile, playingPlayer);
			playingPlayer.PrintHand();
		};

		if (playingPlayer.DetectBust())
		{
			std::cout << "Player " << playingPlayer.GetPlayerID() << " has gone bust!" << std::endl;
			break;
		}
	}
}

void Game::DealerPlay()
{
	std::vector<CnD::Cards::Card*>* pDealerHand = m_Dealer.GetHandPointer();
	std::vector<CnD::Cards::Card*>* pDrawPile = CnD::GetDrawPilePointer();

	Blackjack::CardValues dealerValues = { 0, 0 };

	for (CnD::Cards::Card* cardInHand : *pDealerHand) cardInHand->Unhide();

	std::cout << std::endl;
	std::cout << "Dealer's hand: " << std::endl;
	for (CnD::Cards::Card* card : *pDealerHand) card->Print();
	dealerValues = Blackjack::CalculateCardTotal(*pDealerHand);

	while (dealerValues.totalValueA < 17 || dealerValues.totalValueB < 17)
	{
		(*pDrawPile)[0]->Unhide();
		(*pDrawPile)[0]->Print();
		m_Dealer.Deal(*pDrawPile, m_Dealer);

		dealerValues = Blackjack::CalculateCardTotal(*pDealerHand);
	}

	std::cout << std::endl;
}

void Game::CalculateWinnings()
{
	using namespace Blackjack;
	
	int totalTokensHeld = 0;
	bool dealerIsBust = m_Dealer.DetectBust();
	CardValues dealerCardValues = CalculateCardTotal(*(m_Dealer.GetHandPointer()));

	if (dealerIsBust) std::cout << "Dealer has gone bust!" << std::endl;

	for (Blackjack::Player& player : m_NonDealers)
	{
		CardValues playerCardValues = CalculateCardTotal(*(player.GetHandPointer()));	
		
		if (player.DetectBust()) std::cout << "Player " << player.GetPlayerID() << " has gone bust, loses " << player.GetBet() << " tokens! ";
		else
		{
			bool playerBeatDealer = playerCardValues.totalValueB > 21 ?
				playerCardValues.totalValueA > dealerCardValues.totalValueA || playerCardValues.totalValueA > dealerCardValues.totalValueB :
				playerCardValues.totalValueB > dealerCardValues.totalValueA || playerCardValues.totalValueB > dealerCardValues.totalValueB;

			bool playerTiedDealer =
				playerCardValues.totalValueA == dealerCardValues.totalValueA ||
				playerCardValues.totalValueA == dealerCardValues.totalValueB ||
				playerCardValues.totalValueB == dealerCardValues.totalValueB;

			if (dealerIsBust || playerBeatDealer)
			{
				int newWalletValue = player.GetWallet() + (player.GetBet() * 2);
				player.SetWallet(newWalletValue);
				std::cout << "Player " << player.GetPlayerID() << " gained " << player.GetBet() << " tokens; ";
			}
			else if (playerTiedDealer)
			{
				int newWalletValue = player.GetWallet() + player.GetBet();
				player.SetWallet(newWalletValue);
				std::cout << "Player " << player.GetPlayerID() << " retained " << player.GetBet() << " tokens; ";
			}
			else std::cout << "Player " << player.GetPlayerID() << " loses " << player.GetBet() << " tokens; ";
		}
		std::cout << "Player " << player.GetPlayerID() << " now has " << player.GetWallet() << " tokens." << std::endl;
		totalTokensHeld += player.GetWallet();
	}

	m_Dealer.SetBank(m_DefaultBank - totalTokensHeld);
	std::cout << m_Dealer.GetBank() << " tokens remaning in bank." << std::endl;
	std::cout << std::endl;
}

void Game::Reset()
{
	m_Dealer.ClearHand();
	
	for (Blackjack::Player& player : m_NonDealers)
		player.ClearHand();

	m_Dealer.Initialize(false);
}
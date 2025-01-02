#pragma once
#include <vector>
#include "players.h"

namespace Blackjack
{	
	class Player : public CnD::Players::Player
	{
	public:
		Player(const int& id);
		
		bool DetectBust();
		void PrintHand();
		void ClearHand();

		int GetWallet() const;
		void SetWallet(const int& value);

		int GetBet() const;
		void SetBet();

		int GetPlayerID() const;

	private:
		int m_Wallet;
		int m_Bet;

		const int m_PlayerID;
	};

	class Dealer : public CnD::Players::Dealer
	{
	public:
		void DealToSelf(std::vector<CnD::Cards::Card*>* deckPointer);
		bool DetectBust();
		void ClearHand();

		int GetBank() const;
		void SetBank(const int& value);

		void PayPlayer(Blackjack::Player& payedPlayer, const int& amount);
	private:
		int m_Bank;
	};
}

class Game
{
public:
	Game(const int& playerCount, const int& initialBank, const int& initialWallet);

	void Run();

private:
	void Bet();
	void FirstDeal(const int& cardInHandCount);
	void Play(Blackjack::Player& playingPlayer);
	void DealerPlay();
	void CalculateWinnings();
	void Reset();

	std::vector<Blackjack::Player> m_NonDealers;
	Blackjack::Dealer m_Dealer;

	std::vector<CnD::Cards::Card*>* p_DrawPile;
	std::vector<CnD::Cards::Card*>* p_DiscardPile;

	int m_DefaultBank;
};
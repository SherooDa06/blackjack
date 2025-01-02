#include "players.h"
#include "cards.h"
#include <random>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>

namespace CnD
{
	namespace Players
	{
		Player::Player() = default;

		std::vector<Cards::Card*>* Player::GetHandPointer() { return &m_Hand; }
		std::vector<Cards::Card*>* Player::GetPoolPointer() { return &m_Pool; }

		void Player::CollectCard(Cards::Card* collectedCard) { m_Hand.push_back(collectedCard); }

		void Player::PlayCard(int playedCardIndex)
		{
			auto cardInHandLocation = m_Hand.begin() + playedCardIndex;
			try
			{
				m_Pool.insert(m_Pool.begin(), m_Hand[playedCardIndex]);
				m_Hand.erase(cardInHandLocation);
			}
			catch (std::exception& e) { throw e; }
		}

		void Player::PlayCard(Cards::Card* playedCard)
		{
			auto cardInHandLocation = std::find(m_Hand.begin(), m_Hand.end(), playedCard);
			try
			{
				m_Pool.insert(m_Pool.begin(), playedCard);
				m_Hand.erase(cardInHandLocation);
			}
			catch (std::exception& e) { throw e; }
		}

		void Dealer::Initialize(bool includeJokers)
		{
			std::vector<Cards::Card>* standardDeck = GetStandardDeckPointer();
			std::vector<Cards::Card*>* drawPile = GetDrawPilePointer();

			InitializeDeck((*standardDeck), includeJokers);
			Shuffle((*standardDeck), (*drawPile));
		}

		void Dealer::Shuffle(std::vector<Cards::Card>& inputDeck, std::vector<Cards::Card*>& outputDeck)
		{	
			int sizeOfDeck = inputDeck.size();
			
			auto seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine rng(seed);
			
			std::vector<int> cardIds;
			for (int i = 0; i <= (inputDeck.size() - 1); i++)
				cardIds.push_back(i);

			std::shuffle(cardIds.begin(), cardIds.end(), rng);
			
			outputDeck.clear();
			for (int cardId : cardIds) outputDeck.push_back(&inputDeck[cardId]);
		}

		void Dealer::Shuffle(std::vector<Cards::Card*>& inputDeck)
		{
			auto seed = std::chrono::system_clock::now().time_since_epoch().count();
			std::default_random_engine rng(seed);

			std::shuffle(inputDeck.begin(), inputDeck.end(), rng);
		}

		void Dealer::Deal(std::vector<Cards::Card*>& deck, Player& dealtPlayer)
		{
			if(deck.size())
			{ 
				dealtPlayer.CollectCard(deck[0]);
				deck.erase(deck.begin());
			}
		}
	}

	bool CardIsInStack(const std::vector<Cards::Card*>& stack, const Cards::Card* card)
	{
		for (Cards::Card* iteratedCard : stack)
			if (iteratedCard == card) return true;

		return false;
	}
}
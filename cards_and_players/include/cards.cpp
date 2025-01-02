#include "cards.h"
#include <iostream>

const char* SuitNames[5]{ "Spades", "Hearts", "Diamonds", "Clubs", "Joker" };
const char* ValueNames[13]
{
	"Ace",
	"2", "3", "4",
	"5", "6", "7",
	"8", "9", "10",
	"Jack", "Queen", "King"
};

namespace CnD
{
	std::vector<Cards::Card> StandardDeck;
	std::vector<Cards::Card*> DrawPile;
	std::vector<Cards::Card*> DiscardPile;

	namespace Cards
	{
		Card::Card(const Suit& suit, const Value& value)
			: m_Suit(suit), m_Value(value), m_IsHidden(true) {}

		void Card::Print() const
		{
			std::cout << '\t';
			if (m_IsHidden) std::cout << "X of XXXXXXX" << std::endl;
			else
			{
				if (m_Suit == Suit::Joker) std::cout << "Joker" << std::endl;
				else std::cout << ValueNames[(int)m_Value] << " of " << SuitNames[(int)m_Suit] << std::endl;
			}
		}

		void Card::Hide() { m_IsHidden = true; }
		void Card::Unhide() { m_IsHidden = false; }

		Suit Card::GetSuit() { return m_Suit; }
		Value Card::GetValue() { return m_Value; }
	}

	void InitializeDeck(std::vector<Cards::Card>& deck, bool includeJokers)
	{
		using namespace Cards;

		deck.clear();

		for (int suitIndex = 0; suitIndex < 4; suitIndex++)
		{
			for (int valueIndex = 0; valueIndex < 13; valueIndex++)
			{
				Card newCard((Suit)suitIndex, (Value)valueIndex);
				deck.push_back(newCard);
			}
		}
		
		if (includeJokers)
		{
			Card newJoker(Suit::Joker, Value::Joker);
			for (int i = 0; i < 2; i++) deck.push_back(newJoker);
		}
	}

	std::vector<Cards::Card>* GetStandardDeckPointer() { return &StandardDeck; }
	std::vector<Cards::Card*>* GetDrawPilePointer() { return &DrawPile; }
	std::vector<Cards::Card*>* GetDiscardPilePointer() { return &DiscardPile; }
}
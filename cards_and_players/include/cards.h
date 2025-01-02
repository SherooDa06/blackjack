#pragma once
#include <vector>

namespace CnD
{
	namespace Cards
	{
		const enum class Suit { Spades, Hearts, Diamonds, Clubs, Joker};
		const enum class Value
		{
			Ace,
			Two, Three, Four,
			Five, Six, Seven,
			Eight, Nine, Ten,
			Jack, Queen, King,
			Joker
		};

		class Card
		{
		public:
			Card(const Suit& suit, const Value& value);

			void Print() const;

			void Hide();
			void Unhide();

			Suit GetSuit();
			Value GetValue();

		private:
			const Suit m_Suit;
			const Value m_Value;

			bool m_IsHidden;
		};

	}

	void InitializeDeck(std::vector<Cards::Card>& deck, bool includeJokers);
	std::vector<Cards::Card>* GetStandardDeckPointer();
	std::vector<Cards::Card*>* GetDrawPilePointer();
	std::vector<Cards::Card*>* GetDiscardPilePointer();
}
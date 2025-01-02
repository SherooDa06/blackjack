#pragma once
#include <vector>
#include "cards.h"

namespace CnD
{
	namespace Players
	{
		class Player
		{
		public:
			Player();

			std::vector<Cards::Card*>* GetHandPointer();
			std::vector<Cards::Card*>* GetPoolPointer();

			void CollectCard(Cards::Card* collectedCard);
			void PlayCard(int playedCardIndex);
			void PlayCard(Cards::Card* playedCard);

			int GetPlayerID() const;

		protected:
			std::vector<Cards::Card*> m_Hand;
			std::vector<Cards::Card*> m_Pool;
		};

		class Dealer : public Player
		{
		public:
			void Initialize(bool includeJokers);

			void Shuffle(std::vector<Cards::Card>& inputDeck, std::vector<Cards::Card*>& outputDeck);
			void Shuffle(std::vector<Cards::Card*>& inputDeck);

			void Deal(std::vector<Cards::Card*>& deck, Player& dealtPlayer);
		};
	}

	bool CardIsInStack(const std::vector<Cards::Card*>& stack, const Cards::Card* card);
}
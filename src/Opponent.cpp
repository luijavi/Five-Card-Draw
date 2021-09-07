#include <iostream>
#include "Opponent.h"

Opponent::Opponent()
{
}

Opponent::~Opponent()
{
}

int Opponent::GetDiscardCount()
{
	switch (hand_combo)
	{
		case HandCombo::StraightFlush:
		case HandCombo::FourOfAKind:
		case HandCombo::FullHouse:
		case HandCombo::Flush:
		case HandCombo::Straight:
		case HandCombo::TwoPair:
		{
			return 0;
		}
		case HandCombo::ThreeOfAKind:
		{
			return 2;
		}
		case HandCombo::Pair:
		{
			return 3;
		}
		case HandCombo::HighCard:
		{
			if (NearFlush() || NearStraight())
			{
				return 1;
			}

			if (HasAce())
			{
				return 4;
			}

			return 3;
		}
	}
}

void Opponent::TakeCard(const Card& card)
{
	hand->Push(card);
}

void Opponent::DiscardAndDraw(CardPile*& discard_pile, CardPile*& draw_pile, int opponent_num)
{
	int num_discard = GetDiscardCount();

	// Display number of cards opponents are discarding and drawing
	std::cout << "Computer Player " << opponent_num << " is discarding "
		<< num_discard << " card(s)." << std::endl << std::endl;

	switch (hand_combo)
	{
		case HandCombo::ThreeOfAKind:
		{
			for (std::size_t i = 0; i < max_hand_size; ++i)
			{
				if (hand->GetCard(i).GetRank() != combo_high_card.GetRank())
				{
					discard_pile->Push(hand->GetCard(i));
					hand->Insert(draw_pile->Pop(), i);
				}
			}
		} break;
		case HandCombo::Pair:
		{

		} break;
	}


}

bool Opponent::NearFlush() const
{
	return CountSuit(hand->GetCard(0).GetSuit()) == 4
		|| CountSuit(hand->GetCard(1).GetSuit()) == 4;
}

bool Opponent::NearStraight() const
{
	int card_1_rank = static_cast<int>(hand->GetCard(0).GetRank());
	int card_2_rank = static_cast<int>(hand->GetCard(1).GetRank());
	int card_4_rank = static_cast<int>(hand->GetCard(3).GetRank());
	int card_5_rank = static_cast<int>(hand->GetCard(4).GetRank());

	return ((card_1_rank + 3) == card_4_rank) ||
		   ((card_2_rank + 3) == card_5_rank);
}

#include <algorithm>
#include <cassert>
#include "Player.h"


Player::Player()
	:
	hand_combo(HandCombo::HighCard),
	max_hand_size(5)
{
	hand = nullptr;
	hand = new CardPile(max_hand_size);
}

Player::~Player()
{
	delete hand;
	hand = nullptr;
}

void Player::TakeCard(const Card& c)
{
	hand->Push(c);
}

void Player::TakeCard(const Card& card, std::size_t target_pos)
{
	hand->Insert(card, target_pos);
}

void Player::SortHand()
{
	hand->Sort();
	EvaluateHand();
}

// Sets the hand combo
void Player::EvaluateHand()
{
	// Start by testing highest hand rank
	if (HasStraightFlush())
	{
		hand_combo = HandCombo::StraightFlush;
		combo_high_card = GetHighCard();
		return;
	}
	else if (HasFourOfAKind())
	{
		hand_combo = HandCombo::FourOfAKind;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card current_card = hand->GetCard(i);
			if (CountRank(current_card.GetRank()) == 4)
			{
				combo_high_card = current_card;
				break;
			}
		}

		return;
	}
	else if (HasFullHouse())
	{
		hand_combo = HandCombo::FullHouse;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card current_card = hand->GetCard(i);
			if (CountRank(current_card.GetRank()) == 3)
			{
				combo_high_card = current_card;
				break;
			}
		}

		return;
	}
	else if (HasFlush())
	{
		hand_combo = HandCombo::Flush;
		combo_high_card = GetHighCard();
		return;
	}
	else if (HasStraight())
	{
		hand_combo = HandCombo::Straight;
		combo_high_card = GetHighCard();
		return;
	}
	else if (HasThreeOfAKind())
	{
		hand_combo = HandCombo::ThreeOfAKind;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card current_card = hand->GetCard(i);
			if (CountRank(current_card.GetRank()) == 3)
			{
				combo_high_card = current_card;
				break;
			}
		}

		return;
	}
	else if (HasTwoPair())
	{
		hand_combo = HandCombo::TwoPair;
		bool first_pair_found = false;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card current_card = hand->GetCard(i);
			if (HasPair(current_card.GetRank()))
			{
				for (std::size_t j = (i + 1); j < max_hand_size; ++j)
				{
					Card second_card = hand->GetCard(j);
					if ((second_card.GetRank() != current_card.GetRank())
						&& HasPair(second_card.GetRank()))
					{
						if (second_card.GetRank() > current_card.GetRank())
						{
							combo_high_card = second_card;
							return;
						}
						else
						{
							combo_high_card = current_card;
							return;
						}
					}
				}
			}
		}

		return;
	}
	else if (HasPair())
	{
		hand_combo = HandCombo::Pair;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card current_card = hand->GetCard(i);
			if (CountRank(current_card.GetRank()) == 2)
			{
				combo_high_card = current_card;
				break;
			}
		}

		return;
	}
	else
	{
		combo_high_card = GetHighCard();
		hand_combo = HandCombo::HighCard;
	}
}

bool Player::HasPair(Card::Rank rank_test) const
{
	return CountRank(rank_test) == 2;
}

int Player::CountSuit(Card::Suit suit) const
{
	int num_suit = 0;

	for (std::size_t i = 0; i < max_hand_size; ++i)
	{
		if (hand->GetCard(i).GetSuit() == suit)
		{
			++num_suit;
		}
	}
	return num_suit;
}

int Player::CountRank(Card::Rank rank) const
{
	int num_rank = 0;

	for (std::size_t i = 0; i < max_hand_size; ++i)
	{
		if (hand->GetCard(i).GetRank() == rank)
		{
			++num_rank;
		}
	}
	return num_rank;
}

bool Player::HasAce() const
{
	return CountRank(Card::Rank::Ace);
}

Card Player::GetHighCard() const
{
	Card high_card = hand->GetCard(0);

	for (std::size_t i = 1; i < max_hand_size; ++i)
	{
		if (hand->GetCard(i) > high_card)
		{
			high_card = hand->GetCard(i);
		}
	}
	return high_card;
}

Card Player::GetLowCard() const
{
	Card low_card = hand->GetCard(0);

	for (std::size_t i = 0; i < max_hand_size; ++i)
	{
		if (hand->GetCard(i) < low_card)
		{
			low_card = hand->GetCard(i);
		}
	}
	return low_card;
}

bool Player::HasStraightFlush() const
{
	return HasFlush() && HasStraight();
}

bool Player::HasFlush() const
{
	return CountSuit(hand->GetCard(0).GetSuit()) == 5;
}

bool Player::HasFourOfAKind() const
{
	return (CountRank(hand->GetCard(0).GetRank()) == 4)
		|| (CountRank(hand->GetCard(1).GetRank()) == 4);
}

bool Player::HasFullHouse() const
{
	return HasThreeOfAKind() && HasPair();
}

bool Player::HasStraight() const
{
	if (HasAce())
	{
		bool low_straight = CountRank(Card::Rank::Two) == 1
						 && CountRank(Card::Rank::Three) == 1
						 && CountRank(Card::Rank::Four) == 1
						 && CountRank(Card::Rank::Five) == 1;

		bool high_straight = CountRank(Card::Rank::Ten) == 1
						  && CountRank(Card::Rank::Jack) == 1
						  && CountRank(Card::Rank::Queen) == 1
						  && CountRank(Card::Rank::King) == 1;

		return low_straight || high_straight;
	}
	else
	{
		Card::Rank current_rank = GetLowCard().GetRank();

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			if (CountRank(current_rank) == 1)
			{
				current_rank = static_cast<Card::Rank>(static_cast<int>(current_rank) + 1);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

bool Player::HasThreeOfAKind() const
{
	return (CountRank(hand->GetCard(0).GetRank()) == 3)
		|| (CountRank(hand->GetCard(2).GetRank()) == 3);
}

bool Player::HasTwoPair() const
{
	if (HasPair())
	{
		Card::Rank first_paired_rank;

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card::Rank current_rank = hand->GetCard(i).GetRank();
			if (HasPair(current_rank))
			{
				first_paired_rank = current_rank;
				break;
			}
		}

		for (std::size_t i = 0; i < max_hand_size; ++i)
		{
			Card::Rank current_rank = hand->GetCard(i).GetRank();

			if ((current_rank != first_paired_rank) && (HasPair(current_rank)))
			{
				return true;
			}
		}
	}

	return false;
}

bool Player::HasPair() const
{
	for (int i = 0; i < max_hand_size - 1; ++i)
	{
		if (CountRank(hand->GetCard(i).GetRank()) == 2)
		{
			return true;
		}
	}
	return false;
}


int Player::GetDiscardCount()
{
	int num_dicard = 0;
	int max_discard = 3;
	std::cout << std::endl;

	if (HasAce())
	{
		max_discard = 4;
		std::cout << "Since you have an Ace in your hand, " 
			<< "you may discard up to 4 cards." << std::endl;
	}
	else
	{
		std::cout << "You can discard up to 3 cards." << std::endl;
	}


	std::cout << "How many cards would you like to discard?" << std::endl;
	std::cout << "> ";
	std::cin >> num_dicard;

	// Validate entry
	while (num_dicard > max_discard || num_dicard < 0)
	{
		std::cout << L"\nInvalid entry. Please enter a number between 0 and "
			<< max_discard << std::endl;
		std::cout << L"> ";
		std::cin >> num_dicard;
	}

	return num_dicard;
}

Card Player::Discard(std::size_t card_num)
{
	return hand->GetCard(card_num);
}

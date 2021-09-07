#include "Card.h"

std::ostream& operator<<(std::ostream& os, const Card& c)
{

	if (c.rank < Card::Rank::Ten)
	{
		os << static_cast<int>(c.rank);
	}
	else if (c.rank == Card::Rank::Ten)
	{
		os << 'T';
	}
	else if (c.rank == Card::Rank::Jack)
	{
		os << 'J';
	}
	else if (c.rank == Card::Rank::Queen)
	{
		os << 'Q';
	}
	else if (c.rank == Card::Rank::King)
	{
		os << 'K';
	}
	else if (c.rank == Card::Rank::Ace)
	{
		os << 'A';
	}


	if (c.suit == Card::Suit::Clubs)
	{
		os << 'C';
	}
	else if (c.suit == Card::Suit::Diamonds)
	{
		os << 'D';
	}
	else if (c.suit == Card::Suit::Hearts)
	{
		os << 'H';
	}
	else if (c.suit == Card::Suit::Spades)
	{
		os << 'S';
	}

	return os;
}

bool operator<(const Card& lhs, const Card& rhs)
{
	return lhs.rank < rhs.rank;
}

bool operator>(const Card& lhs, const Card& rhs)
{
	return rhs < lhs;
}

bool operator>=(const Card& lhs, const Card& rhs)
{
	return !(lhs < rhs);
}

bool operator<=(const Card& lhs, const Card& rhs)
{
	return !(lhs > rhs);
}

bool operator==(const Card& lhs, const Card& rhs)
{
	return lhs.rank == rhs.rank &&
		lhs.suit == rhs.suit;
}

bool operator!=(const Card& lhs, const Card& rhs)
{
	return !(lhs == rhs);
}

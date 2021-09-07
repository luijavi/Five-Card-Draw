#pragma once

#include <iostream> // For ostream

class Card
{
public:
	enum class Suit
	{
		Clubs = 1,
		Diamonds,
		Hearts,
		Spades
	};

	enum class Rank
	{
		Two = 2, Three, Four,
		Five, Six, Seven, Eight,
		Nine, Ten, Jack, Queen,
		King, Ace
	};

public:
	Card(Rank card_rank = Rank::Two, Suit card_suit = Suit::Clubs)
		:
		rank(card_rank),
		suit(card_suit)
	{}
	Card& operator=(const Card& rhs)
	{
		if (this != &rhs)
		{
			rank = rhs.rank;
			suit = rhs.suit;
		}
		return *this;
	}

	Rank GetRank() const { return rank; }
	Suit GetSuit() const { return suit; }

	// FRIEND FUNCTIONS
	friend std::ostream& operator <<(std::ostream& os, const Card& c);
	friend bool operator <(const Card& lhs, const Card& rhs);
	friend bool operator >(const Card& lhs, const Card& rhs);
	friend bool operator >=(const Card& lhs, const Card& rhs);
	friend bool operator <=(const Card& lhs, const Card& rhs);
	friend bool operator ==(const Card& lhs, const Card& rhs);
	friend bool operator !=(const Card& lhs, const Card& rhs);

private:
	Rank rank;
	Suit suit;
};
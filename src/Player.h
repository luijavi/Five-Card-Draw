#pragma once

#include "Card.h"
#include "CardPile.h"

enum class HandCombo
{
	HighCard = 1,
	Pair,
	TwoPair,
	ThreeOfAKind,
	Straight,
	Flush,
	FullHouse,
	FourOfAKind,
	StraightFlush
};

class Player
{
public:
	// CONSTRUCTOR(S) AND DESTRUCTOR
	Player();
	virtual ~Player();
	// MEMBER FUNCTIONS
	virtual void TakeCard(const Card& card);
	virtual void TakeCard(const Card& card, std::size_t target_pos);
	void SortHand();
	// VIRTUAL FUNCTIONS
	virtual int GetDiscardCount();
	Card Discard(std::size_t card_num);
	// CONSTANT MEMBER FUNCTIONS
	const CardPile& GetHand() const { return *hand; }
	const HandCombo& GetHandRank() const { return hand_combo; }
	const Card GetHandComboHighCard() const { return combo_high_card; }
	int GetCardsInHand() const { return hand->Size(); }
private:
	// PRIVATE MEMBER FUNCTIONS
	bool HasStraightFlush() const;
	bool HasFlush() const;
	bool HasFourOfAKind() const;
	bool HasFullHouse() const;
	bool HasStraight() const;
	bool HasThreeOfAKind() const;
	bool HasTwoPair() const;
	bool HasPair() const;
	// bool HasNextRank(Card::Rank current_rank);
protected:
	Card GetHighCard() const;
	Card GetLowCard() const;
	int CountSuit(Card::Suit suit) const;
	int CountRank(Card::Rank rank) const;
	bool HasAce() const;
	void EvaluateHand();
	bool HasPair(Card::Rank rank_test) const;
protected:
	const int max_hand_size;
	CardPile* hand;
	HandCombo hand_combo;
	Card combo_high_card;
};
#pragma once

#include <cstdlib>
#include <iostream>
#include "Card.h"

class CardPile
{
public:
	// CONSTRUCTOR(S) AND DESTRUCTOR
	CardPile(std::size_t pile_size);
	~CardPile();
	// MEMBER FUNCTIONS
	void Push(Card card);
	Card Pop();
	Card Remove(std::size_t target_pos);
	const Card GetCard(std::size_t card_pos) const;
	void Insert(const Card& card, std::size_t target_pos);
	void Shuffle();
	void Sort();
	void GroupByRank(Card::Rank rank);
	Card SwapCard(Card card_in, std::size_t target_pos);
	// CONSTANT MEMBER FUNCTIONS
	bool IsEmpty() const { return !Size(); }
	bool IsFull() const { return Size() == Capacity(); }
	std::size_t Size() const { return top + 1; }
	int Capacity() const { return capacity; }
	// bool FindNextRank(Card::Rank current_rank) const;

private:
	Card* cards;
	std::size_t top;
	std::size_t capacity;
};
#include <cassert>
#include <algorithm>
#include <random>
#include <chrono>
#include "CardPile.h"

CardPile::CardPile(std::size_t pile_size)
	:
	capacity(pile_size),
	top(-1),
	cards(nullptr)
{
	cards = new Card[capacity];
}

CardPile::~CardPile()
{
	delete[] cards;
	cards = nullptr;
}

void CardPile::Push(Card card)
{
	assert(!IsFull());

	cards[++top] = card;
}


Card CardPile::Pop()
{
	assert(!IsEmpty());
	return cards[top--];
}

Card CardPile::Remove(std::size_t target_pos)
{
	assert(target_pos >= 0 && target_pos < Size());

	Card target_card = cards[target_pos];

	// Move all of the cards left of target_pos over one space
	if (!target_pos)
	{
		return Pop();
	}
	else
	{
		for (std::size_t i = target_pos; i > 0; --i)
		{
			cards[i] = cards[i - 1];
		}
		--top;
	}
	
	return target_card;
}

const Card CardPile::GetCard(std::size_t card_pos) const
{
	assert(card_pos >= 0 && card_pos < Size());
	return cards[card_pos];
}

void CardPile::Insert(const Card& card, std::size_t target_pos)
{
	assert(target_pos >= 0);
	assert(target_pos <= Size());

	cards[target_pos] = card;
}

void CardPile::Shuffle()
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(cards, cards + Size(), std::default_random_engine(seed));
}

void CardPile::Sort()
{
	std::sort(cards, cards + Size());
}

void CardPile::GroupByRank(Card::Rank rank)
{
	// Count number of ranks that match argument
	Card* temp = nullptr;
	std::size_t rank_count = 0;
	for (std::size_t i = 0; i < Size(); ++i)
	{
		if (cards[i].GetRank() == rank)
		{
			++rank_count;
		}
	}

	temp = new Card[Size()];

	for (std::size_t i = 0, j = rank_count; i < Size() && j < Size(); ++i)
	{
		if (cards[i].GetRank() == rank)
		{
			temp[i] = cards[i];
		}
		else
		{
			temp[j] = cards[i];
			++j;
		}
	}

	std::copy(temp, temp + (Size() - 1), cards);
	delete[] temp;
	temp = nullptr;
}

Card CardPile::SwapCard(Card card_in, std::size_t target_pos)
{
	assert(target_pos >= 0);
	assert(target_pos < Size());

	Card c = cards[target_pos];
	cards[target_pos] = card_in;

	return c;
}

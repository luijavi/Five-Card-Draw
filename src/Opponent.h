#pragma once

#include "Player.h"
#include "CardPile.h"

class Opponent : public Player
{
public:
	Opponent();
	~Opponent() override;
	// INHERITED FUNCTIONS
	void TakeCard(const Card& card) override;
	// Card DiscardCard();
	void DiscardAndDraw(CardPile*& discard_pile, CardPile*& draw_pile, int opponent_num);
private:
	// MEMBER PRIVATE FUNCTIONS
	int GetDiscardCount() override;
	
	// CONSTANT PRIVATE FUNCTIONS
	bool NearFlush() const;
	bool NearStraight() const;
};
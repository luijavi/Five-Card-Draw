#pragma once

#include "CardPile.h"
#include "Player.h"
#include "Opponent.h"

class Game
{
public:
	// CONSTRUCTOR(S) AND DESTRUCTOR
	Game();
	~Game();
	// PUBLIC MEMBER FUNCTIONS
	void Go();
	static bool IsRunning();
	static void PrintStats();
private:
	// INTERNAL GAME PHASE FUNCTIONS
	void InitDrawPile();
	void DealCards();
	void DiscardAndDraw();
	void ShowHands();
	// CONST PRIVATE MEMBER FUNCTIONS
	void PrintHand(const CardPile& hand) const;
	void PrintHandCombo(const HandCombo hand_combo) const;
private:
	static constexpr int min_opponents = 0;
	static constexpr int max_opponents = 3;
	static constexpr int max_hand_size = 5;
	static int num_games;
	static int num_games_won;
	int num_opponents;
	CardPile* draw_pile;
	CardPile* discard_pile;
	Player* user_player;
	Opponent* opponent_players;
};
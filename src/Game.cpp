#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <vector>
#include "Game.h"

int Game::num_games = 0;
int Game::num_games_won = 0;

Game::Game()
	:
	draw_pile(nullptr),
	discard_pile(nullptr),
	user_player(nullptr),
	opponent_players(nullptr)
{
	// Get number of opponents from player
	std::cout << "Enter the number of opponents you'd like to play against (0 to 3):" << std::endl;
	std::cout << "> ";
	std::cin >> num_opponents;
	std::cout << std::endl;

	// TODO: Prevent invalid character entry
	// Check that valid number of opponents entered
	while (num_opponents < min_opponents || num_opponents > max_opponents)
	{
		std::cout << "Invalid entry. Please enter a value between 0 and 3:" << std::endl;
		std::cout << "> ";
		std::cin >> num_opponents;
		std::cout << std::endl;
	}

	user_player = new Player;

	// Create an array of Opponents if num_opponents > 0
	if (num_opponents)
	{
		opponent_players = new Opponent[num_opponents];
	}

}

Game::~Game()
{
	delete[] opponent_players;
	delete draw_pile;
	delete discard_pile;
	delete user_player;

	user_player = nullptr;
	opponent_players = nullptr;
	draw_pile = nullptr;
	discard_pile = nullptr;

	++num_games;
}

void Game::Go()
{
	InitDrawPile();
	DealCards();
	DiscardAndDraw();
	ShowHands();
}

bool Game::IsRunning()
{
	char user_choice = 'y';

	// Ask user if they want to play again if they've played at least 1 hand
	if (num_games > 0)
	{
		std::cout << "Would you like to play again? (y/n)" << std::endl;
		std::cout << "> ";
		std::cin.ignore();
		std::cin >> user_choice;
		std::cout << std::endl;
	}
	if (tolower(user_choice) == 'n')
	{
		PrintStats();
	}

	return tolower(user_choice) == 'y';
}

void Game::PrintStats()
{
	std::cout << std::endl;
	std::cout << "Game statistics:" << std::endl
			  << "----------------------------------" << std::endl
			  << "Number of games played: " << num_games << std::endl
			  << "Number of winning hands: " << num_games_won << std::endl;

	std::cout << std::endl << "Thank you for playing!" << std::endl;
}

void Game::InitDrawPile()
{
	// Print out status
	std::cout << std::endl << "The deck is being shuffled..." << std::endl << std::endl;

	// Create a card pile for 52 cards (standard deck)
	int num_ranks = 13;
	int num_suits = 4;
	draw_pile = new CardPile(num_ranks * num_suits);
	for (int s = static_cast<int>(Card::Suit::Clubs); s <= static_cast<int>(Card::Suit::Spades); ++s)
	{
		for (int r = static_cast<int>(Card::Rank::Two); r <= static_cast<int>(Card::Rank::Ace); ++r)
		{
			draw_pile->Push(Card(static_cast<Card::Rank>(r), static_cast<Card::Suit>(s)));
		}
	}

	draw_pile->Shuffle();

	// Create a new DiscardPile object that holds 16 cards
	// (max number of cards that can be discarded in a game)
	discard_pile = new CardPile(16);
}

void Game::DealCards()
{
	// Print message informing how many players are being delt cards
	std::cout << std::endl << "The cards are being dealt to "
		<< (num_opponents + 1) << " players..." << std::endl << std::endl;

	for (int i = 0; i < max_hand_size; ++i)
	{
		// Check whether there are any opponents to deal cards to
		if (num_opponents)
		{
			for (int j = 0; j < num_opponents; ++j)
			{
				std::cout << "Dealing card " << (i + 1) << " to opponent " << (j + 1) << std::endl;
				opponent_players[j].TakeCard(draw_pile->Pop());
			}
		}
		std::cout << "Dealing card " << (i + 1) << " to user player" << std::endl << std::endl;
		user_player->TakeCard(draw_pile->Pop());
	}


	// TODO: Test whether this first conditional is even needed
	if (num_opponents)
	{
		for (int i = 0; i < num_opponents; ++i)
		{
			opponent_players[i].SortHand();
		}
	}

	user_player->SortHand();
}

void Game::DiscardAndDraw()
{
	// Check whether there are opponents to discard cards before handling
	// opponent discard and draw phase
	if (num_opponents)
	{
		for (int i = 0; i < num_opponents; ++i)
		{
			opponent_players[i].DiscardAndDraw(discard_pile, draw_pile, (i + 1));
			opponent_players[i].SortHand();
		}
	}

	// Handle user player's discard and draw phase
	std::cout << std::endl << "The cards in your hand are:" << std::endl;
	PrintHand(user_player->GetHand());
	std::cout << std::endl;
	int num_discard = user_player->GetDiscardCount();
	std::cout << std::endl << "You will be discarding "
		<< num_discard << " card(s)." << std::endl;

	
	if (num_discard)
	{
		std::cout << "Enter card number you wish to discard. "
			<< "(Enter one at a time.)" << std::endl;
	}
	
	for (int i = 0; i < num_discard; ++i)
	{
		std::size_t target_pos;
		std::cout << "> ";
		std::cin >> target_pos;

		std::cout << "Discarding " << user_player->GetHand().GetCard(target_pos - 1)
			<< std::endl;

		discard_pile->Push(user_player->Discard(target_pos - 1));

		// discard_pile->Push(user_player->DiscardCard(target_pos - 1));

		user_player->TakeCard(draw_pile->Pop(), target_pos - 1);
	}
	if (num_discard)
	{
		user_player->SortHand();
	}
}

void Game::ShowHands()
{
	// First, determine who has the highest rank
	// Begin with evaluating finding the player's hand's rank and compare
	// all other ranks against it
	HandCombo best_combo = user_player->GetHandRank();
	bool user_player_won = true;	// Assumes user player has won before comparing
	int winning_opponent = 0;		// In case opponent wins
	
	std::cout << std::endl << "Showing hands..." << std::endl << std::endl;
	std::cout << "Your hand:" << std::endl;
	PrintHand(user_player->GetHand());
	std::cout << "\t";
	PrintHandCombo(best_combo);
	std::cout << std::endl;

	// Compare user player's hand with opponents'
	if (num_opponents)
	{
		for (int i = 0; i < num_opponents; ++i)
		{
			std::string current_player = "Opponent " + std::to_string(i + 1);
			HandCombo current_combo = opponent_players[i].GetHandRank();

			std::cout << current_player << "'s hand: " << std::endl;
			PrintHand(opponent_players[i].GetHand());
			std::cout << "\t";
			PrintHandCombo(current_combo);
			std::cout << std::endl;

			// Short circuit comparison - if rank is higher, don't need to compare hands
			if (current_combo> best_combo)
			{
				best_combo = current_combo;
				winning_opponent = i + 1;
				user_player_won = false;
			}
			else if (current_combo == best_combo)
			{
				if (opponent_players[i].GetHandComboHighCard() > user_player->GetHandComboHighCard())
				{
					winning_opponent = i + 1;
					user_player_won = false;
				}
			}
		}
	}

	std::cout << std::endl;
	if (user_player_won)
	{
		++num_games_won;
		std::cout << "You";
	}
	else
	{
		std::cout << "Opponent " << winning_opponent;
	}

	std::cout << " won with a ";
	PrintHandCombo(best_combo);
	std::cout << std::endl;
}

void Game::PrintHand(const CardPile& hand) const
{
	for (int i = 0; i < max_hand_size; ++i)
	{
		std::cout << (i + 1) << ")" << hand.GetCard(i) << " ";
	}
}

void Game::PrintHandCombo(const HandCombo hand_combo) const
{
	switch (hand_combo)
	{
		case HandCombo::HighCard:
		{
			std::cout << "High Card" << std::endl;
		} break;
		case HandCombo::Pair:
		{
			std::cout << "Pair" << std::endl;
		} break;
		case HandCombo::TwoPair:
		{
			std::cout << "Two Pair" << std::endl;
		} break;
		case HandCombo::ThreeOfAKind:
		{
			std::cout << "Three of a Kind" << std::endl;
		} break;
		case HandCombo::Straight:
		{
			std::cout << "Straight" << std::endl;
		} break;
		case HandCombo::Flush:
		{
			std::cout << "Flush" << std::endl;
		} break;
		case HandCombo::FullHouse:
		{
			std::cout << "Full House" << std::endl;
		} break;
		case HandCombo::FourOfAKind:
		{
			std::cout << "Four of a Kind" << std::endl;
		} break;
		case HandCombo::StraightFlush:
		{
			std::cout << "Straight Flush" << std::endl;
		} break;
	}
}

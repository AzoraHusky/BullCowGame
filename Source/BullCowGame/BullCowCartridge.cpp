// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();

	Isograms = GetValidWords(Words);

	InitGame(); // Setting up the game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	// Game over
	if (!bGameOver)
	{
		ProcessGuess(Input); // Check player guess
	}
	else
	{
		InitGame();
	}
}

void UBullCowCartridge::InitGame()
{
	ClearScreen();
	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
	Lives = HiddenWord.Len() * 2;
	bGameOver = false;

	// Welcome Message
	PrintLine(TEXT("Welcome to the Bull Cows game!"));
	PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
	PrintLine(TEXT("You have %i lives"), Lives);
	PrintLine(TEXT("Type your guess and\npress enter to continue..."));

	//PrintLine(TEXT("Hidden word is: %s"), *HiddenWord); // Debug line

	//const TCHAR HW[] = TEXT("about");
	//PrintLine(TEXT("Character one of the hidden word is: %c"), HiddenWord[0]); // Print out the first character 'h'
	//PrintLine(TEXT("The fourth charcter of HW is: %c"), HW[3]);	
}

void UBullCowCartridge::EndGame(const FString& Message)
{
	bGameOver = true;
	ClearScreen();
	PrintLine(TEXT("%s\nThe hidden word was: %s"), *Message, *HiddenWord);
	PrintLine(TEXT("\nPlease press enter to to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
	if (Guess == HiddenWord)
	{
		EndGame("You have won!");
		return;
	}

	if (Guess.Len() != HiddenWord.Len()) // Check if the player guess is not equal to HiddenWord
	{
		PrintLine(TEXT("Wrong amount of letters, try again!"));
		PrintLine(TEXT("The word is %i letters long\nYou have %i lives remaining"), HiddenWord.Len(), Lives);
		return;
	}

	// Check if isogram
	if (!IsIsogram(Guess))
	{
		PrintLine(TEXT("There are no repeating letters, try again!"));
		PrintLine(TEXT("The word is %i letters long\nYou have %i lives remaining"), HiddenWord.Len(), Lives);
		return;
	}

	--Lives;

	if (Lives <= 0)
	{
		EndGame("You have ran out of lives!");
		return;
	}

	// Show Bulls and Cows
	FBullCowCount Count = GetBullCows(Guess);

	PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);

	// Show guess is wrong
	PrintLine(TEXT("Wrong guess, try again!"));
	PrintLine(TEXT("You have %i lives now"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString & Guess) const
{
	for (int32 Index = 0; Index < Guess.Len(); Index++) // First loop to compare the variable with the another
	{
		for (int32 Comparison = Index + 1; Comparison < Guess.Len(); Comparison++) // Second loop to compare the variable with the first loop
		{
			if (Guess[Index] == Guess[Comparison])
			{
				return false;
			}
		}
	}

	return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> & WordList) const
{
	TArray<FString> ValidWords;

	for (FString Word : WordList) //Ranged for loop
	{
		if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) //Check if the word is between 4 and 8 characters long and is an isogram
		{
			ValidWords.Emplace(Word); //Use Emplace rather than Add
		}
	}

	return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
	BullCount = 0;
	CowCount = 0;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			BullCount++;
			continue;
		}

		for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
		{
			if (HiddenWord[HiddenWordIndex] == Guess[GuessIndex])
			{
				CowCount++;
				break;
			}
		}
	}
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
		{
			if (HiddenWord[HiddenWordIndex] == Guess[GuessIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}

	return Count;
}
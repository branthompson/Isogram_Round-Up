
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame(); // sets up the game
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // makes a ref of user input
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // checking player guess
    {   
        ProcessGuess(PlayerInput);      
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    // welcome message
    PrintLine(TEXT("Howdy partner! Hope your ready!"));
    PrintLine(TEXT("Here's a warm up! Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives!"), Lives);
    PrintLine(TEXT("Type in your guess and \nPress enter to continue..."));
    // PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); // debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
        {
            PrintLine(TEXT("That's right!"));
            EndGame();
            return;
        }

    if (Guess.Len() != HiddenWord.Len())
        {   

            PrintLine(TEXT("The Hidden word is %i characters long. Guess again!"), HiddenWord.Len());
            return; 
        }
    
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("There are no repeating characters, guess again!"));
        return;
    }

    --Lives;
    PrintLine(TEXT("Wrong! You lost a life!"));
    PrintLine(TEXT("You have %i lives remaining"), Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();  
        return;  
    }

    // show player bulls and cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const 
{
    for (int32 Index = 0; Index < Word.Len(); Index++) // comparing each letter in player guess
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            } 
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords; 

    for (FString Word : Words)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word); // places valid words into new array
        } 
    } 
    return ValidWords;
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

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}

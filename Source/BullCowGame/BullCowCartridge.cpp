// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms= GetValidWords(Words);
    InitGame();    
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }

    //If game is over then do ClearScreen() and InitGame()
    
}

void UBullCowCartridge::InitGame()
{
    PrintLine(TEXT("Heya\nWelcome to the Bull and Cow game\n"));
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    Lives = HiddenWord.Len()*2;
    bGameOver=false;
    //PrintLine(TEXT("The hidden word is: %s"),*HiddenWord);
    PrintLine(TEXT("Guess the %i letter word."),HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."),Lives);
    PrintLine(TEXT("Type in your guess and \nPress Enter to continue......"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver=true;
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess==HiddenWord)
    {
        PrintLine(TEXT("You have won"));
        PrintLine(TEXT("Press enter to play again!!"));
        EndGame();
        return; 
    }
    else
    {
        --Lives;
        PrintLine("Lost a life!");
        
        if(Lives>0)
        {
            if(Guess.Len()== HiddenWord.Len())
            {
                if(IsIsogram(Guess))
                {
                    FBullCowCount Count= GetBullCows(Guess);
                    PrintLine(TEXT("You have %i Bulls and %i Cows"),Count.Bullls,Count.Cows);
                    PrintLine(TEXT("Sorry, try guessing again!\n%i lives remaining"),Lives);
                }
                else
                {
                    PrintLine(TEXT("The hidden word is an Isogram\nNo repeating letters"));
                    PrintLine(TEXT("Try guessing again!\n%i lives remaining."),Lives);
                }
            }
            else
            {
                PrintLine(TEXT("The word is %i letters long."),HiddenWord.Len());
                PrintLine(TEXT("Try guessing again!\n%i lives remaining."),Lives);
            }
        }
        else
        {
            ClearScreen();
            PrintLine(TEXT("You have no lives left"));
            PrintLine(TEXT("The Hidden word was %s."),*HiddenWord);
            PrintLine(TEXT("Press Enter to play again.."));
            EndGame();
            return;
        }
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 Index=0;Index<Word.Len();Index++)
    {
        for(int32 Comparision=Index+1;Comparision<Word.Len();Comparision++)
        {
            if(Word[Index]==Word[Comparision])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for(FString Word : WordList)
    {
        if(Word.Len()>3 && Word.Len()<8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess)
{
    FBullCowCount Count;
    for(int32 GuessIndex =0; GuessIndex <Guess.Len();GuessIndex++)
    {
        if(Guess[GuessIndex]==HiddenWord[GuessIndex])
        {
            Count.Bullls++;
            continue;
        }
        for(int32 HiddenIndex=0;HiddenIndex<HiddenWord.Len();HiddenIndex++)
        {
            if(Guess[GuessIndex]==HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
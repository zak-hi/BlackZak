#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <random>
#include <cstring>

using namespace std;
// zz note to self, worry about the Ace logic later, maybe the last thing i do today

struct Card
{
    string value;
    int numberValue;
    string suite;

    Card(string aValue, int aNumValue, string aSuite)
    {
        value = aValue;
        numberValue = aNumValue;
        suite = aSuite;
    } // zz this is my constructor for my card class

    // zz could i map each jack, queen, king to a value?
};

struct Deck //zz i think i should make this a class, that way when i deal i can pop one off? 
{
    int numCards = 52;
    // zz maybe track how many have gone from each deck? could implement later.
    vector<Card *> myCards;

    Deck(int aNumCards, vector<Card*> aMyCards)
    {
        numCards = aNumCards;
        myCards = aMyCards;
    } //zz unneccessary overloaded constructor.

    void AddCardToDeck(Card* aCard)
    {
        myCards.push_back( aCard );
    }
    Deck()
    {
        cout<<"making deck\n";
        int theThirteenCount = 1;
        vector<string> theSuites = {"Spades", "Clubs", "Hearts", "Diamonds"};
        string theCardTitle;
        int theValueOfCard; 

        for (const auto theCurrentSuite : theSuites)
        {
            cout<<"Current suite" << theCurrentSuite << "\n";
            while (theThirteenCount <= 13)
            {
                // create unique cards for jack queen king.
                if (theThirteenCount == 11)
                {
                    theCardTitle = "Jack";
                    //zz also need to make sure the value is added as 10 and not 11 12 etc
                    theValueOfCard = 10;
                }
                else if (theThirteenCount == 12)
                {
                    theCardTitle = "Queen";
                    theValueOfCard = 10;
                }
                else if (theThirteenCount == 13)
                {
                    theCardTitle = "King";
                    theValueOfCard = 10;
                }
                else
                {
                    theCardTitle = to_string(theThirteenCount);
                    theValueOfCard = theThirteenCount;
                }
                auto theCard = new Card(theCardTitle, theValueOfCard, theCurrentSuite);
                //cout<< "Adding card: " << theCardTitle << theValueOfCard << theCurrentSuite << "\n";
                theThirteenCount += 1;
                AddCardToDeck(theCard);
            }
            theThirteenCount = 1; // reset it for each suite
        }
    }

    Card* getTopCard()
    {
        Card* theReturnCard = myCards.back(); //zz get the last card in the vector and then delete it.
        myCards.pop_back();
        return theReturnCard;
    }
};


void Shuffle( Deck * aDeck )
{
    auto rng = std::default_random_engine {}; //zz refine the alg always in same order.
    shuffle(std::begin(aDeck->myCards), std::end(aDeck->myCards), rng);
    shuffle(std::begin(aDeck->myCards), std::end(aDeck->myCards), rng);
    cout<< "Deck shuffled";
}


void PrintTable( vector< vector< Card* > > aTable, int aPlayers )
{
    int counter = 0;
    for( auto& hand : aTable )
    {
        if( counter == aPlayers )
        {
            cout<< "Dealer: \n";
        }
        else
        {
            counter+=1;
            cout<< "Player " << counter << ":\n";
        }
        for( auto& card : hand )
        {
            cout<< card->value << " " << card->suite << " \n";
        }
    }
}

void PlayTurn( vector< vector < Card* > > aTable, int aNumOfPlayers, Deck* aDeck ) //zz i feel like a class would be better so i dont have to pass the deck around? or maybe return the scores? 
{
    string theChoice;
    int thePlayerTurn = 0;
    bool mFirstAsk = true; //this is a flag for when the user is asked for the first time if they want to hit or stick.
    while( thePlayerTurn <= aNumOfPlayers ) 
    {
        if( thePlayerTurn == aNumOfPlayers )
        {
            cout<< "DEALERS HAND: " << "\n";
        }
        else
        {
            cout<< "PLAYER " << thePlayerTurn +1 << " HAND: \n";
        }
        auto& theActiveHand = aTable[thePlayerTurn];
        int theActiveHandTotal = 0; //reset for each player make this a function? 
        for ( auto& theCard : theActiveHand ) 
        {
            theActiveHandTotal += theCard->numberValue;
        }
        cout<< "Trying to ask if user wants to hit or stick \n";
        while ( ( theActiveHandTotal <= 21 && theChoice == "h" ) || mFirstAsk )
        {
            mFirstAsk = false;
            cout << "Would you like to hit or stick? (h/s) \n";
            getline(cin, theChoice);
            if ( theChoice == "h" )
            {
                auto theDrawnCard = aDeck->getTopCard();
                cout << theDrawnCard->numberValue << " " << theDrawnCard->value << " "<< theDrawnCard->suite << "\n";
                theActiveHand.push_back( theDrawnCard );
                theActiveHandTotal += theDrawnCard->numberValue; //zz this is where Ace logic will come in.
            }
            cout << "Showing: " << theActiveHandTotal << "\n";
        }
        mFirstAsk = true;
        thePlayerTurn+=1;
        PrintTable( aTable, aNumOfPlayers ); 
 
    }   

}


int main()
{
    vector<Card *> mPlayerHand;
    vector<Card *> mDealerHand;
    vector<vector <Card *> > mTable;
    string numPlayers;
    cout << "Welcome to BlackZak ( developed by Zak )\n";
    cout << "Before we begin, please select how many players are playing (1-4): ";
    getline(cin, numPlayers);
    cout << numPlayers << "\n";
    int theNumOfPlayers = stoi(numPlayers);
    cout << theNumOfPlayers << "\n";
    // getline( cin, numOfDecks);
    auto theDeck = new Deck(); 
    Shuffle( theDeck );

    int theCurrentValueDealer = 0;
    auto theCardHand1 = theDeck->getTopCard();
    auto theCardHand2 = theDeck->getTopCard();
    mPlayerHand.push_back( theCardHand1 );
    mPlayerHand.push_back( theCardHand2 );

    mTable.push_back( mPlayerHand );

    cout << theCardHand1->numberValue << " "<< theCardHand1->value << " "<< theCardHand1->suite << "\n";

    int theFirstHandTotalValue = theCardHand1->numberValue + theCardHand2->numberValue;
    cout<<theFirstHandTotalValue << "\n";

    auto theDealerCardHand1 = theDeck->getTopCard();
    auto theDealerCardHand2 = theDeck->getTopCard();
    mDealerHand.push_back( theDealerCardHand1 );
    mDealerHand.push_back( theDealerCardHand2 );

    int theDealerTotalValue = theDealerCardHand1->numberValue + theDealerCardHand2->numberValue;
    cout<< theDealerTotalValue << "\n";
    theDeck->numCards -= 4;
    cout << theDeck->numCards << "\n";

    mTable.push_back( mDealerHand );

    PlayTurn( mTable, theNumOfPlayers, theDeck );
    PrintTable( mTable, theNumOfPlayers ); //so the problem with the way i set it up is that it does not actually modify the deck in memory... it is out of scope and forgets.
    //cause i need logic for how to make the hands happen and the deck updates... 


    return 0;
}





//zz how do i "play" 
//try making it play for a 1 v 1. 
//i define a round being when all players complete their turn
//i define a turn as someone being dealt two cards and deciding their action.


//zz TODO: make it a round/ turn based game, draw cards in the right order, add dealer logic and allow the "table" to see what the dealer is showing.


#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <map>
#include <set>
#include <signal.h>
#include <stdio.h>

#define SPACE ' '

using namespace std;

typedef map <char, char> KeyMap;

class MonoalphabeticCipher
{
public:
    MonoalphabeticCipher() { loadWordData(); }
    static string encrypt(string message, string key);
    static string decrypt(string message, string key);
    void decrypt(string message); // brute force
private:
    void loadWordData();
    bool isCommonWord(string randomWord);
    int  relevancePoint(string message);

    set<string> wordDatabase;
};

class UptimeCalculator
{
    clock_t start;
public:
    UptimeCalculator()
    {
        start = clock();
    }

    double getElpsed()
    {
        return (double)(clock() - start)/CLOCKS_PER_SEC;
    }
};

UptimeCalculator utime;
unsigned long long int attempt = 0;

void signal_handler(int signal)
{
    (void)signal; // skip unused warning

    cout << "Received TERM signal" << endl;
    cout << "Elpsed " << utime.getElpsed() << " seconds" << endl;
    cout << "Total attempt: " << attempt << endl;

    exit(1);
}

int main()
{
    signal(SIGINT, signal_handler);

    const string plainText = // Quote by Steve McConnell
            "GOOD CODE IS ITS OWN BEST DOCUMENTATION";

    string key = "ABCDEFGHIJKLMNOPQRSTZYXWVU"; // using an easy key
    //string key = "ZHCSFTMOUXLIYAEWJDQPBRGNVK"; // hard key, requires several years to break

    cout << "plainText" << endl
         << plainText << endl << endl;

    string cipherText = MonoalphabeticCipher::encrypt(plainText, key);
    cout << "cipherText" << endl
         << cipherText << endl << endl;

    string recoveredText = MonoalphabeticCipher::decrypt(cipherText, key);
    cout << "recoveredText using known key" << endl
         << recoveredText << endl << endl;

    MonoalphabeticCipher mCipher;
    mCipher.decrypt(cipherText);

    cout << "Elpsed " << utime.getElpsed() << " seconds" << endl;

    return 0;
}

string MonoalphabeticCipher::encrypt(string message, string key)
{
    string AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string cipherText;
    KeyMap keyMap;

    for(unsigned int i=0; i<key.size(); i++)
    {
        keyMap.insert(make_pair(AZ[i], key[i]));
    }

    for(unsigned int i=0; i<message.size(); i++)
    {
        char orgChar = message.at(i);
        char encChar;

        if(orgChar == SPACE)
            encChar = SPACE;
        else
            encChar = keyMap[message.at(i)];

        cipherText.push_back(encChar);
    }

    return cipherText;
}

string MonoalphabeticCipher::decrypt(string message, string key)
{
    string AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string plainText;
    KeyMap keyMap;

    for(unsigned int i=0; i<key.size(); i++)
    {
        keyMap.insert(make_pair(key[i], AZ[i]));
    }

    for(unsigned int i=0; i<message.size(); i++)
    {
        char encChar = message.at(i);
        char plainChar;

        if(encChar == SPACE)
            plainChar = SPACE;
        else
            plainChar = keyMap[message.at(i)];

        plainText.push_back(plainChar);
    }

    return plainText;
}

void MonoalphabeticCipher::decrypt(string message)
{
    string key("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    // map of point < message, key >
    map<int, pair<string, string> > relevancePointMap;

    cout << "Brute Force" << endl;
    do
    {
        attempt++;

        string decryptedText = decrypt(message, key);

        int point = relevancePoint(decryptedText);
        cout << setw(3) << point << " " << decryptedText.substr(0, 20) << "..." << endl;

        if(point > 5) // at least 5 meaningful word
            relevancePointMap.insert(make_pair(point, make_pair(decryptedText, key)));

        if(attempt == 500000) // or point > 5)
            break;
    }
    while(next_permutation(key.begin(), key.end()));

    cout << endl << "Point Key" << setw(23) << "" << " Message" << endl;

    for(map<int, pair<string, string> >::iterator it=relevancePointMap.begin(); it!=relevancePointMap.end(); it++)
    {
        cout << setw(5) << it->first << " " << setw(26) << it->second.second << " "
             << it->second.first.substr(0, 20) << "..." << endl;
    }

    cout << endl << "Total attempt " << attempt << endl;
}

void MonoalphabeticCipher::loadWordData()
{
    ifstream wordFile;
    wordFile.open("10000-common-words.txt");

    if(!wordFile.is_open())
    {
        throw runtime_error("Failed to load 10000-common-words.txt");
    }

    int wc = 0;
    string word;
    while(getline(wordFile, word))
    {
        wordDatabase.insert(word);
        wc++;
    }

    wordFile.close();
}

bool MonoalphabeticCipher::isCommonWord(string randomWord)
{
    if (wordDatabase.find(randomWord) != wordDatabase.end())
        return true;
    else
        return false;
}

int MonoalphabeticCipher::relevancePoint(string message)
{
    int point = 0;
    istringstream stream(message);
    string word;
    while(stream >> word)
    {
        point += isCommonWord(word);
    }

    return point;
}

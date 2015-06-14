#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std;

const int KEYSIZE = 26;

#define len(var) ((int)var.size())
#define SPACE ' '
#define ASCII_VALUE_A 'A'

class CaesarCipher
{
private:
    int privateKey; /** stores the key for best meaningful decrypted message */
    set<string> wordDatabase; /** stores words loaded from text file */
    void loadWordData();
    bool isCommonWord(string randomWord);
    int  relevancePoint(string message);

public:
    CaesarCipher() { loadWordData(); privateKey = -1; }
    static string encrypt(string message, int key);
    static string decrypt(string message, int key);
    string decrypt(string message);
    int getPossiblePrivateKey() { return privateKey; }
};

int main()
{
    const string plainText =
            "MOST GOOD PROGRAMMERS DO PROGRAMMING NOT BECAUSE "
            "THEY EXPECT TO GET PAID OR GET ADULATION BY THE PUBLIC "
            "BUT BECAUSE IT IS FUN TO PROGRAM"; // Quote by Linus Torvalds

    cout << "Plain message: " << endl
         << plainText << endl << endl;

    /*************************************************************************/

    const int privateKey = 5;

    cout << "Private key: " << endl
         << privateKey << endl << endl;

    string cipherText = CaesarCipher::encrypt(plainText, privateKey);
    cout << "Ciphered message: " << endl
         << cipherText << endl << endl;

    string decipheredText = CaesarCipher::decrypt(cipherText, privateKey);
    cout << "Deciphered message using known key: " << endl
         << decipheredText << endl << endl;

    /*************************************************************************/

    CaesarCipher mCipher;
    string autoDecryptedText = mCipher.decrypt(cipherText);

    cout << "Deciphered message using auto key finder: " << endl
         << autoDecryptedText << endl << endl;

    cout << "Possible key used: "
         << mCipher.getPossiblePrivateKey() << endl;

    return 0;
}

/**
 * @brief Loads common words into wordDatabase container
 */
void CaesarCipher::loadWordData()
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

    /** @note Debug only **/
    //cout << "Loaded " << wc << " words" << endl;
}

/**
 * @brief Checks if a word is commonly used or not
 * according to the stored word database
 * @param randomWord Any string
 * @return True if the string is found in word database, else False
 */
bool CaesarCipher::isCommonWord(string randomWord)
{
    if (wordDatabase.find(randomWord) != wordDatabase.end())
        return true;
    else
        return false;
}

/**
 * @brief Calcualtes relevance based on how many common words are
 * in the provided message
 * @param message A string with several words
 * @return Total number of common word in the message
 */
int CaesarCipher::relevancePoint(string message)
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

/**
 * @brief Encrypts a message using a substitution key
 * @param message String to encrypt
 * @param key Integer value used for substitution cipher
 * @return Caesar ciphered text
 */
string CaesarCipher::encrypt(string message, int key)
{
    string cipherText;

    for(int i=0; i<len(message); i++)
    {
        char orgChar = message[i];
        char encChar;

        if(orgChar == SPACE)
        {
            encChar = SPACE;
        }
        else
        {
            orgChar -= ASCII_VALUE_A;
            encChar = ((orgChar + key + KEYSIZE) % KEYSIZE) + ASCII_VALUE_A;
        }

        cipherText.push_back(encChar);
    }

    return cipherText;
}

/**
 * @brief Decrypts a message using a substitution key
 * @param message String to decrypt
 * @param key Integer value used for substitution cipher
 * @return Deciphered text
 */
string CaesarCipher::decrypt(string message, int key)
{
    string plainText;

    for(int i=0; i<len(message); i++)
    {
        char decChar = message[i];
        char orgChar;

        if(decChar == SPACE)
        {
            orgChar = SPACE;
        }
        else
        {
            decChar -= ASCII_VALUE_A;
            orgChar = ((decChar - key + KEYSIZE) % KEYSIZE) + ASCII_VALUE_A;
        }

        plainText.push_back(orgChar);
    }

    return plainText;
}

/**
 * @brief Decrypts a message without any key using relevancy
 * @param message String to decrypt
 * @return
 */
string CaesarCipher::decrypt(string message)
{
    map<int, int> relevancePointMap;

    cout << "Weight Message" << endl;

    for(int key=1; key<=KEYSIZE; key++)
    {
        string decryptedText = decrypt(message, key);

        int point = relevancePoint(decryptedText);
        cout << setw(6) << point << " " << decryptedText.substr(0, 20) << "..." << endl;

        relevancePointMap.insert(make_pair(point, key));
    }

    privateKey = relevancePointMap.rbegin()->second;

    cout << endl;

    return decrypt(message, privateKey);
}


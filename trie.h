#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

template <class T>
class Trie
{
private:
    struct TrieNode
    {
        static const int ALPHABET_SIZE = 128;
        TrieNode *children[ALPHABET_SIZE];
        char keyChar;
        bool isEndOfKey;
        T *data;
        vector<string> source_data;

        TrieNode(char c) : keyChar(c), isEndOfKey(false), data(nullptr)
        {

            for (int i = 0; i < ALPHABET_SIZE; i++)
            {
                children[i] = nullptr;
            }
        }
    };

    TrieNode *root;

public:
    Trie();
    ~Trie();

    Trie &insert(const string &word, const string &website);
    void remove(std::string word);
    vector<string> search(std::string word);
    void print();

private:
    void print(const std::string &primaryKey);
    void printTrie(TrieNode *node, const std::string &currentKey);

    void deleteTrieNode(struct TrieNode *node)
    {

        if (!node)
            return;

        for (int i = 0; i < node->ALPHABET_SIZE; i++)
        {

            deleteTrieNode(node->children[i]);
        }

        delete node->data;
        delete node;
    }
};

template <class T>
Trie<T>::Trie() : root(new TrieNode('\0')) {}

template <class T>
Trie<T>::~Trie()
{
    deleteTrieNode(root);
}

template <class T>
Trie<T> &Trie<T>::insert(const string &key, const string &website)
{
    TrieNode *current = root;

    int index = 0;
    while (index < key.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[key[index]] && key[index] == current->children[key[index]]->keyChar)
            ;
        // checking the letters that are present
        else if (!current->children[key[index]])
            current->children[key[index]] = new TrieNode(key[index]);

        if (index == key.size() - 1)
        {
            if (!current->children[key[index]]->isEndOfKey)
            {
                current->children[key[index]]->isEndOfKey = true;

                if (!current->children[key[index]]->data)
                    current->children[key[index]]->data = new T(key);
            }
            bool present = false;

            for (auto &i : current->children[key[index]]->source_data)
            {
                if (i == website)
                {
                    present = true;
                    break;
                }
            }

            if(!present)
                current->children[key[index]]->source_data.push_back(website);
        }

        current = current->children[key[index]];
        index++;
    }
    return *this;
}

template <class T>
vector<string> Trie<T>::search(std::string word)
{
    TrieNode *current = root;

    int index = 0;

    vector<string> a; // empty vector

    while (index < word.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[word[index]] && word[index] == current->children[word[index]]->keyChar)
        {

            if (index == word.size() - 1 && current->children[word[index]]->isEndOfKey == true)
            {

                return current->children[word[index]]->source_data;
            }

            current = current->children[word[index]];
            index++;
        }
        else
            return a;
    }
    return a;
}

template <class T>
void Trie<T>::remove(std::string word)
{

    TrieNode *current = root;

    int index = 0;

    while (index < word.size())
    { // when the index is equal to the size, we stop the search

        if (current->children[word[index]] && word[index] == current->children[word[index]]->keyChar)
        {

            if (index == word.size() - 1 && current->children[word[index]]->isEndOfKey)
            {

                current->children[word[index]]->isEndOfKey = false;
                current->children[word[index]]->source_data.erase(current->children[word[index]]->source_data.begin(), current->children[word[index]]->source_data.end());
            }

            current = current->children[word[index]];
            index++;
        }
        else
            return;
    }

    return;
}

template <class T>
void Trie<T>::print()
{
    print("");
}

template <class T>
void Trie<T>::print(const std::string &primaryKey)
{
    if (primaryKey.empty())
    {

        printTrie(root, "");
    }
    else
    {

        TrieNode *primaryNode = root;
        for (int i = 0; i < primaryKey.length(); i++)
        {

            int index = primaryKey[i];
            if (!primaryNode->children[index])
            {
                std::cout << "{}" << std::endl; // Primary key not found, nothing to print.
                return;
            }
            primaryNode = primaryNode->children[index];
        }
    }
}

template <class T>
void Trie<T>::printTrie(TrieNode *node, const std::string &currentKey)
{
    if (!node)
    {
        return;
    }
    if (node->isEndOfKey)
    {
        std::cout << "   --- " << currentKey << " --- \n";
        std::cout << "Sources:" << std::endl;

        for (auto &i : node->source_data)
            std::cout << i << std::endl;
        std::cout << std::endl;
    }
    for (int i = 0; i < 128; ++i)
    {
        if (node->children[i])
        {
            printTrie(node->children[i], currentKey + static_cast<char>(i));
        }
    }
}
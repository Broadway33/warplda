#include "Vocab.h"
#include "Utils.h"

#include <fstream>
#include <sstream>

Vocab::Vocab()
{
}

Vocab::~Vocab()
{
}

void Vocab::clear()
{
    dict.clear();
    words.clear();
}

bool Vocab::load(std::string fname)
{
    clear();
    bool success = ForEachLinesInFile(fname, [&](std::istringstream sin)
    {
        int id;
        std::string word;
        sin >> id >> word;
        dict[word] = id;
        words.push_back(word);
    });
    return success;
}

bool Vocab::store(std::string fname)
{
    std::ofstream fou(fname);
    if (!fou) return false;
    std::string line;
    for (unsigned i = 0; i < words.size(); i++)
    {
        fou << i << '\t' << words[i] << std::endl;
    }
    fou.close();
    return true;
}

int Vocab::addWord(std::string w)
{
    auto it = dict.find(w);
    if (it == dict.end())
    {
        dict[w] = words.size();
        words.push_back(w);
        return dict[w];
    }else
        return it->second;
}

int Vocab::getIdByWord(std::string w) const
{
    auto it = dict.find(w);
    if (it == dict.end())
        return -1;
    else
        return it->second;
}

std::string Vocab::getWordById(int id) const
{
    if (id < 0 || id >= (int)words.size())
        return "";
    else
        return words[id];
}

int Vocab::nWords() const
{
    return words.size();
}
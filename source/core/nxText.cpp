#include "core/nxText.hpp"
#include <sstream>

using namespace nexus;

std::string core::TextSubtext(const std::string& text, int position, int length)
{
    std::string result;

    int textLength = text.length();

    if (position >= textLength)
    {
        position = textLength - 1;
        length = 0;
    }

    if (length >= textLength) length = textLength;

    result = text.substr(position, length);

    return result;
}

void core::TextReplace(std::string& text, const std::string& replace, const std::string& by)
{
    size_t pos = 0;

    while ((pos = text.find(replace, pos)) != std::string::npos)
    {
        text.replace(pos, replace.length(), by);
        pos += by.length();
    }
}

std::string core::TextInsert(const std::string& text, const std::string& insert, int position)
{
    if (position < 0 || position > static_cast<int>(text.length()))
    {
        throw core::NexusException("core::TextInsert", "Position out of range");
    }

    return text.substr(0, position) + insert + text.substr(position);
}

std::string core::TextJoin(const std::vector<std::string>& textList, const std::string& delimiter)
{
    std::string result;
    for (size_t i = 0; i < textList.size(); ++i)
    {
        result += textList[i];
        if (i < textList.size() - 1)
        {
            result += delimiter;
        }
    }
    return result;
}

std::vector<std::string> core::TextSplit(const std::string& text, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string item;

    while (std::getline(ss, item, delimiter))
    {
        result.push_back(item);
    }

    return result;
}

void core::TextAppend(std::string& text, const std::string& append, int* position)
{
    if (*position < 0 || *position > static_cast<int>(text.length()))
    {
        throw std::out_of_range("core::TextAppend - Position out of range");
    }

    text.insert(*position, append);
    *position += append.length();
}

int core::TextFindIndex(const std::string& text, const std::string& find)
{
    size_t pos = text.find(find);
    return (pos != std::string::npos) ? static_cast<int>(pos) : -1;
}

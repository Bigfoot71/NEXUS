/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

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

#ifndef NEXUS_CORE_TEXT_HPP
#define NEXUS_CORE_TEXT_HPP

#include "../platform/nxPlatform.hpp"
#include "./nxException.hpp"

#include <vector>
#include <string>

namespace nexus { namespace core {

    /**
     * @brief Formats a string using printf-style syntax.
     *
     * @param destination The string where the formatted result will be stored.
     * @param format The format string.
     * @param args The arguments to be formatted.
     */
    template<typename... Args>
    NEXUS_API void TextFormatTo(std::string& destination, const std::string& format, Args... args)
    {
        int fmtSize = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

        if (fmtSize <= 0)
        {
            throw core::NexusException("core::TextFormat(const string&, Args...)", "Error during formatting.");
        }

        destination.resize(fmtSize - 1);
        std::snprintf(&destination[0], fmtSize, format.c_str(), args...);
    }

    /**
     * @brief Formats a string using printf-style syntax and returns the result.
     *
     * @param format The format string.
     * @param args The arguments to be formatted.
     * @return The formatted string.
     */
    template<typename... Args>
    NEXUS_API std::string TextFormat(const std::string& format, Args... args)
    {
        int fmtSize = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;

        if (fmtSize <= 0)
        {
            throw core::NexusException("core::TextFormat(const string&, Args...)", "Error during formatting.");
        }

        std::string result(fmtSize - 1, '\0');
        std::snprintf(&result[0], fmtSize, format.c_str(), args...);

        return result;
    }

    /**
     * @brief Extracts a subtext from the given text.
     *
     * @param text The input text.
     * @param position The starting position of the subtext.
     * @param length The length of the subtext.
     * @return The extracted subtext.
     */
    NEXUS_API std::string TextSubtext(const std::string& text, int position, int length);

    /**
     * @brief Replaces occurrences of a substring with another string.
     *
     * @param text The input text.
     * @param replace The substring to be replaced.
     * @param by The string to replace the substring with.
     */
    NEXUS_API void TextReplace(std::string& text, const std::string& replace, const std::string& by);

    /**
     * @brief Inserts a string into another string at the specified position.
     *
     * @param text The original text.
     * @param insert The string to be inserted.
     * @param position The position at which to insert the string.
     * @return The modified string.
     */
    NEXUS_API std::string TextInsert(const std::string& text, const std::string& insert, int position);

    /**
     * @brief Joins a list of strings into a single string using a delimiter.
     *
     * @param textList The list of strings to be joined.
     * @param delimiter The delimiter to be used between strings.
     * @return The joined string.
     */
    NEXUS_API std::string TextJoin(const std::vector<std::string>& textList, const std::string& delimiter);

    /**
     * @brief Splits a string into a vector of substrings using a delimiter.
     *
     * @param text The input text.
     * @param delimiter The character used as a delimiter.
     * @return A vector of substrings.
     */
    NEXUS_API std::vector<std::string> TextSplit(const std::string& text, char delimiter);

    /**
     * @brief Appends a string to another string at the specified position.
     *
     * @param text The original text.
     * @param append The string to be appended.
     * @param position The position at which to append the string.
     */
    NEXUS_API void TextAppend(std::string& text, const std::string& append, int* position);

    /**
     * @brief Finds the index of the first occurrence of a substring in the given text.
     *
     * @param text The input text.
     * @param find The substring to find.
     * @return The index of the first occurrence of the substring, or -1 if not found.
     */
    NEXUS_API int TextFindIndex(const std::string& text, const std::string& find);

}}

#endif

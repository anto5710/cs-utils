/**
 * @file string_util.h
 * @author Dohyun Ahn (dohyun.ahn@tufts.com)
 * @brief String utils
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <iostream>
#include <regex>
#include <vector>

/**
 * @brief determines if given text starts with given prefix
 *        by matching it at the beginning
 * @param text the tested string
 * @param regex the asserted prefix
 * @return true if the string begins with prefix
 * @return false otherwise
 */
inline bool starts_with(std::string text, std::string regex_prefix)
{
    std::string prefixed = "^" + regex_prefix + "(?:.|\\s)*";
    std::smatch matches;
    return std::regex_search(text, matches, std::regex(prefixed));
}

/**
 * @brief determines if given text ends with given suffix
 * by matching it at the end
 * @param text the tested string
 * @param regex the asserted suffix
 * @return true if the string ends with suffix
 * @return false otherwise
 */
inline bool ends_with(std::string text, std::string regex_suffix)
{
    std::string suffixed = "^" + regex_suffix + "(?:.|\\s)*";
    std::smatch matches;

    return std::regex_search(text, matches, std::regex(suffixed));
}

/**
 * @brief Determines if the latter is substring of the former text
 *
 * @param text the asserted superstring
 * @param regex_sub the asserted substring
 * @return true if superstring contains a substring with given regex pattern
 * @return false otherwise
 */
inline bool search_pattern(std::string text, std::string regex_sub)
{
    std::smatch matches;
    return std::regex_search(text, matches, std::regex(regex_sub));
}

/**
 * @brief Find next token, consuming all characters up to that token,
 * readying to find the next, next token.
 * Used to iterate a to-be-splited string.
 *
 * @param text      currerntly unseen part of string
 * @param token     reference to be updated as next token
 * @param delimiter delimiter spliting the string
 * @return true     if more delimiter and (tokens) remains to be divided.
 *         false    if no more delimiter, or if delimiter is empty
 */
inline bool next_token(std::string &text, std::string &token,
                       std::string delimiter)
{
    size_t di = text.find(delimiter);

    bool reachedEnd = (delimiter.empty() or di == std::string::npos);

    token = reachedEnd ? text : text.substr(0, di);

    if (not reachedEnd) {
        text.erase(0, di + delimiter.length());
    }

    return (not reachedEnd);
}

/**
 * @brief Splits given text into an ArrayList of tokens,
 * which are substrings separated by delimiters.
 * Will collect tokenes in order they appear in the original text.
 *
 * @param text A source text
 * @param delimiter A delimiter with length > 0.
 * @return std::vector<std::string> A list of tokens in order
 */
inline std::vector<std::string> split(std::string text, std::string delimiter)
{
    std::vector<std::string> str_list;

    std::string token;

    while (next_token(text, token, delimiter)) {
        str_list.push_back(token);
    }

    str_list.push_back(token);
    return str_list;
}


#endif
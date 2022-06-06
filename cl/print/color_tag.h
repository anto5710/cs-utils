/**
 * @file      color_tag.h
 * @author    Dohyun Ahn (dohyun.ahn@tufts.com)
 * @brief     Utilset searching/substituting color tags,
 *            which are HTML-like mark-up tags that correspond to
 *            one or more ANSI escape sqeuence(s).
 *
 * @version   0.1
 * @date      2022-02-17
 *
 * @copyright Copyright (c) 2022
 */

#ifndef COLOR_DD_H
#define COLOR_DD_H

#include "color_map.h"
#include "color_stream.h"
#include "string_util.h"
#include <iostream>

const std::string COLOR_TAG_PATTERN = "<[^<>]+>";

/**
 * @brief  Finds first preset ANSISequence that has a
 *         pattern contained in given tag content,
 *         saving result sequence to reference, if any.
 *
 * @param  tag_content string tag content inside < .. >
 * @param  result_seq  the matching preset sequence, if any
 * @return true if a match was found in preset
 *         false otherwise
 */
bool match_tag(std::string tag_content, ANSISequence &result_seq)
{
    for (const ANSISequence seq : preset_sequences) {

        if ((seq.pattern.length() > 0) and
            search_pattern(tag_content, seq.pattern)) {

            result_seq = seq;
            return true;
        }
    }

    return false;
}

/// Extract all color tags < .. > from given string
inline std::string untag(std::string tagged_text)
{
    return std::regex_replace(tagged_text, std::regex(COLOR_TAG_PATTERN), "");
}

/**
 * @brief Concatenates all color tags found in given tag
 *        into one, then appends it to given ColorStream.
 *
 * @param cstream the ColorStream being streamed
 * @param tag     A color tag (including brackets)
 */
inline void push_tag(ColorStream &cstream, std::string tag)
{
    // Content inside <tag> brackets
    std::string tag_inner = tag.substr(1, tag.length() - 2);
    bool        reverse   = starts_with(tag_inner, "\\s*/");

    ANSISequence seq;

    bool count = false;

    for (std::string token : split(tag_inner, ";")) {
        ANSISequence token_seq;

        if (match_tag(tag_inner, token_seq)) {
            seq += token_seq;
            count = true;

        } else if (reverse) {

            if (token.empty()) {
                cstream << R_RESET;
                count = true;

            } else {
                cstream << u_undo;
            }
        }
    }

    if (count) {
        cstream << seq;
    }
}

inline std::string apply_tags(std::string text)
{
    std::regex  all_tags(COLOR_TAG_PATTERN);
    std::smatch matches;

    // stringstream to collect (styled) resulted
    std::stringstream ss;
    ColorStream       cstream(ss);

    // Iterate through each tag
    while (std::regex_search(text, matches, all_tags)) {

        cstream << matches.prefix().str();

        push_tag(cstream, matches.str());

        text = matches.suffix().str();
    }

    return ss.str();
}

#endif

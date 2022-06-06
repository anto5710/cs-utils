/**
 * @file color_map.h
 * @author Dohyun Ahn (dohyun.ahn@tufts.edu)
 * @brief A enum-like container of ANSISequence constants,
 *  each being a labled ANSI code string
 *  that can be easily appended
 *  to output stream.
 *
 * Each ANSI enum is created with ANSI code,
 *  a style name, and a regular expression to detect the code.
 *
 *  e.g.) For black foreground (font) sequence:
 *
 *  C_BLACK("30", "Foreground:Black", "(c:)?bla?ck");
 *
 *  It has ANSI code 30, and should be (will to string and)
 *  printed as \e30m, resulting Black font color.
 *  Its tag is in pattern (c:black) or (black), or shorter.
 *
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef COLOR_MAP_H
#define COLOR_MAP_H
#define _enum const ANSISequence

#include "string_util.h"
#include <iostream>
#include <list>
#include <regex>

/**
 * @brief A struct representing an ANSI Escape Sequence.
 * Used to manipulate color or typeset in which all succeeding string is printed
 */
struct ANSISequence {
    std::string code; ///< ANSI Escape Code: \\e[ .. m
    std::string name; ///< given name
    std::string pattern;

    ANSISequence(std::string escapeCode, std::string sequenceName,
                 std::string regexPattern)
    {
        code    = escapeCode;
        name    = sequenceName;
        pattern = regexPattern;
    }

    ANSISequence(std::string escapeCode, std::string sequenceName) :
        ANSISequence(escapeCode, sequenceName, "")
    {
    }

    ANSISequence() : ANSISequence("28;28", "Reset:Null") {}

    std::string str() const { return ("\e[" + code + "m"); }

    /**
     * @brief Overloads binary '+' operator for ANSISequence
     *  such that operation of A + B
     *  will return a new C with code = A.code + ";" + B.code
     *  and with name = A.name + ", " + B.name
     *  and with regex = A.regex + "\s*;\s*" B.regex
     *
     * @param other Reference to RHS
     * @return ANSISequence A concatenation of two sequences
     */
    ANSISequence operator+(const ANSISequence &other) const
    {
        ANSISequence copy(*this);
        addTo(copy, other);

        return copy;
    }

    ANSISequence &operator+=(const ANSISequence &other)
    {
        addTo((*this), other);
        return (*this);
    }

    /**
     * @brief Appends adder's code, name, and pattern to target
     * That is, LHS += RHS.
     * LHS becomes a R_RESET copy if either sequence has an empty (resetting)
     * code.
     *
     * @param target LHS in the equation, to be modified
     * @param adder RHS in the equation, the modifier.
     */
    void addTo(ANSISequence &target, const ANSISequence &adder) const
    {
        if (target.pattern.empty() or adder.pattern.empty()) {
            target = ANSISequence("", "Reset:All", "/\\*");
        }

        target.code += (";" + adder.code);
        target.name += (", " + adder.name);
        target.pattern += ("\\s*;\\s*" + adder.pattern);
    }

    /**
     * @brief Comparator. Returns if this ANSISequence has
     * exact same content (code and name) as the other.
     *
     * @param other RHS of equation
     * @return true if LHS.equals(RHS)
     * @return false otherwise
     */
    bool equals(const ANSISequence &other) const
    {
        return (code == other.code);
    }

    /// == override, set as LHS.equals(RHS)
    bool operator==(const ANSISequence &RHS) const { return equals(RHS); }
};

inline std::ostream &operator<<(std::ostream &os, ANSISequence &seq)
{
    return (os << seq.str());
}

//--------------------------------------------------------------
// Reset Sequences
//--------------------------------------------------------------
_enum R_NULL("28;28", "Reset:Null");
_enum R_RESET("", "Reset:All", "/\\*");
_enum R_BOLD("21", "Reset:Bold", "/b(old)?");
_enum R_DIM("22", "Reset:Dim", "/d(im)?");
_enum R_UNDERLINE("24", "Reset:Underline", "/u(nder|nderline)?");
_enum R_BLINK("25", "Reset:Blink", "/bl(ink)?");
_enum R_REVERSE("27", "Reset:Reverse", "/(rv|rev|reverse)");
_enum R_INVISIBLE("28", "Reset:Invisible", "/(h(ide|idden)?|invisible)");

//--------------------------------------------------------------
// Typesetting Sequences
//--------------------------------------------------------------
_enum ST_BOLD("1", "Style:Bold", "b(old)?");
_enum ST_ITALIC("3", "Style:Italic", "i(t|talic)?");
_enum ST_UNDERLINE("4", "Style:Underline", "u(nder|nderline)?");
_enum ST_BLINK("5", "Style:Blink", "bl(ink)?");
_enum ST_BLACK_OUTLINE("6", "Style:Black Outline",
                       "b(lack)?[_\\s-]?outl(ine)?");
_enum ST_REVERSE("7", "Style:Reverse", "(rv|rev|reverse)");
_enum ST_INVISIBLE("8", "Style:Invisible", "(h(ide|idden)?|invisible)");
_enum ST_STRIKE("9", "Style:Strikethrough", "(-{2,}|strike(through)?)");

//--------------------------------------------------------------
// Font-Coloring Sequences
//--------------------------------------------------------------
_enum C_DEFAULT("39", "Foreground:Default", "/c");
_enum C_BLACK("30", "Foreground:Black", "(c:)?bla?ck");
_enum C_RED("31", "Foreground:Red", "(c:)?r(ed)?");
_enum C_GREEN("32", "Foreground:Green", "(c:)?g(reen)?");
_enum C_YELLOW("33", "Foreground:Yellow", "(c:)?y(ellow)?");
_enum C_BLUE("34", "Foreground:Blue", "(c:)?blue?");
_enum C_MAGENTA("35", "Foreground:Magenta", "(c:)?(ma?g(enta)?|p(ink)?)");
_enum C_CYAN("36", "Foreground:Cyan", "(c:)?c(yan)?");
_enum C_WHITE("37", "Foreground:White", "(c:)?w(hite)?");

//--------------------------------------------------------------
// Font-Coloring Sequences: Light Variants
//--------------------------------------------------------------
_enum C_DARK_GRAY("90", "Foreground:Dark Gray", "(c:)?d(ark)?[_\\s-]?g(ray)?");
_enum C_LIGHT_RED("91", "Foreground:Light Red",
                  "(c:)?l(i|ight)?[_\\s-]?r(ed)?");
_enum C_LIGHT_GREEN("92", "Foreground:Light Green",
                    "(c:)?l(i|ight)?[_\\s-]?g(reen)?");
_enum C_LIGHT_YELLOW("93", "Foreground:Light Yellow",
                     "(c:)?l(i|ight)?[_\\s-]?y(ellow)?");
_enum C_LIGHT_BLUE("94", "Foreground:Light Blue",
                   "(c:)?l(i|ight)?[_\\s-]?blue?");
_enum C_LIGHT_MAGENTA("95", "Foreground:Light Magenta",
                      "(c:)?l(i|ight)?[_\\s-]?"
                      "(ma?g(enta)?|p(ink)?)");
_enum C_LIGHT_CYAN("96", "Foreground:Light Cyan",
                   "(c:)?l(i|ight)?[_\\s-]?c(yan)?");
_enum C_LIGHT_WHITE("97", "Foreground:Light White", "(c:)?l(i|ight)?w(hite)?");

//--------------------------------------------------------------
// Background-Coloring Sequences
// Note that BG codes are 10 + (FG equivalent)
//--------------------------------------------------------------
_enum BG_DEFAULT("49", "Background:Default", "/c");
_enum BG_BLACK("40", "Background:Black", "bg?:bla?ck");
_enum BG_RED("41", "Background:Red", "bg?:r(ed)?");
_enum BG_GREEN("42", "Background:Green", "bg?:g(reen)?");
_enum BG_YELLOW("43", "Background:Yellow", "bg?:y(ellow)?");
_enum BG_BLUE("44", "Background:Blue", "bg?:blue?");
_enum BG_MAGENTA("45", "Background:Magenta", "bg?:(ma?g(enta)?|p(ink)?)");
_enum BG_CYAN("46", "Background:Cyan", "bg?:c(yan)?");
_enum BG_WHITE("47", "Background:White", "bg?:w(hite)?");

//--------------------------------------------------------------
// Background-Coloring Sequences: Light Variants
//--------------------------------------------------------------
_enum BG_DARK_GRAY("100", "Foreground:Dark Gray", "bg?:d(ark)?[_\\s-]?g(ray)?");
_enum BG_LIGHT_RED("101", "Foreground:Light Red",
                   "bg?:l(i|ight)?[_\\s-]?r(ed)?");
_enum BG_LIGHT_GREEN("102", "Foreground:Light Green",
                     "bg?:l(i|ight)?[_\\s-]?g(reen)?");
_enum BG_LIGHT_YELLOW("103", "Foreground:Light Yellow",
                      "bg?:l(i|ight)?[_\\s-]?y(ellow)?");
_enum BG_LIGHT_BLUE("104", "Foreground:Light Blue",
                    "bg?:l(i|ight)?[_\\s-]?blue?");
_enum BG_LIGHT_MAGENTA("105", "Foreground:Light Magenta",
                       "bg?:l(i|ight)?[_\\s-]?"
                       "(ma?g(enta)?|p(ink)?)");
_enum BG_LIGHT_CYAN("106", "Foreground:Light Cyan",
                    "bg?:l(i|ight)?[_\\s-]?c(yan)?");
_enum BG_LIGHT_WHITE("107", "Foreground:Light White", "bg?:l(i|ight)?w(hite)?");

extern const std::list<ANSISequence> preset_sequences = {
    R_RESET,        R_BOLD,           R_DIM,
    R_UNDERLINE,    R_BLINK,          R_REVERSE,
    R_INVISIBLE,    ST_BOLD,          ST_ITALIC,
    ST_UNDERLINE,   ST_BLINK,         ST_BLACK_OUTLINE,
    ST_REVERSE,     ST_INVISIBLE,     ST_STRIKE,
    C_DEFAULT,      C_BLACK,          C_RED,
    C_GREEN,        C_YELLOW,         C_BLUE,
    C_MAGENTA,      C_CYAN,           C_WHITE,
    C_DARK_GRAY,    C_LIGHT_RED,      C_LIGHT_GREEN,
    C_LIGHT_YELLOW, C_LIGHT_BLUE,     C_LIGHT_MAGENTA,
    C_LIGHT_CYAN,   C_LIGHT_WHITE,    BG_DEFAULT,
    BG_BLACK,       BG_RED,           BG_GREEN,
    BG_YELLOW,      BG_BLUE,          BG_MAGENTA,
    BG_CYAN,        BG_WHITE,         BG_DARK_GRAY,
    BG_LIGHT_RED,   BG_LIGHT_GREEN,   BG_LIGHT_YELLOW,
    BG_LIGHT_BLUE,  BG_LIGHT_MAGENTA, BG_LIGHT_CYAN,
    BG_LIGHT_WHITE, BG_LIGHT_WHITE};


// Note that "\e" is an single character [ESC]
const std::string ESCAPE_PATTERN = "\e\\[[^m\\[]*[m\\[]";
/// Extract all escape sequences "\e[ ... m" from given string
inline std::string unstyle(std::string styled_text)
{
    return std::regex_replace(styled_text, std::regex(ESCAPE_PATTERN), "");
}

#endif
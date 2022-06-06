/**
 * @file      color_stream.h
 * @author    Dohyun Ahn
 * @brief     An Output Stream Writer, with auxiliary functionalities
 *            such as color printing, stacking, and regioning.
 * @version   0.1
 * @date      2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef COLOR_STACK_H
#define COLOR_STACK_H

#include "color_map.h"
#include <iostream>
#include <list>
#include <sstream>
#include <type_traits>

class ColorStack
{
public:
    /**
     * @brief 'Starts' to apply given ANSI sequence to all string that follows
     *        Note that an ANSISequence can be concatenated (A + B),
     *        and such concatenation is treated as single element in this stack.
     * @param seq
     */
    inline void start(const ANSISequence seq)
    {
        if (seq == R_RESET) {
            m_stack.clear();
        } else
            m_stack.push_front(seq.str());
    }

    /// 'undo' latest color sequence, which may be a combination of sequences
    inline void undo()
    {
        if (not m_stack.empty()) {
            m_stack.pop_back();
        }
    }

    /// Concatenates all string (sequence) entries into one
    inline std::string str()
    {
        std::stringstream sstream(R_RESET.str());

        for (std::string seq_str : m_stack) {
            sstream << seq_str;
        }
        return sstream.str();
    }

    /// Clears stack
    inline void clear() { m_stack.clear(); }

    /// Access stack size
    inline int size() { return m_stack.size(); }

private:
    std::list<std::string> m_stack; //< internal stack of sequence str()'s
};

/**
 * @brief A placeholder/method-like struct to generate
 *        mini-color container:
 *        when ColorStream << mini_color,
 *        it 'starts' with given color, prints the text, then 'undoes'
 *        enclosing text in a mini-colored region.
 */
struct mini_color {
    ANSISequence seq;  ///< sequence of this region
    std::string  text; ///< enclosed text

    mini_color(ANSISequence miniSequence, std::string miniText)
    {
        text = miniText;
        seq  = miniSequence;
    }
};

/**
 * @brief An object that delegates to given output stream and
 *        adds ANSISequence-oriented functionalities,
 *        such as << ANSISequence, << mini_color, << u_undo
 */
class ColorStream
{
public:
    std::ostream *output_p;
    ColorStack    m_cstack;

    /**
     * @brief Construct a new ColorStream object writing
     *        to given output stream
     *
     * @param os A reference to output stream
     */
    ColorStream(std::ostream &os) { output_p = (&os); }

    /**
     * @brief  Begin applying given ANSISequence 
     *         to all that follows and retunrs 
     *         this instance in chain
     * @param  seq An ANSISeuquence
     * @return ColorStream& this stream instance
     */
    ColorStream &start(const ANSISequence seq)
    {
        m_cstack.start(seq);

        (*output_p) << seq.str();
        return (*this);
    }

    /**
     * @brief  Undo just the lastest ANSISeuquence fed into this stream,
     *         by calling 'reset-all' sequence and re-enacting 
     *         all previous sequences.
     *
     * @return ColorStream& this stream instance
     */
    ColorStream &undo()
    {
        m_cstack.undo();
        (*output_p) << m_cstack.str();

        return (*this);
    }

    /// Overload of '<<' to internal output stream
    template<typename T>
    inline ColorStream &operator<<(T data)
    {
        (*output_p) << data;
        return (*this);
    }

    /// Overload of ColorStream << ANSISequence and all its subclasses
    inline ColorStream &operator<<(ANSISequence seq) { return start(seq); }

    /// Overload of << output stream manipulators, such as std::endl
    inline ColorStream &operator<<(std::ostream &(*os_op)(std::ostream &))
    {
        (*output_p) << os_op;
        return (*this);
    }

    /// Overload of << ColorStream-specific manipulators, namely U_UNDO
    inline ColorStream &operator<<(ColorStream &(*cs_op)(ColorStream &))
    {
        return cs_op(*this);
    }

    inline ColorStream &operator<<(mini_color mini)
    {
        return (start(mini.seq) << mini.text).undo();
    }

    friend ColorStream &undo(ColorStream &cstream);
};

/**
 * @brief  A ColorStream manipulator body.
 *         Calls undo() of given cstream
 *         and returns it in chain
 *
 * @param  cstream A ColorStream reference
 * @return ColorStream& the same reference chained
 */
ColorStream &undo(ColorStream &cstream)
{
    cstream.undo();
    return cstream;
}

/// u_undo: ColorStream manipulator (used in same manner as std::endl)
ColorStream &(*u_undo)(ColorStream &) = &undo;

// ColorStream &miniColor(ColorStream &cstream, ANSISequence seq, std::string text)
// {
//     return (cstream << seq << text << u_undo);
// }


#endif
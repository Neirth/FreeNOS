/*
 * Copyright (C) 2009 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TERMINAL_TERMINAL_H
#define __TERMINAL_TERMINAL_H

#include <FreeNOS/Config.h>
#include <Device.h>
#include <Macros.h>
#include <Types.h>
#include <teken.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup terminal
 * @{
 */

/**
 * @brief Print this banner per default on new Terminals.
 */
#define BANNER \
    "FreeNOS " RELEASE " [" ARCH "/" SYSTEM "] (" BUILDUSER "@" BUILDHOST ") (" COMPILER_VERSION ") " DATETIME "\r\n"

/**
 * @brief A Terminal enables user to interact with the system.
 */
class Terminal : public Device
{
  public:

    /**
     * @brief Class constructor.
     *
     * @param inputFile Path to the (device) file to use as input source.
     * @param outputFile Path to the (device) file to use as
     *        an output source.
     * @param width Width of the Terminal.
     * @param height Height of the Terminal.
     */
    Terminal(const char *inputFile, const char *outputFile,
             Size width = 80, Size height = 25);

    /**
     * @brief Class destructor.
     */
    virtual ~Terminal();

    /**
     * @brief Retrieve the width of the Terminal.
     *
     * @return Terminal width.
     */
    Size getWidth();

    /**
     * @brief Retrieve the height of the Terminal.
     *
     * @return Terminal height.
     */
    Size getHeight();

    /**
     * @brief Retrieve file descriptor of the input source.
     *
     * @return File descriptor number.
     */
    int getInput();

    /**
     * @brief Retrieve file descriptor of the output source.
     *
     * @return File descriptor number.
     */
    int getOutput();

    /**
     * @brief Retrieve a pointer to the local buffer.
     *
     * @return Pointer to the local buffer.
     */
    u16 * getBuffer();

    /**
     * Saved byte and attribute value at cursor position.
     *
     * @return Saved value.
     */
    u16 * getCursorValue();

    /**
     * Hides the cursor from the VGA screen.
     */
    void hideCursor();

    /**
     * Sets the new position of the cursor.
     *
     * @param pos New position coordinates.
     */
    void setCursor(const teken_pos_t *pos);

    /**
     * Show the VGA cursor.
     */
    void showCursor();

    /**
     * @brief Initializes the Terminal.
     *
     * @return Error result code.
     */
    virtual Error initialize();

    /**
     * Read bytes from the Terminal.
     *
     * @param buffer Output buffer.
     * @param size Number of bytes to read.
     * @param offset Unused.
     *
     * @return Number of bytes read or error code on failure.
     */
    virtual Error read(IOBuffer & buffer, Size size, Size offset);

    /**
     * Write bytes to the Terminal (vga memory).
     *
     * @param buffer Contains the bytes to write.
     * @param size Number of bytes to write.
     * @param offset Unused.
     *
     * @return Number of bytes written or error code on failure.
     */
    virtual Error write(IOBuffer & buffer, Size size, Size offset);

  private:

    /** Terminal state. */
    teken_t state;

    /** Terminal function handlers. */
    teken_funcs_t funcs;

    /** Buffer for local Terminal updates. */
    u16 *buffer;

    /** Saved cursor position. */
    teken_pos_t cursorPos;

    /** Saved value at cursor position. */
        u16 cursorValue;

    /**
     * @brief Path to the input and output files.
     */
    const char *inputFile, *outputFile;

    /**
     * @brief Width and height of the Terminal.
     */
    const Size width, height;

    /** Input and output file descriptors. */
    int input, output;
};

/**
 * Makes a sound (bell).
 *
 * @param term Terminal object pointer.
 */
void bell(Terminal *term);

/**
 * Output a new character.
 *
 * @param term Terminal object pointer.
 * @param pos Terminal position.
 * @param ch Character to output.
 * @param attr Attributes for this character.
 */
void putchar(Terminal *term, const teken_pos_t *pos,
         teken_char_t ch, const teken_attr_t *attr);

/**
 * Sets the Terminal cursor.
 *
 * @param term Terminal object pointer.
 * @param pos Position to put the cursor.
 */
void cursor(Terminal *term, const teken_pos_t *pos);

/**
 * Fills the Terminal buffer with a character.
 *
 * @param term Terminal object pointer.
 * @param rect Indicates where to fill.
 * @param ch Character to be used for filling.
 * @param attr Attributes for this character.
 */
void fill(Terminal *ctx, const teken_rect_t *rect,
      teken_char_t ch, const teken_attr_t *attr);

/**
 * Copy bytes to the terminal.
 *
 * @param ctx Terminal object pointer.
 * @param rect Source rectagular area with VGA memory buffer.
 * @param pos Position to copy the rect to.
 */
void copy(Terminal *ctx, const teken_rect_t *rect,
      const teken_pos_t *pos);

/**
 * Set terminal parameters.
 *
 * @param ctx Terminal object pointer.
 * @param key Option key identifier.
 * @param value New value for the option.
 */
void param(Terminal *ctx, int key, int value);

/**
 * @brief Unused.
 */
void respond(Terminal *ctx, const void *buf, size_t size);

/**
 * @}
 * @}
 */

#endif /* __TERMINAL_TERMINAL_H */

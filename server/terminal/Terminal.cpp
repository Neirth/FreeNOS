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

#include <Types.h>
#include <Macros.h>
#include <VGA.h>
#include <Runtime.h>
#include "Terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

u8 tekenToVGA[] =
{
    BLACK,
    RED,
    LIGHTGREEN,
    LIGHTBROWN,
    LIGHTBLUE,
    LIGHTMAGENTA,
    LIGHTCYAN,
    LIGHTGREY,
    LIGHTGREY,
};

Terminal::Terminal(const char *in, const char *out,
                   Size w, Size h)
    : Device(CharacterDeviceFile), inputFile(in), outputFile(out), width(w), height(h)
{
    m_identifier << "tty0";
    buffer = new u16[width * height];
}

Error Terminal::initialize()
{
    teken_pos_t winsz;

    // Close standard I/O
    ::close(0);
    ::close(1);

    // Attempt to open input file.
    if ((input = ::open(inputFile, O_RDONLY)) < 0)
    {
        printf("failed to open `%s': %s\r\n",
                inputFile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Then open the output file.
    if ((output = ::open(outputFile, O_RDWR)) < 0)
    {
        printf("failed to open `%s': %s\r\n",
                outputFile, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Fill in function pointers
    funcs.tf_bell    = (tf_bell_t *)    bell;
    funcs.tf_cursor  = (tf_cursor_t *)  cursor;
    funcs.tf_putchar = (tf_putchar_t *) putchar;
    funcs.tf_fill    = (tf_fill_t *)    fill;
    funcs.tf_copy    = (tf_copy_t *)    copy;
    funcs.tf_param   = (tf_param_t *)   param;
    funcs.tf_respond = (tf_respond_t *) respond;

    // Reset cursor
    memset(&cursorPos, 0, sizeof(cursorPos));

    // Initialize libteken
    teken_init(&state, &funcs, this);

    // Set appropriate terminal sizes
    winsz.tp_col = 80;
    winsz.tp_row = 25;
    teken_set_winsize(&state, &winsz);

    // Print banners
    FileSystemMessage msg;
    msg.type = ChannelMessage::Request;
    msg.size = 512;
    IOBuffer io(&msg);
    io.bufferedWrite((void *)(BANNER COPYRIGHT "\r\n"), strlen(BANNER)+strlen(COPYRIGHT)+2);
    write(io, io.getCount(), 0);

    // Done
    return ESUCCESS;
}

Terminal::~Terminal()
{
    delete buffer;
    ::close(input);
    ::close(output);
}

Size Terminal::getWidth()
{
    return width;
}

Size Terminal::getHeight()
{
    return height;
}

int Terminal::getInput()
{
    return input;
}

int Terminal::getOutput()
{
    return output;
}

u16 * Terminal::getBuffer()
{
    return buffer;
}

u16 * Terminal::getCursorValue()
{
    return &cursorValue;
}

Error Terminal::read(IOBuffer & buffer, Size size, Size offset)
{
    char tmp[255];
    int n;

    n = ::read(input, tmp, size < sizeof(tmp) ? size : sizeof(tmp));
    if (n > 0)
        buffer.write(tmp, n);

    return n;
}

Error Terminal::write(IOBuffer & buffer, Size size, Size offset)
{
    char cr = '\r', ch;

    // Initialize buffer with the current screen first
    ::lseek(output, 0, SEEK_SET);
    ::read(output, this->buffer, width * height * 2);

    // Loop all input characters. Add an additional carriage return
    // whenever a linefeed is detected.
    for (Size i = 0; i < size; i++)
    {
        if (buffer[i] == '\n')
        {
            teken_input(&state, &cr, 1);
        }
        ch = buffer[i];
        teken_input(&state, &ch, 1);
    }

    // Flush changes back to our output device
    ::lseek(output, 0, SEEK_SET);
    ::write(output, this->buffer, width * height * 2);

    // Done
    return size;
}

void Terminal::hideCursor()
{
    u16 index = cursorPos.tp_col + (cursorPos.tp_row * width);

    // Restore old attributes
    buffer[index] &= 0xff;
    buffer[index] |= (cursorValue & 0xff00);
}

void Terminal::setCursor(const teken_pos_t *pos)
{
    // Save value
    cursorValue = buffer[pos->tp_col + (pos->tp_row * width)];
    cursorPos   = *pos;
}

void Terminal::showCursor()
{
    u16 index = cursorPos.tp_col + (cursorPos.tp_row * width);

    // Refresh cursorValue first
    setCursor(&cursorPos);

    // Write cursor
    buffer[index] &= 0xff;
    buffer[index] |= VGA_ATTR(LIGHTGREY, LIGHTGREY) << 8;
}

void bell(Terminal *term)
{
    // Does nothing yet
}

void putchar(Terminal *term, const teken_pos_t *pos,
             teken_char_t ch, const teken_attr_t *attr)
{
    // Retrieve variables first
    u16 *buffer = term->getBuffer();
    Size width  = term->getWidth();

    // Make sure to don't overwrite cursor
    term->hideCursor();

    // Write the buffer
    buffer[pos->tp_col + (pos->tp_row * width)] =
        VGA_CHAR(ch, tekenToVGA[attr->ta_fgcolor], BLACK);

    // Show cursor again
    term->showCursor();
}

void cursor(Terminal *term, const teken_pos_t *pos)
{
    term->hideCursor();
    term->setCursor(pos);
    term->showCursor();
}

void fill(Terminal *term, const teken_rect_t *rect,
          teken_char_t ch, const teken_attr_t *attr)
{
    // Make sure we don't overwrite the cursor
    term->hideCursor();

    // Fill video memory; loop rows
    for (Size row = rect->tr_begin.tp_row;
                    row < rect->tr_end.tp_row; row++)
    {
        // Loop columns
        for (Size col = rect->tr_begin.tp_col;
                        col < rect->tr_end.tp_col; col++)
        {
            term->getBuffer()[col + (row * term->getWidth())] =
                VGA_CHAR(ch, tekenToVGA[attr->ta_fgcolor], BLACK);
        }
    }
    // Show cursor again
    term->showCursor();
}

void copy(Terminal *term, const teken_rect_t *rect,
          const teken_pos_t *pos)
{
    // Retrieve variables
    u16 *buffer  = term->getBuffer();
    Size width   = term->getWidth();

    // Calculate sizes
    Size numCols = rect->tr_end.tp_col - rect->tr_begin.tp_col;
    Size numRows = rect->tr_end.tp_row - rect->tr_begin.tp_row;

    // Hide cursor first
    term->hideCursor();

    // Copy video memory
    memcpy(buffer + pos->tp_col + (pos->tp_row * width),
           buffer + rect->tr_begin.tp_col + (rect->tr_begin.tp_row * width),
           numCols + (numRows * width) * sizeof(u16));

    // Show cursor again
    term->showCursor();
}

void param(Terminal *term, int key, int value)
{
    // Do nothing
}

void respond(Terminal *ctx, const void *buf, size_t size)
{
    // Do nothing
}

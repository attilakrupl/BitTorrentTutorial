/*!
 * \file   terminalmessages.h
 * \author Attila Kr�pl dr.
 * \date   30/09/2020
 */

#pragma once

namespace nTorrentUI
{
    class TerminalMessages final
    {
        public:
            TerminalMessages() = default;
            ~TerminalMessages() = default;

        public:
            static void PrintUsage();
    };
}

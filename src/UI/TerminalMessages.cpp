/*!
 * \file   terminalmessages.cpp
 * \author Attila Krüpl dr.
 * \date   30/09/2020
 */

#include "stdafx.h"

using namespace nTorrentUI;

void TerminalMessages::PrintUsage()
{
    std::cerr << R"(usage: dump_torrent torrent-file [options]
            OPTIONS:
                --items-limit <count>    set the upper limit of the number of bencode items
                                         in the torrent file.
                --depth-limit <count>    set the recursion limit in the bdecoder
                --show-padfiles          show pad files in file list
                --max-pieces <count>     set the upper limit on the number of pieces to
                                         load in the torrent.
                --max-size <size in MiB> reject files larger than this size limit
        )";
    std::exit( 1 );
}

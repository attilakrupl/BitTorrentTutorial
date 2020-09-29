/*!
 * \file   main.cpp
 * \author Attila Krüpl dr.
 * \date   29/09/2020
 */

#include "stdafx.h"

using namespace lt;
using namespace std;

static const std::vector<std::string> kMenuOptions{ "Begin to download torrent"
                                                  , "Print torrent information" };

namespace
{
    [[noreturn]] void print_usage()
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
}

void ShowMenuOptions( const std::string& aTorrentPath )
{
    cout << "You have added the following torrent file: " << endl;
    cout << aTorrentPath << endl;
    cout << R"(What would you like to do? Enter the corresponding number:
        1) Begin to download torrent
        2) Print torrent information )" << endl;
}

int AskForChoice()
{
    int lResponse;
    
    while ( ! ( cin >> lResponse ) )
    {
        std::cin.clear();
        std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        std::cout << "Invalid input, please re-enter.\n";
    }

    if ( lResponse < 1 
      || lResponse > kMenuOptions.size() )
    {
        cout << "Wrong answer, please enter a correct Menu option! " << endl;
        cin.clear();
        return AskForChoice();
    }

    return lResponse;
}

int main( int argc, char const* argv[] )
try
{
    if ( argc != 2 ) {
        cerr << "Usage: " << argv[0] << " <torrent_file_path>" << endl;
        return 1;
    }

    const std::string lTorrentToDownload = argv[1];
    
    ShowMenuOptions( lTorrentToDownload );

    const int lChoice = AskForChoice();

    nTorrentLogic::TorrentHandlingLogic lTorrentHandlingLogic( lTorrentToDownload );

    switch( lChoice )
    {
    case 1:
        lTorrentHandlingLogic.DownloadTorrent();
        break;
    case 2:
        lTorrentHandlingLogic.PrintInfo();
        break;
    }

    return 0;
}
catch ( exception& e )
{
    cerr << "Exception occurred: " << e.what() << endl;
}
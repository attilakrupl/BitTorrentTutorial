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

int main( int aArgc, char const* aArgv[] )
try
{
    if ( aArgc < 2 )
    {
        nTorrentUI::TerminalMessages::PrintUsage();
        return 1;
    }

    span<char const*> lStrippedArguments( aArgv, aArgc );
                      lStrippedArguments = lStrippedArguments.subspan( 1 );
    const std::string lTorrentToDownload = lStrippedArguments[0];

    ShowMenuOptions( lTorrentToDownload );

    const int lChoice = AskForChoice();

    nTorrentLogic::TorrentHandlingLogic lTorrentHandlingLogic( lStrippedArguments );

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
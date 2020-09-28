#include <iostream>
#include <thread>
#include <chrono>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>

using namespace lt;
using namespace std;

int main( int argc, char const* argv[] ) try
{
    if ( argc != 2 ) {
        cerr << "Usage: " << argv[0] << " <torrent_file_path>" << endl;
        return 1;
    }

    settings_pack       lSettingsPack;
                        lSettingsPack.set_int( settings_pack::alert_mask
                                             , alert_category::all );
    session             lSession( lSettingsPack );
    load_torrent_limits lLimits;
    torrent_info        lTorrentInfo                 ( argv[1], lLimits );
    add_torrent_params  lTorrentParameters;
                        lTorrentParameters.ti        = std::make_shared<torrent_info>( lTorrentInfo );
                        lTorrentParameters.save_path = ".";
    torrent_handle      lTorrentHandle               = lSession.add_torrent( move( lTorrentParameters ) );
    bool                lLoopCondition               = true;
    while ( lLoopCondition )
    {
        vector<alert*> lAlerts;
        lSession.pop_alerts( &lAlerts );

        for ( alert const* lAlert : lAlerts )
        {
            cout << lAlert->message() << endl;
            if ( alert_cast<torrent_finished_alert>( lAlert ) )
            {
                lLoopCondition = false;
                break;
            }
            if ( alert_cast<torrent_error_alert>( lAlert ) )
            {
                lLoopCondition = false;
                break;
            }
        }
        this_thread::sleep_for( chrono::milliseconds( 200 ) );
    }
    cout << "Done, shutting down" << endl;
}
catch ( exception& e )
{
    cerr << "Exception occurred: " << e.what() << endl;
}
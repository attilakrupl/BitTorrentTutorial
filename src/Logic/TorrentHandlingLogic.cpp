/*!
 * \file   torrenthandlinglogic.cpp
 * \author Attila Krüpl dr.
 * \date   29/09/2020
 */

#include "stdafx.h"

using namespace nTorrentLogic;
using namespace lt;

nTorrentLogic::TorrentHandlingLogic::TorrentHandlingLogic( const std::string& aTorrentPath )
    : mTorrentPath( aTorrentPath )
{}

void nTorrentLogic::TorrentHandlingLogic::DownloadTorrent()
{
    settings_pack       lSettingsPack;
                        lSettingsPack.set_int( settings_pack::alert_mask
                                             , alert_category::all );
    session             lSession( lSettingsPack );
    load_torrent_limits lLimits;
    torrent_info        lTorrentInfo                 ( mTorrentPath, lLimits );
    add_torrent_params  lTorrentParameters;
                        lTorrentParameters.ti        = std::make_shared<torrent_info>( lTorrentInfo );
                        lTorrentParameters.save_path = ".";
    torrent_handle      lTorrentHandle               = lSession.add_torrent( std::move( lTorrentParameters ) );
    bool                lLoopCondition               = true;
    while ( lLoopCondition )
    {
        std::vector<alert*> lAlerts;
        lSession.pop_alerts( &lAlerts );

        for ( alert const* lAlert : lAlerts )
        {
            std::cout << lAlert->message() << std::endl;
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
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    }
    std::cout << "Done, shutting down" << std::endl;
}

void nTorrentLogic::TorrentHandlingLogic::PrintInfo()
{
    std::cout << "Function coming soon..." << std::endl;

}

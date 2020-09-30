/*!
 * \file   torrenthandlinglogic.cpp
 * \author Attila Krüpl dr.
 * \date   29/09/2020
 */

#include "stdafx.h"

using namespace nTorrentLogic;
using namespace lt;
using namespace literals;


nTorrentLogic::TorrentHandlingLogic::TorrentHandlingLogic( const lt::span<char const*>& aTorrentArguments )
    : mTorrentArguments( aTorrentArguments )
{}

void nTorrentLogic::TorrentHandlingLogic::DownloadTorrent()
{
    settings_pack       lSettingsPack;
                        lSettingsPack.set_int( settings_pack::alert_mask
                                             , alert_category::all );
    session             lSession( lSettingsPack );
    load_torrent_limits lLimits;
    const char*         lFileName                    = mTorrentArguments[0];
    torrent_info        lTorrentInfo                 ( lFileName, lLimits );
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
    load_torrent_limits         lTorrentLimits;
    bool                        lShowPad  = false;
    const char*                 lFileName = mTorrentArguments[0];
    lt::span<const char*> lArgsCopy = mTorrentArguments.subspan(1);

    while ( !lArgsCopy.empty() )
    {
        if ( lArgsCopy[0] == "--items-limit"_sv && lArgsCopy.size() > 1 )
        {
            lTorrentLimits.max_decode_tokens = atoi( lArgsCopy[1] );
            lArgsCopy = lArgsCopy.subspan( 2 );
        }
        else if ( lArgsCopy[0] == "--depth-limit"_sv && lArgsCopy.size() > 1 )
        {
            lTorrentLimits.max_decode_depth = atoi( lArgsCopy[1] );
            lArgsCopy = lArgsCopy.subspan( 2 );
        }
        else if ( lArgsCopy[0] == "--max-pieces"_sv && lArgsCopy.size() > 1 )
        {
            lTorrentLimits.max_pieces = atoi( lArgsCopy[1] );
            lArgsCopy = lArgsCopy.subspan( 2 );
        }
        else if ( lArgsCopy[0] == "--max-size"_sv && lArgsCopy.size() > 1 )
        {
            lTorrentLimits.max_buffer_size = atoi( lArgsCopy[1] ) * 1024 * 1024;
            lArgsCopy = lArgsCopy.subspan( 2 );
        }
        else if ( lArgsCopy[0] == "--show-padfiles"_sv )
        {
            lShowPad = true;
            lArgsCopy = lArgsCopy.subspan( 1 );
        }
        else
        {
            std::cerr << "Unknown option: " << lArgsCopy[0] << "\n";
            nTorrentUI::TerminalMessages::PrintUsage();
        }
    }

    torrent_info const lTorrentInfo( lFileName, lTorrentLimits );

    if ( !lTorrentInfo.nodes().empty() )
    {
        std::printf( "Nodes:\n" );
        for ( auto const& lNode : lTorrentInfo.nodes() )
        {
            std::printf( "%s: %d\n", lNode.first.c_str(), lNode.second );
        }
    }

    if ( !lTorrentInfo.trackers().empty() )
    {
        puts( "Trackers:\n" );
        for ( auto const& lTracker : lTorrentInfo.trackers() )
            std::printf( "%2d: %s\n", lTracker.tier, lTracker.url.c_str() );
    }

    std::stringstream lTracker;
    lTracker << lTorrentInfo.info_hash();
    std::printf( "Number of pieces: %d\n"
                 "Piece length: %d\n"
                 "Info hash: %s\n"
                 "Comment: %s\n"
                 "Created by: %s\n"
                 "Magnet link: %s\n"
                 "Name: %s\n"
                 "Number of files: %d\n"
                 "Files:\n"
                 , lTorrentInfo.num_pieces()
                 , lTorrentInfo.piece_length()
                 , lTracker.str().c_str()
                 , lTorrentInfo.comment().c_str()
                 , lTorrentInfo.creator().c_str()
                 , make_magnet_uri( lTorrentInfo ).c_str()
                 , lTorrentInfo.name().c_str()
                 , lTorrentInfo.num_files() );

    lt::file_storage const& lFileStorage = lTorrentInfo.files();

    for ( auto const lFile : lFileStorage.file_range() )
    {
        auto const first = lFileStorage.map_file( lFile
                                                , 0
                                                , 0 ).piece;

        auto const last  = lFileStorage.map_file( lFile
                                                , std::max( std::int64_t( lFileStorage.file_size( lFile ) ) - 1
                                                , std::int64_t( 0 ) ), 0 ).piece;

        auto const flags = lFileStorage.file_flags( lFile );

        if ( ( flags & file_storage::flag_pad_file ) 
            && !lShowPad )
        {
            continue;
        }

        std::stringstream lFileRoot;
        std::printf( " %8" PRIx64 " %11" PRId64 " %c%c%c%c [ %5d, %5d ] %7u %s %s %s%s\n"
                     , lFileStorage.file_offset( lFile )
                     , lFileStorage.file_size( lFile )
                     , ( ( flags & file_storage::flag_pad_file ) ? 'p' : '-' )
                     , ( ( flags & file_storage::flag_executable ) ? 'x' : '-' )
                     , ( ( flags & file_storage::flag_hidden ) ? 'h' : '-' )
                     , ( ( flags & file_storage::flag_symlink ) ? 'l' : '-' )
                     , static_cast<int>( first )
                     , static_cast<int>( last )
                     , std::uint32_t( lFileStorage.mtime( lFile ) )
                     , lFileRoot.str().c_str()
                     , lFileStorage.file_path( lFile ).c_str()
                     , ( flags & file_storage::flag_symlink ) ? "-> " : ""
                     , ( flags & file_storage::flag_symlink ) ? lFileStorage.symlink( lFile ).c_str() : "" );
    }

    std::printf( "Web seeds:\n" );
    for ( auto const& ws : lTorrentInfo.web_seeds() )
    {
        std::printf( "%s %s\n"
                     , ws.type == web_seed_entry::url_seed ? "BEP19" : "BEP17"
                     , ws.url.c_str() );
    }
}

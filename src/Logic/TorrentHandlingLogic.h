/*!
 * \file   torrenthandlinglogic.h
 * \author Attila Krüpl dr.
 * \date   29/09/2020
 */

#pragma once

namespace nTorrentLogic
{
    class TorrentHandlingLogic final
    {
        private:
            lt::span<char const*> mTorrentArguments;

        public:
            explicit TorrentHandlingLogic( const lt::span<char const*>& aTorrentArguments );
            ~TorrentHandlingLogic() = default;

        public:
            void DownloadTorrent();
            void PrintInfo();
    };
}

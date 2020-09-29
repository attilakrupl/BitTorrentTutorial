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
            std::string mTorrentPath;

        public:
            explicit TorrentHandlingLogic( const std::string& aTorrentPath );
            ~TorrentHandlingLogic() = default;

        public:
            void DownloadTorrent();
            void PrintInfo();
    };
}

/*!
 * \file   stdafx.h
 * \author Attila Krüpl dr.
 * \date   29/09/2020
 */

#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdio>
#include <cinttypes>

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/bdecode.hpp>
#include <libtorrent/span.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/magnet_uri.hpp>

#include "UI/TerminalMessages.h"

#include "Logic/TorrentHandlingLogic.h"
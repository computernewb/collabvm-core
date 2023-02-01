//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#include <collabvm/core/Panic.hpp>

#include <spdlog/spdlog.h>

#include <cstdlib>

#include <stacktrace>

namespace collabvm::core {

	[[noreturn]] void Panic(std::string_view message, const boost::source_location& loc) {
		spdlog::error("Panic @ {}:{} : {}", loc.file_name(), loc.line(), message);

		spdlog::error("Stack trace:");
		for(const auto& frame : std::stacktrace()) {
			if(!frame.description().empty())
				if(!frame.source_file().empty())
					spdlog::error("    {} ({:p}), @ {}:{}", frame.description(), std::bit_cast<void*>(frame.native_handle()), frame.source_file(), frame.source_line());
				else
					spdlog::error("    {} ({:p})", frame.description(), std::bit_cast<void*>(frame.native_handle()));
			else
				spdlog::error("    {:p}", std::bit_cast<void*>(frame.native_handle()));
		}

		std::quick_exit(1);
	}

} // namespace collabvm::core

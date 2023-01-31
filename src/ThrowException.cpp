//
// CollabVM
//
// (C) 2021-2022 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

// This file overrides Boost.ThrowException calls to Panic() when it's called.

#include <boost/assert/source_location.hpp>
#include <exception>

#include <fmt/core.h>
#include <collabvm/core/Panic.hpp>

namespace boost {

	void throw_exception(const std::exception& e) {
		auto format = fmt::format("Caught attempt to throw exception with what(): \"{}\"", e.what());
		collabvm::core::Panic(format);
	}

	void throw_exception(const std::exception& e, const boost::source_location& loc) {
		auto format = fmt::format("Caught attempt to throw exception with what() @ {}:{}: \"{}\"", loc.file_name(), loc.line(), e.what());
		collabvm::core::Panic(format);
	}
} // namespace boost

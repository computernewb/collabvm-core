//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

#include <boost/assert/source_location.hpp>
#include <string_view>
#include <utility>

namespace collabvm::core {

	/**
	 * Panic (exit) application on unrecoverable errors.
	 */
	[[noreturn]] void Panic(std::string_view message, boost::source_location const& loc = BOOST_CURRENT_LOCATION);

	// TODO find a better spot for this

#define COLLABVM_VERIFY(expr)                                           \
	do {                                                               \
		if(!(expr)) [[unlikely]] {                                     \
			::collabvm::core::Panic("COLLABVM_VERIFY(" #expr ") failed."); \
			__builtin_unreachable();                                   \
		}                                                              \
	} while(0)

#ifndef NDEBUG
	#define COLLABVM_ASSERT(expr)                                           \
		do {                                                               \
			if(!(expr)) [[unlikely]] {                                     \
				::collabvm::core::Panic("COLLAB3_ASSERT(" #expr ") failed."); \
				__builtin_unreachable();                                   \
			}                                                              \
		} while(0)
#else
	#define COLLABVM_ASSERT(expr)
#endif

#define COLLABVM_TRY(expr)                     \
	({                                        \
		auto _result = (expr);                \
		if(!_result.has_value()) [[unlikely]] \
			return std::move(result).error(); \
		return std::move(result).value();     \
	})

#define COLLABVM_MUST(expr)                \
	({                                    \
		auto _result = (expr);            \
		COLLABVM_VERIFY(!expr.has_error()) \
		return std::move(result).value(); \
	})

} // namespace collabvm::core

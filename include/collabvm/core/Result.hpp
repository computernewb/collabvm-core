//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

#include <collabvm/core/Error.hpp>
#include <collabvm/core/FundamentalTypes.hpp>
#include <collabvm/core/Panic.hpp>
#include <expected>

namespace collabvm::core {

	// TODO(lily): While this is a core-wide error, I don't think
	// 	<Result.hpp> is the right spot for it

	enum class GenericError {
		Ok,						///< No error.
		MemoryAllocationFailure ///< Error allocating memory.
	};

	template<>
	struct ErrorCategory<GenericError> : ErrorCategoryMixin<GenericError> {
		constexpr static const char* ErrorMessages[] = {
			"No error",
			"Error allocating memory"
		};
	};

	template<class T, class E>
	using Expected = std::expected<T, E>;

	template<class T, class EC>
	using Result = Expected<T, Error<EC>>;

#define COLLABVM_CORE_TRY(expr)                \
	({                                         \
		auto _result = (expr);                 \
		if(!_result.has_value()) [[unlikely]]  \
			return std::move(_result).error(); \
		return std::move(_result).value();     \
	})

#define COLLABVM_CORE_MUST(expr)           \
	({                                     \
		auto _result = (expr);             \
		COLLABVM_CHECK(!expr.has_error()) \
		return std::move(_result).value(); \
	})

} // namespace collabvm::core

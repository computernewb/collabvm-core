//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

namespace collabvm::core {

	/**
	 * An error category. Provides functions for working
	 * with a single error code type.
	 *
	 * Needs to be specialized using ErrorMixin which will implement
	 * the required static member functions for you.
	 *
	 * \tparam EC
	 */
	template<class EC>
	struct ErrorCategory;

	/**
	 * Mixin type for implementing ErrorCategory.
	 */
	template<class EC, class T = ErrorCategory<EC>>
	struct ErrorCategoryMixin {
		using CodeType = EC;

		constexpr ErrorCategoryMixin() = delete;

		static constexpr const char* Message(CodeType code) {
			return T::ErrorMessages[static_cast<u32>(code)];
		}

		static constexpr CodeType OkSymbol() {
			return EC::Ok;
		}

		static bool Ok(CodeType code) {
			return code == OkSymbol();
		}
	};

	/**
	 * An error. Light-weight wrapper over an error code.
	 *
	 * Error category storage is handled as a part of the type, so all Error(s)
	 * are literal and only sizeof(EC), instead of needing to store a pointer
	 * and pay the price of multiple virtual function calls.
	 *
	 * This type *can* be directly used, but typically is used indirectly via core::Result<>.
	 *
	 * \tparam EC 		Error code type.
	 * \tparam Category The error category.
	 */
	template<class EC, class Category = ErrorCategory<EC>>
	struct Error {
		using CategoryType = Category;

		// Ok error (uses the OkSymbol)
		constexpr Error() :
			Error(Category::OkSymbol()) {
		}

		constexpr Error(typename Category::CodeType code) :
			ec(code) {
			// not a great spot for this kind of thing, but oh well
			static_assert(sizeof(Error) == sizeof(EC), "Basic assumptions with your compiler seem to be horribly wrong");
		}

		constexpr const char* Message() {
			return Category::Message(ec);
		}

		constexpr operator bool() const {
			return Category::Ok(ec);
		}

		constexpr operator typename Category::CodeType() const {
			return ec;
		}

	   private:
		typename Category::CodeType ec;
	};

	template<class EC>
	Error(EC code) -> Error<EC>;

	/**
	 * Generic CollabVM-wide error codes.
	 */
	enum class GenericError {
		Ok = 0x0,						///< No error.
		MemoryAllocationFailure ///< Error allocating memory.
	};

	template<>
	struct ErrorCategory<GenericError> : ErrorCategoryMixin<GenericError> {
		constexpr static const char* ErrorMessages[] {
			"No error",
			"Error allocating memory"
		};
	};

} // namespace collabvm::core

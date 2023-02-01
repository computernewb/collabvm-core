//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

// Some fundamental types.

#pragma once

#include <cstdint>
#include <memory>

namespace collabvm::core {

#define T(bitsize)                             \
	using u##bitsize = std::uint##bitsize##_t; \
	using s##bitsize = std::int##bitsize##_t

	T(8);
	T(16);
	T(32);
	T(64);

#undef T

	using ull = unsigned long long;
	using sll = signed long long;

	using usize = std::size_t;
	using ssize = std::intptr_t;

	namespace detail {

		template<class T>
		struct Point {
			T x;
			T y;
		};

		template<class T>
		struct Size {
			T width;
			T height;

			/**
			 * Returns the linear size, which is the product of
			 * `width * height`. May need an additional stride or pixel size.
			 */
			constexpr T Linear() const {
				return width * height;
			}
		};

		template<class T>
		struct Rect {
			T x;
			T y;
			T width;
			T height;

			constexpr Rect(T x, T y, T w, T h) :
				x(x),
				y(y),
				width(w),
				height(h) {
			}

			constexpr Rect(Size<T> size) :
				x(0),
				y(0),
				width(size.width),
				height(size.height) {
			}

			/**
			 * Get the origin coordinate as a point.
			 * \return a Point<T> with the origin.
			 */
			constexpr auto GetOrigin() const {
				return Point<T> { .x = x, .y = y };
			}

			/**
			 * Get the size of this rect.
			 * \return a Point<T> which contains the calculated size of the rect
			 */
			constexpr auto GetSize() const {
				return Size<T> { .width = width, .height = height };
			}

			constexpr bool InBounds(const Rect& other) {
				if(x < other.x || x + other.width > other.x + other.width)
					return false;

				if(y < other.y || x + other.height > other.y + other.height)
					return false;

				return true;
			}

			// more methods.
		};

	} // namespace detail

	constexpr inline usize operator""_Kb(ull value) {
		return static_cast<usize>(value) * (1024);
	}

	constexpr inline usize operator""_KB(ull value) {
		return static_cast<usize>(value) * (1000);
	}

	constexpr inline usize operator""_Mb(ull value) {
		return static_cast<usize>(value) * (1024 * 1024);
	}

	constexpr inline usize operator""_MB(ull value) {
		return static_cast<usize>(value) * (1000 * 1000);
	}

	constexpr inline usize operator""_Gb(ull value) {
		return static_cast<usize>(value) * (1024 * 1024 * 1024);
	}

	constexpr inline usize operator""_GB(ull value) {
		return static_cast<usize>(value) * (1000 * 1000 * 1000);
	}

	template<class T>
	using Own = std::unique_ptr<T>;

	template<class T>
	using Ref = std::shared_ptr<T>;

	template<class T>
	using Weak = std::weak_ptr<T>;

	using detail::Point;
	using detail::Rect;
	using detail::Size;

} // namespace collabvm::core

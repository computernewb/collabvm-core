//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

#include <collabvm/core/FundamentalTypes.hpp>
#include <collabvm/core/Result.hpp>


namespace collabvm::core::graphics {


	/**
	 * A simple framebuffer.
	 *
	 * Notes:
	 * - The data is always stored in RGBA8888,
	 * 		and data from other sources should be converted beforehand.
	 *
	 * - This framebuffer is intentionally not copyable.
	 * 		It is movable because there may be legitimate use cases for it.
	 */
	struct Framebuffer {
		using SizeType = u16;
		using Point = core::Point<SizeType>;
		using Size = core::Size<SizeType>;
		using Rect = core::Rect<SizeType>;

		union Pixel {
			u32 raw;
			struct {
				u8 r;
				u8 g;
				u8 b;
				u8 a;
			};

			// later: factory functions to convert:
			// - palletized 8bpp
			// - 16bpp
			// to Pixel/RGBA8888
		};

		/**
		 * A cheap subview of pixel data.
		 */
		struct Slice {

			[[nodiscard]] constexpr Pixel* GetBuffer() const {
				return pixelBuffer;
			}

			[[nodiscard]] constexpr const Size& GetSize() const {
				return size;
			}

			constexpr Pixel& operator[](usize x, usize y) {
				return pixelBuffer[size.width * y + x];
			}

			[[nodiscard]] constexpr bool Valid() const {
				return pixelBuffer != nullptr;
			}

			inline static Slice New(Pixel* pixelData, Size size) {
				return Slice {
					pixelData,
					size
				};
			}

		   private:
				friend Framebuffer; // The framebuffer can create arbitrary slices.

				constexpr Slice(Pixel* pixelStart, Size size)
					: pixelBuffer(pixelStart),
						size(size) {

				}

				Pixel* pixelBuffer{};
				Size size;
		};

		/**
		 * Create a new framebuffer.
		 */
		static core::Result<Framebuffer, GenericError> New(Size size);

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		// Allow move-construction of framebuffers
		Framebuffer(Framebuffer&&) noexcept = default;
		Framebuffer& operator=(Framebuffer&&) noexcept = default;

		/**
		 * Resize the framebuffer. This will repaint the contents
		 * to the resized view as well.
		 *
		 * \note Once this function is called, all in-flight slices
		 * 		 should be treated invalidated if this function
		 * 		 is able to successfully complete.
		 */
		core::Result<void, GenericError> Resize(Size newSize);

		[[nodiscard]] constexpr Size GetSize() const {
				return size;
		}

		/**
		 * Get a slice of the framebuffer
		 *
		 * \param[in] rect The rectangle to obtain a slice of.
		 * \return A valid slice or an empty slice on error.
		 */
		[[nodiscard]] Slice GetSlice(const Rect& rect);

		/**
		 * Draw a slice of image data to this framebuffer.
		 *
		 * \param[in] point Where the slice should be drawn
		 * \param[in] slice The slice itself.
		 */
		void PaintSlice(Point location, Slice slice);

	   private:
		Size size{};
		Own<Pixel[]> pixelData;

		Framebuffer() = default;
	};

}
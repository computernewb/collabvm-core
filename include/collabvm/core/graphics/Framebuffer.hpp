//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

#include <collabvm/core/FundamentalTypes.hpp>

namespace collabvm::core::graphics {


	/**
	 * A simple framebuffer.
	 *
	 * Notes:
	 * - The data is always stored in RGBA8888,
	 * 		and data from other sources is converted beforehand.
	 *
	 * - This framebuffer is intentionally not copyable.
	 * 		It is movable because there may be legitimate use cases for it.
	 */
	struct Framebuffer {
		using SizeType = u16;
		using Point = core::Point<SizeType>;
		using Size = core::Size<SizeType>;
		using Rect = core::Rect<SizeType>;

		/**
		 * A cheap subview of pixel data.
		 */
		struct Slice {

			static Slice New();

		   private:
				friend Framebuffer; // The framebuffer can create arbitrary slices.
		};


		static Framebuffer New(Size size);

		void Resize(Size newSize);

		/**
		 * Draw a slice of image data to this framebuffer.
		 *
		 * \param[in] point Where the slice should be drawn
		 * \param[in] slice The slice itself.
		 */
		void PaintSlice(Point point, Slice slice);

	};

}
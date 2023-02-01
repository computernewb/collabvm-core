//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#include <collabvm/core/graphics/Framebuffer.hpp>

#include <algorithm>

namespace collabvm::core::graphics {

	/* static */ core::Result<Framebuffer, GenericError> Framebuffer::New(Size size) {
		Framebuffer framebuffer;

		if(auto res = framebuffer.Resize(size); !res.has_value())
			return std::unexpected(res.error());
		else
			return framebuffer;
	}

	core::Result<void, GenericError> Framebuffer::Resize(Size newSize) {
		// Keep the old pixel data around for a bit
		// (would moving *this work better.. probably)
		auto oldBuffer = std::move(pixelData);
		auto oldSize = size;

		pixelData = std::make_unique<Pixel[]>(newSize.Linear());
		if(!pixelData) {
			pixelData = std::move(oldBuffer);
			return std::unexpected(GenericError::MemoryAllocationFailure);
		}

		size = newSize;

		// might be nice to have this as a utility factory
		// in the Size template itself...

		auto copyableSize = Size {
			std::min(oldSize.width, size.width),
			std::min(oldSize.height, size.height)
		};

		// Copy the old framebuffer data then release it.
		std::memcpy(&pixelData[0], &oldBuffer[0], copyableSize.Linear() * sizeof(Pixel));
		oldBuffer.reset();

		return {};
	}

	Framebuffer::Slice Framebuffer::GetSlice(const Rect& rect) {
		auto drawArea = Rect(size);

		if(drawArea.InBounds(rect))
			return Slice::New(&pixelData[size.width * rect.y + rect.x], rect.GetSize());

		// return an empty slice
		return Slice::New(nullptr, {});
	}

	void Framebuffer::PaintSlice(Point location, Slice slice) {
		if(!pixelData)
			return;

		auto drawArea = Rect(size);
		Rect rect{
			location.x,
			location.y,
			slice.GetSize().width,
			slice.GetSize().height
		};

		if(drawArea.InBounds(rect))
			memcpy(&pixelData[size.width * rect.y + rect.x], slice.GetBuffer(), slice.GetSize().Linear() * sizeof(Pixel));
	}

} // namespace collabvm::core::graphics
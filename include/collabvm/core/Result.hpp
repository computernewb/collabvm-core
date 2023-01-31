//
// CollabVM
//
// (C) 2021-2023 CollabVM Development Team
//
// SPDX-License-Identifier: GPL-3.0
//

#pragma once

#include <tl/expected.hpp>

namespace collabvm::core {

	template<class T, class E>
	using Result = tl::expected<T, E>;

}

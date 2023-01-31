add_library(collabvm_core
		# ASIO/Beast implementation files
		asio/AsioImplTu.cpp
		asio/BeastImplTu.cpp

		ThrowException.cpp
		Panic.cpp
		)

target_include_directories(collabvm_core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(collabvm_core PRIVATE Boost::system)

target_link_libraries(collabvm_core PUBLIC
		fmt::fmt
		spdlog::spdlog
		tl::expected
		)

target_compile_definitions(collabvm_core PUBLIC
		# We always use fmt as a separate library,
		# so stop spdlog's detection logic right in its tracks.
		# Also disable exceptions
		-DSPDLOG_FMT_EXTERNAL
		-DSPDLOG_NO_EXCEPTIONS

		-DBOOST_ASIO_SEPARATE_COMPILATION=1
		-DBOOST_ASIO_NO_DEPRECATED=1
		-DBOOST_ASIO_DISABLE_EPOLL=1
		-DBOOST_ASIO_DISABLE_BOOST_ARRAY=1
		-DBOOST_ASIO_DISABLE_BOOST_BIND=1
		-DBOOST_ASIO_DISABLE_BOOST_DATE_TIME=1
		-DBOOST_ASIO_DISABLE_BOOST_REGEX=1
		-DBOOST_ASIO_NO_EXCEPTIONS=1

		-DBOOST_BEAST_SEPARATE_COMPILATION=1
		)


collabvm_core_target(collabvm_core)


# Add public alias (Use this, please)
add_library(collabvm::core ALIAS collabvm_core)

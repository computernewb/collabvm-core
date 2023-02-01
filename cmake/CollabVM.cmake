# CollabVM.cmake: some useful CollabVM cmake functions

# Adds some stuff to targets to give them CollabVM core defines
# and misc. options
function(collabvm_core_target target)
	target_compile_definitions(${target} PRIVATE "$<$<CONFIG:DEBUG>:COLLABVM_CORE_DEBUG>")
	target_compile_features(${target} PUBLIC cxx_std_23)

	if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
		# Clang does not implicitly support sized deallocation and needs
		# another compiler argument; this ends up breaking a place in
		# <stacktrace> which seems to require it. GCC does do the right thing tbf
		set(_COLLABVM_CORE_COMPILE_ARGS -Wall -Wextra -Werror -fsized-deallocation)
	else()
		set(_COLLABVM_CORE_COMPILE_ARGS -Wall -Wextra -Werror)
	endif()

	# default compile options to the core compile flags
	target_compile_options(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS})

	# Enable asan/tsan and/or ubsan

	if("asan" IN_LIST COLLABVM_BUILD_FEATURES)
		# Error if someone's trying to mix asan and tsan together,
		# they aren't compatible.
		if("tsan" IN_LIST COLLABVM_BUILD_FEATURES)
			message(FATAL_ERROR "ASAN and TSAN cannot be mixed into a build.")
		endif()

		message(STATUS "Enabling ASAN for target ${target} because it was in COLLABVM_BUILD_FEATURES")
		target_compile_options(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=address)
		target_link_libraries(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=address)
	endif()

	if("tsan" IN_LIST COLLABVM_BUILD_FEATURES)
		message(STATUS "Enabling TSAN for target ${target} because it was in COLLABVM_BUILD_FEATURES")
		target_compile_options(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=thread)
		target_link_libraries(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=thread)
	endif()

	if("ubsan" IN_LIST COLLABVM_BUILD_FEATURES)
		message(STATUS "Enabling UBSAN for target ${target} because it was in COLLABVM_BUILD_FEATURES")
		target_compile_options(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=undefined)
		target_link_libraries(${target} PRIVATE ${_COLLABVM_CORE_COMPILE_ARGS} -fsanitize=undefined)
	endif()
endfunction()

function(collabvm_core_add_tag_target name output_path)
	add_custom_target(${name}
		COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/cmake/GitTag.cmake
			WORKING_DIRECTORY ${output_path}
			SOURCES ${output_path}/Version.h)
	set_source_files_properties(${output_path}/Version.h PROPERTIES GENERATED TRUE)
endfunction()

function(collabvm_core_set_alternate_linker linker)
	find_program(LINKER_EXECUTABLE ld.${COLLABVM_LINKER} ${COLLABVM_LINKER})
	if(LINKER_EXECUTABLE)
		message(STATUS "CollabVM Core: Using ${COLLABVM_LINKER} to link")

		# This is supremely, utterly blegh but I guess it works for a global project-wide linker
		if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 12.0.0)
			add_link_options("-ld-path=${COLLABVM_LINKER}")
		else()
			add_link_options("-fuse-ld=${COLLABVM_LINKER}")
		endif()
	else()
		message(FATAL_ERROR "Linker ${COLLABVM_LINKER} does not exist on your system.")
	endif()
endfunction()

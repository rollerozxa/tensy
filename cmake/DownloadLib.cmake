
function(download_dep_tarball depname version url)
	set(lib_path "${CMAKE_SOURCE_DIR}/lib")

	get_filename_component(filename "${url}" NAME)
	set(download_path "${lib_path}/${depname}-${filename}")

	set(dep_path "${lib_path}/${depname}")
	set(ver_file "${dep_path}/.download_version")

	if(EXISTS "${ver_file}")
		file(READ "${ver_file}" current_version)
		if("${current_version}" STREQUAL "${version}")
			# up to date
			return()
		endif()

		# outdated, redownload
		file(REMOVE_RECURSE "${dep_path}")
	endif()

	file(MAKE_DIRECTORY "${dep_path}")

	message(STATUS "Downloading ${depname} ${version}")
	file(DOWNLOAD "${url}" "${download_path}" STATUS status)

	list(GET status 0 status_code)
	if(NOT status_code EQUAL 0)
		message(FATAL_ERROR "Failed to download ${depname}, status: ${status}")
	endif()

	message(STATUS "Extracting...")
	execute_process(
		COMMAND tar -xzf "${download_path}" --strip-components=1
		WORKING_DIRECTORY "${dep_path}"
		RESULT_VARIABLE extract_result)

	if(NOT extract_result EQUAL 0)
		message(FATAL_ERROR "Failed to extract ${depname}")
	endif()

	file(WRITE "${ver_file}" "${version}")
endfunction()

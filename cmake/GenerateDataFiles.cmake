
function(generate_data_files)
	file(GLOB DATA_FILES "${CMAKE_SOURCE_DIR}/data/*")
	foreach(DATA_FILE ${DATA_FILES})
		get_filename_component(FILENAME_WE ${DATA_FILE} NAME_WE)
		set(GEN_HEADER "${CMAKE_BINARY_DIR}/data/${FILENAME_WE}.h")

		add_custom_command(
			OUTPUT ${GEN_HEADER}
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/data"
			COMMAND "${CMAKE_SOURCE_DIR}/lib/bin2h.sh" "${DATA_FILE}" > "${GEN_HEADER}"
			DEPENDS ${DATA_FILE}
			COMMENT "Generating ${GEN_HEADER} from ${DATA_FILE}"
		)

		set(GEN_HEADERS ${GEN_HEADERS} ${GEN_HEADER})
	endforeach()
	set(GENERATED_DATA_HEADERS ${GEN_HEADERS} PARENT_SCOPE)
endfunction()

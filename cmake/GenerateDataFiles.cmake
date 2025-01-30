
function(generate_data_files)
	file(GLOB DATA_FILES "${CMAKE_SOURCE_DIR}/data/*")
	foreach(DATA_FILE ${DATA_FILES})
		get_filename_component(FILENAME_WE ${DATA_FILE} NAME_WE)
		set(GENERATED_HEADER "${CMAKE_BINARY_DIR}/data/${FILENAME_WE}.h")

		add_custom_command(
			OUTPUT ${GENERATED_HEADER}
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/data"
			COMMAND xxd -n ${FILENAME_WE} -i ${DATA_FILE} > ${GENERATED_HEADER}
			DEPENDS ${DATA_FILE}
			COMMENT "Generating ${GENERATED_HEADER} from ${DATA_FILE}"
		)

		set(GENERATED_HEADERS ${GENERATED_HEADERS} ${GENERATED_HEADER} PARENT_SCOPE)
	endforeach()

	add_custom_target(generate_data DEPENDS ${GENERATED_HEADERS})
endfunction()

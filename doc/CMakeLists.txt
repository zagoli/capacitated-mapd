message(DEBUG "processing doc folder")

# --- Doxygen ---

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    doxygen_add_docs(doc
            src ${PROJECT_SOURCE_DIR}/README.md
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            )
else ()
    message(AUTHOR_WARNING "Doxygen not found. Can't generate the documentation.")
endif ()

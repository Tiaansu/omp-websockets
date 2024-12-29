file(GLOB_RECURSE OMPWS_FILES
    *.hpp
    *.cpp
)

add_library(${PROJECT_NAME} SHARED ${OMPWS_FILES})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/deps/pawn/source
    ${PROJECT_SOURCE_DIR}/deps/pawn/source/linux
)

GroupSourcesByFolder(${PROJECT_NAME})

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0600)
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/..:$ORIGIN/")

    set(CMAKE_CXX_FLAGS "-Wno-int-to-pointer-cast -Wno-attributes ${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "-Wno-int-to-pointer-cast -Wno-attributes ${CMAKE_C_FLAGS}")

    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()

target_link_libraries(${PROJECT_NAME}
    ${LIBRARIES}
    OMP-SDK
)
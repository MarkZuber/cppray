include(GoogleTest)

include_directories(${gtest_SOURCE_DIR}/include)

function(add_unittest target)
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} gmock_main)

    add_test(${target} ${target})
    gtest_add_tests(TARGET ${target})

    # add_custom_command(TARGET ${target}
    #                    POST_BUILD
    #                    COMMAND ${target}
    #                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #                    COMMENT "Running ${target}" VERBATIM)
endfunction()

file(GLOB SRCS *.cpp)
add_unittest(sample_tests ${SRCS})


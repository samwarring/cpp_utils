add_executable(
    samwarring_cpp_utils_test
    main.cpp
    instance_tracker_test.cpp
)

# Use Catch2 unit test framework
include(FetchContent)
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.1
)
fetchcontent_makeavailable(Catch2)
target_link_libraries(
    samwarring_cpp_utils_test
    PRIVATE Catch2
            samwarring_cpp_utils
)
fetchcontent_getproperties(
    Catch2
    SOURCE_DIR Catch2_SOURCE_DIR
)
list(
    APPEND
    CMAKE_MODULE_PATH
    "${Catch2_SOURCE_DIR}/contrib"
)
include(Catch)
catch_discover_tests(samwarring_cpp_utils_test)

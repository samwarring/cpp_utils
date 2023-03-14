add_executable(
    samwarring_cpp_utils_test
    instance_tracker_test.cpp
    ring_buffer_test.cpp
    singleton_test.cpp
    quantity_test.cpp
)

# Require std::thread
find_package(
    Threads
    REQUIRED
)

# Use Catch2 unit test framework
include(FetchContent)
fetchcontent_declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.3.2
)
fetchcontent_makeavailable(Catch2)
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

target_link_libraries(
    samwarring_cpp_utils_test
    PRIVATE Catch2::Catch2WithMain
            samwarring_cpp_utils
            Threads::Threads
)

# Get all the C++ files composing the Characters & Blade libraries
file(GLOB utils_FILES ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.hpp")

# Create shared libraries using these files
add_library(Utils SHARED ${utils_FILES})

# Set the C++ standard to use for these libraries
target_compile_features(Utils PUBLIC cxx_std_11)

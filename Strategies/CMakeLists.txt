# Get all the C++ files composing the Characters & Blade libraries
file(GLOB strategies_FILES ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.hpp")

# Create shared libraries using these files
add_library(Strategies SHARED ${strategies_FILES})

# Define the include directory for these libraries
# target_include_directories(Strategies PUBLIC ${CMAKE_SOURCE_DIR}/utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

# Set the C++ standard to use for these libraries
target_compile_features(Strategies PUBLIC cxx_std_11)

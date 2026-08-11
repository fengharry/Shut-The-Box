# Get all the C++ files composing the Characters & Blade libraries
file(GLOB shut_the_box_FILES ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.hpp")

# Create shared libraries using these files
add_library(ShutTheBox SHARED ${shut_the_box_FILES})

# Define the include directory for these libraries
# target_include_directories(ShutTheBox PUBLIC ${CMAKE_SOURCE_DIR}/utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

# Set the C++ standard to use for these libraries
target_compile_features(ShutTheBox PUBLIC cxx_std_11)

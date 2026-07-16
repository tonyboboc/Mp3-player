# Create the executable from main.cpp
add_executable(${PROJECT_NAME} main.cpp)

# Find Raylib (make sure libraylib-dev is installed)
find_package(raylib REQUIRED)

# Link Raylib to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
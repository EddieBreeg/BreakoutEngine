add_subdirectory(entt)
add_subdirectory(fmt)

set(CMAKE_FOLDER "3rd_party/json")
add_subdirectory(json)

set(CMAKE_FOLDER "3rd_party/SDL2")
add_subdirectory(SDL)

set(CMAKE_FOLDER "3rd_party/ImGUI")
add_subdirectory(imgui)

set(CMAKE_FOLDER "3rd_party")
add_subdirectory(tinyfiledialogs)
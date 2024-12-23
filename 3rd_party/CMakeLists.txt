set(CMAKE_FOLDER "3rd_party/SDL3")
add_subdirectory(SDL)

set(CMAKE_FOLDER "3rd_party/ImGUI")
add_subdirectory(imgui)

set(CMAKE_FOLDER "3rd_party")
add_subdirectory(entt)
add_subdirectory(fmt)
add_subdirectory(json)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(bzip2)
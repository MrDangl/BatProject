# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.

project ("BatProject")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (BatResilProject "CMakeProject1.cpp" "CMakeProject1.h")

# Создание библиотеки


add_subdirectory(lib/pugiXML)
add_subdirectory(lib/cmd_line)
find_package(OpenMP)
find_package(pugs)

if(OpenMP_CXX_FOUND)
    target_link_libraries(BatResilProject PUBLIC OpenMP::OpenMP_CXX)
endif()
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BatResilProject PROPERTY CXX_STANDARD 17)
endif()
target_link_libraries(BatResilProject PRIVATE pugs)
target_link_libraries(BatResilProject PRIVATE cmd_line)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.

project ("BatProject")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h")

# Создание библиотеки


add_subdirectory(lib/pugiXML)
find_package(OpenMP)
find_package(pugs)

if(OpenMP_CXX_FOUND)
    target_link_libraries(CMakeProject1 PUBLIC OpenMP::OpenMP_CXX)
endif()
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)
endif()
target_link_libraries(CMakeProject1 PRIVATE pugs)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

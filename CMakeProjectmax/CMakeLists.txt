# CMakeList.txt: projekt CMake dla elementu CMakeProjectmax, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#

# Dodaj źródło do pliku wykonywalnego tego projektu.
cmake_minimum_required(VERSION 3.8)

project("CMakeProjectmax")

# Enable GDI+ and specify the WIN32 subsystem for GUI apps
add_executable(CMakeProjectmax WIN32 CMakeProjectmax.cpp)  # <-- KEY CHANGE

# Link GDI+ to your executable


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeProjectmax PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.

cmake_minimum_required(VERSION 3.16)
project(WordLadderGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. SPUNEM CLION-ULUI UNDE ESTE QT
# Aici trebuie să înlocuiești "6.x.x" cu versiunea exactă de Qt pe care o ai pe Mac (ex: 6.5.3 sau 6.7.0)
set(CMAKE_PREFIX_PATH "/Users/amaliatanase/Qt/6.11.1/macos" ${CMAKE_PREFIX_PATH})

# 2. Activăm compilatoarele speciale pentru interfața grafică
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 3. Căutăm librăriile Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 4. Adăugăm ABSOLUT TOATE fișierele proiectului
add_executable(WordLadderGUI
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        WordLadderGame.cpp
        WordLadderGame.h
        Graph.h
)

target_link_libraries(WordLadderGUI PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
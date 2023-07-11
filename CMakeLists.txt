cmake_minimum_required(VERSION 3.24)
project(tableProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(tableProject main.cpp mainwindow.cpp mainwindow.h utils/csv.cpp utils/csv.h newStudentDialog.cpp newStudentDialog.h StudentTableWidget.cpp StudentTableWidget.h ScheduleTableWidget.cpp ScheduleTableWidget.h NewScheduleDialog.cpp NewScheduleDialog.h TableAbstractWidget.h AboutDialogWindow.cpp AboutDialogWindow.h LogoWidget.cpp LogoWidget.h)
target_link_libraries(tableProject
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )


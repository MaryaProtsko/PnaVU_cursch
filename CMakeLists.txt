cmake_minimum_required(VERSION 3.5)

project(CWLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MySQL_LIBRARY "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")
set(MySQL_INCLUDE_DIR "C:/Program Files/MySQL/MySQL Server 8.0/include")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(MySQL REQUIRED) # MYSQL

set(PROJECT_SOURCES
        main.cpp
        ui/mainwindow.cpp
        ui/mainwindow.h
        ui/mainwindow_old.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CWLibrary
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        ui/mainwindow.ui
        globalinstances.h globalinstances.cpp

        main/global.h main/global.cpp
        main/model.h main/model.cpp
        main/view.h main/view.cpp
        main/controller.h main/controller.cpp
        main/servicehandler.h main/servicehandler.cpp
        main/clienthandler.h main/clienthandler.cpp

        components/logger.h components/logger.cpp
        components/database/database.h components/database/database.cpp
        components/database/internal/tables.cpp
        components/database/internal/tables.h
        components/indexhandler.h components/indexhandler.cpp

        components/requests/requestservice.h components/requests/requestservice.cpp
        components/requests/irequestservice.h components/requests/irequestservice.cpp

        components/database/databaserel.h components/database/databaserel.cpp
        components/database/databasenorel.h components/database/databasenorel.cpp

        components/data/datastorer.h components/data/datastorer.cpp
        components/data/dataconverter.h components/data/dataconverter.cpp

        components/auth/authhandler.h components/auth/authhandler.cpp
        components/auth/accountmanager.h components/auth/accountmanager.cpp
        components/auth/account.h components/auth/account.cpp

        utils/filesystemutils.h utils/filesystemutils.cpp
        utils/datetimeutils.h utils/datetimeutils.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CWLibrary APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CWLibrary SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CWLibrary
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CWLibrary PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CWLibrary PRIVATE ${MySQL_LIBRARY}) # MYSQL
target_link_libraries(CWLibrary PRIVATE Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CWLibrary)
endif()
set_target_properties(CWLibrary PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include_directories(${MySQL_INCLUDE_DIR}) # MYSQL
include(GNUInstallDirs)
install(TARGETS CWLibrary
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CWLibrary)
endif()

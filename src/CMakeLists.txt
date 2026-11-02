cmake_minimum_required(VERSION 3.21)

project(qt6purchasing LANGUAGES CXX)

find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Gui
    Qml
)

qt_standard_project_setup(REQUIRES 6.8)
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Platform-specific sources and libraries
set(PLATFORM_LIBS "")
set(PLATFORM_SOURCES "")
set(PLATFORM_HEADERS "")
set(PLATFORM_INCLUDE "")

if(ANDROID)
    list(APPEND PLATFORM_SOURCES
        android/googleplaystorebackend.cpp
        android/googleplaystoreproduct.cpp
    )
    list(APPEND PLATFORM_HEADERS
        android/googleplaystorebackend.h
        android/googleplaystoreproduct.h
    )

    set(PLATFORM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/android)

    # Add .java file for visibility in Qt Creator (excluded from build process)
    add_custom_target(GooglePlayJavaSource
        SOURCES
            android/GooglePlayBilling.java
    )
elseif(APPLE)
    find_library(FOUNDATIONLIB Foundation)
    find_library(STOREKITLIB StoreKit)
    list(APPEND PLATFORM_LIBS
        ${FOUNDATIONLIB}
        ${STOREKITLIB}
    )

    list(APPEND PLATFORM_SOURCES
        apple/appleappstorebackend.mm
        apple/appleappstoreproduct.mm
    )
    list(APPEND PLATFORM_HEADERS
        apple/appleappstorebackend.h
        apple/appleappstoreproduct.h
    )

    set(PLATFORM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/apple)

    # Ensure ARC is enabled for Objective-C++ files
    set_source_files_properties(
        apple/appleappstorebackend.mm
        apple/appleappstoreproduct.mm
        PROPERTIES
        COMPILE_FLAGS "-fobjc-arc"
    )
elseif(WIN32)
    # Windows Runtime libraries for Microsoft Store APIs
    find_library(WINDOWSAPPLIB WindowsApp)
    list(APPEND PLATFORM_LIBS ${WINDOWSAPPLIB} oleaut32)

    list(APPEND PLATFORM_SOURCES
        windows/microsoftstorebackend.cpp
        windows/microsoftstoreproduct.cpp
        windows/microsoftstoreworkers.cpp
    )
    list(APPEND PLATFORM_HEADERS
        windows/microsoftstorebackend.h
        windows/microsoftstoreproduct.h
        windows/microsoftstoreworkers.h
    )

    set(PLATFORM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/windows)
endif()

# The public-facing library
set(CORE_SOURCES
    abstractproduct.cpp
    abstractstorebackend.cpp
)
set(CORE_HEADERS
    include/qt6purchasing/abstractproduct.h
    include/qt6purchasing/abstractstorebackend.h
    include/qt6purchasing/transaction.h
)

qt_add_library(qt6purchasinglib STATIC
        ${CORE_SOURCES}
        ${CORE_HEADERS}
        ${PLATFORM_SOURCES}
        ${PLATFORM_HEADERS}
)


qt_add_qml_module(qt6purchasinglib
    URI Qt6Purchasing
    VERSION 1.0
)

target_include_directories(qt6purchasinglib
    PRIVATE
        ${PLATFORM_INCLUDE}
		${CMAKE_CURRENT_SOURCE_DIR}/include/qt6purchasing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qt6purchasinglib
    PRIVATE
        Qt6::Core
        Qt6::Gui
    PUBLIC
        Qt6::Qml
        ${PLATFORM_LIBS}
)

# Set <target>'s QT_ANDROID_PACKAGE_SOURCE_DIR to a copy of <source_dir> with GooglePlayBilling.java added
function(qt6purchasing_set_android_package_source_dir target source_dir)
    if(NOT ANDROID)
        return()
    endif()

    get_filename_component(source_dir "${source_dir}" ABSOLUTE)
    if(NOT IS_DIRECTORY "${source_dir}")
        message(FATAL_ERROR "qt6purchasing_set_android_package_source_dir: ${source_dir} is not a directory")
    endif()

    set(package_dir "${CMAKE_CURRENT_BINARY_DIR}/qt6purchasing-android-package-${target}")

    file(REMOVE_RECURSE "${package_dir}")

    # Rerun CMake when a file in source_dir is edited, added or removed
    file(GLOB_RECURSE app_files CONFIGURE_DEPENDS RELATIVE "${source_dir}" "${source_dir}/*")
    foreach(app_file IN LISTS app_files)
        configure_file("${source_dir}/${app_file}" "${package_dir}/${app_file}" COPYONLY)
    endforeach()

    configure_file(
        "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/android/GooglePlayBilling.java"
        "${package_dir}/src/io/github/moritzstoetter/qt6purchasing/GooglePlayBilling.java"
        COPYONLY
    )

    set_target_properties(${target} PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR "${package_dir}")
endfunction()

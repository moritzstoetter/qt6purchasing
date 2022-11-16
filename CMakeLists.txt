cmake_minimum_required(VERSION 3.16)

project(store)

find_package(Qt6 COMPONENTS
    Core
    Qml
    REQUIRED
)

if(ANDROID)
    set(PLATFORM_STORE_SOURCES
        android/googleplaystorebackend.cpp
        android/googleplaystoreproduct.cpp
        android/googleplaystoretransaction.cpp
    )
    set(PLATFORM_STORE_HEADERS
        ${CMAKE_SOURCE_DIR}/../platform/android/src/com/climbsmart/crimptronic/GooglePlayBilling.java
        android/googleplaystorebackend.h
        android/googleplaystoreproduct.h
        android/googleplaystoretransaction.h
    )
elseif(APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(FOUNDATIONLIB Foundation)
    find_library(STOREKITLIB StoreKit)
    set(PLATFORM_STORE_LIBS
        ${FOUNDATIONLIB}
        ${STOREKITLIB}
    )

    set(PLATFORM_STORE_SOURCES
        apple/appleappstorebackend.mm
        apple/appleappstoreproduct.mm
        apple/appleappstoretransaction.mm
    )
    set(PLATFORM_STORE_HEADERS
        apple/appleappstorebackend.h
        apple/appleappstoreproduct.h
        apple/appleappstoretransaction.h
    )
endif()

add_library(store)
target_include_directories(store PRIVATE ${CMAKE_SOURCE_DIR})
target_sources(store
    PRIVATE
        abstractstorebackend.cpp
        abstractproduct.cpp
        abstracttransaction.cpp
        ${PLATFORM_STORE_SOURCES}
    PUBLIC
        abstractstorebackend.h
        abstractproduct.h
        abstracttransaction.h
        ${PLATFORM_STORE_HEADERS}
)
target_link_libraries(store
    PRIVATE
        Qt6::Core
    PUBLIC
        Qt6::Qml
        ${PLATFORM_STORE_LIBS}
)

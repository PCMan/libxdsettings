# version >= 2.8.6 is needed because of CMAKE_AUTOMOC feature
cmake_minimum_required(VERSION 2.8.6)

set(MAJOR_VERSION	0)
set(MINOR_VERSION	1)
set(PATCH_VERSION	0)
set(XDSETTINGS_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

project(xdsettings)

# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Standard directories for installation
include(GNUInstallDirs)

# FIXME: support Qt5
find_package(Qt4 4.6 REQUIRED QtCore)
include(${QT_USE_FILE})

find_package(PkgConfig)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(X11 REQUIRED x11)

# set visibility to hidden to hide symbols, unlesss they're exporeted manually in the code
set(CMAKE_CXX_FLAGS "-DQT_NO_KEYWORDS -fvisibility=hidden -fno-exceptions")

# Eanble CMake auto-moc support for Qt
set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${QT_INCLUDES}
  ${GLIB_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(xdsettings_SRCS
  xsettings/xsettings-client.c
  xsettings/xsettings-common.c
  desktopsettings.cpp
)

add_definitions(-DCOMPILE_XDSETTINGS)

add_library(xdsettings SHARED ${xdsettings_SRCS})

target_link_libraries(xdsettings
  ${QT_QTCORE_LIBRARY}
  ${GLIB_LIBRARIES}
  ${X11_LIBRARIES}
)

# cmake config file
set(XDSETTINGS_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/xdsettings)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/xdsettings-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xdsettings-config.cmake
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xdsettings-config.cmake DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/xdsettings)
install(FILES cmake/xdsettings_use.cmake DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/xdsettings)

# pkgconfig file
include(create_pkgconfig_file)
create_pkgconfig_file(xdsettings
	"Cross desktop settings"
    "QtCore glib-2.0"
    xdsettings
    ${XDSETTINGS_VERSION}
)

# file soname
set_target_properties(xdsettings PROPERTIES
  VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}
  SOVERSION ${MAJOR_VERSION}
)

install(TARGETS xdsettings DESTINATION ${CMAKE_INSTALL_LIBDIR})

# building tarball with CPack -------------------------------------------------
# To create a source distribution, type:
# make package_source
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
# set (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set (CPACK_PACKAGE_VENDOR "")
set (CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set (CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION})
set (CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include (CPack)

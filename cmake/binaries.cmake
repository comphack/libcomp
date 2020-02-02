# This file is part of COMP_hack.
#
# Copyright (C) 2010-2020 COMP_hack Team <compomega@tutanota.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Add these targets to prevent problems during a build.
ADD_CUSTOM_TARGET(gsl)
ADD_CUSTOM_TARGET(zlib-lib)
ADD_CUSTOM_TARGET(openssl)
ADD_CUSTOM_TARGET(mariadb)
ADD_CUSTOM_TARGET(ttvfs-ex)
ADD_CUSTOM_TARGET(physfs-lib)
ADD_CUSTOM_TARGET(civet)
ADD_CUSTOM_TARGET(squirrel3)
ADD_CUSTOM_TARGET(asio)
ADD_CUSTOM_TARGET(tinyxml2-ex)
ADD_CUSTOM_TARGET(googletest)
ADD_CUSTOM_TARGET(jsonbox-ex)

SET(SOURCE_DIR "${CMAKE_SOURCE_DIR}/binaries/gsl")

SET(GSL_INCLUDE_DIRS "${SOURCE_DIR}/include")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/zlib")

SET(ZLIB_INCLUDES "${INSTALL_DIR}/include")
SET(ZLIB_LIBRARIES zlib)

ADD_LIBRARY(zlib STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(zlib PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/zlibstatic.lib"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/zlibstatic_reldeb.lib"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/zlibstaticd.lib")
ELSE()
    SET_TARGET_PROPERTIES(zlib PROPERTIES
        IMPORTED_LOCATION "${INSTALL_DIR}/lib/libz.a")
ENDIF()

SET_TARGET_PROPERTIES(zlib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDES}")

OPTION(USE_SYSTEM_OPENSSL "Build with the system OpenSSL library." OFF)

IF(USE_SYSTEM_OPENSSL)
    IF(WIN32)
        SET(OPENSSL_USE_STATIC_LIBS TRUE)

        IF(USE_STATIC_RUNTIME)
            SET(OPENSSL_MSVC_STATIC_RT TRUE)
        ENDIF(USE_STATIC_RUNTIME)
    ENDIF(WIN32)

    FIND_PACKAGE(OpenSSL)
ENDIF(USE_SYSTEM_OPENSSL)

IF(OPENSSL_FOUND)
    ADD_LIBRARY(ssl STATIC IMPORTED)
    SET_TARGET_PROPERTIES(ssl PROPERTIES IMPORTED_LOCATION
        "${OPENSSL_SSL_LIBRARY}")
    SET_TARGET_PROPERTIES(ssl PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")

    ADD_LIBRARY(crypto STATIC IMPORTED)
    SET_TARGET_PROPERTIES(crypto PROPERTIES IMPORTED_LOCATION
        "${OPENSSL_CRYPTO_LIBRARY}")
    SET_TARGET_PROPERTIES(crypto PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
ELSE(OPENSSL_FOUND)
    SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/openssl")

    SET(OPENSSL_INCLUDE_DIR "${INSTALL_DIR}/include")
    SET(OPENSSL_ROOT_DIR "${INSTALL_DIR}")

    IF(WIN32)
        SET(OPENSSL_LIBRARIES ssleay32 libeay32 crypt32)
    ELSE()
        SET(OPENSSL_LIBRARIES ssl crypto)
    ENDIF()

    IF(WIN32)
        ADD_LIBRARY(ssleay32 STATIC IMPORTED)
        SET_TARGET_PROPERTIES(ssleay32 PROPERTIES
            IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssleay32${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssleay32_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssleay32d${CMAKE_STATIC_LIBRARY_SUFFIX}")

        SET_TARGET_PROPERTIES(ssleay32 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")

        ADD_LIBRARY(libeay32 STATIC IMPORTED)
        SET_TARGET_PROPERTIES(libeay32 PROPERTIES
            IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}libeay32${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}libeay32_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
            IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}libeay32d${CMAKE_STATIC_LIBRARY_SUFFIX}")

        SET_TARGET_PROPERTIES(libeay32 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
    ELSE()
        ADD_LIBRARY(ssl STATIC IMPORTED)
        SET_TARGET_PROPERTIES(ssl PROPERTIES IMPORTED_LOCATION
            "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}")

        SET_TARGET_PROPERTIES(ssl PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")

        ADD_LIBRARY(crypto STATIC IMPORTED)
        SET_TARGET_PROPERTIES(crypto PROPERTIES IMPORTED_LOCATION
            "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}")

        SET_TARGET_PROPERTIES(crypto PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
    ENDIF()
ENDIF(OPENSSL_FOUND)

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/mariadb")

SET(MARIADB_INCLUDE_DIRS "${INSTALL_DIR}/include/mariadb")

ADD_LIBRARY(mariadbclient STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(mariadbclient PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/mariadb/${CMAKE_STATIC_LIBRARY_PREFIX}mariadbclient${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/mariadb/${CMAKE_STATIC_LIBRARY_PREFIX}mariadbclient_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/mariadb/${CMAKE_STATIC_LIBRARY_PREFIX}mariadbclientd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(mariadbclient PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/mariadb/${CMAKE_STATIC_LIBRARY_PREFIX}mariadbclient${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(mariadbclient PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MARIADB_INCLUDE_DIRS}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/ttvfs")

SET(TTVFS_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(ttvfs STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(ttvfs PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfsd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(ttvfs PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(ttvfs PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${TTVFS_INCLUDE_DIRS}")

ADD_LIBRARY(ttvfs_cfileapi STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(ttvfs_cfileapi PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_cfileapi${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_cfileapi_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_cfileapid${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(ttvfs_cfileapi PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_cfileapi${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(ttvfs_cfileapi PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${TTVFS_INCLUDE_DIRS}")

ADD_LIBRARY(ttvfs_zip STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(ttvfs_zip PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_zip${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_zip_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_zipd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(ttvfs_zip PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ttvfs_zip${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(ttvfs_zip PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${TTVFS_INCLUDE_DIRS}")

SET(TTVFS_GEN_PATH "${INSTALL_DIR}/bin/ttvfs_gen")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/physfs")

SET(PHYSFS_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(physfs STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(physfs PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}physfs${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}physfs_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}physfsd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(physfs PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}physfs${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(physfs PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSFS_INCLUDE_DIRS}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/civet")

SET(CIVETWEB_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(civetweb STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(civetweb PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}civetweb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}civetweb_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}civetwebd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(civetweb PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}civetweb${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(civetweb PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CIVETWEB_INCLUDE_DIRS}")

ADD_LIBRARY(civetweb-cxx STATIC IMPORTED)
ADD_DEPENDENCIES(civetweb-cxx civetweb)

IF(WIN32)
    SET_TARGET_PROPERTIES(civetweb-cxx PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cxx-library${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cxx-library_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cxx-libraryd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(civetweb-cxx PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cxx-library${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(civetweb-cxx PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CIVETWEB_INCLUDE_DIRS}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/squirrel3")

SET(SQUIRREL_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(squirrel STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(squirrel PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}squirrel${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}squirrel_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}squirreld${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(squirrel PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}squirrel${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(squirrel PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
    "${SQUIRREL_INCLUDE_DIRS}")

ADD_LIBRARY(sqstdlib STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(sqstdlib PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqstdlib${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqstdlib_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqstdlibd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(sqstdlib PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqstdlib${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(sqstdlib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
    "${SQUIRREL_INCLUDE_DIRS}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/asio")

SET(ASIO_INCLUDE_DIRS "${INSTALL_DIR}/include")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/tinyxml2")

SET(TINYXML2_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(tinyxml2 STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(tinyxml2 PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2d${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(tinyxml2 PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tinyxml2${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(tinyxml2 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${TINYXML2_INCLUDE_DIRS}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/googletest")

SET(GTEST_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(gtest STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(gtest PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtestd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(gtest PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(gtest PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIRS}")

ADD_LIBRARY(gmock STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(gmock PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmockd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(gmock PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(gmock PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIRS}")

ADD_LIBRARY(gmock_main STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(gmock_main PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_maind${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(gmock_main PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(gmock_main PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIRS}")

SET(GMOCK_DIR "${INSTALL_DIR}")

SET(INSTALL_DIR "${CMAKE_SOURCE_DIR}/binaries/jsonbox")

SET(JSONBOX_INCLUDE_DIRS "${INSTALL_DIR}/include")

ADD_LIBRARY(jsonbox STATIC IMPORTED)

IF(WIN32)
    SET_TARGET_PROPERTIES(jsonbox PROPERTIES
        IMPORTED_LOCATION_RELEASE "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}JsonBox${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_RELWITHDEBINFO "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}JsonBox_reldeb${CMAKE_STATIC_LIBRARY_SUFFIX}"
        IMPORTED_LOCATION_DEBUG "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}JsonBoxd${CMAKE_STATIC_LIBRARY_SUFFIX}")
ELSE()
    SET_TARGET_PROPERTIES(jsonbox PROPERTIES IMPORTED_LOCATION
        "${INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}JsonBox${CMAKE_STATIC_LIBRARY_SUFFIX}")
ENDIF()

SET_TARGET_PROPERTIES(jsonbox PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${JSONBOX_INCLUDE_DIRS}")

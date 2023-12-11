# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.0.1/components/bootloader/subproject"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/tmp"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/src"
  "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Downloads/Documents/231_He Thong Nhung/BTL/Source/udp_client/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_argo_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED argo_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(argo_FOUND FALSE)
  elseif(NOT argo_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(argo_FOUND FALSE)
  endif()
  return()
endif()
set(_argo_CONFIG_INCLUDED TRUE)

# output package information
if(NOT argo_FIND_QUIETLY)
  message(STATUS "Found argo: 0.0.0 (${argo_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'argo' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${argo_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(argo_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${argo_DIR}/${_extra}")
endforeach()

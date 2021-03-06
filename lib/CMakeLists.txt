# Copyright(C) 2012-2013 Brazil
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

add_definitions(
  -DGRN_DAT_EXPORT
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/dat
  ${ONIGMO_INCLUDE_DIRS}
  ${MRUBY_INCLUDE_DIRS}
  ${LIBLZ4_INCLUDE_DIRS})
link_directories(
  ${LIBLZ4_LIBRARY_DIRS})

read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/sources.am LIBGROONGA_SOURCES)
read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/dat/sources.am LIBGRNDAT_SOURCES)
string(REGEX REPLACE "([^;]+)" "dat/\\1"
  LIBGRNDAT_SOURCES "${LIBGRNDAT_SOURCES}")
read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/sources.am LIBGRNMRB_SOURCES)
string(REGEX REPLACE "([^;]+)" "mrb/\\1"
  LIBGRNMRB_SOURCES "${LIBGRNMRB_SOURCES}")

set_source_files_properties(${LIBGROONGA_SOURCES} ${LIBGRNMRB_SOURCES}
  PROPERTIES
  COMPILE_FLAGS "${GRN_C_COMPILE_FLAGS}")
set_source_files_properties(dat.cpp ts.cpp ${LIBGRNDAT_SOURCES}
  PROPERTIES
  COMPILE_FLAGS "${GRN_CXX_COMPILE_FLAGS}")

set(GRN_ALL_SOURCES
  ${LIBGROONGA_SOURCES}
  ${LIBGRNDAT_SOURCES}
  ${LIBGRNMRB_SOURCES})
if(GRN_EMBED)
  add_library(libgroonga STATIC ${GRN_ALL_SOURCES})
  set_target_properties(
    libgroonga
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON)
else()
  add_library(libgroonga SHARED ${GRN_ALL_SOURCES})
endif()
set_target_properties(libgroonga PROPERTIES OUTPUT_NAME "groonga")

set(GRN_ALL_LIBRARIES
    ${EXECINFO_LIBS}
    ${RT_LIBS}
    ${PTHREAD_LIBS}
    ${Z_LIBS}
    ${LIBLZ4_LIBRARIES}
    ${DL_LIBS}
    ${M_LIBS}
    ${WS2_32_LIBS}
    ${MRUBY_LIBS}
    ${ONIGMO_LIBS})
if(GRN_EMBED)
  set(GRN_EMBEDDED_PLUGIN_LIBRARIES "")
  if(GRN_WITH_MECAB)
    list(APPEND GRN_EMBEDDED_PLUGIN_LIBRARIES mecab_tokenizer)
  endif()
  target_link_libraries(libgroonga
    ${GRN_ALL_LIBRARIES}
    ${STDCPP_LIBS}
    ${GRN_EMBEDDED_PLUGIN_LIBRARIES})
else()
  target_link_libraries(libgroonga
    ${GRN_ALL_LIBRARIES})
  install(TARGETS libgroonga
    ARCHIVE DESTINATION "${LIB_DIR}"
    LIBRARY DESTINATION "${LIB_DIR}"
    RUNTIME DESTINATION "${BIN_DIR}")
endif()

if(GRN_WITH_MRUBY)
  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/sources.am
    RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/\\1"
    RUBY_SCRIPTS "${RUBY_SCRIPTS}")
  install(
    FILES ${RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}")

  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/command_line/sources.am
    COMMANE_LINE_RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/command_line/\\1"
    COMMANE_LINE_RUBY_SCRIPTS "${COMMANE_LINE_RUBY_SCRIPTS}")
  install(
    FILES ${COMMANE_LINE_RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}/command_line")

  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/context/sources.am
    CONTEXT_RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/context/\\1"
    CONTEXT_RUBY_SCRIPTS "${CONTEXT_RUBY_SCRIPTS}")
  install(
    FILES ${CONTEXT_RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}/context")

  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/initialize/sources.am
    INITIALIZE_RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/initialize/\\1"
    INITIALIZE_RUBY_SCRIPTS "${INITIALIZE_RUBY_SCRIPTS}")
  install(
    FILES ${INITIALIZE_RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}/initialize")

  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/logger/sources.am
    LOGGER_RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/logger/\\1"
    LOGGER_RUBY_SCRIPTS "${LOGGER_RUBY_SCRIPTS}")
  install(
    FILES ${LOGGER_RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}/logger")

  read_file_list(${CMAKE_CURRENT_SOURCE_DIR}/mrb/scripts/query_logger/sources.am
    QUERY_LOGGER_RUBY_SCRIPTS)
  string(REGEX REPLACE "([^;]+)" "mrb/scripts/query_logger/\\1"
    QUERY_LOGGER_RUBY_SCRIPTS "${QUERY_LOGGER_RUBY_SCRIPTS}")
  install(
    FILES ${QUERY_LOGGER_RUBY_SCRIPTS}
    DESTINATION "${GRN_RELATIVE_RUBY_SCRIPTS_DIR}/query_logger")
endif()

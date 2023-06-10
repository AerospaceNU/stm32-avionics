#[[
MIT License

Copyright (c) 2021 Taylor Braun-Jones

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
]]

# source: https://github.com/nocnokneo/cmake-git-versioning-example/blob/master/GenerateVersionHeader.cmake

if(GIT_EXECUTABLE)
  get_filename_component(SRC_DIR ${SRC} DIRECTORY)
  # Generate a git-describe version string from Git repository tags
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --dirty
    WORKING_DIRECTORY ${SRC_DIR}
    OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
    RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT GIT_DESCRIBE_ERROR_CODE)
    set(GIT_VERSION ${GIT_DESCRIBE_VERSION})
  endif()
  
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log "--pretty=format:'%ci'" -n 1
    WORKING_DIRECTORY ${SRC_DIR}
    OUTPUT_VARIABLE GIT_LOG_STR
    RESULT_VARIABLE GIT_LOG_ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT GIT_LOG_ERROR_CODE)
    set(GIT_LOG ${GIT_LOG_STR})

    # Sometimes this string has leading/trailing quotes. If so, yeet em
    string(REGEX REPLACE "\(^'\\s*\)|\(\\s*'$\)" "" GIT_LOG ${GIT_LOG})
    string(STRIP ${GIT_LOG} GIT_LOG)

  endif()
  
  separate_arguments(GIT_MSG_CMD NATIVE_COMMAND "${GIT_EXECUTABLE} log --pretty=format:'%<(50,trunc)%s' -n 1")
  execute_process(
    COMMAND ${GIT_MSG_CMD}
    WORKING_DIRECTORY ${SRC_DIR}
    OUTPUT_VARIABLE GIT_MSG_STR
    RESULT_VARIABLE GIT_MSG_ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT GIT_MSG_ERROR_CODE)
    set(GIT_MSG ${GIT_MSG_STR})

    # Sometimes this string has leading/trailing quotes. If so, yeet em
    string(REGEX REPLACE "\(^'\\s*\)|\(\\s*'$\)" "" GIT_MSG ${GIT_MSG})
    string(STRIP ${GIT_MSG} GIT_MSG)

  endif()
endif()

# Final fallback: Just use a bogus version string that is semantically older
# than anything else and spit out a warning to the developer.
if(NOT DEFINED GIT_VERSION)
  set(GIT_VERSION v0.0.0-unknown)
  message(WARNING "Failed to determine GIT_VERSION from Git tags. Using default version \"${GIT_VERSION}\".")
endif()

configure_file(${SRC} ${DST} @ONLY ESCAPE_QUOTES)

string(STRIP ${GIT_MSG} GIT_MSG2)

#!/usr/bin/python3

#  #############################################################################
#  Empire V - What you do is what you do
#
## Increment the build number in version.cpp
##
## Use Semantic Versioning 2.0.0:  https://semver.org/
##
## Usage:  Update the version.cpp file
##
## @file      update_version.py
## @author    Mark Nelson <marknels@hawaii.edu>
## @copyright (c) 2023 Mark Nelson.  All rights reserved.
#  #############################################################################

from enum import Enum

## Path to version.cpp in C
VERSION_SOURCE_FILE = "./src/version.cpp"

## Path to the Doxygen version file
DOXYGEN_VERSION_FILE = "./.doxygen/Doxy_version.in"

## Increments with major functional changes
major_version = 0

## Increments with minor functional changes and bugfixes
minor_version = 0

## Increments with bugfixes
patch_version = 0

## Monotonic counter that tracks the number of compilations
build_version = 0


# print("Starting update_version.py")


## Extract an integer from a `line` in a file
##
## If the source file had a line like:
##
## `const_build_number_t VERSION_BUILD { 4 }`
##
## then
##
## `extract_int( "const_build_number_t VERSION_BUILD { ", line)` would return `4` as an `int`.
##
## @param key The keyword to search for (needle)
## @param line The string to be searched (haystack)
## @return The integer that follows the keyword
def extract_int(key: str, line: str) -> int:
    i = line.find(key)  # Find the leading string

    if i == -1:  # If not found, return -1
        return -1

    i2 = i + len(key)  # Get the remaining part of the string

    i3 = int(line[i2:-3])  # Skip the last 3 characters "} :" and convert it to an int

    return i3


## Get the full version number (as a string) from `version.cpp`
##
## @return A string like `1.4.0+2202`
def get_full_version() -> str:
    global major_version
    global minor_version
    global patch_version
    global build_version

    with open(VERSION_SOURCE_FILE, "rt") as versionFile:  # open for reading text
        for aLine in versionFile:  # For each line, read to a string,
            # print(myline)        # and print the string.
            i = extract_int("VERSION_MAJOR { ", aLine)
            if i != -1:
                major_version = i

            i = extract_int("VERSION_MINOR { ", aLine)
            if i != -1:
                minor_version = i

            i = extract_int("VERSION_PATCH { ", aLine)
            if i != -1:
                patch_version = i

            i = extract_int("VERSION_BUILD { ", aLine)
            if i != -1:
                build_version = i

    full_version = str(major_version)
    full_version += "." + str(minor_version)
    full_version += "." + str(patch_version)
    full_version += "+" + str(build_version)

    return full_version


## Create a Doxygen version file
##
## @param full_version The full version number for the Doxygen version file
def write_doxygen_version_file(full_version: str):

    with open(DOXYGEN_VERSION_FILE, "wt") as doxygenVersionFile:  # open for writing text
        doxygenVersionFile.write("PROJECT_NUMBER         = \"" + full_version + "\"")


## A switch to control what to replace in update_version()
class ReplacementMode(Enum):
    ## Replace with just the build number: `2202`
    REPLACE_BUILD_NUMBER = 1
    ## Replace with the full version: `5.0.2+2202`
    REPLACE_FULL_VERSION = 2


## Update the build line in version.cpp
##
## If the old build line was: `VERSION_BUILD { 1045 }`
##
## Then the new build line will be:  `VERSION_BUILD { 1046 }`
##
## This routine rewrites version.cpp
##
## @param find_str The keyword to search for (needle)
## @param replacement_mode What to do if we find the needle
## @param filename The file to be searched (haystack)
## @return Nothing
def update_version(find_str: str, replacement_mode: ReplacementMode, filename: str):
    line_array = []

    with open(filename, "rt") as versionFile:
        for line in versionFile:
            line = line.rstrip()  # Trim whitespace from the end of line
            find_index = line.find(find_str)
            if find_index >= 0 and replacement_mode == ReplacementMode.REPLACE_BUILD_NUMBER:
                old_build = extract_int(find_str, line)
                new_build = old_build + 1
                new_line = line.replace(str(old_build), str(new_build))
                line_array.append(new_line)
            elif find_index >= 0 and replacement_mode == ReplacementMode.REPLACE_FULL_VERSION:
                new_line = line[0:find_index + len(find_str)] + "\"" + get_full_version() + "\" };"
                line_array.append(new_line)
            else:
                line_array.append(line)

    j = 0
    with open(filename, "wt") as version_file:
        while j < len(line_array):
            version_file.write(line_array[j])
            version_file.write("\n")
            j += 1


# The main body
update_version("VERSION_BUILD { ", ReplacementMode.REPLACE_BUILD_NUMBER, VERSION_SOURCE_FILE)
update_version("FULL_VERSION { ", ReplacementMode.REPLACE_FULL_VERSION, VERSION_SOURCE_FILE)

## Get the most-current full version string
full_version_string = get_full_version()

write_doxygen_version_file(full_version_string)
print(full_version_string)

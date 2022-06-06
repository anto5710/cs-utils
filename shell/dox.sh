#!/bin/bash 
#==============================================================================
#                                   dox
#==============================================================================
# Author: Dohyun Ahn
#
#     A simple script to run doxygen with pre-set config file
# 
# ----------------------------------------------------------------------------  
# Usage:
#
#     dox [PROJECT_NAME]
#         - clears and rebuilds doxygen at output directory
#           creates dox configuration file at PWD.
#           by default, LaTeX generation is disabled
#
#         - PROJECT_NAME
#           A new name for documented project.
#           Default is "My Project", and it remains once set.
#
#==============================================================================

DOX_CONFIG_PATH="default.doxs"
DOX_OUTPUT_DIR="dox/"

#==============================================================================
# Checks for default config file, write one if missing
#==============================================================================
if [ -f ${DOX_CONFIG_PATH} ]
then
    echo "dox: found default config: $DOX_CONFIG_PATH"
else
    echo "dox: cannot find default config file: $DOX_CONFIG_PATH,"
    echo "     creating one..."

    doxygen -g ${DOX_CONFIG_PATH}

    # Some default settings
    echo "OUTPUT_DIRECTORY       =\"$DOX_OUTPUT_DIR\"" >> $DOX_CONFIG_PATH
    echo "GENERATE_LATEX         = NO"                 >> $DOX_CONFIG_PATH
    echo "GENERATE_TREEVIEW      = YES"                >> $DOX_CONFIG_PATH
fi

#==============================================================================
# Delete previous output
#==============================================================================
echo "dox: emptying output directory: $DOX_OUTPUT_DIR"
rm -rf ${DOX_OUTPUT_DIR}

#==============================================================================
# Append project name, if a new one is given
#==============================================================================
ARGC="$#"
if [ $ARGC -eq 1 ] 
then 
    echo "PROJECT_NAME           = \"$@\"" >> $DOX_CONFIG_PATH
else 
    echo "dox: assuming previous project name"
fi

#==============================================================================
# Build doxygen in PWD
#==============================================================================
doxygen $DOX_CONFIG_PATH
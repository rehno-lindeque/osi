##############################################################################
#
#     SCONSUTILS.PY
#
#     Copyright (c) 2009 Rehno Lindeque. All rights reserved.
#
#     Version 0.02 (2009-04-05)
#
#     (The versioning on this file is usefull because it may be copied often.
#     Try and keep build scripts as up to date as possible. I.e. Always use 
#     the latest SConsUtils.py file)
#
##############################################################################

import os

def print_info(label, value):
  if(value): print '%s: %s' % (label, value)

# Remove a prefix string from another string. This is usefull for removing 
# the absolute paths from sub-paths)
def remove_prefix(str, prefix):
  str_len = len(str)
  prefix_len = len(prefix)
  return str[prefix_len+1:str_len]

# Expand a directory list to include all sub-directories recursively.
# This is usefull for compiling an entire directory of files as opposed to 
# manually listing the files / directories that need to be compiled
def expand_dirs(relative_dir, base_dir_list):
  if len(base_dir_list) == 0:
    return base_dir_list

  sub_dirs = []

  for dir in base_dir_list:
    abs_dir = os.path.abspath(os.path.join(relative_dir,dir))

    sub_dir_list = [item for item in os.listdir(abs_dir)
                   if os.path.isdir(os.path.join(abs_dir,item))
                     and item != '.svn']
    for sub_dir in sub_dir_list:
      sub_dirs.append(os.path.join(dir,sub_dir))

  sub_dirs = expand_dirs(relative_dir, sub_dirs)

  base_dirs = base_dir_list
  for dir in sub_dirs:
    base_dirs.append(dir)

  return base_dirs

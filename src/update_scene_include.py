#!/usr/bin/env python

import os

dir_root = 'scenes/'
file_include = dir_root+'scenes.hpp'

assert(os.path.isdir(dir_root))

# Get files to include in walking through the root directory
files_to_include = []
for root,dirs,files in os.walk(dir_root):
    for f in files:
        if f.endswith(".hpp"):
            path = root+'/'+f
            fid = open(path)
            content = fid.read()
            if content.find("struct scene_model")!=-1:
                files_to_include.append(path)
            fid.close()



# Create the header file
content = '''
#pragma once

// All scenes must be included in this file
//  If you add a new scene: its corresponding header file must be included
//  This can be done manually or using the automatic script

'''
for f in sorted(files_to_include):
    content += '#include "'+f+'"\n'

N = len(files_to_include)
if N<=0:
    print('No scenes files have been found, something doesn\'t look correct.')
    exit()

# Debug message
print('The following '+str(N)+' scene files have been found')
for f in sorted(files_to_include):
    print('  - '+f.replace(dir_root+'/',""))

# Write the file
fid = open(file_include,'w')
fid.write(content)
fid.close()
assert(os.path.isfile(file_include))

# Debug message
print('\nFile "'+file_include+'" updated\n')

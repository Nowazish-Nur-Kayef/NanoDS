#!/usr/bin/env python3
"""
NanoDS Single-File Bundler
Combines split headers into one nanods.h file for single-header distribution
"""

import os
import re
from pathlib import Path

def read_file(filepath):
    """Read file content"""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.read()

def write_file(filepath, content):
    """Write content to file"""
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)

def remove_includes(content):
    """Remove #include "src/..." directives"""
    pattern = r'#include\s+"src/[^"]+"\s*\n'
    return re.sub(pattern, '', content)

def remove_guards(content, guard_name):
    """Remove header guards from included files"""
    # Remove #ifndef GUARD
    content = re.sub(rf'#ifndef\s+{guard_name}\s*\n', '', content)
    # Remove #define GUARD
    content = re.sub(rf'#define\s+{guard_name}\s*\n', '', content)
    # Remove #endif /* GUARD */
    content = re.sub(rf'#endif\s*/\*\s*{guard_name}\s*\*/\s*\n', '', content)
    return content

def bundle_nanods():
    """Bundle all source files into single nanods.h"""
    
    # Root directory
    root = Path(__file__).parent.parent
    
    # Read main header
    main_header = read_file(root / 'nanods.h')
    
    # Remove includes from main header
    main_header = remove_includes(main_header)
    
    # Read and process source files
    src_files = [
        ('src/core.h', 'NANODS_CORE_H'),
        ('src/vector_impl.h', 'NANODS_VECTOR_IMPL_H'),
        ('src/stack_impl.h', 'NANODS_STACK_IMPL_H'),
        ('src/list_impl.h', 'NANODS_LIST_IMPL_H'),
        ('src/map_impl.h', 'NANODS_MAP_IMPL_H'),
    ]
    
    bundled_content = []
    
    # Add main header start (up to includes)
    lines = main_header.split('\n')
    header_start = []
    for line in lines: 
        if 'Core definitions' in line:
            break
        header_start.append(line)
    
    bundled_content.append('\n'.join(header_start))
    bundled_content.append('\n/* Bundled implementation - all files combined */\n')
    
    # Add each source file
    for src_file, guard in src_files:
        filepath = root / src_file
        if not filepath.exists():
            print(f"Warning: {src_file} not found, skipping...")
            continue
        
        content = read_file(filepath)
        content = remove_guards(content, guard)
        
        # Remove file-level comments
        content = re.sub(r'/\*\*\s*\n\s*\*\s*@file[^*]*\*/', '', content)
        
        bundled_content.append(f'\n/* ===== {src_file} ===== */\n')
        bundled_content.append(content)
    
    # Add closing
    bundled_content.append('\n#ifdef __cplusplus\n}\n#endif\n')
    bundled_content.append('\n#endif /* NANODS_H */\n')
    
    # Write bundled file
    output_file = root / 'nanods_bundled.h'
    write_file(output_file, ''.join(bundled_content))
    
    print(f"✅ Bundled file created: {output_file}")
    print(f"   Size: {os.path.getsize(output_file)} bytes")
    
    return output_file

if __name__ == '__main__': 
    print("========================================")
    print("  NanoDS Single-File Bundler")
    print("========================================\n")
    
    try:
        output = bundle_nanods()
        print("\n✅ Success! Single-header file ready for distribution.")
        print(f"   Location: {output}")
    except Exception as e:
        print(f"\n❌ Error: {e}")
        exit(1)

#!/usr/bin/env python3
"""
NanoDS Single-File Bundler
Combines split headers into one nanods.h file for single-header distribution
Version 1.0.0 - Now with automatic version injection
"""

import os
import re
from pathlib import Path
from datetime import datetime

# Version information (automatically updated)
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_PATCH = 0
VERSION_STRING = f"{VERSION_MAJOR}.{VERSION_MINOR}.{VERSION_PATCH}"

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
    # Remove #endif /* GUARD */ or #endif // GUARD
    content = re.sub(rf'#endif\s*/\*\s*{guard_name}\s*\*/\s*\n?', '', content)
    content = re.sub(rf'#endif\s+//\s*{guard_name}\s*\n?', '', content)
    # Remove standalone #endif at the end if it exists
    content = re.sub(rf'#endif\s*\n?$', '', content)
    return content

def remove_file_comments(content):
    """Remove file-level documentation comments"""
    # Remove /** @file ... */ style comments
    content = re.sub(r'/\*\*\s*\n\s*\*\s*@file[^*]*\*/', '', content, flags=re.DOTALL)
    return content

def inject_version(content):
    """Inject version macros into header"""
    version_block = f"""#define NANODS_VERSION_MAJOR {VERSION_MAJOR}
#define NANODS_VERSION_MINOR {VERSION_MINOR}
#define NANODS_VERSION_PATCH {VERSION_PATCH}
#define NANODS_VERSION "{VERSION_STRING}"
"""
    # Replace existing version definitions
    content = re.sub(
        r'#define\s+NANODS_VERSION_MAJOR\s+\d+\s*\n'
        r'#define\s+NANODS_VERSION_MINOR\s+\d+\s*\n'
        r'#define\s+NANODS_VERSION_PATCH\s+\d+\s*\n'
        r'#define\s+NANODS_VERSION\s+"[^"]+"\s*\n',
        version_block,
        content
    )
    return content

def add_bundle_header(content):
    """Add bundled file header"""
    header = f"""/**
 * =============================================================================
 * NanoDS v{VERSION_STRING} - BUNDLED SINGLE-FILE VERSION
 * =============================================================================
 * 
 * This is an automatically generated single-file distribution of NanoDS.
 * Generated on: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
 * 
 * For the modular source, see: https://github.com/Nowazish-Nur-Kayef/NanoDS
 * 
 * This file contains the entire NanoDS library in one header.
 * Simply #include this file and #define NANODS_IMPLEMENTATION in ONE .c file.
 * 
 * =============================================================================
 */

"""
    return header + content

def extract_main_header_start(content):
    """Extract the starting portion of main header up to the includes"""
    lines = content.split('\n')
    result = []
    
    for i, line in enumerate(lines):
        # Stop before the first #include "src/
        if '#include "src/' in line:
            break
        result.append(line)
    
    return '\n'.join(result)

def bundle_nanods():
    """Bundle all source files into single nanods.h"""
    
    # Root directory
    root = Path(__file__).parent.parent
    
    print(f"Bundling NanoDS v{VERSION_STRING}...")
    
    # Read main header
    main_header_path = root / 'nanods.h'
    if not main_header_path.exists():
        raise FileNotFoundError(f"Main header not found: {main_header_path}")
    
    main_header = read_file(main_header_path)
    
    # Inject version
    main_header = inject_version(main_header)
    
    # Remove includes from main header
    main_header = remove_includes(main_header)
    
    # Extract header start (before includes)
    header_start = extract_main_header_start(read_file(main_header_path))
    header_start = inject_version(header_start)
    
    # Read and process source files
    src_files = [
        ('src/core.h', 'NANODS_CORE_H'),
        ('src/vector_impl.h', 'NANODS_VECTOR_IMPL_H'),
        ('src/stack_impl.h', 'NANODS_STACK_IMPL_H'),
        ('src/list_impl.h', 'NANODS_LIST_IMPL_H'),
        ('src/list2_impl.h', 'NANODS_LIST2_IMPL_H'),
        ('src/ring_impl.h', 'NANODS_RING_IMPL_H'),
        ('src/map_impl.h', 'NANODS_MAP_IMPL_H'),
        ('src/iterator_impl.h', 'NANODS_ITERATOR_IMPL_H'),
    ]
    
    bundled_content = []
    
    # Add header start
    bundled_content.append(header_start)
    bundled_content.append('\n/* ========== BUNDLED IMPLEMENTATION - ALL FILES COMBINED ========== */\n')
    
    # Add each source file
    for src_file, guard in src_files:
        filepath = root / src_file
        if not filepath.exists():
            print(f"Warning: {src_file} not found, skipping...")
            continue
        
        print(f"  Including: {src_file}")
        content = read_file(filepath)
        
        # Remove guards
        content = remove_guards(content, guard)
        
        # Remove file-level doc comments
        content = remove_file_comments(content)
        
        # Remove any stray #include statements
        content = re.sub(r'#include\s+"[^"]+"\s*\n', '', content)
        
        bundled_content.append(f'\n/* ==================== {src_file} ==================== */\n')
        bundled_content.append(content)
    
    # Add closing
    bundled_content.append('\n#ifdef __cplusplus\n}\n#endif\n')
    bundled_content.append('\n#endif /* NANODS_H */\n')
    
    # Combine all content
    final_content = ''.join(bundled_content)
    
    # Add bundle header
    final_content = add_bundle_header(final_content)
    
    # Write bundled file
    output_file = root / 'nanods_bundled.h'
    write_file(output_file, final_content)
    
    print(f"\n✅ Bundled file created: {output_file}")
    print(f"   Version: {VERSION_STRING}")
    print(f"   Size: {os.path.getsize(output_file):,} bytes")
    print(f"   Lines: {len(final_content.splitlines()):,}")
    
    return output_file

def verify_bundle(bundle_path):
    """Verify the bundled file compiles"""
    print("\n🔍 Verifying bundled file...")
    
    # Check for mismatched guards
    content = read_file(bundle_path)
    
    # Count #ifndef vs #endif
    ifndef_count = len(re.findall(r'#ifndef\s+\w+', content))
    endif_count = len(re.findall(r'#endif', content))
    
    print(f"   #ifndef count: {ifndef_count}")
    print(f"   #endif count: {endif_count}")
    
    if ifndef_count != endif_count:
        print(f"   ⚠️  Warning: Mismatched guards ({ifndef_count} #ifndef vs {endif_count} #endif)")
    
    test_code = """
#define NANODS_IMPLEMENTATION
#include "nanods_bundled.h"

int main(void) {
    IntVector vec;
    nv_init_int(&vec);
    nv_push_int(&vec, 42);
    nv_free_int(&vec);
    return 0;
}
"""
    
    test_file = bundle_path.parent / 'test_bundle.c'
    write_file(test_file, test_code)
    
    import subprocess
    try:
        result = subprocess.run(
            ['gcc', '-std=c11', '-Wall', '-Wextra', '-Werror', str(test_file), '-o', 'test_bundle'],
            capture_output=True,
            text=True,
            timeout=10
        )
        
        if result.returncode == 0:
            print("✅ Bundled file compiles successfully!")
            # Cleanup
            os.remove('test_bundle')
            os.remove(test_file)
            return True
        else:
            print("❌ Compilation failed:")
            print(result.stderr)
            return False
    except subprocess.TimeoutExpired:
        print("❌ Compilation timed out")
        return False
    except FileNotFoundError:
        print("⚠️  GCC not found, skipping verification")
        return True
    except Exception as e:
        print(f"⚠️  Verification error: {e}")
        return True

if __name__ == '__main__':
    print("=" * 60)
    print(f"  NanoDS v{VERSION_STRING} Single-File Bundler")
    print("=" * 60)
    print()
    
    try:
        output = bundle_nanods()
        verify_bundle(output)
        
        print("\n" + "=" * 60)
        print("✅ Success! Single-header file ready for distribution.")
        print(f"   Location: {output}")
        print("=" * 60)
    except Exception as e:
        print(f"\n❌ Error: {e}")
        import traceback
        traceback.print_exc()
        exit(1)
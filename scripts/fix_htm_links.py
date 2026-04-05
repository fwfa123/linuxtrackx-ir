#!/usr/bin/env python3
"""
Fix remaining .htm link references in converted markdown files.
This script converts any remaining .htm links to .md format.
"""

import os
import re
from pathlib import Path

def fix_htm_links_in_file(file_path: Path) -> int:
    """Fix .htm links in a single markdown file.
    
    Returns:
        Number of links fixed
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Count original .htm links
        original_htm_count = len(re.findall(r'\.htm', content))
        
        # Fix .htm links to .md
        # Pattern: [text](filename.htm) -> [text](filename.md)
        content = re.sub(r'(\w+)\.htm', r'\1.md', content)
        
        # Count remaining .htm links
        remaining_htm_count = len(re.findall(r'\.htm', content))
        fixed_count = original_htm_count - remaining_htm_count
        
        if fixed_count > 0:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"Fixed {fixed_count} .htm links in: {file_path.name}")
        
        return fixed_count
        
    except Exception as e:
        print(f"Error fixing {file_path}: {e}")
        return 0

def main():
    import argparse
    from wiki_paths import default_wiki_root

    parser = argparse.ArgumentParser(description='Fix .htm links to .md in wiki Markdown')
    parser.add_argument('--wiki-dir', type=Path, default=default_wiki_root(),
                       help='Wiki repo checkout root')
    args = parser.parse_args()
    wiki_dir = args.wiki_dir

    if not wiki_dir.exists():
        print(f"Error: Wiki directory not found: {wiki_dir}")
        return
    
    total_fixed = 0
    files_processed = 0
    
    # Fix all markdown files
    for md_file in wiki_dir.glob('*.md'):
        if md_file.is_file():
            fixed_count = fix_htm_links_in_file(md_file)
            total_fixed += fixed_count
            files_processed += 1
    
    print(f"\nLink fixing completed!")
    print(f"Files processed: {files_processed}")
    print(f"Total links fixed: {total_fixed}")

if __name__ == '__main__':
    main()

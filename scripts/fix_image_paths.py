#!/usr/bin/env python3
"""
Fix image paths in converted markdown files.
This script corrects the double "images/images/" paths to just "images/".
"""

import os
import re
from pathlib import Path

def fix_image_paths_in_file(file_path: Path) -> None:
    """Fix image paths in a single markdown file."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Fix double images/ paths
        content = re.sub(r'!\[([^\]]*)\]\(images/images/([^)]+)\)', r'![\1](images/\2)', content)
        
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        print(f"Fixed image paths in: {file_path.name}")
        
    except Exception as e:
        print(f"Error fixing {file_path}: {e}")

def main():
    wiki_dir = Path('/media/mario/Local_Git/git-repos/linuxtrackx-ir/docs/wiki')
    
    # Fix all markdown files
    for md_file in wiki_dir.glob('*.md'):
        fix_image_paths_in_file(md_file)
    
    print("Image path fixing completed!")

if __name__ == '__main__':
    main()

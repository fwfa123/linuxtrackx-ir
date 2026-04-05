#!/usr/bin/env python3
"""
Simple HTML to Markdown Converter (No External Dependencies)

This script converts HTML help files to Markdown format using only Python standard library.
It handles the specific HTML structure found in the LinuxTrack X-IR help files.

Usage:
    python3 simple_html_to_markdown.py
"""

import os
import re
import shutil
from pathlib import Path
from typing import Dict, List, Tuple, Optional

class SimpleHTMLToMarkdownConverter:
    """Simple HTML to Markdown converter using only standard library."""
    
    def __init__(self, source_dir: str, target_dir: str):
        self.source_dir = Path(source_dir)
        self.target_dir = Path(target_dir)
        self.images_dir = self.target_dir / "images"
        self.image_mappings: Dict[str, str] = {}
        
        # Create target directories
        self.target_dir.mkdir(parents=True, exist_ok=True)
        self.images_dir.mkdir(parents=True, exist_ok=True)
    
    def convert_html_to_markdown(self, html_content: str) -> str:
        """Convert HTML content to Markdown format."""
        # Remove HTML/HEAD/BODY tags but keep content
        html_content = re.sub(r'</?(html|head|body)[^>]*>', '', html_content, flags=re.IGNORECASE)
        
        # Convert images first
        html_content = self._convert_images(html_content)
        
        # Convert tables
        html_content = self._convert_tables(html_content)
        
        # Convert headers
        html_content = re.sub(r'<h1[^>]*>(.*?)</h1>', r'# \1\n', html_content, flags=re.IGNORECASE | re.DOTALL)
        html_content = re.sub(r'<h2[^>]*>(.*?)</h2>', r'## \1\n', html_content, flags=re.IGNORECASE | re.DOTALL)
        html_content = re.sub(r'<h3[^>]*>(.*?)</h3>', r'### \1\n', html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert paragraphs
        html_content = re.sub(r'<p[^>]*>(.*?)</p>', r'\1\n', html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert lists
        html_content = self._convert_lists(html_content)
        
        # Convert links
        html_content = re.sub(r'<a[^>]*href=["\']([^"\']*)["\'][^>]*>(.*?)</a>', r'[\2](\1)', html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert code
        html_content = re.sub(r'<pre[^>]*>(.*?)</pre>', r'```\n\1\n```\n', html_content, flags=re.IGNORECASE | re.DOTALL)
        html_content = re.sub(r'<code[^>]*>(.*?)</code>', r'`\1`', html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert formatting
        html_content = re.sub(r'<strong[^>]*>(.*?)</strong>', r'**\1**', html_content, flags=re.IGNORECASE | re.DOTALL)
        html_content = re.sub(r'<em[^>]*>(.*?)</em>', r'*\1*', html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert line breaks
        html_content = re.sub(r'<br[^>]*>', r'\n', html_content, flags=re.IGNORECASE)
        
        # Clean up
        html_content = self._cleanup_markdown(html_content)
        
        return html_content
    
    def _convert_images(self, html_content: str) -> str:
        """Convert img tags to markdown image syntax."""
        def replace_img(match):
            img_tag = match.group(0)
            
            # Extract src attribute
            src_match = re.search(r'src=["\']([^"\']*)["\']', img_tag, re.IGNORECASE)
            src = src_match.group(1) if src_match else ''
            
            # Extract alt attribute
            alt_match = re.search(r'alt=["\']([^"\']*)["\']', img_tag, re.IGNORECASE)
            alt = alt_match.group(1) if alt_match else ''
            
            if src:
                # Copy image if it exists
                self._copy_image(src)
                
                # Create markdown image syntax
                filename = os.path.basename(src)
                if alt:
                    return f"![{alt}](images/{filename})"
                else:
                    return f"![]({filename})"
            
            return img_tag
        
        return re.sub(r'<img[^>]*>', replace_img, html_content, flags=re.IGNORECASE)
    
    def _copy_image(self, src_path: str) -> None:
        """Copy image file to target images directory."""
        source_path = self.source_dir / src_path
        if source_path.exists():
            target_path = self.images_dir / os.path.basename(src_path)
            if not target_path.exists():
                shutil.copy2(source_path, target_path)
                print(f"Copied image: {src_path} -> {target_path}")
            self.image_mappings[src_path] = str(target_path)
        else:
            print(f"Warning: Image not found: {source_path}")
    
    def _convert_tables(self, html_content: str) -> str:
        """Convert HTML tables to Markdown tables."""
        def convert_table(match):
            table_html = match.group(0)
            
            # Check if this is a single-cell table with an image
            if self._is_single_cell_image_table(table_html):
                return self._convert_single_cell_image_table(table_html)
            
            # Convert to regular markdown table
            return self._convert_regular_table(table_html)
        
        return re.sub(r'<table[^>]*>.*?</table>', convert_table, html_content, flags=re.IGNORECASE | re.DOTALL)
    
    def _is_single_cell_image_table(self, table_html: str) -> bool:
        """Check if table contains only one cell with an image."""
        # Count table rows
        tr_count = len(re.findall(r'<tr[^>]*>', table_html, re.IGNORECASE))
        if tr_count != 1:
            return False
        
        # Count cells in the single row
        td_count = len(re.findall(r'<t[hd][^>]*>', table_html, re.IGNORECASE))
        if td_count != 1:
            return False
        
        # Check if the cell contains an image
        return bool(re.search(r'<img[^>]*>', table_html, re.IGNORECASE))
    
    def _convert_single_cell_image_table(self, table_html: str) -> str:
        """Convert single-cell table with image to centered image block."""
        # Extract the image tag
        img_match = re.search(r'<img[^>]*>', table_html, re.IGNORECASE)
        if img_match:
            img_tag = img_match.group(0)
            
            # Extract src and alt
            src_match = re.search(r'src=["\']([^"\']*)["\']', img_tag, re.IGNORECASE)
            alt_match = re.search(r'alt=["\']([^"\']*)["\']', img_tag, re.IGNORECASE)
            
            src = src_match.group(1) if src_match else ''
            alt = alt_match.group(1) if alt_match else ''
            
            if src:
                # Copy image
                self._copy_image(src)
                
                # Create centered image block
                filename = os.path.basename(src)
                return f"\n![{alt}](images/{filename})\n"
        
        return table_html
    
    def _convert_regular_table(self, table_html: str) -> str:
        """Convert regular table to markdown format."""
        rows = []
        
        # Find all table rows
        tr_matches = re.findall(r'<tr[^>]*>(.*?)</tr>', table_html, re.IGNORECASE | re.DOTALL)
        
        for tr in tr_matches:
            cells = []
            
            # Find all cells in the row
            cell_matches = re.findall(r'<t[hd][^>]*>(.*?)</t[hd]>', tr, re.IGNORECASE | re.DOTALL)
            
            for cell in cell_matches:
                # Clean up cell content
                cell_content = re.sub(r'<[^>]+>', '', cell)  # Remove HTML tags
                cell_content = cell_content.strip()
                cells.append(cell_content)
            
            if cells:
                rows.append('| ' + ' | '.join(cells) + ' |')
        
        if not rows:
            return ""
        
        # Add header separator
        if len(rows) > 1:
            first_row_cells = rows[0].count('|') - 1
            separator = '|' + ' --- |' * first_row_cells
            rows.insert(1, separator)
        
        return '\n'.join(rows) + '\n'
    
    def _convert_lists(self, html_content: str) -> str:
        """Convert HTML lists to Markdown lists."""
        # Convert unordered lists
        def convert_ul(match):
            ul_content = match.group(1)
            items = re.findall(r'<li[^>]*>(.*?)</li>', ul_content, re.IGNORECASE | re.DOTALL)
            markdown_items = []
            for item in items:
                # Clean up item content
                item_content = re.sub(r'<[^>]+>', '', item)
                item_content = item_content.strip()
                if item_content:
                    markdown_items.append(f"- {item_content}")
            return '\n'.join(markdown_items) + '\n'
        
        html_content = re.sub(r'<ul[^>]*>(.*?)</ul>', convert_ul, html_content, flags=re.IGNORECASE | re.DOTALL)
        
        # Convert ordered lists
        def convert_ol(match):
            ol_content = match.group(1)
            items = re.findall(r'<li[^>]*>(.*?)</li>', ol_content, re.IGNORECASE | re.DOTALL)
            markdown_items = []
            for i, item in enumerate(items, 1):
                # Clean up item content
                item_content = re.sub(r'<[^>]+>', '', item)
                item_content = item_content.strip()
                if item_content:
                    markdown_items.append(f"{i}. {item_content}")
            return '\n'.join(markdown_items) + '\n'
        
        html_content = re.sub(r'<ol[^>]*>(.*?)</ol>', convert_ol, html_content, flags=re.IGNORECASE | re.DOTALL)
        
        return html_content
    
    def _cleanup_markdown(self, markdown: str) -> str:
        """Clean up the final markdown content."""
        # Remove any remaining HTML tags
        markdown = re.sub(r'<[^>]+>', '', markdown)
        
        # Clean up multiple newlines
        markdown = re.sub(r'\n\s*\n\s*\n', '\n\n', markdown)
        
        # Clean up whitespace
        markdown = re.sub(r'[ \t]+', ' ', markdown)
        
        # Remove trailing whitespace from lines
        lines = [line.rstrip() for line in markdown.split('\n')]
        markdown = '\n'.join(lines)
        
        return markdown.strip()
    
    def convert_file(self, html_file: Path) -> Optional[Path]:
        """Convert a single HTML file to Markdown."""
        try:
            with open(html_file, 'r', encoding='utf-8', errors='ignore') as f:
                html_content = f.read()
            
            markdown_content = self.convert_html_to_markdown(html_content)
            
            # Create output filename
            md_file = self.target_dir / (html_file.stem + '.md')
            
            with open(md_file, 'w', encoding='utf-8') as f:
                f.write(markdown_content)
            
            print(f"Converted: {html_file.name} -> {md_file.name}")
            return md_file
            
        except Exception as e:
            print(f"Error converting {html_file}: {e}")
            return None
    
    def convert_all_files(self) -> List[Path]:
        """Convert all HTML files in the source directory."""
        converted_files = []
        
        for html_file in self.source_dir.glob('*.htm*'):
            if html_file.is_file():
                converted = self.convert_file(html_file)
                if converted:
                    converted_files.append(converted)
        
        return converted_files
    
    def copy_existing_wiki_files(self, wiki_dir: str) -> None:
        """Copy existing wiki files to the target directory."""
        wiki_path = Path(wiki_dir)
        if not wiki_path.exists():
            print(f"Warning: Wiki directory not found: {wiki_dir}")
            return
        
        for md_file in wiki_path.glob('*.md'):
            if md_file.is_file():
                target_file = self.target_dir / md_file.name
                shutil.copy2(md_file, target_file)
                print(f"Copied existing wiki file: {md_file.name}")
                
                # Update image references
                self._update_image_references(target_file)
    
    def _update_image_references(self, md_file: Path) -> None:
        """Update image references in a markdown file."""
        try:
            with open(md_file, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Update image references to use images/ directory
            content = re.sub(r'!\[([^\]]*)\]\(([^)]*\.png)\)', r'![\1](images/\2)', content)
            
            with open(md_file, 'w', encoding='utf-8') as f:
                f.write(content)
                
        except Exception as e:
            print(f"Error updating image references in {md_file}: {e}")

def main():
    from wiki_paths import default_help_dir, default_wiki_root

    source_dir = str(default_help_dir())
    target_dir = str(default_wiki_root())
    wiki_dir = str(default_wiki_root())
    
    print(f"Converting HTML files from {source_dir} to {target_dir}")
    
    # Create converter
    converter = SimpleHTMLToMarkdownConverter(source_dir, target_dir)
    
    # Convert HTML files
    converted_files = converter.convert_all_files()
    print(f"Converted {len(converted_files)} HTML files")
    
    # Copy existing wiki files
    converter.copy_existing_wiki_files(wiki_dir)
    
    # Print image mapping summary
    if converter.image_mappings:
        print(f"\nImage mappings created:")
        for src, dst in converter.image_mappings.items():
            print(f"  {src} -> {dst}")
    
    print("\nConversion completed!")

if __name__ == '__main__':
    main()

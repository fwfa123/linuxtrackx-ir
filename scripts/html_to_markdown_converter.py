#!/usr/bin/env python3
"""
HTML to Markdown Converter for LinuxTrack X-IR Help Files

This script converts HTML help files to Markdown format while preserving:
- All embedded images with proper references
- Internal and external links
- Table formatting (including complex tables)
- All content structure and formatting

Usage:
    python html_to_markdown_converter.py [--source-dir SOURCE] [--target-dir TARGET] [--copy-images]
"""

import os
import re
import shutil
import argparse
from pathlib import Path
from bs4 import BeautifulSoup
from typing import Dict, List, Tuple, Optional

class HTMLToMarkdownConverter:
    def __init__(self, source_dir: str, target_dir: str, copy_images: bool = True):
        self.source_dir = Path(source_dir)
        self.target_dir = Path(target_dir)
        self.images_dir = self.target_dir / "images"
        self.copy_images = copy_images
        self.image_mappings: Dict[str, str] = {}
        
        # Create target directories
        self.target_dir.mkdir(parents=True, exist_ok=True)
        if self.copy_images:
            self.images_dir.mkdir(parents=True, exist_ok=True)
    
    def convert_html_to_markdown(self, html_content: str) -> str:
        """Convert HTML content to Markdown format."""
        soup = BeautifulSoup(html_content, 'html.parser')
        
        # Remove HTML/HEAD/BODY tags but keep content
        for tag in ['html', 'head', 'body']:
            for element in soup.find_all(tag):
                element.unwrap()
        
        # Convert images first (before other processing)
        self._convert_images(soup)
        
        # Convert tables
        self._convert_tables(soup)
        
        # Convert other HTML elements
        self._convert_headers(soup)
        self._convert_paragraphs(soup)
        self._convert_lists(soup)
        self._convert_links(soup)
        self._convert_code(soup)
        self._convert_formatting(soup)
        self._convert_line_breaks(soup)
        
        # Get the text content
        markdown = str(soup)
        
        # Clean up the markdown
        markdown = self._cleanup_markdown(markdown)
        
        return markdown
    
    def _convert_images(self, soup: BeautifulSoup) -> None:
        """Convert img tags to markdown image syntax."""
        for img in soup.find_all('img'):
            src = img.get('src', '')
            alt = img.get('alt', '')
            
            if src:
                # Handle image copying and path mapping
                if self.copy_images:
                    self._copy_image(src)
                
                # Create markdown image syntax
                if alt:
                    img_markdown = f"![{alt}](images/{os.path.basename(src)})"
                else:
                    img_markdown = f"![]({os.path.basename(src)})"
                
                # Replace the img tag with markdown
                img.replace_with(img_markdown)
    
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
    
    def _convert_tables(self, soup: BeautifulSoup) -> None:
        """Convert HTML tables to Markdown tables."""
        for table in soup.find_all('table'):
            markdown_table = self._convert_single_table(table)
            table.replace_with(markdown_table)
    
    def _convert_single_table(self, table) -> str:
        """Convert a single HTML table to Markdown."""
        rows = []
        
        for tr in table.find_all('tr'):
            cells = []
            for cell in tr.find_all(['td', 'th']):
                # Get cell content and clean it up
                cell_content = str(cell)
                # Remove the td/th tags but keep content
                cell_content = re.sub(r'</?(td|th)[^>]*>', '', cell_content)
                # Clean up any remaining HTML tags
                cell_content = re.sub(r'<[^>]+>', '', cell_content)
                # Strip whitespace
                cell_content = cell_content.strip()
                cells.append(cell_content)
            
            if cells:
                rows.append('| ' + ' | '.join(cells) + ' |')
        
        if not rows:
            return ""
        
        # Add header separator for the first row if it looks like a header
        if len(rows) > 1:
            first_row_cells = rows[0].count('|') - 1
            separator = '|' + ' --- |' * first_row_cells
            rows.insert(1, separator)
        
        return '\n'.join(rows) + '\n'
    
    def _convert_headers(self, soup: BeautifulSoup) -> None:
        """Convert HTML headers to Markdown headers."""
        for i in range(1, 7):
            for header in soup.find_all(f'h{i}'):
                text = header.get_text().strip()
                if text:
                    header.replace_with(f"{'#' * i} {text}\n")
    
    def _convert_paragraphs(self, soup: BeautifulSoup) -> None:
        """Convert paragraph tags."""
        for p in soup.find_all('p'):
            text = p.get_text().strip()
            if text:
                p.replace_with(f"{text}\n")
    
    def _convert_lists(self, soup: BeautifulSoup) -> None:
        """Convert HTML lists to Markdown lists."""
        # Convert unordered lists
        for ul in soup.find_all('ul'):
            list_items = []
            for li in ul.find_all('li'):
                text = li.get_text().strip()
                if text:
                    list_items.append(f"- {text}")
            if list_items:
                ul.replace_with('\n'.join(list_items) + '\n')
        
        # Convert ordered lists
        for ol in soup.find_all('ol'):
            list_items = []
            for i, li in enumerate(ol.find_all('li'), 1):
                text = li.get_text().strip()
                if text:
                    list_items.append(f"{i}. {text}")
            if list_items:
                ol.replace_with('\n'.join(list_items) + '\n')
    
    def _convert_links(self, soup: BeautifulSoup) -> None:
        """Convert HTML links to Markdown links."""
        for a in soup.find_all('a'):
            href = a.get('href', '')
            text = a.get_text().strip()
            
            if href and text:
                # Handle internal links (convert .htm to .md)
                if href.endswith('.htm') or href.endswith('.html'):
                    href = href.replace('.htm', '.md').replace('.html', '.md')
                
                a.replace_with(f"[{text}]({href})")
    
    def _convert_code(self, soup: BeautifulSoup) -> None:
        """Convert code tags to Markdown code blocks."""
        # Convert pre tags to code blocks
        for pre in soup.find_all('pre'):
            text = pre.get_text()
            pre.replace_with(f"```\n{text}\n```\n")
        
        # Convert code tags to inline code
        for code in soup.find_all('code'):
            text = code.get_text()
            code.replace_with(f"`{text}`")
    
    def _convert_formatting(self, soup: BeautifulSoup) -> None:
        """Convert formatting tags."""
        # Convert strong tags to bold
        for strong in soup.find_all('strong'):
            text = strong.get_text()
            strong.replace_with(f"**{text}**")
        
        # Convert em tags to italic
        for em in soup.find_all('em'):
            text = em.get_text()
            em.replace_with(f"*{text}*")
    
    def _convert_line_breaks(self, soup: BeautifulSoup) -> None:
        """Convert br tags to line breaks."""
        for br in soup.find_all('br'):
            br.replace_with('\n')
    
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
                
                # Update image references in copied files
                self._update_image_references(target_file)
    
    def _update_image_references(self, md_file: Path) -> None:
        """Update image references in a markdown file to point to the images directory."""
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
    parser = argparse.ArgumentParser(description='Convert HTML help files to Markdown')
    parser.add_argument('--source-dir', default='/media/mario/Local_Git/git-repos/linuxtrackx-ir/src/qt_gui/help',
                       help='Source directory containing HTML files')
    parser.add_argument('--target-dir', default='/media/mario/Local_Git/git-repos/linuxtrackx-ir/docs/wiki',
                       help='Target directory for Markdown files')
    parser.add_argument('--wiki-dir', default='/media/mario/Local_Git/git-repos/linuxtrackx-ir.wiki',
                       help='Directory containing existing wiki files')
    parser.add_argument('--copy-images', action='store_true', default=True,
                       help='Copy images to target directory')
    parser.add_argument('--no-copy-images', dest='copy_images', action='store_false',
                       help='Do not copy images')
    
    args = parser.parse_args()
    
    # Create converter
    converter = HTMLToMarkdownConverter(
        source_dir=args.source_dir,
        target_dir=args.target_dir,
        copy_images=args.copy_images
    )
    
    print(f"Converting HTML files from {args.source_dir} to {args.target_dir}")
    
    # Convert HTML files
    converted_files = converter.convert_all_files()
    print(f"Converted {len(converted_files)} HTML files")
    
    # Copy existing wiki files
    converter.copy_existing_wiki_files(args.wiki_dir)
    
    # Print image mapping summary
    if converter.image_mappings:
        print(f"\nImage mappings created:")
        for src, dst in converter.image_mappings.items():
            print(f"  {src} -> {dst}")
    
    print("\nConversion completed!")

if __name__ == '__main__':
    main()

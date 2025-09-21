#!/usr/bin/env python3
"""
Image Handler for HTML to Markdown Conversion

This module handles:
- Image discovery and copying
- Image reference mapping
- Image validation and error handling
- Batch image processing
"""

import os
import shutil
import re
from pathlib import Path
from typing import Dict, List, Set, Tuple, Optional
from bs4 import BeautifulSoup

class ImageHandler:
    """Handles all image-related operations for HTML to Markdown conversion."""
    
    def __init__(self, source_dir: Path, target_dir: Path):
        self.source_dir = source_dir
        self.target_dir = target_dir
        self.images_dir = target_dir / "images"
        self.image_mappings: Dict[str, str] = {}
        self.missing_images: List[str] = []
        
        # Create images directory
        self.images_dir.mkdir(parents=True, exist_ok=True)
    
    def discover_images_in_html(self, html_file: Path) -> List[Tuple[str, str]]:
        """Discover all images referenced in an HTML file.
        
        Returns:
            List of tuples (src_path, alt_text)
        """
        images = []
        
        try:
            with open(html_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            soup = BeautifulSoup(content, 'html.parser')
            
            for img in soup.find_all('img'):
                src = img.get('src', '')
                alt = img.get('alt', '')
                
                if src:
                    images.append((src, alt))
        
        except Exception as e:
            print(f"Error reading {html_file}: {e}")
        
        return images
    
    def discover_all_images(self, html_files: List[Path]) -> Dict[str, List[Tuple[str, str]]]:
        """Discover all images in a list of HTML files.
        
        Returns:
            Dictionary mapping file paths to their image references
        """
        all_images = {}
        
        for html_file in html_files:
            images = self.discover_images_in_html(html_file)
            if images:
                all_images[str(html_file)] = images
        
        return all_images
    
    def copy_image(self, src_path: str) -> bool:
        """Copy a single image file to the target directory.
        
        Args:
            src_path: Source image path (relative to source_dir)
            
        Returns:
            True if successful, False otherwise
        """
        source_file = self.source_dir / src_path
        
        if not source_file.exists():
            print(f"Warning: Image not found: {source_file}")
            self.missing_images.append(str(source_file))
            return False
        
        target_file = self.images_dir / source_file.name
        
        try:
            if not target_file.exists():
                shutil.copy2(source_file, target_file)
                print(f"Copied image: {src_path} -> {target_file}")
            
            # Store mapping
            self.image_mappings[src_path] = str(target_file)
            return True
            
        except Exception as e:
            print(f"Error copying {source_file}: {e}")
            return False
    
    def copy_all_images(self, all_images: Dict[str, List[Tuple[str, str]]]) -> None:
        """Copy all discovered images to the target directory."""
        unique_images = set()
        
        # Collect all unique image paths
        for file_images in all_images.values():
            for src_path, _ in file_images:
                unique_images.add(src_path)
        
        # Copy each unique image
        for src_path in unique_images:
            self.copy_image(src_path)
    
    def update_image_references(self, markdown_file: Path) -> None:
        """Update image references in a markdown file to use the images directory."""
        try:
            with open(markdown_file, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Update image references to use images/ directory
            def replace_image_ref(match):
                alt_text = match.group(1)
                img_path = match.group(2)
                
                # If it's already in images/ directory, keep it
                if img_path.startswith('images/'):
                    return match.group(0)
                
                # Otherwise, add images/ prefix
                return f"![{alt_text}](images/{img_path})"
            
            # Pattern to match markdown image syntax
            pattern = r'!\[([^\]]*)\]\(([^)]*\.(png|jpg|jpeg|gif|svg))\)'
            content = re.sub(pattern, replace_image_ref, content)
            
            with open(markdown_file, 'w', encoding='utf-8') as f:
                f.write(content)
                
        except Exception as e:
            print(f"Error updating image references in {markdown_file}: {e}")
    
    def validate_images(self) -> Dict[str, List[str]]:
        """Validate that all referenced images exist and are accessible.
        
        Returns:
            Dictionary with validation results
        """
        results = {
            'valid_images': [],
            'missing_images': [],
            'invalid_references': []
        }
        
        # Check copied images
        for src_path, target_path in self.image_mappings.items():
            if Path(target_path).exists():
                results['valid_images'].append(src_path)
            else:
                results['missing_images'].append(src_path)
        
        # Check for missing images
        results['missing_images'].extend(self.missing_images)
        
        return results
    
    def generate_image_report(self) -> str:
        """Generate a report of image processing results."""
        report = []
        report.append("Image Processing Report")
        report.append("=" * 50)
        report.append(f"Total images processed: {len(self.image_mappings)}")
        report.append(f"Missing images: {len(self.missing_images)}")
        report.append("")
        
        if self.image_mappings:
            report.append("Successfully processed images:")
            for src, dst in self.image_mappings.items():
                report.append(f"  {src} -> {dst}")
            report.append("")
        
        if self.missing_images:
            report.append("Missing images:")
            for img in self.missing_images:
                report.append(f"  {img}")
            report.append("")
        
        return '\n'.join(report)
    
    def copy_images_from_wiki(self, wiki_dir: Path) -> None:
        """Copy images from existing wiki directory."""
        wiki_images_dir = wiki_dir / "images"
        
        if not wiki_images_dir.exists():
            print(f"Warning: Wiki images directory not found: {wiki_images_dir}")
            return
        
        # Copy all images from wiki to target
        for img_file in wiki_images_dir.glob('*'):
            if img_file.is_file() and img_file.suffix.lower() in ['.png', '.jpg', '.jpeg', '.gif', '.svg']:
                target_file = self.images_dir / img_file.name
                if not target_file.exists():
                    shutil.copy2(img_file, target_file)
                    print(f"Copied wiki image: {img_file.name}")
    
    def get_image_statistics(self) -> Dict[str, int]:
        """Get statistics about image processing."""
        return {
            'total_images': len(self.image_mappings),
            'missing_images': len(self.missing_images),
            'images_in_target': len(list(self.images_dir.glob('*'))) if self.images_dir.exists() else 0
        }

# Example usage
if __name__ == '__main__':
    source_dir = Path('/media/mario/Local_Git/git-repos/linuxtrackx-ir/src/qt_gui/help')
    target_dir = Path('/media/mario/Local_Git/git-repos/linuxtrackx-ir/docs/wiki')
    
    handler = ImageHandler(source_dir, target_dir)
    
    # Find all HTML files
    html_files = list(source_dir.glob('*.htm*'))
    
    # Discover images
    all_images = handler.discover_all_images(html_files)
    
    # Copy images
    handler.copy_all_images(all_images)
    
    # Generate report
    print(handler.generate_image_report())

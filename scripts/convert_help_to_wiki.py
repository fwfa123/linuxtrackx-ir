#!/usr/bin/env python3
"""
Complete Help to Wiki Conversion Script

This script performs a complete conversion of HTML help files to Markdown wiki format:
1. Converts all HTML files in src/qt_gui/help/ to Markdown
2. Copies and maps all images to docs/wiki/images/
3. Integrates existing wiki content from linuxtrackx-ir.wiki/
4. Updates all image references and internal links
5. Provides comprehensive reporting

Usage:
    python convert_help_to_wiki.py [--dry-run] [--verbose]
"""

import os
import sys
import argparse
from pathlib import Path
from typing import List, Dict

# Add the scripts directory to the path so we can import our modules
script_dir = Path(__file__).parent
sys.path.insert(0, str(script_dir))

from html_to_markdown_converter import HTMLToMarkdownConverter
from image_handler import ImageHandler
from table_converter import TableConverter

class HelpToWikiConverter:
    """Main converter class that orchestrates the entire conversion process."""
    
    def __init__(self, dry_run: bool = False, verbose: bool = False):
        self.dry_run = dry_run
        self.verbose = verbose
        
        # Define paths
        self.project_root = Path('/media/mario/Local_Git/git-repos/linuxtrackx-ir')
        self.help_dir = self.project_root / 'src' / 'qt_gui' / 'help'
        self.wiki_dir = self.project_root / 'linuxtrackx-ir.wiki'
        self.target_dir = self.project_root / 'docs' / 'wiki'
        
        # Initialize components
        self.image_handler = ImageHandler(self.help_dir, self.target_dir)
        self.converter = HTMLToMarkdownConverter(
            source_dir=str(self.help_dir),
            target_dir=str(self.target_dir),
            copy_images=True
        )
        
        self.conversion_stats = {
            'html_files_processed': 0,
            'markdown_files_created': 0,
            'images_copied': 0,
            'wiki_files_copied': 0,
            'errors': []
        }
    
    def log(self, message: str, level: str = 'INFO') -> None:
        """Log a message with appropriate level."""
        if self.verbose or level in ['ERROR', 'WARNING']:
            print(f"[{level}] {message}")
    
    def discover_html_files(self) -> List[Path]:
        """Discover all HTML files in the help directory."""
        html_files = []
        
        for pattern in ['*.htm', '*.html']:
            html_files.extend(self.help_dir.glob(pattern))
        
        self.log(f"Found {len(html_files)} HTML files to convert")
        return html_files
    
    def discover_wiki_files(self) -> List[Path]:
        """Discover existing wiki files."""
        if not self.wiki_dir.exists():
            self.log(f"Wiki directory not found: {self.wiki_dir}", 'WARNING')
            return []
        
        wiki_files = list(self.wiki_dir.glob('*.md'))
        self.log(f"Found {len(wiki_files)} existing wiki files")
        return wiki_files
    
    def convert_html_files(self) -> List[Path]:
        """Convert all HTML files to Markdown."""
        html_files = self.discover_html_files()
        converted_files = []
        
        for html_file in html_files:
            try:
                self.log(f"Converting {html_file.name}")
                
                if not self.dry_run:
                    converted_file = self.converter.convert_file(html_file)
                    if converted_file:
                        converted_files.append(converted_file)
                        self.conversion_stats['markdown_files_created'] += 1
                else:
                    self.log(f"DRY RUN: Would convert {html_file.name}")
                    converted_files.append(html_file.with_suffix('.md'))
                
                self.conversion_stats['html_files_processed'] += 1
                
            except Exception as e:
                error_msg = f"Error converting {html_file}: {e}"
                self.log(error_msg, 'ERROR')
                self.conversion_stats['errors'].append(error_msg)
        
        return converted_files
    
    def copy_wiki_files(self) -> None:
        """Copy existing wiki files to the target directory."""
        wiki_files = self.discover_wiki_files()
        
        for wiki_file in wiki_files:
            try:
                target_file = self.target_dir / wiki_file.name
                
                if not self.dry_run:
                    import shutil
                    shutil.copy2(wiki_file, target_file)
                    self.log(f"Copied wiki file: {wiki_file.name}")
                    
                    # Update image references
                    self.image_handler.update_image_references(target_file)
                else:
                    self.log(f"DRY RUN: Would copy {wiki_file.name}")
                
                self.conversion_stats['wiki_files_copied'] += 1
                
            except Exception as e:
                error_msg = f"Error copying {wiki_file}: {e}"
                self.log(error_msg, 'ERROR')
                self.conversion_stats['errors'].append(error_msg)
    
    def copy_images_from_wiki(self) -> None:
        """Copy images from existing wiki directory."""
        if not self.dry_run:
            self.image_handler.copy_images_from_wiki(self.wiki_dir)
        else:
            self.log("DRY RUN: Would copy images from wiki directory")
    
    def process_images(self) -> None:
        """Process all images for the conversion."""
        html_files = self.discover_html_files()
        
        # Discover all images
        all_images = self.image_handler.discover_all_images(html_files)
        
        if not self.dry_run:
            # Copy all images
            self.image_handler.copy_all_images(all_images)
            self.conversion_stats['images_copied'] = len(self.image_handler.image_mappings)
        else:
            total_images = sum(len(images) for images in all_images.values())
            self.log(f"DRY RUN: Would copy {total_images} images")
    
    def update_all_image_references(self) -> None:
        """Update image references in all markdown files."""
        if self.dry_run:
            self.log("DRY RUN: Would update image references")
            return
        
        for md_file in self.target_dir.glob('*.md'):
            self.image_handler.update_image_references(md_file)
    
    def generate_conversion_report(self) -> str:
        """Generate a comprehensive conversion report."""
        report = []
        report.append("Help to Wiki Conversion Report")
        report.append("=" * 50)
        report.append(f"HTML files processed: {self.conversion_stats['html_files_processed']}")
        report.append(f"Markdown files created: {self.conversion_stats['markdown_files_created']}")
        report.append(f"Images copied: {self.conversion_stats['images_copied']}")
        report.append(f"Wiki files copied: {self.conversion_stats['wiki_files_copied']}")
        report.append(f"Errors encountered: {len(self.conversion_stats['errors'])}")
        report.append("")
        
        if self.conversion_stats['errors']:
            report.append("Errors:")
            for error in self.conversion_stats['errors']:
                report.append(f"  - {error}")
            report.append("")
        
        # Image statistics
        img_stats = self.image_handler.get_image_statistics()
        report.append("Image Statistics:")
        report.append(f"  Total images processed: {img_stats['total_images']}")
        report.append(f"  Missing images: {img_stats['missing_images']}")
        report.append(f"  Images in target directory: {img_stats['images_in_target']}")
        report.append("")
        
        # Directory structure
        report.append("Target Directory Structure:")
        if self.target_dir.exists():
            for item in sorted(self.target_dir.rglob('*')):
                if item.is_file():
                    rel_path = item.relative_to(self.target_dir)
                    report.append(f"  {rel_path}")
        else:
            report.append("  Target directory not created (dry run)")
        
        return '\n'.join(report)
    
    def run_conversion(self) -> None:
        """Run the complete conversion process."""
        self.log("Starting Help to Wiki conversion process")
        
        # Step 1: Create target directory
        if not self.dry_run:
            self.target_dir.mkdir(parents=True, exist_ok=True)
            (self.target_dir / 'images').mkdir(parents=True, exist_ok=True)
        
        # Step 2: Copy images from existing wiki
        self.copy_images_from_wiki()
        
        # Step 3: Process images from HTML files
        self.process_images()
        
        # Step 4: Convert HTML files
        self.convert_html_files()
        
        # Step 5: Copy existing wiki files
        self.copy_wiki_files()
        
        # Step 6: Update image references
        self.update_all_image_references()
        
        # Step 7: Generate report
        report = self.generate_conversion_report()
        print("\n" + report)
        
        # Save report to file
        if not self.dry_run:
            report_file = self.target_dir / 'conversion_report.txt'
            with open(report_file, 'w', encoding='utf-8') as f:
                f.write(report)
            self.log(f"Conversion report saved to: {report_file}")
        
        self.log("Conversion process completed!")

def main():
    parser = argparse.ArgumentParser(description='Convert HTML help files to Markdown wiki format')
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be done without making changes')
    parser.add_argument('--verbose', action='store_true',
                       help='Show detailed output')
    
    args = parser.parse_args()
    
    # Create and run converter
    converter = HelpToWikiConverter(dry_run=args.dry_run, verbose=args.verbose)
    converter.run_conversion()

if __name__ == '__main__':
    main()

#!/usr/bin/env python3
"""
Specialized Table Converter for HTML to Markdown Conversion

This module handles complex HTML table conversions, including:
- Single-cell tables with images (convert to centered image blocks)
- Multi-column tables with mixed content
- Tables with nested HTML elements
- Proper Markdown table formatting
"""

import re
from bs4 import BeautifulSoup
from typing import List, Tuple, Optional

class TableConverter:
    """Handles conversion of HTML tables to Markdown format."""
    
    @staticmethod
    def convert_table(table_element) -> str:
        """Convert an HTML table element to Markdown format."""
        rows = []
        
        # Extract all rows
        for tr in table_element.find_all('tr'):
            cells = TableConverter._extract_cells(tr)
            if cells:
                rows.append(cells)
        
        if not rows:
            return ""
        
        # Check if this is a single-cell table with an image
        if len(rows) == 1 and len(rows[0]) == 1:
            cell_content = rows[0][0]
            if TableConverter._is_image_cell(cell_content):
                return TableConverter._convert_image_cell(cell_content)
        
        # Convert to Markdown table
        return TableConverter._format_markdown_table(rows)
    
    @staticmethod
    def _extract_cells(tr_element) -> List[str]:
        """Extract cell content from a table row."""
        cells = []
        
        for cell in tr_element.find_all(['td', 'th']):
            # Get the cell content as HTML first
            cell_html = str(cell)
            
            # Remove the td/th wrapper tags
            cell_content = re.sub(r'</?(td|th)[^>]*>', '', cell_html)
            
            # Clean up any remaining HTML tags but preserve content
            cell_content = TableConverter._clean_cell_content(cell_content)
            
            # Strip whitespace
            cell_content = cell_content.strip()
            
            cells.append(cell_content)
        
        return cells
    
    @staticmethod
    def _clean_cell_content(content: str) -> str:
        """Clean up cell content while preserving important formatting."""
        # Handle images in cells
        content = re.sub(r'<img[^>]*src=["\']([^"\']+)["\'][^>]*alt=["\']([^"\']*)["\'][^>]*>', 
                        r'![\2](\1)', content)
        
        # Handle links
        content = re.sub(r'<a[^>]*href=["\']([^"\']+)["\'][^>]*>(.*?)</a>', 
                        r'[\2](\1)', content)
        
        # Handle basic formatting
        content = re.sub(r'<strong>(.*?)</strong>', r'**\1**', content)
        content = re.sub(r'<em>(.*?)</em>', r'*\1*', content)
        content = re.sub(r'<code>(.*?)</code>', r'`\1`', content)
        
        # Remove any remaining HTML tags
        content = re.sub(r'<[^>]+>', '', content)
        
        return content
    
    @staticmethod
    def _is_image_cell(content: str) -> bool:
        """Check if a cell contains only an image."""
        # Look for image markdown syntax
        return bool(re.search(r'!\[.*?\]\(.*?\)', content))
    
    @staticmethod
    def _convert_image_cell(content: str) -> str:
        """Convert a single-cell table with image to centered image block."""
        # Extract the image markdown
        img_match = re.search(r'!\[(.*?)\]\((.*?)\)', content)
        if img_match:
            alt_text = img_match.group(1)
            img_path = img_match.group(2)
            
            # Create a centered image block
            return f"\n![{alt_text}]({img_path})\n"
        
        return content
    
    @staticmethod
    def _format_markdown_table(rows: List[List[str]]) -> str:
        """Format rows as a Markdown table."""
        if not rows:
            return ""
        
        # Calculate column count
        max_cols = max(len(row) for row in rows) if rows else 0
        
        # Pad rows to have the same number of columns
        padded_rows = []
        for row in rows:
            padded_row = row + [''] * (max_cols - len(row))
            padded_rows.append(padded_row)
        
        # Format rows
        formatted_rows = []
        for row in padded_rows:
            # Escape pipe characters in cell content
            escaped_row = [cell.replace('|', '\\|') for cell in row]
            formatted_rows.append('| ' + ' | '.join(escaped_row) + ' |')
        
        # Add header separator
        if len(formatted_rows) > 0:
            separator = '|' + ' --- |' * max_cols
            formatted_rows.insert(1, separator)
        
        return '\n'.join(formatted_rows) + '\n'
    
    @staticmethod
    def convert_complex_table(html_content: str) -> str:
        """Convert complex HTML table content to Markdown."""
        soup = BeautifulSoup(html_content, 'html.parser')
        
        # Find all tables
        tables = soup.find_all('table')
        
        for table in tables:
            markdown_table = TableConverter.convert_table(table)
            table.replace_with(markdown_table)
        
        return str(soup)

# Example usage and testing
if __name__ == '__main__':
    # Test with sample HTML table
    test_html = """
    <table>
    <tr>
    <td>
    <img src="Trackir.png" alt="TrackIR setup.">
    </td>
    </tr>
    </table>
    """
    
    converter = TableConverter()
    result = converter.convert_complex_table(test_html)
    print("Converted table:")
    print(result)

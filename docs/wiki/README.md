# LinuxTrack X-IR Documentation

This directory contains comprehensive documentation for LinuxTrack X-IR in both Qt Help System format (HTML files) and GitHub/GitLab wiki format (Markdown files).

## Documentation Structure

### HTML Files (Qt Help System)
Located in: `src/qt_gui/help/`

These HTML files are designed to work with Qt's built-in help system and are integrated into the LinuxTrack X-IR application. They provide context-sensitive help within the application interface.

### Markdown Files (GitHub/GitLab Wiki)
Located in: `docs/wiki/`

These markdown files are formatted for use as GitHub/GitLab wiki pages and provide the same information in a web-friendly format. They can be used to create comprehensive online documentation.

## File Mapping

| HTML File | Markdown File | Purpose |
|-----------|---------------|---------|
| `welcome.htm` | `Welcome.md` | Introduction and overview |
| `news.htm` | `News.md` | Recent changes and updates |
| `dev_setup.htm` | `Device-Setup.md` | Hardware setup and configuration |
| `interface.htm` | `Interface.md` | Developer API documentation |
| `misc.htm` | `System.md` | System information and troubleshooting |
| `extractor.htm` | (Not converted) | TrackIR firmware extraction |
| `wine.htm` | (Referenced in Gaming-Integration.md) | Wine integration details |
| `xpl_setup.htm` | (Not converted) | X-Plane plugin setup |
| `model_setup.htm` | (Not converted) | Model configuration |
| `axes_setup.htm` | (Not converted) | Tracking axes configuration |

## Using the Documentation

### In LinuxTrack X-IR Application
1. Launch the LinuxTrack X-IR GUI application
2. Navigate to **Help** → **Contents** in the menu bar
3. Browse through the help topics using the table of contents
4. Use the search functionality to find specific information

### Online (GitHub/GitLab Wiki)
1. Visit the [GitLab Project Page](https://gitlab.com/fwfa123/linuxtrackx-ir) or [GitHub Mirror](https://github.com/fwfa123/linuxtrackx-ir)
2. Navigate to the **Wiki** section
3. Browse the documentation pages for comprehensive information
4. Use the search functionality to find specific topics

### Local Markdown Files
1. Navigate to `docs/wiki/` in your local repository
2. Open any `.md` file in a markdown viewer or editor
3. Use the cross-references between files for navigation

## Navigation

The documentation is organized with cross-references for easy navigation:

- **[Welcome](Welcome.md)** - Getting started and overview
- **[News](News.md)** - Recent changes and feature updates
- **[Device Setup](Device-Setup.md)** - Hardware configuration guide
- **[Gaming Platform Integration](Gaming-Integration.md)** - Wine Bridge and gaming platform support
- **[System](System.md)** - System information and troubleshooting
- **[Interface](Interface.md)** - Developer API documentation

## Content Updates

The documentation is maintained in sync between HTML and markdown formats:

- **HTML files** (`src/qt_gui/help/`) - Used by Qt Help System within the application
- **Markdown files** (`docs/wiki/`) - Used for GitHub/GitLab wiki pages

When making updates, ensure both formats are updated to maintain consistency.

## Contributing to Documentation

To contribute to the documentation:

1. **For HTML files**: Edit the files in `src/qt_gui/help/` and ensure they remain valid HTML
2. **For Markdown files**: Edit the files in `docs/wiki/` and ensure they follow GitHub/GitLab markdown syntax
3. **Cross-references**: Update links in both formats when adding new documentation
4. **Testing**: Verify that HTML files work in Qt Help System and markdown files render correctly on GitHub/GitLab

## Technical Details

### Qt Help System
- Uses Qt's QHelpEngine for integration
- HTML files are compiled into .qch (Qt Compressed Help) files
- Context-sensitive help is available through F1 key or Help menu

### GitHub/GitLab Wiki
- Standard markdown format with GitHub/GitLab extensions
- Automatic table of contents generation
- Cross-page linking with relative URLs
- Image support with relative paths

### Build Integration
The documentation files are integrated into the build process:

```bash
# Generate Qt Help files
make help

# Build with documentation
make && sudo make install
```

## Troubleshooting

### Qt Help System Issues
- Ensure Qt5 development packages are installed
- Check that HTML files are valid
- Verify help collection files (.qhc/.qch) are generated correctly

### Wiki Rendering Issues
- Verify markdown syntax is correct
- Check relative links between pages
- Ensure images are accessible
- Test on both GitHub and GitLab platforms

### Missing Content
- Check that both HTML and markdown versions exist
- Verify cross-references are updated
- Ensure content is synchronized between formats

## Version History

- **v0.99.25** (September 2025): Updated for latest features, converted to dual-format documentation
- **v0.99.24** (2025): Added gaming platform integration documentation
- **v0.99.23** (2025): Added AppImage and help system documentation
- **v0.99.22** (2025): Comprehensive Qt5 modernization documentation

---

For additional help:

- [GitLab Issues](https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues) - Report documentation issues
- [Community Forums](https://forums.x-plane.org/index.php?act=idx) - Get help from the community
- [Technical Documentation](../technical/) - Detailed technical guides

# GitHub Wiki Setup Instructions

This directory contains the prepared files for the LinuxTrack X-IR GitHub wiki at https://github.com/fwfa123/linuxtrackx-ir/wiki

## Files Structure

The GitHub wiki has been structured according to the QHP file order:

- `_Sidebar.md` - Navigation sidebar (matches QHP structure)
- `Home.md` - Main page (using dev_setup.md content as requested)
- `news.md` - What's new page
- `hall_of_fame.md` - Hall of Fame page
- `dev_setup.md` - Tracker Setup page
- `trackir_permissions.md` - TrackIR Permissions page
- `model_setup.md` - Model Setup page
- `axes_setup.md` - Tracking Setup page
- `extractor.md` - TrackIR firmware and Wine integration page
- `gaming_tab.md` - Gaming Tab page
- `xpl_setup.md` - XPlane plugin setup page
- `interface.md` - Linuxtrack Interface page
- `wine.md` - Wine games support page
- `mickey.md` - Virtual mouse setup page
- `misc.md` - System Information page

## Images Directory

The `images/` directory contains all the images referenced in the wiki pages. These need to be uploaded to the GitHub wiki as well.

## Upload Instructions

### Method 1: Using GitHub Web Interface

1. Go to https://github.com/fwfa123/linuxtrackx-ir/wiki
2. Click "New Page" for each file
3. Copy the content from each .md file
4. Use the filename as the page title (without .md extension)
5. For images, use the "Attach files" button to upload images from the images/ directory

### Method 2: Using Git Clone (Recommended)

1. Clone the wiki repository:
   ```bash
   git clone https://github.com/fwfa123/linuxtrackx-ir.wiki.git
   ```

2. Copy all files from this directory to the cloned wiki:
   ```bash
   cp -r /media/mario/Local_Git/git-repos/linuxtrackx-ir/docs/wiki/github_wiki/* /path/to/linuxtrackx-ir.wiki/
   ```

3. Commit and push:
   ```bash
   cd /path/to/linuxtrackx-ir.wiki
   git add .
   git commit -m "Initial wiki setup with complete documentation"
   git push origin main
   ```

## Important Notes

1. **Home Page**: The `Home.md` file is set as the main page and contains the Tracker Setup content as requested.

2. **Sidebar**: The `_Sidebar.md` file provides navigation that matches the QHP structure exactly.

3. **Image Paths**: All image references use relative paths (`images/filename.png`) which should work correctly in GitHub wiki.

4. **Cross-References**: Internal links have been updated to use GitHub wiki format (page names without .md extension).

5. **Replacement**: This new wiki is intended to replace the existing GitLab wiki at https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home

## Verification

After uploading, verify that:
- All pages load correctly
- Images display properly
- Navigation works in the sidebar
- Cross-references between pages work
- The Home page displays the Tracker Setup content as the main entry point

## Maintenance

To update the wiki in the future:
1. Make changes to the files in this directory
2. Follow the upload instructions above
3. The wiki will automatically update on GitHub

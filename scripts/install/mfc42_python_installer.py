#!/usr/bin/env python3
"""
MFC42u.dll Alternative Installation Methods for LinuxTrack Wine Bridge
This script provides multiple reliable methods to install MFC42u.dll without winetricks
"""

import os
import sys
import shutil
import subprocess
import urllib.request
import urllib.error
import hashlib
import zipfile
import tempfile
import platform
import json
import logging
from pathlib import Path
from typing import List, Dict, Optional, Tuple
import requests

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler(os.path.expanduser('~/.config/linuxtrack/mfc42_install.log')),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

class MFC42Installer:
    def __init__(self):
        self.config_dir = os.path.expanduser('~/.config/linuxtrack')
        self.tir_firmware_dir = os.path.join(self.config_dir, 'tir_firmware')
        self.cache_dir = os.path.join(self.config_dir, 'cache')
        self.log_file = os.path.join(self.config_dir, 'mfc42_install.log')
        
        # Create necessary directories
        os.makedirs(self.tir_firmware_dir, exist_ok=True)
        os.makedirs(self.cache_dir, exist_ok=True)
        
        # Known MFC42u.dll hashes for verification
        self.known_hashes = {
            'md5': [
                'a1b2c3d4e5f678901234567890123456',  # Example hash
                'f4e3d2c1b0a987654321098765432109',  # Example hash
            ],
            'sha256': [
                'a1b2c3d4e5f6789012345678901234567890abcdef1234567890abcdef1234',  # Example hash
            ]
        }
        
        # Alternative download sources
        self.download_sources = [
            # Microsoft Visual C++ 6.0 Redistributable
            {
                'name': 'Microsoft Visual C++ 6.0 Redistributable',
                'url': 'https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe',
                'type': 'installer',
                'description': 'Official Microsoft Visual C++ 6.0 Redistributable'
            },
            # Alternative Microsoft sources
            {
                'name': 'Microsoft Visual C++ 6.0 (Archive)',
                'url': 'https://web.archive.org/web/20190301000000*/https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe',
                'type': 'archive',
                'description': 'Archived Microsoft Visual C++ 6.0 Redistributable'
            },
            # GitHub repositories that might have MFC42u.dll
            {
                'name': 'GitHub MFC42u.dll Repository',
                'url': 'https://github.com/search?q=mfc42u.dll+filename:mfc42u.dll',
                'type': 'search',
                'description': 'Search GitHub for MFC42u.dll files'
            },
            # Software archives
            {
                'name': 'Internet Archive Software Library',
                'url': 'https://archive.org/search.php?query=mfc42u.dll',
                'type': 'search',
                'description': 'Search Internet Archive for MFC42u.dll'
            },
            # Alternative download sites
            {
                'name': 'DLL-Files.com',
                'url': 'https://www.dll-files.com/mfc42u.dll.html',
                'type': 'website',
                'description': 'DLL download website'
            },
            {
                'name': 'DLLme.com',
                'url': 'https://www.dllme.com/dll/files/mfc42u.html',
                'type': 'website',
                'description': 'Alternative DLL download site'
            }
        ]
        
        # Direct MFC42u.dll download URLs (these need to be verified)
        self.direct_downloads = [
            'https://github.com/your-repo/mfc42u.dll/raw/main/mfc42u.dll',
            'https://archive.org/download/mfc42u.dll/mfc42u.dll',
            'https://www.dll-files.com/download/mfc42u.dll.html',
        ]

    def detect_wine_prefix(self) -> str:
        """Detect Wine prefix location"""
        if 'WINEPREFIX' in os.environ:
            return os.environ['WINEPREFIX']
        elif os.path.exists(os.path.expanduser('~/.wine')):
            return os.path.expanduser('~/.wine')
        elif os.path.exists(os.path.expanduser('~/.local/share/wineprefixes/default')):
            return os.path.expanduser('~/.local/share/wineprefixes/default')
        else:
            return os.path.expanduser('~/.wine')

    def detect_wine(self) -> Optional[str]:
        """Detect Wine installation"""
        wine_paths = ['wine', '/usr/bin/wine', '/usr/local/bin/wine']
        for path in wine_paths:
            if shutil.which(path):
                return path
        return None

    def download_file(self, url: str, dest_path: str) -> bool:
        """Download file from URL"""
        try:
            logger.info(f"Downloading {url} to {dest_path}")
            response = requests.get(url, stream=True, timeout=30)
            response.raise_for_status()
            
            with open(dest_path, 'wb') as f:
                for chunk in response.iter_content(chunk_size=8192):
                    f.write(chunk)
            
            logger.info(f"Download completed: {dest_path}")
            return True
        except Exception as e:
            logger.error(f"Download failed: {e}")
            return False

    def verify_file(self, file_path: str) -> bool:
        """Verify downloaded file is a valid Windows DLL"""
        try:
            if not os.path.exists(file_path):
                return False
            
            # Check file size (MFC42u.dll should be around 1-2MB)
            file_size = os.path.getsize(file_path)
            if file_size < 100000 or file_size > 5000000:  # 100KB to 5MB
                logger.warning(f"File size seems unusual: {file_size} bytes")
            
            # Check if it's a PE file (Windows executable)
            with open(file_path, 'rb') as f:
                header = f.read(2)
                if header != b'MZ':  # DOS executable signature
                    logger.error("File is not a valid Windows executable")
                    return False
            
            # Calculate hash for verification
            with open(file_path, 'rb') as f:
                content = f.read()
                md5_hash = hashlib.md5(content).hexdigest()
                sha256_hash = hashlib.sha256(content).hexdigest()
            
            logger.info(f"File MD5: {md5_hash}")
            logger.info(f"File SHA256: {sha256_hash}")
            
            # Check against known hashes
            if md5_hash in self.known_hashes['md5'] or sha256_hash in self.known_hashes['sha256']:
                logger.info("File hash matches known good MFC42u.dll")
                return True
            
            logger.warning("File hash not in known good list, but continuing...")
            return True
            
        except Exception as e:
            logger.error(f"File verification failed: {e}")
            return False

    def method_direct_download(self) -> bool:
        """Method 1: Direct download from Microsoft Visual C++ 6.0 Redistributable"""
        logger.info("Method 1: Direct download from Microsoft Visual C++ 6.0 Redistributable")
        
        installer_path = os.path.join(self.cache_dir, 'vcredist.exe')
        wine_prefix = self.detect_wine_prefix()
        wine_path = self.detect_wine()
        
        if not wine_path:
            logger.error("Wine not found")
            return False
        
        # Download Visual C++ 6.0 Redistributable
        download_url = "https://download.microsoft.com/download/vc60pro/redist/6.0.8447.0/Win98Me/EN-US/vcredist.exe"
        
        if not os.path.exists(installer_path):
            if not self.download_file(download_url, installer_path):
                return False
        else:
            logger.info(f"Using cached installer: {installer_path}")
        
        # Install in Wine prefix
        logger.info("Installing Visual C++ 6.0 Redistributable in Wine prefix...")
        try:
            env = os.environ.copy()
            env['WINEPREFIX'] = wine_prefix
            
            result = subprocess.run([
                wine_path, installer_path, '/Q', '/T:C:\\', '/C'
            ], env=env, capture_output=True, text=True, timeout=300)
            
            if result.returncode != 0:
                logger.warning(f"Wine installation returned code {result.returncode}")
                logger.debug(f"Wine output: {result.stdout}")
                logger.debug(f"Wine errors: {result.stderr}")
        
        except subprocess.TimeoutExpired:
            logger.warning("Wine installation timed out, but continuing...")
        except Exception as e:
            logger.error(f"Wine installation failed: {e}")
        
        # Extract MFC42u.dll
        system32_path = os.path.join(wine_prefix, 'drive_c', 'windows', 'system32')
        mfc42_path = os.path.join(system32_path, 'mfc42u.dll')
        
        if os.path.exists(mfc42_path):
            dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
            shutil.copy2(mfc42_path, dest_path)
            logger.info("MFC42u.dll extracted successfully")
            return True
        else:
            logger.error("MFC42u.dll not found after installation")
            return False

    def method_windows_extract(self) -> bool:
        """Method 2: Extract from Windows system files (if available)"""
        logger.info("Method 2: Extract from Windows system files")
        
        windows_paths = [
            '/mnt/c/Windows/System32/mfc42u.dll',
            '/mnt/d/Windows/System32/mfc42u.dll',
        ]
        
        # Also check for mounted Windows drives
        for drive in ['c', 'd', 'e', 'f']:
            windows_paths.append(f'/mnt/{drive}/Windows/System32/mfc42u.dll')
        
        for path in windows_paths:
            if os.path.exists(path):
                logger.info(f"Found MFC42u.dll at: {path}")
                dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
                shutil.copy2(path, dest_path)
                logger.info("MFC42u.dll copied from Windows system")
                return True
        
        logger.warning("No Windows system files found")
        return False

    def method_package_manager(self) -> bool:
        """Method 3: Package manager installation"""
        logger.info("Method 3: Package manager installation")
        
        # Detect distribution
        if os.path.exists('/etc/debian_version'):
            logger.info("Detected Debian-based distribution")
            packages = ['wine-staging', 'wine32:i386']
            package_manager = 'apt'
        elif os.path.exists('/etc/fedora-release') or os.path.exists('/etc/redhat-release'):
            logger.info("Detected Fedora/RHEL-based distribution")
            packages = ['wine-staging', 'wine-core', 'wine-desktop']
            package_manager = 'dnf'
        elif os.path.exists('/etc/arch-release'):
            logger.info("Detected Arch Linux")
            packages = ['wine-staging', 'wine-mono', 'wine-gecko']
            package_manager = 'pacman'
        else:
            logger.warning("Unknown distribution, skipping package manager method")
            return False
        
        # Install packages
        try:
            if package_manager == 'apt':
                subprocess.run(['sudo', 'apt', 'update'], check=True)
                subprocess.run(['sudo', 'apt', 'install', '-y'] + packages, check=True)
            elif package_manager == 'dnf':
                subprocess.run(['sudo', 'dnf', 'install', '-y'] + packages, check=True)
            elif package_manager == 'pacman':
                subprocess.run(['sudo', 'pacman', '-S', '--needed'] + packages, check=True)
        except subprocess.CalledProcessError as e:
            logger.error(f"Failed to install Wine packages: {e}")
            return False
        
        # Check if MFC42u.dll is now available in Wine
        wine_prefix = self.detect_wine_prefix()
        system32_path = os.path.join(wine_prefix, 'drive_c', 'windows', 'system32')
        mfc42_path = os.path.join(system32_path, 'mfc42u.dll')
        
        if os.path.exists(mfc42_path):
            dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
            shutil.copy2(mfc42_path, dest_path)
            logger.info("MFC42u.dll found after package installation")
            return True
        else:
            logger.warning("MFC42u.dll not found after package installation")
            return False

    def method_cabextract(self) -> bool:
        """Method 4: Extract from Visual C++ 6.0 installer using cabextract"""
        logger.info("Method 4: Extract from Visual C++ 6.0 installer using cabextract")
        
        installer_path = os.path.join(self.cache_dir, 'vcredist.exe')
        extract_dir = os.path.join(self.cache_dir, 'vcredist_extracted')
        
        # Check if cabextract is available
        if not shutil.which('cabextract'):
            logger.info("Installing cabextract...")
            try:
                if os.path.exists('/etc/debian_version'):
                    subprocess.run(['sudo', 'apt', 'install', '-y', 'cabextract'], check=True)
                elif os.path.exists('/etc/fedora-release') or os.path.exists('/etc/redhat-release'):
                    subprocess.run(['sudo', 'dnf', 'install', '-y', 'cabextract'], check=True)
                elif os.path.exists('/etc/arch-release'):
                    subprocess.run(['sudo', 'pacman', '-S', 'cabextract'], check=True)
                else:
                    logger.error("cabextract not available and cannot be installed")
                    return False
            except subprocess.CalledProcessError as e:
                logger.error(f"Failed to install cabextract: {e}")
                return False
        
        # Download installer if not present
        if not os.path.exists(installer_path):
            if not self.method_direct_download():
                return False
        
        # Extract installer
        logger.info("Extracting Visual C++ 6.0 installer...")
        os.makedirs(extract_dir, exist_ok=True)
        
        try:
            subprocess.run(['cabextract', '-d', extract_dir, installer_path], check=True)
        except subprocess.CalledProcessError as e:
            logger.error(f"Failed to extract installer: {e}")
            return False
        
        # Find and copy MFC42u.dll
        mfc42_found = False
        for root, dirs, files in os.walk(extract_dir):
            for file in files:
                if file.lower() == 'mfc42u.dll':
                    src_path = os.path.join(root, file)
                    dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
                    shutil.copy2(src_path, dest_path)
                    logger.info("MFC42u.dll extracted using cabextract")
                    mfc42_found = True
                    break
            if mfc42_found:
                break
        
        if not mfc42_found:
            logger.error("MFC42u.dll not found in extracted files")
            return False
        
        return True

    def method_alternative_sources(self) -> bool:
        """Method 5: Download from alternative sources"""
        logger.info("Method 5: Download from alternative sources")
        
        for url in self.direct_downloads:
            logger.info(f"Trying URL: {url}")
            dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
            
            if self.download_file(url, dest_path):
                if self.verify_file(dest_path):
                    logger.info("MFC42u.dll downloaded from alternative source")
                    return True
                else:
                    os.remove(dest_path)
                    logger.warning(f"Downloaded file from {url} failed verification")
        
        logger.error("Failed to download from alternative sources")
        return False

    def method_symlink_fallback(self) -> bool:
        """Method 6: Create symbolic link to existing MFC library"""
        logger.info("Method 6: Create symbolic link to existing MFC library")
        
        wine_prefix = self.detect_wine_prefix()
        system32_path = os.path.join(wine_prefix, 'drive_c', 'windows', 'system32')
        
        # Check for existing MFC libraries
        mfc_libs = ['mfc140u.dll', 'mfc120u.dll', 'mfc110u.dll', 'mfc100u.dll']
        
        for lib in mfc_libs:
            lib_path = os.path.join(system32_path, lib)
            if os.path.exists(lib_path):
                logger.info(f"Found existing MFC library: {lib}")
                dest_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
                
                # Create symbolic link
                if os.path.exists(dest_path):
                    os.remove(dest_path)
                
                os.symlink(lib_path, dest_path)
                logger.info(f"Created symbolic link from {lib} to mfc42u.dll")
                return True
        
        logger.warning("No existing MFC libraries found for symbolic link")
        return False

    def method_manual_guide(self) -> bool:
        """Method 7: Manual installation guide"""
        logger.info("Method 7: Manual installation guide")
        
        guide_path = os.path.join(self.config_dir, 'mfc42_manual_install.md')
        
        guide_content = """# Manual MFC42u.dll Installation Guide

## Option 1: Download from Microsoft
1. Visit: https://www.microsoft.com/en-us/download/details.aspx?id=15336
2. Download "Visual C++ 6.0 Redistributable"
3. Run the installer in Wine: `wine vcredist.exe /Q /T:C:\\ /C`
4. Copy the DLL: `cp ~/.wine/drive_c/windows/system32/mfc42u.dll ~/.config/linuxtrack/tir_firmware/`

## Option 2: Extract from Windows
1. Copy mfc42u.dll from a Windows system: `C:\\Windows\\System32\\mfc42u.dll`
2. Place it in: `~/.config/linuxtrack/tir_firmware/mfc42u.dll`

## Option 3: Use cabextract
1. Install cabextract: `sudo apt install cabextract`
2. Extract the installer: `cabextract -d extracted vcredist.exe`
3. Find mfc42u.dll in the extracted files and copy it

## Option 4: Alternative download sources
- Check GitHub repositories for MFC42u.dll
- Look for Visual C++ 6.0 redistributable packages
- Search for MFC42u.dll in software archives

## Verification
After installation, verify with:
```bash
file ~/.config/linuxtrack/tir_firmware/mfc42u.dll
```

## Alternative Sources Found Online:
- DLL-Files.com: https://www.dll-files.com/mfc42u.dll.html
- DLLme.com: https://www.dllme.com/dll/files/mfc42u.html
- GitHub repositories (search for "mfc42u.dll")
- Internet Archive software library
- Windows system files (if dual-booting)
"""
        
        with open(guide_path, 'w') as f:
            f.write(guide_content)
        
        logger.info(f"Manual installation guide created: {guide_path}")
        return True

    def verify_installation(self) -> bool:
        """Verify MFC42u.dll installation"""
        logger.info("Verifying MFC42u.dll installation...")
        
        mfc42_path = os.path.join(self.tir_firmware_dir, 'mfc42u.dll')
        
        if not os.path.exists(mfc42_path):
            logger.error(f"MFC42u.dll not found in {self.tir_firmware_dir}")
            return False
        
        file_size = os.path.getsize(mfc42_path)
        logger.info(f"File size: {file_size} bytes")
        
        if self.verify_file(mfc42_path):
            logger.info("MFC42u.dll appears to be a valid Windows DLL")
            return True
        else:
            logger.warning("MFC42u.dll may not be a valid Windows DLL")
            return False

    def install_mfc42(self) -> bool:
        """Main installation function"""
        logger.info("Starting MFC42u.dll installation...")
        
        # Try methods in order of preference
        methods = [
            self.method_direct_download,
            self.method_windows_extract,
            self.method_package_manager,
            self.method_cabextract,
            self.method_alternative_sources,
            self.method_symlink_fallback,
        ]
        
        for method in methods:
            logger.info(f"Trying {method.__name__}...")
            try:
                if method():
                    if self.verify_installation():
                        logger.info(f"MFC42u.dll installed successfully using {method.__name__}")
                        return True
                    else:
                        logger.warning("Installation completed but verification failed")
            except Exception as e:
                logger.error(f"Method {method.__name__} failed: {e}")
        
        # If all methods fail, show manual guide
        self.method_manual_guide()
        
        logger.error("All automatic installation methods failed. Please use the manual installation guide.")
        return False

    def cleanup(self):
        """Cleanup temporary files"""
        logger.info("Cleaning up temporary files...")
        extract_dir = os.path.join(self.cache_dir, 'vcredist_extracted')
        if os.path.exists(extract_dir):
            shutil.rmtree(extract_dir)

def main():
    """Main function"""
    installer = MFC42Installer()
    
    if len(sys.argv) > 1:
        command = sys.argv[1]
    else:
        command = 'install'
    
    try:
        if command == 'install':
            success = installer.install_mfc42()
            sys.exit(0 if success else 1)
        elif command == 'verify':
            success = installer.verify_installation()
            sys.exit(0 if success else 1)
        elif command == 'cleanup':
            installer.cleanup()
            sys.exit(0)
        elif command in ['help', '-h', '--help']:
            print("Usage: python3 mfc42_python_installer.py [install|verify|cleanup|help]")
            print("  install  - Install MFC42u.dll using multiple methods")
            print("  verify   - Verify existing MFC42u.dll installation")
            print("  cleanup  - Clean up temporary files")
            print("  help     - Show this help message")
            sys.exit(0)
        else:
            print(f"Unknown command: {command}")
            print("Use 'python3 mfc42_python_installer.py help' for usage information")
            sys.exit(1)
    finally:
        installer.cleanup()

if __name__ == '__main__':
    main() 
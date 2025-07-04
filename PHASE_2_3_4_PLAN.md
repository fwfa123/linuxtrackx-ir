# LinuxTrack Build System Modernization - Phases 2-4 Plan

## **AI Assistant Prompt for Continuing LinuxTrack MinGW Migration**

You are working on modernizing the LinuxTrack build system to use MinGW instead of wine-devel packages. **Phase 1 is complete** - all NSIS installer scripts have been updated to use native `.exe` and `.dll` files instead of `.exe.so` and `.dll.so` files.

## **Current Status**
- ✅ **Phase 1 Complete**: NSIS installer scripts updated
- ✅ **Build Working**: `make -j$(nproc)` completes successfully
- ✅ **NSIS Installer**: `linuxtrack-wine.exe` (2.3MB) created successfully
- ✅ **Native Windows Binaries**: All `.exe` and `.dll` files built with MinGW

## **Remaining Work: Phases 2-4**

### **Phase 2: Update Makefile.am Files (Priority: HIGH)**

**Objective**: Update all Makefile.am files to remove references to `.exe.so` and `.dll.so` patterns and ensure they work with native Windows binaries.

**Files to Update**:
1. `src/wine_bridge/Makefile.am` (main wine bridge Makefile)
2. `src/wine_bridge/controller/Makefile.am`
3. `src/wine_bridge/tester/Makefile.am`
4. `src/wine_bridge/ft_client/Makefile.am`
5. `src/wine_bridge/ft_tester/Makefile.am`
6. `src/wine_bridge/views/Makefile.am`
7. `src/wine_bridge/client/Makefile.am`

**Changes Needed**:
- Replace all `.exe.so` → `.exe`
- Replace all `.dll.so` → `.dll`
- Update any Wine-specific build rules to use MinGW
- Ensure proper file extensions in install targets
- Update any conditional compilation flags

**Validation**:
- Run `make clean && make -j$(nproc)` after each file update
- Verify all Windows components still build correctly
- Check that NSIS installer still works

### **Phase 3: Update Documentation and Build Scripts (Priority: MEDIUM)**

**Objective**: Update all documentation, build scripts, and configuration files to reflect the MinGW approach.

**Files to Update**:
1. `INSTALL` - Update installation instructions
2. `configure.ac` - Review MinGW configuration
3. `dev-scripts/` - Update any build automation scripts
4. `docs/` - Update any documentation files
5. `README.md` - Final documentation updates

**Changes Needed**:
- Remove all references to wine-devel packages
- Update dependency lists to emphasize MinGW requirements
- Update build instructions to reflect MinGW approach
- Add troubleshooting section for MinGW-specific issues
- Update any example commands or scripts

### **Phase 4: Final Testing and Validation (Priority: HIGH)**

**Objective**: Comprehensive testing to ensure the modernized build system works correctly across different scenarios.

**Testing Checklist**:
1. **Clean Build Test**:
   ```bash
   make distclean
   autoreconf -fiv
   ./configure --prefix=/opt
   make -j$(nproc)
   ```

2. **Installation Test**:
   ```bash
   sudo make install
   ```

3. **Component Verification**:
   - Verify all Linux components install correctly
   - Verify all Windows components are created
   - Test NSIS installer functionality

4. **Cross-Platform Validation**:
   - Test on different Linux distributions
   - Verify MinGW builds work consistently
   - Check for any distribution-specific issues

5. **Documentation Review**:
   - Verify all documentation is accurate
   - Test all example commands
   - Ensure troubleshooting guides are current

## **Technical Context**

### **Key Differences: Wine vs MinGW**
- **Wine Approach**: Created `.exe.so` and `.dll.so` files (Linux ELF with Wine wrapper)
- **MinGW Approach**: Creates native `.exe` and `.dll` files (Windows PE binaries)

### **Build System Architecture**
- **Autotools**: Uses autoconf, automake, libtool
- **Cross-Compilation**: MinGW toolchain for Windows components
- **NSIS**: Windows installer creation
- **Qt5**: GUI framework for Linux components

### **File Patterns to Search For**
- `.exe.so` → `.exe`
- `.dll.so` → `.dll`
- `winegcc` → `i686-w64-mingw32-gcc` or `x86_64-w64-mingw32-gcc`
- `wineg++` → `i686-w64-mingw32-g++` or `x86_64-w64-mingw32-g++`

## **Success Criteria**

**Phase 2 Complete When**:
- All Makefile.am files updated
- Build completes without errors
- All Windows components created with correct extensions
- NSIS installer works correctly

**Phase 3 Complete When**:
- All documentation updated
- Build scripts modernized
- No references to wine-devel remain
- Installation instructions are clear and accurate

**Phase 4 Complete When**:
- All tests pass
- Build system works on multiple distributions
- Documentation is verified and accurate
- No regressions introduced

## **Important Notes**

1. **Backward Compatibility**: Ensure existing Linux functionality is not broken
2. **Incremental Approach**: Test after each file modification
3. **Documentation**: Update README.md progress indicators as phases complete
4. **Git Commits**: Commit changes after each phase with descriptive messages
5. **Validation**: Always test the full build process after changes

## **Next Steps**

1. Start with Phase 2 - Update Makefile.am files
2. Test thoroughly after each file update
3. Document progress in README.md
4. Commit changes with clear commit messages
5. Move to Phase 3 once Phase 2 is complete
6. Conduct comprehensive testing in Phase 4

**Remember**: The goal is to modernize the build system while maintaining full functionality and improving compatibility with modern Linux distributions. 
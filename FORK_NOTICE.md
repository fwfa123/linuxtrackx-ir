# Fork Notice - LinuxTrack X-IR

## About This Fork

**LinuxTrack X-IR** is a fork of the original **LinuxTrack** project created by uglyDwarf. This document explains the relationship between the projects and our commitment to transparency and proper attribution.

## Original Project

- **Project Name**: LinuxTrack
- **Original Author**: uglyDwarf
- **Repository**: https://github.com/uglyDwarf/linuxtrack
- **Status**: Abandoned/Inactive
- **License**: MIT License

## This Fork

- **Project Name**: LinuxTrack X-IR
- **Fork Maintainer**: fwfa123
- **Repository**: https://github.com/fwfa123/linuxtrackx-ir
- **Status**: Active Development
- **License**: MIT License (maintained from original)

## Why This Fork Exists

The original LinuxTrack project was abandoned, leaving Linux users without updates for modern distributions and build systems. This fork was created to:

1. **Continue Development**: Provide ongoing support and updates
2. **Modernize Build System**: Update from Qt4 to Qt5 and modern autotools
3. **Enhance Compatibility**: Add MinGW cross-compilation for Windows
4. **Improve Security**: Enable modern security features (PIE, stack protector)
5. **Focus on TrackIR**: Prioritize TrackIR 4 & 5 compatibility

## Key Differences from Original

| Aspect | Original LinuxTrack | LinuxTrack X-IR |
|--------|-------------------|-----------------|
| **Qt Version** | Qt4 (legacy) | Qt5 (modern) |
| **Windows Support** | Wine-based | MinGW native |
| **Build System** | Legacy autotools | Modern autotools + CMake |
| **Security** | Basic | PIE, stack protector, fortify |
| **Development Status** | Abandoned | Active |
| **Distribution Support** | Legacy Linux | Modern Linux distributions |

## Attribution and Credits

### Original Authors (Full Credit Maintained)
- **uglyDwarf** - Original creator and main developer
- **Tulthix** - Early contributor
- **f.josef** - Main developer and maintainer (302 commits)
- **Michal Navratil** - Significant contributor (20 commits)
- **raven** - Contributor (4 commits)
- **aledin** - Contributor (2 commits)

### Fork Contributors
- **fwfa123** - Fork maintainer and Qt5 modernization (61 commits)
- **Cursor AI** - Assisted with build system modernization

## License Compliance

This fork maintains the same MIT License as the original project to ensure:
- **Compatibility**: Same licensing terms for users
- **Respect**: Honoring the original authors' licensing choice
- **Transparency**: Clear licensing for all derivative works

## Community Guidelines

1. **Respect Original Work**: Always acknowledge the original LinuxTrack project
2. **Transparency**: Be clear about what's original vs. new development
3. **Collaboration**: Welcome contributions that improve the project
4. **Documentation**: Maintain clear documentation of changes and improvements

## Contact and Support

- **Fork Issues**: https://github.com/fwfa123/linuxtrackx-ir/issues
- **Original Project**: https://github.com/uglyDwarf/linuxtrack (for reference)

---

**Note**: This fork is created with the utmost respect for the original LinuxTrack project and its contributors. We aim to continue their excellent work while modernizing it for current Linux distributions and build systems. 
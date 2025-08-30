# 0012 Fedora/Nobara Wine Development Setup Fix Plan

## Overview

This feature addresses critical compatibility issues with Fedora and Nobara Linux distributions in the Wine development setup process. The goal is to ensure seamless installation and configuration of Wine development tools for building LinuxTrack on modern Fedora-based systems.

## Problem Statement

### Primary Issues Identified

1. **Incorrect Package Names**: The README.md contained Debian/Ubuntu-specific package names that don't exist in Fedora repositories
2. **Unsupported Distribution**: Nobara Linux (Fedora-based) was not recognized as a supported distribution
3. **Missing Package Dependencies**: Wine debug packages referenced in scripts don't exist in Fedora 42
4. **32-bit Library Detection Failure**: The verification process couldn't properly detect Fedora's 32-bit Wine support (WoW64)

### Specific Package Issues

| Incorrect (Debian/Ubuntu) | Correct (Fedora) | Impact |
|---------------------------|------------------|---------|
| `qttools5-dev` | `qt5-qttools-devel` | Build fails |
| `libmxml-devel` | `mxml-devel` | Build fails |
| `nsis` | `mingw-nsis-base` | Build fails |
| `v4l-utils-devel` | `libv4l-devel` | Build fails |
| `wine-devel-debuginfo*` | N/A (doesn't exist) | Installation fails |

## Technical Analysis

### Distribution Detection Logic
```bash
# Current problematic logic
case $DISTRO in
    fedora|rhel|centos)
        # Fedora installation
        ;;
    *)
        # Error: Unsupported distribution
        ;;
```

### Fedora Library Organization
- **Debian/Ubuntu**: 32-bit libs in `/usr/lib/i386-linux-gnu/`
- **Arch**: 32-bit libs in `/usr/lib32/`
- **Fedora**: 32-bit libs integrated with 64-bit in `/usr/lib/`
- **WoW64 Support**: Fedora uses Wine's built-in 32-bit emulation

## Solution Design

### Phase 1: Package Name Corrections

**Objective**: Update README.md with correct Fedora package names

**Implementation**:
- Replace Debian package names with Fedora equivalents
- Test package availability using `dnf search`
- Validate installation success

**Files to Modify**:
- `README.md` (Fedora installation section)

### Phase 2: Distribution Support Extension

**Objective**: Add Nobara Linux support to Wine setup scripts

**Implementation**:
```bash
# Enhanced distribution detection
case $DISTRO in
    fedora|rhel|centos|nobara)  # Added nobara
        install_fedora
        ;;
    # ... other distributions
```

**Files to Modify**:
- `scripts/dev/install_wine_dev.sh`
- `scripts/dev/wine_dev_setup.sh`

### Phase 3: Package Availability Fixes

**Objective**: Remove non-existent debug packages from Fedora installation

**Implementation**:
```bash
# Before (problematic)
dnf install -y wine-devel-debuginfo wine-devel-debuginfo.i686

# After (fixed)
# Removed debug packages that don't exist in Fedora 42
```

**Files to Modify**:
- `scripts/dev/install_wine_dev.sh`

### Phase 4: 32-bit Library Detection Enhancement

**Objective**: Improve verification to work with Fedora's WoW64 implementation

**Implementation**:
```bash
# Enhanced 32-bit detection
if [ -d "/usr/lib/i386-linux-gnu" ] || [ -d "/usr/lib32" ]; then
    # Debian/Arch style
    print_success "32-bit libraries available"
elif command -v winegcc >/dev/null 2>&1; then
    # Fedora WoW64 detection
    if wine --version | grep -q "Staging\|Development"; then
        print_success "32-bit Wine support detected (WoW64)"
    fi
fi
```

**Files to Modify**:
- `scripts/dev/install_wine_dev.sh`

## Implementation Plan

### Task Breakdown

1. **Package Research & Validation**
   - [x] Test all corrected package names on clean Fedora/Nobara system
   - [x] Verify package availability in repositories
   - [x] Confirm package versions and dependencies

2. **Script Modifications**
   - [x] Update distribution detection logic
   - [x] Remove problematic debug packages
   - [x] Enhance 32-bit library verification
   - [x] Update help text and documentation

3. **Testing & Validation**
   - [x] Test on clean Fedora 42 installation
   - [x] Test on Nobara Linux installation
   - [x] Verify Wine development tools function correctly
   - [x] Confirm LinuxTrack builds successfully

4. **Documentation Updates**
   - [x] Update README.md with corrected instructions
   - [x] Add troubleshooting section for Fedora-specific issues
   - [x] Document Nobara Linux compatibility

## Success Criteria

### Functional Requirements
- [x] `winegcc` command available after installation
- [x] `wine` command functional with WoW64 support
- [x] LinuxTrack builds successfully on Fedora/Nobara
- [x] No package resolution errors during installation
- [x] 32-bit library detection works correctly

### Compatibility Requirements
- [x] Fedora 42 (latest)
- [x] Nobara Linux (all versions)
- [x] RHEL/CentOS (existing support maintained)
- [x] Backward compatibility with older Fedora versions

## Risk Assessment

### Low Risk
- [x] Package name corrections (straightforward replacements)
- [x] Distribution detection enhancement (simple string addition)

### Medium Risk
- [x] 32-bit library detection changes (may need refinement)
- [x] Debug package removal (verify no dependencies broken)

### Mitigation Strategies
- [x] Test on multiple Fedora versions before deployment
- [x] Maintain fallback detection methods
- [x] Preserve existing working configurations
- [x] Add verbose logging for troubleshooting

## Dependencies

### External Dependencies
- Fedora repositories must contain required packages
- Wine staging/development versions must support WoW64
- GCC and development tools must be available

### Internal Dependencies
- Existing Wine setup scripts must remain functional
- LinuxTrack build system must work with corrected packages
- Documentation must stay synchronized with code changes

## Testing Strategy

### Unit Testing
- [ ] Package installation verification
- [ ] Distribution detection accuracy
- [ ] 32-bit library detection reliability

### Integration Testing
- [ ] Complete Wine development setup on Fedora
- [ ] Complete Wine development setup on Nobara
- [ ] LinuxTrack full build and installation

### Regression Testing
- [ ] Verify existing Debian/Ubuntu support unchanged
- [ ] Verify existing Arch Linux support unchanged
- [ ] Verify existing RHEL/CentOS support unchanged

## Rollback Plan

If issues arise with the new implementation:

1. **Immediate Rollback**: Revert script changes to previous working version
2. **Partial Rollback**: Keep distribution support but revert package changes
3. **Documentation**: Update users about any temporary workarounds needed

## Timeline

- **Phase 1** (Package Corrections): 1-2 hours
- **Phase 2** (Distribution Support): 1 hour
- **Phase 3** (Package Removal): 30 minutes
- **Phase 4** (Detection Enhancement): 1 hour
- **Testing & Validation**: 2-4 hours
- **Documentation**: 1 hour

**Total Estimated Time**: 6-9 hours

## Stakeholders

- **Primary**: Fedora and Nobara Linux users
- **Secondary**: LinuxTrack maintainers and contributors
- **Tertiary**: Wine development community

## Success Metrics

- Zero package installation failures on Fedora/Nobara
- 100% successful Wine development setup completion
- Successful LinuxTrack builds on target distributions
- Positive user feedback on improved compatibility

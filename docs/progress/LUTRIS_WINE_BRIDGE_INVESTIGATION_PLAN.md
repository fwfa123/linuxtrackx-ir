# 🎮 Lutris Wine Bridge Installation Investigation Plan

**Branch**: `gaming-tab-and-lutris-install`  
**Phase**: 1.1 - QT Interface Upgrades - Wine/Steam Tab  
**Priority**: HIGH | Effort: MEDIUM | Impact: HIGH  
**Status**: INVESTIGATION PHASE

---

## 📋 Investigation Overview

This document outlines the comprehensive investigation plan for implementing the **"Install Wine Bridge in Lutris Prefix"** button as part of the new Wine/Steam tab feature. The investigation will cover all aspects from GUI design to backend implementation.

---

## 🎯 Investigation Goals

### Primary Objectives
1. **Understand Current Wine Bridge Implementation**
   - Analyze existing wine bridge installation code
   - Identify current installation methods and scripts
   - Understand wine prefix detection and management

2. **Research Lutris Integration Requirements**
   - Understand Lutris wine prefix structure
   - Identify Lutris-specific installation requirements
   - Research Lutris API or configuration methods

3. **Design GUI Integration Strategy**
   - Plan new Wine/Steam tab layout
   - Design Lutris-specific installation workflow
   - Plan user experience and error handling

4. **Technical Implementation Planning**
   - Identify required code changes
   - Plan wine bridge installation process
   - Design error handling and user feedback

---

## 🔍 Investigation Areas

### 1. Current Wine Bridge Implementation Analysis

#### 1.1 Existing Installation Code
**Files to Investigate:**
- `src/qt_gui/plugin_install.cpp` - Current wine bridge installation
- `src/qt_gui/ltr.ui` - Current GUI layout (Misc tab)
- `scripts/appimage/install_wine_bridge.sh` - Installation script
- `src/wine_bridge/` - Wine bridge components

**Investigation Points:**
- [ ] How current wine bridge installation works
- [ ] What wine prefixes are currently supported
- [ ] How wine prefix detection is implemented
- [ ] Current error handling and user feedback
- [ ] Installation script structure and dependencies

#### 1.2 Wine Prefix Management
**Files to Investigate:**
- `src/qt_gui/wine_launcher.cpp` - Wine environment management
- `src/qt_gui/extractor.cpp` - Wine prefix creation and management

**Investigation Points:**
- [ ] How wine prefixes are created and managed
- [ ] Wine environment variable handling
- [ ] Wine version detection and selection
- [ ] Cross-distribution wine compatibility

### 2. Lutris Integration Research

#### 2.1 Lutris Wine Prefix Structure
**Research Areas:**
- [ ] Lutris wine prefix directory structure
- [ ] Lutris configuration file format
- [ ] Lutris wine prefix naming conventions
- [ ] Lutris wine version management

**Expected Locations:**
- `~/.local/share/lutris/runners/wine/`
- `~/.config/lutris/`
- `~/.local/share/lutris/games/`

#### 2.2 Lutris API and Configuration
**Research Areas:**
- [ ] Lutris configuration file format (YAML)
- [ ] Lutris wine prefix configuration
- [ ] Lutris wine runner management
- [ ] Lutris game-specific wine prefixes

**Files to Investigate:**
- Lutris configuration files in user directories
- Lutris documentation and API references
- Existing Lutris integration examples

#### 2.3 Lutris Wine Bridge Requirements
**Research Areas:**
- [ ] Specific DLL requirements for Lutris
- [ ] Wine bridge compatibility with Lutris wine versions
- [ ] Lutris-specific installation paths
- [ ] Integration with Lutris game launcher

### 3. GUI Design Investigation

#### 3.1 Current Tab Structure Analysis
**Files to Investigate:**
- `src/qt_gui/ltr.ui` - Current tab layout
- `src/qt_gui/ltr_gui.cpp` - Tab management
- `src/qt_gui/ltr_gui.h` - Tab class definitions

**Investigation Points:**
- [ ] Current tab widget implementation
- [ ] How new tabs can be added
- [ ] Tab switching and state management
- [ ] Button placement and layout options

#### 3.2 Wine/Steam Tab Design Planning
**Design Considerations:**
- [ ] Tab layout and organization
- [ ] Button placement and grouping
- [ ] User interface flow
- [ ] Error message display
- [ ] Progress indication

**Proposed Layout:**
```
Wine/Steam Tab:
├── Wine Bridge Installation
│   ├── Install Wine Bridge (Default)
│   ├── Install Wine Bridge in Steam Proton
│   └── Install Wine Bridge in Lutris Prefix
├── TrackIR Support
│   ├── Launch Mickey (TrackIR)
│   └── Launch ltr_pipe for Antimicrox
└── System Information
    └── (Future: Wine/Proton/Lutris detection)
```

### 4. Technical Implementation Planning

#### 4.1 Code Structure Analysis
**Files to Modify:**
- `src/qt_gui/ltr.ui` - Add new Wine/Steam tab
- `src/qt_gui/ltr_gui.cpp` - Add tab management
- `src/qt_gui/ltr_gui.h` - Add new class members
- `src/qt_gui/plugin_install.cpp` - Add Lutris installation logic

**New Files to Create:**
- `src/qt_gui/lutris_installer.cpp` - Lutris-specific installer
- `src/qt_gui/lutris_installer.h` - Lutris installer class
- `scripts/install/install_lutris_wine_bridge.sh` - Lutris installation script

#### 4.2 Installation Process Design
**Process Flow:**
1. **Lutris Detection**
   - Check if Lutris is installed
   - Detect Lutris configuration directory
   - List available Lutris wine prefixes

2. **Prefix Selection**
   - Show available Lutris wine prefixes
   - Allow user to select specific prefix
   - Validate prefix accessibility

3. **Installation Execution**
   - Copy wine bridge files to selected prefix
   - Register DLLs in wine registry
   - Verify installation success

4. **Verification and Feedback**
   - Test wine bridge functionality
   - Provide user feedback
   - Handle installation errors

#### 4.3 Error Handling Strategy
**Error Scenarios:**
- [ ] Lutris not installed
- [ ] No Lutris wine prefixes found
- [ ] Wine bridge files not available
- [ ] Installation permission issues
- [ ] Wine bridge already installed
- [ ] Installation verification failure

**Error Handling Approach:**
- [ ] User-friendly error messages
- [ ] Detailed logging for debugging
- [ ] Graceful fallback options
- [ ] Installation rollback capability

---

## 🔬 Investigation Tasks

### Phase 1: Codebase Analysis (Week 1)

#### Task 1.1: Analyze Current Wine Bridge Implementation
- [ ] **Review `src/qt_gui/plugin_install.cpp`**
  - Understand current wine bridge installation flow
  - Identify wine prefix detection methods
  - Analyze error handling patterns
  - Document installation process steps

- [ ] **Review `scripts/appimage/install_wine_bridge.sh`**
  - Understand shell script installation process
  - Identify wine bridge file structure
  - Analyze wine environment setup
  - Document script dependencies

- [ ] **Review `src/qt_gui/wine_launcher.cpp`**
  - Understand wine version detection
  - Analyze wine environment management
  - Identify wine prefix creation methods
  - Document wine compatibility handling

#### Task 1.2: Analyze Current GUI Structure
- [ ] **Review `src/qt_gui/ltr.ui`**
  - Document current tab structure
  - Identify button placement patterns
  - Analyze layout management
  - Plan new tab integration

- [ ] **Review `src/qt_gui/ltr_gui.cpp`**
  - Understand tab management
  - Analyze button signal handling
  - Identify class structure
  - Document GUI initialization

### Phase 2: Lutris Research (Week 1-2)

#### Task 2.1: Lutris Installation Analysis
- [ ] **Install Lutris on test system**
  - Document installation process
  - Identify configuration directories
  - Analyze wine prefix structure
  - Document wine runner management

- [ ] **Analyze Lutris Configuration**
  - Review `~/.config/lutris/` structure
  - Document YAML configuration format
  - Identify wine prefix configuration
  - Analyze game-specific settings

#### Task 2.2: Lutris Wine Prefix Research
- [ ] **Create test Lutris wine prefixes**
  - Document prefix directory structure
  - Analyze wine version management
  - Identify prefix naming conventions
  - Test wine bridge compatibility

- [ ] **Research Lutris API/Integration**
  - Search for Lutris command-line tools
  - Investigate Lutris configuration API
  - Research existing Lutris integrations
  - Document available integration methods

### Phase 3: Technical Design (Week 2)

#### Task 3.1: GUI Design Planning
- [ ] **Design Wine/Steam Tab Layout**
  - Create mockup of new tab design
  - Plan button placement and grouping
  - Design user interaction flow
  - Plan error message display

- [ ] **Plan Code Structure**
  - Design new class structure
  - Plan signal/slot connections
  - Design installation workflow
  - Plan error handling strategy

#### Task 3.2: Installation Process Design
- [ ] **Design Lutris Detection Process**
  - Plan Lutris installation detection
  - Design wine prefix discovery
  - Plan prefix validation
  - Design user selection interface

- [ ] **Design Installation Workflow**
  - Plan wine bridge file copying
  - Design registry modification
  - Plan installation verification
  - Design rollback procedures

### Phase 4: Implementation Planning (Week 2-3)

#### Task 4.1: Code Modification Plan
- [ ] **Plan UI Changes**
  - List required UI file modifications
  - Plan new tab addition
  - Design button integration
  - Plan layout adjustments

- [ ] **Plan Backend Changes**
  - List required C++ file modifications
  - Plan new class implementation
  - Design installation logic
  - Plan error handling

#### Task 4.2: Testing Strategy
- [ ] **Plan Testing Approach**
  - Design unit tests for new functionality
  - Plan integration testing
  - Design user acceptance testing
  - Plan cross-distribution testing

---

## 📊 Investigation Deliverables

### Phase 1 Deliverables
- [ ] **Current Implementation Documentation**
  - Wine bridge installation process flow
  - GUI structure and tab management
  - Error handling patterns
  - Wine prefix management methods

### Phase 2 Deliverables
- [ ] **Lutris Integration Documentation**
  - Lutris configuration structure
  - Wine prefix management in Lutris
  - Available integration methods
  - Compatibility requirements

### Phase 3 Deliverables
- [ ] **Technical Design Documents**
  - GUI design mockups
  - Code structure plans
  - Installation workflow diagrams
  - Error handling specifications

### Phase 4 Deliverables
- [ ] **Implementation Roadmap**
  - Detailed code modification plan
  - Testing strategy document
  - Risk assessment and mitigation
  - Timeline and milestones

---

## 🚀 Next Steps

### Immediate Actions (This Week)
1. **Start Phase 1 Investigation**
   - Begin codebase analysis
   - Document current implementation
   - Identify key files and methods

2. **Set Up Test Environment**
   - Install Lutris for testing
   - Create test wine prefixes
   - Prepare development environment

3. **Begin Lutris Research**
   - Install and configure Lutris
   - Analyze Lutris configuration
   - Document wine prefix structure

### Week 2 Actions
1. **Complete Lutris Research**
   - Finish Lutris integration analysis
   - Document available methods
   - Test wine bridge compatibility

2. **Begin Technical Design**
   - Create GUI mockups
   - Design code structure
   - Plan implementation approach

### Week 3 Actions
1. **Finalize Implementation Plan**
   - Complete technical design
   - Create detailed implementation roadmap
   - Plan testing strategy

2. **Begin Implementation**
   - Start with GUI modifications
   - Implement basic functionality
   - Begin testing

---

## 📝 Notes and Considerations

### Technical Challenges
- **Lutris Integration Complexity**: Lutris may not have a simple API for external integration
- **Wine Prefix Management**: Different wine versions and configurations may require different approaches
- **Cross-Distribution Compatibility**: Need to ensure compatibility across different Linux distributions
- **Error Handling**: Comprehensive error handling for various failure scenarios

### User Experience Considerations
- **Simple Interface**: Keep the interface simple and intuitive
- **Clear Feedback**: Provide clear progress and error messages
- **Fallback Options**: Provide alternative installation methods if Lutris integration fails
- **Documentation**: Ensure users understand what the feature does

### Future Enhancements
- **Steam Proton Integration**: Similar functionality for Steam Proton prefixes
- **Automatic Detection**: Automatically detect and list available wine prefixes
- **Batch Installation**: Install to multiple prefixes at once
- **Uninstall Functionality**: Remove wine bridge from selected prefixes

---

*Last Updated: [Current Date]*  
*Next Review: [Date + 1 week]* 
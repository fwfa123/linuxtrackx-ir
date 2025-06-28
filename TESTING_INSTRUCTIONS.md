# LinuxTrack Testing Instructions

## Local Git Repository Testing

Your LinuxTrack Qt5 repository is now ready for testing! The changes have been committed and pushed to your local git repository.

### 🔧 **Repository Location**

The prepared repository is available at:
```
/mnt/Local_Git/git-repos/linuxtrack-modernized (branch: trackir2winebridge)
```

### 📋 **Testing Process for Another OS**

#### Step 1: Clone the Repository
```bash
# From your other OS, clone the repository
git clone /mnt/Local_Git/git-repos/linuxtrack-modernized linuxtrack-test
cd linuxtrack-test

# Switch to the prepared branch
git checkout trackir2winebridge
```

#### Step 2: Quick Test Build (Automated)
```bash
# Test the one-command build process
./build.sh --deps --clean --install

# If successful, launch the application
./run_qt5_gui.sh
```

#### Step 3: Manual Build Test (Alternative)
```bash
# Test manual build process as documented in README
autoreconf -fiv
./configure --prefix=/usr/local
make -j$(nproc)
sudo make install
sudo ./setup.sh
```

### 🧪 **What to Test**

#### ✅ **Build System Testing**
- [ ] Dependencies install correctly on target OS
- [ ] Autotools configuration completes without errors
- [ ] Qt5 is properly detected (not Qt6)
- [ ] All libraries build successfully
- [ ] GUI application compiles and links

#### ✅ **TrackIR Hardware Testing**
- [ ] TrackIR 5 device is detected by system
- [ ] Device permissions are correct
- [ ] LinuxTrack GUI detects TrackIR without "permission errors"
- [ ] Tracking works and responds to head movement

### 🎯 **Success Confirmation**

**The test is successful if:**
- Build completes with `./build.sh --deps --clean --install`
- GUI launches with `./run_qt5_gui.sh`
- TrackIR 5 is detected and shows **no permission errors**
- Head tracking works smoothly

The repository is professionally prepared and ready for public distribution once testing confirms everything works as expected! 🎮✈️🏎️

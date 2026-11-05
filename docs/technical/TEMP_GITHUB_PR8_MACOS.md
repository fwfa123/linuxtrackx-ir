# TEMP — GitHub PR #8 (macOS support) weekend note

**Maintainer-only.** Delete this file before tagging **3.0.0** so it does not ship in the release. Do not link it from `DOCUMENTATION_INDEX.md`.

**GitHub PR:** [fwfa123/linuxtrackx-ir#8](https://github.com/fwfa123/linuxtrackx-ir/pull/8) — *macOS support* (wilmai)  
**Branch:** `wilmai:feat/macos-support` at `f342895`  
**Opened against:** `f5f6536` (1 September 2026)  
**Reviewed against GitLab `main`:** `37560d2` (2026-11-05)  
**No GitLab issue** exists for this work (search for macOS returned nothing).

---

## Weekend checklist

- [ ] **delete-this** — Remove `docs/technical/TEMP_GITHUB_PR8_MACOS.md` before the 3.0.0 tag
- [ ] **merge-control-points** — Merge `feature/curve-control-points` into GitLab `main`
- [ ] **tag-300** — Tag and push `v3.0.0`
- [ ] **post-pr8** — Paste the comment below on [GitHub PR #8](https://github.com/fwfa123/linuxtrackx-ir/pull/8), filling in the tag URL and commit
- [ ] **leave-open** — Leave the PR open; merge into GitLab only after wilmai rebases onto `v3.0.0` (use the workflow below)

---

## How to merge a GitHub PR into GitLab

The GitHub PR never lands on GitHub. Pull the contributor’s branch into this GitLab clone and merge (or open a GitLab MR) here. Do **not** use GitHub’s Merge button unless that GitHub repo is what you actually release from.

`origin` for this worktree is GitLab: `git@gitlab.com:fwfa123/linuxtrackx-ir.git`.

### 1. Fetch the PR branch

```bash
git fetch origin
git checkout main
git pull origin main

# one-shot fetch of the contributor branch (PR #8)
git fetch https://github.com/wilmai/linuxtrackx-ir.git feat/macos-support:refs/heads/pr-8-macos
git checkout pr-8-macos
```

Or add a remote once and reuse it:

```bash
git remote add wilmai https://github.com/wilmai/linuxtrackx-ir.git   # once
git fetch wilmai feat/macos-support
git checkout -b pr-8-macos wilmai/feat/macos-support
```

Prefer waiting until wilmai has rebased onto `v3.0.0` and pushed to the same GitHub PR, then fetch that updated tip.

### 2. Rebase onto current GitLab `main` (or `v3.0.0`)

```bash
git rebase origin/main   # or: git rebase v3.0.0
# fix conflicts if any, then:
git rebase --continue
```

### 3. Merge into GitLab `main` and push

```bash
git checkout main
git merge --no-ff pr-8-macos   # or fast-forward if you prefer
git push origin main
```

Or push the branch and open a GitLab merge request:

```bash
git push -u origin pr-8-macos
# then open MR on GitLab UI / glab
```

### 4. Close the GitHub PR

On GitHub, comment that it was merged into GitLab (link the GitLab commit or MR), then close the PR.

### 5. After merge (PR #8 specific)

- Bump `Formula/linuxtrackx-ir.rb` to the new GitLab revision if the Homebrew formula is kept.
- Sync the GitHub mirror later if needed: push the same `main` (or tag) so GitHub catches up.
- Retest one TIR5 v2 Linux session (see below).

---

## Repo roles

| Host | Role |
|------|------|
| **GitLab** (`gitlab.com/fwfa123/linuxtrackx-ir`) | Official development and releases |
| **GitHub** (`github.com/fwfa123/linuxtrackx-ir`) | Mirror / where this contributor opened the PR |

GitLab `main` at review time: `37560d2` — docs: note Proton Wayland breaks in-game global hotkeys.

GitHub `main` at review time: `4ea5122` — three documentation commits behind GitLab (`65be086`, `4d81a1f`, `37560d2`).

SSH can push this feature branch to GitLab. Posting on the GitHub PR needs the other PC (HTTPS + authenticator).

---

## PR identity

Three commits (all 2026-09-01):

1. `cd49a7b` — feat: ltr extractor now works with TrackIR 5.5.3 installer
2. `be9d7e5` — macos: Support TIR5V2, TIR5V3
3. `f342895` — macos: update brew formula for merge

**Diff vs its base:** 58 files, +11,456 / −208.

**Author claims:** TrackIR 5 only (no Wine). TIR5 v2 tested on macOS 26 Tahoe (Apple arm64) and Fedora 44. TIR5 v3 implemented, untested. LLM-assisted, human-reviewed. Install via Homebrew formula after merge (formula git revision must be updated at release).

Local fetch for review (already done once):

```bash
git fetch https://github.com/wilmai/linuxtrackx-ir.git feat/macos-support:refs/review/pr-8-macos
```

---

## Merge difficulty (as of 2026-11-05)

GitLab `main` was **33 commits** ahead of the PR base (`f5f6536`).

`git merge-tree` of the PR onto GitLab `main` completed **with no conflicts**.

Seven files edited on both sides since the merge-base; all auto-merged (GitLab side kept where overlapping):

- `CMakeLists.txt`
- `src/tir_hw.c`
- `src/mickey/keyb.cpp`
- `src/qt_gui/ltr_gui.cpp`
- `src/qt_gui/glwidget.cpp`
- `src/qt_gui/CMakeLists.txt`
- `src/extract.c`

GitLab **#70** (BMS pose backpressure) is in `src/ltr_srv_master.cpp` — the PR does not touch that file. Hotkey and skinning work is either wrapped with `#ifdef DARWIN` or untouched.

### After 3.0.0 lands

`feature/curve-control-points` adds the Profile / curve-editor work. Overlap with the PR is essentially the root **`CMakeLists.txt` only**:

- PR still has `project(LinuxTrack VERSION 2.2.0 …)`
- This branch has `project(LinuxTrack VERSION 3.0.0 …)`

Curve-editor commits do **not** touch `tir_hw.c` or `macos/`. Rebase after the tag should be small; the real cost is retesting the Linux camera path (see below).

---

## What the PR changes

### New / isolated

- `macos/TrackIRUserSpace/` — IOUSBHost userspace driver (`TrackIRUSBBackend.mm`, `TrackIRUSBTransport.mm`). No Apple signing.
- `Formula/linuxtrackx-ir.rb` — Homebrew formula; **bump git revision after merge**.
- `src/native_extractor/` + vendored **libmspack** (LGPL-2.1, includes `COPYING.LIB`) — native extract from TrackIR 5.5.3 installer without Wine. Path: `ltr_extractor --extract --installer TrackIR_SETUP.exe`. Existing blob path unchanged.

On Apple: skip joystick helper and `ltr_pipe`; build `libltusb1` from the Objective-C++ backend. On Linux: still `libusb_ifc.c`.

### Shared Linux TrackIR path (needs retest after rebase)

- `tir_driver.c` wraps existing libusb symbols in `ltr_usb_transport`.
- `tir_hw.c` / `tir_img.c` call the wrapper instead of libusb function pointers directly.
- Packet build/parse moved to `tir_protocol.c` (status, firmware checksum, LED, threshold, exposure, stripe decode). Byte layouts match the old code. LED bytes masked with `0xF0` (status LEDs already in that nibble).
- Portability: `finite()` → `isfinite()`, `rindex()` → `strrchr()`, `malloc.h` → `stdlib.h`, `dl` → `${CMAKE_DL_LIBS}`, Mini-XML 4 ifdef (Mini-XML 3 path unchanged).
- `ltr_server1` now requires the full 6 slave arguments; short argv exits with an error instead of calling `ltr_int_slave()`.

### Explicit macOS limits (OK as noted)

- Global hotkeys return failure until a native backend exists.
- Graphics diagnostic page unavailable on macOS.

---

## Retest ask after rebase onto v3.0.0

One **TrackIR 5 v2** Linux session:

1. Device found
2. Firmware load
3. Blobs / tracking
4. Threshold
5. Exposure
6. Status LED

Their Fedora 44 confirmation was against the **September** tree, not post-hotkey / post-3.0.0 `main`.

---

## Paste-ready comment for GitHub PR #8

Fill in the tag URL and commit after `v3.0.0` is pushed, then paste on the PR. Do **not** send the contributor to GitLab.

```text
Thanks for this. The IOUSBHost backend, the transport split, and the native 5.5.3 extractor are the right shape, and a trial merge onto then-current main was already clean.

Please hold this branch until 3.0.0 is tagged. Development and releases live on GitLab; this GitHub repo follows that. 3.0.0 is the Profile tab with editable axis curves, and it moves the version to 3.0.0 in the root CMakeLists.txt. Rebasing before that tag would just have to be redone afterward.

3.0.0 is now out:
  Tag: https://gitlab.com/fwfa123/linuxtrackx-ir/-/tags/v3.0.0
  Commit: <FILL_IN_v3.0.0_COMMIT_SHA>

Please rebase feat/macos-support onto v3.0.0 and push to this PR. A rebase keeps your three commits intact. I will merge the updated branch into GitLab from here.

After the rebase, one TrackIR 5 v2 session on Linux is the check that matters: device found, firmware load, blobs, threshold, exposure, status LED. The September Fedora test was against the old base, and the Linux driver now reaches libusb through the new transport wrapper. TIR5 v3 untested, macOS hotkeys left unimplemented, and the Homebrew formula revision updated at release are all fine to leave as noted.
```

If you need a shorter “wait for the tag” comment *before* release weekend (optional; the checklist above posts after the tag):

```text
Thanks for this — reviewing and will merge after our 3.0.0 release (Profile / curve editor), expected this weekend. Please hold rebasing until that tag is published; I'll comment here with the exact base once v3.0.0 is up. GitLab is the official line; this GitHub PR is still the right place to push your updates.
```

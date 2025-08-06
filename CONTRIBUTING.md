# Contributing to LinuxTrack X-IR

Thank you for your interest in contributing to LinuxTrack X-IR! This document provides guidelines for reporting issues, suggesting features, and contributing code.

## 🐛 Reporting Issues

### Before Reporting
1. **Search existing issues** to avoid duplicates
2. **Check the documentation** in `/docs` for solutions
3. **Test with the latest version** (currently v0.99.22)

### Issue Report Template
When creating an issue, please include:

```markdown
## Issue Description
Brief summary of the problem

## Steps to Reproduce
1. Download LinuxTrack X-IR v0.99.22
2. Run the AppImage
3. Go to [specific menu/tab]
4. Click [specific button]
5. Error occurs

## Expected Behavior
What should happen

## Actual Behavior
What actually happens

## System Information
- **OS**: Ubuntu 22.04 / Fedora 37 / Arch Linux / etc.
- **LinuxTrack Version**: v0.99.22
- **Installation Method**: AppImage / Source / Package
- **Wine Version**: (if applicable)
- **Hardware**: CPU, GPU, RAM
- **Desktop Environment**: GNOME / KDE / XFCE / etc.

## Additional Information
- Screenshots (if applicable)
- Log files (if available)
- Error messages (copy/paste exact text)
```

## 💡 Feature Requests

### Before Requesting
1. **Check existing issues** for similar requests
2. **Review the roadmap** in documentation
3. **Consider if it fits the project scope**

### Feature Request Template
```markdown
## Feature Description
Brief description of the requested feature

## Use Case
How would this feature be used?

## Proposed Implementation
Any ideas on how to implement this?

## Alternatives Considered
What other approaches were considered?

## Additional Context
Any other relevant information
```

## 🔧 Code Contributions

### Development Setup
1. **Fork the repository** on GitLab
2. **Clone your fork**:
   ```bash
   git clone https://gitlab.com/YOUR_USERNAME/linuxtrackx-ir.git
   cd linuxtrackx-ir
   ```
3. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```
4. **Make your changes**
5. **Test thoroughly**
6. **Commit with clear messages**:
   ```bash
   git commit -m "Add feature: brief description"
   ```
7. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```
8. **Create a merge request**

### Code Style Guidelines
- **C/C++**: Follow existing code style
- **Qt**: Use Qt coding conventions
- **Documentation**: Update relevant docs
- **Comments**: Add comments for complex logic
- **Tests**: Include tests for new features

### Commit Message Format
```
type(scope): brief description

Detailed description if needed

Fixes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## 🎮 Testing

### Testing Checklist
- [ ] Test on multiple Linux distributions
- [ ] Test with different Wine versions
- [ ] Test AppImage functionality
- [ ] Test source build
- [ ] Test with different desktop environments
- [ ] Test head tracking functionality
- [ ] Test gaming integration features

### Testing Environments
- **Ubuntu 20.04+**
- **Fedora 33+**
- **Arch Linux**
- **Debian 11+**
- **OpenSUSE Tumbleweed**

## 📚 Documentation

### Contributing to Documentation
1. **Update relevant files** in `/docs`
2. **Keep README.md current**
3. **Update release notes** for new features
4. **Add inline comments** for complex code

### Documentation Standards
- Use clear, concise language
- Include code examples
- Add screenshots for UI changes
- Keep installation instructions current

## 🏷️ Issue Labels

### Bug Reports
- `bug`: General bug
- `crash`: Application crashes
- `regression`: Previously working feature broken
- `performance`: Performance issues

### Feature Requests
- `enhancement`: New feature request
- `gaming`: Gaming-related features
- `wine`: Wine bridge improvements
- `ui/ux`: User interface improvements

### Technical
- `build`: Build system issues
- `appimage`: AppImage-specific issues
- `dependencies`: Dependency problems
- `documentation`: Documentation issues

### Priority
- `high`: Critical issues
- `medium`: Important issues
- `low`: Minor issues
- `wontfix`: Issues that won't be fixed

## 🤝 Community Guidelines

### Be Respectful
- Be patient with responses
- Respect different skill levels
- Help other contributors
- Follow the project's code of conduct

### Communication
- Use clear, descriptive language
- Provide context for issues
- Be specific about problems
- Include relevant system information

## 📞 Getting Help

### Before Asking for Help
1. **Check the documentation**
2. **Search existing issues**
3. **Try the troubleshooting guide**
4. **Check the FAQ**

### Where to Get Help
- **Issues**: For bug reports and feature requests
- **Documentation**: `/docs` directory
- **README**: Basic setup and usage
- **Release Notes**: Latest changes and features

## 🎯 Project Goals

LinuxTrack X-IR aims to:
- Provide reliable head tracking for Linux
- Support Windows games through Wine
- Maintain cross-distribution compatibility
- Keep the codebase clean and maintainable
- Foster an open and welcoming community

## 📄 License

By contributing to LinuxTrack X-IR, you agree that your contributions will be licensed under the same license as the project (MIT License).

---

Thank you for contributing to LinuxTrack X-IR! Your help makes this project better for everyone. 
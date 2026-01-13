# Deployment Guide for GoGame

This guide covers multiple ways to deploy and share your GoGame project online.

## Table of Contents
- [Web Deployment (Browser-Based)](#web-deployment-browser-based)
- [GitHub Pages Setup](#github-pages-setup)
- [Release Distribution](#release-distribution)
- [Continuous Integration](#continuous-integration)

---

## Web Deployment (Browser-Based)

GoGame can be compiled to WebAssembly using Emscripten, allowing it to run directly in web browsers.

### Prerequisites

1. **Install Emscripten SDK**
   ```bash
   # Download and install Emscripten
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

2. **Verify Installation**
   ```bash
   emcc --version
   ```

### Building for Web

Use the provided Makefile for web builds:

```bash
# Build the web version
make web

# This creates:
# - index.html (main page)
# - gogame.js (JavaScript glue code)
# - gogame.wasm (WebAssembly binary)
# - gogame.data (embedded assets)
```

### Testing Locally

You cannot open `index.html` directly due to CORS restrictions. Use a local server:

```bash
# Option 1: Python (Python 3)
python -m http.server 8000

# Option 2: Python (Python 2)
python -m SimpleHTTPServer 8000

# Option 3: Node.js (if you have http-server installed)
npx http-server . -p 8000

# Then open: http://localhost:8000
```

### Web Build Configuration

The Emscripten build includes:
- Preloaded assets from the `assets/` directory
- Memory optimizations for browser constraints
- Touch controls for mobile devices
- Responsive canvas sizing

---

## GitHub Pages Setup

Host your game for free on GitHub Pages.

### Method 1: Manual Deployment

1. **Build the web version**
   ```bash
   make web
   ```

2. **Create gh-pages branch**
   ```bash
   git checkout --orphan gh-pages
   git rm -rf .
   ```

3. **Copy web build files**
   ```bash
   cp path/to/index.html .
   cp path/to/gogame.* .
   ```

4. **Commit and push**
   ```bash
   git add .
   git commit -m "Deploy to GitHub Pages"
   git push origin gh-pages
   ```

5. **Enable GitHub Pages**
   - Go to Settings → Pages
   - Source: Deploy from branch
   - Branch: gh-pages
   - Save

Your game will be available at: `https://yourusername.github.io/GoGame/`

### Method 2: Automated with GitHub Actions

The repository includes a GitHub Actions workflow that automatically:
- Builds the web version on every push to main
- Deploys to GitHub Pages
- Creates releases for desktop versions

Simply push your code and the workflow handles the rest!

---

## Release Distribution

### Creating Desktop Releases

1. **Build for Windows**
   ```bash
   make
   # Produces: run.exe
   ```

2. **Package the Release**
   ```bash
   mkdir GoGame-v1.0-Windows
   cp run.exe GoGame-v1.0-Windows/
   cp -r assets GoGame-v1.0-Windows/
   cp README.md GoGame-v1.0-Windows/
   zip -r GoGame-v1.0-Windows.zip GoGame-v1.0-Windows
   ```

3. **Create GitHub Release**
   - Go to Releases → Draft a new release
   - Tag version: v1.0
   - Upload the zip file
   - Publish release

### Automated Releases

The GitHub Actions workflow automatically creates releases when you push a git tag:

```bash
git tag -a v1.0 -m "Version 1.0"
git push origin v1.0
```

This creates a release with:
- Windows executable (.exe)
- Web version (HTML + WASM)
- Linux binary (if built)
- Source code archive

---

## Continuous Integration

### GitHub Actions Workflow

The `.github/workflows/deploy.yml` file provides:

1. **Automated Building**
   - Builds on every push and pull request
   - Tests compilation for multiple platforms

2. **Web Deployment**
   - Automatically deploys to GitHub Pages on main branch
   - Updates the live version instantly

3. **Release Creation**
   - Creates releases on version tags
   - Uploads binaries and web builds

### Monitoring Builds

1. Go to the "Actions" tab in your GitHub repository
2. View build status and logs
3. Download artifacts from successful builds

---

## Platform-Specific Notes

### Windows
- The default `make` builds a Windows executable
- Requires MinGW/MSYS2 environment
- Ships with all necessary DLLs

### Linux
To build for Linux:
```bash
# Install dependencies
sudo apt-get install libraylib-dev

# Build
make linux
```

### macOS
To build for macOS:
```bash
# Install dependencies
brew install raylib

# Build
make macos
```

### Web (All Platforms)
- Runs in any modern browser
- No installation required
- Works on mobile devices
- Hosted for free on GitHub Pages

---

## Troubleshooting

### Emscripten Build Issues

**Error: emcc not found**
```bash
source /path/to/emsdk/emsdk_env.sh
```

**Error: Asset files not found**
- Ensure assets directory is in the same location as your build
- Check the `--preload-file` flag includes all asset directories

**Memory Issues**
- Increase initial memory: `-s INITIAL_MEMORY=67108864`
- Enable memory growth: `-s ALLOW_MEMORY_GROWTH=1`

### GitHub Pages Not Loading

1. Check the GitHub Actions workflow completed successfully
2. Verify GitHub Pages is enabled in Settings
3. Ensure gh-pages branch exists
4. Clear browser cache
5. Check browser console for errors

### Performance Issues

**Desktop Version**
- Already optimized with `-O2` flag
- Runs at 60 FPS on most hardware

**Web Version**
- Use optimization flags: `-O3`
- Enable WebAssembly SIMD if supported
- Reduce asset sizes

---

## Best Practices

### Version Control
- Keep web builds in a separate branch (gh-pages)
- Tag releases with semantic versioning (v1.0.0)
- Don't commit build artifacts to main branch

### Asset Management
- Compress images before including
- Use appropriate audio formats (smaller files for web)
- Preload critical assets only

### Security
- Don't include sensitive data in web builds
- Sanitize user inputs
- Keep dependencies updated

---

## Additional Resources

- [Raylib for Web](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))
- [Emscripten Documentation](https://emscripten.org/docs/getting_started/index.html)
- [GitHub Pages Documentation](https://docs.github.com/en/pages)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)

---

## Support

For issues or questions:
1. Check existing GitHub issues
2. Create a new issue with details
3. Include platform and browser information
4. Attach relevant logs or screenshots

---

**Congratulations!** Your GoGame is now online and accessible to players worldwide! 🎮

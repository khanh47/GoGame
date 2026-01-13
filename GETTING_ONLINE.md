# 🚀 Getting Your GoGame Project Online - Complete Guide

Welcome! This guide will help you bring your GoGame project online and make it accessible to everyone.

## 📋 Table of Contents

1. [Quick Overview](#quick-overview)
2. [Method 1: GitHub Pages (Easiest)](#method-1-github-pages-easiest)
3. [Method 2: Manual Web Deployment](#method-2-manual-web-deployment)
4. [Method 3: Downloadable Releases](#method-3-downloadable-releases)
5. [What Happens Next](#what-happens-next)
6. [Troubleshooting](#troubleshooting)

---

## 🎯 Quick Overview

Your GoGame project can now be deployed in **three ways**:

1. **🌐 Web Version (Browser)** - Play directly in any browser, no installation needed
2. **💾 Desktop Version (Download)** - Traditional executable for Windows/Linux/macOS
3. **🔄 Automated Deployment** - GitHub Actions handles everything automatically

**Best Option**: Use GitHub Pages for instant online access! (Method 1)

---

## Method 1: GitHub Pages (Easiest) ⭐

This is the **recommended** approach - fully automated!

### Prerequisites
✅ GitHub account  
✅ Your code pushed to GitHub repository  

### Step-by-Step Instructions

#### 1️⃣ Enable GitHub Actions Permissions

1. Go to your repository on GitHub
2. Click **Settings** (top navigation)
3. Click **Actions** → **General** (left sidebar)
4. Scroll to "Workflow permissions"
5. Select: ☑️ **Read and write permissions**
6. Click **Save**

#### 2️⃣ Enable GitHub Pages

1. Still in Settings, click **Pages** (left sidebar)
2. Under "Build and deployment":
   - **Source**: Select **GitHub Actions**
3. Click **Save**

#### 3️⃣ Trigger the Deployment

Option A - Just push to main:
```bash
git push origin main
```

Option B - Create a release tag:
```bash
git tag -a v1.0 -m "First release"
git push origin v1.0
```

#### 4️⃣ Wait for Build

1. Go to **Actions** tab in your repository
2. Watch the "Build and Deploy" workflow run
3. Wait 2-5 minutes for completion (first build takes longer)

#### 5️⃣ Access Your Game! 🎉

Your game is now live at:
```
https://[YOUR-USERNAME].github.io/GoGame/
```

For example: `https://khanh47.github.io/GoGame/`

### What Gets Deployed

✅ Fully playable web version  
✅ No installation required  
✅ Works on desktop and mobile  
✅ Automatic updates on every push  
✅ Free hosting forever  

---

## Method 2: Manual Web Deployment 🔧

For custom hosting or local testing.

### Prerequisites

Install Emscripten (WebAssembly compiler):

**Linux/macOS:**
```bash
# Download Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Verify
emcc --version
```

**Windows:**
```bash
# Download Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate
emsdk install latest
emsdk activate latest
emsdk_env.bat

# Verify
emcc --version
```

### Build Steps

1. **Navigate to your project**
   ```bash
   cd /path/to/GoGame
   ```

2. **Build for web**
   ```bash
   make web
   ```

3. **Find your build**
   ```bash
   cd web_build
   ls
   ```
   
   You should see:
   - `gogame.html` - Main page
   - `gogame.js` - JavaScript code
   - `gogame.wasm` - WebAssembly binary
   - `gogame.data` - Game assets

4. **Test locally**
   ```bash
   # Using Python 3
   python -m http.server 8000
   
   # Using Python 2
   python -m SimpleHTTPServer 8000
   
   # Using Node.js
   npx http-server . -p 8000
   ```

5. **Open in browser**
   ```
   http://localhost:8000
   ```

### Deploy to Hosting

Upload the `web_build` contents to any of these services:

**Free Options:**
- GitHub Pages (via Method 1)
- Netlify: Drag & drop the web_build folder
- Vercel: Connect your GitHub repo
- Render: Connect your GitHub repo
- Surge.sh: `surge web_build`

**Your Own Server:**
- Upload via FTP/SFTP
- Use rsync: `rsync -avz web_build/ user@server:/var/www/html/`
- Copy to web directory

---

## Method 3: Downloadable Releases 📦

Create downloadable executables for users.

### Automatic Release (Recommended)

1. **Tag your version**
   ```bash
   git tag -a v1.0 -m "Release version 1.0"
   git push origin v1.0
   ```

2. **Wait for GitHub Actions**
   - Builds Windows executable
   - Builds web version
   - Creates GitHub release
   - Uploads both as downloadable files

3. **Access releases**
   ```
   https://github.com/[USERNAME]/GoGame/releases
   ```

### Manual Release

1. **Build Windows version**
   ```bash
   make run.exe
   ```

2. **Create package**
   ```bash
   mkdir GoGame-v1.0-Windows
   cp run.exe GoGame-v1.0-Windows/
   cp -r assets GoGame-v1.0-Windows/
   cp README.md GoGame-v1.0-Windows/
   zip -r GoGame-v1.0-Windows.zip GoGame-v1.0-Windows
   ```

3. **Upload to GitHub**
   - Go to repository → Releases
   - Click "Draft a new release"
   - Upload the zip file
   - Publish release

---

## 🎊 What Happens Next

### After Deploying to GitHub Pages:

✅ **Automatic Updates**: Every push to `main` updates your live game  
✅ **Version Control**: GitHub tracks all changes  
✅ **Free CDN**: Fast loading worldwide  
✅ **HTTPS**: Secure connection automatically  
✅ **Custom Domain**: Optional - add your own domain  

### Your Game URLs:

- **Play Online**: `https://[username].github.io/GoGame/`
- **Source Code**: `https://github.com/[username]/GoGame`
- **Releases**: `https://github.com/[username]/GoGame/releases`
- **Documentation**: Available at repository root

### Share Your Game! 🎮

Share these links with:
- Friends and family
- Social media (Twitter, Reddit, LinkedIn)
- Gaming communities
- Programming communities
- Academic presentations
- Portfolio/resume

**Social Media Template:**
```
🎮 Check out my Go board game implementation!

Built with C++ and Raylib, runs in your browser via WebAssembly

Play: https://[username].github.io/GoGame/
Code: https://github.com/[username]/GoGame

Features:
✅ Multiple board sizes
✅ AI opponent
✅ Undo/Redo system
✅ Save/Load games

#gamedev #cpp #raylib #webassembly
```

---

## 🐛 Troubleshooting

### GitHub Actions Won't Run

**Problem**: Workflow doesn't trigger  
**Solution**:
1. Settings → Actions → General
2. Enable "Allow all actions and reusable workflows"
3. Set permissions to "Read and write"

### GitHub Pages Shows 404

**Problem**: Page not found  
**Solution**:
1. Settings → Pages → Source: "GitHub Actions"
2. Wait 5 minutes after first deploy
3. Check Actions tab for errors
4. Verify workflow completed successfully

### Web Build Fails

**Problem**: `make web` doesn't work  
**Solution**:
```bash
# Re-source Emscripten
source /path/to/emsdk/emsdk_env.sh

# Verify
emcc --version

# Try again
make clean-web
make web
```

### Game Won't Load in Browser

**Problem**: Black screen or loading forever  
**Solutions**:
1. Check browser console (F12) for errors
2. Try different browser (Chrome/Firefox recommended)
3. Clear browser cache
4. Ensure you're using a local server (not file://)
5. Check that all .wasm, .js, .data files are present

### Performance Issues

**Web Version:**
- Enable hardware acceleration in browser
- Close other tabs
- Try different browser
- Check console for WebGL errors

**Desktop Version:**
- Already optimized with -O2 flag
- Should run 60 FPS on most hardware

### Assets Not Loading

**Problem**: Missing textures or sounds  
**Solution**:
```bash
# Ensure assets directory exists
ls assets/

# Rebuild with assets
make clean-web
make web

# Check the .data file was created
ls web_build/gogame.data
```

---

## 📚 Additional Documentation

Detailed guides for specific topics:

- **[QUICK_START.md](QUICK_START.md)** - Fast setup guide
- **[DEPLOYMENT.md](DEPLOYMENT.md)** - Comprehensive deployment options
- **[README.md](README.md)** - Full project documentation

### External Resources

- [Raylib for Web](https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5))
- [Emscripten Docs](https://emscripten.org/docs/getting_started/index.html)
- [GitHub Pages Guide](https://docs.github.com/en/pages)
- [GitHub Actions Tutorial](https://docs.github.com/en/actions)

---

## ✅ Success Checklist

Mark these off as you complete them:

- [ ] Code pushed to GitHub
- [ ] GitHub Actions enabled with write permissions
- [ ] GitHub Pages enabled with "GitHub Actions" source
- [ ] Workflow ran successfully (check Actions tab)
- [ ] Game loads at `https://[username].github.io/GoGame/`
- [ ] Game is playable and responsive
- [ ] Shared with friends! 🎉

---

## 🎯 Summary

**Easiest Path to Online:**
1. Enable GitHub Actions (Settings → Actions → Permissions)
2. Enable GitHub Pages (Settings → Pages → Source: GitHub Actions)
3. Push to main branch
4. Wait 2-5 minutes
5. Play at `https://[username].github.io/GoGame/`

**That's it!** Your game is now online and accessible to everyone worldwide! 🌍

---

## 💬 Need Help?

If you encounter issues:

1. Check the [Troubleshooting](#troubleshooting) section above
2. Review GitHub Actions logs (Actions tab)
3. Check browser console (F12) for errors
4. Open an issue on GitHub with details
5. Include error messages and screenshots

---

**Congratulations on bringing your GoGame project online! 🚀🎮**

Enjoy sharing your creation with the world!

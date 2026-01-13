# Quick Start: Bring GoGame Online

This guide will help you get your GoGame project online in minutes!

## 🚀 Fastest Way: GitHub Pages (Recommended)

### Step 1: Enable GitHub Actions

Your repository now includes a GitHub Actions workflow. Simply:

1. Go to your repository on GitHub
2. Click on **Settings** → **Actions** → **General**
3. Under "Workflow permissions", select **Read and write permissions**
4. Click **Save**

### Step 2: Enable GitHub Pages

1. Still in Settings, click on **Pages** (left sidebar)
2. Under "Build and deployment":
   - Source: Select **GitHub Actions**
3. Click **Save**

### Step 3: Trigger the Build

Push any commit to the `main` branch, and the workflow will:
- Automatically build the web version
- Deploy to GitHub Pages
- Make your game available at: `https://[your-username].github.io/GoGame/`

**That's it!** Your game is now online! 🎉

---

## 🌐 Manual Web Deployment

If you want to build and deploy manually:

### 1. Install Emscripten

```bash
# Clone Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # Linux/Mac
# OR
emsdk_env.bat          # Windows
```

### 2. Build for Web

```bash
cd /path/to/GoGame
make web
```

### 3. Test Locally

```bash
cd web_build
python -m http.server 8000
```

Open http://localhost:8000 in your browser.

### 4. Deploy to Any Web Host

Upload the contents of `web_build/` to any web hosting service:
- GitHub Pages
- Netlify
- Vercel
- Your own server
- Any static hosting service

---

## 📦 Creating Downloadable Releases

### Using GitHub Releases

1. **Tag a Version**
   ```bash
   git tag -a v1.0 -m "Release version 1.0"
   git push origin v1.0
   ```

2. **Automatic Build**
   - GitHub Actions will automatically:
     - Build Windows executable
     - Build web version
     - Create a release
     - Upload both versions as downloadable files

3. **Download**
   - Users can download from: `https://github.com/[username]/GoGame/releases`

### Manual Release

1. **Build Windows Version**
   ```bash
   make run.exe
   ```

2. **Package**
   ```bash
   mkdir GoGame-v1.0-Windows
   cp run.exe GoGame-v1.0-Windows/
   cp -r assets GoGame-v1.0-Windows/
   cp README.md GoGame-v1.0-Windows/
   zip -r GoGame-v1.0-Windows.zip GoGame-v1.0-Windows
   ```

3. **Upload to GitHub**
   - Go to Releases → Draft a new release
   - Upload the zip file
   - Publish!

---

## 🎮 Where to Share Your Game

Once online, share your game at:

1. **Direct Play**: `https://[username].github.io/GoGame/`
2. **Source Code**: `https://github.com/[username]/GoGame`
3. **Releases**: `https://github.com/[username]/GoGame/releases`

### Social Media Template

```
🎮 Play my Go board game implementation!

Built with C++ and Raylib, compiled to WebAssembly

🕹️ Play: https://[username].github.io/GoGame/
💻 Code: https://github.com/[username]/GoGame

Features:
✅ Multiple board sizes (9x9, 13x13, 19x19)
✅ AI opponent with Minimax algorithm  
✅ Undo/Redo system
✅ Save/Load game states
✅ Runs in your browser!

#gamedev #cpp #raylib #webassembly #go #weiqi
```

---

## 🐛 Troubleshooting

### GitHub Actions Not Running

1. Check if workflows are enabled: Settings → Actions
2. Check workflow permissions: Settings → Actions → General
3. Ensure `.github/workflows/deploy.yml` exists in your repo

### GitHub Pages Not Loading

1. Verify GitHub Pages is enabled: Settings → Pages
2. Check that source is set to "GitHub Actions"
3. Wait a few minutes after the first deployment
4. Check the Actions tab for build errors

### Web Build Fails Locally

1. Ensure Emscripten is installed: `emcc --version`
2. Source the environment: `source /path/to/emsdk/emsdk_env.sh`
3. Check that all source files exist
4. Ensure assets directory is present

### Performance Issues

**Desktop**: Already optimized! Should run at 60 FPS.

**Web**: 
- Try a different browser (Chrome/Firefox recommended)
- Close other tabs
- Check browser console for errors
- Ensure hardware acceleration is enabled

---

## 📚 Additional Resources

- **Full Deployment Guide**: [DEPLOYMENT.md](DEPLOYMENT.md)
- **Main Documentation**: [README.md](README.md)
- **Raylib Web Guide**: https://github.com/raysan5/raylib/wiki/Working-for-Web-(HTML5)
- **Emscripten Docs**: https://emscripten.org/docs/getting_started/index.html
- **GitHub Actions**: https://docs.github.com/en/actions
- **GitHub Pages**: https://docs.github.com/en/pages

---

## ✅ Checklist

- [ ] Repository pushed to GitHub
- [ ] GitHub Actions enabled with write permissions
- [ ] GitHub Pages enabled with "GitHub Actions" source
- [ ] First workflow run successful
- [ ] Game accessible at GitHub Pages URL
- [ ] Shared with friends! 🎉

---

**Congratulations!** Your GoGame is now online and ready to share with the world! 🚀

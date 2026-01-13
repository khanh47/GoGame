# ✅ GoGame Online Deployment Checklist

Use this checklist to deploy your GoGame project online step by step.

## 📋 Pre-Deployment Checklist

### Repository Setup
- [ ] Code is committed to a GitHub repository
- [ ] Repository is public (or you have GitHub Pro for private Pages)
- [ ] You have admin access to the repository
- [ ] All source files are present in `src/` and `include/`
- [ ] Assets directory exists at `assets/`
- [ ] Game builds and runs locally

### Local Testing (Optional but Recommended)
- [ ] Run `make` to test desktop build
- [ ] Game launches and is playable
- [ ] No missing textures or assets
- [ ] No compilation errors

---

## 🚀 GitHub Actions Setup

### Step 1: Enable GitHub Actions
1. - [ ] Go to your repository on GitHub
2. - [ ] Click **Settings** (top navigation)
3. - [ ] Click **Actions** → **General** (left sidebar)
4. - [ ] Under "Workflow permissions":
   - [ ] Select: **Read and write permissions**
5. - [ ] Click **Save**

### Step 2: Verify Workflow File
- [ ] File exists: `.github/workflows/deploy.yml`
- [ ] File is committed to repository
- [ ] YAML syntax is valid (check for any errors)

---

## 🌐 GitHub Pages Setup

### Step 1: Enable GitHub Pages
1. - [ ] In Settings, click **Pages** (left sidebar)
2. - [ ] Under "Build and deployment":
   - [ ] **Source**: Select **GitHub Actions**
3. - [ ] Click **Save**

### Step 2: Note Your URL
- [ ] Your game will be at: `https://[YOUR-USERNAME].github.io/GoGame/`
- [ ] Write it down: ________________________________

---

## 📤 Deploy Your Game

### Method A: Push to Main (Recommended)
1. - [ ] Ensure you're on the main branch: `git branch`
2. - [ ] Stage your changes: `git add .`
3. - [ ] Commit: `git commit -m "Deploy GoGame online"`
4. - [ ] Push: `git push origin main`

### Method B: Create a Release Tag
1. - [ ] Create tag: `git tag -a v1.0 -m "First release"`
2. - [ ] Push tag: `git push origin v1.0`
3. - [ ] This creates both web deployment AND downloadable release

---

## ⏱️ Wait for Build

### Monitor the Workflow
1. - [ ] Go to your repository on GitHub
2. - [ ] Click the **Actions** tab
3. - [ ] Find "Build and Deploy" workflow
4. - [ ] Watch it run (takes 2-5 minutes)
5. - [ ] Wait for green checkmark ✅

### What's Happening?
- [ ] Building Windows version (1-2 min)
- [ ] Building Web version (1-2 min)
- [ ] Deploying to GitHub Pages (1 min)
- [ ] Creating release (if you used a tag)

---

## 🎮 Test Your Game

### Access the Game
1. - [ ] Open: `https://[YOUR-USERNAME].github.io/GoGame/`
2. - [ ] Wait for game to load (may take 10-30 seconds first time)
3. - [ ] See the styled loading screen
4. - [ ] Game loads successfully

### Test Functionality
- [ ] Game board displays correctly
- [ ] Can place stones
- [ ] Menu navigation works
- [ ] Assets load (images, sounds)
- [ ] No console errors (press F12 to check)

### Test on Different Devices
- [ ] Desktop browser (Chrome/Firefox/Edge)
- [ ] Mobile browser (if accessible)
- [ ] Tablet (if available)

---

## 📱 Share Your Game

### Get Your Links
- [ ] **Play Online**: `https://[username].github.io/GoGame/`
- [ ] **Source Code**: `https://github.com/[username]/GoGame`
- [ ] **Releases**: `https://github.com/[username]/GoGame/releases`

### Share On
- [ ] Twitter/X
- [ ] Reddit (r/gamedev, r/programming)
- [ ] LinkedIn
- [ ] Discord communities
- [ ] Facebook
- [ ] Friends and family
- [ ] Your portfolio/resume

### Social Media Post Template
```
🎮 Check out my Go board game implementation!

Built with C++ and Raylib, compiled to WebAssembly

▶️ Play: https://[username].github.io/GoGame/
💻 Code: https://github.com/[username]/GoGame

Features:
✅ Multiple board sizes (9×9, 13×13, 19×19)
✅ AI opponent with Minimax algorithm
✅ Undo/Redo system
✅ Runs in your browser!

#gamedev #cpp #raylib #webassembly #go
```

---

## 🔧 Troubleshooting

### Workflow Won't Run
- [ ] Check: Settings → Actions → Workflows are enabled
- [ ] Check: Settings → Actions → Permissions set to "Read and write"
- [ ] Try: Manual workflow run (Actions → Build and Deploy → Run workflow)

### Workflow Fails
- [ ] Click on failed workflow in Actions tab
- [ ] Read the error message
- [ ] Check common issues:
  - [ ] All source files present?
  - [ ] Assets directory exists?
  - [ ] No syntax errors in code?
  - [ ] Web templates (web/shell.html) exist?

### GitHub Pages Shows 404
- [ ] Settings → Pages → Source is "GitHub Actions"
- [ ] Workflow completed successfully (green checkmark)
- [ ] Wait 5 minutes after first deploy
- [ ] Try clearing browser cache
- [ ] Check URL is exactly: `https://[username].github.io/GoGame/`

### Game Loads but Crashes
- [ ] Open browser console (F12)
- [ ] Look for error messages
- [ ] Common issues:
  - [ ] WebGL not supported? Try different browser
  - [ ] Assets not loading? Check network tab
  - [ ] Out of memory? Reduce asset sizes

### Game Loads Black Screen
- [ ] Wait 30 seconds (might still be loading)
- [ ] Check browser console for errors
- [ ] Ensure browser supports WebAssembly
- [ ] Try Chrome or Firefox (best compatibility)

---

## 🔄 Making Updates

### Update Your Game
1. - [ ] Make changes to code locally
2. - [ ] Test with `make`
3. - [ ] Commit: `git commit -am "Update: description"`
4. - [ ] Push: `git push origin main`
5. - [ ] Wait 2-5 minutes
6. - [ ] Refresh your game URL
7. - [ ] Changes are live!

### Create New Release
1. - [ ] Tag version: `git tag -a v1.1 -m "Version 1.1"`
2. - [ ] Push: `git push origin v1.1`
3. - [ ] Check Releases page
4. - [ ] Downloads available automatically

---

## 🎯 Optional Enhancements

### Add Custom Domain
- [ ] Buy domain name
- [ ] Settings → Pages → Custom domain
- [ ] Configure DNS records
- [ ] Enable HTTPS

### Add Analytics
- [ ] Set up Google Analytics
- [ ] Add tracking code to `web/shell.html`
- [ ] Rebuild: `make web`
- [ ] Deploy

### Customize Appearance
- [ ] Edit `web/shell.html`
- [ ] Change colors, styles, text
- [ ] Test locally: `make web`
- [ ] Push to deploy

---

## ✅ Success Criteria

You've successfully deployed when:
- [ ] Workflow runs without errors
- [ ] Green checkmark in Actions tab
- [ ] Game loads at GitHub Pages URL
- [ ] Game is fully playable in browser
- [ ] No console errors
- [ ] Tested on multiple browsers
- [ ] Shared with at least one person

---

## 📚 Documentation Reference

Need more help? Check these guides:

- **START_HERE.md** - Quick overview
- **GETTING_ONLINE.md** - Detailed deployment guide
- **QUICK_START.md** - Fast 5-minute setup
- **DEPLOYMENT.md** - Technical documentation
- **ARCHITECTURE.md** - System diagrams
- **README.md** - Project overview

---

## 🎉 Congratulations!

If you've checked all the boxes above, your GoGame is now:
- ✅ Online and accessible worldwide
- ✅ Automatically deployed on every push
- ✅ Hosted for free on GitHub Pages
- ✅ Ready to share with everyone

**You did it! Now go share your game! 🚀🎮**

---

**Questions or issues?** 
- Check the troubleshooting section
- Review GETTING_ONLINE.md for detailed help
- Open an issue on GitHub

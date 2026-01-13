# GoGame Deployment Architecture

## Deployment Flow Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                      YOUR LOCAL MACHINE                          │
│                                                                  │
│  ┌──────────────┐                                               │
│  │   Source     │                                               │
│  │   Code       │                                               │
│  │  (C++/Raylib)│                                               │
│  └──────┬───────┘                                               │
│         │                                                        │
│         │ git push                                               │
└─────────┼────────────────────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────────────────────┐
│                         GITHUB                                   │
│                                                                  │
│  ┌──────────────────────────────────────────────────────┐      │
│  │           GitHub Actions Workflow                     │      │
│  │                                                        │      │
│  │  ┌───────────────┐         ┌──────────────┐         │      │
│  │  │ Build Windows │         │  Build Web   │         │      │
│  │  │   (MSYS2)     │         │ (Emscripten) │         │      │
│  │  │               │         │              │         │      │
│  │  │   run.exe    │         │  .wasm + .js │         │      │
│  │  └───────┬───────┘         └──────┬───────┘         │      │
│  │          │                        │                  │      │
│  │          │                        │                  │      │
│  │          ▼                        ▼                  │      │
│  │  ┌──────────────┐        ┌──────────────┐          │      │
│  │  │   Artifact   │        │ GitHub Pages │          │      │
│  │  │   Storage    │        │  Deployment  │          │      │
│  │  └──────┬───────┘        └──────┬───────┘          │      │
│  │         │                       │                   │      │
│  └─────────┼───────────────────────┼───────────────────┘      │
│            │                       │                           │
│            ▼                       ▼                           │
│   ┌─────────────────┐    ┌──────────────────┐                │
│   │ GitHub Releases │    │  GitHub Pages    │                │
│   │   (Downloads)   │    │  (Live Website)  │                │
│   └─────────────────┘    └──────────────────┘                │
└─────────────────────────────────────────────────────────────────┘
          │                         │
          │                         │
          ▼                         ▼
┌──────────────────┐      ┌──────────────────┐
│  Users Download  │      │  Users Play in   │
│  run.exe         │      │  Browser         │
│  (Desktop)       │      │  (Web)           │
└──────────────────┘      └──────────────────┘
```

## Three Deployment Paths

### Path 1: Automated Web Deployment (Recommended)
```
Local Code → git push → GitHub Actions → Build Web → Deploy to Pages → Live!
Time: ~5 minutes after push
URL: https://[username].github.io/GoGame/
```

### Path 2: Manual Web Build
```
Local Code → Install Emscripten → make web → Upload to Host → Live!
Time: ~30 minutes first time
Hosting: Any static host (Netlify, Vercel, etc.)
```

### Path 3: Release Downloads
```
Local Code → git tag v1.0 → GitHub Actions → Build Binaries → Release → Downloads!
Time: ~10 minutes after tag
Access: GitHub Releases page
```

## Technology Stack

```
┌─────────────────────────────────────────────────────────┐
│                    DESKTOP VERSION                       │
├─────────────────────────────────────────────────────────┤
│  Language:    C++20                                     │
│  Graphics:    Raylib                                    │
│  Platform:    Windows/Linux/macOS                       │
│  Compiler:    GCC/Clang                                 │
│  Build:       Make                                      │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                      WEB VERSION                         │
├─────────────────────────────────────────────────────────┤
│  Language:    C++20 → WebAssembly                       │
│  Graphics:    Raylib → WebGL                            │
│  Platform:    Any modern browser                        │
│  Compiler:    Emscripten                                │
│  Build:       Make + Emscripten                         │
│  Output:      .wasm + .js + .html                       │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                    CI/CD PIPELINE                        │
├─────────────────────────────────────────────────────────┤
│  Platform:    GitHub Actions                            │
│  Triggers:    Push to main, Tags, PRs                   │
│  Jobs:        Build Windows, Build Web, Deploy, Release │
│  Artifacts:   Executables, Web builds                   │
└─────────────────────────────────────────────────────────┘
```

## File Structure After Setup

```
GoGame/
│
├── 📄 Documentation
│   ├── START_HERE.md          ⭐ Start here!
│   ├── GETTING_ONLINE.md      (Detailed deployment guide)
│   ├── QUICK_START.md         (5-minute guide)
│   ├── DEPLOYMENT.md          (Technical details)
│   └── README.md              (Project overview)
│
├── ⚙️ Configuration
│   ├── Makefile               (Build system + web target)
│   ├── .gitignore             (Exclude build artifacts)
│   └── .github/workflows/
│       └── deploy.yml         (CI/CD automation)
│
├── 🌐 Web Templates
│   └── web/
│       ├── shell.html         (Custom styled template)
│       ├── index_template.html(Alternative template)
│       ├── index.html         (Redirect page)
│       └── README.md          (Web docs)
│
├── 💻 Source Code
│   ├── src/                   (C++ source files)
│   ├── include/               (Header files)
│   └── assets/                (Game assets)
│
└── 🏗️ Build Output (gitignored)
    ├── run.exe                (Desktop build)
    └── web_build/             (Web build)
        ├── gogame.html
        ├── gogame.js
        ├── gogame.wasm
        └── gogame.data
```

## Workflow Triggers

```
┌─────────────────┐
│ Trigger Events  │
└────────┬────────┘
         │
         ├─► Push to main     → Build & Deploy to Pages
         │
         ├─► Push tag v*      → Build & Create Release
         │
         ├─► Pull Request     → Build & Test only
         │
         └─► Manual Dispatch  → Build on demand
```

## Deployment Checklist

```
Setup Phase:
☐ Code in GitHub repository
☐ Enable GitHub Actions (Settings → Actions)
☐ Set write permissions (Settings → Actions → General)
☐ Enable GitHub Pages (Settings → Pages)
☐ Set source to "GitHub Actions"

Deployment Phase:
☐ Push to main branch
☐ Watch workflow in Actions tab
☐ Wait 2-5 minutes for build
☐ Check deployment success
☐ Test game at GitHub Pages URL

Optional:
☐ Add custom domain
☐ Create release tags
☐ Monitor analytics
☐ Share with community
```

## URL Mapping

```
Repository URL:
https://github.com/[username]/GoGame

Live Game URL:
https://[username].github.io/GoGame/

Releases URL:
https://github.com/[username]/GoGame/releases

Actions (Build Status):
https://github.com/[username]/GoGame/actions
```

## Support & Resources

- **Quick Help**: START_HERE.md
- **Full Guide**: GETTING_ONLINE.md  
- **Technical**: DEPLOYMENT.md
- **Fast Setup**: QUICK_START.md
- **Project Info**: README.md

---

**Ready to deploy?** Open START_HERE.md and follow the steps!

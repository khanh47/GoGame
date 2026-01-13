# Web Build Templates

This directory contains templates for building GoGame as a web application.

## Files

- **shell.html** - Custom HTML shell with styled loading screen and game info
- **index_template.html** - Alternative standalone HTML template

## Usage

These templates are used by Emscripten when building the web version:

```bash
make web
```

The build process uses `shell.html` as the template, which provides:
- Styled loading screen
- Game controls information
- Responsive layout
- Mobile support
- Links to documentation

## Customization

You can modify these templates to customize:
- Styling and colors
- Loading messages
- Game information
- Links and branding
- Layout and responsiveness

After modifying, rebuild with `make web` to see changes.

---
header-includes: |
    \setlength{\parskip}{1em plus 2pt minus 1pt}
    \usepackage{titlesec}
    \newcommand{\sectionbreak}{\clearpage}
    \renewcommand{\familydefault}{\sfdefault}
geometry: margin=1.2in
papersize: a4
fontfamily: helvet
linestretch: 1.15
numbersections: true
---

# Installation instructions

This project uses `pandoc` to generate pdfs from markdown documents. In order to use this, you will first have to install pandoc and texlive.

## Windows 

1. Install `pandoc` using the [windows installer](https://github.com/jgm/pandoc/releases/download/2.18/pandoc-2.18-windows-x86_64.msi).
2. Install `texlive` using the [windows installer](https://mirror.ctan.org/systems/texlive/tlnet/install-tl-windows.exe). Uncheck the "Install TeXworks front end".
3. Right click `.tools/watch.ps1`, copy, Win + R, `shell:startup`, right click, paste as shortcut.
4. Right click windows icon, select PowerShell, and enable script execution: `Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Unrestricted`
5. Reboot or run the powershell script. This will keep running in the background and will convert `.md` files to `.pdf` (when they change).
6. Optional: Install SumatraPDF as your pdf viewer. This viewer will automatically reload the pdf when it changes. If you install this, you might want to disable the notifications in `.tools/settings.txt`.
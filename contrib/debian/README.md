
Debian
====================
This directory contains files used to package voyacoind/voyacoin-qt
for Debian-based Linux systems. If you compile voyacoind/voyacoin-qt yourself, there are some useful files here.

## voyacoin: URI support ##


voyacoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install voyacoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your voyacoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/voyacoin128.png` to `/usr/share/pixmaps`

voyacoin-qt.protocol (KDE)


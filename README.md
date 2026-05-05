# Global QSS

This engine allows theming of Qt applications using QSS files, similar to how GTK applications are themed with CSS files.

It finds the theme name from the environment variable `GQSS_THEME`, or from MATE settings, or from GNOME settings. It supports theme reload on desktop theme change (via DBus).

## Tips

Like CSS for GTK isn't CSS, QSS for Qt isn't CSS, [read the docs](https://doc.qt.io/qt-6/stylesheet-reference.html).

- Use `image: url("img/menu-checkbox-checked.svg");` for theme images
- Use `image: url('/usr/share/icons/Adwaita/symbolic/ui/pan-down-symbolic.svg');` for system images

For an example, see [awf-qt](https://github.com/luigifab/awf-extended) and [human-theme](https://github.com/luigifab/human-theme).

## Installation

It requires **Qt 5.15** or **Qt 6.0+** *(including 6.11)*.

#### Installation for Debian, Devuan, Ubuntu, Trisquel, Linux Mint, MX Linux

* `sudo apt install qt5-style-globalqss qt6-style-globalqss`

#### Installation for Fedora

* `sudo dnf install qt5-globalqss qt6-globalqss`

#### Installation for openSUSE

* `sudo zypper install qt5-globalqss qt6-globalqss`

#### Installation for Mageia

* `sudo urpmi qt5-globalqss qt6-globalqss`

#### Building from source

* To compile with Qt 5 & 6 run: `build.sh`
* To compile and create DEB packages for Debian & Ubuntu run: `scripts/debian/deb.sh`
* To compile and create RPM packages for Fedora run: `scripts/fedora/rpm.sh`
* To compile and create RPM packages for Mageia run: `scripts/mageia/rpm.sh`
* To compile and create RPM packages for openSUSE run: `scripts/opensuse/rpm.sh`

#### Alternative installation for Debian, Devuan, Ubuntu, Trisquel, Linux Mint, MX Linux

```bash
# PPA: https://launchpad.net/~luigifab/+archive/ubuntu/packages
# from Debian 12 you can use noble/questing (unix.stackexchange.com/a/669008/364800)
#     Debian: focal for 10/buster, jammy for 11/bullseye, noble for 12/bookworm,
#              plucky for 13/trixie, questing for 14/forky and 15/duke
#     Devuan: focal for 3/beowulf, jammy for 4/chimaera, noble for 5/daedalus,
#              plucky for 6/excalibur, questing for 7/freia and 8/gryphon and ceres
# Linux Mint: focal for 20.x and 4/debbie, jammy for 21.x and 5/elsie,
#              noble for 22.x and 6/faye, plucky for 7/gigi
#   MX Linux: focal for 19.x, jammy for 21.x, noble for 23.x, questing for 25.x
#   Trisquel: focal for nabia, jammy for aramo
# Kali Linux: questing

sudo add-apt-repository ppa:luigifab/packages
sudo apt update
sudo apt install qt5-style-globalqss qt6-style-globalqss
# or
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys FFE5BD439356DF7D
echo "deb http://ppa.launchpad.net/luigifab/packages/ubuntu focal main" | sudo tee -a /etc/apt/sources.list
sudo apt update
sudo apt install qt5-style-globalqss qt6-style-globalqss
# or
sudo wget -O /etc/apt/trusted.gpg.d/luigifab.gpg https://www.luigifab.fr/apt.gpg
echo "deb http://ppa.launchpad.net/luigifab/packages/ubuntu focal main" | sudo tee -a /etc/apt/sources.list
sudo apt update
sudo apt install qt5-style-globalqss qt6-style-globalqss
# or
wget -qO - https://www.luigifab.fr/apt.gpg | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/luigifab.gpg
echo "deb http://ppa.launchpad.net/luigifab/packages/ubuntu focal main" | sudo tee -a /etc/apt/sources.list
sudo apt update
sudo apt install qt5-style-globalqss qt6-style-globalqss

# sha256sum /etc/apt/trusted.gpg.d/luigifab.gpg
578c89a677048e38007462d543686b53587efba9f93814601169253c45ff9213
# apt-key list
/etc/apt/trusted.gpg.d/luigifab.gpg
pub   rsa4096 2020-10-31 [SC]
      458B 0C46 D024 FD8C B8BC  99CD FFE5 BD43 9356 DF7D
```

## Known issues & Dev

Sometimes, `Segmentation fault` can appear.

You can set `GQSS_DEBUG=1` from command line to enable debug mode.\
When the plugin has applied the theme, `GQSS_READY=yes` is set.

From any program, you can reload the theme with:
```c++
	if (qEnvironmentVariableIsSet("GQSS_SET")) {
		//qputenv("GQSS_THEME", "[newThemeName]");
		qputenv("GQSS_RELOAD", "yes");
		QApplication::style()->polish(qApp);
		QApplication::processEvents();
		window->adjustSize();
	}
```

On Fedora with Qt 5, the GTK 3 platform theme _(qt5-qtbase-gui)_ works better than on Debian _(qt5-gtk-platformtheme)_, but the fonts remain blurry, see [bug 2459509](https://bugzilla.redhat.com/show_bug.cgi?id=2459509).

#### VLC

The customize interface dialog can crash the program.

#### OpenShot

Stay on version 3.1.1 to use the system theme.

## Copyright

- Current version: 1.1.0 (05/05/2026)
- Compatibility: Qt 5.15 / 6.0..6.11
- Links: [luigifab.fr](https://www.luigifab.fr/gtkqt/globalqss) - [github.com](https://github.com/luigifab/globalqss) - [ppa/dpa](https://launchpad.net/~luigifab/+archive/ubuntu/packages)

This program is provided under the terms of the **GNU GPLv2+** license.\
If you like, take some of your time to improve the translations, go to https://bit.ly/2HyCCEc.

## Packages in official distros repositories

[![Packages status](https://repology.org/badge/vertical-allrepos/globalqss.svg?header=globalqss)](https://repology.org/project/globalqss/versions)

# This engine allows theming of Qt 5/6 applications using QSS files,
# similar to how GTK applications are themed with CSS files.
#
# Files (*.qss) comes from:
#                ~/.themes/[themeName]/qt5/* + ~/.themes/[themeName]/qt6/*
# or when not found from:
#        /usr/share/themes/[themeName]/qt5/* + /usr/share/themes/[themeName]/qt6/*
# or when not found from:
#  /usr/local/share/themes/[themeName]/qt5/* + /usr/local/share/themes/[themeName]/qt6/*
#
# where [themeName] is GQSS_THEME=xyz or current MATE GTK theme or current GNOME GTK theme
#
# You can override with:
#      ~/.config/qt5/qt.qss     + ~/.config/qt6/qt.qss
#      ~/.config/qt5/qt-rtl.qss + ~/.config/qt6/qt-rtl.qss
#
# export GQSS_THEME=xyz
# export GQSS_DEBUG=1
export QT_STYLE_OVERRIDE=GlobalQSS

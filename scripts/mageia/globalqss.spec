%bcond qt5 1
%bcond qt6 1

Name:          globalqss
Version:       1.0.0
Release:       %mkrel 1
Summary:       GlobalQSS style engine for Qt
Summary(fr):   Moteur de style GlobalQSS pour Qt
License:       GPLv2+
Group:         Todo
URL:           https://github.com/luigifab/globalqss
Source0:       %{url}/archive/v%{version}/%{name}-%{version}.tar.gz

BuildRequires: aspell-fr
BuildRequires: cmake
BuildRequires: gcc-c++
%if %{with qt5}
BuildRequires: qt5-devel
Requires:      qt5
Recommends:    qt5-svg
%endif
%if %{with qt6}
BuildRequires: qt6-devel
Requires:      qt6
Recommends:    qt6-svg
%endif

%if %{with qt5}
%package -n qt5-globalqss
Summary:       GlobalQSS style engine for Qt 5
Summary(fr):   Moteur de style GlobalQSS pour Qt 5
%endif

%if %{with qt6}
%package -n qt6-globalqss
Summary:       GlobalQSS style engine for Qt 6
Summary(fr):   Moteur de style GlobalQSS pour Qt 6
%endif


%description %{expand:
This engine allows theming of Qt applications using QSS files,
similar to how GTK applications are themed with CSS files.

After installation you must restart your session.
After uninstallation be sure to remove the config file.}

%description -l fr %{expand:
Ce moteur de style permet de personnaliser le thème des applications Qt
à l'aide de fichiers QSS, de la même manière que les applications
GTK sont personnalisées à l'aide de fichiers CSS.

Après l'installation vous devez redémarrer votre session.
Après la désinstallation, veillez à supprimer le fichier de config.}


%if %{with qt5}
%description -n qt5-globalqss %{expand:
This engine allows theming of Qt applications using QSS files,
similar to how GTK applications are themed with CSS files.

This package provides the program for Qt 5.

After installation you must restart your session.
After uninstallation be sure to remove the config file:
 /etc/profile.d/qt5-style-globalqss.sh}

%description -n qt5-globalqss -l fr %{expand:
Ce moteur de style permet de personnaliser le thème des applications Qt
à l'aide de fichiers QSS, de la même manière que les applications
GTK sont personnalisées à l'aide de fichiers CSS.

Ce paquet fournit le programme pour Qt 5.

Après l'installation vous devez redémarrer votre session.
Après la désinstallation, veillez à supprimer le fichier de config :
 /etc/profile.d/qt5-style-globalqss.sh}
%endif


%if %{with qt6}
%description -n qt6-globalqss %{expand:
This engine allows theming of Qt applications using QSS files,
similar to how GTK applications are themed with CSS files.

This package provides the program for Qt 6.

After installation you must restart your session.
After uninstallation be sure to remove the config file:
 /etc/profile.d/qt6-style-globalqss.sh}

%description -n qt6-globalqss -l fr %{expand:
Ce moteur de style permet de personnaliser le thème des applications Qt
à l'aide de fichiers QSS, de la même manière que les applications
GTK sont personnalisées à l'aide de fichiers CSS.

Ce paquet fournit le programme pour Qt 6.

Après l'installation vous devez redémarrer votre session.
Après la désinstallation, veillez à supprimer le fichier de config :
 /etc/profile.d/qt6-style-globalqss.sh}
%endif


%prep
%setup -q -n globalqss-%{version}

%build
%if %{with qt5}
 mkdir build-qt5 && pushd build-qt5
 %cmake -S ../src-5/
 %cmake_build
 popd
%endif
%if %{with qt6}
 mkdir build-qt6 && pushd build-qt6
 %cmake -S ../src-6/
 %cmake_build
 popd
%endif

%install
%if %{with qt5}
 pushd build-qt5
 install -Dpm 644 ../data/profile.sh %{buildroot}%{_sysconfdir}/profile.d/qt5-style-globalqss.sh
 install -Dpm 755 build/libglobalqssplugin.so %{buildroot}/usr/lib64/qt5/plugins/styles/libglobalqssplugin.so
 popd
%endif
%if %{with qt6}
 pushd build-qt6
 install -Dpm 644 ../data/profile.sh %{buildroot}%{_sysconfdir}/profile.d/qt6-style-globalqss.sh
 install -Dpm 755 build/libglobalqssplugin.so %{buildroot}/usr/lib64/qt6/plugins/styles/libglobalqssplugin.so
 popd
%endif

%if %{with qt5}
%files -n qt5-globalqss
 %config(noreplace) %{_sysconfdir}/profile.d/qt5-style-globalqss.sh
 %license LICENSE
 %doc README.md
 %attr(644,root,root) /usr/lib64/qt5/plugins/styles/libglobalqssplugin.so
%endif

%if %{with qt6}
%files -n qt6-globalqss
 %config(noreplace) %{_sysconfdir}/profile.d/qt6-style-globalqss.sh
 %license LICENSE
 %doc README.md
 %attr(644,root,root) /usr/lib64/qt6/plugins/styles/libglobalqssplugin.so
%endif



%changelog
* Tue Mar 03 2026 Fabrice Creuzot <code@luigifab.fr> - 1.0.0-1.mga
- Initial Mageia package release (Closes: mbz#35065)

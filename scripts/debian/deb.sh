#!/bin/bash
# Debian: sudo apt install dpkg-dev devscripts build-essential dh-cmake cmake qtbase5-dev qt6-base-dev


cd "$(dirname "$0")"
export DH_QUIET=1
version="1.1.0"


mkdir -p builder
rm -rf builder/*

# copy to a tmp directory
if [ true ]; then
	cd builder
	wget https://github.com/luigifab/globalqss/archive/v$version/globalqss-$version.tar.gz
	tar xzf globalqss-$version.tar.gz
	cd ..
else
	temp=globalqss-$version
	mkdir /tmp/$temp
	cp -r ../../* /tmp/$temp/
	rm -rf /tmp/$temp/scripts/*/builder/

	mv /tmp/$temp builder/
	cp /usr/share/common-licenses/GPL*2 builder/$temp/LICENSE

	cd builder/
	tar czf $temp.tar.gz $temp
	cd ..
fi


# create packages for Debian and Ubuntu and MX Linux
for serie in experimental resolute questing noble jammy mx25 mx23; do

	printf "\n\n#################################################################### $serie\n\n"
	if [ $serie = "experimental" ]; then
		# copy for Ubuntu
		cp -a builder/globalqss-$version/ builder/globalqss-$version+src/
		cd builder/globalqss-$version/
	elif [ $serie = "unstable" ]; then
		rm -rf builder/globalqss-$version/
		cp -a builder/globalqss-$version+src/ builder/globalqss-$version/
		cd builder/globalqss-$version/
	else
		cp -a builder/globalqss-$version+src/ builder/qt-style-globalqss-$serie-$version/
		cd builder/qt-style-globalqss-$serie-$version/
	fi

	dh_make -s -y -f ../globalqss-$version.tar.gz -p qt-style-globalqss

	rm -rf debian/*/*ex debian/*ex debian/*EX debian/README* debian/*doc*
	cp scripts/debian/* debian/
	rm -f debian/deb.sh
	mkdir debian/upstream ; mv debian/metadata debian/upstream/metadata



	# debhelper: experimental:13 focal/mx21:12 bionic:9 xenial:9 trusty:9
	if [ $serie = "experimental" ] || [ $serie = "unstable" ]; then
		mv debian/control.debian debian/control
		#nano debian/control
	elif [ $serie = "mx21" ]; then
		sed -i 's/debhelper-compat (= 13)/debhelper-compat (= 12)/g' debian/control
	elif [ $serie = "focal" ]; then
		mv debian/control.ubuntu debian/control
		sed -i 's/debhelper-compat (= 13)/debhelper-compat (= 12)/g' debian/control
	elif [ $serie = "bionic" ]; then
		mv debian/control.ubuntu debian/control

		sed -i 's/execute_before_dh_install:/override_dh_update_autotools_config:/g' debian/rules
		sed -i 's/debhelper-compat (= 13)/debhelper-compat (= 9)/g' debian/control
	elif [ $serie = "xenial" ]; then
		mv debian/control.ubuntu debian/control

		sed -i 's/execute_before_dh_install:/override_dh_update_autotools_config:/g' debian/rules
		sed -i 's/debhelper-compat (= 13)/debhelper (>= 9)/g' debian/control
		sed -i ':a;N;$!ba;s/Rules-Requires-Root: no\n//g' debian/control
		echo 9 > debian/compat
	elif [ $serie = "trusty" ]; then
		mv debian/control.ubuntu debian/control
		sed -i 's/dh $@/dh $@ --with autotools_dev/g' debian/rules
		sed -i 's/execute_before_dh_install:/override_dh_autotools-dev_updateconfig:/g' debian/rules
		sed -i 's/debhelper-compat (= 13)/debhelper (>= 9), autotools-dev/g' debian/control
		sed -i ':a;N;$!ba;s/Rules-Requires-Root: no\n//g' debian/control
		echo 9 > debian/compat
	else
		mv debian/control.ubuntu debian/control
	fi

	if [ $serie = "mx25" ] || [ $serie = "mx23" ] || [ $serie = "mx21" ]; then
		mv debian/changelog.mx debian/changelog
		sed -i 's/-1) /-1~'$serie'+1) /' debian/changelog
		sed -i 's/ experimental; / mx; /' debian/changelog
		sed -i 's/ unstable; / mx; /' debian/changelog
	elif [ $serie = "experimental" ] || [ $serie = "unstable" ]; then
		sed -i 's/ experimental; / '$serie'; /g' debian/changelog
		mv debian/changelog.debian debian/changelog
	else
		mv debian/changelog.ubuntu debian/changelog
		sed -i 's/ experimental; / '$serie'; /g' debian/changelog
		sed -i 's/-1) /-1+'$serie') /' debian/changelog
	fi
	rm -f debian/*.mx debian/*.debian debian/*.ubuntu

	if [ $serie = "experimental" ]; then
		echo "===================== build package ($serie) =="
		dpkg-buildpackage -us -uc
		echo "=========================== lintian ($serie) =="
		lintian -EviIL +pedantic ../qt-style-globalqss_$version*.changes
		rm ../*amd64.changes
	fi

	echo "============== build source package ($serie) =="
	dpkg-buildpackage -us -uc -ui -d -S
	cd ..

	if [ $serie = "experimental" ] || [ $serie = "unstable" ]; then
		echo "=========================== debsign ($serie) =="
		debsign qt-style-globalqss*$version-*_source.changes
	else
		echo "=========================== debsign ($serie) =="
		debsign qt-style-globalqss*$version*$serie*source.changes
	fi
	cd ..
done

printf "\n\n"
rm builder/*dbgsym*deb
ls -dlth "$PWD"/builder/*.deb "$PWD"/builder/*.changes
printf "\n"
rm -rf builder/*/
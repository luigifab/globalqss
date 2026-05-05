#!/bin/bash
# Mageia: sudo urpmi --no-recommends rpmdevtools rpm-sign cmake qt5-qtbase-devel qt6-qtbase-devel aspell-fr enchant2-aspell


cd "$(dirname "$0")"
version="1.1.0"


mkdir -p builder ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
find builder/* ! -name "*$version*.rpm" ! -name "*$version*.gz" -exec rm -rf {} + 2>/dev/null
rm -f ~/rpmbuild/SOURCES/globalqss-$version.tar.gz

# copy to a tmp directory
if [ true ]; then
	rm globalqss.spec
	wget https://raw.githubusercontent.com/luigifab/globalqss/refs/tags/v$version/scripts/opensuse/globalqss.spec
	chmod 644 globalqss.spec
	spectool -g -R globalqss.spec
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

	cp builder/$temp.tar.gz ~/rpmbuild/SOURCES/globalqss-$version.tar.gz
	chmod 644 globalqss.spec
fi

# create package (rpm sign https://access.redhat.com/articles/3359321)
rpmbuild --quiet -ba globalqss.spec
echo "=================================== rpm-sign =="
rpm --addsign ~/rpmbuild/RPMS/*/*globalqss*.rpm
rpm --addsign ~/rpmbuild/SRPMS/*globalqss*.rpm
mv ~/rpmbuild/RPMS/*/*globalqss*.rpm builder/
mv ~/rpmbuild/SRPMS/*globalqss*.rpm builder/
rpm --checksig builder/*.rpm
echo "=================================== rpm-lint =="
rpmlint globalqss.spec builder/*.rpm | grep globalqss
echo "==============================================="
rm builder/*debug*rpm
ls -dlth "$PWD"/builder/*.rpm

# cleanup
rm -rf builder/*/
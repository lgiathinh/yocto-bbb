DESCRIPTION = "Custom-banner"

LICENSE = "CLOSED"

FILESEXTRAPATHS:prepend = "${THISDIR}/${PN}:"

S ="${WORKDIR}"

SRC_URI = "file://banner.sh"

do_install(){
    install -d ${D}${sysconfdir}/profile.d
    install -m 0075 ${WORKDIR}/banner.sh ${D}${sysconfdir}/profile.d/
}
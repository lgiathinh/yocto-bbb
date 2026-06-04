DESCRIPTION = "Realtek RTL8821AU USB WiFi driver"
SECTION = "kernel/modules"
LICENSE = "CLOSED"
SRC_URI = "git://github.com/morrownr/8821au-20210708.git;branch=main;protocol=https"
SRCREV = "${AUTOREV}"
PV = "1.0+git${SRCPV}"
S = "${WORKDIR}/git"

inherit module

KERNEL_MODULE_AUTOLOAD += "8821au"

do_compile() {
    unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
    oe_runmake ARCH=${ARCH} \
               CROSS_COMPILE=${TARGET_PREFIX} \
               KSRC=${STAGING_KERNEL_DIR} \
               modules
}

do_install() {
    install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless
    install -m 0644 ${S}/8821au.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless/
}

FILES:${PN} += "${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless/8821au.ko"
RPROVIDES:${PN} += "kernel-module-rtl8821au"
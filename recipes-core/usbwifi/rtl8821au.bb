DESCRIPTION = "Realtek RTL8821AU USB WiFi driver"
SECTION = "kernel/modules"
LICENSE = "GPL-2.0-only"


LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"
 
SRC_URI = "git://github.com/morrownr/8821au-20210708.git;branch=main;protocol=https"
SRCREV = "3a7cdb591b64d99d2670e455bde67c8ab338525b" 
 
PV = "1.0+git${SRCPV}"
S = "${WORKDIR}/git"
 
inherit module
 
KERNEL_MODULE_AUTOLOAD += "8821au"
EXTRA_OEMAKE += "KSRC=${STAGING_KERNEL_DIR}"

do_install() {
    install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless
    install -m 0644 8821au.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless/
}
 
# Manually declare the kernel module for packaging
FILES:${PN} += "${nonarch_base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/wireless/8821au.ko"
RPROVIDES:${PN} += "kernel-module-rtl8821au"

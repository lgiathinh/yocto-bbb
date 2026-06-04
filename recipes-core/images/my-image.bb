SUMMARY = "My custom Linux Image"
IMAGE_INSTALL = "packagegroup-core-boot ${CORE_IMAGE_EXTRA_INSTALL}"
IMAGE_LINGUAS = " "
LICENSE = "MIT"
inherit core-image

inherit extrausers
#Change root password to test
PASSWD = "\$5\$y9Aeg5ctwntRHo/g\$CAKtoTfQg7VPGfVAMGo5ZG/0GJLn3AD0JdoQ.i0dDFC"
EXTRA_USERS_PARAMS = "\
    usermod -p '${PASSWD}' root; \
    "

IMAGE_INSTALL:append = " \
    iproute2 \
    iputils \
    openssh \
"

#USB WiFi package
IMAGE_INSTALL:append = " \
    rtl8821au \
    linux-firmware \
    dhcpcd \
    iw \
    wpa-supplicant \
    wireless-regdb-static \
"
KERNEL_MODULE_AUTOLOAD:append = " \
    8821au  \
    "
#SSH VSCode server package
IMAGE_INSTALL:append = " \
    bash \
    tar \
    xz \
    procps \
    coreutils \
    curl \
    libgcc \
    libstdc++ \
    libatomic \
"

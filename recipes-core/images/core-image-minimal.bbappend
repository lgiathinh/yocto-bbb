# User "root" has password set to "test" in the image.
# printf "%q" $(mkpasswd -m sha256crypt test)
# \$5\$1ywTDE4jLgPDskTp\$yK5Ap.2xjc5gYeFQ4MGvR6C0VzA4VDSMIFkQ5.TJO84
inherit extrausers
PASSWD = "\$5\$y9Aeg5ctwntRHo/g\$CAKtoTfQg7VPGfVAMGo5ZG/0GJLn3AD0JdoQ.i0dDFC"
EXTRA_USERS_PARAMS = "\
    usermod -p '${PASSWD}' root; \
    "

IMAGE_INSTALL:append = "custom-banner"

# Resize rootfs to 200MB, max size 2GB
# IMAGE_OVERHEAD_FACTOR ?= "1.0"  # Factor to multiply the rootfs size by to account for overhead. Default of yocto is 1.3.
# IMAGE_ROOTFS_SIZE ?= "204800"   # Minimum size of the rootfs in KB
# IMAGE_ROOTFS_MAXSIZE = "2097152"# Maximum size of the rootfs in KB

# USB WIFI Package
IMAGE_INSTALL:append = " \
    kernel-module-rtl8821au \
    linux-firmware \
    iw \
    wpa-supplicant \
    wireless-regdb-static \
    openssh \
    "

# Open SSH
IMAGE_INSTALL:append = " \
    openssh-sshd \
    openssh-sftp-server \
    openssh-keygen \
    dhcpcd \
    iproute2 \
    iputils \
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

# I2C 
IMAGE_INSTALL:append = " \
    packagegroup-core-buildessential \ 
    i2c-tools \
    nano \
    "

KERNEL_MODULE_AUTOLOAD:append = " \
    rtl8821au \
    "

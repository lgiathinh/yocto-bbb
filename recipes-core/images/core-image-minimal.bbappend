# User "root" has password set to "test" in the image.
# printf "%q" $(mkpasswd -m sha256crypt test)
# \$5\$1ywTDE4jLgPDskTp\$yK5Ap.2xjc5gYeFQ4MGvR6C0VzA4VDSMIFkQ5.TJO84
inherit extrausers
PASSWD = "\$5\$y9Aeg5ctwntRHo/g\$CAKtoTfQg7VPGfVAMGo5ZG/0GJLn3AD0JdoQ.i0dDFC"
EXTRA_USERS_PARAMS = "\
    usermod -p '${PASSWD}' root; \
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

IMAGE_INSTALL:append = "custom-banner"
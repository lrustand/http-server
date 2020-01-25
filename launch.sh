#!/bin/sh
unshare --fork --kill-child --mount-proc --pid chroot container /bin/sh -c "export PATH=/bin; server; tail -f /var/log/httpd.log"

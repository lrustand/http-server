#!/bin/sh
PATH=/bin unshare --fork --kill-child --mount-proc --pid chroot container /bin/sh -c "server; tail -f /var/log/httpd.log"

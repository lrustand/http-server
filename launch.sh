#!/bin/sh
PATH=/bin unshare --fork --pid chroot container /bin/sh -c "server; tail -f /var/log/httpd.log"

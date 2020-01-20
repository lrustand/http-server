#!/bin/sh
unshare --fork --pid chroot container bin/sh -c "/bin/server; /bin/tail -f /var/log/httpd.log"

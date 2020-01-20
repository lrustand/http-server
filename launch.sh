#!/bin/sh
unshare --fork --pid chroot container bin/sh

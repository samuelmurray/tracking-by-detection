SHELL = /bin/sh

export PKG_CONFIG_PATH := ./config:$(PKG_CONFIG_PATH)

REQUIREMENTS := opencv dlib-1 boost caffe cuda

.PHONY: start test

all: test

#.SECONDEXPANSION:

test: start $(REQUIREMENTS)

start:
	@echo 'Make pkg-config look in ./config directory...'
	@echo 'PKG_CONFIG_PATH=./config:$$PKG_CONFIG_PATH'
	@echo 'Looking for packages using "pkg-config --exists <package>"'
	@echo

$(REQUIREMENTS): 
	@echo 'Looking for $@...'
	@pkg-config --exists $@ ; \
	if [ $$? -eq 0 ] ; \
	then echo "$@ FOUND!" ; \
	else echo "$@ NOT FOUND!" ; \
	echo "Make sure it is installed and that $@.pc is in PKG_CONFIG_PATH or in the ./config directory" ; \
	fi
	@echo


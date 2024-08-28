#!/bin/bash
trap "trapError" ERR

trapError() {
	echo
	echo " ^ Received error ^"
	exit 1
}


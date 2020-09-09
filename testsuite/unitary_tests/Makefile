POK_PATH := $(CURDIR)/../..

check::
	avocado run  --xunit report.xml --mux-yaml testconfig.yaml -- $(POK_PATH)/misc/execution_test.py

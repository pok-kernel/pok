POK_PATH := $(CURDIR)/../..

check::
	avocado run --max-parallel-tasks 1 --xunit report.xml --mux-yaml testconfig.yaml -- $(POK_PATH)/misc/execution_test_multiprocessing.py

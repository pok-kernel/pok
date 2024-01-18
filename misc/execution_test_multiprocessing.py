#                               POK header
#
# The following file is a part of the POK project. Any modification should
# be made according to the POK licence. You CANNOT use this file or a part
# of a file for your own project.
#
# For more information on the POK licence, please see our LICENCE FILE
#
# Please follow the coding guidelines described in doc/CODING_GUIDELINES
#
#                                      Copyright (c) 2007-2024 POK team

from avocado import Test, fail_on, TestError
from avocado.utils import build, process
import os
import itertools


class ExecutionTest(Test):

    def setUp(self):
        self.dir = self.params.get("dir")
        self.compiler = self.params.get("compiler")
        self.qemu = self.params.get("qemu")
        if os.path.exists("/dev/kvm"):
            self.qemu += " -accel kvm"
        self.processors = self.params.get("proc")
        self.affinity_max = self.params.get("affinity_max", default=0)
        self.partitions = self.params.get("partitions")

    @fail_on(process.CmdError)
    def test(self):
        l = list(itertools.product(
            range(self.affinity_max+1), repeat=self.affinity_max+1))
        for x in l:
            affinity = ' '.join(["-DAFFINITY_{}={}".format(i, x[i])
                                 for i in range(self.affinity_max+1)])
            build.make(self.dir, extra_args="clean")
            build.make(self.dir, env={"MAKEFLAGS": "-j1"},
                       extra_args="CC='{}'".format(self.compiler+" "+affinity))
            output = process.system_output(
                "{} -nographic -smp {} -kernel {}/pok.elf".format(self.qemu, self.processors, self.dir))
            try:
                output = output.split(b"POK kernel initialized\n", 1)[1]
            except IndexError:
                raise Exception("unable to find POK kernel startup message")

            if output.count(b"SUCCESS!!!") != self.partitions-1:
                self.error("Test failed with affinities: {}".format(x))

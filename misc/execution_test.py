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
#                                      Copyright (c) 2007-2020 POK team

from avocado import Test, fail_on
from avocado.utils import build, process
import os

class ExecutionTest(Test):

    def setUp(self):
        self.dir = self.params.get("dir")
        self.compiler = self.params.get("compiler")
        self.qemu = self.params.get("qemu")
        if os.path.exists("/dev/kvm"):
            self.qemu += " -accel kvm"
        self.expected = self.params.get("expected",
          default=os.path.join(self.dir, "expected.txt"))

    @fail_on(process.CmdError)
    def test(self):
        build.make(self.dir, extra_args="clean")
        build.make(self.dir, env={"CC": self.compiler, "MAKEFLAGS": "-j1"})
        output = process.system_output("{} -nographic -kernel {}/pok.elf".format(self.qemu, self.dir))
        try:
            output = output.split(b"POK kernel initialized\n", 1)[1]
        except IndexError:
            raise Exception("unable to find POK kernel startup message")
        try:
            expected = open(self.expected, "rb").read().rstrip(b"\n")
        except FileNotFoundError:
            expected = b""
        self.log.debug("expected = {}".format(expected))
        self.assertEqual(output, expected)

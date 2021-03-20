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
#                                      Copyright (c) 2007-2021 POK team

from avocado import Test, fail_on
from avocado.utils import build, process
import os


class ExecutionTest(Test):

    def setUp(self):
        self.dir = self.params.get("dir")
        self.compiler = self.params.get("compiler")
        self.qemu = self.params.get("qemu")
        self.processors = self.params.get("proc")
        if os.access("/dev/kvm", os.R_OK):
            self.qemu += " -accel kvm"
        self.expected = self.params.get("expected",
                                        default=os.path.join(self.dir, "expected.txt"))
        self.preserved_output = self.params.get("preserved-output",
                                        default=os.path.join(self.dir, "preserved-output.txt"))

    @fail_on(process.CmdError)
    def test(self):
        try:
            os.unlink(self.preserved_output)
        except OSError:
            pass
        build.make(self.dir, extra_args="clean")
        build.make(self.dir, env={"MAKEFLAGS": "-j1"},
                   extra_args="CC='{}'".format(self.compiler))
        output = process.system_output(
            "{} -nographic -smp {} -kernel {}/pok.elf".format(self.qemu, self.processors, self.dir))
        try:
            output = output.split(b"POK kernel initialized\n", 1)[1]
        except IndexError:
            raise Exception("unable to find POK kernel startup message")
        try:
            expected = open(self.expected, "rb").read()
        except FileNotFoundError:
            expected = b""
        self.log.debug("expected = {}".format(expected))
        try:
            self.assertEqual(output.rstrip(b"\n"), expected.rstrip(b"\n"))
        except Exception as e:
            open(self.preserved_output, "wb").write(output + b'\n')
            raise e

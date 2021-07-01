import utils
import os
import shutil
import subprocess

def MKDIR(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def RMDIR(directory):
    if os.path.exists(directory):
        shutil.rmtree(directory)

gcc_w_flags = ["-Wall", "-Wextra", "-Werror"]
gcc_freestanding_flags = ["-ffreestanding", "-nostdlib", "-std=gnu11", "-nostdinc"]

def GCC(input_file, output_file, includes, defines, strict):
    if utils.IsUpToDate(output_file, input_file):
        return True

    print(" CC %s -> %s" % (input_file, output_file))

    defines_flags = []
    for d in defines:
        defines_flags.append("-D%s" % d)

    includes_flags = []
    for i in includes:
        includes_flags.append("-I%s" % i)

    flags = ["gcc"]
    flags += ["-m32"]
    flags += defines_flags
    flags += includes_flags

    if strict:
        flags += gcc_w_flags

    flags += gcc_freestanding_flags
    flags += ["-c", "-o", output_file, input_file ]


    print(' '.join(flags))

    return subprocess.call(flags) == 0

def NASM(input_file, output_file):
    if utils.IsUpToDate(output_file, input_file):
        return True

    print(" AS %s -> %s" % (input_file, output_file))
    return subprocess.call(["nasm", "-felf32", input_file, "-o", output_file]) == 0

def AR(objects, output_file):
    print(" AR %s -> %s" % (objects, output_file))
    return subprocess.call(["as", "rcs"] + objects + [output_file])

def LD(objects, libs, output_file, script):
    print(" LD %s (%s) -> %s" % (objects, libs, output_file))
    return subprocess.call(["ld"] + ["-T", script] + ["-o", output_file] + objects + libs)

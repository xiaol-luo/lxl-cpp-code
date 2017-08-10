import os
import shlex
import subprocess
import shutil
import sys

is_rebuild = False
if len(sys.argv) > 1:
    is_rebuild = (sys.argv[1].lower() == "r")
build_dir = os.path.abspath("../cmake_cache_0001")
is_rebuild = is_rebuild or not os.path.isdir(build_dir)
ret = True
vstoll_cmd = "D:/Program Files/Microsoft Visual Studio 14.0/VC/vcvarsall.bat x86"
old_dir_abs_path = os.path.abspath(os.curdir).replace('\\', '/')
if is_rebuild:
    if  os.path.isdir(build_dir):
        shutil.rmtree(build_dir)
        pass
    if not os.path.isdir(build_dir):
        os.makedirs(build_dir)
    cmake_cmd = "cmake -G 'NMake Makefiles' {0}".format(old_dir_abs_path)
    os.chdir(build_dir)
    ret = ret and 0 == subprocess.call(shlex.split("{0} & {1}".format(vstoll_cmd, cmake_cmd)))
os.chdir(build_dir)
ret = ret and 0 == subprocess.call(shlex.split("{0} & {1}".format(vstoll_cmd, "nmake")))
ret and shutil.copy("main.exe", old_dir_abs_path)
ret and shutil.copy("main.pdb", old_dir_abs_path)
os.chdir(old_dir_abs_path)
#shutil.rmtree(build_dir)
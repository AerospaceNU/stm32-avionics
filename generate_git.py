import datetime
import os
import subprocess

Import("env")

VERSION_FILE = 'git_info.h'

describe = subprocess.run(['git', 'describe'], stdout=subprocess.PIPE).stdout.decode('utf-8').strip()
log = subprocess.run('git log --pretty=format:"%ci" -n 1'.split(" "), stdout=subprocess.PIPE).stdout.decode('utf-8').strip()
message = subprocess.run('git log --pretty=format:"%<(50,trunc)%s" -n 1'.split(" "), stdout=subprocess.PIPE).stdout.decode('utf-8').strip()

VERSION_CONTENTS = """
#ifndef UTILS_GIT_INFO_H_
#define UTILS_GIT_INFO_H_

#ifdef __cplusplus
extern "C" {{
#endif

const char* GIT_DESCRIBE = "{}";
const char* GIT_COMMIT_TIME = {};
const char* GIT_COMMIT_MESSAGE = {};

#ifdef __cplusplus
}}
#endif

#endif  // UTILS_GIT_INFO_H_
""".format(describe, log, message)

VERSION_FILE = "common" + os.sep + "utils" + os.sep + VERSION_FILE

print("Updating {} with git info...".format(VERSION_FILE))
with open(VERSION_FILE, 'w+') as FILE:
        FILE.write(VERSION_CONTENTS)

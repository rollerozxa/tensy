#!/bin/bash

cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"

iwyu-tool -p build/compile_commands.json src/ -- -Xiwyu --mapping_file=../util/iwyu.imp > /tmp/iwyu_output.txt

iwyu-fix-includes --nocomments --reorder --nosafe_headers --quoted_includes_first --noblank_lines < /tmp/iwyu_output.txt

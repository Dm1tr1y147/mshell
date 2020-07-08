# A simple UNIX shell written in C

I created it for personal usage, but decided to upload it into public repo in case if anybody would be interested in it.

Work is still in porgress, buf when you will see a "finished" topic assigned to it, feel free to contribute.

# Supported features
* Command input with `left` and `right` arrow, `home` and `end` keys navigation and `backspace`, `delete` support
* Running commands in separate process and termination them with `ctrl+c`
* `cd` and `exit` builtin commands

# Builtin commands
* `cd`: changes current working directory to the one specified by user. If no arguments provided, shows error.
* `exit`: exits shell
  
# TODO
* Pipes and files input/output
* Replace linux `echo` command with builtin one with support of environmental variables
* Environmental variables
* `Ctrl+Z` running programm with `fd`
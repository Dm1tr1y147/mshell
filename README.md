# A simple UNIX shell written in C

I created it for personal usage, but decided to upload it into public repo in case if anybody would be interested in it.

Work is still in porgress, buf when you will see a "finished" topic assigned to it, feel free to contribute.

# Supported features
* Command input with `left` and `right` arrow, `home` and `end` keys navigation and `backspace`, `delete` support
* Running commands in separate process and termination them with `ctrl+c`
* `cd`, `exit` and `exec` builtin commands
* Files and commands from `PATH` autocompletion on `Tab` keypress
* History of commands and navigation or search through it with `up`/`down` keys
* Username, ip address and current path in prompt before each command input
* Show previous command return status in prompt and invert it with `!`, separated with space, specified before it
* Running multiple commands separated by `;`, `&&` or `||`
* Expand `*` wildcards
* Commands I/O redirection with `|` pipes and `>`, `<`, `>>`, `<>` file input/output
* Save history into file and access recent command in another instance of shell
* `$ENVIRONMENT_VARIABLES` expansion
* Own `execvpe` implimentation as `mexecvpe` with commands getting from `$PATH`

# Builtin commands
* `cd`: changes current working directory to the one specified by user. If no arguments provided, shows error.
* `exit`: exits shell
* `exec`: executes entered command and exits
  
# TODO
* `Ctrl+Z` running programm with `fd`
* `$()` subcommands
* Setting environment variables with `export` and before commands

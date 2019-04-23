# minishell

The **minishell** program is the second project of the *Algorithm* branch from the [42](https://www.42.fr) curriculum. 
The goal is to recreate a basic shell environnment with simple builtin functionnalities.

## Useful links
- The [minishell PDF subject](https://github.com/AugustinLopez/minishell/blob/master/rcs/minishell.pdf)
- The [POSIX Documentation](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap01.html#tag_17)
- The [42ShellTester](https://github.com/we-sh/42ShellTester)
- A [list of POSIX man](https://www.unix.com/man-page-posix-repository.php)


## Credit
I always check other implementation on GitHub and try to implement my favorite segment. 
- [Still a solid implementation, even if the subject has evolved](https://github.com/R4meau/minishell)

## Download & Launch

```
git clone https://github.com/AugustinLopez/minishell
cd minishell
make
./minishell
```

## Builtins
Unless indicated otherwise, the implementation is POSIX compliant.
- **cd**: Not POSIX compliant: -P is the default option, *-L is not supported*. *CDPATH* is supported.
- **exit**
- **env**
- **setenv**
- **unsetenv**
- **echo**: -n and hexadecimal supported.

Simplified version of ~ and $ are supported: they can be used with any variable shown by the command ```env```.

### Bonus builtin:
- **mspath**: show/stop showing path to current directory in the prompt.
- **msname**: ```mspath + ARG``` change the text showed by the prompt.

## Supported bonuses
- \<C-c\> signal
- Execution right
- Command separation with ;
- On input: double quote ", single quote ', escape character \\
- Autocompletion with tab is *not supported*.

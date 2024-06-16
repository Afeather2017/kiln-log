# kiln-log: A fast async log libaray.

Here we have:

[x] Formated log, with date time, thread info, etc.

[x] Log to file with rolling and log to console.

[x] Multi-level log

## Install

Install requirements:

```shell
pacman -S cmake fmt
```

Install to your project.

- Use CPM.

```cmake
CPMAddPackage("gh:Afeather2017/kiln-log#release")
target_link_libraries(you-exec kilnlog)
```

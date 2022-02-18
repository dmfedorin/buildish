# buildish documentation

buildish is a tool which allows the user to write a build script for a project, which is executed when the `buildish` command is called. this script is contained in a file called `buildishprocs`.

### script

buildish scripts are divided into procedures, which are defined as follows:
```
@procname
{
}
```
every procedure has a name (identified by the `@` prefix) and a body, which must immediately follow the name. buildish procedures are similar to routines or functions in programming languages, the body contains instructions which are sequentially executed. as for the instructions, the command instruction:
```
$("gcc -o main main.c");
```
will have the system command line execute the string contained within the parentheses as a command, so the given command will use gcc to compile a C file. the call instruction:
```
>(procname);
```
will do the equivalent of calling a function in a programming language. in this case, it will call the procedure called `procname`. the log instruction:
```
!("this is a log message");
```
will write a message to the standard output. you now know everything needed to create a buildish script. here is an example of one:
```
@build
{
    $("gcc -o main *.c");
    !("build complete");
}

@run
{
    $("./main");
    !("finished running");
}

@main
{
    >(build);
    >(run);
}
```
indeed, buildish uses a `main` procedure as the entry point for a build script, and you will recieve an error if you forget this. in the case that you need to write a comment in your buildish script, you can use backticks to denote the start and end of one, as follows:
```
`this is a comment, as it is in backticks`
@main
{
    !("this, however, is not a comment");
    !("and neither is this");
}
```

### usage

when you have created your build script in a `buildishprocs` file, navigate to the directory containing the file and run `buildish` in the command line.
# Capacitated Multi Agent Pickup & Delivery

This software can generate instances of the CMAPD problem and solve them.
The task-assignment part is solved by Google's OR-Tools library, and the actual conflict-free paths
are calculated using a Conflict Based Search or a Prioritized Planning.  
The code is inspired by [https://github.com/fenoy/capacitated_mapd]().

## How to

### Build the main executable

Once you have downloaded the code, in the project directory run:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Obviously, you can use a different generator, such as Ninja, which is much faster!  
If you wish to install the executable in the [bin/]() directory you can alternatively do the following:

```
$ mkdir build
$ cd build
$ cmake ..
$ make install
```

Or, if you want, you can first build and then install.

**Note:** The project uses the OR-Tools library. The version for Ubuntu 22.04 LTS is included
in binary form under [third_party/or_tools](third_party/or_tools), if you want to use this project on a different
platform, replace the contents of that directory with the correct version of the library.
You can download the binaries and see the needed requirements on the
[installation page for C++](https://developers.google.com/optimization/install/cpp).

**Note:** The included OR-Tools is a built as a shared library. The installed binary will search for the library in
its [current location](third_party/or_tools/lib).

**Note:** You will need an internet connection to build the project, because some dependencies are downloaded
on the fly.

---

### Build the documentation

To build the documentation, first run cmake like explained before, and then in the build directory
run the following command:

```
$ make doc
```

This will create a [doc/html]() folder with the documentation.  
**Note:** You need Doxygen in order to generate the documentation.  
Take also a look at the UML diagram for this project:  
![UML SCHEMA](doc/cmapd_uml.drawio.svg)

---

### Run the tests

To run all the unit and integration tests, in the build directory you can:

```
$ make
$ make test
```

Please note that some tests are particularly slow, it takes an average of one minute to run them all on my machine :-)

---

### Compile with coverage data enabled

You can configure CMake with: `-DCOVERAGE=ON`. When you run the main executable or the tests,
report files describing coverage will be created which can be then analyzed with the preferred
tool of your choice.

---

### Enable sanitizers

Sanitizers can be enabled by configuring CMake with:

```
-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined’>
```

---

### Enable static analyzers

Static Analyzers can be enabled by configuring CMake with:

```
-DUSE_STATIC_ANALYZER=<clang-tidy | iwyu | cppcheck>
```

or a combination of those in quotation marks, separated by semicolons.  
By default, analyzers will automatically find configuration files such as .clang-format.
Additional arguments can be passed to the analyzers by setting the `CLANG_TIDY_ARGS`, `IWYU_ARGS`
or `CPPCHECK_ARGS` variables.  
Alternatively, in the build directory you can run:

```
$ scan-build make
```

**Note:** you must have these analyzers installed to be able to run them.

## Usage

With this program, you can:

- generate instances for a map
- evaluate those instances
- combine the two previous functions.

Type `cmapd -h` to see all available options.

### Generation of instances

To generate instances, you must provide a map in the format specified below. You also need to specify
the number of agents and tasks in each instance, and the number of instances to be generated. For example:

```
$ cmapd --generate --agents 5 --tasks 5 --instances 20 path/to/map.txt
```

The previous command will generate 20 instances in the `output` directory next to the executable. It is
possible to change the output directory with the option `--instances-output`.

### Evaluation of instances

To solve some already generated instances, you must say where those instances are, which map they are
referring to, the capacity of the agents and the type of conflict solver you want to use. For example:

```
$ cmapd --evaluate path/to/instances --capacity 2 --solver PP path/to/map.txt
```

### Map format

The map is saved as a txt file. The map must be rectangular, with `#` indicating a wall, ` ` (a whitespace)
indicating a free cell and `O` (big o) indicating a possible starting point for agents, or starting/ending
point for tasks. For example a possible map is:

```
                                   
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO ########## ########## OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO                       OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO ########## ########## OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO                       OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO ########## ########## OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO                       OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO ########## ########## OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO                       OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
 OO OO ########## ########## OO OO 
 OO OO OOOOOOOOOO OOOOOOOOOO OO OO 
                                   
```

### Generated instance format

The first row contains the number of agents, followed by the number of tasks.
Then there is a row for every agent with its starting point written as row and column indexes.
Finally, a row for every task with its starting and ending points written as row and column indexes.
Instances are automatically generated, but here's an example anyway:

```
1 1         # one agent and one task
0 0         # the agent is in position {0,0}
1 1 3 3     # the tasks starting point is at {1,1} and its ending point is at {3,3}
```

**Note:** no comments are allowed in an instance file!

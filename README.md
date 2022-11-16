# Capacitated Multi Agent Pickup & Delivery

This software can generate instances of the CMAPD problem and solve them.
The task-assignment part is solved by Google's OR-Tools library, and the actual conflict-free paths
are calculated using a Conflict Based Search or a Priority Based Search.  
The code is inspired by [https://github.com/fenoy/capacitated_mapd]().

## How to

---

### Build the main executable

Once you have downloaded the code, in the project directory run:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Obviously, you can use a different generator, such as Ninja.  
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

---

### Build the documentation

To build the documentation, first run cmake like explained before, and then in the build directory
run the following command:

```
$ make doc
```

This will create a [doc/html]() folder with the documentation.  
Take also a look at the UML diagram for this project:  
![UML SCHEMA](doc/cmapd_uml.drawio.svg)

---

### Run the tests

To run all the unit and integration tests, in the build directory you can:

```
$ make
$ make test
```

Please note that some tests are particularly slow.

---

### Compile with coverage data enabled

TODO

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
Answers to the questions:

1)
 A)Every file that we would like it to use the header that define this struct, would have to
 'import' this definition also, even if it doesn't need to know about this struct, and the file
 only cares about the rest of the header.

 B)If the header file defines the struct, other files that include that header can't change the
 members of the struct, so it force them to use the struct in only one way.

2)
The header file tells the compiler what types external functions and variables are defines.
Any functions and variables that get referenced in the source file will be linked in during the
linker phase. If we include the header file in the source file, and use the struct that defined in
the source file, the linker will try to find that struct either in the object files we
explicitly link, or in the libraries it is configured to search.
There is no real difference between including a file and copying the contents of that file into
the source file. If that included file contains function or data definitions (not just
declarations), then these do become part of the object file.
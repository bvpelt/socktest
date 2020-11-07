# C++ Sockets

- https://www.geeksforgeeks.org/socket-programming-cc/
- https://beej.us/guide/bgnet/html/ 

# VSCode
- https://code.visualstudio.com/docs/languages/cpp
- https://dev.to/tardisgallifrey/visual-studio-code-c-with-makefile-cheat-1hb4

# Makefile
- https://www.gnu.org/software/make/manual/make.html

# Valgrind
Valgrind is tooling to check for memory leaks and threads behaviour.
It is downloadable through 
```bash
 $ git clone git://sourceware.org/git/valgrind.git
 ```

 Documentation visable at: https://www.valgrind.org/docs/manual/manual.html 

 ## usage
 ```bash
 $ # memory checks
 $ valgrind --leak-check=yes ./mlist
 $
 $ # thread checks
 $ valgrind --tool=helgrind  ./cvsimple 
 ```
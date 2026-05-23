# DEVLOG

### 22 May, 2026 
Just started another rewrite of the rendering engine project because I left the last one for over 2 months and have no idea what I was doing in that last one. 
Probably the 5th rewrite or something. Last one had some AI generated code too since I was trying to add stuff as fast as possible without putting in the time to learn all those extra libraries so now that I'm free for the next 2 months (summer vacation), this is hopefully gonna gonna be the last rewrite of the engine without a single line of AI generated or copied code.
__New: Trying to learn CMake to replace makefile for this project. Why? Because I'm too free. (add a :salute_empji: right here)__

### 23 May, 2026 [8:27 A.M.]
Alright so I spent yesterday trying to make sense of how CMake works. Simple enough to understand once you get the hang of the basics. (I still can't write cmake files without looking anywhere else but that's just for bigger projects like this one.)
Now moving on to the main stuff. So far I've decided to split the window initialization process into a separate interface / virtual classes so that I don't have to do any random refactoring when all of my other classes will need to access the window's data. Last time I ended up writing a generic window implementation inside the main engine class and that meant I was including the Engine.hpp in almost every single header file which resulted in a lot of circular include statement errors. Not gonna happen this time around. (insert another :salute_emoji: here)

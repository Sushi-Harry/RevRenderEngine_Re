# DEVLOG

### 22 May, 2026 
Just started another rewrite of the rendering engine project because I left the last one for over 2 months and have no idea what I was doing in that last one. 
Probably the 5th rewrite or something. Last one had some AI generated code too since I was trying to add stuff as fast as possible without putting in the time to learn all those extra libraries so now that I'm free for the next 2 months (summer vacation), this is hopefully gonna gonna be the last rewrite of the engine without a single line of AI generated or copied code.
__New: Trying to learn CMake to replace makefile for this project. Why? Because I'm too free. (add a :salute_empji: right here)__

### 23 May, 2026 [8:27 A.M.]
Alright so I spent yesterday trying to make sense of how CMake works. Simple enough to understand once you get the hang of the basics. (I still can't write cmake files without looking anywhere else but that's just for bigger projects like this one.)
Now moving on to the main stuff. So far I've decided to split the window initialization process into a separate interface / virtual classes so that I don't have to do any random refactoring when all of my other classes will need to access the window's data. Last time I ended up writing a generic window implementation inside the main engine class and that meant I was including the Engine.hpp in almost every single header file which resulted in a lot of circular include statement errors. Not gonna happen this time around. (insert another :salute_emoji: here)

__[Same Day, 5:16 P.M.]__: Started going through some of the relevant chapters of "Game Engine Architecture" by Jason Gregory. Great book. That and a little bit of googling led me to the decision of fully committing to just splitting everything into two parts: The OpenGL part controlled by the "backend" and then the actual abstract implementation of the rest of the engine by a separate layer. So the _backend handles the opengl stuff_ while the _engine kinda just layers over the backend and the engine files will not have any include statements that include the actual glfw files._

__Here's the reasoning for why I'm doing it this way__: I'm hoping that when I eventually move on to a different API like Vulkan or DX12, I won't have to change the entirety of the code later.

__[Same Day, 7:41 P.M.]__: Kinda in the middle of working on the events system. So far, I've added the basic event abstract class and an event dispatcher class. I've also added two Events right now: WindowResizeEvent and WindowCloseEvent. The former has been giving me some trouble cause I'm kinda clueless as to _where_ to write the framebuffer_size_callback function since it requires glfw3.h and I can't include glfw3.h anywhere in the engine/ directory according to my previously established rule of splitting everything into two parts. But the latter has been added successfully.

__Need to Figure out next time__: Window Resize Callback function, other callbacks and the polling system has to be implemented

That'll be it for today!

### 24 May, 2026 [10:58 A.M.]
Great start to the day. Just started working on it again and fixed the problem I was facing regarding the WindowResizeEvent yesterday. So now two events have been set up correctly and verified to be working properly. Will begin working on the polling and input system today.
Starting to get more familiar with std::function and lambda functions. Feels good.
__Forgot to mention: A LOT OF THE CODE IN THIS PROJECT IS INSPIRED FROM THE CHERNOS' HAZEL ENGINE__
in fact, the events claas might look identical to the chernos' project cause I kinda had no idea how to write the events system.

__[Same Day, 11:15 P.M.]__: Done adding the input interface. Need to implement the actual glfw backend part of the input system tomorrow. Couldn't get much work done today cause I was distracted with Forza Horizon 5. Will get back to everything tomorrow.

### 25 May, 2026 [2:47 P.M.]
Alright so I just finished the input system. Tested it too and it works so far. Haven't tested the mouse inputs though but since the keyboard input and mouse input have extremely similar code, it should work fine. Will move on to something else now. Haven't decided what's next. Will do that after a cup of coffee.

__[Same Day, 10:17 P.M.]__: So I've added the layers part of the basic architecture and although it looks almost identical to how Hazel Engine is written, I kinda didn't know how else to write any of this since I've never undertaken such a big project. (I mean I have but never been completely successful) so I'm kinda just using the hazel engine architecture. But I'm sure it'll look different in the renderer part of the project so no worries. That's the actual meat and potatoes. (Coping but what can I say)
Will do the Imgui layer tomorrow along with some renderer stuff so that'll be it for today.

### 26 May, 2026 [10:16 A.M.]
Just finished adding the gui layer and also tested it so should be good to go though it just shows the imgui default demo window for now. Will do the rendering part later today.
It's evening right now and I'm reading through the book by Jason Gregory to get some more details down about the part I'm currently working on.
Spent some time thinking about how exactly I should structure the shader class and the buffer classes but still no idea. Gonna look at some open source projects similar to this one and then decide what to do next. But that's all work for tomorrow.

### 27 May, 2026 [11:32 P.M.]
Couldn't get much work done today at all. Had some other work to do and was tired all day long due to that so yeah. But I did get the shader class done. The backend implementation (backend: opengl_shader.hpp / .cpp) and the interface (engine: shader.hpp / .cpp) are done. I decided to port over most of my shader class from my previous projects which made it faster that I was expecting. But yeah everything has been written the same way as other systems (Split into abstract interfaces that don't use opengl at all and their backend implementations that use opengl)
Will do the rest of the work tomorrow.

### 28 May, 2026 [10:30 A.M.]
Alright so I woke up at like 6 today and started working on it at around 7 am. I've added the buffer layout, vertex buffer and element buffer and thir backend implementations and also added the vertex array class and its backend implementation. Took well over 3hrs and I'm very happy with the result since I was just doing it all in one go and didn't even try compiling anything until after I added everything. And guess what? IT COMPILES WITH NO ISSUES!!! Let's go!
Gonna get back to it in the afternoon.

### 29 May, 2026 [2:02 P.M.]
Alright so I spent a few hours on going through some of my own code and some of Hazel Engine's code and then I implemented the Rendering API class and the Texture class. Haven't tested if it compiles yet so that's the next step and then I'll check if those classes actually work and can draw a triangle on screenw without any issues. 

__[Same Day, 8:50 P.M.]__ Added some more stuff, not worth mentioning. Will work on the 3D Free Look camera tomorrow along with testing most of the untested stuff so there's quite a lot of stuff left for tomorrow.

### 30 May, 2026 [10:08 P.M.]
Man I couldn't get anything done today. I just kept thinking about the ways I could implement the whole rendering system. I can either just create an ECS system that I created in my last rewrite for everything which would be pretty easy since I've done it before and most of the previous code can be reused with a tad bit of refactoring here and there or I could research some more on industry standard ways to implement everything. Either way, it shouldn't take longer than 10 days to finish if we're talking just about the renderer. 

### 31 May, 2026 [7:25 P.M.]
So I've added quite a bit of stuff today and have also decided on some design decisions for the project going forward. Commiting to the ECS way of things and I've added some basic component structs, a model loader and a resource manager. 
Leaving the implementation of the resource manager for tomorrow.

### 1 June, 2026 [7:00 P.M.]
Did a lot of refactoring and rethinking about how the resource manager has to be implemented. And I think I'm on the right track. The resource manager now has function definitions for loading and managing shaders and textures. Need to add the same for Models so I'll work on that today in the remaining time before I go to sleep tonight.
Added default shader and textures in case the user requested shaders and textures don't exist in the cache.

__[Same Day, 11:47 P.M.]__ Getting late so I'm gonna end today's work here but will continue it as soon as I wake up tomorrow morning. Only got the material loding part done after the last session.

### 2 June, 2026 [1:19 P.M.]
The model struct has a uint32_t _id variable that was supposed to contain the abstract id generated for the model by the resource manager but now that I'm writing this, I feel like that extra variable is kinda useless since the resource manager is the only thing that cares about those ids. So I'm removing it.

__[Same Day, 5:56 P.M.]__ Added the render system too. Just need to patch out some minor issues and we'll be ready to render. Need to write the camera class too though.

### 3 June, 2026 [2:00 P.M.]
So I'll admit that I'm guilty of using AI for this rewrite. I couldn't understand the linker errors that came up after compiling the project for the first time in 3 days and I had to use some AI to debug that stuff. But I'll try my best not to use it next time.

Other than that: Major progress made today. Compiled the project, tested it and it runs great. Camera works too (can't move it yet but I'll have it done by tonight.), Default shaders and materials work nicely along with a default model.

The default model isn't really working too well right now cause I kinda ignored the fact that exporting a model with multiple separate objects also means that each one of those objects in the model is centered at 0,0,0 coordinates locally which is different from the global 0,0,0 coordinates. So I'll add a local transformation matrix to the mesh struct and look up how to account for that while loading a model using assimp

### 4 June, 2026 [3:04 P.M.]
I have to be the dumbest guy to ever walk this planet. I had some "placeholder" code in the event dispatching part of the Application class. (specifically the Application::onEvent function) I forgot to set the viewport for the window on initialization and spent an hour straight, figuring out what the hell is wrong with the code.

__[Same Day, 5:37 P.M.]__ ALRIGHT! So I decided to add EnTT to the project for implementing ECS. AND IT WORKS! Major overhaul. Spent longer than I'd like to admit on adding it to the project. Tore up the main rendering function and refactored it into an ecs compatible alternative and gave rise to a bunch of errors. But all of that is fixed now. Models load perfectly, shaders load perfectly and for now, the test code works like intended. On that note, I'll end this session.

### 5 and 6 June, 2026 [12:16 A.M. of 6 June]
So I finally hooked up the textures being extracted from assimp to the rendering pipeline. ANd it works for now. (Tested it with a custom ideal case shader at revrender/assets/models/cyborg/directory) And it works. Also offloaded some more hardcoded stuff to the appropriate locations / interfaces. Good work done today overall.

### 6 June, 2026 [10.40 A.M.]
So I've now also added the mouse callback function's implementation so the camera can be moved around with both the mouse and the keyboard.
Man I don't know what to do right now. I wrote a basic shader with lighting and stuff but nothing else done today. Don't know what to do. Don't even feel like doing anything at all to be very honest.

### 7 June, 2026 [6:44 P.M.]
So yesterday and most of today has been an absolute waste in terms of productivity. But back in business now. So I thought I should create separate classes for different types of object and just found out that that's not standard practice and it's kinda inefficient in the long run for me since I would run into virtual inheritance and the like when implementing it and I REALLYYY don't wanna deal with that if it's not necessary. Not because I'm unaware about how those concepts work, but because I'd rather not waste time debugging stuff later if I can avoid it beforehand using a better approach. So I'm just gonna create more functions int he entity class to create entities of different types by attaching different components as per the requirement in the new functions.

Man keeping everything structured is harder than I imagined. I'm trying to improve the material class to make it so that the shader uniforms can be set using just an Apply() function defined in the material class but I need to access the resource manager for getting the texture data and if I include resource_manager.hpp in the same file as the material class, I'll run into circular dependency problems in the render_system.hpp file.

Created a separate file for material class but still can't think of a way to get around the circular dependency thing. Cause I need a material type of object in the ResourceManager class to serve as the default material but I don't think that's allowed if I remove the include statement for material.hpp and use just the forward declaration. I need to have a defined class to make an object of its type. (At least that's what I think, judging by the red squiggly lines)

DONE!!! Fixed the circular dependency thing and managed to get everything working somehow. Though it still seems to be held together by hopes and prayers but it runs exactly like it should. Will explain more about it during tomorrow's devlog entry. Too sleepy to carry on right now. 

### 8 June, 2026 [9:10 A.M.]
Alright so had to transfer the material struct from the components.hpp to its own header called material.hpp and to fix the circular dependency problem, I used forward declaration of resource manager in material class and included the actual header in the source file for the material class instead of the header file and defined the apply function in the source file instead of keeping it inside the header file. 
That was the simple fix I used last night. 
BUT! Now the problem is that my apply function loops through all the textures in the _diffuse and _specular arrays and applies all of them to the saampler 2d uniform in the current iterration's shader. Which means that if the material class in the header has only one specular and diffuse texture but the cpp material object contains more than one of each then this loop won't work as intended. So I'll start fixing that for now.

I've managed to fix the previously stated problem. How? Well I changed the material class to store just singular texture ids instead of an array of them. So basically, each sub mesh in a model will have its own material now instead of just one material having ids for all the submeshes' textures.
For now, the project compiles fine and runs as expected but the default_lit_shader.frag/.vert can't be used right now cause they're having some issues due to lack of pointlights in the scene. (Those default lit shaders are the ones with lighting implementations so yeah...)

### 9 June, 2026 [4:38 P.M.]
Yeah so the default lighting shader works now. Point lights are done. Will move on to other types of lights now.
Alright so I've added directional lilghts too. But I haven't managed to add a skybox yet. Will add it in the next session.
Still kinda confused by how I should implement the skybox but I'll figure it out soon enough.

### 10 June, 2026 [10:55 A.M.]
So I've added a separate cubemap class in order to prepare for adding the skybox later. Adding the skybox should be quite easy so I'll get it done as fast as possible and move on to shadow mapping and I'll try to get shadow mapping done by today too. But for now I'm going on break.

__[Same Day, 7:07 P.M.]__ Yeah so shadow mapping isn't gonna happen today. No chance. I just finished adding the skybox. I am not kidding when I say that I am the dumbest person to ever step foot on this earth. I spent over an hour figuring out why the skybox wouldn't get drawn at all. Turns out there was a typo in the vertex shader. (Misspelled the out variable name as TexCoords instead of u_TexCoords that the fragment shader was expecting.) and then I spent another hour figuring out why that shader fix wasn't working either. Turns out there was ANOTHER type in the shader loading statement for the skybox shader. I was loading default_shader.vert/.frag instead of skybox.vert/.frag

### 11 June, 2026 [8:12 P.M.]
Shadow Mapping is still a work in progress but I did end up adding a framebuffer class and it works (kinda). But Visually, the shadow maps are broken since I haven't implemented them in the shaders used for rendering the other models and stuff so yeah.
This took an unnecessarily large amount of time and effort. I am so braindead right now that I literally don't even remember where I'm binding texture units. And that is my cue to call it a day.

### 12 June, 2026 [4:22 P.M.]
I cannot express in words how happy I am right now. I have finally added shadow mapping for directional lights. Took a lot of debugging cause I wrote the framebuffer class in a half asleep state. I basically had a lot of bugs lying around in the framebuffer code. I was sending the wrong framebuffer specs to the framebuffer object (width and height data to be specific). It the framebuffer needed to have a 1:1 aspect ratio and I was literally sending it 1280x720 resolution insteado of 1024x1024 or 2048x2048. And then another problem I was facing was related to the viewport size getting messed up whenever I was using the framebuffer. Turns out I was never resetting the viewport after binding to the default framebuffer. But I've managed to fix all those stupid mistakes and stuff is working great.

This was so annoying to fix that I managed to forget that I had a cup of coffee waiting for me to drink it. It was a hot cup of coffee. Now that I'm drinking it, it feels like it's a standard cup of cold coffee.

And another problem still remains. I need to fix how the material related uniforms are set cause when I'm loading models with no textures defined in the obj file, the loaded model ends up using the texture that a different model is supposed to be using. Definitely gonna fix it by the end of today.

__[12:35 A.M.]__ So I've managed to fix that texture slot issue I talkeed about earlier and I've also set up debug and release presets. Runs great right now. Just need to change the shadow draw distance next time. I'll do point light shadow maps and uniform buffer objects tomorrow and work on some optimization stuff too.

### 13 June, 2026 [9:08 P.M.]
I underestimated uniform buffer objects. The implementation of the entire thing was easy enough. Too easy to be very honest. But man. The integration gave me a headache. Kill me now. 
Uniform Buffer objects are done. That's all I can say for today.

### 14 June, 2026 [2:58 P.M.]
So I've added an enum in the core/utilities.hpp to define texture binding slots for different kinds of textures and cubemaps to make it more convenient to change the binding slots for textures later if I want to.
The next thing I'm gonna work on is cleaning up a bit. Will write more about it later.
After a lot of debugging, I have managed to get the shadows working for spot lights. Might do point light shadow stoday too but not sure if I'm feeling too happy about the effort I just had to put into spotlight shadows. Kinda braindead right now.

__[Explanation for the little block of code in `RenderSystem::CalculateLightSpaceMatrix` function]:__ So when calculating using the glm::LookAt function, it does these 3 things in sequence:

1) Calculating the _forward vector_, (let's call it `f`): `f = normalize(TargetDirection)`
2) Calculating the _right vector_, (let's call it `r`): `r = normalize(cross(f, up_vector))`
3) Calculating the _local up vector_, (let's call it `u`): `u = cross(r, up_vector)`

If the point light is looking straight down (-Y direction), `f` becomes `<0, -1, 0>` and before adding that llittle block of code, the up veector being passed to the lookAt function was hardcoded to be `<0, 1, 0>`. Why does that matter though? Here's why: the `f` vector in the mentioned case causes the cross product to be a null vector. And when glm tries to normalize is using the formula `<0, 0, 0>/sqrt(0^2 + 0^2 + 0^2)`. And as you'd imagine, it will return `NaN` for every component. So that's gonna cause all the related calculations after that one to just fail kind of.

### 15 June 2026, [5:42 P.M.]
So I started working on gui today. Couldn't get much done today at all cause something came up and ruined the rest of the day for me in the morning. But yeah, the gui will probably become passable at thevery least by tonight before I sleep.

### 16 June 2026, [11:12 A.M.]
So the gui is coming along nicely. Swithed to the docking branch of ImGui to get some sick docking features. The viewport now renders in a separate imgui frame that can be moved around using the mouse and docked anywhere in the window. Other gui elements can be docked too. This was a very simple and easy to do thing so that's why I did this before framebuffer based effects and stuff. Wanted to lift my mood up a bit so I had to do something that was quick and easy and gave me a hit of dopamine by looking at the visual results. I will work on adding some basic effects using the framebuffer class today. And maybe after that I'll move on to PBR materials and stuff. So we're officially nearing the completion of this project finally. 

### 17 and 18 June 2026, [5:21 P.M.]
So I didn't write anything yesterday cause I was just a bit too busy and was having a bad day so didn't even want to work on anything at all. But I still did get some work done yesterday. I implemented a basic interface for the post processing pipeline yesterday.

As for today, I've managed to get the entire post processing pipeline working flawlessly. The filmgrain effect is broken but that's a glsl shader issue so it can be fixed later when I feel like it. But for now, major achievement is that the PostProcessing system is done and tested plus the gui implementation for all the effects. I've added a crt scanline, chromatic aberration, pixelation and filmgrain(WIP) effects. And I've also tested them and they work very nicely. Next I'll move on to some more complex stuff like SSR.

### 19 June 20226, [1:58 P.M.]
So I'm not gonna be able to do much today cause I'll be quite busy with stuff today. But I did add FXAA effect and it works great. All thanks to the [guide by Simon Rodriguez](https://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html) 

### 20 June 2026, [4:50 P.M.]
So I added imguizmo to the project and hooked it up to the rest of the engine through the sandbox layer and it works very nicely witht the newly added gizmos. Finally starting to look more like a real game engine.

### 21 June 2026, [10:15 P.M.]
So I spent most of today getting some college work done. (Even though it's summer vacations. Absolutely rotten luck) and also spent a bit of time figuring out how deferred shading works. That's kinda it for today. Will implement deferred shading tomorrow and then move on to SSR.

### 22 June 2026, [5:23 P.M.]
After a tonne of debugging and what feels like a million path typos I have finally done it. Deferred shading now works. Now I just need to add SSR and some other screen space effects and this project is as good as finished. Just a tiny bit of work left before I can move on to a new project.

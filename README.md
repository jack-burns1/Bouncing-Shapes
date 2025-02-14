***Bouncing Shapes**

This project uses SFML to generate a window, and animate moving objets across the screen.
Additionally, it uses imgui to create a user interface for the user to control the objects.

Objects that are animated are simple 2D circles and rectangles.
The window size, font specification, and shape configurations are read in from a single file.

The format of the file is as follows:
Window [width] [height] (integers)
Font [ttf filename] [font size (int)] [R] [G] [B (all int)]
[Shape type (Rectangle or Circle)] [Dsiplay name] [x position (float)] [y position (float)] [x velocity (float)] [y velocity (float)] [R] [G] [B] [size*]

for size, it is width and height for a rectangle, and radius for a circle
--------------------------------------------------------------------------------------------------
Shapes will be drawn in their initial positions moving at their initial speeds. Additionally, 
the shape name will be drawn in the specified font in the exact center of the shape.
When a shape hits the edge of the window, it will 'bounce' off and move with the opposite velocity.

The gui will allow the user to select a specific shape on the screen by its name.
The user will then be able to:
- Toggle whether or not the selected shape will be drawn
- change the scale of the shape
- Change the color of the shape
- Change the name of the shape


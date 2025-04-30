meme to start with for better reading

![image](https://github.com/user-attachments/assets/e3135e02-318e-45ac-85a2-e6d077aefcbb)


**C Sudoku**
---
A classic Sudoku Game in C Terminal. Game supports saving, loading game and has 3 lvls (4x4, 9x9, 16x16) and 3 difficulties (easy, medium, hard)

Instruction to the game provided with screenshots of example gameplay:

What is sudoku? - Sudoku originally called Number Place is a logic-based, combinatorial number-placement puzzle. In classic Sudoku, the objective is to fill a 9 × 9 grid with digits so that each column, each row, and each of the nine 3 × 3 subgrids that compose the grid (also called "boxes", "blocks", or "regions") contains all of the digits from 1 to 9. This version also supports 4x4 and 16x16 grid.

**How the Program looks?:**

Main menu

![image](https://github.com/user-attachments/assets/d00b5acc-2750-4b44-be66-56ebc00e0465) 

New Game

![image](https://github.com/user-attachments/assets/1ed214ec-92ab-459b-8f03-3b7446013ed8) 

Inside of the game

![image](https://github.com/user-attachments/assets/19a33ea5-6035-43cb-b7a8-01267405466d) 

![image](https://github.com/user-attachments/assets/16678074-7a3c-4745-aca7-89beb9e78320) -- Filling the grid --> ![image](https://github.com/user-attachments/assets/d7f07d0a-5646-4147-97ea-4c825a4d4f7b)

There Can be 2 more messages while filling the grid

![image](https://github.com/user-attachments/assets/8a0bb1f6-1b9c-4b6e-8801-5e9052cad32a) or ![image](https://github.com/user-attachments/assets/3ca5c7d8-69ce-4d3c-8c19-04cd8934bbaf)

Keep in mind that attempting to place an incorrect number in the grid counts as a move, while trying to fill an already occupied cell should not be penalized-such actions may simply result from a user mistake or a typographical error.

win message

![image](https://github.com/user-attachments/assets/e8218172-28fe-4249-bdcb-566fc7d19b67) 

When you decide to save the game such communicate will appear

![image](https://github.com/user-attachments/assets/d1d4281c-7b2b-4e99-a2c9-0a880219be48)

While on main menu and pressing continue a list of saves will appear. You need to write number next to it.

![image](https://github.com/user-attachments/assets/86c8fb85-f447-49f0-b91c-a6ae37330481)

while selecting a specific save, a game will resume with number of moves used previously.

![image](https://github.com/user-attachments/assets/3004322a-4ba8-4156-b64c-b19e8955df93)

To Do in the future:
* Add an option to delete saves.
* Create a high score table.
* Fix minor 4x4 grid rendering bug (sometimes generates an unusable grid; regenerating usually fixes it).\
* User is able to erese his insertion
* Game statistic, like time (how long did it take to solve the sudoku)
---
Gif of example gameplay and basic functionality:

![howItWorks](https://github.com/user-attachments/assets/b01979e4-7a6c-43f5-ab9d-cd4f4eb37065)

# Project "Bowling"  [![Build Status](https://travis-ci.com/EugeneTolstikhin/eugenetolstikhin.github.io.svg?branch=main)](https://travis-ci.com/github/EugeneTolstikhin/eugenetolstikhin.github.io)
----
## Preview

In this section the architectures' principles will be described, which are used for this project

## S.O.L.I.D. Principles

S.O.L.I.D. - an acronym for 5 first Object Oriented Design (OOD) principles:
S - Single responsibility principle
O - Open-close principle
L - Liskov substitution principle
I - Interface sagregation principle
D - Dependency inversion principle

Now, let's try to describe each principle with the examples scpecifically for this project.

## S (Single responsibility principle)
Each class should have one and only one reason to change. That means that class should have only one job.

***What does this mean for this project:***
- Eacn bowling hall can contain 1 or more bowling lanes -> The only one job - run the array of lanes
- Each bowling lane can contain many players, who can throw the ball one-by-one. This lane contains only 1 score table. -> The only one job - run the score of the players
- Each player, when he/she plays the game, has his/her own row in the score table -> The only one job - run his/her own game (row in the table)
- Each game (row in the table) has the frames (in classical bowling, there are 10 frames) -> The only one job - run the array of the frames
- Each frame has the score points, which are calculated according to the rules -> The only one job - proper calculating the points according to the bowling rules
- The class that listen network and accept the score of each ball throw -> The only one job - listen the network and accept the input data
- The class for providing logs -> The ony one job - provide proper log format for each class
- ***TBD:*** The class for Backup/Restore the score -> The only one job - provide the safe mechanism of keeping the data in case of unexpected crash

## O (Open-close principle)
Object or entities should be open for extension, but closed for modification.

***What does this mean for this project:***
Each class that accept the input score data from the outside should override the abstract class (e.g. all the classes inside ***interface*** folder), which should contain all the common interface functions described (pure virtual functions) and each class should implement all the common interface functions by itself 

## L (Liskov substitution principle)
Object in the program should be replaceable with instances of their subtypes without altering the correctness of the program

***What does this mean for this project:***
The Backup/Restore class for each database/file/etc. should be fully replaceable inside each class described above (should be overriden from the interface class, e.g. IBackupRestore, without any additional interface inside)

Examples ***TBD*** (all have the same interface as IBackupRestore, w/o any extensions):
- BackupRestoreFile (IBackupRestoreFile)
	1) BackupRestoreXML
	2) BackupRestoreJSON
	3) etc.
- BackupRestoreRDBMS (IBackupRestoreRDBMS)
	1) BackupRestoreSQLiteLocal
	2) BackupRestoreSQLiteServer
	3) etc.
- BackupRestoreNoSQL (IBackupRestore)
	1) BackupRestoreMongoDBLocal
	2) BackupRestoreMongoDBServer
	3) etc.

Another example of substitution is Logger which can be easily substituted between CLI and File (according to the current implementation)

## I (Interface sagregation principle)
The client should never be forced to implement the interface that it doesn't use or the client should never be forced to depend on the methods it doesn't use

***What does this mean for this project:***
Each interface class should contain full list of the methods that need to be overriden and this list of methods should be as small as it is needed for the fully usage inside of each instance of the class. No extra functions that will not be needed outside the interface should be included in that interface and overriden by all the children classes

## D (Dependency inversion principle)
High level modules should not depend on low level modules, but on their abstractions. Abstractions should not depend on details. Details should depend on abstractions

***What does this mean for this project:***
Logging of ***MODEL***, ***VIEW*** and ***CONTROLLER*** (See **MVC** chapter below) should not depend on the specific logging way but only on the Logger interface. Each class should not depend on the low level of Logger implementation, but only on the interfaces

# DRY - Don't repeat yourself
It's better to avoid using the duplication of the code. The solution proposes to encapsulate the dupicated code into, e.g. function, and use it whenever it is needed.

# KISS - Keep it simple, stupid
All the code should be intuitively understandable. All functions should be as simple as possible, not to add extra complexity without any reasons.

# Design patterns

## MVC (Model-View-Controller)
This pattern is used to separate application's concerns and contains 3 parts:

- **Model** - represents an object independently from the user interface (UI). It directly manages the data, logic and rules of the application
- **View** - outputs representation of the information. Multiple views of the same info is possible. That is what the user sees in reality
- **Controller** - accepts the inputs and convert them to the commands for the ***Model*** and ***View***. It acts as an interface between ***Model*** and ***View*** components to process all the business logic and incoming requests, manipulates the data by using the ***Model*** component and interacts with the ***View***s components to render the final output

***What does this mean for this project:***
Each class (*Frame*, *Game*, *Lane*, *Player*) represent the ***Model*** of the Bowling game. The class that inherited from **IView** for the user interaction (e.g. via Console or UI) represents the ***View***. Finally, the classes, responsible for network data exchange, like GameInitaliser and PointsListener - accepting the amount of points after each throw and the class that manipulates the game after the admin's request represent the ***Controller***

## Factory Method Pattern (Virtual Constructor)
Deal with the problem of creating the objects without having to specify the exact class of the object that will be created

***What does this mean for this project:***
Class ***Game*** waits for the points and receives them from the Listener class without knowing what kind of Listener it uses. The specific type of Listener is created based on the type of the data (as for now it is a member variable of the ***Game*** class, but it can be also an input parameter from the build script) the ListenerFactory is received

## Observer Pattern
The object (called the Subject) maintains a list of its dependents (called Observers), and notifies them automatically of their state changes, usually by calling one of their methods. Used inside MVC pattern to inform, e.g. Views, about the changes inside model to refresh the views

***What does this mean for this project:***
Every class (Lane, Player, Game, Frame) has the visual representation inside the Views. Each class should notify the Views components about their changes independently. e.g. during the init procedure, the Views should be able to visualize the clear Score table. During the play, each change should be visualized immediately

# Bowling rules
## Components
1) A bowling court has many **Lanes**
2) Each **Lane** can contains many **Players**
3) Each **Player** plays his own **Game**
4) Each game contains ***10*** **Frames**
5) Each **Player** plays one by another

***Score calculation:***

1) Every **Frame** has ***2*** **Trial** except the 10th **Frame**, which can have ***3*** **Trials** if the first 2 contain 10 points in total
2) The ***1st*** **Trial** can have 0-10 points
3) The ***2nd*** **Trial** can have 0-(10 - 1st_trial_points)
4) If the ***1st*** **Trial** contains 10 point then the **Frame** is marked as **Strike**
5) If the ***1st*** **Trial** contains <10 point and the ***2nd*** contains the rest points, the **Frame** is marked as **Spare**
6) If the **Trial** contains 0 points then this **Trial** is marked as **NULL** (sign: *-*)
7) If the sum of points after the ***1st*** and the ***2nd*** **Trials** equal <10 points, no additional marks are added to the **Frame**

---

# TODO:

- [X] Implement each Model class according to the SOLID principles
- [X] Implement the proper design patterns
- [ ] Cover each class with proper unit test
- [ ] Refactor CMakeLists.txt for unit tests
- [X] Fill in readme
- [X] Create the process (independent app) relates for throwing the ball and calculating the points which will send the amount of points to the score app
- [X] Create the process (independent app) relates to lane management and other admin parts

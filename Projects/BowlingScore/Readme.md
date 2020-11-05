# Project "Bowling"
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

- ***TDB:*** The class that listen network and accept the score of each ball throw -> The only one job - listen the network and accept the input data
- ***TBD:*** The class for Backup/Restore the score (SOLID principles need to be taken into account) -> The only one job - provide the safe mechanism of keeping the data in case of unexpected crash
- ***TBD:*** The class for providing logs (SOLID principles need to be taken into account) -> The ony one job - provide proper log format for each class

## O (Open-close principle)
Object or entities should be open for extension, but closed for modification.

***What does this mean for this project:***
Each class that accept the input score data from the outside should override the abstract class (e.g. INetworkData), which should contain all the common interface functions described (pure virtual functions) and each class should all the common interface functions implement by itself 

## L (Liskov substitution principle)
Object in the program should be replaceable with instances of their subtypes without altering the correctness of the program

***What does this mean for this project:***
The Backup/Restore class for each database/file/etc. should be fully replaceable inside each class described above (should be overriden from the interface class, e.g. IBackupRestore, without any additional interface inside)

Examples (all have the sxame interface as IBackupRestore, w/o any extensions):
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

# Design patterns

## TODO:
Define which patterns should be used for this project

---

# Bowling rules

1) A bowling court has many **Lanes**
2) Each **Lane** can contains many **Players**
3) Each **Player** plays his own **Game**
4) Each game contains ***10*** **Frames**
5) Each **Player** plays one by another

***Score calculation:***

1) Every **Frame** has ***2*** **Trial**, except the 10th **Frame**, which can has ***3*** **Trials** if the first 2 contains 10 points each
2) The ***1st*** **Trial** can have 0-10 points
3) The ***2nd*** **Trial** can have 0-(10 - 1st_trial_points)
4) If the ***1st*** **Trial** contains 10 point, then the **Frame** marked as **Strike**
5) If the ***1st*** **Trial** contains <10 point, and the ***2nd*** contains the rest points, the **Frame** marked as **Spare**
6) If the sum of points after the ***1st*** and the ***2nd*** **Trials** contains <10 points, no additional marks are added to the **Frame**

---

# TODO:

- [ ] Implement each class according to the SOLID principles
- [ ] Implement the proper design patterns
- [ ] Cover each class with proper unit test
- [ ] Fill in readme
- [ ] Refactor CMakeLists.txt for unit tests

- [ ] Create the process (independent app) relates for throwing the ball and calculating the points which will send the amount of points to the score app
- [ ] Create the process (independent app) relates to lane management and other admin parts

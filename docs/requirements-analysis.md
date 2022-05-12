---
header-includes: |
    \setlength{\parskip}{1em plus 2pt minus 1pt}
    \usepackage{titlesec}
    \newcommand{\sectionbreak}{\clearpage}
    \renewcommand{\familydefault}{\sfdefault}
geometry: margin=1.2in
papersize: a4
fontfamily: helvet
linestretch: 1.15
numbersections: true
toc: true

title: Requirements Analysis
date: April 26th, 2022
author:
- Dennis Ponstein
- Julian van Rijckevorsel
- Stephan Stanisic
- Zinedine Sghari
---

# Project description
The main goal of this project will be to get children moving while they learn. The target audience of this project is children in the last two grades from elementary school to the first two grades of middle school. This means these children would be from ages ten to about fourteen. The way would get these children moving is through Virtual Reality using the Oculus Quest 2 headset. 

The players will be playing several educational minigames. The participants will all be playing single-player minigames in their own pre-defined space. The project group will discuss the content of the minigames with the product owner and the schools where the project will be piloted.

# Priorities
During the initial meeting with the project owner the priorities got clear. Many aspects of the project are still unclear since only the heading is known. This means that this document will change over time as more requirements get clear. 

The main priority of the project is to have maintainable code, since there will not be enough time to completely finish the project. The focus will be to create a foundation where a new project can build upon. The requirements for this base will be defined in the 

# Requirements


## Must have
There are different requirements that are "must have". These requirements have to be implemented in order to complete the project. These are the minimum for the project team to be done.
 
### R1: The game is in VR
The end product must be in VR, this will create an immersive envioroment that will encourage the end users to be more physically active while playing the game. In addition it will give the game more creative freedom for the excersices.

#### Acceptation
* The game is developed so that it can utilise Virtual Reality


### R2: The game must be made for the Oculus Quest 2
The game must be made for the Oculus Quest 2 Virtual Reality headset. These are what Bewegend Leren uses and what they want to keep using. Therefore the game must be made for this.

#### Acceptation
* The game works on the Oculus Quest 2


### R3: The game must work out of the box
The game works without needing to install anything or changing any settings. This way it will be very usable and the step to be moving will be even smaller

#### Acceptation
* The user does not need to change any settings
* The user does not need to install anything
* The user puts on the headset, selects the game and it works


### R4: The game must have a grade selection
The end user must be able to select their grade. This way the assignments can be more specific for their level of knowledge in the subject.

#### Acceptation
* The user can make a selection between grades 7, 8, 1 and 2
* The user can not select more than one grade at the same time


### R5: The game must have a level selection
The end user must be able to select a level. This way the assignments can be even more specific.

#### Acceptation
* The user can make a selection between levels easy, medium and hard
* The user can not select more than one level at the same time
* There is overlap between the levels and the grades, for instance:
  * Level hard at grade 7 is equal to level easy at grade 8


### R6<!-- Siege-->: The game must have a subject selection
The end user must be able to select a subject. This way the assignments can be made to practice the right topics.

#### Acceptation
* The user can make a selection between subjects maths, language and .. to be decided
* The user can not select more than one subject at the same time
<!--* //???? There are specific games for different subjects-->


### R7: The game must have a assignment selection
The end user must be able to select an assignment. This way the user can select the topic they want to practice.

#### Acceptation
* The user can make a selection between certain parts of a subject
* The user can not select more than one assignment at the same time
* The user sees the same type of questions but with different variables when they open it
* The user sees different physical assignments


### R8: The game must be expandable
The game must be expandable so that next project groups can expand it without rebuilding the whole game.

#### Acceptation
* The SOLID principles will be followed
* The coding standards of Unreal Engine 5 will be used [@unreal2022].
* The documentation will be kept up to date 
* The documentation will explain the code clearly

### R9: Single Generic Minigame
As a minimal viable product/proof of concept the game needs at least one generic minigames that can be utilised by multiple subjects/grades/difficulties. We will implement the following: The player gets a question and three answers and has to throw a ball into the hoop of the correct answer. This increases reusability and can make it so the project team can use a generic minigame if there isn't a good original idea for a specific subject/topic.

#### Acceptation
* Implementation of throwing the ball into a hoop.
  * When thrown in a specific hoop the system will call back which answer was picked
    * If the correct answer is chosen the player will win the minigame
    * If the wrong answer is chosen the player will lose the minigame
* The minigame content has to be generically implemented so it can theoretically be used for every subject or topic.



## Should have

### R10: Unique subject-specific minigames
In order to satisfy the learning part of the game there have to be minigames tailored to the subject and topic. These are currently unknown, as there are no specific requirements for the minigames. This requirement will be updated later when more information is known on the specific minigames.


### R11: There should be more generic minigames
To make the game bigger and more challenging there should be more genereic minigames. The completion of these minigames will be decided when the product owner has talked with the teachers of the schools so there is more clarity. 

## Could have
At this point there are no "could have" requirements yet. Whenever there are some they will be added.


## Won't have

### R12: Supported for multiple headsets
The current build target is the Oculus Quest 2. We won't develop the game with other headsets in mind because the product owner already has bought the Quest 2's and it would take more time to make it compatibile with for example the Index or Rift. 

### R13: Support for data logging
The project will not have logging of scores, and not things related to this such as a scoreboard with highscores. This is in order to keep the scope of the project limited.

### R14: Multiplayer support
The project will not have support for multiplayer. Foremost this would add a layer of complexity that would make the project too ambitious in the timespan of 10 weeks. In addition the product owner want us focus on the individual learning process and wants to avoid the customers from running into each other, possibily hurting themselfs.


# Bibliography {-}
&nbsp;

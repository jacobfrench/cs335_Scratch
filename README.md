# BattlePong
BattlePong is an arena-like combat fighting game in which two players face off against each other in the form of Pong. 

## Members :
-   Jacob
-   Brian
-   Cory
-   Keith

## Functional Requirements

Unlike the origin Pong, the main of objective of BattlePong is to defeat your opponent by any means necessary. This can include...
-   Causing damage to your opponent via your BattleBall
-   Area of Effect damage via PowerUps
-   Time Counter counts down to 0

Your score is based on the criteria of the following
 - Number of times you successfully hit your own BattleBall
 - Number of times successfully dodged your opponents BattleBall

The visual aesthic will pay homage to the old-style terminal computers in the 80s.

After the game loop ends, a backend script will insert the scores into a database. The top 10 scores will then be echo'ed to the terminal session.

## Nonfunctional Requirements
- The target platform is Linux. The program must compile on a Linux machine with no warnings or error
- Testing, unit testing? Continous Integration? 

# TODOS 
 - Agree upon coding style below
 - Assign tasks to functionality and non-functionality
 - Create UML diagram detailing the overall design of the program
 - Implement testing standards

# Coding Style

### Indenting
Tabs, 2 spaces

### Class Definitions
```sh
class MyClass {
 private:
    param1
    param2
 public:
    method1();
};
```


### Function Definitions
```sh
function myFunc( param1, param2 ) {

}
```

### Conditional Statements
```sh
if (true) {
 something();
} 
else {
 somethingElse();
}
```

### Variable Definitions
```sh
int camelCase;
int CONSTANT_VARIABLE;
```



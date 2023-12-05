//libraries needed for code
#include "FEHLCD.h"
#include "FEHUtility.h"
#include <FEHImages.h>
#include <time.h>
#include <FEHSD.h>

//x and y are global variables needed for the reaction test thta track the red circle's location
//count tracks the number of red circles clicked
//keyboard tracks the values touched on the keyboard for the counting test
//mem is the memory score
//count_mem is the total number of images popping up more than once
float x, y, count, keyboard,mem;
float count_mem;

//flashed total and all the flash variables track how many red circles were flash for different parts of the game
float flashed_total, flash1, flash2, flash3;
//the count variables track how many red circles were touched for different parts of the game
float count1, count2, count3;
//toucher tracks if the red circle was touched or not(TRUE: touched, FALSE: not touched)
bool toucher;

//reaction tracks the reaction time of the user when clicking on the circles
float reaction;
//flasher tracks the time when the red circle pops up on the screen
time_t flasher;

//these variables are for the counting part of the game(STAT variables)
float orientation;
float understanding;
float concentration;

//Writing to a file for all the resulting statistics to input into MATLAB
FEHFile *results = SD.FOpen("results.txt","w");
FEHFile *results2 = SD.FOpen("results2.txt","w");



//memory image class has objects which correspond to the variables of certian images, each image has an object that corresponds to it
//the object has a certain number and tools that track how many times the image popped up on the screen
class MemImage{
    public:
        MemImage();
        int getCount();
        int getNum(int l = 0);  
        void makeZero();    
        int IncreaseCount();
    private:
        int num;
        int count;

};

//constructor for the class(Sreekar)
MemImage::MemImage(){
    num = 0;
    count = 0;
}

//gets the count of how many times the image popped up
int MemImage::getCount(){
    return count;
}

//increases the count of popping up by 1
int MemImage::IncreaseCount(){
    count = count + 1;
    return count;

}

//gets the number corresponding for the image
int MemImage::getNum(int l){
    num = l;
    return l;

}

//makes the count 0 
void MemImage::makeZero(){
    count = 0;
}

//REACTION GAME /////////////////////////////////

//function creates a random coordinate value that can be used to randomly populate the circles in the reaction test
int RandomCoord(int lower, int upper){
    int num = (rand() & (upper - lower + 1)) + lower;
    return num;
}


//creates red circle in random location
void CreateCircle(float sec){
    LCD.SetBackgroundColor(BLACK);
    //LCD.Clear();
    x = RandomCoord(0,300);
    y = RandomCoord(0,230);
    LCD.SetFontColor(RED);
    LCD.FillCircle(x,y,10);
    Sleep(sec);
    LCD.ClearBuffer();
    LCD.Clear();
}

//creates blue circles in random location; takes in an input of how many blue circles want to be populated

void CreateBlue(int num){
    for(int i = 0; i < num; i++){
        LCD.SetFontColor(BLUE);
        LCD.FillCircle(RandomCoord(0,300), RandomCoord(0,230),10);
    }

}

//creates moving purple circle that is used to know when the next circle will be flashed in the first level, also could be used for eye tracking 
void CreatePurple(){
    float x, y;

    int x1 = 150, y1 = 220;
    int r = 10;

    // Draw jumper
    LCD.SetFontColor(PURPLE);
    LCD.DrawCircle(x1, y1, r);
    LCD.FillCircle(x1, y1, r);
    LCD.Update();

    // Jump into the air
    while (y1 > 50) {
        // Clear previous jumper
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(x1, y1, r);
        LCD.Update();

        // Change y coordinate
        y1 -= 10;

        // Draw new jumper
        LCD.SetFontColor(PURPLE);
        LCD.DrawCircle(x1, y1, r);
        LCD.FillCircle(x1, y1, r);

        Sleep(.1);
    }
    
    // Fall back down
    while (y1 > 220) {
        // Clear previous jumper
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(x1, y1, r);
        LCD.Update();

        // Change y coordinate
        y1 += 10;

        // Draw new jumper
        LCD.SetFontColor(PURPLE);
        LCD.DrawCircle(x1, y1, r);
        LCD.FillCircle(x1, y1, r);

        Sleep(.1);
    }

}






//flashes circle for a certain amount of time
void FlashCircle(float sec){
    CreateCircle(sec);
    flasher = time(NULL);
}


//touch checker checks if user touched the red circle
void TouchChecker(){
    float x_pos, y_pos;
    float x_t, y_t;
    time_t start;
    start = time(NULL);
    while(time(NULL) - start < 1.1){
        if(LCD.Touch(&x_pos,&y_pos)){
            if((x_pos > (x-20)) && (x_pos < (x+20)) && (y_pos > (y-20)) && (y_pos < (y+20))){
                count = count + 1;
                LCD.WriteLine(count);
                toucher = true;
                reaction = reaction + (flasher - time(NULL));
                break;
            }
        }
    }

}


//checktouch checks if screen in memory part of game is touched 
bool CheckTouch(){
    float x_pos, y_pos;
    float x_t, y_t;
    time_t start;
    start = time(NULL);
    while(time(NULL) - start < 1.1){
        if(LCD.Touch(&x_pos,&y_pos)){
            if((x_pos > 0) && (x_pos < 319) && (y_pos > 0) && (y_pos < 300)){
                return true;
            }
        }
    }

}

//reaction game - flashes red and blue circles - 3 levels
void Flash(){
    time_t start;
    start = time(NULL);
    CreatePurple();
    while(time(NULL) - start < 20){
        Sleep(1.0);
        CreateBlue(2);
        FlashCircle(1.0);
        flashed_total = flashed_total + 1;
        flash1 = flash1 + 1;
        TouchChecker();
        if(toucher == true){
            count1+=1;
        }
        CreatePurple();
        Sleep(1.0);
        LCD.Clear();
    }
    start = time(NULL);
    while(time(NULL) - start < 20){
        Sleep(1.0);
        CreateBlue(4);
        FlashCircle(0.75);
        flashed_total = flashed_total + 1;
        flash2 = flash2 + 1;
        TouchChecker();
        if(toucher == true){
            count2+=1;
        }
        Sleep(1.0);
        LCD.Clear();
    }
    start = time(NULL);
    while(time(NULL) - start < 20){
        Sleep(1.0);
        CreateBlue(8);
        FlashCircle(0.5);
        flashed_total = flashed_total + 1;
        flash3 = flash3 + 1;
        TouchChecker();
        if(toucher == true){
            count3+=1;
        }
        Sleep(1.0);
        LCD.Clear();
    }
    
}

//COUNTING GAME ///////////////////////////////////////

//Draws a keyboard that can be used by the user to input answers
void KeyBoard(){
    
    //draw outline
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(260,0,59,239);
    LCD.DrawVerticalLine(289.5,0,191.4);
    LCD.DrawHorizontalLine(47.9,260,318);
    LCD.DrawHorizontalLine(95.6,260,318);
    LCD.DrawHorizontalLine(143.5,260,318);
    LCD.DrawHorizontalLine(191.4,260,318);

    //draw numbers
    LCD.WriteRC("1",1,22);
    LCD.WriteRC("2",1,25);
    LCD.WriteRC("3",4,22);
    LCD.WriteRC("4",4,25);
    LCD.WriteRC("5",7,22);
    LCD.WriteRC("6",7,25);
    LCD.WriteRC("7",10,22);
    LCD.WriteRC("8",10,25);
    LCD.WriteRC("9",12,24);


}

//user can touch keyboard to input answer
void KeyTouch(){
    float x_pos, y_pos;
    float x_t, y_t;

    LCD.ClearBuffer();
        
    while(!LCD.Touch(&x_pos,&y_pos)) {};
       
    while(LCD.Touch(&x_t,&y_t)) {};
   
    if((x_pos > 260) && (x_pos < 289.5) && (y_pos > 0) && (y_pos <47.9)){
        keyboard = 1;
    }

    if((x_pos > 289.5) && (y_pos > 0) && (y_pos <47.9)){
        keyboard = 2;

    }

    if((x_pos > 260) && (x_pos < 289.5) && (y_pos > 47.9) && (y_pos <95.6)){
        keyboard = 3;

    }

    if((x_pos > 289.5) && (y_pos > 47.9) && (y_pos <95.6)){
        keyboard = 4;

    }

    if((x_pos > 260) && (x_pos < 289.5) && (y_pos > 95.6) && (y_pos <143.5)){
        keyboard = 5;
    }

    if((x_pos > 289.5) && (y_pos > 95.6) && (y_pos <143.5)){
        keyboard = 6;

    }

    if((x_pos > 260) && (x_pos < 289.5) && (y_pos > 143.5) && (y_pos <191.4)){
        keyboard = 7;

    }

    if((x_pos > 289.5) && (y_pos > 143.5) && (y_pos <191.4)){
        keyboard = 8;
    }

    if((x_pos > 260) && (y_pos > 191.4)){
        keyboard = 9;
    }



}

//question 1 of counting test
void Question1(){
    LCD.Clear();
    KeyBoard();
    LCD.WriteLine("What number month are we in? ");
    KeyTouch();
    if(keyboard == 1){
        KeyTouch();
        if(keyboard == 2){
            LCD.WriteLine("Correct");
            Sleep(1.0);
            orientation +=2;
       }
       else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
       }
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
    }
    keyboard = 0;


}

//question 2 of counting test
void Question2(){
    LCD.Clear();
    KeyBoard();
    LCD.WriteLine("How many purple circles do you see?");
    LCD.SetFontColor(PURPLE);
    LCD.FillCircle(160,120,10);
    LCD.FillCircle(160,140,10);
    LCD.SetFontColor(ORANGE);
    LCD.FillCircle(140,140,10);
    KeyTouch();
    if(keyboard == 2){
        LCD.WriteLine("Correct");
        orientation+=1;
        Sleep(1.0);
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(160,120,10);
        LCD.FillCircle(160,140,10);
        LCD.FillCircle(140,140,10);
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(160,120,10);
        LCD.FillCircle(160,140,10);
        LCD.FillCircle(140,140,10);
    }
    LCD.Clear();
    keyboard = 0;

}

//question 3 of counting test
void Question3(){
    LCD.Clear();
    KeyBoard();
    FEHImage unicorn;
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("How many unicorns do you see?");
    unicorn.Open("UnicornFEH.pic");
    unicorn.Draw(160,120);
    unicorn.Draw(110,70);
    KeyTouch();
    if(keyboard == 2){
        LCD.WriteLine("Correct");
        orientation+=1;
        Sleep(1.0);

    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
        
    }
    keyboard = 0;

}

//question 4 of counting test
void Question4(){
    LCD.Clear();
    KeyBoard();
    LCD.WriteLine("How many yellow pixels do you see?");
    LCD.SetFontColor(YELLOW);
    LCD.DrawPixel(160,120);
    LCD.DrawPixel(160,100);
    LCD.DrawPixel(150,110);
    KeyTouch();
    if(keyboard == 3){
        LCD.WriteLine("Correct");
        orientation+=1;
        Sleep(1.0);
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(160,120,10);
        LCD.FillCircle(160,140,10);
        LCD.FillCircle(140,140,10);
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
        LCD.SetFontColor(BLACK);
        LCD.FillCircle(160,120,10);
        LCD.FillCircle(160,140,10);
        LCD.FillCircle(140,140,10);
    }
    keyboard = 0;

}

//question 5 of counting test
void Question5(){
    LCD.Clear();
    KeyBoard();
    FEHImage shape1;
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("How many sides does the shape have?");
    shape1.Open("shapeFEH.pic");
    shape1.Draw(160,120);
    KeyTouch();
    if(keyboard == 6){
        LCD.WriteLine("Correct");
        understanding+=1;
        Sleep(1.0);

    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
        
    }
    keyboard = 0;

}

//question 6 of counting test
void Question6(){
    LCD.Clear();
    KeyBoard();
    FEHImage shape2;
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("How many sides does the shape have?");
    shape2.Open("shape2FEH.pic");
    shape2.Draw(160,120);
    KeyTouch();
    if(keyboard == 8){
        LCD.WriteLine("Correct");
        understanding+=1;
        Sleep(1.0);

    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
        
    }
    keyboard = 0;

}
//question 7 of counting test
void Question7(){
    LCD.Clear();
    KeyBoard();
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Remember this number: ");
    LCD.WriteLine("4-9-3");
    LCD.WriteLine("Enter it in backwards. ");
    Sleep(3.0);
    LCD.Clear();
    KeyBoard();
    KeyTouch();
    if(keyboard == 3){
        KeyTouch();
        if(keyboard == 9){
            KeyTouch();
            if(keyboard == 4){
                LCD.WriteLine("Correct");
                concentration+=1;
                Sleep(1.0);
            }
            else{
                LCD.WriteLine("Incorrect");
                Sleep(1.0);
            }
       }
       else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
       }
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
    }
    keyboard = 0;



}

//question 8 of counting test
void Question8(){
    LCD.Clear();
    KeyBoard();
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Remember this number: ");
    LCD.WriteLine("3-2-7-9");
    LCD.WriteLine("Enter it in backwards. ");
    Sleep(4.0);
    LCD.Clear();
    KeyBoard();
    KeyTouch();
    if(keyboard == 9){
        KeyTouch();
        if(keyboard == 7){
            KeyTouch();
            if(keyboard == 2){
                KeyTouch();
                if(keyboard == 3){
                    LCD.WriteLine("Correct");
                    concentration+=1;
                    Sleep(1.0);
                }
                else{
                    LCD.WriteLine("Incorrect");
                    Sleep(1.0);
                }
            }
            else{
                LCD.WriteLine("Incorrect");
                Sleep(1.0);
            }
       }
       else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
       }
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
    }
    keyboard = 0;



}

//question 9 of counting test
void Question9(){
    LCD.Clear();
    KeyBoard();
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Remember this number: ");
    LCD.WriteLine("6-2-9-7-1");
    LCD.WriteLine("Enter it in backwards. ");
    Sleep(4.0);
    LCD.Clear();
    KeyBoard();
    KeyTouch();
    if(keyboard == 1){
        KeyTouch();
        if(keyboard == 7){
            KeyTouch();
            if(keyboard == 9){
                KeyTouch();
                if(keyboard == 2){
                    KeyTouch();
                    if(keyboard == 6){
                        LCD.WriteLine("Correct");
                        concentration+=1;
                        Sleep(1.0);   
                    }
                    else{
                        LCD.WriteLine("Incorrect");
                        Sleep(1.0);
                    }
                }
                else{
                    LCD.WriteLine("Incorrect");
                    Sleep(1.0);
                }
            }
            else{
                LCD.WriteLine("Incorrect");
                Sleep(1.0);
            }
       }
       else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
       }
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
    }
    keyboard = 0;



}

//question 10 of counting test
void Question10(){
    LCD.Clear();
    KeyBoard();
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Remember this number: ");
    LCD.WriteLine("7-1-8-4-6-2");
    LCD.WriteLine("Enter it in backwards. ");
    Sleep(5.0);
    LCD.Clear();
    KeyBoard();
    KeyTouch();
    if(keyboard == 2){
        KeyTouch();
        if(keyboard == 6){
            KeyTouch();
            if(keyboard == 4){
                KeyTouch();
                if(keyboard == 8){
                    KeyTouch();
                    if(keyboard == 1){
                        KeyTouch();
                        if(keyboard == 7){
                            LCD.WriteLine("Correct");
                            concentration+=1;
                            Sleep(1.0);
                        }   
                        else{
                            LCD.WriteLine("Incorrect");
                            Sleep(1.0);
                        }
                    }
                    else{
                        LCD.WriteLine("Incorrect");
                        Sleep(1.0);
                    }
                }
                else{
                    LCD.WriteLine("Incorrect");
                    Sleep(1.0);
                }
            }
            else{
                LCD.WriteLine("Incorrect");
                Sleep(1.0);
            }
       }
       else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
       }
    }
    else{
        LCD.WriteLine("Incorrect");
        Sleep(1.0);
    }
    keyboard = 0;



}










//function creates counting game and runs each question, also collecting stats
void Counting(){
    time_t start;
    start = time(NULL);
    Question1();
    Question2();
    Question3();
    Question4();
    Question5();
    Question6();
    Question7();
    Question8();
    Question9();
    Question10();

    orientation=orientation/5.0;
    understanding=understanding/2.0;
    concentration=concentration/4.0;


}




///MEMORY GAME/////////////////////////////////////
    

//function initializes images that will pop up on screen 
//each image has an object that corresponds to it that has its count and number
//when an image pops up on the screen, the object's count is increased and checked to see if it has popped up on the screen more than once
int DrawPicRandom(){
    time_t start;
    float x_position, y_position;

    int b1,c1,e1,ca1,p1,r1,s1,sl1,sn1,t1, r, r3;
    int b2,c2,e2,ca2,p2,r2,s2,sl2,sn2,t2;


    FEHImage bell;
    FEHImage choco1;
    FEHImage elf;
    FEHImage candy;
    FEHImage presents;
    FEHImage reindeer;
    FEHImage santa;
    FEHImage sleigh;
    FEHImage snowflake;
    FEHImage tree;

    bell.Open("bellFEH.pic");
    choco1.Open("chocoFEH.pic");
    elf.Open("elfFEH.pic");
    candy.Open("candyFEH.pic");
    presents.Open("presentsFEH.pic");
    reindeer.Open("reindeerFEH.pic");
    santa.Open("santaFEH.pic");
    sleigh.Open("sleighFEH.pic");
    snowflake.Open("snowflakeFEH.pic");
    tree.Open("treeFEH.pic");

    MemImage bell0;
    MemImage choco0;
    MemImage elf0;
    MemImage candy0;
    MemImage presents0;
    MemImage reindeer0;
    MemImage santa0;
    MemImage sleigh0;
    MemImage snowflake0;
    MemImage tree0;

    b1 = bell0.getNum(1);
    c1 = choco0.getNum(2);
    e1 = elf0.getNum(3);
    ca1 = candy0.getNum(4);
    p1 = presents0.getNum(5);
    r1 = reindeer0.getNum(6);
    s1 = santa0.getNum(7);
    sl1 = sleigh0.getNum(8);
    sn1 = snowflake0.getNum(9);
    t1 = tree0.getNum(10);

    
    

    start = time(NULL);

    while(time(NULL) - start < 60){
        r = RandomCoord(4,10);
        while(r == r3){
            r = RandomCoord(1,10);
        }
        if(r == 1){
            bell.Draw(30,30);
            bell0.IncreaseCount();
            if(bell0.getCount() > 1){
                if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                    }
                }
                else{
                     count_mem+=1;
                }
            }

        if(r == 2){
            choco1.Draw(0,0);
            choco0.IncreaseCount();
            if(choco0.getCount() > 1){
                if(CheckTouch()){ 
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 3){
            elf.Draw(0,0);
            elf0.IncreaseCount();
            if(elf0.getCount() > 1){
                if(CheckTouch()){              
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 4){
            candy.Draw(0,0);
            candy0.IncreaseCount();
            if(candy0.getCount() > 1){
                if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 5){
            presents.Draw(0,0);
            presents0.IncreaseCount();
            if(presents0.getCount() > 1){
                if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1; 
                }
            }
        }
        if(r == 6){
            reindeer.Draw(0,0);
            reindeer0.IncreaseCount();
            if(reindeer0.getCount() > 1){
               if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 7){
            santa.Draw(0,0);
            santa0.IncreaseCount();
            if(santa0.getCount() > 1){
                if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 8){
            sleigh.Draw(0,0);
            sleigh0.IncreaseCount();
            if(sleigh0.getCount() > 1){
                if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 9){
            snowflake.Draw(0,0);
            snowflake0.IncreaseCount();
            if(snowflake0.getCount() > 1){
               if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }
        if(r == 10){
            tree.Draw(0,0);
            tree0.IncreaseCount();
            if(tree0.getCount() > 1){
               if(CheckTouch()){
                    mem = mem + 1;
                    count_mem+=1;
                }
                else{
                    count_mem+=1;
                }
            }
        }

        Sleep(3.0);
        LCD.Clear();

        r3 = r;
        LCD.WriteAt(mem,170,200);

    }


}


// MENU ///////////////////////////////////////

//creates credits section of menu
void Credits(){
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.WriteLine("Developers:");
    LCD.WriteLine(" ");
    LCD.WriteLine(" ");
    LCD.WriteLine("1. Sreekar Kutagulla");
    LCD.WriteLine("2. Ryan Gick");
}

//start screen of the reaction game
void ReactionGameStart(){
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(RED);
    LCD.DrawCircle(160,120,10);

}

//stats section of menu
void Stats(){
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.WriteLine("Hit/Total: ");
    LCD.WriteLine(count);
    LCD.WriteLine("Average Reaction Time(sec): ");
    LCD.WriteLine(reaction);
    LCD.WriteLine("Orientation %: ");
    LCD.WriteLine(orientation*100);
    LCD.WriteLine("Understanding%: ");
    LCD.WriteLine(understanding*100);
    LCD.WriteLine("Memory Score: ");
    LCD.WriteLine(mem/count_mem);

}

//instructions section of game
void Instructions(){
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.WriteLine("1. Reaction Game: Click only the red circles as soon as possible after flashing.");
    LCD.WriteLine("2. Counting Game: Answer Questions that pop on the screen.");
    LCD.WriteLine("3. Memory Game: Click anywhere on the screen if you see an image pop up more than once.");

}

//Creates menu outline
void Menu() {


    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.DrawVerticalLine(160,0,319);
    LCD.DrawHorizontalLine(120,0,319);

    LCD.WriteRC("PLAY",3,4);
    LCD.WriteRC("STATS",3,18);
    LCD.WriteAt("INSTRUCTIONS",6,187);
    LCD.WriteRC("CREDITS",11,17);


}

//if user wants to return to menu
void MenuReturn(){
    float x_pos, y_pos;
    float x_t, y_t;
    LCD.SetFontColor(GREEN);
    LCD.FillRectangle(299,0,20,10);
    while(true)
    {
    
        LCD.ClearBuffer();
        
        while(!LCD.Touch(&x_pos,&y_pos)) {};
       
        while(LCD.Touch(&x_t,&y_t)) {};
 
        if((x_pos > 299) && (y_pos > 0) && (y_pos < 10)){
            Credits();
            LCD.ClearBuffer();
            Menu();
            break;
        }
    }



}

//MAIN FUNCTION////////////////////////////////////////////////////////////////////////


int main() {
    char pl[5000];
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    float x_position, y_position;
    float x_trash, y_trash;

    //LCD.WriteRC("Hello",7,8);
    Menu();
    while(true)
    {
        LCD.ClearBuffer();
        while(!LCD.Touch(&x_position,&y_position)) {};
        while(LCD.Touch(&x_trash,&y_trash)) {};

        if((x_position > 160) && (y_position > 111) && (y_position < 239)){
            Credits();
            LCD.ClearBuffer();
            MenuReturn();
        }
        else if((x_position < 160) && (y_position < 111) && (y_position > 0)){
            ReactionGameStart();
            LCD.Clear();
            Flash();
            LCD.Clear();
            LCD.WriteLine("TIME FOR COUNTING TEST!");
            Sleep(4.0);
            Counting();
            LCD.Clear();
            LCD.WriteLine("TIME FOR MEMORY TEST!");
            Sleep(4.0);
            DrawPicRandom();
            LCD.ClearBuffer();
            MenuReturn();
            reaction = reaction/count;
            count = count/flashed_total;
            count1 = count1/flash1;
            count2 = count2/flash2;
            count3 = count2/flash3;
            SD.FPrintf(results, "Hit/Total: %.4f\nH/T for level 1: %.4f\nH/T for level 2: %.4f\nH/T for level 3: %.4f\n Average Reaction Time: %.4f\n",count,count1,count2,count3,reaction);
            SD.FPrintf(results2, "%.4f\n%.4f\n%.4f\n%.4f\n%.4f\n",count,count1,count2,count3,reaction);
            SD.FPrintf(results,"Orientation: %0.4f \nUnderstanding: %0.4f\nConcentration: %0.4f\n",orientation,understanding,concentration);
            SD.FPrintf(results2,"%0.4f \n%0.4f\n %0.4f\n",orientation,understanding,concentration);
            SD.FPrintf(results,"Memory Score: %0.1f\n",mem/count_mem);
            SD.FPrintf(results2,"%0.1f\n",mem/count_mem);
            SD.FClose(results);
            SD.FClose(results2);
        }
        else if((x_position > 160) && (y_position < 111) && (y_position > 0)){
            Stats();
            LCD.ClearBuffer();
            MenuReturn();
        }
        else if((x_position < 160) && (y_position > 111) && (y_position < 239)){
            Instructions();
            LCD.ClearBuffer();
            MenuReturn();
        }
}

    while (1) {
        LCD.Update();
        // Never end
    }
    return 0;
}



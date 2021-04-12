# Basic-interpreter

   Author: Kendrick Zhou
   ID: 519021911217
   SJTU/SE2019/DS/Proj1

## Intro
   This is a simple Basic-interpreter. You can run your Basic code by this!

## Manual
1. Browers
   * Code: Contains your code.
   * Result: Contains run results, such as the output in PRINT expression.
   * LAST: **Language And Syntax Trees**. Your code is parsed into a syntax tree here.
   * Input: Input a **Expression ** or **Command ** here, and press **Enter ** when you finish inputing.
      Your code should be typed in one of the following ways:
         * **LineNumber + Statement**:
      
           ``10 LET a = 10``
      
        * **Command**
      
          You can get details about command in Part 3.
      
        * **Statement**
      
          Specifically, you can input a statement without linenumber. But it only supports **{LET, INPUT,PRINT}**.
      
          It may help you to debug more easily.
2. Button
   * LOAD: You can load a file to be executed through LOAD button.
   
     ​			**Attention**: The code in file **MUST** be in the form of **LineNumber + Statement**.
   
   * RUN: Starts program execution beginning at the lowest-numbered line, Execution ends                        when the program hits the END statement or continues past the last statement in                        the program. By the way, **The syntax tree** will be displayed.
   
   * CLEAR: Clears the content of all three windows, and your **Context** will also be cleared.
   
     **Attention**: You can **ONLY** clear the Context by clicking **CLEAR** button.
   
3. Command
   * LOAD: Just equal to press LOAD button.
   * RUN: Just equal to press RUN button.
   * CLEAR: Just equal to press CLEAR button.
   * HELP: Get help information.
   * QUIT: Exit the interpreter.

4. Error

   When you RUN a wrong program, the interpreter will behave as follows:

   * The interpreter will stop on the **First Error Line**.

   * An Error message window will pop up, you can get some **Error Information** by reading this.

   * The interpreter will still try to build the syntax tree. If the interpreter encouters an instruction with a syntax error, it will print (LineNumber + Error).

     > 10  Error

   * **Logical error** do not affect  the construction of the syntax tree.

-----------------------------

Get more details by reading [Basic-doc.pdf](./Basic-doc.pdf)

Hope you can get started really soon!

​																																			**Kendrick**

​																																		    **2021/4/11**
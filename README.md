# Project: Multi-Process Competition

### Description

This project involves creating a multi-processing application on Linux that utilizes signals, pipes, and FIFOs. The objective is to simulate a competition between two teams, each consisting of two partner processes, and a co-processor to assist with calculations. Here's how the competition works:

### Key Components and Process Flow

1. **Process Initialization:**
   - The parent process starts by forking five child processes.
   - The first two processes belong to Team 1, and the next two processes belong to Team 2.
   - The fifth process is a co-processor that assists the parent with calculations.

2. **Range Specification:**
   - The parent process writes two integer values (min and max) into a file named "range.txt." These values define a numeric range.

3. **Generating Values:**
   - Upon instruction, the first four processes generate four floating-point values within the specified range. Each process stores its value in a file named after its process ID (PID).
   - Each process notifies the parent when its value is ready for retrieval.

4. **Value Transmission:**
   - The parent gathers the four generated values and sends them as a message (e.g., comma-separated) through a pipe to the co-processor (fifth child process).

5. **Co-Processor Calculations:**
   - The co-processor receives the four values and performs calculations:
     - It sums the first two values to create "Sum1."
     - It sums the third and fourth values to create "Sum2."
   - The co-processor returns these two sums to the parent as a message (e.g., comma-separated).

6. **Determining the Winner:**
   - The parent process compares "Sum1" and "Sum2" to determine which team has the higher sum.

7. **Multiple Rounds:**
   - The application repeats the steps 2 to 6 as many times as instructed by the user. If no argument is provided, it assumes five rounds.

8. **Termination:**
   - Upon completion, the parent declares the winner of the competition, terminates all child processes, releases allocated resources, and exits.

This project simulates a competitive scenario using multi-processing techniques. It involves generating values, performing calculations, and determining a winner based on the sums obtained. The competition can run for multiple rounds, and upon completion, the application announces the victorious team before gracefully terminating all processes and releasing resources.

Mailbox Messaging Program
This program implements a thread-based message passing mechanism in C/C++ using semaphores and pthreads. Each thread is assigned a mailbox where messages can be sent and received. The program allows threads to communicate by sending and receiving messages to and from each other’s mailboxes, with proper synchronization using semaphores.

To compile the program, run the command:
gcc -pthread mailbox.c -o mailbox

To run the program, use the command:
./mailbox [number_of_threads]

Example:
./mailbox 3

Features
Message Passing: Threads can send and receive messages through individual mailboxes, blocking when necessary to avoid conflicts.
Synchronization: Uses semaphores to manage access to mailboxes and ensure message delivery.
Thread Management: Supports multiple threads (up to 10), with each thread summing values received in messages.
Non-blocking Send (NBSendMsg): An additional feature that allows messages to be sent without blocking, returning a code if the mailbox is full.
Queue of Undelivered Messages: Maintains a queue of undelivered requests when using the non-blocking send feature.
How to Run the Non-blocking Version
To run the non-blocking version of the program, use:
./mailbox [number_of_threads] nb


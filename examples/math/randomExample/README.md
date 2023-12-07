#level1_randomExample
--
### Learning Objectives

This openFrameworks Example is designed to demonstrate the OF random Engine and it's seeding. 

During the ofBaseApp initializations (i.e. before the setup() call), OF instanciates a thread-safe random engine based on Mersenne-Twister, with a non-deterministic random seed sequence. This provides a centralized source of unpredictable randomness (which is generally what is expected by a random function).

However if desired, you can supply a determinist seed. This makes it possible to generate deterministic works (or test things with repeatable random).

This example demonstrates the Engine coupled with the ofShuffle function.

### Expected Behavior

When launching this app, you should see:

* numbers 1-8, in an unpredictably shuffled order.

### Instructions for use:

* Drag the seed value to activate determinism and notice how the sequence is shuffled differently. For a given seed, this random order will be the same, on all C++17 platforms (until the Mersenne-Twister implementation or std::uniform_int_distributions are modified or deprecated).

* Click reset to destroy and re-instanciate the Engine, with a fresh random seed sequence.

### Other classes used in this file

This Example uses ofxGui. 

# ECE3LineFollower
Line following TI RSLK MAX robot for ECE3 Fall 2020.
David Hill and Nevin Liang

Save files to your tirslk_max_1_00_00 folder.

# FINAL PROJECT REPORT #

1. Introduction and Background

TODO: path sensing circuitry

In order to calibrate our car, we placed it underneath our fixed lighting condition on blank white paper and measured the IR sensor outputs to Serial output. We looked at the many sets of 8 numbers appearing over time, and took the minimums of each sensor. Doing this, we were able to calculate the offset we needed for each of the 8 numbers. Subtracting the values from the offset, we could get a good range of values for different situations the car was in. Since the values were  now anywhere from 0 to 2500-offset, we decided to scale this range to 0-1000 so that we only would have to change the offset array during different times of day, since all the future calculations would be based on a range from 0-1000 instead of 0 to a variable number.

In order to fuse the data from all 8 sensors into 1 number we could manipulate the motors with, we applied weights to each of the 8 sensors, with larger values towards the ends of the array of sensors, so that a bigger feedback would happen from a larger distance off course. We chose to use the array {8, 4, 2, 1, -1, -2, -4, -8} because each successive multiple of 2 away from the origin would be twice as dramatic of feedback. Negative values would imply that it is to the right and vice versa for to the left. This way, given any single number (we called err), we would be able to determine where the car was relative to the line on the track. After this, we needed one more calibration step, where we calculated what the value of 'err' was when the car was at the center of the track. This way, we new what meant to much to the left and what meant too much to the right.

TODO: circuit schematics are required

2.

My test setup was on a flat surface in our dining room because it was one of the few places in our house where the surface was completely flat. Any angle or bump would be super detrimental to the car's path because the light reflecting off the white surface would be different at different points along the path. Initially, I tried to run it in my living room, but it was during the day and the lighting from outside changed so much that I was better off running with all the windows closed and an artificial light source.

The main tests I had to do were the following:
    - figuring out the offsets
    - figuring out the delays
        - for rotating 180
        - for beginning the return path
    - figuring out left/right epsilon values to determine what was deemed too far off course to require correction
    - figuring out the ratio the error had on the change in speed for the motor

Only the last one was dependent on light source, so I will cover the other 2 tests first. The offsets were calculated as written above in the description. The delays were calculated with a simple mental binary search, adding a few and subtracting a few until it worked. Once we had the delay and motor speed for the 180 degree rotation, we multiplied the motor speed by 2 and halved the delay to make the rotation faster. We had the car scoot a little forwards before implementing the course correction software we wrote, and just put 200 milliseconds so that it wouldn't be too far off course, but enough to escape the black T.

Logging for offsets: (a lot because I was being dumb and decided to move around the house)

Data for Sensors 0-7:
629, 800, 668, 524, 570, 710, 804, 751
640, 722, 662, 521, 585, 723, 728, 752
686, 700, 661, 526, 582, 701, 709, 730
670, 640, 680, 530, 547, 742, 642, 760
610, 590, 640, 541, 558, 732, 590, 758
650, 660, 663, 514, 592, 715, 664, 762
645, 700, 675, 528, 541, 761, 703, 748
640, 710, 664, 528, 595, 710, 710, 757

Logging for Rotational 180 Delay:
times: 2000, 1000, 1750, 1250, 1100, 1150, 1125, 1130
speed: 40

Logging for the time required to return before using path correction algorithm:
200, 300

Left/Right Epsilon Values:
lEps: 100, 200, 300, 400, 300
rEps: -100, -200, -300, -400, -300

Delta Ratio (essentially a Kp, but written in my own terms):
deltaOuter: 10, 20, 30, 40, 30
deltaInner: 5, 10, 20, 10

3. For the data in table 1: sensor data, it boiled down to the environment and position of the car along the track's white areas when calculating. Repeated trials kept differing even after I set on using one specific location in the house, and one specific place on the page. However, this had to work for all places on the page, so I did multiple trials (the last 5 were on different parts of the track). from there, I took the minimum because it was better for my offsets to be positive than negative. This was probably what took the most work to do, because each time I ran was during a different time of the day and my partner had to do this step as well when he tested out my code.

The logging for the rotational 180 delay was much easier, as this worked no matter the condition of the surroundings. I just had to time how long it took to rotate a full 180 at a given slow speed. Then, I multiplied the speed by 2 and divided the time by 2 to make my overall time faster. For the path correction algorithm, it was mostly just a guess and check, as I wanted the car to inch forward enough that it was away from the black T, but not so far that it would jerk in a direction towards the track because it was too far off. The left/right epsilon values were very very easy to figure out, because I knew that the err value of 50 was dead center on the track, so all I had to try were intervals away from 50 until my car ran smoothly. 300 worked very well for this.

Lastly, the delta ratio was separate from the lighting conditions relative to surroundings, but was also extremely difficult to figure out. I ended trying all possible combinations, and figured out that a slower outer wheel increase in speed with a larger decrease in speed for the inner wheel was much better because this was faster rotation rather than faster movement. As long as the curve stuck to the inner bank, it would progress with less jerk and a more straightforward path. I ended with a 1/10 factor of the error for decrease in speed for the inner wheel, and a 1/30 factor of the error for increase in speed of the outer wheel. This did not have to be changed for different lighting conditions.

TODO: Discuss how well your vehicle performed on Race Day. Provide links to videos, if any.

The limitations of my code were that it varied wildly depending on the lighting conditions, because the offsets had to be evaluated. Another limitation was that it wasn't able to go too fast because the corrections were in real time and didn't have forward calculating that would run independent of sensor data.

I would conduct the project differently by setting up my environment perfectly if I were to do this again. The environment, I realized halfway through my project, was the most important factor that led to incorrect calculations and errors I had to fix by running all my trials all over again.
